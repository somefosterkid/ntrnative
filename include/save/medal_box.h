// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/medal_box.h: the lines in use so far.
#ifndef BWPORT_SAVE_MEDAL_BOX_H
#define BWPORT_SAVE_MEDAL_BOX_H

// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"

// Black 2's, not checked against Black
#define MEDAL_COUNT 255

// Black 2's, not checked against Black
// What the player knows of a medal
enum {
    MEDAL_STATUS_UNKNOWN,
    MEDAL_STATUS_1,
    // Shown as a hint
    MEDAL_STATUS_DISCOVERED,
    // Earned, but Mr. Medal hasn't handed it over yet
    MEDAL_STATUS_EARNED,
    MEDAL_STATUS_OBTAINED,
};

#endif // BWPORT_SAVE_MEDAL_BOX_H
