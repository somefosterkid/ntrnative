#include "global.h"

#include "metatile_behavior.h"

s32 ov02_0224E4CC(u8 tile, s32 arg1);

s32 ov02_0224E4CC(u8 tile, s32 arg1) {
    if (arg1 != 0) {
        return 0;
    }
    return sub_0205BAD0(tile);
}

s32 ov02_0224E4DC(u8 tile, s32 arg1);

s32 ov02_0224E4DC(u8 tile, s32 arg1) {
    if (arg1 != 0) {
        return 0;
    }
    return MetatileBehavior_IsSurfableWater_thunk(tile);
}
