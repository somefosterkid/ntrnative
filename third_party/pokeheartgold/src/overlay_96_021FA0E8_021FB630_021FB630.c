#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_unk_struct_ov96_021_f94a8.h"

void ov96_021FBA3C(PokeathlonCourseData *param_1, UnkStruct_ov96_021F94A8 *param_2, void *param_3, u32 param_4, u32 param_5, u8 param_6);

void ov96_021FB630(PokeathlonCourseData *param_1);

void ov96_021FB630(PokeathlonCourseData *param_1) {
    u8 bVar1;
    BOOL BVar2;
    u8 *puVar5;
    u32 uVar6;
    s32 iVar7;
    UnkStruct_ov96_021F94A8 *pvVar3;
    s16 uVar4;
    u8 *puStack_24;
    BVar2 = ov96_021E5F24(param_1);
    if (BVar2 == 0) {
        puStack_24 = PokeathlonCourse_GetDataCopyArea(param_1);
        pvVar3 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
        bVar1 = PokeathlonCourse_GetParticipantCount(param_1);
        uVar4 = bVar1;
        BVar2 = uVar4;
        BVar2 = uVar4;
        if (BVar2 < 4) {
            puStack_24 = (puStack_24 + 0x50) + (uVar4 * 0x28);
            do {
                puVar5 = ov96_021E8A20(puStack_24);
                uVar6 = 0;
                iVar7 = (BVar2 - uVar4) * 3;
                do {
                    ov96_021FBA3C(param_1, pvVar3, (u8 *)puVar5, BVar2 & 0xff, uVar6 & 0xff, (u8)iVar7);
                    uVar6 = uVar6 + 1;
                    iVar7 = iVar7 + 1;
                } while (((s32)uVar6) < 3);
                puStack_24 = puStack_24 + 0x28;
                BVar2 = BVar2 + 1;
            } while (((s32)BVar2) < 4);
        }
    }
    return;
}
