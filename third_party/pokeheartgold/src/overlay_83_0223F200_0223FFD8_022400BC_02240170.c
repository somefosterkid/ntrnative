#include "global.h"
struct BattleCastleSelfApp;

#include "bg_window.h"
#include "overlay_83_battle_castle_self_app.h"
#include "overlay_80_02237A70.h"


#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/overlay_83_022401A4.c (tools/pokehg_abi.py)
void ov83_022401A4(void *, Window *, s32, u8);
#else
s32 ov83_022401A4(void *, Window *, u8, s32);
#endif

void ov83_02240170(struct BattleCastleSelfApp *arg0, Window *arg1);

void ov83_02240170(BattleCastleSelfApp *arg0, Window *window) {
    s32 v0;
    u32 v1;

    v0 = ov80_02237B24(arg0->challengeType, 1);
    v1 = 0;
    if (v0 > 0) {
        do {
            ov83_022401A4(arg0, window, v1, 0);
            v1 += 1;
        } while ((s32)v1 < v0);
    }
    ScheduleWindowCopyToVram(window);
}
