#include "shopdialog.h"
#include "mainwindow.h"
#include "ui_shopdialog.h"

ShopDialog::ShopDialog(Player* player, MainWindow* mainWindow, QWidget *parent)
    : QDialog(parent), ui(new Ui::ShopDialog), player(player), mainWindowRef(mainWindow)
{
    ui->setupUi(this);
}

ShopDialog::~ShopDialog()
{
    delete ui;
}

/**
 * @brief Handles the logic when the player clicks the Buy button in the shop.
 *
 * Deducts the cost from the player's money if affordable, adds a power to their inventory,
 * shows a notification, and updates the UI. Otherwise, shows a failure message.
 */
void ShopDialog::on_buyMove_clicked()
{
    int cost = 1000;
    if (player->getMoney() >= cost) {
        player->addMoney(-cost);
        player->addPower(new MovePower());
        player->notify("You bought an energy drink.");
        mainWindowRef->updatePlayerUI();
    } else {
        player->notify("Dang. Not enough money for an energy drink.");
    }
}

void ShopDialog::on_buySteal_clicked()
{
    int cost = 20000;
    if (player->getMoney() >= cost) {
        player->addMoney(-cost);
        player->addPower(new StealPower());
        player->notify("You bought a really shady html script.");
        mainWindowRef->updatePlayerUI();
    } else {
        player->notify("Not enouugh money for that. Stick to inspect element.");
    }
}

void ShopDialog::on_buyMoney_clicked()
{
    int cost = 20000;
    if (player->getMoney() >= cost) {
        player->addMoney(-cost);
        player->addPower(new MoneyPower());
        player->notify("Nepotism rules! You got yourself a connection.");
        mainWindowRef->updatePlayerUI();
    } else {
        player->notify("Not enough cash... stick to Linkedin.");
    }
}

/**
 * @brief Handles the logic when the player clicks the Buy button.
 *
 * Deducts the ticket cost from the player's money and randomly determines a prize.
 * Prizes range from $500 to $10000 based on a 1–10 roll. Displays the result
 * and updates the UI with the new money value.
 */
void ShopDialog::on_buyLottery_clicked()
{
    int cost = 3000;
    if (player->getMoney() >= cost) {
        player->addMoney(-cost);

        int roll = rand() % 10 + 1;
        int reward = 0;

        if (roll <= 5) {
            reward = 500;
        } else if (roll <= 8) {
            reward = 1500;
        } else if (roll == 9) {
            reward = 3000;
        } else {
            reward = 10000;
        }

        player->addMoney(reward);

        QString resultText =
            "You bought a lottery ticket!\n"
            "You input the numbers and...\n\n"
            "Your ticket won $" + QString::number(reward) + "!";

        player->notify(resultText);
        mainWindowRef->updatePlayerUI();
    } else {
        player->notify("You don't have enough money to buy a lottery ticket.");
    }
}

/**
 * @brief Handles the logic when the player clicks the Buy button.
 *
 * Deducts $85000 from the player's funds and increases their passive income multiplier by 500%.
 * Shows a confirmation message and updates the UI accordingly.
 */
void ShopDialog::on_buyStore_clicked()
{
    int cost = 85000;
    if (player->getMoney() >= cost) {
        player->addMoney(-cost);
        player->modifyIncome(5.0f);
        player->notify("You hand the cashier 85k.\nThis store is yours now.\nYour passive income skyrocketed by 500%.");
        mainWindowRef->updatePlayerUI();
    } else {
        player->notify("You can't afford a store yet. Keep hustling.");
    }
}
