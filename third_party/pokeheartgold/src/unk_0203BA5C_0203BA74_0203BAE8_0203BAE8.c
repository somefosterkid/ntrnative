#include "global.h"

#include "field_types_def.h"
#include "unk_0203BA5C.h"

extern s32 sSpawnMaps;

void GetSpecialSpawnWarpData(u16 spawnId, Location *dest) {
    s32 temp_r1;

    temp_r1 = SpawnIdToTableIndex((s32)spawnId) * 0x12;
    dest->mapId = (s32) * (u16 *)(((u8 *)&sSpawnMaps) + 0xC + temp_r1);
    dest->warpId = -1;
    dest->x = (s32) * (u16 *)(((u8 *)&sSpawnMaps) + 0xE + temp_r1);
    dest->y = (s32) * (u16 *)(((u8 *)&sSpawnMaps) + 0x10 + temp_r1);
    dest->direction = 1;
}
