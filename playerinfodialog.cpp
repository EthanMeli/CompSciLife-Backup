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

    accept();
}

