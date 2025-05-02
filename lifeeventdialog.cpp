#include "lifeeventdialog.h"
#include "ui_lifeeventdialog.h"

/**
 * @brief Constructs a LifeEventDialog with the given parent.
 * @param parent The parent widget.
 */
LifeEventDialog::LifeEventDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LifeEventDialog)
{
    ui->setupUi(this);
}

LifeEventDialog::~LifeEventDialog()
{
    delete ui;
}

void LifeEventDialog::updateText(const QString& newText) {
    ui->eventDetails->setText(newText);
}

void LifeEventDialog::disableDiceRoll() {
    ui->rollDiceButton->setEnabled(false);
}

void LifeEventDialog::setDiceRollManually(int value) {
    diceRoll = value;
}

void LifeEventDialog::on_rollDiceButton_clicked()
{
    diceRoll = rand() % 6 + 1;
    accept();
}
