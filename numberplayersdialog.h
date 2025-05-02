#ifndef NUMBERPLAYERSDIALOG_H
#define NUMBERPLAYERSDIALOG_H

#include <QDialog>

namespace Ui {
class NumberPlayersDialog;
}

class NumberPlayersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NumberPlayersDialog(QWidget *parent = nullptr);
    ~NumberPlayersDialog();

    bool getAISelected() const;
    bool getAllCPUSelected() const;

private slots:
    void on_twoPlayerButton_clicked();

    void on_vsCPUButton_clicked();

    void on_allCPUButton_clicked();

private:
    Ui::NumberPlayersDialog *ui;
    bool ai_selected = false;
    bool all_cpu_selected = false;
};

#endif // NUMBERPLAYERSDIALOG_H
