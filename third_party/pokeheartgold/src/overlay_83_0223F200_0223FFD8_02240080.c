#include "global.h"
struct BattleCastleSelfApp;

#include "bg_window.h"
#include "overlay_83_battle_castle_self_app.h"
#include "overlay_80_02237A70.h"


s32 ov83_022400BC(void *, Window *, s32, s32);

void ov83_02240080(struct BattleCastleSelfApp *arg0, Window *arg1);

void ov83_02240080(BattleCastleSelfApp *arg0, Window *window) {
    s32 v0;
    u32 v1;

    FillWindowPixelBuffer(window, 0U);
    v0 = ov80_02237B24(arg0->challengeType, 1);
    v1 = 0;
    if (v0 > 0) {
        do {
            ov83_022400BC(arg0, window, (u8)v1, 0);
            v1 += 1;
        } while ((s32)v1 < v0);
    }
    ScheduleWindowCopyToVram(window);
}
