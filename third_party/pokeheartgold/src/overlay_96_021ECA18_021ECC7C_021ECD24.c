#include "global.h"

#include "sprite_system.h"
#include "unk_02005D10.h"
#include "overlay_96_app_021ed1d0.h"

u32 ov98_0221F120(u32, s32);

void ov96_021ECD24(Ov96PlayerSlot *arg0);

void ov96_021ECD24(Ov96PlayerSlot *arg0) {
    s32 var_r4;
    ManagedSprite **var_r5;
    u16 temp_r7;
    u16 temp_r5;
    u16 temp_r4;
    u32 var_r1;
    if (arg0->counting != 0) {
        var_r1 = arg0->shown + 1;
        if (var_r1 > 0x3E7U) {
            var_r1 = 0x3E7;
        }
        arg0->shown = var_r1;
        if (IsSEPlaying(0x8E3U) == 0) {
            PlaySE(0x8E3U);
        }
        temp_r7 = ov98_0221F120(arg0->shown, 3);
        temp_r5 = ov98_0221F120(arg0->shown, 2);
        temp_r4 = ov98_0221F120(arg0->shown, 1);
        ManagedSprite_SetAnim(arg0->sprites[1], temp_r7 + 1);
        ManagedSprite_SetAnim(arg0->sprites[2], temp_r5 + 1);
        ManagedSprite_SetAnim(arg0->sprites[3], temp_r4 + 1);
        if ((((u32)arg0->shown) % 20U) == 0) {
            var_r4 = 0;
            var_r5 = arg0->sprites;
            do {
                ManagedSprite_OffsetPositionXY(*var_r5, 0, -1);
                var_r4 += 1;
                var_r5 += 1;
            } while (var_r4 < 4);
        }
        if (((u32)arg0->shown) >= ((u32)arg0->target)) {
            arg0->counting = 0;
        }
    }
}
