#include "numberplayersdialog.h"
#include "ui_numberplayersdialog.h"

NumberPlayersDialog::NumberPlayersDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NumberPlayersDialog)
{
    ui->setupUi(this);
}

NumberPlayersDialog::~NumberPlayersDialog()
{
    delete ui;
}

void NumberPlayersDialog::on_twoPlayerButton_clicked()
{
    // Keep AI selected as false
    ai_selected = false;
    accept();
}


void NumberPlayersDialog::on_vsCPUButton_clicked()
{
    ai_selected = true;
    accept();
}
