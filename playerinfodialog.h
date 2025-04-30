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
    explicit PlayerInfoDialog(QWidget *parent = nullptr);
    ~PlayerInfoDialog();

    // Getting inputted player names
    QString getPlayer1Name() const;
    QString getPlayer2Name() const;

private slots:
    void on_startGameButton_clicked();

private:
    Ui::PlayerInfoDialog *ui;
};

#endif // PLAYERINFODIALOG_H
