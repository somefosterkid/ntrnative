#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "screen_fade.h"
#include "overlay_96_ov96_0220_data.h"

u16 ov96_0220AD4C(void *arg0);

u32 ov96_022093D0(PokeathlonCourseData *param_1);

u32 ov96_022093D0(PokeathlonCourseData *param_1) {
    u16 value;
    Ov96_0220Data *pvVar2;
    BOOL BVar3;
    s32 uVar4;
    u8 *puVar5;
    s32 iVar6;
    pvVar2 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    BVar3 = IsPaletteFadeFinished();
    if (BVar3 != 0) {
        uVar4 = ov96_021E5F24(param_1);
        value = ov96_0220AD4C(pvVar2->unk_40);
        PokeathlonCourse_SetField5E0_AtIndex(param_1, uVar4 & 0xff, value);
        if (uVar4 == 0) {
            iVar6 = PokeathlonCourse_GetParticipantCount(param_1);
            uVar4 = (u32)iVar6;
            if (uVar4 < 4) {
                iVar6 = uVar4 * 0x28;
                do {
                    puVar5 = PokeathlonCourse_GetDataCopyArea(param_1);
                    puVar5 = ov96_021E8A20((puVar5 + 0x50) + iVar6);
                    PokeathlonCourse_SetField5E0_AtIndex(param_1, uVar4 & 0xff, (*((u32 *)puVar5)) & 0xff);
                    uVar4 = uVar4 + 1;
                    iVar6 = iVar6 + 0x28;
                } while (((s32)uVar4) < 4);
            }
        }
        return 1;
    }
    return 0;
}
