#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tiles.h"
#include <QMainWindow>
#include <QVector>
#include <QTimer>
#include <QGraphicsEllipseItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Player* p1, Player* p2, int numGames = 1, QWidget *parent = nullptr);
    ~MainWindow();

    // Game logic
    void setupBoard();
    void updatePlayerUI();
    void updatePlayerPositions();
    void updatePlayerPanels();
    void handleTile(Player* p);
    void animatePlayerMove();
    void startImmediateMove(int newPosition);
    void switchToNextActivePlayer();
    void updateLeaderboard(const QString& name, int money);
    void startNewAIGame();
    void onGameEnd(bool showResults = true);

private slots:
    void on_p1ViewPower_clicked();
    void on_p2ViewPower_clicked();
    void on_rollDice_clicked();
    void on_Leaderboard_clicked();
    void on_Tutorial_clicked();
    void on_Shop_clicked();

private:
    Ui::MainWindow* ui;

    // Players
    Player* player1 = new Player("Player 1");
    Player* player2 = new Player("Player 2");
    Player* currentPlayer = nullptr;
    bool isPlayer1Turn = true;

    // AI handling
    bool isAI = false;
    bool allAI = false;
    void doAITurn();

    // Multi Game AI
    int totalGames = 1;
    int gamesPlayed = 0;
    int player1Wins = 0;
    int player2Wins = 0;
    bool multiGameMode = false;

    // Player pieces
    QGraphicsEllipseItem* player1Piece = nullptr;
    QGraphicsEllipseItem* player2Piece = nullptr;

    // Board data
    int tileSize = 50;
    int rows = 11;
    int cols = 13;
    QVector<QVector<Tile*>> boardGrid;
    QVector<QPair<int, int>> playableTileCoords;

    // Animation
    QTimer* moveTimer = nullptr;
    int animationStep = 0;
    int targetPosition = 0;
};

#endif // MAINWINDOW_H
