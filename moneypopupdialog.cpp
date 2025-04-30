#include "moneypopupdialog.h"
#include "ui_moneypopupdialog.h"

MoneyPopupDialog::MoneyPopupDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MoneyPopupDialog)
{
    ui->setupUi(this);
}

MoneyPopupDialog::~MoneyPopupDialog()
{
    delete ui;
}

void MoneyPopupDialog::updateText(const QString &newText)
{
    ui->moneyLabel->setText(newText);
}
