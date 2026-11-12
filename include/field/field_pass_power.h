// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_pass_power.h: the lines in use
// so far.
#ifndef BWPORT_FIELD_FIELD_PASS_POWER_H
#define BWPORT_FIELD_FIELD_PASS_POWER_H

#include "types.h"
#include "struct_decls.h"

// pokebw2's EventPassPowerList_Create
// Lists the active pass powers and the time each has left, until A or B is pressed
// A GameEventProvider: args is the Field
GameEvent *func_ov083_021f5368(GameSystem *gsys, void *args);

// pokebw2's EventPassPowerFlash_Create
// Flashes the screens white with a sound (script command 0x1DF)
GameEvent *func_ov083_021f5340(GameSystem *gsys);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's EventPassPowerActivate_Create
GameEvent *func_ov083_021f5260(GameSystem *gsys, void *args);
// pokebw2's EventPassPowerDepleted_Create
GameEvent *func_ov083_021f532c(GameSystem *gsys, void *args);

#endif // BWPORT_FIELD_FIELD_PASS_POWER_H
