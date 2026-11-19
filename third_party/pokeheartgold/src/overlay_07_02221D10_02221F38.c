#include "global.h"
#include "overlay_07_battle_anim_funcs.h"

s32 ov07_0221BFC0(void *arg0);

s32 ov07_02221F04(s32, s32, s16 *, s16 *);

void ov07_02221F38(void *arg0, s32 arg1, void *arg2, void *arg3);

void ov07_02221F38(void *arg0, s32 arg1, void *arg2, void *arg3) {
    if (ov07_0221BFC0(arg0) == 1) {
        ov07_02221F04(0, arg1 + 2, arg2, arg3);
        return;
    }
    if (ov07_0221FAB0(arg0) == 1) {
        ov07_02221F04(1, arg1 - 2, arg2, arg3);
        return;
    }
    ov07_02221F04(0, arg1, arg2, arg3);
}
