#include "Card.h"
#include "Enums.h"
#include <stdexcept>


Card::Card(FaceAnimal animal, FaceBackground background)
    : animal(animal), background(background) {}

// Converts a row (0,1,2) of the card to a string like "RGB" or " F "
std::string Card::operator()(int row) const {
    if (row < 0 || row >= getNRows())
        throw std::out_of_range("Invalid row index");

    char fill;

    // Background fill character
    switch (background) {
        case FaceBackground::Red:    fill = 'R'; break;
        case FaceBackground::Green:  fill = 'G'; break;
        case FaceBackground::Purple: fill = 'P'; break;
        case FaceBackground::Blue:   fill = 'B'; break;
        case FaceBackground::Yellow: fill = 'Y'; break;
        default: fill = '?'; break;
    }

    std::string result(3, fill);

    // Center row shows the animal letter
    if (row == 1) {
        char center;
        switch (animal) {
            case FaceAnimal::Crab:     center = 'C'; break;
            case FaceAnimal::Penguin:  center = 'P'; break;
            case FaceAnimal::Octopus:  center = 'O'; break;
            case FaceAnimal::Turtle:   center = 'T'; break;
            case FaceAnimal::Walrus:   center = 'W'; break;
            default:                   center = '?'; break;
        }
        result[1] = center;
    }

    return result;
}

Card::operator FaceAnimal() const {
    return animal;
}

Card::operator FaceBackground() const {
    return background;
}
