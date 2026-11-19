#include "global.h"

#include "bg_window.h"
#include "overlay_67_app.h"

void ov67_021E6490(Ov67App *app) {
    u16 i;
    u16 j;
    Window *iVar2;
    u32 uVar4;

    if (app->unk_4A2 == 0) {
        iVar2 = &app->windows[11];
    } else {
        iVar2 = &app->windows[23];
    }
    i = app->unk_4A0 * 12;
    for (j = 0; j < 12; i++, j++) {
        FillWindowPixelBuffer(&iVar2[j], 0);
        if (i < 0x1e) {
            if (app->wifiTrainers[i].anonNameIdx == 0) {
                uVar4 = 0x70800;
            } else {
                uVar4 = 0x30400;
            }
            ov67_021E6118(&iVar2[j], app->wifiTrainers[i].name, 0, 0, FONT_SYSTEM, uVar4, 0);
        }
        ScheduleWindowCopyToVram(&iVar2[j]);
    }
    app->unk_4A2 = app->unk_4A2 ^ 1;
}
