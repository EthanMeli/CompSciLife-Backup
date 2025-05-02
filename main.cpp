#include "mainwindow.h"
#include "playerinfodialog.h"
#include "numberplayersdialog.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    NumberPlayersDialog playerSelectDialog;
    if (playerSelectDialog.exec() != QDialog::Accepted) {
        return 0;
    }
    bool isAISelected = playerSelectDialog.getAISelected();

    PlayerInfoDialog playerDialog(isAISelected);
    if (playerDialog.exec() != QDialog::Accepted) {
        return 0;
    }
    QString player1 = playerDialog.getPlayer1Name();
    QString player2 = playerDialog.getPlayer2Name();

    Player* p1 = new Player(player1);
    Player* p2 = new Player(player2);

    MainWindow w(p1, p2);
    w.show();
    return a.exec();
}
