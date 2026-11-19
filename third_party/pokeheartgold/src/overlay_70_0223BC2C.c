#include "global.h"
struct GTSApplicationState;

#include "bg_window.h"
#include "heap.h"
#include "msgdata.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "sprite.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_0223BC2C(GTSApplicationState *param_1);

u8 *ov70_0223F684(s32);
s32 ov70_0223F948(void *);
void ov70_0223BC7C(GTSApplicationState *arg0);
void ov70_0223BCD0(struct GTSApplicationState *arg0);
s32 ov70_02238D84(void *, s32, s32);
s32 ov70_0223CAC4(void *, s32, s32, s32, s32);
s32 ov70_0223BD04(GTSApplicationState *arg0);

void ov70_0223BC2C(GTSApplicationState *param_1) {
    s32 iVar1;
    Window *pWVar2;

    RemoveWindow(&param_1->unk_1198);
    RemoveWindow(&param_1->bottomInstructionWindow);
    iVar1 = 0;
    pWVar2 = &param_1->infoWindows[13];
    do {
        RemoveWindow(pWVar2);
        iVar1 = iVar1 + 1;
        pWVar2 = pWVar2 + 1;
    } while (iVar1 < 9);
    iVar1 = 0;
    pWVar2 = param_1->locationCriteriaWindows;
    do {
        RemoveWindow(pWVar2);
        iVar1 = iVar1 + 1;
        pWVar2 = pWVar2 + 1;
    } while (iVar1 < 2);
    return;
}

void ov70_0223BC7C(GTSApplicationState *arg0) {
    arg0->genericMessageBuffer = String_New(0xB4U, HEAP_ID_61);
    arg0->title = NewString_ReadMsgData(arg0->gtsMessageLoader, 0x2A);
    arg0->speciesMenuState = Heap_Alloc(HEAP_ID_61, sizeof(GTSSpeciesMenuState));
    MIi_CpuClearFast(0U, arg0->speciesMenuState, sizeof(GTSSpeciesMenuState));
    arg0->speciesMenuState->dexSeenFlags = ov70_0223F684(0x3D);
    ov70_0223F948(&arg0->charpadScrollState);
}

void ov70_0223BCD0(GTSApplicationState *arg0) {
    Heap_Free(arg0->speciesMenuState->dexSeenFlags);
    Heap_Free(arg0->speciesMenuState);
    String_Delete(arg0->genericMessageBuffer);
    String_Delete(arg0->title);
}

s32 ov70_0223BD04(GTSApplicationState *arg0) {
    if (arg0->screenArgument == 0xF) {
        ov70_0223CAC4(arg0, 0x20, 0, 0, 0xF0F);
        if (GX_GetMasterBrightness() == -0x10) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 0x10, 1, HEAP_ID_61);
            arg0->currentScreenInstruction = 0x1E;
        } else {
            arg0->currentScreenInstruction = 1;
        }
    } else {
        ov70_0223CAC4(arg0, 8, 1, 0, 0xF0F);
        ov70_02238D84(arg0, 0x15, 1);
    }
    Sprite_SetDrawFlag(arg0->unk_F10, 1);
    return 3;
}
