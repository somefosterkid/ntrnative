#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_102_easy_chat_sentence_view.h"

s32 ov102_021E8F70(struct EasyChatApp *app);

void ov102_021EAE40(EasyChatSentenceView *view, u32 mode) {
    MessageFormat *messageFormat;
    String *dest;
    s32 strno;

    FillWindowPixelBuffer(view->textWindow, 9);
    switch (mode) {
    case 0:
        messageFormat = MessageFormat_New_Custom(2, 0x3c, HEAP_ID_DEFAULT);
        dest = String_New(300, HEAP_ID_DEFAULT);
        BufferPocketName(messageFormat, 0, 0);
        strno = ov102_021E8F70(view->app);
        ReadMsgDataIntoString(view->msgData, strno, view->string);
        StringExpandPlaceholders(messageFormat, dest, view->string);
        AddTextPrinterParameterizedWithColor(view->textWindow, FONT_MESSAGE, dest, 0, 0, 0xff, 0x10209, (PrinterCallback_t)0x0);
        String_Delete(dest);
        MessageFormat_Delete(messageFormat);
        break;
    case 2:
        ReadMsgDataIntoString(view->msgData, 6, view->string);
        AddTextPrinterParameterizedWithColor(view->textWindow, FONT_MESSAGE, view->string, 0, 0, 0xff, 0x10209, (PrinterCallback_t)0x0);
        break;
    case 1:
        ReadMsgDataIntoString(view->msgData, 7, view->string);
        AddTextPrinterParameterizedWithColor(view->textWindow, FONT_MESSAGE, view->string, 0, 0, 0xff, 0x10209, (PrinterCallback_t)0x0);
        break;
    case 3:
        ReadMsgDataIntoString(view->msgData, 8, view->string);
        AddTextPrinterParameterizedWithColor(view->textWindow, FONT_MESSAGE, view->string, 0, 0, 0xff, 0x10209, (PrinterCallback_t)0x0);
    }
    CopyWindowToVram(view->textWindow);
    return;
}

void ov102_021EAF44(EasyChatSentenceView *view) {
    ov102_021EB088(view, 0);
}
