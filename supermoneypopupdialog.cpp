#include "supermoneypopupdialog.h"
#include "ui_supermoneypopupdialog.h"

SuperMoneyPopupDialog::SuperMoneyPopupDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SuperMoneyPopupDialog)
{
    ui->setupUi(this);
}

SuperMoneyPopupDialog::~SuperMoneyPopupDialog()
{
    delete ui;
}

void SuperMoneyPopupDialog::updateText(const QString &newText)
{
    ui->moneyLabel->setText(newText);
}
