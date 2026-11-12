// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/marine_tube_board.h: the lines in use so
// far.
#ifndef BWPORT_APP_MARINE_TUBE_BOARD_H
#define BWPORT_APP_MARINE_TUBE_BOARD_H

#include "types.h"
#include "gfl/overlay.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
#define OVERLAY_MARINE_TUBE_BOARD OVERLAY_ID(318)

// Black 2's, not checked against Black
struct MarineTubeBoardParam {
    GameSystem *gsys;
    // Which board it is, which also picks its message
    u16 board;
};

#endif // BWPORT_APP_MARINE_TUBE_BOARD_H
