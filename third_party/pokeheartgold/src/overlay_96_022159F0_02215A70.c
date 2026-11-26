#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "screen_fade.h"

#include "overlay_96_ov96_rd_work.h"

s32 ov96_02215A70(PokeathlonCourseData *param_1, u8 *param_2);

s32 ov96_02215A70(PokeathlonCourseData *param_1, u8 *param_2) {
    Ov96RdWork *pvVar1;
    BOOL BVar2;
    u32 uVar3;
    pvVar1 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    if ((*param_2) == 0) {
        BVar2 = IsPaletteFadeFinished();
        if (0 != BVar2) {
            BVar2 = ov96_021E5F24(param_1);
            if (BVar2 == 0) {
                uVar3 = 0;
                do {
                    PokeathlonCourse_SetField5E0_AtIndex(param_1, uVar3 & 0xff, pvVar1->course.players[uVar3].obj.cnt);
                    uVar3 = uVar3 + 1;
                } while (((s32)uVar3) < 4);
            }
            return 1;
        }
    } else {
        GF_AssertFail();
    }
    return 0;
}
