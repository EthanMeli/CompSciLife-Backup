#include "gamelogic.h"
#include <QDebug>
#include <random>
#include <QMessageBox>

// ------------------------- POWER UP METHODS -------------------------

/**
 * @brief Applies the Job Promotion power up.
 * @param player The player using the power up.
 * Moves the player forward by 3 spaces and notifies them.
 */
void MovePower::apply(Player& player) {
    player.moveForward(3);
    player.modifyIncome(0.5f);
    player.setUsedPowerupThisTurn(true);
    player.notify("Used Job Promotion! Moved 3 spaces and got a 50% income boost!");
}

/**
 * @brief Gets the name of the power up.
 * @return The name of the power up.
 */
// Returns the name of the power.
QString MovePower::name() const {
    return "Job Promotion";
}

/**
 * @brief Applies the Bank Account Hack power up.
 * @param player The player using the power up.
 * Adds $50 to the player's money.
 */
void StealPower::apply(Player& player) {
    Player* opponent = player.getOpponent();
    if (opponent) {
        int amount = opponent->getMoney() * 0.3;
        opponent->subtractMoney(amount);
        player.addMoney(amount);
        player.notify("Used Bank Account Hack! Stole 30% of your opponent's cash: $" + QString::number(amount));
    } else {
        player.notify("No opponent to steal from!");
    }
    player.setUsedPowerupThisTurn(false); // No movement needed
}

QString StealPower::name() const {
    return "Bank Account Hack";
}

/**
 * @brief Applies the Energy Drink power up.
 * @param player The player using the power up.
 * Adds $100 to the player's money.
 */
void MoneyPower::apply(Player& player) {
    player.addMoney(1000);
    player.notify("Used Energy Drink! You worked all night and gained $1000.");
    player.setUsedPowerupThisTurn(false); // No movement needed
}

QString MoneyPower::name() const {
    return "Energy Drink";
}

// ------------------------- PLAYER METHODS -------------------------

/**
 * @brief Constructs a new Player.
 * @param name The name of the player.
 */
Player::Player(QString name) : name(name) {}

/**
 * @brief Adds money to the player's total.
 * @param amount The amount to add.
 */
void Player::addMoney(int amount) {
    money += amount;
}

/**
 * @brief Subtracts money from the player's total.
 * @param amount The amount to subtract.
 */
void Player::subtractMoney(int amount) {
    money = std::max(0, money - amount);
}

/**
 * @brief Modifies the player's income multiplier.
 * @param percent The percent change to apply.
 */
void Player::modifyIncome(float percent) {
    incomeMultiplier += percent;
}

/**
 * @brief Moves the player forward by a number of steps.
 * @param steps The number of steps to move forward.
 */
void Player::moveForward(int steps) {
    position += steps;
}

/**
 * @brief Moves the player backward by a number of steps, but not below 0.
 * @param steps The number of steps to move backward.
 */
void Player::moveBackward(int steps) {
    position = std::max(0, position - steps);
}

/**
 * @brief Gives the player a random power up.
 * No parameters or return value.
 */
void Player::giveRandomPowerup() {
    int r = rand() % 3;
    switch (r) {
    case 0:
        powerups.push_back(std::make_unique<MovePower>());
        notify("Gained Power-Up: Move Power!");
        break;
    case 1:
        powerups.push_back(std::make_unique<StealPower>());
        notify("Gained Power-Up: Steal Power!");
        break;
    case 2:
        powerups.push_back(std::make_unique<MoneyPower>());
        notify("Gained Power-Up: Money Power!");
        break;
    }
}

/**
 * @brief Gets the list of power ups the player has.
 * @return A const reference to the player's power up vector.
 */
const std::vector<std::unique_ptr<PowerUp>>& Player::getPowerups() const {
    return powerups;
}

/**
 * @brief Uses the power up at a given index.
 * @param index The index of the power up to use.
 */
void Player::usePowerup(int index) {
    if (index >= 0 && index < static_cast<int>(powerups.size())) {
        usedPowerupThisTurn = false;
        powerups[index]->apply(*this);
        powerups.erase(powerups.begin() + index);
    }
}

/**
 * @brief Prints a message to the debug console.
 * @param message The message to display.
 */
void Player::notify(const QString& message) {
    QMessageBox::information(nullptr, name, message);
}

/**
 * @brief Sets the opponent player.
 * @param opp Pointer to the opponent.
 */
void Player::setOpponent(Player* opp) {
    opponent = opp;
}

/**
 * @brief Gets the opponent player.
 * @return Pointer to the opponent player.
 */
Player* Player::getOpponent() const {
    return opponent;
}

void Player::setUsedPowerupThisTurn(bool moved) {
    usedPowerupThisTurn = moved;
}

bool Player::didUsePowerupMove() const {
    return usedPowerupThisTurn;
}
