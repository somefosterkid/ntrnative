#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "item.h"
#include "unk_02005D10.h"
#include "unk_0201956C.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "pokemon_types_def.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E6094(void *, u32, s32, void *);
s32 ov14_021E64D0(BoxPokemon *boxMon);
s32 ov14_021F2ED0(void *, u32, u32, s32);
s32 ov14_021F3844(UnkStruct_ov14_boxsys *, u16);

s32 ov14_021E6070(void *, s32, s32, void *);
void *ov14_021E60C0(void *, u32, u32);
s32 ov14_021E7588(void *, s32);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F1F38(void *);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F34C8(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F391C(UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F39D0(struct UnkStruct_ov14_boxsys *);

s32 ov14_021EA928(UnkStruct_ov14_boxapp *app);

s32 ov14_021EF6FC(UnkStruct_ov14_boxapp *arg0) {
    u16 temp_r1_2;
    u16 temp_r2;
    u16 temp_r4_2;
    u16 temp_r7;
    u8 temp_r2_2;
    void *temp_r1;
    void *temp_r4;

    temp_r1 = *((void * *)&arg0->sys);
    temp_r4 = *(void **)((u8 *)temp_r1 + 0xC);
    if (sub_020199E4(((UnkStruct_ov14_boxsys *)temp_r1)->unk_2F0, 0x10) != 0) {
        return 0x85;
    }
    temp_r1_2 = *(u16 *)temp_r4;
    if (temp_r1_2 == arg0->unk21) {
        temp_r4_2 = *(u16 *)((u8 *)temp_r4 + 2);
        ov14_021F1F38(arg0);
        ov14_021F34C8(*((void * *)&arg0->sys), arg0->unk21, 0);
        GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32) arg0->unk21);
        if ((ItemIdIsMail(arg0->sys->unk_88C8) == 1) && (temp_r4_2 != arg0->unk21)) {
            PlaySE(0x5F3U);
            ov14_021F6730(arg0, 0x25);
            *((s32 *)&arg0->unk30) = 0x87;
            return 6;
        }
        GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
        return 0x82;
    }
    ov14_021F34C8(*((void * *)&arg0->sys), temp_r1_2, 0);
    ov14_021F34C8(*((void * *)&arg0->sys), arg0->unk21, 1);
    temp_r7 = ov14_021E6070(arg0, *(u16 *)temp_r4, 6, 0);
    ov14_021E6094(arg0, *(u16 *)temp_r4, 6, *((void * *)&arg0->sys) + 0x88C8);
    if (ov14_021E64D0(ov14_021E60C0(arg0, arg0->unk1F, *(u16 *)temp_r4)) == 1) {
        temp_r2 = *(u16 *)temp_r4;
        ov14_021F2ED0(arg0, arg0->unk1F, temp_r2, *(u8 *)((u8 *)(*((void * *)&arg0->sys) + temp_r2) + 0x4094));
    }
    ov14_021E7588(arg0, *(u16 *)temp_r4);
    GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32)(u8) * (u16 *)temp_r4);
    arg0->sys->unk_88C8 = temp_r7;
    ov14_021E6094(arg0, (u16) arg0->unk21, 6, *((void * *)&arg0->sys) + 0x88C8);
    if (ov14_021E64D0(ov14_021E60C0(arg0, arg0->unk1F, (u16) arg0->unk21)) == 1) {
        temp_r2_2 = arg0->unk21;
        ov14_021F2ED0(arg0, arg0->unk1F, (u16)temp_r2_2, *(u8 *)((u8 *)(*((void * *)&arg0->sys) + temp_r2_2) + 0x4094));
    }
    temp_r1_2 = arg0->sys->unk_88C8;
    if (temp_r1_2 == 0) {
        arg0->unk21 = (u8) * (u16 *)temp_r4;
        ov14_021F1F38(arg0);
        GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
        return 0x82;
    }
    ov14_021F3844(*((void * *)&arg0->sys), temp_r1_2);
    ov14_021F391C(*((void * *)&arg0->sys), 1);
    ov14_021F29E4(*((void * *)&arg0->sys), 0xB, 1);
    ov14_021F2A18(*((void * *)&arg0->sys), 0xB, 1);
    ov14_021F39D0(*((void * *)&arg0->sys));
    PlaySE(0x5EBU);
    return ov14_021F0234(arg0, ov14_021EA928, 0x86);
}
