#include "global.h"

#include "bg_window.h"
#include "overlay_43_palpad.h"

void ov43_0222AA70(PalPadGfx *gfx);

void ov43_0222D090(PalPadFriendCode *friendCode, PalPadGfx *gfx);

void ov43_0222D090(PalPadFriendCode *friendCode, PalPadGfx *gfx) {
    ov43_0222AA70(gfx);
    FillBgTilemapRect(gfx->bgConfig, 2, 0, 0, 0, 0x20, 0x18, 0);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, 2);
    ClearWindowTilemapAndScheduleTransfer(&friendCode->windows[0]);
    ClearWindowTilemapAndScheduleTransfer(&friendCode->windows[1]);
    ClearWindowTilemapAndScheduleTransfer(&friendCode->windows[2]);
    FillBgTilemapRect(gfx->bgConfig, 3, 0, 0, 0, 0x20, 0x18, 0);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, 3);
}
