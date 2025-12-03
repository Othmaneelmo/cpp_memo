#include "Game.h"
#include "Rules.h"
#include "Player.h"
#include "CardDeck.h"
#include "RubisDeck.h"
#include "Board.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main() {
    std::cout << "Welcome to Memoarr!\n";

    // Ask for game version
    std::string version;
    std::cout << "Choose game version (base/expert_display/expert_rules): ";
    std::cin >> version;
    bool expertDisplay = (version == "expert_display");
    bool expertRules = (version == "expert_rules");

    // Ask for number of players
    int numPlayers;
    std::cout << "Number of players (2-4): ";
    std::cin >> numPlayers;
    if (numPlayers < 2 || numPlayers > 4) {
        std::cout << "Invalid number of players.\n";
        return 1;
    }

    // Ask for player names
    std::vector<std::string> names;
    std::cin.ignore(); // Clear newline
    for (int i = 0; i < numPlayers; ++i) {
        std::string name;
        std::cout << "Player " << (i+1) << " name: ";
        std::getline(std::cin, name);
        names.push_back(name);
    }

    // Create decks
    CardDeck& cardDeck = CardDeck::make_CardDeck();
    RubisDeck& rubisDeck = RubisDeck::make_RubisDeck();

    // Create game
    Game game(cardDeck, expertDisplay);
    Rules rules(expertRules);

    // Create players
    std::vector<Side> sides = {Side::top, Side::bottom, Side::left, Side::right};
    for (int i = 0; i < numPlayers; ++i) {
        Player p(names[i], sides[i]);
        game.addPlayer(p);
    }

    // Display initial game
    std::cout << "\n" << game << "\n";

    // Game loop - 7 rounds
    while (!rules.gameOver(game)) {
        game.nextRound();
        std::cout << "\n========== ROUND " << game.getRound() << " ==========\n";

        // Pre-round reveal phase
        std::cout << "\nRevealing cards for each player (memorize them)...\n";
        for (const auto& p : game.getPlayers()) {
            auto locs = game.getSightLocations(p.getSide());
            std::cout << p.getName() << " can see: ";
            for (auto loc : locs) {
                std::cout << char('A' + (int)loc.first) << ((int)loc.second + 1) << " ";
            }
            std::cout << "\n";
            
            for (auto loc : locs) {
                game.turnFaceUp(loc.first, loc.second);
            }
        }
        
        std::cout << "\n" << game << "\n";
        
        std::cout << "Press Enter to hide cards and begin round...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();

        // Hide cards again
        for (const auto& p : game.getPlayers()) {
            auto locs = game.getSightLocations(p.getSide());
            for (auto loc : locs) {
                game.turnFaceDown(loc.first, loc.second);
            }
        }
        
        std::cout << "\n" << game << "\n";

        // Round play with proper player rotation
        int currentPlayerIndex = 0;
        bool skipNextPlayer = false;

        while (!rules.roundOver(game)) {
            // Get mutable players list
            auto& players = game.getPlayersMutable();
            
            // Find next active player starting from currentPlayerIndex
            bool foundActive = false;
            int startIndex = currentPlayerIndex;
            
            for (int i = 0; i < (int)players.size(); ++i) {
                int idx = (startIndex + i) % players.size();
                if (players[idx].isActive()) {
                    currentPlayerIndex = idx;
                    foundActive = true;
                    break;
                }
            }
            
            if (!foundActive) break; // No active players
            
            Player& currentPlayer = players[currentPlayerIndex];
            
            // Handle skip from Turtle
            if (skipNextPlayer) {
                std::cout << "\n" << currentPlayer.getName() << " is skipped due to Turtle effect!\n";
                skipNextPlayer = false;
                currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
                continue;
            }

            std::cout << "\n>>> " << currentPlayer.getName() << "'s turn <<<\n";
            
            bool playAgain = true;
            bool isSecondTurn = false;

            while (playAgain) {
                playAgain = false; // Reset unless Crab effect
                
                // Get card selection
                char letter;
                int number;
                Letter l;
                Number n;
                bool validPick = false;

                while (!validPick) {
                    std::cout << "Choose a card to reveal (e.g. A1): ";
                    std::cin >> letter >> number;
                    
                    if (letter < 'A' || letter > 'E' || number < 1 || number > 5) {
                        std::cout << "Invalid format. Use A-E and 1-5.\n";
                        continue;
                    }

                    l = static_cast<Letter>(letter - 'A');
                    n = static_cast<Number>(number - 1);

                    // Check center
                    if (l == Letter::C && n == Number::Three) {
                        std::cout << "Center position is empty. Choose another.\n";
                        continue;
                    }

                    // Check if blocked by Walrus
                    if (game.isBlocked(l, n)) {
                        std::cout << "That card is blocked by Walrus! Choose another.\n";
                        continue;
                    }
                    
                    // Check if already face up
                    try {
                        if (game.getBoard().isFaceUp(l, n)) {
                            std::cout << "That card is already revealed. Choose a hidden card.\n";
                            continue;
                        }
                        validPick = true;
                    } catch (const std::exception& e) {
                        std::cout << "Invalid position: " << e.what() << "\n";
                    }
                }

                // Clear Walrus block after valid selection
                game.resetBlocked();

                // Reveal card
                game.turnFaceUp(l, n);
                Card* card = game.getCard(l, n);
                game.setCurrentCard(card);
                
                std::cout << "\n" << game << "\n";

                // Apply expert rules if enabled
                ExpertEffect effect = ExpertEffect::None;
                if (expertRules && card) {
                    effect = rules.applyExpertRule(game, *card, currentPlayer);
                    if (effect != ExpertEffect::None) {
                        std::cout << "\n" << game << "\n";
                    }
                }

                // Check validity
                if (!rules.isValid(game)) {
                    std::cout << "❌ MISMATCH! " << currentPlayer.getName() 
                              << " is eliminated from this round.\n";
                    currentPlayer.setActive(false);
                } else {
                    std::cout << "✓ Valid match!\n";
                    
                    // Handle special effects
                    if (effect == ExpertEffect::PlayAgain && !isSecondTurn) {
                        std::cout << "→ Crab effect: Play again!\n";
                        playAgain = true;
                        isSecondTurn = true;
                    } else if (effect == ExpertEffect::SkipNext) {
                        skipNextPlayer = true;
                    }
                }
            }
            
            // Move to next player
            currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
        }

        // Round over - award rubis to winner
        std::cout << "\n--- Round " << game.getRound() << " Complete ---\n";
        
        for (auto& p : game.getPlayersMutable()) {
            if (p.isActive()) {
                Rubis* rubis = rubisDeck.getNext();
                if (rubis) {
                    std::cout << "🏆 " << p.getName() << " wins and receives " 
                              << *rubis << "!\n";
                    p.addRubis(*rubis);
                    delete rubis;
                } else {
                    std::cout << "⚠ No more rubies in deck!\n";
                }
                break;
            }
        }

        // Display current standings
        auto sortedPlayers = game.getPlayers();
        std::sort(sortedPlayers.begin(), sortedPlayers.end(), 
                  [](const Player& a, const Player& b) {
                      return a.getNRubies() < b.getNRubies();
                  });
        
        std::cout << "\n--- Current Standings ---\n";
        for (auto& p : sortedPlayers) {
            std::cout << p.getName() << ": " << p.getNRubies() << " rubis\n";
        }
    }

    // Game over - announce winner
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "GAME OVER - 7 ROUNDS COMPLETE!\n";
    std::cout << std::string(50, '=') << "\n\n";
    
    const auto& players = game.getPlayers();
    auto winnerIt = std::max_element(players.begin(), players.end(),
                                      [](const Player& a, const Player& b) {
                                          return a.getNRubies() < b.getNRubies();
                                      });
    
    if (winnerIt != players.end()) {
        std::cout << "🎉 WINNER: " << winnerIt->getName() 
                  << " with " << winnerIt->getNRubies() << " rubis! 🎉\n\n";
        
        // Final standings
        auto sortedPlayers = players;
        std::sort(sortedPlayers.begin(), sortedPlayers.end(),
                  [](const Player& a, const Player& b) {
                      return a.getNRubies() > b.getNRubies();
                  });
        
        std::cout << "Final Standings:\n";
        int rank = 1;
        for (const auto& p : sortedPlayers) {
            std::cout << rank++ << ". " << p.getName() 
                      << ": " << p.getNRubies() << " rubis\n";
        }
    }

    return 0;
}
