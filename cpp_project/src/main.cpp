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
#include <thread>
#include <chrono>

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
    for (int i = 0; i < numPlayers; ++i) {
        std::string name;
        std::cout << "Player " << (i+1) << " name: ";
        std::cin >> name;
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
    std::cout << game;

    // Game loop
    while (!rules.gameOver(game)) {
        game.nextRound();
        std::cout << "\n=== Round " << game.getRound() << " ===\n";

        // Pre-round Reveal Phase
        std::cout << "Revealing cards for players... (Memorize them!)\n";
        for (const auto& p : game.getPlayers()) {
             auto locs = game.getSightLocations(p.getSide());
             for(auto loc : locs) {
                 game.turnFaceUp(loc.first, loc.second);
             }
        }
        
        std::cout << game; // Show the revealed cards
        
        std::cout << "Press Enter to hide cards and start...";
        std::cin.ignore();
        std::cin.get();

        // Hide them again
        for (const auto& p : game.getPlayers()) {
             auto locs = game.getSightLocations(p.getSide());
             for(auto loc : locs) {
                 game.turnFaceDown(loc.first, loc.second);
             }
        }
        
        // Ensure standard clean board display
        if(!expertDisplay) std::cout << std::string(50, '\n'); 
        std::cout << game;

        bool skipNextPlayer = false;

        while (!rules.roundOver(game)) {
            // Get next player, handling skips
            const Player& currentPlayer = rules.getNextPlayer(game);
            
            if (skipNextPlayer) {
                std::cout << currentPlayer.getName() << " is skipped due to Turtle!\n";
                skipNextPlayer = false;
                continue; 
            }

            std::cout << "\n" << currentPlayer.getName() << "'s turn.\n";
            
            bool turnActive = true;
            bool isSecondTurn = false; // For Crab

            while (turnActive) {
                // Get card selection
                char letter;
                int number;
                bool validPick = false;
                Letter l;
                Number n;

                while (!validPick) {
                    std::cout << "Choose card (e.g. A1): ";
                    std::cin >> letter >> number;
                    
                    if (letter < 'A' || letter > 'E' || number < 1 || number > 5) {
                        std::cout << "Invalid format. Try again.\n";
                        continue;
                    }

                    l = static_cast<Letter>(letter - 'A');
                    n = static_cast<Number>(number - 1);

                    if (l == Letter::C && n == Number::Three) {
                        std::cout << "Center is empty.\n";
                        continue;
                    }

                    if (game.isBlocked(l, n)) {
                        std::cout << "That card is blocked by the Walrus! Choose another.\n";
                        continue;
                    }
                    
                    try {
                        // Check if already face up
                        // The rules say "choose a card to turn face up". 
                        // If it's already up, strictly speaking you can't turn it up.
                        // But usually in memory games you pick a hidden one.
                        if (game.getBoard().isFaceUp(l, n)) {
                             // If it's already up, just select it as current?
                             // Standard memory rules usually require picking hidden.
                             // But let's assume you can pick visible to chain matches.
                             // However, logic below calls turnFaceUp.
                        }
                        validPick = true;
                    } catch (...) {
                        std::cout << "Invalid position.\n";
                    }
                }

                // Reset block for next turn immediately after valid pick? 
                // Or at end of turn? Usually blocks last 1 turn.
                game.resetBlocked();

                // Turn card up
                bool wasHidden = game.turnFaceUp(l, n);
                Card* card = game.getCard(l, n);
                game.setCurrentCard(card);
                std::cout << game;

                ExpertEffect effect = ExpertEffect::None;
                
                // Apply expert rule if applicable
                if (expertRules && card && wasHidden) { // Only trigger on reveal
                    effect = rules.applyExpertRule(game, *card, const_cast<Player&>(currentPlayer));
                    std::cout << game; // Show updates after expert rule
                }

                // Check if valid match
                if (!rules.isValid(game)) {
                    std::cout << "Mismatch! " << currentPlayer.getName() << " is eliminated from this round.\n";
                    game.setPlayerActive(currentPlayer.getSide(), false);
                    turnActive = false;
                } else {
                    std::cout << "Match valid!\n";
                    if (effect == ExpertEffect::PlayAgain && !isSecondTurn) {
                        std::cout << "Crab effect: Play again!\n";
                        isSecondTurn = true;
                        // turnActive stays true, loop continues
                    } else {
                        if (effect == ExpertEffect::SkipNext) {
                            skipNextPlayer = true;
                        }
                        turnActive = false; // Turn ends successfully
                    }
                }
            } // End turn loop
        } // End round loop

        // Round over, give rubis to winner
        const auto& players = game.getPlayers();
        for (size_t i = 0; i < players.size(); ++i) {
            if (players[i].isActive()) {
                Rubis* rubis = rubisDeck.getNext();
                if (rubis) {
                    std::cout << players[i].getName() << " wins the round and gets a ruby!\n";
                    game.addRubisToPlayer(players[i].getSide(), *rubis);
                    delete rubis;
                } else {
                    std::cout << "No more rubies!\n";
                }
            }
        }

        // Print players sorted by rubis
        std::vector<Player> sortedPlayers = game.getPlayers();
        std::sort(sortedPlayers.begin(), sortedPlayers.end(), [](const Player& a, const Player& b) {
            return a.getNRubies() < b.getNRubies();
        });
        
        std::cout << "\n--- Standings ---\n";
        for (auto& p : sortedPlayers) {
            p.setDisplayMode(true);
            std::cout << p << "\n";
        }
    }

    // Game over, print winner
    const auto& players = game.getPlayers();
    auto winnerIt = std::max_element(players.begin(), players.end(), [](const Player& a, const Player& b) {
        return a.getNRubies() < b.getNRubies();
    });
    if (winnerIt != players.end()) {
        std::cout << "\nGAME OVER! Winner: " << winnerIt->getName() << " with " << winnerIt->getNRubies() << " rubis!\n";
    }

    return 0;
}