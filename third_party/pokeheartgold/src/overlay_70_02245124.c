#include "global.h"

#include "sys_task.h"
#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"
#include "overlay_70_gtsapplication_state.h"

void sub_0203A880(void);

void ov70_02245124(void);

void ov70_0224512C(GTSApplicationState *arg0, s32 arg1);
void ov70_022451A8(GTSApplicationState *arg0, s32 arg1, s32 arg2);

void ov70_02245124(void) {
    sub_0203A880();
}

void ov70_0224512C(GTSApplicationState *arg0, s32 strno) {
    String *string;

    string = String_New(0x100U, HEAP_ID_61);
    ReadMsgDataIntoString(arg0->disconnStatusMessageLoader, strno, string);
    StringExpandPlaceholders(arg0->stringTemplate, arg0->shortErrorBuffer, string);
    FillWindowPixelBuffer(&arg0->networkErrWindow, 0xFU);
    DrawFrameAndWindow1(&arg0->networkErrWindow, 1, 0x1FU, 0xBU);
    arg0->textPrinter = (s32)AddTextPrinterParameterized(&arg0->networkErrWindow, FONT_MESSAGE, arg0->shortErrorBuffer, 0U, 0U, 0U, NULL);
    String_Delete(string);
}

void ov70_022451A8(GTSApplicationState *arg0, s32 arg1, s32 num) {
    s32 v0;

    v0 = arg1;
    if (v0 == -1) {
        v0 = 0xB;
    }
    BufferIntegerAsString(arg0->stringTemplate, 0U, num, 5U, PRINTING_MODE_LEADING_ZEROS, 1);
    ClearFrameAndWindow2(&arg0->bottomInstructionWindow, 1);
    ov70_0224512C(arg0, v0);
}
