#include "global.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

#include "pokeathlon/pokeathlon.h"

void ov96_021F715C(PokeathlonCourseData *param_1, void *param_2, s32 param_3, UnkStruct_ov96_021F5018_Ball *param_4);

void ov96_021F715C(PokeathlonCourseData *param_1, void *param_2, s32 param_3, UnkStruct_ov96_021F5018_Ball *param_4) {
    unsigned char uVar1;
    PokeathlonMonKinds *iVar2;
    uVar1 = ov96_021E5F24(param_1);
    iVar2 = ov96_021E60D8(param_1, uVar1 = (u8)uVar1, param_3);
    param_4->unk_2C = *((u32 *)(param_2 + (((u32)(iVar2->statIdx[2])) * 4)));
    param_4->unk_30 = *((u32 *)((param_2 + (((u32)(iVar2->statIdx[3])) * 4)) + 0x14));
    param_4->unk_34 = *((u32 *)(((param_2) + (((u32)(iVar2->statIdx[4])) * 4)) + 0x28));
    return;
}
