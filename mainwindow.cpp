#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tiles.h"
#include "powerdialog.h"
#include "playerinfodialog.h"
#include "resultsgraphdialog.h"
#include "shopdialog.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <algorithm>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMap>

/**
 * @brief Constructor for MainWindow. Initializes the UI and game board.
 * @param parent Pointer to parent QWidget.
 */
MainWindow::MainWindow(Player* p1, Player* p2, int numGames, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player1 = p1;
    player2 = p2;

    isAI = (player2->getName() == "CPU");
    allAI = (player1->getName() == "CPU" && player2->getName() == "CPU");
    multiGameMode = allAI && numGames > 1;
    totalGames = numGames;
    gamesPlayed = 0;

    player1->setOpponent(player2);
    player2->setOpponent(player1);

    currentPlayer = player1; // Game starts with player 1, duh

    setupBoard();
    updatePlayerPanels();
    updatePlayerUI();

    if (multiGameMode) {
        QTimer::singleShot(500, this, &MainWindow::doAITurn);
    }
}

/**
 * @brief Destructor for MainWindow. Cleans up UI.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Initializes the board tiles, sets up special tiles, and configures player paths and visuals.
 */
void MainWindow::setupBoard() {
    QGraphicsScene *scene = new QGraphicsScene(this);
    // Attach scene to graphics view
    ui->graphicsView->setScene(scene);

    // Setup board tile grid
    boardGrid.resize(rows);
    for (int i = 0; i < rows; i++) {
        boardGrid[i].resize(cols);
    }
    // Fill all tiles initially with EmptyTile
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            boardGrid[row][col] = new EmptyTile();
        }
    }

    boardGrid[0][12] = new MoveBackwardTile();
    boardGrid[2][3] = new MoveBackwardTile();
    boardGrid[4][0] = new MoveBackwardTile();
    boardGrid[6][4] = new MoveBackwardTile();
    boardGrid[8][3] = new MoveBackwardTile();
    boardGrid[10][6] = new MoveBackwardTile();
    boardGrid[2][8] = new MoveBackwardTile();
    boardGrid[6][6] = new MoveBackwardTile();
    boardGrid[10][10] = new MoveBackwardTile();

    boardGrid[0][2] = new MoveForwardTile();
    boardGrid[2][6] = new MoveForwardTile();
    boardGrid[4][4] = new MoveForwardTile();
    boardGrid[6][1] = new MoveForwardTile();
    boardGrid[0][6] = new MoveForwardTile();
    boardGrid[4][6] = new MoveForwardTile();
    boardGrid[8][11] = new MoveForwardTile();

    boardGrid[0][8] = new MoneyTile();
    boardGrid[2][0] = new MoneyTile();
    boardGrid[4][3] = new MoneyTile();
    boardGrid[6][2] = new MoneyTile();
    boardGrid[6][10] = new MoneyTile();
    boardGrid[8][5] = new MoneyTile();
    boardGrid[10][8] = new MoneyTile();

    boardGrid[2][10] = new LifeEventTile();
    boardGrid[4][8] = new LifeEventTile();
    boardGrid[6][12] = new LifeEventTile();
    boardGrid[8][0] = new LifeEventTile();
    boardGrid[10][4] = new LifeEventTile();
    boardGrid[2][12] = new LifeEventTile();
    boardGrid[6][0] = new LifeEventTile();
    boardGrid[10][9] = new LifeEventTile();

    boardGrid[0][4] = new PowerupTile();
    boardGrid[2][1] = new PowerupTile();
    boardGrid[4][10] = new PowerupTile();
    boardGrid[6][7] = new PowerupTile();
    boardGrid[8][9] = new PowerupTile();

    boardGrid[4][2] = new SuperMoneyTile();
    boardGrid[10][5] = new SuperMoneyTile();

    // Tile visuals
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            QGraphicsRectItem *tile = scene->addRect(col * tileSize, row * tileSize, tileSize, tileSize);

            // Special handling for white tiles in odd rows (off-path tiles)
            if (row % 2 == 1 && boardGrid[row][col]->getColor() == Qt::white) {
                bool keepTile = false;

                // Only keep the drop-down tile at each odd row
                if ((row - 1) % 4 == 0 && col == cols - 1)  // Coming from left to right, drop at right
                    keepTile = true;
                else if ((row - 1) % 4 == 2 && col == 0)    // Coming from right to left, drop at left
                    keepTile = true;

                if (keepTile)
                    tile->setBrush(boardGrid[row][col]->getColor());
                else
                    tile->setBrush(QColor("#aaaaaa")); // Mark unplayable
            }
            else {
                tile->setBrush(boardGrid[row][col]->getColor());
            }

            tile->setPen(QPen(QColor("#222222")));
        }
    }
    // Populate playable tile coordinates that follows the pattern (right down left down... etc)
    bool goingRight = true;
    int currentRow = 0;

    while (currentRow < rows) {
        if (goingRight) {
            for (int col = 0; col < cols; ++col)
                playableTileCoords.append({currentRow, col});
        } else {
            for (int col = cols - 1; col >= 0; --col)
                playableTileCoords.append({currentRow, col});
        }

        int dropCol = goingRight ? cols - 1 : 0;
        if (currentRow + 1 < rows)
            playableTileCoords.append({currentRow + 1, dropCol});

        currentRow += 2;
        goingRight = !goingRight;
    }
    // Initialize player pieces Red/Blue, change if needed
    player1Piece = scene->addEllipse(0, 0, tileSize * 0.6, tileSize * 0.6, QPen(Qt::black), QBrush(Qt::red));
    player2Piece = scene->addEllipse(0, 0, tileSize * 0.6, tileSize * 0.6, QPen(Qt::black), QBrush(Qt::blue));
    // Initial positions
    player1Piece->setPos(0, 0);
    player2Piece->setPos(10, 10);
}

/**
 * @brief Updates the scene positions of player tokens based on their current game tile.
 * Introduces a slight offset so that if players are on the same tile then their pieces are differentiable.
 */
void MainWindow::updatePlayerPositions() {
    int p1Pos = player1->getPosition();
    if (p1Pos >= 0 && p1Pos < playableTileCoords.size()) {
        auto [r, c] = playableTileCoords[p1Pos];
        player1Piece->setPos(c * tileSize + 5, r * tileSize + 5);
    }

    int p2Pos = player2->getPosition();
    if (p2Pos >= 0 && p2Pos < playableTileCoords.size()) {
        auto [r, c] = playableTileCoords[p2Pos];
        player2Piece->setPos(c * tileSize + 20, r * tileSize + 20);
    }
}

/**
 * @brief Updates the player stats shown in the side UI panel.
 */
void MainWindow::updatePlayerUI() {
    // Player 1
    ui->p1Name->setText(player1->getName());
    ui->p1Name->setStyleSheet("color: red;");
    ui->p1Money->setText(QString::number(player1->getMoney()));
    ui->p1Income->setText(QString::number(player1->getIncomePercent()) + "%");
    ui->p1Pos->setText(QString::number(player1->getPosition()));

    // Player 2
    ui->p2Name->setText(player2->getName());
    ui->p2Name->setStyleSheet("color: blue;");
    ui->p2Money->setText(QString::number(player2->getMoney()));
    ui->p2Income->setText(QString::number(player2->getIncomePercent()) + "%");
    ui->p2Pos->setText(QString::number(player2->getPosition()));
}

/**
 * @brief Updates the UI panels to indicate whose turn it is.
 * Highlights the active player and grays out the inactive player.
 */
void MainWindow::updatePlayerPanels() {
    if (isPlayer1Turn) {
        ui->p1->setEnabled(true);
        ui->p1->setStyleSheet(""); // Reset style (normal)

        ui->p2->setEnabled(false);
        ui->p2->setStyleSheet("background-color: lightgray;"); // Gray out
    } else {
        ui->p1->setEnabled(false);
        ui->p1->setStyleSheet("background-color: lightgray;"); // Gray out

        ui->p2->setEnabled(true);
        ui->p2->setStyleSheet(""); // Reset style (normal)
    }
}

/**
 * @brief Starts an immediate player movement animation toward a new board position.
 * This is used when a tile effect changes the player's current position.
 *
 * @param newPosition The new logical position (index in playableTileCoords) to animate towards.
 */
void MainWindow::startImmediateMove(int newPosition) {
    if (moveTimer && moveTimer->isActive()) return;  // Prevent overlapping animations

    animationStep = currentPlayer->getPosition();
    targetPosition = newPosition;
    ui->rollDice->setEnabled(false); // Disable again during auto-move

    if (!moveTimer) {
        moveTimer = new QTimer(this);
        connect(moveTimer, &QTimer::timeout, this, &MainWindow::animatePlayerMove);
    }
    moveTimer->start(150);
}
/**
 * @brief Handles game logic for specific tiles
 */
void MainWindow::handleTile(Player *p) {
    int pos = p->getPosition();
    if (pos >= 0 && pos < playableTileCoords.size()) {
        auto [r, c] = playableTileCoords[pos];
        Tile* tile = boardGrid[r][c];
        tile->activate(*p);

        int after = p->getPosition();
        if (after != pos && after < playableTileCoords.size()) {
            startImmediateMove(after);
        }
    }
}

/**
 * @brief Animates a player’s movement from current position to target position, step by step.
 *        Uses a timer to simulate movement.
 */
void MainWindow::animatePlayerMove() {
    if (animationStep < targetPosition) {
        animationStep++;
    }
    else if (animationStep > targetPosition) {
        animationStep--;
    }
    else {
        moveTimer->stop();
        currentPlayer->setPosition(targetPosition);
        updatePlayerPositions();

        int prevPos = currentPlayer->getPosition();
        handleTile(currentPlayer);  // may trigger another move

        if (currentPlayer->getPosition() != prevPos) {
            return; // A tile effect changed position; wait for that animation to finish first
        }

        updatePlayerUI();

        if (currentPlayer->getPosition() >= playableTileCoords.size() - 1) {
            currentPlayer->markFinished();
        }

        QTimer::singleShot(300, this, [this]() {
            switchToNextActivePlayer();

            // Enable the Roll button if it's the player's turn next (human)
            if (!allAI && !(isAI && !isPlayer1Turn)) {
                ui->rollDice->setEnabled(true);
            }
        });

        return;
    }

    auto [r, c] = playableTileCoords[animationStep];

    if (isPlayer1Turn) {
        player1Piece->setPos(c * tileSize + 5, r * tileSize + 5);
    } else {
        player2Piece->setPos(c * tileSize + 20, r * tileSize + 20);
    }
}

/**
 * @brief Switches the current player turn. If both players are finished, ends the game and shows final result popups.
 */
void MainWindow::switchToNextActivePlayer() {   
    for (int i = 0; i < 2; ++i) {
        isPlayer1Turn = !isPlayer1Turn;
        currentPlayer = isPlayer1Turn ? player1 : player2;

        if (!currentPlayer->isFinished()) {
            animationStep = currentPlayer->getPosition();
            updatePlayerPanels();

            if (allAI || (!isPlayer1Turn && isAI)) {
                QTimer::singleShot(500, this, &MainWindow::doAITurn);
            }

            return;
        }
    }

    if (player1->isFinished() && player2->isFinished()) {
        auto getEnding = [](int money) {
            if (money < 0) {
                return "In Debt\nYou owe your school, your landlord, and your mom. Good luck.";
            } else if (money < 10000) {
                return "Unemployed\nYou have no job and live with your parents, downloading random scripts online for your Instagram meme account.";
            } else if (money < 25000) {
                return "Intern\nYou get paid in coffee and startup stickers, but at least you’re in the system.";
            } else if (money < 40000) {
                return "QA Tester\nYou test mobile apps and make TikToks on the side.";
            } else if (money < 60000) {
                return "Junior Dev\nYou finally have a desk, but your monitor is half the size of your phone.";
            } else if (money < 80000) {
                return "Startup Developer\nYou work 80-hour weeks for equity and instant ramen.";
            } else if (money < 100000) {
                return "FAANG Employee\nYou have project deadline after project deadline, but at least you have a stable, nice income.";
            } else if (money < 115000) {
                return "CTO\nYou drink cold brew, say 'scale' a lot, and live on Slack.";
            } else {
                return "CEO\nYou force your employees to play golf with you every Sunday.";
            }
        };

        int p1Money = player1->getMoney();
        int p2Money = player2->getMoney();

        if (!multiGameMode) {
            QString p1Ending = getEnding(p1Money);
            QString p1Stats = player1->getName() + "\n$" + QString::number(p1Money);
            QMessageBox::information(this, "Player 1 Ending", p1Ending + "\n\n" + p1Stats);

            QString p2Ending = getEnding(p2Money);
            QString p2Stats = player2->getName() + "\n$" + QString::number(p2Money);
            QMessageBox::information(this, "Player 2 Ending", p2Ending + "\n\n" + p2Stats);

            ui->rollDice->setEnabled(false);
            if (player1->getName() != "CPU")
                updateLeaderboard(player1->getName(), p1Money);

            if (player2->getName() != "CPU")
                updateLeaderboard(player2->getName(), p2Money);

            QTimer::singleShot(500, this, []() {
                QApplication::quit();
            });
        } else {
            onGameEnd();
        }


    }
}

/**
 * @brief Updates the leaderboard CSV by adding money earned by the given player.
 *
 * This function reads from a file called leaderboard.csv, adds the money earned
 * in the current session to the player's total, and writes the updated totals back.
 *
 * @param name The name of the player whose earnings should be added.
 * @param money The amount of money the player earned in this game session.
 *
 * @return void
 */
void MainWindow::updateLeaderboard(const QString& name, int money) {
    QFile file("leaderboard.csv");
    QMap<QString, int> data;

    // Load existing data
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 2) {
                QString player = parts[0];
                int total = parts[1].toInt();
                data[player] = total;
            }
        }
        file.close();
    }

    // Update the current player's total
    data[name] += money;

    // Rewrite the file
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (auto it = data.begin(); it != data.end(); ++it) {
            out << it.key() << "," << it.value() << "\n";
        }
        file.close();
    }
}

// ------------------------- AI GAME LOGIC HANDLING -------------------------
/**
 * @brief Does AI turn
 */
void MainWindow::doAITurn() {
    if (moveTimer && moveTimer->isActive()) return;  // Prevent double trigger

    if (currentPlayer->isFinished()) {
        switchToNextActivePlayer();
        return;
    }

    if (currentPlayer->shouldSkipTurn()) {
        currentPlayer->clearSkipTurn();
        currentPlayer->notify("You're frozen in time.. turn was skipped!");
        switchToNextActivePlayer();
        return;
    }

    if (!currentPlayer->accessPowerups().empty() && rand() % 2 == 0) {
        int index = rand() % currentPlayer->accessPowerups().size();
        std::unique_ptr<PowerUp>& selected = currentPlayer->accessPowerups()[index];
        selected->apply(*currentPlayer);
        currentPlayer->accessPowerups().erase(currentPlayer->accessPowerups().begin() + index);
    }

    int roll = rand() % 6 + 1;
    ui->Dice->setText("DICE: " + QString::number(roll));
    animationStep = currentPlayer->getPosition();
    targetPosition = std::min(animationStep + roll, static_cast<int>(playableTileCoords.size()) - 1);

    ui->rollDice->setEnabled(false);

    if (!moveTimer) {
        moveTimer = new QTimer(this);
        connect(moveTimer, &QTimer::timeout, this, &MainWindow::animatePlayerMove);
    }
    moveTimer->start(150);
}
/**
 * @brief Starts a new game with the same AI players
 */
void MainWindow::startNewAIGame() {
    // Reset board and players but keep win statistics
    player1->setPosition(0);
    player2->setPosition(0);
    player1->addMoney(-player1->getMoney()); // Reset money to 0
    player2->addMoney(-player2->getMoney()); // Reset money to 0
    player1->resetFinished();
    player2->resetFinished();

    // Reset turn to player 1
    isPlayer1Turn = true;
    currentPlayer = player1;
    animationStep = 0;
    targetPosition = 0;

    // Update UI
    updatePlayerPositions();
    updatePlayerUI();
    updatePlayerPanels();

    // Start the next game
    QTimer::singleShot(500, this, &MainWindow::doAITurn);
}
/**
 * @brief Called when a game ends to track statistics and start a new game if in multi-game mode
 */
void MainWindow::onGameEnd(bool showResults) {
    gamesPlayed++;

    // Determine winner based on money
    int p1Money = player1->getMoney();
    int p2Money = player2->getMoney();

    if (p1Money > p2Money) {
        player1Wins++;
    } else if (p2Money > p1Money) {
        player2Wins++;
    }
    // Ties don't count as wins for either player

    // Show the graph if requested
    if (showResults) {
        ResultsGraphDialog resultsDialog(player1Wins, player2Wins, gamesPlayed, totalGames - gamesPlayed, this);
        resultsDialog.exec();
    }

    // Check if there are more games to play
    if (gamesPlayed < totalGames) {
        startNewAIGame();
    } else {
        // All games complete, show final results
        ResultsGraphDialog finalDialog(player1Wins, player2Wins, gamesPlayed, 0, this);
        finalDialog.exec();

        QApplication::quit();
    }
}

// ------------------------- ON CLICKED SLOTS -------------------------
/**
 * @brief Opens the power-up dialog for Player 1.
 */
void MainWindow::on_p1ViewPower_clicked()
{
    PowerDialog* dialog = new PowerDialog(player1, this, this);
    dialog->exec();
}
/**
 * @brief Opens the power-up dialog for Player 2.
 */
void MainWindow::on_p2ViewPower_clicked()
{
    PowerDialog* dialog = new PowerDialog(player2, this, this);
    dialog->exec();
}
/**
 * @brief Handles dice roll: animates movement and manages turn transitions.
 */
void MainWindow::on_rollDice_clicked() {
    if (currentPlayer->isFinished()) {
        // Skip if already finished
        switchToNextActivePlayer();
        return;
    }

    if (currentPlayer->shouldSkipTurn()) {
        // Reset skip state
        currentPlayer->clearSkipTurn();
        currentPlayer->notify("You're frozen in time.. turn was skipped!");
        switchToNextActivePlayer();
        return;
    }
    // Roll between 1 and 6
    int roll = rand() % 6 + 1;
    ui->Dice->setText("DICE: " + QString::number(roll));
    // Start pos
    animationStep = currentPlayer->getPosition();
    targetPosition = std::min(animationStep + roll, static_cast<int>(playableTileCoords.size()) - 1);
    // Disable while moving
    ui->rollDice->setEnabled(false);

    if (!moveTimer) {
        moveTimer = new QTimer(this);
        connect(moveTimer, &QTimer::timeout, this, &MainWindow::animatePlayerMove);
    }
    // Animate every 150ms
    moveTimer->start(150);
}
/**
 * @brief Displays the leaderboard in a styled popup message box.
 */
void MainWindow::on_Leaderboard_clicked()
{
    QFile file("leaderboard.csv");
    QVector<QPair<QString, int>> entries;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            // Parse CSV
            QStringList parts = line.split(",");
            if (parts.size() == 2) {
                QString name = parts[0].trimmed();
                int money = parts[1].toInt();
                entries.append({name, money});
            }
        }
        file.close();
    }

    // Sort descending by money
    std::sort(entries.begin(), entries.end(), [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
        return a.second > b.second;
    });

    QString leaderboardText = " <b>Leaderboard</b> – <i>Forbes Top List</i><br><br>";
    leaderboardText += "<table width='100%' style='font-family: monospace;'>"
                       "<tr><th align='left'>Rank</th><th align='left'>Name</th><th align='right'>Total $</th></tr>";

    int rank = 1;
    for (const auto& entry : entries) {
        // Show top 10 only
        if (rank > 10) break;
        leaderboardText += QString("<tr><td>%1.</td><td>%2</td><td align='right'>$%3</td></tr>")
                               .arg(rank++)
                               .arg(entry.first)
                               .arg(QString::number(entry.second));
    }

    leaderboardText += "</table>";
    // Show popup
    QMessageBox msgBox;
    msgBox.setWindowTitle("Leaderboard");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(leaderboardText);
    msgBox.exec();
}
/**
 * @brief Shows a basic tutorial popup explaining how to play the game.
 */
void MainWindow::on_Tutorial_clicked()
{
    QMessageBox::information(
        this,
        "How to Play",
        "Click the 'Roll' button to roll the dice and move forward.\n\n"
        "Land on different tiles to gain money, power-ups, or encounter events.\n"
        "Reach the end of the board to finish the game!"
        );
}

void MainWindow::on_Shop_clicked()
{
    ShopDialog* dialog = new ShopDialog(currentPlayer, this);
    dialog->exec();
}

