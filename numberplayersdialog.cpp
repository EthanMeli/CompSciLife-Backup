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

bool NumberPlayersDialog::getAISelected() const {
    return ai_selected;
}

bool NumberPlayersDialog::getAllCPUSelected() const {
    return all_cpu_selected;
}

void NumberPlayersDialog::on_twoPlayerButton_clicked() {
    ai_selected = false;
    all_cpu_selected = false;
    accept();
}

void NumberPlayersDialog::on_vsCPUButton_clicked() {
    ai_selected = true;
    all_cpu_selected = false;
    accept();
}

void NumberPlayersDialog::on_allCPUButton_clicked() {
    ai_selected = true;
    all_cpu_selected = true;
    accept();
}
