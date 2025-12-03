#ifndef DECKFACTORY_H
#define DECKFACTORY_H

#include <vector>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <ctime>

template <typename C>
class DeckFactory {
protected:
    std::vector<C*> deck;
    size_t currentIndex;

public:
    DeckFactory() : currentIndex(0) {
        // Seed random for std::random_shuffle
        static bool seeded = false;
        if (!seeded) {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
            seeded = true;
        }
    }
    
    virtual ~DeckFactory() {
        for (auto* item : deck) {
            delete item;
        }
    }

    void shuffle() {
        // PDF explicitly requires std::random_shuffle
        // Note: deprecated in C++14, removed in C++17
        // If using C++17+, may need to use std::shuffle instead
        #if __cplusplus >= 201703L
            // C++17 or later - use std::shuffle
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(deck.begin(), deck.end(), g);
        #else
            // C++14 or earlier - use std::random_shuffle as required
            std::random_shuffle(deck.begin(), deck.end());
        #endif
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
