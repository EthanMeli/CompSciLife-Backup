#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QString>
#include <vector>
#include <memory>

// Forward declarations
class PowerUp;
class Tile;

// ------------------------- Player -------------------------
class Player {
public:
    Player(QString name);

    // Core
    void addMoney(int amount);
    void modifyIncome(float percent);
    void moveForward(int steps);
    void moveBackward(int steps);
    void giveRandomPowerup();
    void notify(const QString& message);

    // Power ups
    const std::vector<std::unique_ptr<PowerUp>>& getPowerups() const;
    void usePowerup(int index);
    void setUsedPowerupThisTurn(bool moved);
    bool didUsePowerupMove() const;
    void addPower(PowerUp* p);

    // Turn management
    void setSkipNextTurn(bool skip);
    bool shouldSkipTurn() const;
    void clearSkipTurn();

    // Position
    int getPosition() const;
    void setPosition(int pos);
    bool isFinished();
    void resetFinished() { finished = false; };
    void markFinished();

    // Player Stuff
    QString getName() const;
    int getMoney() const;
    float getIncomePercent() const;
    std::vector<std::unique_ptr<PowerUp>>& accessPowerups();

    // Opponent
    void setOpponent(Player* opponent);
    Player* getOpponent() const;

private:
    QString name;
    int position = 0;
    int money = 0;
    float incomeMultiplier = 1.0f;
    bool usedPowerupThisTurn = false;
    bool skipTurn = false;
    bool finished = false;

    std::vector<std::unique_ptr<PowerUp>> powerups;
    Player* opponent = nullptr;
};

// ------------------------- PowerUp -------------------------
class PowerUp {
public:
    virtual ~PowerUp() {}
    virtual void apply(Player& player) = 0;
    virtual QString name() const = 0;
};

class MovePower : public PowerUp {
public:
    void apply(Player& player) override;
    QString name() const override;
};

class StealPower : public PowerUp {
public:
    void apply(Player& player) override;
    QString name() const override;
};

class MoneyPower : public PowerUp {
public:
    void apply(Player& player) override;
    QString name() const override;
};

class SkipPower : public PowerUp {
public:
    void apply(Player& player) override;
    QString name() const override;
};

class SuperMovePower : public PowerUp {
public:
    void apply(Player& player) override;
    QString name() const override;
};

// ------------------------- Board (placeholder) -------------------------
class Board {
private:
    std::vector<Tile*> tiles;
};

// ------------------------- LifeManager (Singleton) -------------------------
class LifeManager {
public:
    static LifeManager* instance();

private:
    LifeManager();
    static LifeManager* _instance;
};

#endif // GAMELOGIC_H
