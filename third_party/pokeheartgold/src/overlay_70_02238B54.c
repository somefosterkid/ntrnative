#include "global.h"
struct GTSApplicationState;

#include "constants/heap.h"

#include "gf_gfx_planes.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_02238B54(SpriteTemplate *template, GTSApplicationState *appState, SpriteResourcesHeader *header, NNS_G2D_VRAM_TYPE whichScreen);

#if defined(SDK_PORT) || defined(NONMATCHING)
struct YesNoPrompt *ov70_02238C2C(void *, u8, s32, s32, s32, u8);
#else
struct YesNoPrompt *ov70_02238C2C(void *, s32, s32, s32, s32, s32);
#endif
struct YesNoPrompt *ov70_02238C14(void *arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4);

void ov70_02238B54(SpriteTemplate *template, GTSApplicationState *appState, SpriteResourcesHeader *header, NNS_G2D_VRAM_TYPE whichScreen) {
    template->spriteList = appState->spriteList;
    template->header = header;
    template->position.z = 0;
    template->scale.x = FX32_ONE;
    template->scale.y = FX32_ONE;
    template->scale.z = FX32_ONE;
    template->rotation = 0;
    template->drawPriority = 1;
    template->whichScreen = whichScreen;
    template->heapID = HEAP_ID_61;
}

void ov70_02238B80(GTSApplicationState *arg0);

void ov70_02238B80(GTSApplicationState *arg0) {
    CreateSpriteResourcesHeader(&arg0->cursorSpriteResourceHeader, 0, 0, 0, 0, -1, -1, 0, 0, arg0->spriteResourceCollection[0], arg0->spriteResourceCollection[1], arg0->spriteResourceCollection[2], arg0->spriteResourceCollection[3], NULL, NULL);
    CreateSpriteResourcesHeader(&arg0->avatarSpriteResourceHeader, 1, 1, 1, 1, -1, -1, 0, 0, arg0->spriteResourceCollection[0], arg0->spriteResourceCollection[1], arg0->spriteResourceCollection[2], arg0->spriteResourceCollection[3], NULL, NULL);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
}

struct YesNoPrompt *ov70_02238C14(void *arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4) {
    return ov70_02238C2C(arg0, arg1, arg2, arg3, 0, arg4);
}
