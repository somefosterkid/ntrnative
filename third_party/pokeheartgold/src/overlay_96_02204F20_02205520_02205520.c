#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "screen_fade.h"

u32 ov96_02205520(PokeathlonCourseData *param_1, u8 *param_2);

u32 ov96_02205520(PokeathlonCourseData *param_1, u8 *param_2) {
    u16 value;
    u8 *puVar1;
    u32 uVar5;
    BOOL BVar2;
    u32 *puVar3;
    u32 *new_var;
    u32 *puVar6;
    u16 uVar4;
    puVar1 = PokeathlonCourse_GetDataCopyArea(param_1);
    PokeathlonCourse_GetHeapAllocPtr4(param_1);
    if (!(*param_2)) {
        BVar2 = IsPaletteFadeFinished();
        if (BVar2 != 0) {
            *param_2 = 1;
        }
        return 0;
    }
    BVar2 = ov96_021E5F24(param_1);
    if (BVar2 != 0) {
        return 1;
    }
    new_var = (u32 *)ov96_021E8A20(puVar1 + 0x28);
    uVar5 = 0;
    puVar3 = (puVar6 = new_var);
    do {
        uVar4 = (u16)(0x3ff & ((*puVar3) >> 0x12)) + (u16)(0x1f & (*(u16 *)((u8 *)puVar6 + 0x18) >> 0xb));
        value = (u16)uVar4;
        if (999 < uVar4) {
            value = 999;
        }
        PokeathlonCourse_SetField5E0_AtIndex(param_1, uVar5 & 0xff, value);
        uVar5 = uVar5 + 1;
        puVar6 = (u32 *)(((u8 *)puVar6) + 2);
        puVar3 = puVar3 + 1;
    } while (((s32)uVar5) < 4);
    return 1;
}
