#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "igt.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"
#include "trainer_case.h"

enum Badge {
    BADGE_ID_COAL = 0,
    BADGE_ID_FOREST = 1,
    BADGE_ID_COBBLE = 2,
    BADGE_ID_FEN = 3,
    BADGE_ID_RELIC = 4,
    BADGE_ID_MINE = 5,
    BADGE_ID_ICICLE = 6,
    BADGE_ID_BEACON = 7,
    MAX_BADGES = 8,
};

void ov51_021E757C(Window *windows, TrainerCase *trainerCase, String *unused);

void ov51_021E757C(Window *windows, TrainerCase *trainerCase, String *unused) {
    u8 bVar1;
    u16 uVar2;
    MsgData *msgData;
    String *dest;
    String *dest_00;
    MessageFormat *messageFormat;
    if (trainerCase->playTime == 0L) {
        GF_AssertFail();
    }
    GetIGTHours((IGT *)trainerCase->playTime);
    FillWindowPixelRect(windows + 5, 0, 0xb8, 0, 0x28, 0x10);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x2d7, HEAP_ID_25);
    dest = String_New(0x20, HEAP_ID_25);
    dest_00 = String_New(0x20, HEAP_ID_25);
    messageFormat = MessageFormat_New_Custom(2, 0x20, HEAP_ID_25);
    uVar2 = GetIGTHours((IGT *)trainerCase->playTime);
    BufferIntegerAsString(messageFormat, 0, (u32)uVar2, 3, PRINTING_MODE_RIGHT_ALIGN, 1);
    bVar1 = GetIGTMinutes((IGT *)trainerCase->playTime);
    BufferIntegerAsString(messageFormat, 1, (u32)bVar1, 2, PRINTING_MODE_LEADING_ZEROS, 1);
    ReadMsgDataIntoString(msgData, 0x15, dest_00);
    StringExpandPlaceholders(messageFormat, dest, dest_00);
    u32 uVar3;
    uVar3 = 0xe0 - FontID_String_GetWidth(FONT_SYSTEM, dest, 0);
    AddTextPrinterParameterizedWithColor(windows + 5, FONT_SYSTEM, dest, uVar3, 0, 0, 0x10200, (PrinterCallback_t)0x0);
    DestroyMsgData(msgData);
    String_Delete(dest);
    String_Delete(dest_00);
    MessageFormat_Delete(messageFormat);
    return;
}
