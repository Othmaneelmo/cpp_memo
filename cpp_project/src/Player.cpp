#include "Player.h"
#include "Rubis.h"
#include <iostream>

Player::Player(const std::string& name, Side side) : name(name), side(side), nRubies(0), active(true), displayMode(false) {}

std::string Player::getName() const {
    return name;
}

void Player::setActive(bool active) {
    this->active = active;
}

bool Player::isActive() const {
    return active;
}

int Player::getNRubies() const {
    return nRubies;
}

void Player::addRubis(const Rubis& rubis) {
    nRubies += static_cast<int>(rubis);
}

void Player::setDisplayMode(bool endOfGame) {
    displayMode = endOfGame;
}

Side Player::getSide() const {
    return side;
}

void Player::setSide(Side side) {
    this->side = side;
}

std::string sideToString(Side s) {
    switch (s) {
        case Side::top: return "top";
        case Side::bottom: return "bottom";
        case Side::left: return "left";
        case Side::right: return "right";
        default: return "";
    }
}

std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << player.name << ": ";
    if (player.displayMode) {
        os << player.nRubies << " rubis";
    } else {
        os << sideToString(player.side);
        if (player.active) {
            os << " (active)";
        }
    }
    return os;
}
