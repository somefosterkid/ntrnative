#include "global.h"

#include "constants/heap.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0247.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "message_format.h"
#include "msgdata.h"
#include "mystery_gift.h"
#include "overlay_74_wonder_cards_app_data.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "text.h"

void ov74_0222D1D4(Window *window, int textEntryID);
void ov74_0222D248(BgConfig *bgConfig);
void ov74_0222D448(WonderCardsAppData *appData);
int ov74_0222D9E0(WonderCardsAppData *appData, Window *window, int windowTemplateIdx, int baseTile);
enum WonderCardsAppState ov74_0222D2D4(OverlayManager *man);
enum WonderCardsAppState ov74_0222D308(OverlayManager *man);

void ov74_0222D1D4(Window *window, int textEntryID) {
    MessageFormat *messageFormat;
    MsgData *msgData;
    String *string;

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, msg_0247, HEAP_ID_85);
    messageFormat = MessageFormat_New(HEAP_ID_85);
    FillWindowPixelBuffer(window, GetFontAttribute(FONT_MESSAGE, 6));
    string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, textEntryID, HEAP_ID_85);
    AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    CopyWindowToVram(window);
    String_Delete(string);
    DestroyMsgData(msgData);
    MessageFormat_Delete(messageFormat);
}

void ov74_0222D248(BgConfig *bgConfig) {
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 0xF, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, 0x20, HEAP_ID_85);
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 0x14, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, 0x20, HEAP_ID_85);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 0x15, bgConfig, GF_BG_LYR_MAIN_1, 0, 0xC00, TRUE, HEAP_ID_85);
    GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 0x16, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, HEAP_ID_85);
    BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0x20, 0x18, 0xC);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_MAIN_1);
}

enum WonderCardsAppState ov74_0222D2D4(OverlayManager *man) {
    WonderCardsAppData *appData = OverlayManager_GetData(man);

    ov74_0222D448(appData);
    ov74_0222D1D4(&appData->messageBox, msg_0247_00047);
    ov74_0222D9E0(appData, &appData->standardWindow, MSG_BOX_CONFIRM_WC_SHARE, appData->nextWindowBasetile);
    return WC_APP_STATE_WAIT_CONFIRM_START_WIRELESS;
}

enum WonderCardsAppState ov74_0222D308(OverlayManager *man) {
    WonderCardsAppData *appData = OverlayManager_GetData(man);

    ov74_0222D448(appData);
    if (SaveMysteryGift_HasAnyGift(appData->mysteryGift, appData->selectedWondercardSlot) == TRUE) {
        ov74_0222D1D4(&appData->messageBox, msg_0247_00063);
    } else {
        ov74_0222D1D4(&appData->messageBox, msg_0247_00062);
    }
    ov74_0222D9E0(appData, &appData->standardWindow, MSG_BOX_CONFIRM_WC_DELETE, appData->nextWindowBasetile);
    return WC_APP_STATE_WAIT_CONFIRM_DELETION;
}
