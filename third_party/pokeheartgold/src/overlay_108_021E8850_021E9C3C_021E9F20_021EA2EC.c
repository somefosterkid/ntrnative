#include "global.h"
#include "overlay_108_021E8850.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_108_safari_decoration_app.h"

void ov108_021EA2EC(SafariDecorationApp *arg0, u8 arg1) {
    s32 var_r5;
    SafariDecorationApp *var_r4;
    s16 var_r3;
    s16 var_r2;

    var_r4 = arg0;
    if (arg1 >= 6U) {
        var_r3 = 0xE0;
        var_r2 = 0xB4;
        var_r5 = 1;
    } else {
        var_r3 = (arg1 % 2) << 7;
        var_r2 = 0x28 * ((arg1 / 2) + 1);
        var_r5 = 0;
    }
    Sprite_SetPositionXY(var_r4->sprites[0], var_r3, var_r2);
    Sprite_SetAnimCtrlSeq(var_r4->sprites[0], var_r5);
}
