// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/manual.h: the lines in use so far.
#ifndef BWPORT_APP_MANUAL_H
#define BWPORT_APP_MANUAL_H

#include "types.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the values of MANUAL_ARC_BG_NCLR, MANUAL_ARC_OBJ_NCLR, MANUAL_ARC_BG_NCGR,
// MANUAL_ARC_OBJ_NCGR, MANUAL_ARC_TOP_NSCR, MANUAL_ARC_LIST_NSCR, MANUAL_ARC_EXPLAIN_NSCR, MANUAL_ARC_OBJ_NCER and
// MANUAL_ARC_OBJ_NANR in Black
// The members of ARCID_MANUAL
enum {
    MANUAL_ARC_BG_NCLR,
    MANUAL_ARC_OBJ_NCLR,
    MANUAL_ARC_BG_NCGR,
    MANUAL_ARC_OBJ_NCGR,
    MANUAL_ARC_TOP_NSCR,
    MANUAL_ARC_LIST_NSCR,
    MANUAL_ARC_EXPLAIN_NSCR,
    MANUAL_ARC_OBJ_NCER,
    MANUAL_ARC_OBJ_NANR,
    MANUAL_ARC_CATEGORY_DATA,
    MANUAL_ARC_CATEGORY_OFFSETS,
    MANUAL_ARC_TITLE_DATA,
    MANUAL_ARC_TITLE_OFFSETS,
};

// Not from swan: the screen's heap, a child of HEAPID_USER
#define HEAPID_MANUAL 0x90

// What the manual ends with: the menu closed, or back to the menu
enum {
    MANUAL_RESULT_CLOSE,
    MANUAL_RESULT_RETURN,
};

extern const GameProcFunctions data_ov225_021bbde4;

#endif // BWPORT_APP_MANUAL_H
