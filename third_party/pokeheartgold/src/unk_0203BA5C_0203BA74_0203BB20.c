#include "global.h"

extern u16 sSpawnMaps;

u32 MapHeader_GetSpawnIdForDeathWarp(u32 mapId);

u32 MapHeader_GetSpawnIdForDeathWarp(u32 mapId) {
    u16 *new_var[2];
    u32 var_r2;
    new_var[1] = &sSpawnMaps;
    var_r2 = 0;
    new_var[0] = &sSpawnMaps;
loop_1:
    if ((mapId == (*((u16 *)(((u8 *)new_var[1]) + 2)))) && ((((u32)((*new_var[0]) << 0x17)) >> 0x1F) != 0)) {
        return var_r2 + 1;
    }

    var_r2 += 1;
    new_var[1] += 0x9;
    new_var[0] += 0x9;
    if (var_r2 >= 0x1EU) {
        return 0U;
    }
    goto loop_1;
}
