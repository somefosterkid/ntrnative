#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"
#include "sprite_system.h"
#include "text.h"
#include "bg_window.h"
#include "overlay_68_move_relearner.h"

void ov68_021E6234(MoveRelearner *controller, u32 windowID, u32 fontID, u32 color, u32 align, u8 yOffset);

void ov68_021E7028(MoveRelearner *controller, u16 move, u16 index);

void ov68_021E6320(MoveRelearner *arg0);

void ov68_021E6320(MoveRelearner *arg0) {
    String *tmpStr;
    MsgData *moveMsgData;
    BoxPokemon *boxMon;
    u16 move;
    u32 i;

    ReadMsgDataIntoString(arg0->msgData, 0x24, arg0->string);
    ov68_021E6234(arg0, 7, 0, 0x10200, 0, 4);
    ScheduleWindowCopyToVram(&arg0->windows[7]);
    ReadMsgDataIntoString(arg0->msgData, 0x23, arg0->string);
    ov68_021E6234(arg0, 8, 4, 0xF0E00, 2, 4);
    ReadMsgDataIntoString(arg0->msgData, 0x22, arg0->string);
    ov68_021E6234(arg0, 9, 4, 0xF0E00, 2, 4);
    ScheduleWindowCopyToVram(&arg0->windows[9]);
    ReadMsgDataIntoString(arg0->msgData, 0x16, arg0->string);
    ov68_021E6234(arg0, 0, 0, 0xF0E00, 0, 0);
    ScheduleWindowCopyToVram(&arg0->windows[0]);
    ReadMsgDataIntoString(arg0->msgData, 0x17, arg0->string);
    ov68_021E6234(arg0, 1, 0, 0xF0E00, 0, 0);
    ScheduleWindowCopyToVram(&arg0->windows[1]);
    ReadMsgDataIntoString(arg0->msgData, 0x18, arg0->string);
    ov68_021E6234(arg0, 2, 0, 0xF0E00, 0, 0);
    ScheduleWindowCopyToVram(&arg0->windows[2]);
    tmpStr = String_New(0x100U, HEAP_ID_66);
    ReadMsgDataIntoString(arg0->msgData, 0x27, tmpStr);
    BufferBoxMonNickname(arg0->msgFormat, 0U, Mon_GetBoxMon(arg0->args->mon));
    StringExpandPlaceholders(arg0->msgFormat, arg0->string, tmpStr);
    ov68_021E6234(arg0, 0xC, 0, 0x10200, 0, 4);
    ScheduleWindowCopyToVram(&arg0->windows[12]);
    moveMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x2EE, HEAP_ID_66);
    boxMon = Mon_GetBoxMon(arg0->args->mon);
    for (i = 0; i < 4; i++) {
        move = GetBoxMonData(boxMon, i + MON_DATA_MOVE1, NULL);
        if (move == 0) {
            ManagedSprite_SetDrawFlag(arg0->sprites[8 + i], 0);
        } else {
            ManagedSprite_SetDrawFlag(arg0->sprites[8 + i], 1);
            ov68_021E7028(arg0, move, i + 4);
            ReadMsgDataIntoString(moveMsgData, move, arg0->string);
            ov68_021E6234(arg0, 0xD, 0, 0xF0E00, 0, i * 32);
            AddTextPrinterParameterizedWithColor(&arg0->windows[13], FONT_SYSTEM, arg0->ppLabel, 0x10U, i * 32 + 16, 0xFFU, 0x10200U, NULL);
            BufferIntegerAsString(arg0->msgFormat, 0U, (s32)GetBoxMonData(boxMon, i + MON_DATA_MOVE1_PP, NULL), 2U, PRINTING_MODE_RIGHT_ALIGN, 1);
            BufferIntegerAsString(arg0->msgFormat, 1U, (s32)GetBoxMonData(boxMon, i + MON_DATA_MOVE1_MAX_PP, NULL), 2U, PRINTING_MODE_LEFT_ALIGN, 1);
            StringExpandPlaceholders(arg0->msgFormat, arg0->string, arg0->ppFormat);
            AddTextPrinterParameterizedWithColor(&arg0->windows[13], FONT_SYSTEM, arg0->string, 0x2DU, i * 32 + 16, 0xFFU, 0x10200U, NULL);
        }
    }
    ScheduleWindowCopyToVram(&arg0->windows[13]);
    ReadMsgDataIntoString(arg0->msgData, 0x25, tmpStr);
    BufferBoxMonSpeciesName(arg0->msgFormat, 0U, boxMon);
    StringExpandPlaceholders(arg0->msgFormat, arg0->string, tmpStr);
    AddTextPrinterParameterizedWithColor(&arg0->windows[14], FONT_SYSTEM, arg0->string, 0U, 0U, 0xFFU, 0x10200U, NULL);
    ReadMsgDataIntoString(arg0->msgData, 0x26, tmpStr);
    BufferIntegerAsString(arg0->msgFormat, 0U, (s32)GetBoxMonData(boxMon, MON_DATA_LEVEL, NULL), 3U, PRINTING_MODE_RIGHT_ALIGN, 1);
    StringExpandPlaceholders(arg0->msgFormat, arg0->string, tmpStr);
    ov68_021E6234(arg0, 0xE, 0, 0x10200, 1, 0x10);
    ScheduleWindowCopyToVram(&arg0->windows[14]);
    DestroyMsgData(moveMsgData);
    String_Delete(tmpStr);
}
