#ifndef AIGAMESDIALOG_H
#define AIGAMESDIALOG_H

#include <QDialog>

namespace Ui {
class AIGamesDialog;
}

class AIGamesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AIGamesDialog(QWidget *parent = nullptr);
    ~AIGamesDialog();
    int getNumberOfGames() const;

private slots:
    void on_startGamesButton_clicked();

private:
    Ui::AIGamesDialog *ui;
};

#endif // AIGAMESDIALOG_H
