#ifndef CARD_H
#define CARD_H

#include "Enums.h"
#include <string>

class Card {
private:
    FaceAnimal animal;
    FaceBackground background;
    Card(FaceAnimal animal, FaceBackground background); // private constructor
    friend class CardDeck;

public:
    std::string operator()(int row) const;
    int getNRows() const { return 3; }
    operator FaceAnimal() const;
    operator FaceBackground() const;
};

#endif
