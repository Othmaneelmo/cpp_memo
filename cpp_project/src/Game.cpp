#include "Game.h"
#include "Card.h"
#include "CardDeck.h"

Game::Game(CardDeck& deck, bool expertDisplay) 
    : board(deck), round(0), previousCard(nullptr), currentCard(nullptr), 
      expertDisplay(expertDisplay), hasBlockedCard(false) {}

int Game::getRound() const {
    return round;
}

void Game::addPlayer(const Player& player) {
    players.push_back(player);
}

Player& Game::getPlayer(Side side) {
    for (auto& p : players) {
        if (p.getSide() == side) return p;
    }
    throw std::runtime_error("Player not found");
}

const Card* Game::getPreviousCard() const {
    return previousCard;
}

const Card* Game::getCurrentCard() const {
    return currentCard;
}

void Game::setCurrentCard(const Card* card) {
    previousCard = currentCard;
    currentCard = card;
}

Card* Game::getCard(const Letter& l, const Number& n) {
    return board.getCard(l, n);
}

void Game::setCard(const Letter& l, const Number& n, Card* card) {
    board.setCard(l, n, card);
}

void Game::nextRound() {
    ++round;
    board.allFacesDown();
    resetBlocked(); // Clear walrus blocks
    previousCard = nullptr;
    currentCard = nullptr;
    for (auto& p : players) {
        p.setActive(true);
    }
}

bool Game::isExpertDisplay() const {
    return expertDisplay;
}

bool Game::turnFaceUp(const Letter& l, const Number& n) {
    return board.turnFaceUp(l, n);
}

bool Game::turnFaceDown(const Letter& l, const Number& n) {
    return board.turnFaceDown(l, n);
}

const std::vector<Player>& Game::getPlayers() const {
    return players;
}

void Game::setPlayerActive(Side side, bool active) {
    for (auto& p : players) {
        if (p.getSide() == side) {
            p.setActive(active);
            return;
        }
    }
}

void Game::addRubisToPlayer(Side side, const Rubis& rubis) {
    for (auto& p : players) {
        if (p.getSide() == side) {
            p.addRubis(rubis);
            return;
        }
    }
}

void Game::setBlockedCard(Letter l, Number n) {
    hasBlockedCard = true;
    blockedLetter = l;
    blockedNumber = n;
}

bool Game::isBlocked(Letter l, Number n) const {
    return hasBlockedCard && blockedLetter == l && blockedNumber == n;
}

void Game::resetBlocked() {
    hasBlockedCard = false;
}

void Game::swapCards(Letter l1, Number n1, Letter l2, Number n2) {
    board.swapCards(l1, n1, l2, n2);
}

std::vector<std::pair<Letter, Number>> Game::getSightLocations(Side side) const {
    std::vector<std::pair<Letter, Number>> locs;
    switch (side) {
        case Side::top: // A2, A3, A4
            locs.push_back({Letter::A, Number::Two});
            locs.push_back({Letter::A, Number::Three});
            locs.push_back({Letter::A, Number::Four});
            break;
        case Side::bottom: // E2, E3, E4
            locs.push_back({Letter::E, Number::Two});
            locs.push_back({Letter::E, Number::Three});
            locs.push_back({Letter::E, Number::Four});
            break;
        case Side::left: // B1, C1, D1
            locs.push_back({Letter::B, Number::One});
            locs.push_back({Letter::C, Number::One});
            locs.push_back({Letter::D, Number::One});
            break;
        case Side::right: // B5, C5, D5
            locs.push_back({Letter::B, Number::Five});
            locs.push_back({Letter::C, Number::Five});
            locs.push_back({Letter::D, Number::Five});
            break;
    }
    return locs;
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    if (game.expertDisplay) {
        // Expert display: only revealed cards with positions
        std::vector<std::pair<std::string, std::string>> revealed;
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                if (i == 2 && j == 2) continue;
                Letter l = static_cast<Letter>(i);
                Number n = static_cast<Number>(j);
                if (game.board.isFaceUp(l, n)) {
                    std::string pos = std::string(1, 'A' + i) + std::to_string(j + 1);
                    std::string cardStr;
                    Card* card = game.board.getCard(l, n);
                    for (int r = 0; r < card->getNRows(); ++r) {
                        cardStr += (*card)(r) + " ";
                    }
                    cardStr.pop_back(); 
                    revealed.emplace_back(cardStr, pos);
                }
            }
        }
        
        // Simple linear print for expert mode
        os << "Expert Display (Revealed Cards):\n";
        for (const auto& item : revealed) {
            os << item.second << ": " << item.first << "\n";
        }
        os << "\n";
    } else {
        os << game.getBoard();
    }
    // Print players
    for (const auto& p : game.getPlayers()) {
        os << p << "\n";
    }
    return os;
}