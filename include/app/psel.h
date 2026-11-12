// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/psel.h: the lines in use so far.
#ifndef BWPORT_APP_PSEL_H
#define BWPORT_APP_PSEL_H

#include "types.h"
#include "gfl/overlay.h"

// Black 2's, not checked against Black
// The screen's heap and its archive of graphics. Our names, not swan's
#define HEAPID_PSEL 0x87

// Black's (near twins)
#define ARCID_PSEL 205

// Black 2's, not checked against Black
// What the screen is called with. It writes the chosen Pokémon to *result: 0 for Snivy, 1 for Tepig and 2 for
// Oshawott, and to choice as well
struct PselParam {
    u16 *result;
    s32 choice;
};

// Black 2's, not checked against Black
#define OVERLAY_PSEL OVERLAY_ID(316)

#endif // BWPORT_APP_PSEL_H
