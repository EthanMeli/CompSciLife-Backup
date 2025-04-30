#ifndef TEXTPOPUPDIALOG_H
#define TEXTPOPUPDIALOG_H

#include <QDialog>

namespace Ui {
class TextPopupDialog;
}

class TextPopupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextPopupDialog(QWidget *parent = nullptr);

    void updateText(const QString &text);

    ~TextPopupDialog();

private:
    Ui::TextPopupDialog *ui;
};

#endif // TEXTPOPUPDIALOG_H
