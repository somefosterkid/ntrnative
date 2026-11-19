#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon_save.h"

#include "bg_window.h"
#include "coins.h"
#include "field_system.h"
#include "field_types_def.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "frontier_data.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"

void ov01_021EEC7C(FieldSystem *fieldSystem, Window *window, u8 type);

void ov01_021EEC7C(FieldSystem *fieldSystem, Window *window, u8 type) {
    String *src;
    MsgData *msgData;
    MessageFormat *messageFormat;
    String *dest;
    u16 *coins;
    FrontierData *frontierData;
    PokeathlonSave *pokeathlonSave;
    u32 uVar2;
    FillWindowPixelBuffer(window, 0xf);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xbf, HEAP_ID_FIELD1);
    messageFormat = MessageFormat_New(HEAP_ID_FIELD1);
    dest = String_New(0x10, HEAP_ID_FIELD1);
    switch (type) {
    case 0:
        src = NewString_ReadMsgData(msgData, 0xc1);
        coins = Save_PlayerData_GetCoinsAddr(fieldSystem->saveData);
        uVar2 = Coins_GetValue(coins);
        break;

    case 1:
        src = NewString_ReadMsgData(msgData, 0xdc);
        frontierData = Save_FrontierData_Get(fieldSystem->saveData);
        uVar2 = FrontierData_BattlePointAction(frontierData, 0, 0);
        break;

    case 2:

    default:
        src = NewString_ReadMsgData(msgData, 0xdf);
        pokeathlonSave = Save_Pokeathlon_Get(fieldSystem->saveData);
        uVar2 = PokeathlonSave_GetAthletePoints(pokeathlonSave);
        break;
    }

    BufferIntegerAsString(messageFormat, 0, uVar2, (u32)5, PRINTING_MODE_RIGHT_ALIGN, 1);
    StringExpandPlaceholders(messageFormat, dest, src);
    uVar2 = FontID_String_GetWidth(FONT_SYSTEM, dest, 0);
    uVar2 = 0x50 - uVar2;
    AddTextPrinterParameterized(window, FONT_SYSTEM, dest, uVar2, 0, 0xff, (PrinterCallback_t)0x0);
    String_Delete(src);
    String_Delete(dest);
    MessageFormat_Delete(messageFormat);
    DestroyMsgData(msgData);
    ScheduleWindowCopyToVram(window);
    return;
}
