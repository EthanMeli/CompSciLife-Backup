#include "textpopupdialog.h"
#include "ui_textpopupdialog.h"

TextPopupDialog::TextPopupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextPopupDialog)
{
    ui->setupUi(this);
}

TextPopupDialog::~TextPopupDialog()
{
    delete ui;
}

void TextPopupDialog::updateText(const QString &text)
{
    ui->textLabel->setText(text);
}
