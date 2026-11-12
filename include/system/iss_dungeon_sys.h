// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/iss_dungeon_sys.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_SYSTEM_ISS_DUNGEON_SYS_H
#define BWPORT_SYSTEM_ISS_DUNGEON_SYS_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

void ISSDungeonSys_Free(ISSDungeonSys *sys);
void ISSDungeonSys_Update(ISSDungeonSys *sys);
// Whether the dungeon archive lists the zone
BOOL ISSDungeonSys_IsZoneRegist(ISSDungeonSys *sys, u16 zoneId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
ISSDungeonSys *ISSDungeonSys_Create(GameData *gameData, PlayerState *playerState, HeapID heapId);
void ISSDungeonSys_Disable(ISSDungeonSys *sys);
// The zone changes on the next update
void ISSDungeonSys_ReqChangeZone(ISSDungeonSys *sys, u16 zoneId);
void ISSDungeonSys_Enable(ISSDungeonSys *sys);
BOOL ISSDungeonSys_IsEnabled(ISSDungeonSys *sys);

#endif // BWPORT_SYSTEM_ISS_DUNGEON_SYS_H
