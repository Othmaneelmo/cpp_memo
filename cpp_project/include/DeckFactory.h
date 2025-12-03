#ifndef DECKFACTORY_H
#define DECKFACTORY_H

#include <vector>
#include <algorithm>
#include <random>

template <typename C>
class DeckFactory {
protected:
    std::vector<C*> deck;
    size_t currentIndex;

public:
    DeckFactory() : currentIndex(0) {}
    virtual ~DeckFactory() {
        for (auto* item : deck) {
            delete item;
        }
    }

    void shuffle() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(deck.begin(), deck.end(), g);
        currentIndex = 0;
    }

    C* getNext() {
        if (currentIndex < deck.size()) {
            return deck[currentIndex++];
        }
        return nullptr;
    }

    bool isEmpty() const {
        return currentIndex >= deck.size();
    }
};

#endif
