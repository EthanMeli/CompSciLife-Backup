#ifndef PLAYERINFODIALOG_H
#define PLAYERINFODIALOG_H

#include <QDialog>

namespace Ui {
class PlayerInfoDialog;
}

class PlayerInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlayerInfoDialog(bool isAI, QWidget *parent = nullptr);
    ~PlayerInfoDialog();

    QString getPlayer1Name() const;
    QString getPlayer2Name() const;

private slots:
    void on_startGameButton_clicked();

private:
    Ui::PlayerInfoDialog* ui;
    bool isAIPlayer = false;
};

#endif // PLAYERINFODIALOG_H
