#ifndef POWERDIALOG_H
#define POWERDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <vector>
#include "gamelogic.h"

class MainWindow;

class PowerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PowerDialog(Player* player, MainWindow* mainWindow, QWidget* parent = nullptr);
    ~PowerDialog();

private slots:
    void usePowerup(int index);

private:
    void setupUI();

    Player* playerRef;
    MainWindow* mainWindowRef;
    std::vector<QPushButton*> useButtons;
};

#endif // POWERDIALOG_H
