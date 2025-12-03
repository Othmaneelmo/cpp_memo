#include "Rules.h"
#include "Game.h"
#include "Card.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

bool Rules::isValid(const Game& game) const {
    if (!game.getPreviousCard() || !game.getCurrentCard()) return true; // first card always valid
    const Card* prev = game.getPreviousCard();
    const Card* curr = game.getCurrentCard();
    return (FaceAnimal)*prev == (FaceAnimal)*curr || (FaceBackground)*prev == (FaceBackground)*curr;
}

bool Rules::gameOver(const Game& game) const {
    return game.getRound() >= 7;
}

bool Rules::roundOver(const Game& game) const {
    int activeCount = 0;
    for (const auto& p : game.getPlayers()) {
        if (p.isActive()) ++activeCount;
    }
    return activeCount <= 1;
}

const Player& Rules::getNextPlayer(const Game& game) const {
    static size_t index = 0;
    const auto& players = game.getPlayers();
    if (players.empty()) throw std::runtime_error("No players");

    // Loop to find next active player
    for (size_t i = 0; i < players.size(); ++i) {
        size_t idx = (index + i) % players.size();
        if (players[idx].isActive()) {
            index = (idx + 1) % players.size(); // Advance index for next call
            return players[idx];
        }
    }
    throw std::runtime_error("No active players");
}

ExpertEffect Rules::applyExpertRule(Game& game, const Card& card, Player& currentPlayer) {
    if (!expertRules) return ExpertEffect::None;
    
    FaceAnimal animal = (FaceAnimal)card;
    char lChar;
    int nInt;
    
    switch (animal) {
        case FaceAnimal::Octopus: {
            std::cout << "Octopus! Swap with an adjacent card.\n";
            // Logic to ask user for swap
            std::cout << "Enter adjacent card position (e.g. B2) to swap with current card: ";
            std::cin >> lChar >> nInt;
            Letter l2 = static_cast<Letter>(lChar - 'A');
            Number n2 = static_cast<Number>(nInt - 1);
            
            // Find current card position (brute force search since we don't pass coords)
            // Ideally we'd pass coords to this function, but let's scan
            Letter l1; Number n1;
            bool found = false;
             for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < 5; ++j) {
                    if (i==2 && j==2) continue;
                    if (game.getCard((Letter)i, (Number)j) == &card) {
                        l1 = (Letter)i; n1 = (Number)j;
                        found = true; break;
                    }
                }
                if(found) break;
            }
            
            try {
                // Check adjacency (simple manhattan distance check)
                int r1 = (int)l1, c1 = (int)n1, r2 = (int)l2, c2 = (int)n2;
                if (std::abs(r1 - r2) + std::abs(c1 - c2) == 1) {
                    game.swapCards(l1, n1, l2, n2);
                    std::cout << "Cards swapped.\n";
                } else {
                    std::cout << "Not adjacent. Ignored.\n";
                }
            } catch (...) {
                std::cout << "Invalid position. Ignored.\n";
            }
            break;
        }
        case FaceAnimal::Penguin: {
            std::cout << "Penguin! Turn a visible card face down.\n";
            std::cout << "Enter position (e.g. A1): ";
            std::cin >> lChar >> nInt;
            try {
                game.turnFaceDown(static_cast<Letter>(lChar - 'A'), static_cast<Number>(nInt - 1));
            } catch (...) {
                std::cout << "Invalid. Ignored.\n";
            }
            break;
        }
        case FaceAnimal::Walrus: {
            std::cout << "Walrus! Block a card for the next player.\n";
            std::cout << "Enter position to block (e.g. A1): ";
            std::cin >> lChar >> nInt;
            try {
                game.setBlockedCard(static_cast<Letter>(lChar - 'A'), static_cast<Number>(nInt - 1));
                std::cout << "Position blocked.\n";
            } catch (...) {
                std::cout << "Invalid. Ignored.\n";
            }
            break;
        }
        case FaceAnimal::Crab: {
            std::cout << "Crab! You must play again.\n";
            return ExpertEffect::PlayAgain;
        }
        case FaceAnimal::Turtle: {
            std::cout << "Turtle! Next player skips a turn.\n";
            return ExpertEffect::SkipNext;
        }
        default: break;
    }
    return ExpertEffect::None;
}