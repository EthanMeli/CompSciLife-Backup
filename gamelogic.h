#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QString>
#include <vector>
#include <memory>

class PowerUp;
class Tile;
class Player;

class Player {
public:
    Player(QString name);

    void addMoney(int amount);
    void subtractMoney(int amount);
    void modifyIncome(float percent);
    void moveForward(int steps);
    void moveBackward(int steps);
    void giveRandomPowerup();
    void notify(const QString& message);

    const std::vector<std::unique_ptr<PowerUp>>& getPowerups() const;

    void usePowerup(int index);
    QString getName() const { return name; }
    int getMoney() const { return money; }
    float getIncomePercent() const { return incomeMultiplier * 100.0f; }
    int getPosition() const { return position; }
    void setPosition(int pos) { position = pos; }

    void setOpponent(Player* opponent);
    Player* getOpponent() const;

    bool finished = false;
    bool isFinished() const { return finished; }
    void markFinished() { finished = true; }

    void setUsedPowerupThisTurn(bool moved);
    bool didUsePowerupMove() const;

private:
    QString name;
    int position = 0;
    int money = 0;
    float incomeMultiplier = 1.0f;
    bool usedPowerupThisTurn = false;
    std::vector<std::unique_ptr<PowerUp>> powerups;
    Player* opponent = nullptr;
};

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

class Board {
    public:

    private:
        std::vector<Tile*> tiles;
};

// Singleton LifeManager
class LifeManager {
public:
    static LifeManager* instance();

private:
    LifeManager();
    static LifeManager* _instance;
};
#endif // GAMELOGIC_H
