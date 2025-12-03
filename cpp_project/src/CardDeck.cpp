#include "CardDeck.h"
#include <algorithm>

CardDeck* CardDeck::instance = nullptr;

CardDeck::CardDeck() {
    // Create all 25 combinations of animals and backgrounds
    for (int a = 0; a < 5; ++a) {
        for (int b = 0; b < 5; ++b) {
            FaceAnimal animal = static_cast<FaceAnimal>(a);
            FaceBackground background = static_cast<FaceBackground>(b);
            deck.push_back(new Card(animal, background));
        }
    }
    // Shuffle the deck
    shuffle();
}

CardDeck& CardDeck::make_CardDeck() {
    if (instance == nullptr) {
        instance = new CardDeck();
    }
    return *instance;
}
