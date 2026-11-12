// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/btl_rec_sel.h: the lines in use so far.
#ifndef BWPORT_APP_BTL_REC_SEL_H
#define BWPORT_APP_BTL_REC_SEL_H

#include "types.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
// The screen's heap and its archive of graphics. Our names
#define HEAPID_BTL_REC_SEL 0x75

// Black's, three more than Black 2's 185 (btl_rec_sel.c's code)
#define ARCID_BTL_REC_SEL 188

// What the screen is called with
typedef struct {
    GameData *gameData;
    // Whether the other player's game can record the battle; the can't-record-with-this-Trainer message if not
    BOOL compatible;
    // Whether the other player is at the same screen, which puts a time limit on the choice and waits for them at the
    // end
    BOOL comm;
    // Whether the battle was recorded at all; the last-battle-can't-be-recorded message if not
    BOOL recorded;
    // The battle mode's index and the header's counter, for func_0200be50
    u32 mode;
    s32 counter;
} BtlRecSelParam;

#endif // BWPORT_APP_BTL_REC_SEL_H
