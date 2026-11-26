#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "pokemon_types_def.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E6094(void *, u32, s32, void *);
s32 ov14_021E64D0(BoxPokemon *boxMon);
s32 ov14_021F2ED0(void *, u32, u32, s32);
s32 ov14_021F3844(UnkStruct_ov14_boxsys *, u16);

s32 ov14_021E6070(void *, s32, s32, void *);
void *ov14_021E60C0(void *, u32, u32);
s32 ov14_021E7588(void *, s32);
s32 ov14_021E8248(void *);
s32 ov14_021E82A8(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F1F38(void *);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A04(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F391C(UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F39D0(struct UnkStruct_ov14_boxsys *);

s32 ov14_021E94BC(UnkStruct_ov14_boxapp *app);

s32 ov14_021EA728(UnkStruct_ov14_boxapp *app);

s32 ov14_021EF024(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2) {
    u16 *temp_r4;
    u16 temp_r2;
    u16 temp_r7;
    u8 temp_r2_2;
    UnkStruct_ov14_boxsys *temp_r0;

    if (ov14_021F2A04(arg0->sys, 0xB, arg2) == 1) {
        return 0x7F;
    }
    ov14_021F391C(arg0->sys, 0);
    ov14_021F2A18(arg0->sys, 0xB, 0);
    temp_r4 = ((u16 *)arg0->sys->unk_C);
    temp_r7 = ov14_021E6070(arg0, *temp_r4, 6, 0);
    ov14_021E6094(arg0, *temp_r4, 6, &arg0->sys->unk_88C8);
    if (ov14_021E64D0(ov14_021E60C0(arg0, arg0->unk1F, *temp_r4)) == 1) {
        temp_r2 = *temp_r4;
        ov14_021F2ED0(arg0, arg0->unk1F, temp_r2, arg0->sys->unk_4094[temp_r2]);
    }
    ov14_021E7588(arg0, *temp_r4);
    arg0->sys->unk_88C8 = temp_r7;
    ov14_021E6094(arg0, (u16) arg0->unk21, 6, &arg0->sys->unk_88C8);
    if (ov14_021E64D0(ov14_021E60C0(arg0, arg0->unk1F, (u16) arg0->unk21)) == 1) {
        temp_r2_2 = arg0->unk21;
        ov14_021F2ED0(arg0, arg0->unk1F, (u16)temp_r2_2, arg0->sys->unk_4094[temp_r2_2]);
    }
    if (arg0->sys->unk_88C8 == 0) {
        arg0->unk21 = (u8)*temp_r4;
        ov14_021F1F38(arg0);
        ov14_021E8248(((u8 *)arg0->sys->unk_2F0));
        ov14_021E82A8(((u8 *)arg0->sys->unk_2F0));
        ov14_021E8328(arg0->sys->unk_2F0);
        GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32) arg0->unk21);
        GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
        return ov14_021F0234(arg0, ov14_021E94BC, 0x75);
    }
    PlaySE(0x5EBU);
    temp_r0 = arg0->sys;
    ov14_021F3844(temp_r0, temp_r0->unk_88C8);
    ov14_021F391C(arg0->sys, 1);
    ov14_021F29E4(arg0->sys, 0xB, 1);
    ov14_021F2A18(arg0->sys, 0xB, 1);
    ov14_021F39D0(arg0->sys);
    return ov14_021F0234(arg0, ov14_021EA728, 0x80);
}
