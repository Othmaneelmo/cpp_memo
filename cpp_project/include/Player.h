#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Enums.h"

class Rubis;

class Player {
private:
    std::string name;
    Side side;
    int nRubies;
    bool active;
    mutable bool displayMode; // true for endOfGame

public:
    Player(const std::string& name, Side side);
    std::string getName() const;
    void setActive(bool active);
    bool isActive() const;
    int getNRubies() const;
    void addRubis(const Rubis& rubis);
    void setDisplayMode(bool endOfGame);
    Side getSide() const;
    void setSide(Side side);
    friend std::ostream& operator<<(std::ostream& os, const Player& player);
};

#endif
