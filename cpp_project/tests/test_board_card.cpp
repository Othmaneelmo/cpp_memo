#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "Board.h"
#include "CardDeck.h"
#include "Card.h"
#include "Enums.h"
#include "Exceptions.h"

// -------------------
// Card Tests
// -------------------
TEST_CASE("Card displays correct pattern", "[Card]") {
    CardDeck& deck = CardDeck::make_CardDeck();
    Card* c = deck.getNext();
    REQUIRE(c != nullptr);

    REQUIRE(c->getNRows() == 3);
    std::string row1 = (*c)(1);
    FaceAnimal animal = (FaceAnimal)(*c);
    char expectedAnimalChar;
    switch (animal) {
        case FaceAnimal::Crab: expectedAnimalChar = 'C'; break;
        case FaceAnimal::Penguin: expectedAnimalChar = 'P'; break;
        case FaceAnimal::Octopus: expectedAnimalChar = 'O'; break;
        case FaceAnimal::Turtle: expectedAnimalChar = 'T'; break;
        case FaceAnimal::Walrus: expectedAnimalChar = 'W'; break;
        default: expectedAnimalChar = '?'; break;
    }
    REQUIRE(row1[1] == expectedAnimalChar);

    FaceBackground bg = (FaceBackground)(*c);
    char expectedBgChar;
    switch (bg) {
        case FaceBackground::Red: expectedBgChar = 'R'; break;
        case FaceBackground::Green: expectedBgChar = 'G'; break;
        case FaceBackground::Purple: expectedBgChar = 'P'; break;
        case FaceBackground::Blue: expectedBgChar = 'B'; break;
        case FaceBackground::Yellow: expectedBgChar = 'Y'; break;
        default: expectedBgChar = '?'; break;
    }
    REQUIRE(row1[0] == expectedBgChar);
    REQUIRE(row1[2] == expectedBgChar);

    delete c;
}

// -------------------
// Board Tests
// -------------------
TEST_CASE("Board functionality", "[Board]") {
    CardDeck& deck = CardDeck::make_CardDeck();
    Board board(deck);

    // Board initializes correctly
    REQUIRE(board.getCard(Letter::A, Number::One) != nullptr);
    REQUIRE(board.getCard(Letter::A, Number::Two) != nullptr);
    REQUIRE(board.isFaceUp(Letter::A, Number::One) == false);

    // Board turnFaceUp and turnFaceDown works
    bool changed = board.turnFaceUp(Letter::A, Number::One);
    REQUIRE(changed == true);
    REQUIRE(board.isFaceUp(Letter::A, Number::One) == true);

    changed = board.turnFaceUp(Letter::A, Number::One);
    REQUIRE(changed == false);

    changed = board.turnFaceDown(Letter::A, Number::One);
    REQUIRE(changed == true);
    REQUIRE(board.isFaceUp(Letter::A, Number::One) == false);

    changed = board.turnFaceDown(Letter::A, Number::One);
    REQUIRE(changed == false);

    // Board allFacesDown resets all cards
    board.turnFaceUp(Letter::A, Number::One);
    board.turnFaceUp(Letter::A, Number::Two);

    board.allFacesDown();

    REQUIRE(board.isFaceUp(Letter::A, Number::One) == false);
    REQUIRE(board.isFaceUp(Letter::A, Number::Two) == false);

    // Board swapCards swaps cards and face-up status
    Card* card1 = board.getCard(Letter::A, Number::One);
    Card* card2 = board.getCard(Letter::A, Number::Two);

    board.turnFaceUp(Letter::A, Number::One);
    board.turnFaceDown(Letter::A, Number::Two);

    board.swapCards(Letter::A, Number::One, Letter::A, Number::Two);

    REQUIRE(board.getCard(Letter::A, Number::One) == card2);
    REQUIRE(board.getCard(Letter::A, Number::Two) == card1);

    REQUIRE(board.isFaceUp(Letter::A, Number::One) == false);
    REQUIRE(board.isFaceUp(Letter::A, Number::Two) == true);

    // Board invalid positions throw exceptions
    REQUIRE_THROWS_AS(board.getCard(Letter::C, Number::Three), OutOfRange);
    REQUIRE_THROWS_AS(board.turnFaceUp(Letter::C, Number::Three), OutOfRange);
    REQUIRE_THROWS_AS(board.turnFaceDown(Letter::C, Number::Three), OutOfRange);
    REQUIRE_THROWS_AS(board.swapCards(Letter::A, Number::One, Letter::C, Number::Three), OutOfRange);
}
