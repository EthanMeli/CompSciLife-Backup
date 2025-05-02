#include "resultsgraphdialog.h"
#include <QApplication>

BarGraphWidget::BarGraphWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(300, 200);
}

void BarGraphWidget::setResults(int p1Wins, int p2Wins)
{
    player1Wins = p1Wins;
    player2Wins = p2Wins;
    update();
}

void BarGraphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Calculate total for percentage
    int total = player1Wins + player2Wins;
    if (total == 0) total = 1; // Prevent division by 0

    int width = this->width();
    int height = this->height() - 40;

    // Draw background
    painter.fillRect(0, 0, width, height + 40, Qt::white);

    // Draw bars
    int barWidth = width / 5;
    int spacing = width / 10;
    int p1BarHeight = player1Wins * height / total;
    int p2BarHeight = player2Wins * height / total;

    // Player 1 bar (red)
    painter.setBrush(Qt::red);
    painter.setPen(Qt::black);
    QRect p1Rect(spacing, height - p1BarHeight, barWidth, p1BarHeight);
    painter.drawRect(p1Rect);

    // Player 2 bar (blue)
    painter.setBrush(Qt::blue);
    QRect p2Rect(width - spacing - barWidth, height - p2BarHeight, barWidth, p2BarHeight);
    painter.drawRect(p2Rect);

    // Draw labels
    painter.setPen(Qt::black);
    painter.drawText(spacing, height + 20, barWidth, 20, Qt::AlignCenter, "CPU 1");
    painter.drawText(width - spacing - barWidth, height + 20, barWidth, 20, Qt::AlignCenter, "CPU 2");

    // Draw win counts
    painter.drawText(spacing, height - p1BarHeight - 20, barWidth, 20, Qt::AlignCenter, QString::number(player1Wins));
    painter.drawText(width - spacing - barWidth, height - p2BarHeight - 20, barWidth, 20, Qt::AlignCenter, QString::number(player2Wins));
}

ResultsGraphDialog::ResultsGraphDialog(int p1Wins, int p2Wins, int gamesPlayed, int gamesRemaining, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Game Results");

    QVBoxLayout *layout = new QVBoxLayout(this);

    resultsLabel = new QLabel(QString("Games played: %1 / %2").arg(gamesPlayed).arg(gamesPlayed + gamesRemaining));
    layout->addWidget(resultsLabel);

    graphWidget = new BarGraphWidget(this);
    graphWidget->setResults(p1Wins, p2Wins);
    layout->addWidget(graphWidget);

    continueButton = new QPushButton("Continue", this);
    layout->addWidget(continueButton);

    connect(continueButton, &QPushButton::clicked, this, &QDialog::accept);

    setLayout(layout);
    resize(400, 300);
}

ResultsGraphDialog::~ResultsGraphDialog()
{

}
