#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "pokemon.h"
#include "pokemon_types_def.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

BoxPokemon *ov14_021E60C0(void *, u32, s32);
void ov14_021F2A18(struct UnkStruct_ov14_boxsys *arg0, s32 arg1, s32 arg2);

void ov14_021E75F4(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    BoxPokemon *temp_r0;
    UnkStruct_ov14_mon *temp_r0_2;
    s32 temp_r7;
    UnkStruct_ov14_boxsys *temp_r0_3;
    UnkStruct_ov14_boxsys *temp_r0_4;

    temp_r0 = ov14_021E60C0(arg0, arg0->unk1F, arg1);
    temp_r7 = AcquireBoxMonLock(temp_r0);
    temp_r0_2 = ov14_021E7358(temp_r0);
    ov14_021F36DC(arg0, temp_r0_2, 2);
    temp_r0_3 = arg0->sys;
    ov14_021F2A18(temp_r0_3, (temp_r0_3->unk_88D0 ^ 1) + 2, 1);
    temp_r0_4 = arg0->sys;
    ov14_021F2A18(temp_r0_4, temp_r0_4->unk_88D0 + 2, 0);
    ov14_021F3D70(arg0->sys, temp_r0_2);
    ov14_021F5368(arg0, temp_r0_2);
    ov14_021E7468(temp_r0_2);
    ReleaseBoxMonLock(temp_r0, temp_r7);
}
