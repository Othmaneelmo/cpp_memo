#ifndef RUBIS_H
#define RUBIS_H

#include <iostream>

class Rubis {
private:
    int value;
    Rubis(int value); // private constructor
    friend class RubisDeck;

public:
    operator int() const { return value; }
    friend std::ostream& operator<<(std::ostream& os, const Rubis& rubis) {
        os << rubis.value << " rubis";
        return os;
    }
};

#endif
