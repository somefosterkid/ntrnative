#include "global.h"

#include "sprite_system.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021E5A50(UnkStruct_ov14_boxsys *sys, UnkFuncPtr_ov14_boxtask task);
#if defined(SDK_PORT) || defined(NONMATCHING)
void ov14_021F7AC4(UnkStruct_ov14_boxsys *arg0, s32 target, s32 arg2);
#else
void ov14_021F7AC4(UnkStruct_ov14_boxsys *arg0, s32 target, s32 arg2);
#endif
void ov14_021F7670(UnkStruct_ov14_boxapp *arg0, s32 target, s32 arg2);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);

s32 ov14_021E9F20(UnkStruct_ov14_boxapp *app);

void ov14_021F7670(UnkStruct_ov14_boxapp *arg0, s32 target, s32 arg2) {
    ov14_021F7AC4(arg0->sys, target, arg2);
    ov14_021E5A50(arg0->sys, ov14_021E9F20);
}

void ov14_021F7688(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    u8 py[2];
    s32 v0;

    v0 = arg1;
    if ((u32)(v0 - 6) <= 1U) {
        v0 = (s32) arg0->unk25 % 6;
        arg0->sys->unk_43C = v0;
    }
    GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32)(u8)v0);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(arg0->sys->unk_2C, v0), &py[1], &py[0]);
    ManagedSprite_SetPositionXY(arg0->sys->sprites[9], (s16) py[1], (s16) py[0]);
    if ((v0 >= 0) && (v0 <= 5)) {
        ov14_021F29E4(arg0->sys, 9, 0xE);
        return;
    }
    ov14_021F29E4(arg0->sys, 9, 8);
}
