// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/townmap_util.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_TOWNMAP_UTIL_H
#define BWPORT_FIELD_TOWNMAP_UTIL_H

#include "types.h"
#include "struct_decls.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The zone the town map shows for a zone, after the zones whose place depends on the game
u16 func_ov010_02166c7c(GameData *gameData, u16 zoneId);
// Whether the player has the event flag that shows a place on the map
BOOL func_ov010_02166cd0(GameData *gameData, u16 flag);

#endif // BWPORT_FIELD_TOWNMAP_UTIL_H
