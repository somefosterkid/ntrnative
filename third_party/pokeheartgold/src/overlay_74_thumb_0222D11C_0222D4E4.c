#include "global.h"

#include "msgdata.h"
#include "mystery_gift.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_74_wonder_cards_app_data.h"

BOOL ov74_0222D4E4(WonderCardsAppData *appData, Window *window, u32 color);

BOOL ov74_0222D4E4(WonderCardsAppData *appData, Window *window, u32 color) {
    MysteryGiftSave *mg;
    String *string;
    s32 index;
    u32 msgno;

    index = appData->selectedWondercardSlot;
    if (appData->wonderCards[index]->unk104.unk8E_3 == 0) {
        msgno = 0x28;
    } else {
        mg = appData->mysteryGift;
        if ((mg == NULL) || (SaveMysteryGift_HasAnyGift(mg, index) == 1)) {
            msgno = 0x26;
        } else {
            msgno = 0x27;
        }
    }
    string = ReadMsgData_ExpandPlaceholders(appData->strTemplate, appData->msgLoader, msgno, appData->heapID);
    AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, string, 0U, 0U, 0xFFU, color, NULL);
    String_Delete(string);
    return 1;
}
