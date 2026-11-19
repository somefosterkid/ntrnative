#include "global.h"

#include "bg_window.h"

void ov96_021FFBD8(BgConfig *arg0, s32 arg1);

void ov96_021FFBD8(BgConfig *bgConfig, s32 arg1) {
    if ((arg1 > 0x110) && (arg1 < 0x300)) {
        FillBgTilemapRect(bgConfig, 0U, 1U, 0xAU, 3U, 2U, 0x15U, 0x10U);
    } else {
        FillBgTilemapRect(bgConfig, 0U, 0x20U, 0xAU, 3U, 2U, 0x15U, 0x10U);
    }
    ScheduleBgTilemapBufferTransfer(bgConfig, 0U);
}
