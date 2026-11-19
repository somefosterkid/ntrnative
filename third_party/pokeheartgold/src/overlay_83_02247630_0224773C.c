#include "global.h"

#include "sprite.h"

void ov83_0224773C(void **arg0, u32 arg1, s32 arg2);

void ov83_0224773C(void **arg0, u32 arg1, s32 arg2) {
    s32 var_r7;
    u32 var_r4;
    short new_var;
    void **var_r5;
    var_r5 = arg0;
    if (arg2 == 1) {
        var_r7 = GX_OAM_MODE_XLU;
    } else {
        var_r7 = GX_OAM_MODE_NORMAL;
    }
    new_var = 0U;
    var_r4 = 0;
    if (arg1 > new_var) {
        do {
            Sprite_SetOamMode((Sprite *)(*((Sprite **)(((u8 *)(*var_r5)) + 4))), (GXOamMode)var_r7);
            var_r4 += 1;
            var_r5 += 1;
        } while (var_r4 < arg1);
    }
}
