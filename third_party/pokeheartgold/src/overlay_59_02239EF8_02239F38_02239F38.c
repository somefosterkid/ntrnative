#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "sprite.h"
#include "sprite_system.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"

extern TouchscreenHitbox ov59_0223C940;

s32 ov59_02239F38(Ov59ApricornApp *arg0);

s32 ov59_02239F38(Ov59ApricornApp *arg0) {
    s16 x;
    s16 y;
    u8 var_r4;

    Sprite_GetPositionXY(arg0->sprites[arg0->selected + 2], &x, &y);
    var_r4 = (u8)TouchscreenHitbox_PointIsIn(&ov59_0223C940, (u32)x, (u32)y);
    if (System_GetTouchHeld() != 0) {
        x = (s16)gSystem.touchX;
        y = (s16)gSystem.touchY;
        Sprite_SetPositionXY(arg0->sprites[arg0->selected + 2], x, y);
        Sprite_SetPositionXY(arg0->sprites[1], x, (s16)(y - 6));
        if (var_r4 != 0) {
            if (arg0->unk_53 == 0) {
                arg0->unk_53 = 1U;
                PlaySE(0x8E6U);
                Sprite_SetAnimCtrlSeq(arg0->sprites[10], 0x15);
            }
        } else if (arg0->unk_53 != 0) {
            arg0->unk_53 = 0U;
            PlaySE(0x8E6U);
            Sprite_SetAnimCtrlSeq(arg0->sprites[10], 0x14);
        }
        return -1;
    }
    arg0->unk_53 = 0;
    if (var_r4 != 0) {
        PlaySE(0x5EAU);
        return 1;
    }
    PlaySE(0x682U);
    return 0;
}
