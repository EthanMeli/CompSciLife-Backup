#include "playerinfodialog.h"
#include "ui_playerinfodialog.h"
#include <QMessageBox>

PlayerInfoDialog::PlayerInfoDialog(bool isAI, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PlayerInfoDialog)
{
    ui->setupUi(this);
    isAIPlayer = isAI;
    if (isAIPlayer) {
        ui->lineEditP2->setText("CPU");
        ui->lineEditP2->setEnabled(false);
    }
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
    QString name1 = getPlayer1Name();
    QString name2 = getPlayer2Name();

    if (name1.isEmpty() || name2.isEmpty()) {
        QMessageBox::warning(this, "Missing Input", "Please enter both player names.");
        return;
    }

    if (name1 == name2) {
        QMessageBox::warning(this, "Duplicate Names", "Players must have different names.");
        return;
    }

    if (name1.toUpper() == "CPU") {
        QMessageBox::warning(this, "You ain't a robot!", "Player 1 cannot be named 'CPU'.");
        return;
    }

    if (!isAIPlayer && name2.toUpper() == "CPU") {
        QMessageBox::warning(this, "You ain't a robot!", "Player 2 cannot be named 'CPU' unless you're playing vs AI.");
        return;
    }

    accept();
}

