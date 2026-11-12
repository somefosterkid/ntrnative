// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/iss_city_unit.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_SYSTEM_ISS_CITY_UNIT_H
#define BWPORT_SYSTEM_ISS_CITY_UNIT_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u16 ISSCityUnit_GetZoneID(const ISSCityUnit *unit);
// The city tracks' volume, 0 to 127, at a position
u8 ISSCityUnit_CalcEmitterVolume(const ISSCityUnit *unit, const VecFx32 *pos);
// Loads unit `index` of ARCID_ISS_CITY
ISSCityUnit *ISSCityUnit_Create(HeapID heapId, u32 index);
// pokebw2's ISSCityUnit_Free
void func_0202af28(ISSCityUnit *unit);

#endif // BWPORT_SYSTEM_ISS_CITY_UNIT_H
