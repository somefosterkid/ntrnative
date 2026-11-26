#include "global.h"

#include "touchscreen.h"
#include "pokemon.h"
#include "screen_fade.h"
#include "unk_02005D10.h"
#include "overlay_70_gtsapplication_state.h"

extern TouchscreenHitbox ov70_022456C8;

void ov70_0223BD80(void);

s32 ov70_02238D84(void *, s32, s32);
s32 ov70_0223CAC4(void *, s32, s32, s32, s32);
s32 ov70_0223EDE4(void *, s32);
void ov70_0223BD90(void *arg0, u32 arg1);

void ov70_0223BD80(void) {
    TouchscreenHitbox_FindRectAtTouchNew(&ov70_022456C8);
}

void ov70_0223BD90(void *arg0, u32 arg1) {
    s16 species;

    switch (arg1) {
    case 0:
        *((s32 *)&((GTSApplicationState *)arg0)->currentScreenInstruction) = 3;
        PlaySE(0x5DCU);
        return;
    case 1:
        species = ((GTSApplicationState *)arg0)->searchRequirements.species;
        if (species != 0) {
            ((GTSApplicationState *)arg0)->speciesMenuState->selectedSpeciesGenderRatio = GetMonBaseStat((s32)species, 0x12);
            if (ov70_0223EDE4(&((GTSApplicationState *)arg0)->searchRequirements.species, ((GTSApplicationState *)arg0)->speciesMenuState->selectedSpeciesGenderRatio) != 0) {
                PlaySE(0x5DCU);
                return;
            }
        }
        *((s32 *)&((GTSApplicationState *)arg0)->currentScreenInstruction) = 6;
        PlaySE(0x5DCU);
        return;
    case 2:
        *((s32 *)&((GTSApplicationState *)arg0)->currentScreenInstruction) = 9;
        PlaySE(0x5DCU);
        return;
    case 3:
        *((s32 *)&((GTSApplicationState *)arg0)->currentScreenInstruction) = 0xC;
        PlaySE(0x5DCU);
        return;
    case 4:
        if (*((s32 *)&((GTSApplicationState *)arg0)->searchResultCount) != 0) {
            *((s32 *)&((GTSApplicationState *)arg0)->currentScreenInstruction) = 0x1D;
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 0x10, 1, HEAP_ID_61);
            PlaySE(0x5DCU);
            return;
        }
    default:
        return;
    case 5:
        *((s32 *)&((GTSApplicationState *)arg0)->currentScreenInstruction) = 0xF;
        return;
    case 6:
        ov70_0223CAC4(arg0, 0xF, 1, 0, 0xF0F);
        ov70_02238D84(arg0, 0x15, 0x17);
        PlaySE(0x5DCU);
        break;
    }
}
