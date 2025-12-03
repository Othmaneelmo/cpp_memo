#ifndef RULES_H
#define RULES_H

#include "Game.h"
#include "Player.h"
#include "Enums.h"
#include "Card.h" 

class Rules {
private:
    bool expertRules;

public:
    // Constructor initializes the expert mode flag
    Rules(bool expertRules = false) : expertRules(expertRules) {}

    // core game logic methods implemented in Rules.cpp
    bool isValid(const Game& game) const;
    bool gameOver(const Game& game) const;
    bool roundOver(const Game& game) const;
    const Player& getNextPlayer(const Game& game) const;
    
    // Expert rule logic
    ExpertEffect applyExpertRule(Game& game, const Card& card, Player& currentPlayer);
};

#endif