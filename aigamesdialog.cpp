#include "aigamesdialog.h"
#include "ui_aigamesdialog.h"

AIGamesDialog::AIGamesDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AIGamesDialog)
{
    ui->setupUi(this);
}

AIGamesDialog::~AIGamesDialog()
{
    delete ui;
}

int AIGamesDialog::getNumberOfGames() const
{
    return ui->numGamesSlider->value();
}

void AIGamesDialog::on_startGamesButton_clicked()
{
    accept();
}
