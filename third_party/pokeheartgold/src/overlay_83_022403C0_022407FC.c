#include "global.h"

#include "bg_window.h"
#include "overlay_83_battle_castle_self_app.h"

s32 ov83_02240C48(void *, s32, s32, s32, s32);

s32 ov83_02241DD8(void *, void *, void *, s32, s32, s32, s32, s32, s32);

void ov83_022407FC(BattleCastleSelfApp *arg0);

void ov83_022407FC(BattleCastleSelfApp *arg0) {
    u32 v0;

    FillWindowPixelBuffer(&arg0->windows[56], 0U);
    ov83_02240C48(arg0, 0, arg0->page + 1, 1, 0);
    ov83_02240C48(arg0, 1, ((s32)(arg0->numEntries - 1) / 6) + 1, 1, 0);
    v0 = GetWindowWidth(&arg0->windows[56]) * 8;
    ov83_02241DD8(arg0, &arg0->windows[56], arg0->msgLoader, 0x67, (s32)(v0 + (v0 >> 0x1F)) >> 1, 0, 0, 0x10200, 2);
    ScheduleWindowCopyToVram(&arg0->windows[56]);
}
