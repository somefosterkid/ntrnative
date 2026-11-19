#include "global.h"

#include "sprite_system.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E8248(void *);
s32 ov14_021E82A8(void *);
s32 ov14_021E88F8(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F391C(UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F6654(void *, s32);

s32 ov14_021F40DC(void *);

s32 ov14_021E9450(UnkStruct_ov14_boxapp *app);

void ov14_021EEDB8(void *arg0) {
    u8 sp0[2];
    u16 var_r4;
    u8 *new_var;
    var_r4 = ((UnkStruct_ov14_boxapp *)arg0)->unk21;
    if (((u32)var_r4) < 0x1EU) {
        ov14_021E8248(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
        ov14_021E82A8(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
        ov14_021F3488(arg0, 0x81, 1);
    } else {
        var_r4 = (u16)(var_r4 - 0x1E);
        ov14_021F3488(arg0, 0x82, 1);
    }
    ov14_021E8328(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
    GridInputHandler_SetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, (u8)var_r4);
    new_var = (u8 *)arg0;
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(((UnkStruct_ov14_boxapp *)new_var)->sys->unk_2C, (s32)var_r4), &sp0[1], &sp0[0]);
    ManagedSprite_SetPositionXY(((UnkStruct_ov14_boxapp *)new_var)->sys->sprites[9], (s16)(sp0[1]), (s16)(sp0[0]));
    ov14_021F40DC(arg0);
    ov14_021F391C(((void *)((UnkStruct_ov14_boxapp *)new_var)->sys), 1);
    ov14_021F29E4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 0xB, 2);
    ov14_021E88F8(((u8 *)((UnkStruct_ov14_boxapp *)new_var)->sys->unk_2F0));
    ov14_021F6654(((void *)((UnkStruct_ov14_boxapp *)new_var)->sys), 0x25);
    if (((u32)(((UnkStruct_ov14_boxapp *)arg0)->unk21)) < 0x1EU) {
        ov14_021F3488(arg0, 0x81, 1);
    } else {
        ov14_021F3488(arg0, 0x82, 1);
    }
    ov14_021F0234(arg0, ov14_021E9450, 0x7B);
}
