// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_recorder/br_bvrank_proc.h: the
// lines in use so far.
#ifndef BWPORT_APP_BATTLE_RECORDER_BR_BVRANK_PROC_H
#define BWPORT_APP_BATTLE_RECORDER_BR_BVRANK_PROC_H

#include "types.h"

// Black 2's; twinned code confirms the values of BR_BVRANK_MODE_SEARCH in Black
// The list the screen downloads from the Global Link, in its mode: one of the server's three lists, which the menu
// chooses, or the videos that the search screen asked for
enum {
    BR_BVRANK_MODE_LIST1,
    BR_BVRANK_MODE_LIST3,
    BR_BVRANK_MODE_LIST2,
    BR_BVRANK_MODE_SEARCH,
};

#endif // BWPORT_APP_BATTLE_RECORDER_BR_BVRANK_PROC_H
