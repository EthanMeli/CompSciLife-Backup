#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tiles.h"
#include "powerdialog.h"
#include "playerinfodialog.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <algorithm>
#include <QMessageBox>

/**
 * @brief Constructor for MainWindow. Initializes the UI and game board.
 * @param parent Pointer to parent QWidget.
 */
MainWindow::MainWindow(Player* p1, Player* p2, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player1 = p1;
    player2 = p2;

    player1->setOpponent(player2);
    player2->setOpponent(player1);

    currentPlayer = player1; // Game starts with player 1, duh

    setupBoard();
    updatePlayerPanels();
    updatePlayerUI();
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

    // Assign special tiles at certain locations
    boardGrid[0][2] = new MoveForwardTile();
    boardGrid[0][4] = new PowerupTile();
    boardGrid[0][8] = new MoneyTile();
    boardGrid[1][8] = new MoveBackwardTile();
    boardGrid[2][0] = new MoneyTile();
    boardGrid[2][6] = new LifeEventTile();
    boardGrid[2][3] = new MoveBackwardTile();
    boardGrid[2][1] = new PowerupTile();
    boardGrid[4][0] = new MoveBackwardTile();
    boardGrid[4][3] = new MoneyTile();
    boardGrid[4][4] = new MoveForwardTile();
    boardGrid[4][8] = new LifeEventTile();
    boardGrid[6][7] = new PowerupTile();
    boardGrid[6][5] = new MoneyTile();
    boardGrid[6][4] = new MoveBackwardTile();
    boardGrid[6][2] = new MoneyTile();
    boardGrid[6][1] = new MoveForwardTile();
    boardGrid[8][0] = new LifeEventTile();
    boardGrid[8][3] = new MoveBackwardTile();
    boardGrid[8][5] = new MoneyTile();
    boardGrid[8][7] = new MoveBackwardTile();

    // Tile visuals
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            QGraphicsRectItem *tile = scene->addRect(col * tileSize, row * tileSize, tileSize, tileSize);
            // Special handling for white tiles in odd rows
            if (row % 2 == 1) {
                if (boardGrid[row][col]->getColor() == Qt::white) {
                    if ((row == 1 || row == 5) && col == 8) {
                        tile->setBrush(boardGrid[row][col]->getColor());
                    }
                    else if ((row == 3 || row == 7) && col == 0) {
                        tile->setBrush(boardGrid[row][col]->getColor());
                    }
                    else {
                        tile->setBrush(Qt::darkGray); // Unplayable tile
                    }
                }
                else {
                    tile->setBrush(boardGrid[row][col]->getColor());
                }
            }
            else {
                tile->setBrush(boardGrid[row][col]->getColor());
            }
            tile->setPen(QPen(Qt::black));
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
    if (player1->getPosition() < playableTileCoords.size()) {
        auto [r, c] = playableTileCoords[player1->getPosition()];
        player1Piece->setPos(c * tileSize + 5, r * tileSize + 5);
    }

    if (player2->getPosition() < playableTileCoords.size()) {
        auto [r, c] = playableTileCoords[player2->getPosition()];
        player2Piece->setPos(c * tileSize + 20, r * tileSize + 20);
    }
}

/**
 * @brief Updates the player stats shown in the side UI panel.
 */
void MainWindow::updatePlayerUI() {
    // Player 1
    ui->p1Name->setText(player1->getName());
    ui->p1Money->setText(QString::number(player1->getMoney()));
    ui->p1Income->setText(QString::number(player1->getIncomePercent()) + "%");
    ui->p1Pos->setText(QString::number(player1->getPosition()));

    // Player 2
    ui->p2Name->setText(player2->getName());
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
 * TODO: CREATE UI DESIGN FOR EACH TILE OPTION
 */
void MainWindow::handleTile(Player *p) {
    int before = p->getPosition(); // Save position before activation
    auto [r, c] = playableTileCoords[before];
    Tile* tile = boardGrid[r][c];

    tile->activate(*p); // Could move player

    int after = p->getPosition(); // After activation

    if (after != before) {
        startImmediateMove(after);
    }
}

/**
 * @brief Animates a player’s movement from current position to target position, step by step.
 *        Uses a timer to simulate movement.
 */
// Maybe change it so that animation is more fluid? Idk
void MainWindow::animatePlayerMove() {
    if (animationStep < targetPosition) {
        animationStep++;
    }
    else if (animationStep > targetPosition) {
        animationStep--;
    }
    else {
        // Animation done
        moveTimer->stop();

        currentPlayer->setPosition(targetPosition);
        updatePlayerPositions();

        int prevPos = currentPlayer->getPosition();
        handleTile(currentPlayer);
        updatePlayerUI();

        if (currentPlayer->getPosition() != prevPos) {
            return;
        }

        if (currentPlayer->getPosition() >= playableTileCoords.size() - 1) {
            currentPlayer->markFinished();
        }

        switchToNextActivePlayer();
        ui->rollDice->setEnabled(true);
        return;
    }

    // After adjusting animationStep, move the piece visually
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
            return;
        }
    }
    // End of game
    // Show each player's outcome in popups
    if (player1->isFinished() && player2->isFinished()) {
        QString p1Ending;
        int p1Money = player1->getMoney();

        if (p1Money < 1000) {
            p1Ending = "Unemployed\nYou have no job and live with your parents, downloading random scripts online for your Instagram meme account.";
        } else if (p1Money < 10000) {
            p1Ending = "McDonald's Manager\nYou rule over your local McDonald's and put the fries in the bag.";
        } else if (p1Money < 100000) {
            p1Ending = "FAANG Employee\nYou have project deadline after project deadline, but at least you have a stable, nice income.";
        } else {
            p1Ending = "CEO\nYou force your employees to play golf with you every Sunday.";
        }

        QString p1Stats = player1->getName() + "\n$" + QString::number(p1Money);

        QMessageBox::information(this, "Player 1 Ending", p1Ending + "\n\n" + p1Stats);

        QString p2Ending;
        int p2Money = player2->getMoney();

        if (p2Money < 1000) {
            p2Ending = "Unemployed\nYou have no job and live with your parents, downloading random scripts online for your Instagram meme account.";
        } else if (p2Money < 10000) {
            p2Ending = "McDonald's Manager\nYou rule over your local McDonald's and put the fries in the bag.";
        } else if (p2Money < 100000) {
            p2Ending = "FAANG Employee\nYou have project deadline after project deadline, but at least you have a stable, nice income.";
        } else {
            p2Ending = "CEO\nYou force your employees to play golf with you every Sunday.";
        }

        QString p2Stats = player2->getName() + "\n$" + QString::number(p2Money);

        QMessageBox::information(this, "Player 2 Ending", p2Ending + "\n\n" + p2Stats);

        // Game ends
        ui->rollDice->setEnabled(false);
    }
}

// ------------------------- ON CLICKED SLOTS -------------------------

// Player 1 power up list
void MainWindow::on_p1ViewPower_clicked()
{
    PowerDialog* dialog = new PowerDialog(player1, this, this);
    dialog->exec();
}
// Player 2 power up list
void MainWindow::on_p2ViewPower_clicked()
{
    PowerDialog* dialog = new PowerDialog(player2, this, this);
    dialog->exec();
}
// Rolls the dice, animates movement, and handles turn switching.
void MainWindow::on_rollDice_clicked() {
    if (currentPlayer->isFinished()) {
        switchToNextActivePlayer();
        return;
    }

    int roll = rand() % 6 + 1;
    ui->Dice->setText("DICE: " + QString::number(roll));

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



