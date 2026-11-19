#include "global.h"
#include "overlay_83_battle_castle_opponent_app.h"
struct BattleCastleOpponentApp;

s32 ov83_02244DA0(void *, s32 *, s32 *, u32);

s32 ov83_0224691C(void *, s32);

s32 ov83_02246938(void *, s32);

s32 ov83_02247568(void *, u16, u16);

s32 ov83_022475D4(void *, s32);

void ov83_02244D0C(struct BattleCastleOpponentApp *arg0, u32 arg1, s32 arg2);

void ov83_02244D0C(BattleCastleOpponentApp *arg0, u32 arg1, s32 arg2) {
    s32 v1;
    s32 v0;
    s32 v2;
    s32 v3;
    void *v4;

    if (arg2 == 0) {
        v3 = 1;
        v4 = *((void **)&arg0->cursorSprite);
        v2 = 0;
    } else {
        v3 = 2;
        v4 = *((void **)&arg0->partnerCursorSprite);
        v2 = 0x11;
    }
    if (arg1 >= (u32) arg0->exitSlot) {
        ov83_022475D4(v4, v2);
        ov83_02247568(v4, 0xE0U, 0xA0U);
        if (arg2 == 0) {
            ov83_02246938(arg0, 0);
        }
    } else {
        ov83_022475D4(v4, v3);
        ov83_02244DA0(arg0, &v1, &v0, arg1);
        ov83_02247568(v4, (u16)v1, (u16)v0);
        if (arg2 == 0) {
            if ((u32) arg0->previousSlot >= (u32) arg0->exitSlot) {
                ov83_02246938(arg0, 1);
            }
            ov83_0224691C(arg0, 0);
        }
    }
}
