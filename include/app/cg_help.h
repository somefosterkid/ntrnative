// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/cg_help.h: the lines in use so far.
#ifndef BWPORT_APP_CG_HELP_H
#define BWPORT_APP_CG_HELP_H

#include "types.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the offsets of playerInfo in Black
// Without a parameter the process makes its own, with a player of the gender that the R button held at the start picks
typedef struct {
    PlayerInfo *playerInfo;
    GameSystem *gsys;
} CGHelpParam;

// pokebw2's CG_HELP_PROC_FUNCTIONS
extern GameProcFunctions data_ov124_021d4d40;

#endif // BWPORT_APP_CG_HELP_H
