// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/area_data.c, ported by tools/port_file.py.
#include "system/area_data.h"
#include "types.h"
#include "constants/arc.h"
#include "gfl/heap.h"
#include "system/file_util.h"

// An area's record, which names the graphics its zones share. The ROM doesn't name the file; the name is a guess.
// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#define AREA_COUNT 0x199

// Black's area data has 282 records
#define AREA_DATA_COUNT 282

// Reads the record of areaId + variant (the season of an area that has one record per season), or record 0 if there
// is none
AreaData *AreaData_Create(HeapID heapId, u16 areaId, u32 variant) {
    if (func_020140e8(areaId)) {
        areaId += variant;
    }
    if (areaId >= AREA_DATA_COUNT) {
        areaId = 0;
    }
    return GFL_ArcSysReadRawResource(ARCID_AREADATA, heapId, areaId * sizeof(AreaData), sizeof(AreaData));
}

void AreaData_Free(AreaData *areaData) {
    GFL_HeapFree(areaData);
}

u32 AreaData_GetTexSetID(AreaData *areaData) {
    return areaData->texSetId;
}

u32 AreaData_GetSRTAnmID(AreaData *areaData) {
    u32 id = areaData->srtAnmId;

    if (id == AREA_ANM_NONE) {
        return 0xffffffff;
    }
    return id;
}

u32 AreaData_GetPatAnmID(AreaData *areaData) {
    u32 id = areaData->patAnmId;

    if (id == AREA_ANM_NONE) {
        return 0xffffffff;
    }
    return id;
}

BOOL AreaData_IsExterior(AreaData *areaData) {
    return areaData->isExterior;
}

u8 func_020140dc(AreaData *areaData) {
    return areaData->lightsId;
}

u8 AreaData_GetEdgeColorTableID(AreaData *areaData) {
    return areaData->edgeColorTableId;
}

u8 AreaData_GetActorMatColorID(AreaData *areaData) {
    return areaData->actorMatColorId;
}

BOOL func_020140e8(u32 areaId) {
    if (areaId >= 2 && areaId < 0xd2) {
        return TRUE;
    }
    return FALSE;
}

