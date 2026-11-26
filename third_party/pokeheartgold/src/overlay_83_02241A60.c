#include "global.h"

#include "bg_window.h"
#include "party.h"
#include "overlay_83_battle_castle_self_app.h"

s32 ov83_0223FAA8(void *, s32);

void ov83_0223FD4C(BattleCastleSelfApp *param_1, Window *param_2, Pokemon *param_3);

#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov83_02247768(u8, u8);
#else
u8 ov83_02247768(u8, s32);
#endif

s32 ov83_022477EC(s32, s32, void *);

void ov83_02241A60(BattleCastleSelfApp *arg0, s32 arg1);

s32 ov83_0223FB24(void *, s32);
void ov83_0223FF44(BattleCastleSelfApp *arg0, Window *window, void *arg2);
void ov83_02241ABC(BattleCastleSelfApp *arg0, s32 arg1);

void ov83_02241A60(BattleCastleSelfApp *arg0, s32 arg1) {
    ov83_0223FAA8(arg0, 2);
    ClearWindowTilemapAndScheduleTransfer(&arg0->windows[6]);
    ClearWindowTilemapAndScheduleTransfer(&arg0->windows[5]);
    ClearWindowTilemapAndScheduleTransfer(&arg0->windows[15]);
    ov83_0223FD4C(arg0, &arg0->windows[4], Party_GetMonByIndex(arg0->party, ov83_02247768(arg0->numSlots, arg1)));
    ov83_022477EC(2, 1, &arg0->unk_868);
}

void ov83_02241ABC(BattleCastleSelfApp *arg0, s32 arg1) {
    ov83_0223FB24(arg0, 2);
    ClearWindowTilemapAndScheduleTransfer(&arg0->windows[6]);
    ClearWindowTilemapAndScheduleTransfer(&arg0->windows[5]);
    ClearWindowTilemapAndScheduleTransfer(&arg0->windows[15]);
    ov83_0223FF44(arg0, &arg0->windows[4], Party_GetMonByIndex(arg0->party, ov83_02247768(arg0->numSlots, arg1)));
    ov83_022477EC(2, 1, &arg0->unk_868);
}
