#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_74_wonder_cards_app2_data.h"
#include "pm_string.h"
#include "text.h"

void ov74_0222F314(WonderCardsApp2Data *appData, int screen);

void ov74_0222F478(WonderCardsApp2Data *appData);

void ov74_0222F024(WonderCardsApp2Data *appData, BOOL unused, int screen);

void ov74_0222F024(WonderCardsApp2Data *appData, BOOL unused, int screen) {
    u32 i;
    int baseTile;
    u32 color;
    String *string;
    WonderCardsApp2WindowTemplate *windowTemplates = ov74_0223C700;

    appData->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xF7, appData->heapID);
    appData->msgFormat = MessageFormat_New(appData->heapID);
    appData->currentScreen = screen;
    ov74_0222F314(appData, screen);
    for (i = 0; i < 11; i++) {
        if (windowTemplates[i].screen != screen && windowTemplates[i].screen != 2) {
            if (appData->windows[i].bgConfig != NULL) {
                ClearWindowTilemapAndCopyToVram(&appData->windows[i]);
                RemoveWindow(&appData->windows[i]);
            }
        }
    }
    baseTile = 49;
    for (i = 0; i < 11; i++) {
        if (windowTemplates[i].screen == screen) {
            if (appData->windows[i].bgConfig == NULL) {
                appData->windowBasetiles[i] = baseTile;
                AddWindowParameterized(appData->bgConfig, &appData->windows[i], GF_BG_LYR_MAIN_0, windowTemplates[i].tilemapLeft, windowTemplates[i].tilemapTop, windowTemplates[i].width, windowTemplates[i].height, 15, baseTile);
            }
            FillWindowPixelBuffer(&appData->windows[i], GetFontAttribute(windowTemplates[i].font, (u8)windowTemplates[i].bgColor));
            color = windowTemplates[i].textColor;
            if (windowTemplates[i].setup(appData, &appData->windows[i], color) == TRUE && windowTemplates[i].entryID != 0) {
                string = ReadMsgData_ExpandPlaceholders(appData->msgFormat, appData->msgData, windowTemplates[i].entryID, appData->heapID);
                AddTextPrinterParameterizedWithColor(&appData->windows[i], windowTemplates[i].font, string, windowTemplates[i].leftMarginSize, windowTemplates[i].topMarginSize, TEXT_SPEED_NOTRANSFER, color, NULL);
                String_Delete(string);
            }
            CopyWindowToVram(&appData->windows[i]);
            baseTile += windowTemplates[i].width * windowTemplates[i].height;
        }
    }
    DestroyMsgData(appData->msgData);
    MessageFormat_Delete(appData->msgFormat);
    if (screen == 0) {
        ov74_0222F478(appData);
    }
}
