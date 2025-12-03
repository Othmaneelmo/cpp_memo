#include "Board.h"
#include "CardDeck.h"
#include <stdexcept>
#include <algorithm> // Required for std::swap and std::fill

Board::Board(CardDeck& deck) : grid(5, std::vector<Card*>(5, nullptr)), faceUp(5, std::vector<bool>(5, false)) {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i == 2 && j == 2) continue; 
            Card* card = deck.getNext();
            if (!card) throw NoMoreCards("Not enough cards in deck");
            grid[i][j] = card;
        }
    }
}

Board::~Board() {
    for (auto& row : grid) {
        for (auto card : row) {
            delete card;
        }
    }
}

int Board::letterToIndex(Letter l) const {
    switch (l) {
        case Letter::A: return 0;
        case Letter::B: return 1;
        case Letter::C: return 2;
        case Letter::D: return 3;
        case Letter::E: return 4;
        default: throw OutOfRange("Invalid letter");
    }
}

int Board::numberToIndex(Number n) const {
    switch (n) {
        case Number::One: return 0;
        case Number::Two: return 1;
        case Number::Three: return 2;
        case Number::Four: return 3;
        case Number::Five: return 4;
        default: throw OutOfRange("Invalid number");
    }
}

bool Board::isValidPosition(Letter l, Number n) const {
    int i = letterToIndex(l);
    int j = numberToIndex(n);
    return !(i == 2 && j == 2); // center invalid
}

bool Board::isFaceUp(const Letter& l, const Number& n) const {
    if (!isValidPosition(l, n)) throw OutOfRange("Invalid position");
    int i = letterToIndex(l);
    int j = numberToIndex(n);
    return faceUp[i][j];
}

bool Board::turnFaceUp(const Letter& l, const Number& n) {
    if (!isValidPosition(l, n)) throw OutOfRange("Invalid position");
    int i = letterToIndex(l);
    int j = numberToIndex(n);
    bool wasUp = faceUp[i][j];
    faceUp[i][j] = true;
    return !wasUp;
}

bool Board::turnFaceDown(const Letter& l, const Number& n) {
    if (!isValidPosition(l, n)) throw OutOfRange("Invalid position");
    int i = letterToIndex(l);
    int j = numberToIndex(n);
    bool wasDown = !faceUp[i][j];
    faceUp[i][j] = false;
    return wasDown;
}

Card* Board::getCard(const Letter& l, const Number& n) const {
    if (!isValidPosition(l, n)) throw OutOfRange("Invalid position");
    int i = letterToIndex(l);
    int j = numberToIndex(n);
    return grid[i][j];
}

void Board::setCard(const Letter& l, const Number& n, Card* card) {
    if (!isValidPosition(l, n)) throw OutOfRange("Invalid position");
    int i = letterToIndex(l);
    int j = numberToIndex(n);
    grid[i][j] = card;
}

// Missing implementation from your original paste
void Board::swapCards(const Letter& l1, const Number& n1, const Letter& l2, const Number& n2) {
    if (!isValidPosition(l1, n1) || !isValidPosition(l2, n2)) 
        throw OutOfRange("Invalid position");
    
    int r1 = letterToIndex(l1);
    int c1 = numberToIndex(n1);
    int r2 = letterToIndex(l2);
    int c2 = numberToIndex(n2);

    // Swap the card pointers
    std::swap(grid[r1][c1], grid[r2][c2]);
    
    // Swap the face-up status safely
    bool temp = faceUp[r1][c1];
    faceUp[r1][c1] = faceUp[r2][c2];
    faceUp[r2][c2] = temp;
}


void Board::allFacesDown() {
    for (auto& row : faceUp) {
        std::fill(row.begin(), row.end(), false);
    }
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
    // Print the board as 19x19 grid
    for (int row = 0; row < 19; ++row) {
        for (int col = 0; col < 19; ++col) {
            // Determine which card this position belongs to
            int cardRow = row / 4; // 0-4 for rows A-E
            int cardCol = col / 4; // 0-4 for cols 1-5
            int subRow = row % 4;
            int subCol = col % 4;

            if (cardRow >= 5 || cardCol >= 5 || (cardRow == 2 && cardCol == 2)) {
                os << ' ';
            } else if (subRow == 3 || subCol == 3) { // space between cards
                os << ' ';
            } else {
                Card* card = board.grid[cardRow][cardCol];
                if (card && board.faceUp[cardRow][cardCol]) {
                    os << (*card)(subRow)[subCol];
                } else {
                    os << 'z';
                }
            }
        }
        os << '\n';
    }
    
    os << "1 2 3 4 5\n";
    return os;
}