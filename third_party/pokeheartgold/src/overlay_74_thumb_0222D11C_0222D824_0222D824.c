#include "global.h"
#include "overlay_74_wonder_cards_app_data.h"
#include "constants/heap.h"
#include "heap.h"
#include "overlay_manager.h"
#include "sprite_resources_header.h"
#include "text.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0247.h"
#include "filesystem_files_def.h"
#include "gf_gfx_planes.h"

extern WonderCardsAppWindowTemplate ov74_0223C340[NUM_WC_APP_WINDOWS];
void ov74_0222D824(WonderCardsAppData *appData, BOOL unused, enum WonderCardsAppScreen screen);
void ov74_0222DB70(WonderCardsAppData *appData, enum WonderCardsAppScreen screen);
void ov74_0222DCD4(WonderCardsAppData *appData);
void ov74_0222D824(WonderCardsAppData *appData, BOOL unused, enum WonderCardsAppScreen screen) {
    int i;
    int baseTile;
    WonderCardsAppWindowTemplate *windowTemplates;
    windowTemplates = ov74_0223C340;
    appData->msgLoader = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, msg_0247, appData->heapID);
    appData->strTemplate = MessageFormat_New(appData->heapID);
    appData->currentScreen = screen;
    ov74_0222DB70(appData, screen);
    for (i = 0; i < NELEMS(ov74_0223C340); i++) {
        if ((windowTemplates[i].screen != screen) && (windowTemplates[i].screen != WC_MESSAGE_BOX)) {
            if (appData->windows[i].bgConfig) {
                ClearWindowTilemapAndCopyToVram(&appData->windows[i]);
                RemoveWindow(&appData->windows[i]);
            }
        }
    }

    baseTile = ((1 + (3 * 3)) + (3 * 3)) + ((6 * 3) + ((2 * 2) * 3));
    for (i = 0; i < NELEMS(ov74_0223C340); i++) {
        if (windowTemplates[i].screen == screen) {
            if (appData->windows[i].bgConfig == NULL) {
                appData->windowBasetiles[i] = baseTile;
                AddWindowParameterized(appData->bgConfig, &appData->windows[i], GF_BG_LYR_MAIN_0, windowTemplates[i].tilemapLeft, windowTemplates[i].tilemapTop, windowTemplates[i].width, windowTemplates[i].height, 15, baseTile);
            }
            FillWindowPixelBuffer(&appData->windows[i], GetFontAttribute(windowTemplates[i].font, windowTemplates[i].bgColor.fontAttribute));
            u32 color = windowTemplates[i].textColor;
            if (windowTemplates[i].setup(appData, &appData->windows[i], color) == 1) {
                if (windowTemplates[i].entryID) {
                    String *string = ReadMsgData_ExpandPlaceholders(appData->strTemplate, appData->msgLoader, windowTemplates[i].entryID, appData->heapID);
                    {
                        u32 yOffset = (windowTemplates[i].leftMarginSize == (-1)) ? (FontID_String_GetCenterAlignmentX(windowTemplates[i].font, string, 0, windowTemplates[i].width * 8)) : (windowTemplates[i].leftMarginSize);
                        AddTextPrinterParameterizedWithColor(&appData->windows[i], windowTemplates[i].font, string, yOffset, windowTemplates[i].topMarginSize, TEXT_SPEED_NOTRANSFER, color, NULL);
                    }
                    String_Delete(string);
                }
            }
            CopyWindowToVram(&appData->windows[i]);
            baseTile += windowTemplates[i].width * windowTemplates[i].height;
        }
    }

    DestroyMsgData(appData->msgLoader);
    MessageFormat_Delete(appData->strTemplate);
    if (screen == WC_SCREEN_WONDERCARD_FRONT) {
        ov74_0222DCD4(appData);
    }
}
int ov74_0222D9E0(WonderCardsAppData *appData, Window *window, int windowTemplateIdx, int baseTile);
int ov74_0222D9E0(WonderCardsAppData *appData, Window *window, int windowTemplateIdx, int baseTile) {
    WonderCardsAppWindowTemplate *windowTemplate = &ov74_0223C340[windowTemplateIdx];
    if (NULL == window->bgConfig) {
        AddWindowParameterized(appData->bgConfig, window, GF_BG_LYR_MAIN_2, windowTemplate->tilemapLeft, windowTemplate->tilemapTop, windowTemplate->width, windowTemplate->height, 15, baseTile);
    }
    FillWindowPixelBuffer(window, windowTemplate->bgColor.colorIndex);
    if (windowTemplate->entryID) {
        appData->msgLoader = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, msg_0247, HEAP_ID_85);
        appData->strTemplate = MessageFormat_New(HEAP_ID_85);
    }
    appData->messageBoxBaseTile = baseTile;
    appData->messageBoxTemplate = windowTemplate;
    windowTemplate->setup(appData, window, MAKE_TEXT_COLOR(1, 2, 0));
    if (windowTemplate->entryID) {
        String *string = ReadMsgData_ExpandPlaceholders(appData->strTemplate, appData->msgLoader, windowTemplate->entryID, HEAP_ID_85);
        AddTextPrinterParameterizedWithColor(window, windowTemplate->font, string, 0, 0, TEXT_SPEED_NOTRANSFER, windowTemplate->textColor, NULL);
        String_Delete(string);
        DestroyMsgData(appData->msgLoader);
        MessageFormat_Delete(appData->strTemplate);
    }
    if (window == (&appData->messageBox)) {
        DrawFrameAndWindow2(window, 0, (1 + (3 * 3)) + (3 * 3), 10);
    } else {
        DrawFrameAndWindow1(window, 0, (3 * 3) + 1, 14);
    }
    return baseTile + (windowTemplate->width * windowTemplate->height);
}
