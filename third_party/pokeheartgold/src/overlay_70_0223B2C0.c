#include "global.h"

#include "overlay_70_gtsapplication_state.h"
#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sprite.h"
#include "text.h"
#include "overlay_70_s70.h"
#include "overlay_70_app_0223b2c0.h"
s32 ov70_0223B258(void *arg0);

s32 ov70_0223B2C0(void *arg0);

s32 ov70_0223B2D0(GTSApplicationState *arg0);
s32 ov70_02238E50(void *, s32, s32);
s32 ov70_0223B300(GTSApplicationState *arg0);
s32 ov70_0223B344(GTSApplicationState *arg0);
void ov70_0223B364(GTSApplicationState *arg0, s32 arg1, u32 arg2);
s32 ov70_02245084(Window *, String *, s32, s32, s32, s32);
void ov70_0223B3BC(MsgData *arg0, void *arg1, s32 arg2);
s32 ov70_022450B8(Window *, String *, s32, s32, s32, s32);
void ov70_0223B3EC(MsgData *arg0, void *arg1, s32 arg2);

s32 ov70_0223B2C0(void *arg0) {
    ov70_0223B258(arg0);
    *(s32 *)((u8 *)arg0 + 0x2C) = 1;
    return 3;
}

s32 ov70_0223B2D0(GTSApplicationState *arg0) {
    arg0->unk_F14 = (s32)(arg0->unk_F14 + 1);
    if (IsPaletteFadeFinished() != 0) {
        arg0->unk_F14 = 0;
        arg0->currentScreenInstruction = 1;
        Sprite_SetDrawFlag(arg0->unk_F0C, 1);
    }
    return 3;
}

s32 ov70_0223B300(GTSApplicationState *arg0) {
    arg0->unk_F14 = (s32)(arg0->unk_F14 - 1);
    if (IsPaletteFadeFinished() != 0) {
        *(u16 *)REG_POWCNT_ADDR &= 0xFFFF7FFF;
        arg0->currentScreenInstruction = 2;
        ov70_02238E50(arg0, 4, 0xF);
        arg0->unk_F14 = 0x10;
    }
    return 3;
}

s32 ov70_0223B344(GTSApplicationState *arg0) {
    if (TextPrinterCheckActive((u8)arg0->textPrinter) == 0) {
        arg0->currentScreenInstruction = (s32)arg0->nextScreenInstruction;
    }
    return 3;
}

void ov70_0223B364(GTSApplicationState *arg0, s32 strno, u32 textSpeed) {
    ReadMsgDataIntoString(((Ov70App_0223B2C0 *)arg0)->unk_BA0, strno, *((String **)&((S70 *)arg0)->unk_BBC));
    FillWindowPixelBuffer(&arg0->bottomInstructionWindow, 0xFU);
    DrawFrameAndWindow2(&arg0->bottomInstructionWindow, 0, 1U, 2U);
    *((s32 *)&((S70 *)arg0)->unk_BF0) = (s32)AddTextPrinterParameterized(&arg0->bottomInstructionWindow, FONT_MESSAGE, *((String **)&((S70 *)arg0)->unk_BBC), 0U, 0U, textSpeed, NULL);
}

void ov70_0223B3BC(MsgData *msgData, void *arg1, s32 strno) {
    String *string;

    string = NewString_ReadMsgData(msgData, strno);
    ov70_02245084(arg1, string, 0, 0, 0, 0xF0200);
    String_Delete(string);
}

void ov70_0223B3EC(MsgData *msgData, void *arg1, s32 strno) {
    String *string;

    string = NewString_ReadMsgData(msgData, strno);
    ov70_022450B8(arg1, string, 0, 0, 1, 0xF0200);
    String_Delete(string);
}
