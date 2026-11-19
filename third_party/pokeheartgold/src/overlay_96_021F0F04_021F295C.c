#include "global.h"

#include "bg_window.h"

s32 ov96_021F2984(u32, BgConfig *);

void ov96_021F295C(s32 arg0, BgConfig *arg1);

void ov96_021F295C(s32 arg0, BgConfig *bgConfig) {
    u32 v0;

    v0 = 1;
    if (arg0 >= 1) {
        do {
            ov96_021F2984((u8)v0, bgConfig);
            v0 += 1;
        } while ((s32)v0 <= arg0);
    }
    ScheduleBgTilemapBufferTransfer(bgConfig, 1U);
}
