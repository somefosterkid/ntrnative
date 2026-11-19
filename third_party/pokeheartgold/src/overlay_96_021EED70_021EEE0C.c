#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "gf_gfx_planes.h"
#include "screen_fade.h"
#include "unk_0203A3B0.h"

#include "overlay_96_app_021eefac.h"

u32 ov96_021EE830(Ov96TextBox *box);

void ov96_021EE8CC(Ov96TextBox *box, s32 strno);

void ov96_021EE994(Ov96TextBox *box);

void ov96_021EEFAC(PokeathlonCourseData *param_1);

u32 ov96_021EEE0C(PokeathlonCourseData *param_1, u32 param_2, u32 param_3, u32 param_4);

u32 ov96_021EEE0C(PokeathlonCourseData *param_1, u32 param_2, u32 param_3, u32 param_4) {
    u8 uVar1;
    Ov96App_021EEFAC *pHVar2;
    BOOL BVar3;
    u32 uVar4;

    pHVar2 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    uVar1 = PokeathlonCourse_GetField1ED(param_1);
    switch (uVar1) {
    case 0:
        ov96_021EEFAC(param_1);
        PokeathlonCourse_IncrementField1ED(param_1);
        break;
    case 1:
        sub_0203A994(1);
        GfGfx_EngineATogglePlanes(0x10, 1);
        GfGfx_EngineBTogglePlanes(0x10, 0);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 4, 1, pHVar2->heapId);
        PokeathlonCourse_IncrementField1ED(param_1);
        break;
    case 2:
        BVar3 = IsPaletteFadeFinished();
        if (BVar3 != 0) {
            ov96_021EE8CC(pHVar2->textBox, 0x136);
            PokeathlonCourse_IncrementField1ED(param_1);
        }
        break;
    case 3:
        uVar4 = ov96_021EE830(pHVar2->textBox);
        if (uVar4 == 0) {
            ov96_021EE994(pHVar2->textBox);
            PokeathlonCourse_SetStateTransitionType(param_1, 0x11);
            return 1;
        }
        break;
    default:
        GF_AssertFail();
    }
    return 0;
}
