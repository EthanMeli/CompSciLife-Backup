#include "tiles.h"
#include "moneypopupdialog.h"
#include "supermoneypopupdialog.h"
#include "lifeeventdialog.h"
#include "textpopupdialog.h"
#include <cstdlib>
#include <ctime>
#include <QMessageBox>
#include <QTimer>

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
        case TileType::SUPER_MONEY: return new SuperMoneyTile();
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

    if (player.getName() == "CPU") {
        popup.show();
        QTimer::singleShot(1000, &popup, &QDialog::accept);
    } else {
        popup.exec();
    }
}

/**
 * @brief Activates the SuperMoneyTile. Grants the player a huge amount of money.
 * @param player Reference to the Player interacting with this tile.
 */
void SuperMoneyTile::activate(Player& player) {
    int jackpots[] = {10000, 25000, 50000};
    int jackpot = jackpots[rand() % 3];

    QString moneyLabel = "$" + QString::number(jackpot);
    player.addMoney(jackpot);

    SuperMoneyPopupDialog popup;
    popup.updateText(moneyLabel);

    if (player.getName() == "CPU") {
        popup.show();
        QTimer::singleShot(1000, &popup, &QDialog::accept);
    } else {
        popup.exec();
    }
}

/**
 * @brief Activates the PowerupTile. Grants the player a random power-up and shows a basic text popup.
 * @param player Reference to the Player interacting with this tile.
 */
void PowerupTile::activate(Player& player) {
    player.giveRandomPowerup();
}

/**
 * @brief Activates the MoveForwardTile. Moves the player forward 1-5 steps.
 * @param player Reference to the Player interacting with this tile.
 */
void MoveForwardTile::activate(Player& player) {
    int eventRoll = rand() % 10 + 1;
    int steps = rand() % 5 + 1;
    player.moveForward(steps);

    QString baseMessage;
    switch (eventRoll) {
    case 1: baseMessage = "Your parents send their support."; break;
    case 2: baseMessage = "You fixed all the bugs during an all-nighter!"; break;
    case 3: baseMessage = "Stack Exchange came through with perfect docs."; break;
    case 4: baseMessage = "You're typing faster than light!"; break;
    case 5: baseMessage = "It worked on the first try?!"; break;
    case 6: baseMessage = "Professor curved the exam... heavily."; break;
    case 7: baseMessage = "Your resume went viral on LinkedIn. \nFreelance gig gave you $1000!"; player.addMoney(1000); break;
    case 8: baseMessage = "You optimized a O(n^50) function \ndown to O(n). TA just handed \nyou $300 in awe."; player.addMoney(300); break;
    case 9: baseMessage = "You suddenly get a scholarship for \nbeing a stellar student. Gain $5000!"; player.addMoney(5000); break;
    case 10: baseMessage = "Google is trying to bribe you with $1500 \nto join them because you're so good at CS!"; player.addMoney(1500); break;
    }

    QString fullMessage = "Nice!\n" + baseMessage + "\nYou move forward " + QString::number(steps) + " spaces!";
    TextPopupDialog popup;
    popup.updateText(fullMessage);

    if (player.getName() == "CPU") {
        popup.show();
        QTimer::singleShot(1000, &popup, &QDialog::accept);
    } else {
        popup.exec();
    }
}

/**
 * @brief Activates the MoveBackwardTile. Moves the player backward 1-5 steps.
 * @param player Reference to the Player interacting with this tile.
 */
void MoveBackwardTile::activate(Player& player) {
    int eventRoll = rand() % 20 + 1;
    int steps = rand() % 5 + 1;
    player.moveBackward(steps);

    QString baseMessage;
    switch (eventRoll) {
    case 1: baseMessage = "You did your partner’s part of the group project."; break;
    case 2: baseMessage = "You missed the project meeting!"; break;
    case 3: baseMessage = "You got sick, and your prof said 'tough luck'."; break;
    case 4: baseMessage = "Endless bugs... try reinstalling VSCode?"; break;
    case 5: baseMessage = "You deleted System32.\nRIP. $500 also flies out\nof your pocket."; player.addMoney(-500); break;
    case 6: baseMessage = "Turns out your homework is the\nexact same as another student's.\nHuh."; break;
    case 7: baseMessage = "You spilled coffee on your new\nMacbook. Repairs cost $1000."; player.addMoney(-1000); break;
    case 8: baseMessage = "You opened TikTok and lost\n3.51 hours of productivity."; break;
    case 9: baseMessage = "Your code compiles but nothing works.\nEmotional damage."; break;
    case 10: baseMessage = "You forgot to stay hydrated\nand fainted during your midterm."; break;
    case 11: baseMessage = "Your GPU exploded during training.\nReplacement cost: $2000."; player.addMoney(-2000); break;
    case 12: baseMessage = "You accidentally committed your\nprivate key to GitHub. Your bank account\n is now $5000 lighter."; player.addMoney(-5000); break;
    case 13: baseMessage = "You invested in Dogecoin at its peak.\nMarket crash wipes out $3000."; player.addMoney(-3000); break;
    case 14: baseMessage = "Laptop stolen in a coffee shop.\nYou lose $1500 buying a new one."; player.addMoney(-1500); break;
    case 15: baseMessage = "You bought a $2000 NFT. It is now worth... nothing."; player.addMoney(-2000); break;
    case 16: baseMessage = "Your project got flagged for plagiarism\nby ChatGPT detector.\nLegal fees cost $7000."; player.addMoney(-7000); break;
    case 17: baseMessage = "You got scammed by a phishing email.\nGoodbye, $9000."; player.addMoney(-9000); break;
    case 18: baseMessage = "Your landlord raised rent retroactively.\nPay $10000 or get evicted."; player.addMoney(-10000); break;
    case 19: baseMessage = "You accidentally bought a year's\nsupply of GPUs on your mom’s credit card.\nShe makes you pay back $15000."; player.addMoney(-15000); break;
    case 20: baseMessage = "North Korea just hacked you for no reason.\nRansomware hit. You lose $50000."; player.addMoney(-50000); break;
    }

    QString fullMessage = "Oh no!\n" + baseMessage + "\nYou move backward " + QString::number(steps) + " spaces!";
    TextPopupDialog popup;
    popup.updateText(fullMessage);

    if (player.getName() == "CPU") {
        popup.show();
        QTimer::singleShot(1000, &popup, &QDialog::accept);
    } else {
        popup.exec();
    }
}

/**
 * @brief Activates the LifeEventTile. Simulates a life event like exams or interviews with random outcomes (Depends on Dice Roll).
 * @param player Reference to the Player interacting with this tile.
 */
void LifeEventTile::activate(Player& player) {
    bool isCPU = (player.getName() == "CPU");
    int eventRoll = rand() % 6 + 1;
    int roll = rand() % 6 + 1;
    QString event;

    LifeEventDialog popup;

    auto showPopupForCPU = [&](const QString& msg) {
        popup.updateText(msg);
        popup.setDiceRollManually(roll);
        popup.show();
        QTimer::singleShot(1000, &popup, &QDialog::accept);
    };

    auto showPopupForHuman = [&](const QString& msg) {
        popup.updateText(msg);
        if (popup.exec() == QDialog::Accepted) {
            roll = popup.getDiceRoll();
            popup.disableDiceRoll();
        }
    };

    switch (eventRoll) {
    case 1:
        event = "You will have a job interview!\nRoll the dice to find out your fate!";
        isCPU ? showPopupForCPU(event) : showPopupForHuman(event);
        if (roll <= 2)
            event = "You failed :(\nNothing happens. All you've lost is your pride...";
        else if (roll <= 4) {
            event = "Congrats you passed!\nYou're on pace to becoming\na fine software dev!\n+15% income.";
            player.modifyIncome(0.15f);
        } else {
            event = "You're the new CEO! +30% income.";
            player.modifyIncome(0.30f);
        }
        break;

    case 2:
        event = "You're taking your final exam for CSCI 3010!\nI hope you studied...\nRoll the dice to find out your fate!";
        isCPU ? showPopupForCPU(event) : showPopupForHuman(event);
        if (roll <= 2) {
            event = "You got a 15% on your final :(\nNot even the curve will save this.\nMove back 2 spaces.";
            player.moveBackward(2);
        } else if (roll <= 4)
            event = "You passed!\nYou know what they say, \"C's get degrees!\"\nNothing happens.";
        else {
            event = "You got an A!\nAll of that studying finally paid off!\nMove forward 2 spaces!";
            player.moveForward(2);
        }
        break;

    case 3:
        event = "Your project is due today at 11:59PM.\nRoll the dice to find out your fate!";
        isCPU ? showPopupForCPU(event) : showPopupForHuman(event);
        if (roll <= 2) {
            event = "You failed to turn in your project on time :(\nCut back on gaming maybe.\nMove back 2 spaces!";
            player.moveBackward(2);
        } else if (roll <= 4)
            event = "You turned in your project on time.\nNo extra reward for doing your job.\nNothing happens.";
        else {
            event = "Wow! You turned it in 5 months early!\nMove forward 2 spaces!";
            player.moveForward(2);
        }
        break;

    case 4:
        event = "Your friend invites you to a hangout.\nRoll the dice to see how it goes!";
        isCPU ? showPopupForCPU(event) : showPopupForHuman(event);
        if (roll <= 2) {
            event = "You got wasted and now you're hungover.\nYour friends ran away, you pay for everything..\nMove back 1 space, pay $1000.";
            player.moveBackward(1);
            player.addMoney(-1000);
        } else if (roll <= 4)
            event = "It was an alright hangout.\nNothing drastic happened.";
        else {
            event = "You got a well-deserved break!\nYou're energized.\nMove forward 1 space.";
            player.moveForward(1);
        }
        break;

    case 5:
        event = "You got an internship offer!\nRoll to see which company...";
        isCPU ? showPopupForCPU(event) : showPopupForHuman(event);
        if (roll <= 2) {
            event = "Unpaid crypto internship...\nThey pay you in exposure.\nMove back 1 space.";
            player.moveBackward(1);
        } else if (roll <= 4) {
            event = "Internship at a dev shop.\nGood experience, no raise.\n+10% income.";
            player.modifyIncome(0.10f);
        } else {
            event = "FAANG internship! Free hoodie and $500.\n+25% income!";
            player.modifyIncome(0.25f);
            player.addMoney(500);
        }
        break;

    case 6:
        event = "It's group presentation day!\nRoll the dice!";
        isCPU ? showPopupForCPU(event) : showPopupForHuman(event);
        if (roll <= 2) {
            event = "No one showed up but you.\nYou froze on stage.\nMove back 2 spaces.";
            player.moveBackward(2);
        } else if (roll <= 4)
            event = "You mumbled through your slides.\nPassable. Nothing happens.";
        else {
            event = "Your presentation went viral!\nThe prof clapped.\nMove forward 2 spaces!";
            player.moveForward(2);
        }
        break;
    }

    popup.updateText(event);

    if (!isCPU) {
        popup.exec();  // Let players see the result (Not AI)
    } else {
        popup.show();  // Auto close after 1 second because AIs dont have hands to close dialogs
        QTimer::singleShot(1000, &popup, &QDialog::accept);
    }
}


