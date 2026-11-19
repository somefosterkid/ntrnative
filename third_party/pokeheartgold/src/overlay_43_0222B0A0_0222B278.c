#include "global.h"

#include "bg_window.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "overlay_43_palpad.h"

s32 ov43_0222B3A4(PalPadMenu *, PalPadGfx *);

s32 ov43_0222B3C4(PalPadMenu *, PalPadGfx *);

s32 ov43_0222B278(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx);

s32 ov43_0222B278(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx) {
    s32 v2;
    u32 v0;
    u32 v1;

    v2 = 0;
    if (0xCF3 & gSystem.newKeys) {
        shared->inputState = 0;
    }
    if (gSystem.newKeys & 1) {
        ov43_0222B3C4(menu, gfx);
        PlaySE(0x5DDU);
        return 1;
    }
    if (gSystem.newKeys & 2) {
        PlaySE(0x5DDU);
        menu->selection = 3;
        return 1;
    }
    if (0x40 & gSystem.newAndRepeatedKeys) {
        v0 = menu->selection;
        if (v0 != 0) {
            menu->selection = v0 - 1;
            v2 = 1;
        } else {
            menu->selection = 3;
            v2 = 1;
        }
    } else if (gSystem.newAndRepeatedKeys & 0x80) {
        v1 = menu->selection;
        if (v1 < 3U) {
            menu->selection = v1 + 1;
            v2 = 1;
        } else {
            menu->selection = 0;
            v2 = 1;
        }
    }
    if (v2 == 1) {
        PlaySE(0x5E5U);
        ov43_0222B3A4(menu, gfx);
        ScheduleBgTilemapBufferTransfer(gfx->bgConfig, 2U);
    }
    return 0;
}

extern TouchscreenHitbox ov43_0222EE1A;

s32 ov43_0222B324(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx, s32 *arg3);

s32 ov43_0222B324(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx, s32 *arg3) {
    s32 v0;

    v0 = TouchscreenHitbox_FindHitboxAtTouchNew(&ov43_0222EE1A);
    if (v0 == -1) {
        return 0;
    }
    *arg3 = 1;
    shared->inputState = 1;
    menu->selection = v0;
    PlaySE(0x5DDU);
    ov43_0222B3C4(menu, gfx);
    ov43_0222B3A4(menu, gfx);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, 2U);
    return 1;
}
