// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/map_matrix.c, ported by tools/port_file.py.
#include "types.h"
#include "field/field_map.h"
#include "field/zone.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "nitro/mi.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
u16 GetVersionedMapChangeZoneNum2(u16 zoneId);
void *sys_memcpy32(const void *src, void *dest, u32 size);
void sys_memset16(u16 value, void *dest, u32 size);

MapMatrix *InitMapMatrix(HeapID heapId) {
    MapMatrix *matrix = GFL_HeapAllocate(heapId, sizeof(MapMatrix), TRUE, "map_matrix.c", 0x4b);

    matrix->heapId = heapId;
    MIi_CpuClear32((u32)-1, matrix->chunkIds, sizeof(matrix->chunkIds));
    MIi_CpuClear32(0xffff, matrix->zoneIds, sizeof(matrix->zoneIds));
    return matrix;
}

void MapMatrix_Load(MapMatrix *matrix, u16 matrixId, u16 zoneId, HeapID heapId) {
    void *data = GFL_ArcSysReadHeapNew(9, matrixId, heapId);

    ProcessMapMatrix(matrix, data, matrixId, zoneId);
    GFL_HeapFree(data);
}

void FreeMapMatrix(void *ptr) {
    GFL_HeapFree(ptr);
}


u16 GetZoneIDAtMatrixXZ(MapMatrix *matrix, s32 x, s32 z) {
    return matrix->zoneIds[z * matrix->width + x];
}

u32 GetZoneIDAtMatrixXZWorld(MapMatrix *matrix, s32 x, s32 z) {
    return GetZoneIDAtMatrixXZ(matrix, GetChunkCoordOfWorld(x), GetChunkCoordOfWorld(z));
}


u16 GetMapMatrixWidth(MapMatrix *matrix) {
    return matrix->width;
}

u16 GetMapMatrixHeight(MapMatrix *matrix) {
    return matrix->height;
}

u32 GetMapMatrixChunkIDCount(MapMatrix *matrix) {
    return matrix->chunkIdCount;
}

void func_ov010_02159c00(MapMatrix *matrix, u8 width, u8 height) {
    u32 chunkIdCount = width * height;
    if (width <= 0x1E && height <= 0x1E && chunkIdCount <= 900) {
        matrix->width = width;
        matrix->height = height;
        matrix->chunkIdCount = chunkIdCount;
    }
}


u32 *GetMapMatrixChunkIDs(MapMatrix *matrix) {
    return matrix->chunkIds;
}

BOOL RangeCheckChunkCoordinate(MapMatrix *matrix, s32 x, s32 z) {
    return z >= 0 && z < matrix->height && x >= 0 && x < matrix->width;
}

BOOL RangeCheckChunkCoordinateWorld(MapMatrix *matrix, s32 x, s32 z) {
    return RangeCheckChunkCoordinate(matrix, GetChunkCoordOfWorld(x), GetChunkCoordOfWorld(z));
}

void MapReplace_Patch(MapMatrix *matrix, MapReplace *replace, HeapID heapId) {
    u32 oldValue;
    u32 newValue;
    u32 result = MapReplace_ResolvePatch(replace, &oldValue, &newValue);
    u32 i;

    switch (result) {
    case 2:
        MapMatrix_Load(matrix, (u16)newValue, matrix->zoneId, heapId);
        return;
    case 1:
        for (i = 0; i < matrix->chunkIdCount; i++) {
            if (matrix->chunkIds[i] == oldValue) {
                matrix->chunkIds[i] = newValue;
            }
        }
        break;
    case 0:
        break;
    }
}

void MapMatrix_Patch(MapMatrix *matrix, GameSystem *gsys, HeapID heapId) {
    MapReplace *replace = MapReplace_Create(heapId, gsys);
    s32 count = MapReplace_GetEntryCount(replace);
    s32 i;

    for (i = 0; i < count; i++) {
        s32 entryId = MapReplace_LoadEntry(replace, i);

        if (entryId == matrix->matrixId) {
            MapReplace_Patch(matrix, replace, heapId);
        }
    }
    MapReplace_Free(replace);
}

void ProcessMapMatrix(MapMatrix *matrix, const MapMatrixFileHeader *data, u32 matrixId, u16 zoneId) {
    u16 width;
    u16 height;
    const u32 *chunkIds;
    u16 *zoneIds;

    matrix->zoneId = zoneId;
    matrix->matrixId = matrixId;
    width = data->width;
    height = data->height;
    chunkIds = data->chunkIds;
    matrix->height = height;
    matrix->chunkIdCount = width * height;
    matrix->format = data->format;
    matrix->width = width;
    chunkIds = data->chunkIds;
    MIi_CpuCopy32(chunkIds, matrix->chunkIds, matrix->chunkIdCount * sizeof(u32));
    zoneIds = matrix->zoneIds;
    if (matrix->format == 1) {
        const u32 *zones = &chunkIds[matrix->chunkIdCount];
        u32 i;
        for (i = 0; i < matrix->chunkIdCount; i++) {
            zoneIds[i] = (u16)zones[i];
        }
    } else {
        MIi_CpuClear16(zoneId, matrix->zoneIds, sizeof(matrix->zoneIds));
    }
}

u32 GetChunkCoordOfWorld(s32 coordinate) {
    return ((coordinate / 0x1000) / 16) / 32;
}
