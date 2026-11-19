#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "overlay_67_app.h"

void ov67_021E6080(Ov67App *app) {
    Window *var_r5;
    s32 var_r4;
    s32 var_r6;

    FontID_Release(FONT_UNK_4);
    if (app->isRankingsScreen == 0) {
        var_r6 = 0xA;
    } else {
        var_r6 = 0x23;
    }
    var_r4 = 0;
    if (var_r6 > 0) {
        var_r5 = app->windows;
        do {
            ClearWindowTilemapAndCopyToVram(var_r5);
            RemoveWindow(var_r5);
            var_r4 += 1;
            var_r5++;
        } while (var_r4 < var_r6);
    }
}
