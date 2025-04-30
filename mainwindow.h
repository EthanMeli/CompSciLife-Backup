#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tiles.h"
#include <QMainWindow>
#include <QVector>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Player* p1, Player* p2, QWidget *parent = nullptr);
    ~MainWindow();
    void updatePlayerUI();
    void handleTile(Player *p); // Main logic for handling tile specific actions
    void updatePlayerPositions();
    void setupBoard();
    void animatePlayerMove();
    void switchToNextActivePlayer();
    void updatePlayerPanels();
    void startImmediateMove(int newPosition);

private slots:
    void on_p2ViewPower_clicked();

    void on_p1ViewPower_clicked();

    void on_rollDice_clicked();

private:
    Ui::MainWindow *ui;    
    Player* player1 = new Player("Player 1");
    Player* player2 = new Player("Player 2");

    QGraphicsEllipseItem* player1Piece = nullptr;
    QGraphicsEllipseItem* player2Piece = nullptr;

    Player* currentPlayer;
    bool isPlayer1Turn = true;

    QVector<QPair<int, int>> playableTileCoords;

    // Tile settings
    int tileSize = 50;
    int rows = 9;
    int cols = 9;

    // Board setup
    QVector<QVector<Tile*>> boardGrid;

    QTimer* moveTimer = nullptr;
    int animationStep = 0;
    int targetPosition = 0;
};
#endif // MAINWINDOW_H
