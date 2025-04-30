#include "playerinfodialog.h"
#include "ui_playerinfodialog.h"
#include <QMessageBox>

PlayerInfoDialog::PlayerInfoDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PlayerInfoDialog)
{
    ui->setupUi(this);
}

PlayerInfoDialog::~PlayerInfoDialog()
{
    delete ui;
}

QString PlayerInfoDialog::getPlayer1Name() const {
    return ui->lineEditP1->text().trimmed();
}

QString PlayerInfoDialog::getPlayer2Name() const {
    return ui->lineEditP2->text().trimmed();
}

void PlayerInfoDialog::on_startGameButton_clicked()
{
    if (getPlayer1Name().isEmpty() || getPlayer2Name().isEmpty()) {
        QMessageBox::warning(this, "Missing Input", "Please enter both player names.");
        return;
    }

    accept(); // Close dialog and return QDialog::Accepted
}

