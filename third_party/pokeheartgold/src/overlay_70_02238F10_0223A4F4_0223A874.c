#include "global.h"

#include "pokemon.h"
#include "screen_fade.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_0223F470(void *, void *, void *, void *, s32, s32, s32);

s32 ov70_0223F864(s32, s32, s32);

void ov70_0223A874(GTSApplicationState *arg0);

void ov70_0223A874(GTSApplicationState *arg0) {
    ov70_0223F470(arg0->gtsMessageLoader, arg0->speciesMessageLoader, arg0->stringTemplate, &arg0->infoWindows[22], (s32) arg0->receivedListing.requirements.species, (s32) arg0->receivedListing.requirements.gender, ov70_0223F864(arg0->receivedListing.requirements.level, arg0->receivedListing.requirements.level2, 0));
}

s32 ov70_0223A578(void *, void *, void *, void *, BoxPokemon *, void *);

s32 ov70_0223A72C(void *, void *, void *, void *, void *);

s32 ov70_0223A7E4(void *);

s32 ov70_0223AB3C(void *, s32);

s32 ov70_0223ABF4(void *);

s32 ov70_0223AC98(void *);

s32 ov70_0223ACF4(GTSApplicationState *);

s32 ov70_0223AE98(GTSApplicationState *arg0);

s32 ov70_0223B258(void *);

s32 ov70_0223B3BC(void *, void *, s32);

s32 ov70_0223B3EC(void *, void *, s32);

s32 ov70_02241330(void *, s32, s32);

extern s32 ov70_0223B4D4;

s32 ov70_0223A8BC(void *arg0);

s32 ov70_0223A8BC(void *arg0) {
    s32 v0;
    s32 v1;

    ov70_0223AE98(arg0);
    ov70_0223AB3C(*((void **)&((GTSApplicationState *)arg0)->bgConfig), -0x20 - ((GTSApplicationState *)arg0)->unk_F14);
    ov70_0223ABF4(arg0);
    ov70_0223ACF4(arg0);
    ov70_0223AC98(arg0);
    v1 = *((s32 *)&((GTSApplicationState *)arg0)->selectedSearchResult) * 0x124;
    ov70_0223A578(((GTSApplicationState *)arg0)->gtsMessageLoader, ((GTSApplicationState *)arg0)->speciesMessageLoader, ((GTSApplicationState *)arg0)->stringTemplate, arg0 + 0x1058, Mon_GetBoxMon(arg0 + 0x260 + v1), arg0 + 0x34C + v1);
    v0 = *((s32 *)&((GTSApplicationState *)arg0)->selectedSearchResult) * 0x124;
    ov70_0223A72C(((GTSApplicationState *)arg0)->gtsMessageLoader, arg0 + 0x10C8, arg0 + 0x36C + v0, arg0 + 0x260 + v0, arg0 + 0x1118);
    ov70_0223A7E4(arg0 + 0x260 + (0x124 * *((s32 *)&((GTSApplicationState *)arg0)->selectedSearchResult)));
    ov70_0223B3BC(((GTSApplicationState *)arg0)->gtsMessageLoader, arg0 + 0x1138, 0x4D);
    ov70_0223B3BC(((GTSApplicationState *)arg0)->gtsMessageLoader, arg0 + 0x10E8, 0x51);
    ov70_0223B3EC(((GTSApplicationState *)arg0)->gtsMessageLoader, arg0 + 0xF58, 0x58);
    ov70_0223B3EC(((GTSApplicationState *)arg0)->gtsMessageLoader, arg0 + 0xF68, 0x6D);
    ov70_0223B258(arg0);
    ov70_02241330(arg0, *((s32 *)&((GTSApplicationState *)arg0)->selectedSearchResult), 0 - ((GTSApplicationState *)arg0)->unk_F14);
    *((s32 **)&((GTSApplicationState *)arg0)->unk_1208) = &ov70_0223B4D4;
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    if (*((s32 *)&((GTSApplicationState *)arg0)->screenArgument) == 0x11) {
        if ((GX_GetMasterBrightness() == -0x10) && (GXS_GetMasterBrightness() != -0x10)) {
            BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_61);
        } else if ((GX_GetMasterBrightness() != -0x10) && (GXS_GetMasterBrightness() == -0x10)) {
            BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_61);
        } else {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_61);
        }
    }
    *((s32 *)&((GTSApplicationState *)arg0)->currentScreenInstruction) = 0;
    return 2;
}
