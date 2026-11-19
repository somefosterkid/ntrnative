#include "global.h"
#include "overlay_28.h"
#include "sprite.h"

void ov28_0225E3BC(DowsingMchnApp *app) {
    u16 sp0[2];
    u32 var_r4;
    u32 var_r4_2;
    if (app->signalStrength == 0) {
        var_r4 = 0;
        do {
            Sprite_SetDrawFlag(app->sprites[4 + var_r4], 0);
            var_r4 += 1;
        } while (var_r4 < 4U);
        return;
    }
    ov28_0225E374(app, &sp0[1], &sp0[0]);
    var_r4_2 = 0;
    do {
        Sprite_SetPalIndexRespectVramOffset(app->sprites[4 + var_r4_2], sp0[0] + 1);
        Sprite_SetAnimCtrlSeq(app->sprites[4 + var_r4_2], (s32)sp0[1]);
        Sprite_SetDrawFlag(app->sprites[4 + var_r4_2], 1);
        var_r4_2 += 1;
    } while (var_r4_2 < 4U);
}
