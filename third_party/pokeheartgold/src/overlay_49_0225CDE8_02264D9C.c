#include "global.h"

#include "overlay_49.h"

u8 ov45_0222AA10(void *);

s32 ov49_0225A3AC(void *, s32, s32);

s32 ov49_0225A3BC(void *, s32, s32);

s32 ov49_0225A3CC(void *, s32, s32);

s32 ov49_0225A3DC(void *, s32, s32);

s32 ov49_0225A3EC(void *, s32, s32);

s32 ov49_0225A3FC(void *, s32, s32);

void ov49_02264D9C(void *arg0, s32 arg1, s32 arg2, void *arg3) {
    u32 v0;

    v0 = ov45_0222AA10(arg3);
    switch (v0) {
    case 1:
        ov49_0225A3AC(arg0, arg1, arg2);
        return;
    case 2:
        ov49_0225A3BC(arg0, arg1, arg2);
        return;
    case 3:
        ov49_0225A3CC(arg0, arg1, arg2);
        return;
    case 4:
        ov49_0225A3DC(arg0, arg1, arg2);
        return;
    case 5:
        ov49_0225A3EC(arg0, arg1, arg2);
        return;
    case 7:
        ov49_0225A3FC(arg0, arg1, arg2);
        return;
    default:
        ov49_0225A3BC(arg0, arg1, arg2);
        return;
    }
}
