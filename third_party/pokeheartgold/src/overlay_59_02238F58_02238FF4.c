#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "sprite.h"

s32 ov59_022387D0(Ov59ApricornApp *, s32);

s32 ov59_0223891C(Ov59ApricornApp *, s32);

void ov59_02238FF4(Ov59ApricornApp *arg0, s32 arg1);

void ov59_02238FF4(Ov59ApricornApp *arg0, s32 arg1) {
    s32 var_r4;
    u8 temp_r0;
    u8 temp_r0_2;

    if ((u32)(arg1 - 1) <= 1U) {
        var_r4 = 0;
        do {
            if ((var_r4 == arg0->selected) && (arg1 == 2)) {
                Sprite_SetOamMode(arg0->sprites[var_r4 + 2], GX_OAM_MODE_NORMAL);
            } else {
                Sprite_SetOamMode(arg0->sprites[var_r4 + 2], GX_OAM_MODE_XLU);
            }
            var_r4 += 1;
        } while (var_r4 < 7);
        ov59_0223891C(arg0, 0);
        ov59_022387D0(arg0, 2);
        temp_r0 = arg0->selected;
        if ((u32)temp_r0 < 7U) {
            Sprite_SetDrawPriority(arg0->sprites[temp_r0 + 2], 2U);
        }
    } else {
        var_r4 = 0;
        do {
            Sprite_SetOamMode(arg0->sprites[var_r4 + 2], GX_OAM_MODE_NORMAL);
            var_r4 += 1;
        } while (var_r4 < 7);
        ov59_0223891C(arg0, 1);
        ov59_022387D0(arg0, 0);
        temp_r0_2 = arg0->selected;
        if ((u32)temp_r0_2 < 7U) {
            Sprite_SetDrawPriority(arg0->sprites[temp_r0_2 + 2], 3U);
        }
    }
}
