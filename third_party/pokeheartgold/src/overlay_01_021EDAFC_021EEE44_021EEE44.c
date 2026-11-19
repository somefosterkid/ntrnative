#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "field_system.h"
#include "field_types_def.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "save_pokegear.h"
#include "string_util.h"
#include "text.h"

void ov01_021EEE44(FieldSystem *fieldSystem, Window *window);

void ov01_021EEE44(FieldSystem *fieldSystem, Window *window) {
    MsgData *new_var2;
    String *sp10;
    MsgData *spC;
    long long new_var;
    unsigned short new_var4;
    MessageFormat *temp_r6;
    MsgData *temp_r0;
    String *new_var3;
    String *temp_r0_2;
    String *temp_r4;
    FillWindowPixelRect(window, 0xFU, 0U, 0x80U, 0x80U, 0x10U);
    FillWindowPixelRect(window, 0xFU, 0U, 0x180U, 0x80U, 0x10U);
    temp_r0 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xBF, HEAP_ID_FIELD1);
    spC = temp_r0;
    temp_r6 = MessageFormat_New(HEAP_ID_FIELD1);
    temp_r4 = String_New(0x10U, HEAP_ID_FIELD1);
    temp_r0_2 = NewString_ReadMsgData(temp_r0, 0x14C);
    sp10 = temp_r0_2;
    BufferIntegerAsString(temp_r6, 0U, (s32)PhoneCallPersistentState_MomSavings_BalanceAction(SaveData_GetPhoneCallPersistentState(fieldSystem->saveData), 0, 0U), 6U, PRINTING_MODE_LEFT_ALIGN, 1);
    StringExpandPlaceholders(temp_r6, temp_r4, sp10);
    new_var4 = 0x80;
    new_var = new_var4 - FontID_String_GetWidth(FONT_SYSTEM, temp_r4, 0U);
    AddTextPrinterParameterized(window, FONT_SYSTEM, temp_r4, new_var, 0x10U, 0xFFU, 0L);
    BufferIntegerAsString(temp_r6, 0U, (s32)PlayerProfile_GetMoney(Save_PlayerData_GetProfile(fieldSystem->saveData)), 6U, PRINTING_MODE_LEFT_ALIGN, 1);
    StringExpandPlaceholders(temp_r6, temp_r4, sp10);
    new_var3 = temp_r4;
    new_var = 0x80 - FontID_String_GetWidth(FONT_SYSTEM, temp_r4, 0U);
    AddTextPrinterParameterized(window, FONT_SYSTEM, new_var3, new_var, 0x30U, 0xFFU, 0L);
    String_Delete(temp_r0_2);
    String_Delete(temp_r4);
    MessageFormat_Delete(temp_r6);
    new_var2 = temp_r0;
    DestroyMsgData(new_var2);
    ScheduleWindowCopyToVram(window);
}
