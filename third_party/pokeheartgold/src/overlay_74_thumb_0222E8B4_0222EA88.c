#include "global.h"
struct WonderCardsAppData;

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "unk_0203A3B0.h"
#include "overlay_74_wonder_cards_app_data.h"

void ov74_0222EA88(WonderCardsAppData *appData, Window *window, s32 count);

void ov74_0222EA88(WonderCardsAppData *appData, Window *window, s32 count) {
    String *string;

    appData->connectedPlayersCount = count;
    appData->msgLoader = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xF7, HEAP_ID_85);
    appData->strTemplate = MessageFormat_New(HEAP_ID_85);
    FillWindowPixelBuffer(window, 0U);
    BufferIntegerAsString(appData->strTemplate, 0U, appData->connectedPlayersCount, 1U, PRINTING_MODE_RIGHT_ALIGN, 1);
    string = ReadMsgData_ExpandPlaceholders(appData->strTemplate, appData->msgLoader, 0x39U, HEAP_ID_85);
    AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, string, 0U, 0U, 0xFFU, 0x10200U, NULL);
    CopyWindowToVram(window);
    String_Delete(string);
    DestroyMsgData(appData->msgLoader);
    MessageFormat_Delete(appData->strTemplate);
}

s32 ov74_02229DF8(void);

void ov74_0222EB28(WonderCardsAppData *appData, int *state, enum WonderCardsAppState nextState);

void ov74_0222EB28(WonderCardsAppData *appData, int *state, enum WonderCardsAppState nextState) {
    ov74_02229DF8();
    sub_0203A914();
    appData->queuedState = nextState;
    *state = WC_APP_STATE_WAIT_FOR_COMM_MAN_EXIT;
}
