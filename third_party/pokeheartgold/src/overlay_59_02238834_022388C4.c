#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "sprite_system.h"

s32 ov59_02238C24(Ov59ApricornApp *);

s32 ov59_02238C7C(Ov59ApricornApp *);

u8 ov59_02239EA8(Ov59ApricornApp *);

extern s32 ov59_0223C6C4;

extern s32 ov59_0223C6C6;

void ov59_022388C4(Ov59ApricornApp *arg0);

void ov59_022388C4(Ov59ApricornApp *arg0) {
    s16 var_r6;
    s16 var_r4;
    s32 temp_r0;
    s32 temp_r0_2;
    temp_r0 = ov59_02239EA8(arg0);
    switch (temp_r0) {
    case 7:
        var_r6 = 0xC8;
        var_r4 = 0x5C;
        ov59_02238C24(arg0);
        break;

        temp_r0 = temp_r0 * 4;
    case 8:
        var_r6 = 0xF0;
        var_r4 = 0xA8;
        break;

    default:
        temp_r0_2 = temp_r0;
        var_r6 = (*((u16 *)((&ov59_0223C6C4) + temp_r0_2))) + 4;
        var_r4 = (*((u16 *)((&ov59_0223C6C6) + temp_r0_2))) - 0x10;
        break;
    }

    Sprite_SetPositionXY(arg0->sprites[1], var_r6, var_r4);
    ov59_02238C7C(arg0);
}
