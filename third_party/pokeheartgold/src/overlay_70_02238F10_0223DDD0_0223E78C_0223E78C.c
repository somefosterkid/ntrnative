#include "global.h"

#include "overlay_70_gtsapplication_state.h"

#include "constants/heap.h"

#include "screen_fade.h"

void ov70_0223E954(BgConfig *);

void ov70_0223EA6C(GTSApplicationState *);

void ov70_0223EB34(GTSApplicationState *);

void ov70_0223EC0C(GTSApplicationState *);

void ov70_0223ECCC(GTSApplicationState *);

void ov70_0223F3D8(struct MsgData *, struct MsgData *, struct MessageFormat *, Window *, u32, u32, u32);

void ov70_0223F508(struct MsgData *, struct MessageFormat *, Window *, struct BoxPokemon *, GTSPokemonCriteria *);

struct UnkStruct_ov70_02242E58 *ov70_02242014(GTSSearchPanelTemplate *, u32, u32);

void ov70_02245124(GTSApplicationState *);

typedef struct Words2 {
    u32 w[2];
} Words2;

u32 ov70_0223E78C(GTSApplicationState *param_1);

u32 ov70_0223E78C(GTSApplicationState *param_1) {
    GTSSearchPanelTemplate iStack_3c;

    ov70_0223ECCC(param_1);
    ov70_0223E954(param_1->bgConfig);
    ov70_0223EA6C(param_1);
    ov70_0223EB34(param_1);
    ov70_0223EC0C(param_1);
    BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_61);
    ov70_02245124(param_1);
    ov70_0223F3D8(param_1->gtsMessageLoader, param_1->speciesMessageLoader, param_1->stringTemplate, &param_1->infoWindows[13], 0, 3, -1);
    ov70_0223F508(param_1->gtsMessageLoader, param_1->stringTemplate, &param_1->infoWindows[16], param_1->selectedBoxMon, &param_1->offeredPokemonCriteria);
    iStack_3c.bgConfig = param_1->bgConfig;
    iStack_3c.menuButtonWindows = param_1->menuButtonWindows;
    iStack_3c.unk_08 = &param_1->unk_1168;
    iStack_3c.unk_0C = param_1->unused_DD0;
    iStack_3c.unk_10 = param_1->boxArrowSprites[0];
    iStack_3c.unk_14 = param_1->boxArrowSprites[1];
    iStack_3c.cursorSprite = 0;
    iStack_3c.gtsMessageLoader = param_1->gtsMessageLoader;
    iStack_3c.speciesMessageLoader = param_1->speciesMessageLoader;
    iStack_3c.countryMessageLoader = param_1->countryMessageLoader;
    iStack_3c.pokedex = *(struct Pokedex **)((u8 *)param_1->playerData + 0x10);
    iStack_3c.dexSeenFlags = param_1->speciesMenuState->dexSeenFlags;
    param_1->unk_11A8 = ov70_02242014(&iStack_3c, 2, 0);
    param_1->currentScreenInstruction = 0;
    return 2;
}
