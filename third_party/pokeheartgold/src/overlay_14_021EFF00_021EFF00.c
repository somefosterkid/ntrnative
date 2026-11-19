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
s32 ov14_021E7EE0(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F1F38(void *);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A04(struct UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F391C(UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F39D0(struct UnkStruct_ov14_boxsys *);

s32 ov14_021E94BC(UnkStruct_ov14_boxapp *app);

s32 ov14_021E9518(UnkStruct_ov14_boxapp *app);

s32 ov14_021EAF08(UnkStruct_ov14_boxapp *app);

s32 ov14_021EFF00(UnkStruct_ov14_boxapp *arg0) {
    u16 *temp_r4;
    u16 temp_r2;
    u16 temp_r7;
    u8 temp_r2_2;
    void *temp_r0;
    void *temp_r0_2;

    if (ov14_021F2A04(*((void * *)&arg0->sys), 0xB) == 1) {
        return 0x8E;
    }
    ov14_021F391C(*((void * *)&arg0->sys), 0);
    ov14_021F2A18(*((void * *)&arg0->sys), 0xB, 0);
    temp_r4 = ((u16 *)arg0->sys->unk_C);
    temp_r7 = ov14_021E6070(arg0, *temp_r4, 6, 0);
    ov14_021E6094(arg0, *temp_r4, 6, *((void * *)&arg0->sys) + 0x88C8);
    if (ov14_021E64D0(ov14_021E60C0(arg0, arg0->unk1F, *temp_r4)) == 1) {
        temp_r2 = *temp_r4;
        ov14_021F2ED0(arg0, arg0->unk1F, temp_r2, *(u8 *)((u8 *)(*((void * *)&arg0->sys) + temp_r2) + 0x4094));
    }
    ov14_021E7588(arg0, *temp_r4);
    arg0->sys->unk_88C8 = temp_r7;
    ov14_021E6094(arg0, (u16) arg0->unk21, 6, *((void * *)&arg0->sys) + 0x88C8);
    if (ov14_021E64D0(ov14_021E60C0(arg0, arg0->unk1F, (u16) arg0->unk21)) == 1) {
        temp_r2_2 = arg0->unk21;
        ov14_021F2ED0(arg0, arg0->unk1F, (u16)temp_r2_2, *(u8 *)((u8 *)(*((void * *)&arg0->sys) + temp_r2_2) + 0x4094));
    }
    temp_r0 = *((void * *)&arg0->sys);
    if (((UnkStruct_ov14_boxsys *)temp_r0)->unk_88C8 == 0) {
        if (*((u8 *)&((UnkStruct_ov14_boxsys *)temp_r0)->unk_448[2]) == 0) {
            arg0->unk21 = (u8)*temp_r4;
            ov14_021F1F38(arg0);
            GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32)(u8)(arg0->unk21 - 0x1E));
            ov14_021E8328(arg0->sys->unk_2F0);
            return ov14_021F0234(arg0, ov14_021E94BC, 0x8F);
        }
        *((u8 *)&((UnkStruct_ov14_boxsys *)temp_r0)->unk_448[2]) = 0U;
        ov14_021F1F38(arg0);
        GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32)(u8)(arg0->unk21 - 0x1E));
        ov14_021E7EE0(((u8 *)arg0->sys->unk_2F0));
        ov14_021F3488(arg0, 1, 1);
        return ov14_021F0234(arg0, ov14_021E9518, 0x8F);
    }
    PlaySE(0x5EBU);
    temp_r0_2 = *((void * *)&arg0->sys);
    ov14_021F3844(temp_r0_2, ((UnkStruct_ov14_boxsys *)temp_r0_2)->unk_88C8);
    ov14_021F391C(*((void * *)&arg0->sys), 1);
    ov14_021F29E4(*((void * *)&arg0->sys), 0xB, 1);
    ov14_021F2A18(*((void * *)&arg0->sys), 0xB, 1);
    ov14_021F39D0(*((void * *)&arg0->sys));
    return ov14_021F0234(arg0, ov14_021EAF08, 0x90);
}
