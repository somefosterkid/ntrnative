// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/dendou_pc.h: the lines in use so far.
#ifndef BWPORT_APP_DENDOU_PC_H
#define BWPORT_APP_DENDOU_PC_H

#include "types.h"
#include "struct_decls.h"

// The screen's heap, a child of HEAPID_USER
#define HEAPID_DENDOU_PC 0x8c

// Black 2's; twinned code confirms the values of DENDOU_PC_MODE_FIRST_CLEAR in Black
// What the screen shows
enum {
    // The team that finished the story, of save block 0x40
    DENDOU_PC_MODE_FIRST_CLEAR,
    // The teams of the Hall of Fame, extra save block 8
    DENDOU_PC_MODE_HALL_OF_FAME,
};

struct DendouPCParam {
    GameData *gameData;
    u16 mode;
    u16 result;
};

// How the screen was left
enum {
    DENDOU_PC_RESULT_RETURN,
    DENDOU_PC_RESULT_CLOSE,
};

// Black's, three more than Black 2's 210 (dpc_main.c's code)
// The screen's graphics
#define ARCID_DENDOU_PC 213

#endif // BWPORT_APP_DENDOU_PC_H
