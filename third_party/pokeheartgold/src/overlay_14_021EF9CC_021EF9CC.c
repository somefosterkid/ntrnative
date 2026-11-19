#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "pokemon_types_def.h"
#include "overlay_14_box_unk_0201956C.h"
struct UnkStruct_ov14_boxapp;

s32 ov14_021E6070(void *, s32, s32, void *);

s32 ov14_021E6094(void *, u32, s32, void *);

void *ov14_021E60C0(void *, u32, u32);

s32 ov14_021E64D0(BoxPokemon *boxMon);

s32 ov14_021E7588(void *, s32);

s32 ov14_021E88BC(void *);

s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);

s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);

s32 ov14_021F2ED0(void *, u32, u32, s32);

s32 ov14_021F34C8(void *, s32, s32, s32);

s32 ov14_021F3844(UnkStruct_ov14_boxsys *, u16);

s32 ov14_021F38B0(struct UnkStruct_ov14_boxsys *, u16);

s32 ov14_021F39A0(struct UnkStruct_ov14_boxsys *, u32, s32);

s32 ov14_021F5564(void *, u16);

s32 sub_02019A60(void *, s32, void *);

s32 ov14_021E9970(UnkStruct_ov14_boxapp *app);

s32 ov14_021EAA04(UnkStruct_ov14_boxapp *app);

void ov14_021EF9CC(struct UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov14_021EF9CC(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_r0_5;
    u16 temp_r2_2;
    u16 temp_r5;
    u8 temp_r1;
    u8 temp_r2;
    void *temp_r0;
    void *temp_r0_2;
    void *temp_r0_3;
    void *temp_r0_4;
    void *temp_r0_6;
    void *temp_r2_3;
    void *temp_r2_4;
    UnkStruct_ov14_boxsys *temp_r3;
    UnkStruct_ov14_boxsys *temp_r3_2;
    UnkStruct_ov14_boxsys *temp_r3_3;
    void *temp_r3_4;
    UnkStruct_ov14_boxsys *temp_r5_2;

    temp_r0 = ((void *)arg0->sys);
    ov14_021F34C8(temp_r0, ((UnkStruct_ov14_boxsys *)temp_r0)->unk_44C, 0, arg3);
    temp_r0_2 = ((void *)arg0->sys);
    temp_r1 = ((UnkStruct_ov14_boxsys *)temp_r0_2)->unk_44C;
    if (temp_r1 == ((UnkStruct_ov14_boxsys *)temp_r0_2)->unk_88CA) {
        ((UnkStruct_ov14_boxsys *)temp_r0_2)->unk_88C8 = 0U;
        ov14_021E7588(arg0, arg0->sys->unk_44C);
        ov14_021E8620(arg0->sys->unk_2F0);
        ov14_021F0234(arg0, ov14_021E9970, 0x82);
        return;
    }
    temp_r5 = ov14_021E6070(arg0, temp_r1, 6, 0);
    temp_r3 = ((void *)arg0->sys);
    ov14_021E6094(arg0, temp_r3->unk_44C, 6, &temp_r3->unk_88C8);
    if (ov14_021E64D0(ov14_021E60C0(arg0, arg0->unk1F, arg0->sys->unk_44C)) == 1) {
        temp_r3_2 = arg0->sys;
        temp_r2 = temp_r3_2->unk_44C;
        ov14_021F2ED0(arg0, arg0->unk1F, temp_r2, temp_r3_2->unk_4094[temp_r2]);
    }
    ov14_021E7588(arg0, arg0->sys->unk_44C);
    arg0->sys->unk_88C8 = temp_r5;
    temp_r5_2 = ((void *)arg0->sys);
    ov14_021E6094(arg0, temp_r5_2->unk_88CA, 6, &temp_r5_2->unk_88C8);
    if (ov14_021E64D0(ov14_021E60C0(arg0, arg0->unk1F, arg0->sys->unk_88CA)) == 1) {
        temp_r3_3 = arg0->sys;
        temp_r2_2 = temp_r3_3->unk_88CA;
        ov14_021F2ED0(arg0, arg0->unk1F, temp_r2_2, temp_r3_3->unk_4094[temp_r2_2]);
    }
    temp_r2_3 = ((void *)arg0->sys);
    if (((UnkStruct_ov14_boxsys *)temp_r2_3)->unk_88C8 == 0) {
        ov14_021E8620(*((void **)&((UnkStruct_ov14_boxsys *)temp_r2_3)->unk_2F0));
        ov14_021F0234(arg0, ov14_021E9970, 0x82);
        return;
    }
    ((UnkStruct_ov14_boxsys *)temp_r2_3)->unk_44B = 1;
    ov14_021F2A18(((void *)arg0->sys), 0xB, 0);
    temp_r0_3 = ((void *)arg0->sys);
    ov14_021F39A0(temp_r0_3, ((UnkStruct_ov14_boxsys *)temp_r0_3)->unk_44C, 2);
    temp_r0_4 = ((void *)arg0->sys);
    ov14_021F3844(temp_r0_4, ((UnkStruct_ov14_boxsys *)temp_r0_4)->unk_88C8);
    temp_r0_5 = ov14_021F5564(arg0, arg0->sys->unk_88C8);
    temp_r3_4 = ((void *)arg0->sys);
    sub_02019A60(*((void **)&((UnkStruct_ov14_boxsys *)temp_r3_4)->unk_2F0), 0x10, temp_r3_4 + 0x30 + (temp_r0_5 * 0x10));
    temp_r2_4 = ((void *)arg0->sys);
    sub_02019A60(*((void **)&((UnkStruct_ov14_boxsys *)temp_r2_4)->unk_2F0), 0x10, temp_r2_4 + 0x30 + ((temp_r0_5 + 1) * 0x10));
    temp_r0_6 = ((void *)arg0->sys);
    ov14_021F38B0(temp_r0_6, ((UnkStruct_ov14_boxsys *)temp_r0_6)->unk_88C8);
    ov14_021E88BC(*(void **)&arg0->sys->unk_2F0);
    ov14_021F0234(arg0, ov14_021EAA04, 0x88);
}
