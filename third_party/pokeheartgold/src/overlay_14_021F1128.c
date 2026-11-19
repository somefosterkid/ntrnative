#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
struct UnkStruct_ov14_boxapp;

#include "sprite_system.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F1100(void *, u8);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
void ov14_021F1128(struct UnkStruct_ov14_boxapp *arg0);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F48B4(void *);
s32 ov14_021F57B8(void *);
s32 ov14_021F1004();
s32 ov14_021F11F8(struct UnkStruct_ov14_boxapp *arg0);

void ov14_021F1128(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F2A18(((void *)arg0->sys), 9, 0);
    arg0->sys->unk_43C = (s32)((s32) arg0->unk1F % 6);
    arg0->unk2C = (s32) arg0->sys->unk_43C;
    if (ov14_021E8544(arg0->sys->unk_2F0) == 0) {
        ov14_021ED5B0(arg0);
        return;
    }
    ov14_021F1100(arg0, 0x35);
}

s32 ov14_021F1170(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    u8 py[2];
    s32 v0;

    arg0->unk25 = (u8)(arg1 + (6 * ((s32) arg0->unk25 / 6)));
    ov14_021F48B4(arg0);
    ov14_021F57B8(arg0);
    v0 = GridInputHandler_GetNextInput(arg0->sys->unk_2C);
    arg0->sys->unk_43C = v0;
    GridInputHandler_SetNextLastUnk0FInputs(arg0->sys->unk_2C, 8, v0, v0);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(arg0->sys->unk_2C, 8), &py[1], &py[0]);
    ManagedSprite_SetPositionXY(arg0->sys->sprites[9], (s16) py[1], (s16) py[0]);
    ov14_021F29E4(*((void * *)&arg0->sys), 9, 8);
    ov14_021F2A18(*((void * *)&arg0->sys), 9, 1);
    return 0x3D;
}

s32 ov14_021F11F8(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F1004();
    GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32)(u8)((s32) arg0->unk25 % 6));
    GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
    return 0x3D;
}
