#include "global.h"
#include "overlay_112_sprite_app.h"

u8 ov112_021F0F60(Ov112Record *arg0);

s32 ov112_021F0F70(u32 **arg0, u32 **arg1);

u8 ov112_021F0F60(Ov112Record *arg0) {
    u16 v0;

    v0 = arg0->page;
    if ((u32)v0 >= 8U) {
        v0 = 0;
    }
    return v0;
}

s32 ov112_021F0F70(u32 **arg0, u32 **arg1) {
    u32 v0;
    u32 v1;

    v1 = **arg1;
    v0 = **arg0;
    if (v0 == v1) {
        return 0;
    }
    if (v0 > v1) {
        return 1;
    }
    return -1;
}
