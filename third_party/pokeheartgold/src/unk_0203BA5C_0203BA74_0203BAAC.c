#include "global.h"

#include "field_types_def.h"

s32 SpawnIdToTableIndex(s32 arg0);

extern s32 sSpawnMaps;

void GetDeathWarpData(u16 spawnId, Location *dest);

void GetDeathWarpData(u16 spawnId, Location *dest) {
    s32 temp_r2;
    Location *new_var;
    void *temp_r1;
    temp_r2 = SpawnIdToTableIndex(spawnId) * 0x12;
    dest->mapId = (s32)(*((u16 *)((((u8 *)&sSpawnMaps) + 2) + temp_r2)));
    dest->warpId = -1;
    temp_r1 = ((u8 *)&sSpawnMaps) + temp_r2;
    new_var = dest;
    new_var->x = (s32)((u8)(*((u16 *)(((u8 *)temp_r1) + 4))));
    dest->y = (s32)(((u32)((*((u16 *)(((u8 *)temp_r1) + 4))) << 0x10)) >> 0x18);
    dest->direction = 0;
}
