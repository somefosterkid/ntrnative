#include "global.h"
struct BattleCastleSelfApp;

#include "frontier/overlay_80_022340E8.h"

#include "unk_02005D10.h"
#include "overlay_83_battle_castle_self_app.h"
#include "battle_castle_app_sprite.h"

void ov83_02240DA8(BattleCastleSelfApp *arg0, s32 *arg1, s32 arg2);

void ov83_02240DA8(BattleCastleSelfApp *arg0, s32 *arg1, s32 arg2) {
    arg0->subState = 0;
    *arg1 = arg2;
}

void ov83_02240DE0(struct BattleCastleSelfApp *arg0, u32 arg1, s32 arg2);

s32 ov83_02241368(BattleCastleSelfApp *, u16, u16);

void ov83_02240DB0(struct BattleCastleSelfApp *arg0);

void ov83_02240DB0(BattleCastleSelfApp *arg0) {
    PlaySE(0x5DCU);
    if (ov80_02237D8C(arg0->challengeType) == 1) {
        ov83_02241368(arg0, 0xC, arg0->selectedMonSlot);
    }
    ov83_02240DE0(arg0, arg0->selectedMonSlot, 0);
}

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov83_02240E70(void *, s32 *, s32 *, u8);
#else
s32 ov83_02240E70(void *, s32 *, s32 *, u32);
#endif

s32 ov83_02242814(void *, s32);

s32 ov83_02242844(BattleCastleSelfApp *, s32);

s32 ov83_02247568(s32, u16, u16);

s32 ov83_022475D4(s32, s32);

void ov83_02240DE0(BattleCastleSelfApp *arg0, u32 arg1, s32 arg2) {
    s32 v1;
    s32 v0;
    s32 v2;
    s32 v3;
    s32 v4;

    if (arg2 == 0) {
        v4 = *((s32 *)&arg0->cursorSprite);
        v3 = 1;
        v2 = 0;
    } else {
        v3 = 2;
        v4 = *((s32 *)&arg0->partnerCursorSprite);
        v2 = 0x11;
    }
    if (arg1 >= (u32) arg0->exitSlot) {
        ov83_022475D4(v4, v2);
        ov83_02247568(v4, 0xE0U, 0xA0U);
        if (arg2 == 0) {
            ov83_02242844(arg0, 0);
        }
    } else {
        ov83_022475D4(v4, v3);
        ov83_02240E70(arg0, &v1, &v0, arg1);
        ov83_02247568(v4, (u16)v1, (u16)v0);
        if (arg2 == 0) {
            if ((u32) arg0->previousSlot >= (u32) arg0->exitSlot) {
                ov83_02242844(arg0, 1);
            }
            ov83_02242814(arg0, 0);
        }
    }
}
