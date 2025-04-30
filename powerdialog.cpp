#include "powerdialog.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>

// Constructor: Initializes dialog with a reference to the player and sets up the UI
PowerDialog::PowerDialog(Player* player, MainWindow* mainWindow, QWidget *parent)
    : QDialog(parent), playerRef(player), mainWindowRef(mainWindow)
{
    // Dialog title
    setWindowTitle("Available Power-Ups");
    // Default size
    setMinimumSize(400, 200);
    // Create layout and widgets
    setupUI();
}

// Destructor
PowerDialog::~PowerDialog() {}

// Sets up the vertical layout with 3 power up slots, each with a label and button
void PowerDialog::setupUI() {
    // Main layout container
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    // Get current powerups from the player
    const auto& powerups = playerRef->getPowerups();

    QFont titleFont;
    titleFont.setBold(true);

    for (int i = 0; i < 3; ++i) {
        QHBoxLayout* rowLayout = new QHBoxLayout();

        QLabel* label = new QLabel(this);
        label->setMinimumWidth(200);
        // Button to activate the power-up
        QPushButton* useButton = new QPushButton("Use", this);
        useButtons.push_back(useButton);

        if (i < static_cast<int>(powerups.size())) {
            label->setText(powerups[i]->name());
            label->setFont(titleFont);
            useButton->setEnabled(true);
            connect(useButton, &QPushButton::clicked, [=]() { usePowerup(i); });
        } else {
            label->setText("None");
            useButton->setEnabled(false);
        }
        // Layout arrangement
        rowLayout->addWidget(label);
        rowLayout->addStretch();
        rowLayout->addWidget(useButton);
        mainLayout->addLayout(rowLayout);
    }

    mainLayout->addStretch();
    setLayout(mainLayout);
}
// Applies the selected power up and closes the dialog
void PowerDialog::usePowerup(int index) {
    if (index >= 0 && index < static_cast<int>(playerRef->getPowerups().size())) {
        playerRef->usePowerup(index);

        if (mainWindowRef) {
            mainWindowRef->updatePlayerUI();
            mainWindowRef->updatePlayerPositions();
        }

        accept(); // Close dialog
    }
}
