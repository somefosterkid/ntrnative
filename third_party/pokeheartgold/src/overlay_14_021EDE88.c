#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "sprite_system.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E637C(void *);
s32 ov14_021E765C(void *);
s32 ov14_021E7F4C(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F08F0(void *);
void ov14_021EDF08(void *arg0);
s32 ov14_021F6678(void *, s32);
s32 ov14_021F685C(void *, u32, s32, s32);
s32 ov14_021F2A18(UnkStruct_ov14_boxsys *, s32, s32);

s32 ov14_021E9518(UnkStruct_ov14_boxapp *app);

void ov14_021EDE88(UnkStruct_ov14_boxapp *arg0) {
    u8 py[2];

    PlaySE(0x5EAU);
    GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32) arg0->unk21);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(arg0->sys->unk_2C, (s32) arg0->unk21), &py[1], &py[0]);
    ManagedSprite_SetPositionXY(arg0->sys->sprites[9], (s16) py[1], (s16) py[0]);
    arg0->unk21 = 0xFFU;
    ov14_021E637C(arg0);
    ov14_021F08F0(arg0);
    ov14_021E765C(arg0);
    ov14_021E7F4C(((u8 *)arg0->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E9518, 0x56);
}

s32 ov14_021E95B4(UnkStruct_ov14_boxapp *app);

void ov14_021EDF08(void *arg0) {
    ov14_021E8328(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
    ov14_021F0234(arg0, ov14_021E95B4, 0x52);
}

s32 ov14_021EDF28(UnkStruct_ov14_boxapp *arg0) {
    s32 nextInput;
    u8 v0;

    nextInput = *(s32 *)((u8 *)(arg0->sys->unk_C) + 0xE4);
    PlaySE(0x5EAU);
    ov14_021E637C(arg0);
    ov14_021F08F0(arg0);
    ov14_021F6678(*((void * *)&arg0->sys), 0x28);
    v0 = arg0->unk21;
    if (v0 == 0xFF) {
        ov14_021F685C(arg0, 0U, 0, 0x27);
    } else {
        ov14_021F685C(arg0, v0, 1, 0x27);
        nextInput = 0x22;
    }
    GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32)(u8)nextInput);
    GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
    return 0x51;
}

s32 ov14_021EDF90(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F2A18(arg0->sys, 9, 1);
    return 0x51;
}
