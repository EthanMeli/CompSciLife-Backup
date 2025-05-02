#include "gamelogic.h"
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <random>

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
        int opponentMoney = opponent->getMoney();
        if (opponentMoney > 0) {
            int amount = opponentMoney * 0.3;
            opponent->addMoney(-amount);
            player.addMoney(amount);
            player.notify("Used Bank Account Hack! Stole 30% of your opponent's cash: $" + QString::number(amount));
        } else {
            opponent->addMoney(-3000);
            player.notify("Used Bank Account Hack!\nYour opponent had no money...\nSo you added $3000 to their debt.");
        }
    } else {
        player.notify("No opponent to steal from!");
    }

    player.setUsedPowerupThisTurn(false); // No movement needed
}

QString StealPower::name() const {
    return "Bank Account Hack";
}

/**
 * @brief Applies the Time Freeze power up.
 * @param player The player using the power up.
 * Skips the opponent's next turn.
 */
void SkipPower::apply(Player& player) {
    Player* opponent = player.getOpponent();
    if (opponent) {
        opponent->setSkipNextTurn(true);
        player.notify("Used Time Freeze! Your opponent's next turn will be skipped.");
    } else {
        player.notify("No opponent to skip!");
    }

    player.setUsedPowerupThisTurn(false); // No movement
}

QString SkipPower::name() const {
    return "Time Freeze";
}

/**
 * @brief Applies the Speed Boost power up.
 * @param player The player using the power up.
 * Moves the player forward by 6 spaces.
 */
void SuperMovePower::apply(Player& player) {
    player.moveForward(6);
    player.notify("Used Speed Boost! Zoomed forward 6 spaces!");
    player.setUsedPowerupThisTurn(true);
}

QString SuperMovePower::name() const {
    return "Speed Boost";
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

/**
 * @brief Adds a power-up to the player's inventory.
 * @param p Pointer to the PowerUp to add. Ownership is transferred to the player.
 */
void Player::addPower(PowerUp* p) {
    powerups.emplace_back(p);
}

// ------------------------- PLAYER METHODS -------------------------

/**
 * @brief Constructs a new Player.
 * @param name The name of the player.
 */
Player::Player(QString name) : name(name) {}

/**
 * @brief Gets the player's name.
 * @return Player's name.
 */
QString Player::getName() const {
    return name;
}

/**
 * @brief Sets the player's board position.
 * @param pos The new board index.
 */
void Player::setPosition(int pos) {
    position = pos;
}

/**
 * @brief Checks if the player is finished.
 * @return True if finished, false otherwise.
 */
bool Player::isFinished() {
    return finished;
}

/**
 * @brief Gets the player's current income percentage.
 * @return Income percent as a float (e.g., 115.0 for 1.15x).
 */
float Player::getIncomePercent() const {
    return incomeMultiplier * 100.0f;
}

/**
 * @brief Adds money to the player's total.
 * @param amount The amount to add.
 */
void Player::addMoney(int amount) {
    money += amount;
}

/**
 * @brief Gets the player's current money.
 * @return The player's money amount.
 */
int Player::getMoney() const {
    return money;
}

/**
 * @brief Provides mutable access to the player's power-up list.
 * @return Reference to the vector of unique_ptrs holding the player's power-ups.
 */
std::vector<std::unique_ptr<PowerUp>>& Player::accessPowerups() {
    return powerups;
}

/**
 * @brief Modifies the player's income multiplier.
 * @param percent The percent change to apply.
 */
void Player::modifyIncome(float percent) {
    incomeMultiplier += percent;
}

/**
 * @brief Sets whether the player's next turn should be skipped.
 * @param skip True to skip the next turn, false otherwise.
 */
void Player::setSkipNextTurn(bool skip) {
    skipTurn = skip;
}

/**
 * @brief Marks the player as having finished the game.
 */
void Player::markFinished() {
    finished = true;
}

/**
 * @brief Returns whether the player's turn should be skipped.
 * @return True if the next turn should be skipped.
 */
bool Player::shouldSkipTurn() const {
    return skipTurn;
}

/**
 * @brief Clears the skip turn flag for the player.
 */
void Player::clearSkipTurn() {
    skipTurn = false;
}

/**
 * @brief Gets the player's current position on the board.
 * @return The current tile index.
 */
int Player::getPosition() const {
    return position;
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
    int r = rand() % 5; // Now 5 possible powerups
    std::unique_ptr<PowerUp> newPower;

    switch (r) {
    case 0:
        newPower = std::make_unique<MovePower>();
        break;
    case 1:
        newPower = std::make_unique<StealPower>();
        break;
    case 2:
        newPower = std::make_unique<MoneyPower>();
        break;
    case 3:
        newPower = std::make_unique<SkipPower>();
        break;
    case 4:
        newPower = std::make_unique<SuperMovePower>();
        break;
    }

    notify("Gained Power-Up: " + newPower->name() + "!");
    powerups.push_back(std::move(newPower));
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
 * @brief Displays a message popup. Auto-closes if the player is AI.
 * @param message The message to show.
 */
void Player::notify(const QString& message) {
    QMessageBox* box = new QMessageBox(QMessageBox::Information, name, message, QMessageBox::Ok);
    box->setAttribute(Qt::WA_DeleteOnClose); // Clean
    box->show();

    if (name == "CPU") {
        // Auto close after 1.5 seconds for AI players
        QTimer::singleShot(1500, box, &QMessageBox::accept);
    }
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

/**
 * @brief Sets whether the player has used a power up that involved movement this turn.
 * @param moved True if the player moved due to a power up, false otherwise.
 */
void Player::setUsedPowerupThisTurn(bool moved) {
    usedPowerupThisTurn = moved;
}

/**
 * @brief Checks if the player used a power-up to move this turn.
 * @return True if a movement power-up was used, false otherwise.
 */
bool Player::didUsePowerupMove() const {
    return usedPowerupThisTurn;
}
