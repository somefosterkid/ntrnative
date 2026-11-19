#include "global.h"

#include "bg_window.h"
#include "overlay_68_move_relearner.h"

void ov68_021E66F0(MoveRelearner *, u8, u8);

s32 ov68_021E70BC(MoveRelearner *);

void ov68_021E67E0(MoveRelearner *arg0);

void ov68_021E67E0(MoveRelearner *arg0) {
    u32 v0;

    FillWindowPixelBuffer(&arg0->windows[10], 0U);
    v0 = 0;
    do {
        ov68_021E66F0(arg0, (u8)arg0->args->scrollOffset, v0);
        v0 += 1;
    } while ((u32)v0 < 4U);
    ScheduleWindowCopyToVram(&arg0->windows[10]);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 7U);
    ov68_021E70BC(arg0);
}
