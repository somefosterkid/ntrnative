#include "global.h"
struct GTSApplicationState;

#include "bg_window.h"
#include "sprite.h"
#include "heap.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_02239D44(void *, u32);

extern u32 ov70_02245920;

void ov70_0223EB34(GTSApplicationState *param_1);

void ov70_0223EBD4(GTSApplicationState *param_1);
void ov70_02238B54(SpriteTemplate *template, struct GTSApplicationState *appState, SpriteResourcesHeader *header, NNS_G2D_VRAM_TYPE whichScreen);
void ov70_0223EC0C(GTSApplicationState *param_1);

void ov70_0223ECA4(struct GTSApplicationState *arg0);
u16 *ov70_0223F634(s32, s32, void *);
u8 *ov70_0223F684(s32);
s32 ov70_0223F948(void *);
void ov70_0223ECCC(GTSApplicationState *arg0);
void ov70_0223ED24(struct GTSApplicationState *arg0);
s32 ov70_02238D84(GTSApplicationState *, s32, s32);
s32 ov70_0223E8E8(GTSApplicationState *, s32, s32, s32, s32);
s32 ov70_0223ED58(GTSApplicationState *arg0);

void ov70_0223EB34(GTSApplicationState *param_1) {
    u16 *puVar1;
    Window *window;
    s32 iVar2;
    u32 baseTile;

    AddWindowParameterized(param_1->bgConfig, (Window *)(&param_1->bottomInstructionWindow), 0, 2, 0x15, 0x1b, 2, 0xd, 0x60);
    FillWindowPixelBuffer((Window *)(&param_1->bottomInstructionWindow), 0);
    puVar1 = (u16 *)&ov70_02245920;
    iVar2 = 0;
    baseTile = 0x96;
    window = (Window *)(&param_1->infoWindows[13]);
    do {
        AddWindowParameterized(param_1->bgConfig, window, 3, (u8)*puVar1, (u8)puVar1[1], 0xb, 2, 0xd, baseTile);
        FillWindowPixelBuffer(window, 0);
        CopyWindowToVram(window);
        iVar2 = iVar2 + 1;
        baseTile = baseTile + 0x16;
        puVar1 = puVar1 + 2;
        window = window + 1;
    } while (iVar2 < 6);
    ov70_02239D44(param_1, 3);
    return;
}

void ov70_0223EBD4(GTSApplicationState *param_1) {
    s32 iVar1;
    Window *window;

    RemoveWindow(&param_1->unk_1198);
    iVar1 = 0;
    window = &param_1->infoWindows[13];
    do {
        RemoveWindow(window);
        iVar1 = iVar1 + 1;
        window = window + 1;
    } while (iVar1 < 6);
    RemoveWindow(&param_1->bottomInstructionWindow);
    return;
}

void ov70_0223EC0C(GTSApplicationState *param_1) {
    Sprite *pSVar1;
    SpriteTemplate SStack_38;

    ov70_02238B54(&SStack_38, param_1, &param_1->cursorSpriteResourceHeader, NNS_G2D_VRAM_TYPE_2DMAIN);
    SStack_38.position.x = 0xa0000;
    SStack_38.position.y = 0x20000;
    pSVar1 = Sprite_CreateAffine(&SStack_38);
    param_1->unused_DD0 = pSVar1;
    Sprite_SetAnimCtrlSeq(param_1->unused_DD0, 0x2f);
    Sprite_SetDrawFlag(param_1->unused_DD0, 0);
    SStack_38.position.x = 0xe4000;
    SStack_38.position.y = 0x75000;
    pSVar1 = Sprite_CreateAffine(&SStack_38);
    param_1->boxArrowSprites[0] = pSVar1;
    Sprite_SetAnimCtrlSeq(param_1->boxArrowSprites[0], 0x26);
    Sprite_SetDrawFlag(param_1->boxArrowSprites[0], 0);
    SStack_38.position.x = 0x8c000;
    pSVar1 = Sprite_CreateAffine(&SStack_38);
    param_1->boxArrowSprites[1] = pSVar1;
    Sprite_SetAnimCtrlSeq(param_1->boxArrowSprites[1], 0x27);
    Sprite_SetDrawFlag(param_1->boxArrowSprites[1], 0);
    return;
}

void ov70_0223ECA4(GTSApplicationState *arg0) {
    Sprite_Delete(arg0->unused_DD0);
    Sprite_Delete(arg0->boxArrowSprites[0]);
    Sprite_Delete(arg0->boxArrowSprites[1]);
}

void ov70_0223ECCC(GTSApplicationState *arg0) {
    arg0->genericMessageBuffer = String_New(0xB4U, HEAP_ID_61);
    arg0->speciesMenuState = Heap_Alloc(HEAP_ID_61, sizeof(GTSSpeciesMenuState));
    MIi_CpuClearFast(0U, arg0->speciesMenuState, sizeof(GTSSpeciesMenuState));
    arg0->speciesMenuState->alphabeticalSpecies = ov70_0223F634(0x3D, 0, &arg0->speciesMenuState->alphabeticalSpeciesCount);
    arg0->speciesMenuState->dexSeenFlags = ov70_0223F684(0x3D);
    ov70_0223F948(&arg0->charpadScrollState);
}

void ov70_0223ED24(GTSApplicationState *arg0) {
    Heap_Free(arg0->speciesMenuState->dexSeenFlags);
    Heap_Free(arg0->speciesMenuState->alphabeticalSpecies);
    Heap_Free(arg0->speciesMenuState);
    String_Delete(arg0->genericMessageBuffer);
}

s32 ov70_0223ED58(GTSApplicationState *arg0) {
    if (IsPaletteFadeFinished() != 0) {
        ov70_0223E8E8(arg0, 9, 1, 0, 0xF0F);
        ov70_02238D84(arg0, 0x10, 1);
    }
    return 3;
}
