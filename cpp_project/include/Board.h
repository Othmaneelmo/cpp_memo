#ifndef BOARD_H
#define BOARD_H

#include "Card.h"
#include "CardDeck.h"
#include "Enums.h"
#include "Exceptions.h"
#include <vector>
#include <string>
#include <iostream>

class Board {
private:
    std::vector<std::vector<Card*>> grid; // 5x5, but center is nullptr
    std::vector<std::vector<bool>> faceUp; // 5x5, true if face up

    int letterToIndex(Letter l) const;
    int numberToIndex(Number n) const;
    bool isValidPosition(Letter l, Number n) const;

public:
    Board(CardDeck& deck);
    ~Board();

    bool isFaceUp(const Letter& l, const Number& n) const;
    bool turnFaceUp(const Letter& l, const Number& n);
    bool turnFaceDown(const Letter& l, const Number& n);
    Card* getCard(const Letter& l, const Number& n) const;
    void setCard(const Letter& l, const Number& n, Card* card);
    void allFacesDown();

    // Added for Octopus ability
    void swapCards(const Letter& l1, const Number& n1, const Letter& l2, const Number& n2);

    friend std::ostream& operator<<(std::ostream& os, const Board& board);
};

#endif