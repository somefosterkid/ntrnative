#include "global.h"

#include "save_vars_flags.h"
#include "sys_flags.h"
#include "field_system.h"

extern u16 sSpawnMaps;

void FlypointFlagAction(FieldSystem *arg0, s32 arg1);

void FlypointFlagAction(FieldSystem *arg0, s32 arg1) {
    u16 *var_r2;
    u16 *var_r3;
    u32 var_r4;
    var_r2 = &sSpawnMaps;
    var_r4 = 0;
    var_r3 = &sSpawnMaps;
loop_1:
    if ((arg1 == (*((u16 *)(((u8 *)var_r2) + 6)))) && ((((u32)((*var_r3) << 0x16)) >> 0x1F) != 0)) {
        Save_VarsFlags_FlypointFlagAction(Save_VarsFlags_Get(arg0->saveData), 1U, (u32)((u8)(*((&sSpawnMaps) + var_r4 * 9))));
        return;
    }

    var_r4 += 1;
    var_r2 += 9;
    var_r3 += 9;
    if (var_r4 >= 0x1EU) {
        return;
    }
    goto loop_1;
}
