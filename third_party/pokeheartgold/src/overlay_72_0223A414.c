#include "global.h"

#include "sys_task.h"
#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"
#include "overlay_72_wfc_connect.h"

void sub_0203A880(void);

void ov72_0223A414(void);

void ov72_0223A41C(void);
void ov72_0223A420(NintendoWfcConnect *arg0);
void ov72_0223A444(NintendoWfcConnect *arg0);
void ov72_0223A460(NintendoWfcConnect *arg0, s32 arg1);
void ov72_0223A4DC(NintendoWfcConnect *arg0, s32 arg1, s32 arg2);

void ov72_0223A414(void) {
    sub_0203A880();
}

void ov72_0223A41C(void) {
}

void ov72_0223A420(NintendoWfcConnect *arg0) {
    if (arg0->waitingIcon == NULL) {
        arg0->waitingIcon = WaitingIcon_New(&arg0->msgWindow, 1U);
    }
}

void ov72_0223A444(NintendoWfcConnect *arg0) {
    WaitingIcon *waitingIcon;

    waitingIcon = arg0->waitingIcon;
    if (waitingIcon != NULL) {
        sub_0200F450(waitingIcon);
        arg0->waitingIcon = NULL;
    }
}

void ov72_0223A460(NintendoWfcConnect *arg0, s32 strno) {
    String *string;

    string = String_New(0x100U, HEAP_ID_67);
    ReadMsgDataIntoString(arg0->msgDataError, strno, string);
    StringExpandPlaceholders(arg0->msgFormat, arg0->errorString, string);
    FillWindowPixelBuffer(&arg0->errorWindow, 0xFU);
    DrawFrameAndWindow1(&arg0->errorWindow, 1, 0x1FU, 0xBU);
    arg0->textPrinterId = (s32)AddTextPrinterParameterized(&arg0->errorWindow, FONT_MESSAGE, arg0->errorString, 0U, 0U, 0U, NULL);
    String_Delete(string);
}

void ov72_0223A4DC(NintendoWfcConnect *arg0, s32 arg1, s32 num) {
    s32 v0;

    v0 = arg1;
    if (v0 == -1) {
        v0 = 0xB;
    }
    BufferIntegerAsString(arg0->msgFormat, 0U, num, 5U, PRINTING_MODE_LEADING_ZEROS, 1);
    ClearFrameAndWindow2(&arg0->msgWindow, 1);
    ov72_0223A460(arg0, v0);
}
