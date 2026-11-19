#include "global.h"

#include "sprite.h"
#include "overlay_112_sub_screen.h"

void ov112_021F0980(Ov112SubScreen *arg0);

void ov112_021F0980(Ov112SubScreen *arg0) {
    u8 var_r4;
    Ov112SubScreenSprites *temp_r5;

    temp_r5 = &arg0->sprites;
    for (var_r4 = 0; (u32)var_r4 < 6U; var_r4 += 1) {
        Sprite_SetDrawFlag(temp_r5->slotSprites[var_r4], 0);
    }
    Sprite_SetDrawFlag(temp_r5->monSprite, 0);
}
