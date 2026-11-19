#include "global.h"

#include "unk_0203BA5C.h"

extern s32 sSpawnMaps;

void GetFlyWarpData(s32 arg0, void *arg1);

void GetFlyWarpData(s32 arg0, void *arg1) {
    s32 temp_r1;

    temp_r1 = SpawnIdToTableIndex(arg0) * 0x12;
    *(s32 *)arg1 = (s32) * (u16 *)(((u8 *)&sSpawnMaps) + 6 + temp_r1);
    *(s32 *)((u8 *)arg1 + 4) = -1;
    *(s32 *)((u8 *)arg1 + 8) = (s32) * (u16 *)(((u8 *)&sSpawnMaps) + 8 + temp_r1);
    *(s32 *)((u8 *)arg1 + 0xC) = (s32) * (u16 *)(((u8 *)&sSpawnMaps) + 0xA + temp_r1);
    *(s32 *)((u8 *)arg1 + 0x10) = 1;
}
