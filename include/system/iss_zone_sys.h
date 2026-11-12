// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/iss_zone_sys.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_SYSTEM_ISS_ZONE_SYS_H
#define BWPORT_SYSTEM_ISS_ZONE_SYS_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

ISSZoneSys *ISSZoneSys_Create(HeapID heapId);
void ISSZoneSys_Update(ISSZoneSys *sys);
void ISSZoneSys_ChangeZone(ISSZoneSys *sys, u16 zoneId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void ISSZoneSys_Enable(ISSZoneSys *sys, u16 zoneId);
// pokebw2's ISSZoneSys_Free
void func_0202bea4(ISSZoneSys *sys);
// pokebw2's ISSZoneSys_Disable
void func_0202bee4(ISSZoneSys *sys);

#endif // BWPORT_SYSTEM_ISS_ZONE_SYS_H
