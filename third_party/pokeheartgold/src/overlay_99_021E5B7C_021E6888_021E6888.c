#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "overlay_98.h"
#include "overlay_99_obj99.h"
#include "overlay_manager.h"
#include "screen_fade.h"
#include "unk_02005D10.h"

s32 ov98_0221EF24(void);

extern void _021EA5CC(void);

BOOL ov99_021E6888(OverlayManager *man, s32 *state);

BOOL ov99_021E6888(OverlayManager *man, s32 *state) {
    Obj99 *pvVar1;
    s32 iVar2;
    s32 sVar3;

    pvVar1 = OverlayManager_GetData(man);
    switch (*state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, pvVar1->heap);
        *state = 1;
        break;
    case 1:
        sVar3 = IsPaletteFadeFinished();
        if (sVar3 != 0) {
            *state = 2;
        }
        break;
    case 2:
        iVar2 = ov98_0221EF24();
        if (iVar2 != 0) {
            PlaySE(0x5dc);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, pvVar1->heap);
            *state = 3;
        }
        break;
    case 3:
        sVar3 = IsPaletteFadeFinished();
        if (sVar3 != 0) {
            *state = 4;
        }
        break;
    case 4:
        return 1;
    default:
        if ((u32)_021EA5CC != 0) {
            GF_AssertFail();
        }
    }
    ov98_0221E6CC(pvVar1->spriteContext);
    return 0;
}
