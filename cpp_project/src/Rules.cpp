#include "Rules.h"
#include "Game.h"
#include "Card.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <limits>

bool Rules::isValid(const Game& game) const {
    if (!game.getPreviousCard() || !game.getCurrentCard()) return true;
    const Card* prev = game.getPreviousCard();
    const Card* curr = game.getCurrentCard();
    return (FaceAnimal)*prev == (FaceAnimal)*curr || 
           (FaceBackground)*prev == (FaceBackground)*curr;
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
    const auto& players = game.getPlayers();
    if (players.empty()) throw std::runtime_error("No players");

    // Find first active player (cyclic search from beginning each time)
    // This ensures consistent player order without static state issues
    for (const auto& p : players) {
        if (p.isActive()) {
            return p;
        }
    }
    throw std::runtime_error("No active players");
}

// Helper function to safely read position input
static bool safeReadPosition(char& lChar, int& nInt) {
    if (!(std::cin >> lChar >> nInt)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

ExpertEffect Rules::applyExpertRule(Game& game, const Card& card, Player& currentPlayer) {
    if (!expertRules) return ExpertEffect::None;
    
    FaceAnimal animal = (FaceAnimal)card;
    char lChar;
    int nInt;
    
    switch (animal) {
        case FaceAnimal::Octopus: {
            std::cout << "Octopus! Swap with an adjacent card.\n";
            
            // Find current card position
            Letter l1 = Letter::A;
            Number n1 = Number::One;
            bool found = false;
            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < 5; ++j) {
                    if (i == 2 && j == 2) continue;
                    if (game.getCard((Letter)i, (Number)j) == &card) {
                        l1 = (Letter)i;
                        n1 = (Number)j;
                        found = true;
                        break;
                    }
                }
                if (found) break;
            }
            
            if (!found) {
                std::cout << "Error: Could not find card position.\n";
                break;
            }
            
            std::cout << "Current card is at " << char('A' + (int)l1) << ((int)n1 + 1) << "\n";
            std::cout << "Enter adjacent card position (e.g. B2) to swap with: ";
            
            // FIXED: Safe input reading
            if (!safeReadPosition(lChar, nInt)) {
                std::cout << "Invalid input. Swap ignored.\n";
                break;
            }
            
            // Convert to uppercase
            if (lChar >= 'a' && lChar <= 'e') {
                lChar = lChar - 'a' + 'A';
            }
            
            try {
                Letter l2 = static_cast<Letter>(lChar - 'A');
                Number n2 = static_cast<Number>(nInt - 1);
                
                // Check adjacency (Manhattan distance = 1, same row or column)
                int r1 = (int)l1, c1 = (int)n1;
                int r2 = (int)l2, c2 = (int)n2;
                
                bool adjacent = (std::abs(r1 - r2) + std::abs(c1 - c2) == 1);
                
                if (adjacent) {
                    game.swapCards(l1, n1, l2, n2);
                    std::cout << "Cards swapped.\n";
                } else {
                    std::cout << "Not adjacent. Swap ignored.\n";
                }
            } catch (...) {
                std::cout << "Invalid position. Swap ignored.\n";
            }
            break;
        }
        
        case FaceAnimal::Penguin: {
            // Check if there are other visible cards and it's not first turn
            if (!game.getPreviousCard()) {
                std::cout << "Penguin: No effect (first turn).\n";
                break;
            }
            
            // Count visible cards
            int visibleCount = 0;
            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < 5; ++j) {
                    if (i == 2 && j == 2) continue;
                    try {
                        if (game.getBoard().isFaceUp((Letter)i, (Number)j)) {
                            visibleCount++;
                        }
                    } catch (...) {}
                }
            }
            
            if (visibleCount <= 1) {
                std::cout << "Penguin: No other visible cards to turn down.\n";
                break;
            }
            
            std::cout << "Penguin! Turn a visible card face down.\n";
            std::cout << "Enter position (e.g. A1): ";
            
            // FIXED: Safe input reading
            if (!safeReadPosition(lChar, nInt)) {
                std::cout << "Invalid input. Penguin effect ignored.\n";
                break;
            }
            
            // Convert to uppercase
            if (lChar >= 'a' && lChar <= 'e') {
                lChar = lChar - 'a' + 'A';
            }
            
            try {
                Letter l = static_cast<Letter>(lChar - 'A');
                Number n = static_cast<Number>(nInt - 1);
                
                if (game.getBoard().isFaceUp(l, n)) {
                    game.turnFaceDown(l, n);
                    std::cout << "Card turned face down.\n";
                } else {
                    std::cout << "Card is not face up. Ignored.\n";
                }
            } catch (...) {
                std::cout << "Invalid position. Ignored.\n";
            }
            break;
        }
        
        case FaceAnimal::Walrus: {
            std::cout << "Walrus! Block a card for the next player.\n";
            std::cout << "Enter position to block (e.g. A1): ";
            
            // FIXED: Safe input reading
            if (!safeReadPosition(lChar, nInt)) {
                std::cout << "Invalid input. Block ignored.\n";
                break;
            }
            
            // Convert to uppercase
            if (lChar >= 'a' && lChar <= 'e') {
                lChar = lChar - 'a' + 'A';
            }
            
            try {
                Letter l = static_cast<Letter>(lChar - 'A');
                Number n = static_cast<Number>(nInt - 1);
                game.setBlockedCard(l, n);
                std::cout << "Position " << lChar << nInt << " blocked for next player.\n";
            } catch (...) {
                std::cout << "Invalid position. Block ignored.\n";
            }
            break;
        }
        
        case FaceAnimal::Crab: {
            std::cout << "Crab! You must play again.\n";
            return ExpertEffect::PlayAgain;
        }
        
        case FaceAnimal::Turtle: {
            std::cout << "Turtle! Next player skips their turn.\n";
            return ExpertEffect::SkipNext;
        }
        
        default:
            break;
    }
    
    return ExpertEffect::None;
}
