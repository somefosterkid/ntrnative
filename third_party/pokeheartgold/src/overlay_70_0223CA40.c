#include "global.h"

#include "constants/heap.h"

#include "screen_fade.h"
#include "sprite.h"
#include "system.h"
#include "unk_02005D10.h"
#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238E50(void *, s32, s32);

s32 ov70_02241164(s32);

s32 ov70_0223CA40(GTSApplicationState *arg0);

void ov70_0223CAC4(GTSApplicationState *arg0, s32 arg1, u32 arg2);
s32 ov70_02245084(void *, String *, s32, s32, s32, s32);
s32 ov70_022450B8(void *, String *, s32, s32, s32, s32);
void ov70_0223CB1C(void *arg0, void *arg1, MsgData *arg2);

s32 ov70_0223CA40(GTSApplicationState *arg0) {
    s32 v0;

    if (gSystem.newKeys & 1) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 0x10, 1, HEAP_ID_61);
    } else {
        v0 = ov70_02241164(arg0->searchResultCount);
        if ((arg0->searchResultsVisible != 0) && (v0 >= 0)) {
#if defined(SDK_PORT) || defined(NONMATCHING)
            Sprite_SetAnimCtrlSeq(arg0->avatarSprites[v0 + 1], (v0 * 4) + 0x10);
#else
            // DS: the index is scaled before the field offset is added (avatarSprites[v0 + 1])
            Sprite_SetAnimCtrlSeq(*(Sprite **)(((u8 *)arg0 + ((v0 + 1) * 4)) + 0xEE4), (v0 * 4) + 0x10);
#endif
            arg0->currentScreenInstruction = 2;
            ov70_02238E50(arg0, 3, 0);
            arg0->selectedSearchResult = v0;
            PlaySE(0x5DCU);
        }
    }
    return 3;
}

void ov70_0223CAC4(GTSApplicationState *arg0, s32 strno, u32 textSpeed) {
    ReadMsgDataIntoString(arg0->gtsMessageLoader, strno, arg0->genericMessageBuffer);
    FillWindowPixelBuffer(&arg0->bottomInstructionWindow, 0xFU);
    DrawFrameAndWindow2(&arg0->bottomInstructionWindow, 0, 1U, 0xEU);
    arg0->textPrinter = (s32)AddTextPrinterParameterized(&arg0->bottomInstructionWindow, FONT_MESSAGE, arg0->genericMessageBuffer, 0U, 0U, textSpeed, NULL);
}

void ov70_0223CB1C(void *arg0, void *arg1, MsgData *msgData) {
    String *string;
    String *v0;
    String *v1;
    String *v2;
    String *v3;
    String *v4;

    string = NewString_ReadMsgData(msgData, 0x3D);
    ov70_02245084(arg0, string, 0, 0, 0, 0xF0200);
    String_Delete(string);
    v0 = NewString_ReadMsgData(msgData, 0x3F);
    ov70_02245084(arg0 + 0x20, v0, 0, 0, 0, 0xF0200);
    String_Delete(v0);
    v1 = NewString_ReadMsgData(msgData, 0x41);
    ov70_02245084(arg0 + 0x40, v1, 0, 0, 0, 0xF0200);
    String_Delete(v1);
    v2 = NewString_ReadMsgData(msgData, 0xAB);
    ov70_02245084(arg1, v2, 0, 0, 0, 0xF0200);
    String_Delete(v2);
    v3 = NewString_ReadMsgData(msgData, 0x43);
    ov70_022450B8(arg0 + 0x60, v3, 0, 0, 0, 0xF0200);
    String_Delete(v3);
    v4 = NewString_ReadMsgData(msgData, 0xEE);
    ov70_022450B8(arg0 + 0x70, v4, 0, 0, 0, 0xF0200);
    String_Delete(v4);
}
