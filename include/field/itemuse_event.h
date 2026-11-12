// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/itemuse_event.h, the lines in use so
// far.
#ifndef BWPORT_FIELD_ITEMUSE_EVENT_H
#define BWPORT_FIELD_ITEMUSE_EVENT_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

GameEvent *CallFieldCommonEventFunc(u32 id, GameSystem *gsys, Field *field);

GameEvent *EventFieldToggleCycling_Create(Field *field, GameSystem *gsys);
GameEvent *EventEntralinkWarpIn_CreateDefault(Field *field, GameSystem *gsys);
GameEvent *EventFieldToggleDowsing_Create(Field *field, GameSystem *gsys);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void PlayerActionPerms_Create(PlayerActionPerms *perms, GameSystem *gsys, Field *field);

#endif // BWPORT_FIELD_ITEMUSE_EVENT_H
