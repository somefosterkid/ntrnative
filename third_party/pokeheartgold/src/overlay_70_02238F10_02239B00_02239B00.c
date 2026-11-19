#include "global.h"

#include "bg_window.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02239C34(Window *, void *, s32, s32, s32);

extern s32 ov70_0224526C;

void ov70_02239B00(GTSApplicationState *arg0);

void ov70_02239B00(GTSApplicationState *arg0) {
    Window *var_r4;
    s32 var_r6;
    s32 *var_r5;
    int new_var5;
    long long new_var3;
    new_var5 = 0x10;
    var_r4 = &arg0->menuButtonWindows[0];
    var_r6 = (new_var3 = (arg0->isPokemonListed));
    var_r5 = (&ov70_0224526C) + (var_r6 * 3);
    var_r6 = 0;
    do {
        FillWindowPixelRect(var_r4, 0xFU, 0U, 0U, 0x78U, new_var5 = 8U);
        FillWindowPixelRect(var_r4, 0xEU, 0U, 8U, 0x78U, 2U);
        FillWindowPixelRect(var_r4, 0xDU, 0U, 0xAU, 0x78U, 6U);
        ov70_02239C34(var_r4, arg0->gtsMessageLoader, 4, *var_r5, 0);
        CopyWindowToVram(var_r4);
        var_r6 += 1;
        var_r4 += 1;
        var_r5 += 1;
    } while (var_r6 < 3);
}
