#ifndef SHOPDIALOG_H
#define SHOPDIALOG_H

#include <QDialog>
#include "gamelogic.h"
#include "mainwindow.h"

namespace Ui {
class ShopDialog;
}

class ShopDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShopDialog(Player* player, MainWindow* mainWindow, QWidget *parent = nullptr);
    ~ShopDialog();

private slots:
    void on_buyMove_clicked();
    void on_buySteal_clicked();
    void on_buyMoney_clicked();
    void on_buyLottery_clicked();
    void on_buyStore_clicked();

private:
    Ui::ShopDialog *ui;
    Player* player;
    MainWindow* mainWindowRef;
};

#endif // SHOPDIALOG_H
