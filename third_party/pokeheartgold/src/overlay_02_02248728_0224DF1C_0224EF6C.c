#include "global.h"

#include "overlay_2/overlay_02_02248728.h"

void ov02_0224EF6C(u8 arg0, s32 arg1, s32 arg2);

void ov02_0224EF6C(u8 tile, s32 arg1, s32 arg2) {
    if (arg2 == 0) {
        ov02_0224E4CC(tile, arg1);
        return;
    }
    ov02_0224E4DC(tile, arg1);
}
