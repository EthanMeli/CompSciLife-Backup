#ifndef SUPERMONEYPOPUPDIALOG_H
#define SUPERMONEYPOPUPDIALOG_H

#include <QDialog>

namespace Ui {
class SuperMoneyPopupDialog;
}

class SuperMoneyPopupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SuperMoneyPopupDialog(QWidget *parent = nullptr);
    ~SuperMoneyPopupDialog();

    void updateText(const QString& newText);

private:
    Ui::SuperMoneyPopupDialog* ui;
};

#endif // SUPERMONEYPOPUPDIALOG_H
