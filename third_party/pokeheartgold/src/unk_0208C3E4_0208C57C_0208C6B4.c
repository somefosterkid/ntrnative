#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "unk_02088288.h"
#include "unk_0208C3E4.h"
#include "pokemon_summary_app.h"

void sub_0208C6B4(PokemonSummary *arg0) {
    String *string;
    u16 itemId;

    FillWindowPixelBuffer(&arg0->windows[31], 0U);
    itemId = arg0->mon.heldItem;
    if (itemId != 0) {
        BufferItemName(arg0->msgFormat, 0U, (u32)itemId);
        string = NewString_ReadMsgData(arg0->msgData, 5);
        StringExpandPlaceholders(arg0->msgFormat, arg0->buffer, string);
        String_Delete(string);
    } else {
        ReadMsgDataIntoString(arg0->msgData, 6, arg0->buffer);
    }
    sub_0208C778(arg0, &arg0->windows[31], 0x10200, 0);
    ScheduleWindowCopyToVram(&arg0->windows[6]);
    ScheduleWindowCopyToVram(&arg0->windows[31]);
}

s32 sub_0208C73C(PokemonSummary *arg0) {
    if ((arg0->mon.otId == arg0->args->OTID) && (arg0->mon.otGender == arg0->args->OTGender) && (String_Compare(arg0->mon.otName, arg0->playerName) == 0)) {
        return 1;
    }
    return 0;
}

void sub_0208C778(PokemonSummary *arg0, Window *window, u32 color, u32 arg3) {
    u32 v0;
    u32 x;
    u8 v1;
    u8 v2;

    x = arg3;
    switch (x) {
    case 0:
        x = 0;
        break;
    case 1:
        v1 = (u8)FontID_String_GetWidth(FONT_SYSTEM, arg0->buffer, 0U);
        x = (u32)(u8)((u8)(GetWindowWidth(window) * 8) - v1);
        break;
    case 2:
        v2 = (u8)FontID_String_GetWidth(FONT_SYSTEM, arg0->buffer, 0U);
        v0 = (u8)(GetWindowWidth(window) * 8) - v2;
        x = (u32)((v0 + (v0 >> 0x1F)) << 0x17) >> 0x18;
        break;
    }
    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, arg0->buffer, x, 0U, 0xFFU, color, NULL);
}
