#include "lifeeventdialog.h"
#include "ui_lifeeventdialog.h"

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

void LifeEventDialog::on_rollDiceButton_clicked()
{
    diceRoll = rand() % 6 + 1;
    accept();
}
