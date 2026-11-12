// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/area_data.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_SYSTEM_AREA_DATA_H
#define BWPORT_SYSTEM_AREA_DATA_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// -1 if the area has none
u32 AreaData_GetSRTAnmID(AreaData *areaData);
u32 AreaData_GetPatAnmID(AreaData *areaData);

// No animation of that kind
#define AREA_ANM_NONE 0xff

// Black 2's; matching code confirms the offsets of srtAnmId, patAnmId, isExterior, lightsId, edgeColorTableId and
// actorMatColorId in Black
// One record of ARCID_AREADATA
struct AreaData {
    u16 propBundleId;
    u16 texSetId;
    u8 srtAnmId;
    u8 patAnmId;
    u8 isExterior;
    u8 lightsId;
    u8 edgeColorTableId;
    u8 actorMatColorId;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Reads the record of areaId + variant, or record 0 if there is none. An area with seasons has one record per season
AreaData *AreaData_Create(HeapID heapId, u16 areaId, u32 variant);
void AreaData_Free(AreaData *areaData);
u32 AreaData_GetTexSetID(AreaData *areaData);
BOOL AreaData_IsExterior(AreaData *areaData);
u8 func_020140dc(AreaData *areaData);
// Whether the area has one record per season
BOOL func_020140e8(u32 areaId);
u8 AreaData_GetEdgeColorTableID(AreaData *areaData);
u8 AreaData_GetActorMatColorID(AreaData *areaData);

#endif // BWPORT_SYSTEM_AREA_DATA_H
