#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_74_mystery_gift_app_data.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"

void ov74_0222AA18(OverlayManager *arg0, Window *arg1, u32 arg2);

void ov74_0222AA18(OverlayManager *man, Window *window, u32 msgno) {
    String *string;
    MysteryGiftAppData *v0;

    v0 = OverlayManager_GetData(man);
    v0->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xF7, HEAP_ID_84);
    v0->msgFmt = MessageFormat_New(HEAP_ID_84);
    FillWindowPixelBuffer(window, GetFontAttribute(1U, 6));
    string = v0->string;
    if (string == NULL) {
        string = ReadMsgData_ExpandPlaceholders(v0->msgFmt, v0->msgData, msgno, HEAP_ID_84);
    }
    v0->textPrinterId = (s32)AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, string, 0U, 0U, v0->msgBoxPrinterDelay, 0x1020FU, NULL);
    if (v0->string == NULL) {
        String_Delete(string);
    }
    DrawFrameAndWindow2(window, 0, 1U, 2U);
    DestroyMsgData(v0->msgData);
    MessageFormat_Delete(v0->msgFmt);
    v0->msgBoxPrinterDelay = 0xFFU;
}

void ov74_0222AAAC(OverlayManager *arg0, Window *arg1, u16 *arg2);

void ov74_0222AAAC(OverlayManager *man, Window *window, u16 *src) {
    String *string;

    OverlayManager_GetData(man);
    string = String_New(0x25U, HEAP_ID_84);
    CopyU16ArrayToStringN(string, src, 0x24U);
    FillWindowPixelBuffer(window, GetFontAttribute(0U, 6));
    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, 0U, 0U, 0xFFU, 0x1020FU, NULL);
    DrawFrameAndWindow1(window, 0, 0x1FU, 3U);
    String_Delete(string);
}
