#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "party.h"
#include "pokemon_storage_system.h"
#include "sprite_system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E7930(void *, u32);
s32 ov14_021F2270(void *, s32, s32);
void ov14_021F131C(struct UnkStruct_ov14_boxapp *arg0);
s32 ov14_021E83F4(void *);
s32 ov14_021E884C(UnkStruct_ov14_boxsys *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F6654(void *, s32);
s32 ov14_021F685C(void *, s32, s32, s32);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F48B4(void *);
s32 ov14_021F57B8(void *);

void ov14_021F131C(UnkStruct_ov14_boxapp *arg0) {
    u8 v0;

    v0 = *((u8 *)&arg0->sys->unk_44D);
    if (((u32)v0 >= 0x10U) && (PCStorage_IsBonusWallpaperUnlocked(*((PCStorage * *)&arg0->storage), v0 - 0x10) == 0)) {
        PlaySE(0x5F3U);
        ov14_021F2270(arg0, 0xC, 0x42);
        return;
    }
    if (*((u8 *)&arg0->sys->unk_44D) == ov14_021E7930(arg0, arg0->unk25)) {
        PlaySE(0x5F3U);
        ov14_021F2270(arg0, 0xC, 0x42);
        return;
    }
    if (arg0->unk25 != arg0->unk1F) {
        PlaySE(0x5DCU);
        ov14_021F2270(arg0, 0xC, 0xA5);
        return;
    }
    PlaySE(0x5DDU);
    ov14_021F2270(arg0, 0xC, 0x47);
}

s32 ov14_021E9434(UnkStruct_ov14_boxapp *app);

s32 ov14_021F13B0(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F2A18(*((void * *)&arg0->sys), 9, 0);
    if (Party_GetCount(arg0->party) != 6) {
        ov14_021F6654(*((void * *)&arg0->sys), 0x27);
        ov14_021E84A4(arg0->sys->unk_2F0);
        ov14_021E884C(*((void * *)&arg0->sys));
        return ov14_021F0234(arg0, ov14_021E9434, 0x53);
    }
    ov14_021E83F4(((u8 *)arg0->sys->unk_2F0));
    ov14_021F685C(arg0, 0, 2, 0x25);
    *((s32 *)&arg0->unk30) = 0xE;
    return 6;
}

s32 ov14_021E9450(UnkStruct_ov14_boxapp *app);

void ov14_021F1414(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F6654(arg0->sys, 0x27);
    ov14_021F2A18(arg0->sys, 9, 0);
    ov14_021E84A4(arg0->sys->unk_2F0);
    ov14_021F0234(arg0, ov14_021E9450, 0x5C);
}

s32 ov14_021F1448(void *arg0, s32 arg1) {
    u8 py[2];
    s32 v0;

    ((UnkStruct_ov14_boxapp *)arg0)->unk25 = (u8)(arg1 + (6 * ((s32) ((UnkStruct_ov14_boxapp *)arg0)->unk25 / 6)));
    if (System_GetTouchNew() == 0) {
        if (PCStorage_CountMonsAndEggsInBox(*(PCStorage **)((u8 *)arg0 + 4), (u32) ((UnkStruct_ov14_boxapp *)arg0)->unk25) == 0x1E) {
            PlaySE(0x5F3U);
        } else {
            PlaySE(0x5DDU);
        }
        return 0x66;
    }
    PlaySE(0x5DDU);
    ov14_021F48B4(arg0);
    ov14_021F57B8(arg0);
    v0 = GridInputHandler_GetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C);
    ((UnkStruct_ov14_boxapp *)arg0)->sys->unk_43C = v0;
    GridInputHandler_SetNextLastUnk0FInputs(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, 8, v0, v0);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, 8), &py[1], &py[0]);
    ManagedSprite_SetPositionXY(((UnkStruct_ov14_boxapp *)arg0)->sys->sprites[9], (s16) py[1], (s16) py[0]);
    ov14_021F29E4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 9, 8);
    ov14_021F2A18(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 9, 1);
    return 0x61;
}
