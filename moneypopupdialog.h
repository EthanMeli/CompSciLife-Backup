#ifndef MONEYPOPUPDIALOG_H
#define MONEYPOPUPDIALOG_H

#include <QDialog>

namespace Ui {
class MoneyPopupDialog;
}

class MoneyPopupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MoneyPopupDialog(QWidget *parent = nullptr);

    void updateText(const QString &newText);

    ~MoneyPopupDialog();

private:
    Ui::MoneyPopupDialog *ui;
};

#endif // MONEYPOPUPDIALOG_H
