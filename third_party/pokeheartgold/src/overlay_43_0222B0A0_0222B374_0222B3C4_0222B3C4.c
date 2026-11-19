#include "global.h"

#include "bg_window.h"
#include "overlay_43_palpad.h"

void ov43_0222B3C4(PalPadMenu *param_1, PalPadGfx *param_2);

void ov43_0222B3C4(PalPadMenu *param_1, PalPadGfx *param_2) {
    u8 palette;
    s32 iVar1;
    u32 y;

    iVar1 = 0;
    y = 4;
    do {
        if (param_1->selection == iVar1) {
            palette = 9;
        } else {
            palette = 2;
        }
        BgTilemapRectChangePalette(param_2->bgConfig, 2, 2, y, 0x1c, 4, palette);
        iVar1 = iVar1 + 1;
        y = y + 5;
    } while (iVar1 < 4);
    ScheduleBgTilemapBufferTransfer(param_2->bgConfig, 2);
    return;
}
