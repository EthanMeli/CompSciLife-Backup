#include "mainwindow.h"
#include "playerinfodialog.h"
#include "numberplayersdialog.h"
#include "aigamesdialog.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    NumberPlayersDialog playerSelectDialog;
    if (playerSelectDialog.exec() != QDialog::Accepted) {
        return 0;
    }

    bool isAISelected = playerSelectDialog.getAISelected();
    bool isAllCPU = playerSelectDialog.getAllCPUSelected();

    Player* p1;
    Player* p2;
    int totalGames = 1;

    if (isAllCPU) {
        // Ask for number of games
        AIGamesDialog aiDialog;
        if (aiDialog.exec() != QDialog::Accepted) {
            return 0;
        }
        totalGames = aiDialog.getNumberOfGames();

        p1 = new Player("CPU");
        p2 = new Player("CPU");
    } else {
        PlayerInfoDialog playerDialog(isAISelected);
        if (playerDialog.exec() != QDialog::Accepted) {
            return 0;
        }

        QString player1 = playerDialog.getPlayer1Name();
        QString player2 = playerDialog.getPlayer2Name();

        p1 = new Player(player1);
        p2 = new Player(player2);
    }

    MainWindow w(p1, p2, totalGames);
    w.show();
    return a.exec();
}
