#include "global.h"

#include "bg_window.h"
#include "overlay_43_palpad.h"

void ov43_0222B408(PalPadMenu *param_1, PalPadGfx *param_2);

void ov43_0222B408(PalPadMenu *param_1, PalPadGfx *param_2) {
    u32 y;
    s32 iVar1;

    iVar1 = 0;
    y = 4;
    do {
        BgTilemapRectChangePalette(param_2->bgConfig, 2, 2, y, 0x1c, 4, 2);
        ScheduleBgTilemapBufferTransfer(param_2->bgConfig, 2);
        y = y + 5;
        iVar1 = iVar1 + 1;
    } while (iVar1 < 4);
    return;
}
