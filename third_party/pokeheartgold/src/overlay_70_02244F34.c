#include "global.h"

#include "text.h"
#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "overlay_70_gtsapplication_state.h"
u32 ov70_02244F34(GTSApplicationState *param_1);

struct YesNoPrompt *ov70_02238C14(void *, s32, s32, s32, u8);
s32 ov70_02244F68(GTSApplicationState *arg0);
void ov70_02244FA4(GTSApplicationState *arg0, MsgData *arg1, s32 arg2, u32 arg3);

u32 ov70_02244F34(GTSApplicationState *param_1) {
    u8 uVar1;

    uVar1 = TextPrinterCheckActive((u8)param_1->textPrinter);
    if (uVar1 == 0) {
        if (0x1e < param_1->unk_11C0) {
            param_1->currentScreenInstruction = param_1->nextScreenInstruction;
        }
        param_1->unk_11C0 = param_1->unk_11C0 + 1;
    }
    return 3;
}

s32 ov70_02244F68(GTSApplicationState *arg0) {
    if (TextPrinterCheckActive((u8) arg0->textPrinter) == 0) {
        arg0->unk_11C8 = ov70_02238C14(arg0->bgConfig, 0xA, 0x234, 8, 0);
        arg0->currentScreenInstruction = (s32) arg0->nextScreenInstruction;
    }
    return 3;
}

void ov70_02244FA4(GTSApplicationState *arg0, MsgData *msgData, s32 strno, u32 textSpeed) {
    String *string;

    string = NewString_ReadMsgData(msgData, strno);
    StringExpandPlaceholders(arg0->stringTemplate, arg0->genericMessageBuffer, string);
    String_Delete(string);
    FillWindowPixelBuffer(&arg0->bottomInstructionWindow, 0xFU);
    DrawFrameAndWindow2(&arg0->bottomInstructionWindow, 0, 1U, 0xEU);
    arg0->textPrinter = (s32)AddTextPrinterParameterized(&arg0->bottomInstructionWindow, FONT_MESSAGE, arg0->genericMessageBuffer, 0U, 0U, textSpeed, NULL);
    arg0->unk_11C0 = 0;
}
