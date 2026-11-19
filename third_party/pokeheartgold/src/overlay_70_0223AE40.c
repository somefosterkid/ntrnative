#include "global.h"
struct GTSApplicationState;

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "sprite.h"
#include "unk_02005D10.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_0223AE40(GTSApplicationState *param_1);

void ov70_0223AE98(GTSApplicationState *arg0);
void ov70_0223AF30(struct GTSApplicationState *arg0);
s32 ov70_0223AF5C(GTSApplicationState *arg0);

s32 ov70_02238E50(void *, s32, s32);
s32 ov70_02241330(void *, s32, s32);
void ov70_0223AFA8(GTSApplicationState *arg0, s32 arg1);

void ov70_0223AE40(GTSApplicationState *param_1) {
    s32 iVar1;
    Window *window;

    RemoveWindow(&param_1->bottomInstructionWindow);
    ClearWindowTilemapAndCopyToVram(&param_1->menuButtonWindows[1]);
    ClearWindowTilemapAndCopyToVram(param_1->menuButtonWindows);
    RemoveWindow(&param_1->menuButtonWindows[1]);
    RemoveWindow(param_1->menuButtonWindows);
    iVar1 = 0;
    window = &param_1->infoWindows[13];
    do {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        iVar1 = iVar1 + 1;
        window = window + 1;
    } while (iVar1 < 0x10);
    return;
}

void ov70_0223AE98(GTSApplicationState *arg0) {
    u8 countryId;
    u8 cityId;
    void *v0;

    v0 = &arg0->searchResults[0].pokemon.bytes[0x124 * arg0->selectedSearchResult];
    arg0->genericMessageBuffer = String_New(0xB4U, HEAP_ID_61);
    MessageFormat_ResetBuffers(arg0->stringTemplate);
    countryId = *(u8 *)((u8 *)v0 + 0x11E);
    if (countryId != 0) {
        BufferCountryName(arg0->stringTemplate, 8U, (u32)countryId);
    }
    cityId = *(u8 *)((u8 *)v0 + 0x11F);
    if (cityId != 0) {
        BufferCityName(arg0->stringTemplate, 9U, (u32) * (u8 *)((u8 *)v0 + 0x11E), (u32)cityId);
    }
    arg0->trainerLocationStrings[0] = ReadMsgData_ExpandPlaceholders(arg0->stringTemplate, arg0->gtsMessageLoader, 0x52U, HEAP_ID_61);
    arg0->trainerLocationStrings[1] = ReadMsgData_ExpandPlaceholders(arg0->stringTemplate, arg0->gtsMessageLoader, 0x53U, HEAP_ID_61);
}

void ov70_0223AF30(GTSApplicationState *arg0) {
    String_Delete(arg0->trainerLocationStrings[0]);
    String_Delete(arg0->trainerLocationStrings[1]);
    String_Delete(arg0->genericMessageBuffer);
}

s32 ov70_0223AF5C(GTSApplicationState *arg0) {
    Sprite_SetDrawFlag(arg0->unk_F10, 0);
    if (arg0->screenArgument == 0x10) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 0x10, 1, HEAP_ID_61);
        arg0->currentScreenInstruction = 7;
    } else {
        arg0->currentScreenInstruction = 1;
        Sprite_SetDrawFlag(arg0->unk_F0C, 1);
    }
    return 3;
}

void ov70_0223AFA8(GTSApplicationState *arg0, s32 arg1) {
    if ((arg1 != arg0->selectedSearchResult) && (arg1 >= 0)) {
        {
            s32 i = arg1 + 1;
            Sprite_SetAnimCtrlSeq(arg0->avatarSprites[i], (arg1 * 4) + 0x10);
        }
        arg0->currentScreenInstruction = 2;
        ov70_02238E50(arg0, 3, 0x11);
        arg0->selectedSearchResult = arg1;
        ov70_02241330(arg0, arg1, 0);
        PlaySE(0x5DCU);
    }
}
