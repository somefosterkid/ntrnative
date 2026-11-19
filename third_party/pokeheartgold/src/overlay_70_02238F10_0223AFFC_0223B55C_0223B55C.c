#include "global.h"

#include "overlay_70_gtsapplication_state.h"

#include "constants/heap.h"

#include "screen_fade.h"

void ov70_0223B7CC(BgConfig *);

void ov70_0223B8E0(GTSApplicationState *);

void ov70_0223B9C8(GTSApplicationState *);

void ov70_0223BAE0(GTSApplicationState *);

void ov70_0223BC7C(void *arg0);

void ov70_0223CB1C(Window *, Window *, struct MsgData *);

void ov70_0223CC04(BgConfig *, Window *, struct MsgData *, s32);

void ov70_0223F1D8(Window *, struct MsgData *, s32, u32, u32, u32);

void ov70_0223F244(Window *, struct MsgData *, struct MsgData *, s32, u32, u32, u32);

void ov70_0223F2BC(Window *, struct MsgData *, s32, u32, u32, u32, u32);

void ov70_0223F370(Window *, struct MsgData *, u32, u32, u32, u32, u32);

u32 ov70_0223F864(s32, s32, u32);

struct UnkStruct_ov70_02242E58 *ov70_02242014(GTSSearchPanelTemplate *, u32, u32);

void ov70_0223CCA4(void *arg0);

u32 ov70_0223B55C(GTSApplicationState *param_1);

u32 ov70_0223B55C(GTSApplicationState *param_1) {
    u32 uVar2;
    GTSSearchPanelTemplate iStack_3c;

    ov70_0223BC7C(param_1);
    ov70_0223B7CC(param_1->bgConfig);
    ov70_0223B8E0(param_1);
    ov70_0223BAE0(param_1);
    ov70_0223B9C8(param_1);
    iStack_3c.bgConfig = param_1->bgConfig;
    iStack_3c.menuButtonWindows = param_1->menuButtonWindows;
    iStack_3c.unk_08 = &param_1->unk_1168;
    iStack_3c.unk_0C = param_1->unused_DD0;
    iStack_3c.unk_10 = param_1->boxArrowSprites[0];
    iStack_3c.unk_14 = param_1->boxArrowSprites[1];
    iStack_3c.cursorSprite = param_1->cursorSprite;
    iStack_3c.gtsMessageLoader = param_1->gtsMessageLoader;
    iStack_3c.speciesMessageLoader = param_1->speciesMessageLoader;
    iStack_3c.countryMessageLoader = param_1->countryMessageLoader;
    iStack_3c.pokedex = *(struct Pokedex **)((u8 *)param_1->playerData + 0x10);
    iStack_3c.dexSeenFlags = param_1->speciesMenuState->dexSeenFlags;
    param_1->unk_11A8 = ov70_02242014(&iStack_3c, 2, 1);
    ov70_0223CB1C(&param_1->infoWindows[13], &param_1->locationCriteriaWindows[0], param_1->gtsMessageLoader);
    ov70_0223CC04(param_1->bgConfig, &param_1->infoWindows[21], param_1->gtsMessageLoader, param_1->searchResultsVisible);
    ov70_0223F1D8(&param_1->infoWindows[14], param_1->speciesMessageLoader, (s32) param_1->searchRequirements.species, 0, 0, 0x10200);
    ov70_0223F2BC(&param_1->infoWindows[16], param_1->gtsMessageLoader, (s32) param_1->searchRequirements.gender, 1, 0, 0, 0x10200);
    uVar2 = ov70_0223F864((s32) param_1->searchRequirements.level, (s32) param_1->searchRequirements.level2, 1);
    ov70_0223F370(&param_1->infoWindows[18], param_1->gtsMessageLoader, uVar2, 0, 0, 0x10200, 1);
    ov70_0223F244(&param_1->locationCriteriaWindows[1], param_1->countryMessageLoader, param_1->gtsMessageLoader, param_1->selectedCountryIndex, 0, 0, 0x10200);
    param_1->unk_1208 = (void *)ov70_0223CCA4;
    if (param_1->screenArgument == 0xd) {
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_61);
    }
    param_1->currentScreenInstruction = 0;
    return 2;
}
