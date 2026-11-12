// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/iss_switch_set.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_SYSTEM_ISS_SWITCH_SET_H
#define BWPORT_SYSTEM_ISS_SWITCH_SET_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// Black 2's, not checked against Black
// A switch of the zone's music. The Battle Subway only fades in switches below ISS_SWITCH_COUNT
typedef enum {
    ISS_SWITCH_COUNT = 9,
} ISSSwitchIndex;

ISSSwitchSet *ISSSwitchSet_Create(HeapID heapId);
// pokebw2's ISSSwitchSet_LoadArcData: the set's data, file datId of the switch archive (heapId unused)
void func_0202c318(ISSSwitchSet *set, u32 datId, HeapID heapId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void ISSSwitchSet_Free(ISSSwitchSet *set);
u32 ISSSwitchSet_GetBGMId(ISSSwitchSet *set);
BOOL ISSSwitchSet_CheckHasZone(ISSSwitchSet *set, u16 zoneId);
BOOL ISSSwitchSet_IsSwitchOn(ISSSwitchSet *set, ISSSwitchIndex switchIndex);
void ISSSwitchSet_Update(ISSSwitchSet *set);

#endif // BWPORT_SYSTEM_ISS_SWITCH_SET_H
