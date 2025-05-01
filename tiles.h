#ifndef TILES_H
#define TILES_H

#include "gamelogic.h"
#include <QGraphicsScene>

enum class TileType {
    EMPTY,
    MONEY,
    POWERUP,
    FORWARD,
    BACKWARD,
    LIFE_EVENT,
    SUPER_MONEY
};

class Tile {
public:
    virtual ~Tile() {}
    virtual void activate(Player& player) = 0;
    virtual QColor getColor() { return Qt::white; }
    virtual QString getType() { return "Tile"; }
};

// Tile Types
class EmptyTile : public Tile {
public:
    void activate(Player& player) override;
    QString getType() override { return "EmptyTile"; }
};

class MoneyTile : public Tile {
public:
    void activate(Player& player) override;
    QColor getColor() override { return QColor("#a8e6a1"); }
    QString getType() override { return "MoneyTile"; }
};

class PowerupTile : public Tile {
public:
    void activate(Player& player) override;
    QColor getColor() override { return QColor("#c7a8e6"); }
    QString getType() override { return "PowerupTile"; }
};

class MoveForwardTile : public Tile {
public:
    void activate(Player& player) override;
    QColor getColor() override { return QColor("#ffe066"); }
    QString getType() override { return "MoveForwardTile"; }
};

class MoveBackwardTile : public Tile {
public:
    void activate(Player& player) override;
    QColor getColor() override { return QColor("#f4a6a6"); }
    QString getType() override { return "MoveBackwardTile"; }
};

class LifeEventTile : public Tile {
public:
    void activate(Player& player) override;
    QColor getColor() override { return QColor("#99ccee"); }
    QString getType() override { return "LifeEventTile"; }
};

class SuperMoneyTile : public Tile {
public:
    void activate(Player& player) override;
    QColor getColor() override { return QColor("#66aa88"); }
    QString getType() override { return "SuperMoneyTile"; }
};

// Factory
class TileFactory {
public:
    static Tile* createTile(TileType type);
};

#endif // TILES_H
