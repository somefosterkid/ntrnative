#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"
#include "screen_fade.h"

u32 ov96_0220E394(PokeathlonCourseData *param_1, u8 *param_2);

u32 ov96_0220E394(PokeathlonCourseData *param_1, u8 *param_2) {
    BOOL BVar1;
    UnkStruct_ov96_0220E3F0 *pvVar2;
    u32 uVar3;
    unsigned char new_var;
#if !defined(SDK_PORT) && !defined(NONMATCHING)
    u32 *puVar4; // DS: walks a pointer to entries[i].unk_E0
#endif
    PokeathlonCourse_GetHeapAllocPtr4(param_1);
    if ((*param_2) == 0) {
        BVar1 = IsPaletteFadeFinished();
        if (BVar1 != 0) {
            BVar1 = ov96_021E5F24(param_1);
            pvVar2 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
            if (BVar1 == 0) {
                new_var = 0x3ffff;
                uVar3 = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
                do {
                    PokeathlonCourse_SetField5E0_AtIndex(param_1, uVar3 & 0xff, (u16)(u16)((pvVar2->entries[uVar3].unk_E0 << 14) >> 16));
                    uVar3 = uVar3 + 1;
                } while (((s32)uVar3) < 4);
#else
                puVar4 = &pvVar2->entries[0].unk_E0;
                do {
                    PokeathlonCourse_SetField5E0_AtIndex(param_1, uVar3 & 0xff, (u16)(u16)((*puVar4 << 14) >> 16));
                    uVar3 = uVar3 + 1;
                    puVar4 = (u32 *)((u8 *)puVar4 + sizeof(UnkStruct_ov96_0220E3F0_Entry));
                } while (((s32)uVar3) < 4);
#endif
            }
            return 1;
        }
    } else {
        GF_AssertFail();
    }
    return 0;
}
