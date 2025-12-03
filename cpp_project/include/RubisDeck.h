#ifndef RUBISDECK_H
#define RUBISDECK_H

#include "DeckFactory.h"
#include "Rubis.h"

class RubisDeck : public DeckFactory<Rubis> {
private:
    static RubisDeck* instance;
    RubisDeck();

public:
    static RubisDeck& make_RubisDeck();
};

#endif
