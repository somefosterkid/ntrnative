// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/iss_city_sys.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_SYSTEM_ISS_CITY_SYS_H
#define BWPORT_SYSTEM_ISS_CITY_SYS_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
ISSCitySys *ISSCitySys_Create(PlayerState *player, HeapID heapId);
void ISSCitySys_Update(ISSCitySys *sys);
void ISSCitySys_Enable(ISSCitySys *sys);
void ISSCitySys_Disable(ISSCitySys *sys);
void ISSCitySys_ChangeZone(ISSCitySys *sys, u16 zoneId);
// pokebw2's ISSCitySys_Free
void func_0202ab28(ISSCitySys *sys);

#endif // BWPORT_SYSTEM_ISS_CITY_SYS_H
