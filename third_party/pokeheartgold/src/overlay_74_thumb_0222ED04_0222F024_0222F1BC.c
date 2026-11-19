#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_74_wonder_cards_app2_data.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"

int ov74_0222F1BC(WonderCardsApp2Data *appData, Window *window, int windowTemplateIdx, int baseTile);

int ov74_0222F1BC(WonderCardsApp2Data *appData, Window *window, int windowTemplateIdx, int baseTile) {
    String *string;
    WonderCardsApp2WindowTemplate *windowTemplate = &ov74_0223C700[windowTemplateIdx];

    if (window->bgConfig == NULL) {
        AddWindowParameterized(appData->bgConfig, window, GF_BG_LYR_MAIN_2, windowTemplate->tilemapLeft, windowTemplate->tilemapTop, windowTemplate->width, windowTemplate->height, 15, baseTile);
    }
    FillWindowPixelBuffer(window, windowTemplate->bgColor);
    if (windowTemplate->entryID != 0) {
        appData->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xF7, HEAP_ID_85);
        appData->msgFormat = MessageFormat_New(HEAP_ID_85);
    }
    appData->messageBoxBaseTile = baseTile;
    appData->messageBoxTemplate = windowTemplate;
    windowTemplate->setup(appData, window, MAKE_TEXT_COLOR(1, 2, 0));
    if (windowTemplate->entryID != 0) {
        string = ReadMsgData_ExpandPlaceholders(appData->msgFormat, appData->msgData, windowTemplate->entryID, HEAP_ID_85);
        AddTextPrinterParameterizedWithColor(window, windowTemplate->font, string, 0, 0, TEXT_SPEED_NOTRANSFER, windowTemplate->textColor, NULL);
        String_Delete(string);
        DestroyMsgData(appData->msgData);
        MessageFormat_Delete(appData->msgFormat);
    }
    if (window == &appData->messageBox) {
        DrawFrameAndWindow2(window, FALSE, 0x13, 10);
    } else {
        DrawFrameAndWindow1(window, FALSE, 10, 14);
    }
    return baseTile + windowTemplate->width * windowTemplate->height;
}
