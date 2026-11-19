#include "global.h"

#include "metatile_behavior.h"

int ov01_02200AB0(u8 param0);

int ov01_02200AB0(u8 param0) {
    if (MetatileBehavior_IsWarpEntranceNorth(param0)) {
        return 0;
    }

    if (MetatileBehavior_IsWarpEntranceSouth(param0)) {
        return 1;
    }

    if (MetatileBehavior_IsWarpEntranceWest(param0)) {
        return 2;
    }

    if (MetatileBehavior_IsWarpEntranceEast(param0)) {
        return 3;
    }

    return -1;
}
