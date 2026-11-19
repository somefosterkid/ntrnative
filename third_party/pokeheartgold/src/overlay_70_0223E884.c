#include "global.h"

#include "unk_0203A3B0.h"
#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "sprite.h"
#include "text.h"
#include "overlay_70_gtsapplication_state.h"

int ov70_02238E44(void);

extern void (*ov70_02246614[17])(GTSApplicationState *);

void ov70_0223E884(GTSApplicationState *param_1);

s32 ov70_02238E58(void *);
s32 ov70_0223EA40(BgConfig *);
s32 ov70_0223EBD4(GTSApplicationState *);
s32 ov70_0223ECA4(void *);
s32 ov70_0223ED24(void *);
s32 ov70_0224212C(void *);
s32 ov70_0223E8A4(GTSApplicationState *arg0);
void ov70_0223E8E8(GTSApplicationState *arg0, s32 arg1, u32 arg2);

void ov70_0223E884(GTSApplicationState * param_1) {
    s32 a0;

    a0 = ov70_02238E44();
    sub_0203A930(a0);
    ov70_02246614[param_1->currentScreenInstruction](param_1);
    return;
}

s32 ov70_0223E8A4(GTSApplicationState *arg0) {
    sub_0203A914();
    ov70_0223ECA4(arg0);
    ov70_0224212C(arg0->unk_11A8);
    ov70_0223ED24(arg0);
    ov70_0223EBD4(arg0);
    ov70_0223EA40(((void *)arg0->bgConfig));
    Sprite_SetDrawFlag(arg0->unk_F10, 0);
    ov70_02238E58(arg0);
    return 1;
}

void ov70_0223E8E8(GTSApplicationState *arg0, s32 strno, u32 textSpeed) {
    String *string;

    string = NewString_ReadMsgData(arg0->gtsMessageLoader, strno);
    StringExpandPlaceholders(arg0->stringTemplate, arg0->genericMessageBuffer, string);
    FillWindowPixelBuffer(&arg0->bottomInstructionWindow, 0xFU);
    DrawFrameAndWindow2(&arg0->bottomInstructionWindow, 0, 1U, 0xEU);
    arg0->textPrinter = (s32)AddTextPrinterParameterized(&arg0->bottomInstructionWindow, FONT_MESSAGE, arg0->genericMessageBuffer, 0U, 0U, textSpeed, NULL);
    String_Delete(string);
}
