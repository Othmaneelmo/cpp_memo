#include "RubisDeck.h"

RubisDeck* RubisDeck::instance = nullptr;

RubisDeck::RubisDeck() {
    // 3 with 1, 2 with 2, 1 with 3, 1 with 4
    for (int i = 0; i < 3; ++i) deck.push_back(new Rubis(1));
    for (int i = 0; i < 2; ++i) deck.push_back(new Rubis(2));
    deck.push_back(new Rubis(3));
    deck.push_back(new Rubis(4));
    shuffle();
}

RubisDeck& RubisDeck::make_RubisDeck() {
    if (!instance) {
        instance = new RubisDeck();
    }
    return *instance;
}
