#include "global.h"
#include "overlay_07_battle_anim_common.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov07_022220B8(void *, void *, s16, s16);
#else
s32 ov07_022220B8(void *, void *, s32, s32);
#endif

s32 ov07_022227D8(XYTransformContext *ctx);

s32 ov07_0222283C(void *arg0, s32 arg1, s32 arg2, void *arg3);

s32 ov07_0222283C(void *arg0, s32 arg1, s32 arg2, void *arg3) {
    if (ov07_022227D8(arg0) != 0) {
        ov07_022220B8(arg0, arg3, arg1, arg2);
        return 1;
    }
    return 0;
}
