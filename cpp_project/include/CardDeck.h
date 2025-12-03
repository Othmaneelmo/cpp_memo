#ifndef CARDDECK_H
#define CARDDECK_H

#include "DeckFactory.h"
#include "Card.h"

class CardDeck : public DeckFactory<Card> {
private:
    static CardDeck* instance;
    CardDeck();

public:
    static CardDeck& make_CardDeck();
};

#endif
