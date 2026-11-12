// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/iss_road_sys.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_SYSTEM_ISS_ROAD_SYS_H
#define BWPORT_SYSTEM_ISS_ROAD_SYS_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

ISSRoadSys *ISSRoadSys_Create(PlayerState *player, HeapID heapId);
void ISSRoadSys_Update(ISSRoadSys *sys);
void ISSRoadSys_Enable(ISSRoadSys *sys);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ISSRoadSys_Free
void func_0202b1ec(ISSRoadSys *sys);
// pokebw2's ISSRoadSys_Disable
void func_0202b204(ISSRoadSys *sys);

#endif // BWPORT_SYSTEM_ISS_ROAD_SYS_H
