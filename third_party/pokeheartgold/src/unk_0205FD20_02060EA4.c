#include "global.h"

#include "metatile_behavior.h"
#include "field_types_def.h"

s32 sub_02060EA4(void *arg0, u16 arg1);

s32 sub_02060EBC(LocalMapObject *object, s32 tile);

s32 sub_02060EA4(void *arg0, u16 tile) {
    if (MetatileBehavior_IsSand(tile) != 0) {
        return 1;
    }
    return 0;
}

s32 sub_02060EBC(LocalMapObject *object, s32 tile) {
    if (MetatileBehavior_IsSnow((u8)tile) != 0) {
        return 1;
    }
    return 0;
}
