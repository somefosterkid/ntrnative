#include "global.h"

#include "sprite_system.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F0EE8(void *, s32);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F48B4(void *);
s32 ov14_021F57B8(void *);
s32 ov14_021F0D58(void *arg0, s32 arg1);

s32 ov14_021E9920(UnkStruct_ov14_boxapp *app);

s32 ov14_021E9970(UnkStruct_ov14_boxapp *app);

s32 ov14_021F0D58(void *arg0, s32 arg1) {
    u8 py[2];
    s32 v2;
    u32 v0;
    u8 v1;

    if (*(s32 *)((u8 *)(*(void **)arg0) + 8) == 3) {
        v2 = 0x82;
    } else {
        v2 = 0x29;
    }
    v1 = ((UnkStruct_ov14_boxapp *)arg0)->unk25;
    v0 = arg1 + (6 * ((s32)v1 / 6));
    if (v0 != v1) {
        ((UnkStruct_ov14_boxapp *)arg0)->unk25 = v0;
        ov14_021F48B4(arg0);
        ov14_021F57B8(arg0);
    }
    if (((UnkStruct_ov14_boxapp *)arg0)->unk25 == ((UnkStruct_ov14_boxapp *)arg0)->unk1F) {
        if (ov14_021E85E4(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0) == 1) {
            GridInputHandler_SetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, (s32)(u8)(((s32) ((UnkStruct_ov14_boxapp *)arg0)->unk25 % 6) + 0x25));
            GridInputHandler_SetButtonInputMode(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, 1);
            ov14_021F29E4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 9, 0xE);
            return ov14_021F0EE8(arg0, v2);
        }
        if (ov14_021E8648(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0) == 1) {
            GridInputHandler_SetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, (s32)(u8)(((s32) ((UnkStruct_ov14_boxapp *)arg0)->unk25 % 6) + 0x25));
            GridInputHandler_SetButtonInputMode(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, 1);
            ov14_021F29E4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 9, 0xE);
            ov14_021E8634(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
            return ov14_021F0234(arg0, ov14_021E9970, v2);
        }
        ov14_021F29E4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 9, 0xE);
        return v2;
    }
    GridInputHandler_SetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, 0x2D);
    if (GridInputHandler_IsButtonInputMode(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C) == 1) {
        DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, 0x2D), &py[1], &py[0]);
        ManagedSprite_SetPositionXY(((UnkStruct_ov14_boxapp *)arg0)->sys->sprites[9], (s16) py[1], (s16) py[0]);
        ov14_021F29E4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 9, 8);
        ov14_021F2A18(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 9, 1);
    }
    if (ov14_021E85E4(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0) == 1) {
        return v2;
    }
    ov14_021F604C(arg0);
    if (ov14_021E8648(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0) == 1) {
        ov14_021E8634(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
    }
    return ov14_021F0234(arg0, ov14_021E9920, v2);
}
