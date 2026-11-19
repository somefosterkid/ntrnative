#include "global.h"

#include "party.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_0223FD4C(BattleCastleSelfApp *param_1, Window *param_2, Pokemon *param_3);

s32 ov83_02240DB0(void *);

u8 ov83_02247768(u8, u8);

void ov83_02241208(BattleCastleSelfApp *arg0, s32 arg1);

void ov83_02241208(BattleCastleSelfApp *arg0, s32 arg1) {
    s8 v1;
    u8 v0;

    v1 = *((s8 *)&arg0->selectedMonSlot) + arg1;
    v0 = arg0->numSlots;
    if ((s32)v1 < 0) {
        v1 = v0 - 1;
    } else if ((s32)v1 >= (s32)v0) {
        v1 = 0;
    }
    *((s8 *)&arg0->selectedMonSlot) = v1;
    ov83_02240DB0(arg0);
    ov83_0223FD4C(arg0, &arg0->windows[4], Party_GetMonByIndex(arg0->party, ov83_02247768(arg0->numSlots, (u8) *((s8 *)&arg0->selectedMonSlot))));
}

void ov83_0223FF44(BattleCastleSelfApp *arg0, Window *window, void *arg2);

void ov83_02241254(BattleCastleSelfApp *arg0, s32 arg1);

void ov83_02241254(BattleCastleSelfApp *arg0, s32 arg1) {
    s8 v1;
    u8 v0;

    v1 = *((s8 *)&arg0->selectedMonSlot) + arg1;
    v0 = arg0->numSlots;
    if ((s32)v1 < 0) {
        v1 = v0 - 1;
    } else if ((s32)v1 >= (s32)v0) {
        v1 = 0;
    }
    *((s8 *)&arg0->selectedMonSlot) = v1;
    ov83_02240DB0(arg0);
    ov83_0223FF44(arg0, &arg0->windows[4], Party_GetMonByIndex(arg0->party, ov83_02247768(arg0->numSlots, (u8) *((s8 *)&arg0->selectedMonSlot))));
}

void ov83_0224042C(BattleCastleSelfApp *arg0);

s32 ov83_02241354(void *);

s32 ov83_02241730(BattleCastleSelfApp *arg0);

s32 ov83_02241B18(BattleCastleSelfApp *);

s32 ov83_0224755C(void *, s32);

s32 ov83_022477EC(s32, s32, void *);

void ov83_022412A0(BattleCastleSelfApp *arg0);

void ov83_022412A0(BattleCastleSelfApp *arg0) {
    ov83_02241730(arg0);
    ov83_0224042C(arg0);
    ov83_02241354(&arg0->windows[6]);
    ov83_02241B18(arg0);
    ov83_022477EC(2, 0, &arg0->unk_868);
    ov83_0224755C(arg0->bigSparkles, 0);
}
