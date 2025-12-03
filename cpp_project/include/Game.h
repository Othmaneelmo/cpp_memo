#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Player.h"
#include <vector>
#include <iostream>

class Game {
private:
    Board board;
    std::vector<Player> players;
    int round;
    const Card* previousCard;
    const Card* currentCard;
    bool expertDisplay;
    
    // For Walrus ability
    bool hasBlockedCard;
    Letter blockedLetter;
    Number blockedNumber;

    friend class Rules;

public:
    Game(CardDeck& deck, bool expertDisplay = false);
    int getRound() const;
    void addPlayer(const Player& player);
    Player& getPlayer(Side side);
    const Card* getPreviousCard() const;
    const Card* getCurrentCard() const;
    void setCurrentCard(const Card* card);
    Card* getCard(const Letter& l, const Number& n);
    void setCard(const Letter& l, const Number& n, Card* card);
    void nextRound();
    bool isExpertDisplay() const;
    bool turnFaceUp(const Letter& l, const Number& n);
    bool turnFaceDown(const Letter& l, const Number& n); // Added wrapper
    const std::vector<Player>& getPlayers() const;
    void setPlayerActive(Side side, bool active);
    void addRubisToPlayer(Side side, const Rubis& rubis);
    const Board& getBoard() const { return board; }
    std::vector<Player>& getPlayersMutable() { return players; }

    // Expert Logic Helpers
    void setBlockedCard(Letter l, Number n);
    bool isBlocked(Letter l, Number n) const;
    void resetBlocked();
    void swapCards(Letter l1, Number n1, Letter l2, Number n2);
    
    // Start of round peek helper
    std::vector<std::pair<Letter, Number>> getSightLocations(Side side) const;

    friend std::ostream& operator<<(std::ostream& os, const Game& game);
};

#endif