#include "global.h"

#include "constants/heap.h"

#include "overlay_112_pokewalker_app.h"
#include "screen_fade.h"

void ov112_021F050C(Ov112SubScreen *);

s32 sub_020935E0(void *arg0, s32);

void sub_02093A40(void *);

u32 ov112_021EF03C(Ov112PokewalkerApp *param_1, s32 arg1) {
    s32 iVar1;
    BOOL BVar2;
    Ov112PokewalkerLink *puVar3;

    puVar3 = &param_1->link;
    switch (puVar3->active) {
    case 0:
        puVar3->active = 1;
        break;
    case 1:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_154);
        puVar3->active = 2;
        break;
    case 2:
        BVar2 = IsPaletteFadeFinished();
        if (BVar2 != 0) {
            puVar3->active = 3;
        }
        break;
    case 3:
        iVar1 = sub_020935E0(puVar3->unk_4, arg1);
        if (iVar1 == 2) {
            puVar3->unk_14 = 0;
            puVar3->active = 4;
        } else if (iVar1 == 3) {
            puVar3->unk_14 = 1;
            puVar3->active = 4;
        }
        break;
    case 4:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_154);
        puVar3->active = 5;
        break;
    case 5:
        BVar2 = IsPaletteFadeFinished();
        if (BVar2 != 0) {
            ov112_021EF31C(puVar3, param_1);
            return 3;
        }
    }
    sub_02093A40(puVar3->unk_4);
    ov112_021F050C(puVar3->subScreen);
    return 2;
}
