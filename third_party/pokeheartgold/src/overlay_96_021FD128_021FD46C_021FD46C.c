#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "screen_fade.h"
#include "overlay_96_ov96_rower_work.h"

u32 ov96_021FD46C(PokeathlonCourseData *param_1, u8 *param_2);

u32 ov96_021FD46C(PokeathlonCourseData *param_1, u8 *param_2) {
    Ov96RowerWork *work;
    BOOL BVar2;
    u32 uVar3;
    Ov96Rower *rower;

    work = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    if (*param_2 == 0) {
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
    uVar3 = 0;
    rower = work->rowers;
    do {
        PokeathlonCourse_SetField5E0_AtIndex(param_1, uVar3 & 0xff, (rower->unk7C / 0x1000) + (u16)rower->unk9C * 0x400);
        uVar3 = uVar3 + 1;
        rower++;
    } while ((s32)uVar3 < 4);
    return 1;
}
