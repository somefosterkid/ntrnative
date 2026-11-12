// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/iss_switch_sys.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_SYSTEM_ISS_SWITCH_SYS_H
#define BWPORT_SYSTEM_ISS_SWITCH_SYS_H

#include "types.h"
#include "gfl/heap.h"
#include "system/iss_switch_set.h"
#include "struct_decls.h"

BOOL ISSSwitchSys_IsSwitchOn(ISSSwitchSys *switchSys, ISSSwitchIndex switchIndex);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
ISSSwitchSys *ISSSwitchSys_Create(HeapID heapId);
void ISSSwitchSys_Free(ISSSwitchSys *switchSys);
void ISSSwitchSys_Update(ISSSwitchSys *switchSys);
void ISSSwitchSys_Enable(ISSSwitchSys *switchSys);
void ISSSwitchSys_Disable(ISSSwitchSys *switchSys);
// Disables the system when the BGM's switch set does not play in the zone
void ISSSwitchSys_ChangeZone(ISSSwitchSys *switchSys, u16 zoneId);
void ISSSwitchSys_ReqSwitchFadeIn(ISSSwitchSys *switchSys, ISSSwitchIndex switchIndex);
void ISSSwitchSys_ReqSwitchFadeOut(ISSSwitchSys *switchSys, ISSSwitchIndex switchIndex);
// pokebw2's ISSSwitchSys_ResetSwitches
void func_0202c110(ISSSwitchSys *switchSys);

#endif // BWPORT_SYSTEM_ISS_SWITCH_SYS_H
