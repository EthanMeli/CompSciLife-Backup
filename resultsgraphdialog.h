#ifndef RESULTSGRAPHDIALOG_H
#define RESULTSGRAPHDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>

class BarGraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BarGraphWidget(QWidget *parent = nullptr);
    void setResults(int p1Wins, int p2Wins);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int player1Wins = 0;
    int player2Wins = 0;
};

class ResultsGraphDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultsGraphDialog(int p1Wins, int p2Wins, int gamesPlayed, int gamesRemaining, QWidget *parent = nullptr);
    ~ResultsGraphDialog();

private:
    BarGraphWidget *graphWidget;
    QPushButton *continueButton;
    QLabel *resultsLabel;
};

#endif // RESULTSGRAPHDIALOG_H
