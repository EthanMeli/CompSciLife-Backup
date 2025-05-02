#ifndef LIFEEVENTDIALOG_H
#define LIFEEVENTDIALOG_H

#include <QDialog>

namespace Ui {
class LifeEventDialog;
}

class LifeEventDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LifeEventDialog(QWidget* parent = nullptr);
    ~LifeEventDialog();

    void updateText(const QString& newText);
    void disableDiceRoll();
    int getDiceRoll() const { return diceRoll; }
    void setDiceRollManually(int value);

private slots:
    void on_rollDiceButton_clicked();

private:
    Ui::LifeEventDialog* ui;
    int diceRoll = 1;
};

#endif // LIFEEVENTDIALOG_H
