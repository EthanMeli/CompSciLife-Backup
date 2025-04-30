#include "tiles.h"
#include "moneypopupdialog.h"
#include "lifeeventdialog.h"
#include "textpopupdialog.h"
#include <cstdlib>
#include <ctime>
#include <QMessageBox>

/**
 * @brief Creates and returns a new tile based on the specified type.
 * @param type The type of tile to create (enum: TileType).
 * @return Pointer to a dynamically allocated Tile object of the appropriate subclass.
 */
Tile* TileFactory::createTile(TileType type) {
    switch (type) {
        case TileType::EMPTY:     return new EmptyTile();
        case TileType::MONEY:     return new MoneyTile();
        case TileType::POWERUP:   return new PowerupTile();
        case TileType::FORWARD:   return new MoveForwardTile();
        case TileType::BACKWARD:  return new MoveBackwardTile();
        case TileType::LIFE_EVENT:return new LifeEventTile();
        default:                  return new EmptyTile();
    }
}

/**
 * @brief Activates the EmptyTile. Does nothing.
 * @param player Reference to the Player interacting with this tile.
 */
void EmptyTile::activate(Player& player) {
    // Contemplate life.
}

/**
 * @brief Activates the MoneyTile. Adds $100 to the player's balance.
 * @param player Reference to the Player interacting with this tile.
 */
void MoneyTile::activate(Player& player) {
    float moneyAdd = 100 * player.getIncomePercent();
    QString moneyLabel = "$" + QString::number(moneyAdd);
    player.addMoney(moneyAdd);

    MoneyPopupDialog popup;
    popup.updateText(moneyLabel);
    popup.exec();  // Modal dialog
}

/**
 * @brief Activates the PowerupTile. Grants the player a random power-up and shows a basic text popup.
 * @param player Reference to the Player interacting with this tile.
 */
void PowerupTile::activate(Player& player) {
    player.giveRandomPowerup();

    QMessageBox* msgBox = new QMessageBox();
    msgBox->setWindowTitle("Power-up!");
    msgBox->setText("You received a power up! Click the power up button to view your power ups.");
    msgBox->show();
}

/**
 * @brief Activates the MoveForwardTile. Moves the player forward 1-5 steps.
 * @param player Reference to the Player interacting with this tile.
 */
void MoveForwardTile::activate(Player& player) {
    int steps = rand() % 5 + 1;
    player.moveForward(steps);

    QString baseMessage;
    switch (steps) {
    case 1: baseMessage = "Your parents send their support."; break;
    case 2: baseMessage = "You fixed all the bugs during an all-nighter!"; break;
    case 3: baseMessage = "Stack Exchange came through with perfect docs."; break;
    case 4: baseMessage = "You're typing faster than light!"; break;
    case 5: baseMessage = "It worked on the first try?!"; break;
    }

    // Build the full message
    QString fullMessage = "Nice!\n" + baseMessage + "\nYou move forward " + QString::number(steps) + " spaces!";

    // Show popup
    TextPopupDialog popup;
    popup.updateText(fullMessage);
    popup.exec();  // Modal popup
}

/**
 * @brief Activates the MoveBackwardTile. Moves the player backward 1-5 steps.
 * @param player Reference to the Player interacting with this tile.
 */
void MoveBackwardTile::activate(Player& player) {
    int steps = rand() % 5 + 1;
    player.moveBackward(steps);

    QString baseMessage;
    switch (steps) {
    case 1: baseMessage = "You did your partner’s part of the group project."; break;
    case 2: baseMessage = "You missed the project meeting!"; break;
    case 3: baseMessage = "You got sick, and your prof said 'tough luck'."; break;
    case 4: baseMessage = "Endless bugs... try reinstalling VSCode?"; break;
    case 5:
        baseMessage = "You deleted System32. RIP. Also lost $50.";
        player.addMoney(-50);
        break;
    }

    // Build the full message
    QString fullMessage = "Oh no!\n" + baseMessage + "\nYou move backward " + QString::number(steps) + " spaces!";

    // Show popup
    TextPopupDialog popup;
    popup.updateText(fullMessage);
    popup.exec();
}


/**
 * @brief Activates the LifeEventTile. Simulates a life event like exams or interviews with random outcomes (Depends on Dice Roll).
 * @param player Reference to the Player interacting with this tile.
 */
void LifeEventTile::activate(Player& player) {
    int roll = rand() % 3 + 1;
    LifeEventDialog popup;
    if (roll == 1) {
        QString event = "You will have a job interview!\nRoll the dice to find out your fate!";
        popup.updateText(event);
        if (popup.exec() == QDialog::Accepted) {
            roll = popup.getDiceRoll(); // Updates roll based on dialog button
            popup.disableDiceRoll();
        }
        if (roll <= 2) {
            event = "You failed :(\nNothing happens. All you've lost is your pride...";
            popup.updateText(event);
            popup.exec();
        } else if (roll <= 4) {
            event = "Congrats you passed!\nYou're on pace to becoming\n a fine software dev!\n+15% income.";
            popup.updateText(event);
            popup.exec();
            player.modifyIncome(0.15f);
        } else {
            event = "You're the new CEO! +30% income.";
            popup.updateText(event);
            popup.exec();
            player.modifyIncome(0.30f);
        }
    } else if (roll == 2) {
        QString event = "You're taking your final exam for CSCI 3010!\nI hope you studied...\nRoll the dice to find out your fate!";
        popup.updateText(event);
        if (popup.exec() == QDialog::Accepted) {
            roll = popup.getDiceRoll();
            popup.disableDiceRoll();
        }
        if (roll <= 2) {
            event = "You got a 15% on your final :(\n Not even the curve will save this.\nMove back 2 spaces.";
            popup.updateText(event);
            popup.exec();
            player.moveBackward(2);
        } else if (roll <= 4) {
            event = "You passed!\nYou know what they say \"C's get degrees!\"\nNothing happens.";
            popup.updateText(event);
            popup.exec();
        } else {
            event = "You got an A!\nAll of that studying finally paid off!\nMove forward 2 spaces to celebrate!";
            popup.updateText(event);
            popup.exec();
            player.moveForward(2);
        }
    } else {
        QString event = "Your project is due today at 11:59PM.\nRoll the dice to find out your fate!";
        popup.updateText(event);
        if (popup.exec() == QDialog::Accepted) {
            roll = popup.getDiceRoll();
            popup.disableDiceRoll();
        }
        if (roll <= 2) {
            event = "You failed to turn in your project on time :(\nYou may want to cut back on gaming this month\nMove back 2 spaces!";
            popup.updateText(event);
            popup.exec();
            player.moveBackward(2);
        } else if (roll <= 4) {
            event = "Congrats you turned in your project on time!\nUnfortunately we don't reward mediocracy\nso nothing happens.";
            popup.updateText(event);
            popup.exec();
        } else {
            event = "Wow you went above and beyond!\nYou turned in your project 5 months early\nand left time for your other assignments!\nMove forward 2 spaces!";
            popup.updateText(event);
            popup.exec();
            player.moveForward(2);
        }
    }
}
