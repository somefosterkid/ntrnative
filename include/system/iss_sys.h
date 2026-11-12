// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/iss_sys.h, the lines in use so far.
#ifndef BWPORT_SYSTEM_ISS_SYS_H
#define BWPORT_SYSTEM_ISS_SYS_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

ISS3DSoundSys *ISS_Get3DSoundSys(ISS *iss);

void ISS_ChangeZone(ISS *iss, u16 zoneId);

void ISS_Update(ISS *iss);

// Black 2's; twinned code confirms the values of ISS_SUBSYSTEM_COUNT in Black
// The subsystem a BGM uses, from the BGM info archive
enum {
    ISS_SUBSYSTEM_NONE,
    // 1 and 6 have no subsystem
    ISS_SUBSYSTEM_UNUSED_1,
    ISS_SUBSYSTEM_ROAD,
    ISS_SUBSYSTEM_CITY,
    ISS_SUBSYSTEM_3D_SOUND,
    ISS_SUBSYSTEM_DUNGEON,
    ISS_SUBSYSTEM_UNUSED_6,
    ISS_SUBSYSTEM_SWITCH,
    ISS_SUBSYSTEM_ZONE,
    ISS_SUBSYSTEM_COUNT,
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
ISSSwitchSys *ISS_GetSwitchSys(ISS *iss);
ISS *ISS_Create(GameData *gameData, HeapID heapId);
void ISS_Free(ISS *iss);
ISSDungeonSys *ISS_GetDungeonSys(ISS *iss);

#endif // BWPORT_SYSTEM_ISS_SYS_H
