#include "global.h"

#include "bg_window.h"
#include "overlay_43_palpad_fns.h"

extern u8 ov43_0222EDB8[];

extern u8 ov43_0222EDF8[];

extern u8 ov43_0222F0C4[];

void ov43_0222DB94(PalPadDetails *details, PalPadGfx *gfx, s16 page) {
    NNSG2dScreenData *screen;
    u8 x;
    u8 w;

    if (details->hasFrontierPages == 1) {
        screen = gfx->screens.data[1];
        CopyToBgTilemapRect(gfx->bgConfig, 2, 0, 0x14, 0x20, 4, screen->rawData, 0, 0xC, (u8)((u32)screen->screenWidth >> 3), (u8)((u32)screen->screenHeight >> 3));
    } else {
        screen = gfx->screens.data[1];
        CopyToBgTilemapRect(gfx->bgConfig, 2, 0, 0x14, 0x20, 4, screen->rawData, 0, 0x14, (u8)((u32)screen->screenWidth >> 3), (u8)((u32)screen->screenHeight >> 3));
    }
    if (page == 0xFF) {
        ScheduleBgTilemapBufferTransfer(gfx->bgConfig, 2);
        return;
    }
    screen = gfx->screens.data[1];
    x = (ov43_0222EDB8 + details->hasFrontierPages * 8)[page];
    w = (ov43_0222EDF8 + details->hasFrontierPages * 8)[page];
    CopyToBgTilemapRect(gfx->bgConfig, 2, x, 0x14, w, 4, screen->rawData, x, ov43_0222F0C4[details->hasFrontierPages], (u8)((u32)screen->screenWidth >> 3), (u8)((u32)screen->screenHeight >> 3));
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, 2);
}
