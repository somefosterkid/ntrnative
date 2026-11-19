#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "unk_0208C3E4.h"
#include "pokemon_summary_app.h"

void sub_0208D178(PokemonSummary *param_1) {
    u8 bVar1;
    String *src;
    MsgData *msgData;

    sub_0208C7F8(param_1, 0x10, 0x6f, 0, 0);
    sub_0208C7F8(param_1, 0x11, 0x70, 1, 0);
    sub_0208C7F8(param_1, 0x12, 0x71, 3, 0);
    sub_0208C7F8(param_1, 0x13, 0x72, 4, 0);
    sub_0208C7F8(param_1, 0x14, 0x73, 2, 0);
    ScheduleWindowCopyToVram(&param_1->windows[2]);
    ScheduleWindowCopyToVram(&param_1->windows[15]);
    ScheduleWindowCopyToVram(&param_1->windows[16]);
    ScheduleWindowCopyToVram(&param_1->windows[17]);
    ScheduleWindowCopyToVram(&param_1->windows[18]);
    ScheduleWindowCopyToVram(&param_1->windows[19]);
    ScheduleWindowCopyToVram(&param_1->windows[20]);
    ScheduleWindowCopyToVram(&param_1->windows[21]);
    FillWindowPixelBuffer(param_1->pageWindows, 0);
    FillWindowPixelBuffer(&param_1->pageWindows[1], 0);
    FillWindowPixelBuffer(&param_1->pageWindows[2], 0);
    FillWindowPixelBuffer(&param_1->pageWindows[3], 0);
    FillWindowPixelBuffer(&param_1->pageWindows[4], 0);
    FillWindowPixelBuffer(&param_1->pageWindows[5], 0);
    FillWindowPixelBuffer(&param_1->pageWindows[6], 0);
    FillWindowPixelBuffer(&param_1->pageWindows[7], 0);
    bVar1 = GetWindowWidth(param_1->pageWindows);
    sub_0208C8C8(param_1, 0, 0x75, 0x77, 0x76, param_1->mon.hp, param_1->mon.maxHp, 3, (bVar1 & 0x3f) << 2, 0);
    sub_0208C87C(param_1, 0x78, param_1->mon.attack, 3, 0);
    sub_0208C778(param_1, &param_1->pageWindows[1], 0x10200, 1);
    sub_0208C87C(param_1, 0x79, param_1->mon.defense, 3, 0);
    sub_0208C778(param_1, &param_1->pageWindows[2], 0x10200, 1);
    sub_0208C87C(param_1, 0x7a, param_1->mon.spAttack, 3, 0);
    sub_0208C778(param_1, &param_1->pageWindows[3], 0x10200, 1);
    sub_0208C87C(param_1, 0x7b, param_1->mon.spDefense, 3, 0);
    sub_0208C778(param_1, &param_1->pageWindows[4], 0x10200, 1);
    sub_0208C87C(param_1, 0x7c, param_1->mon.speed, 3, 0);
    sub_0208C778(param_1, &param_1->pageWindows[5], 0x10200, 1);
    BufferAbilityName(param_1->msgFormat, 0, param_1->mon.ability);
    src = NewString_ReadMsgData(param_1->msgData, 0x7d);
    StringExpandPlaceholders(param_1->msgFormat, param_1->buffer, src);
    String_Delete(src);
    sub_0208C778(param_1, &param_1->pageWindows[6], 0x10200, 0);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x2d2, HEAP_ID_19);
    ReadMsgDataIntoString(msgData, param_1->mon.ability, param_1->buffer);
    DestroyMsgData(msgData);
    AddTextPrinterParameterizedWithColor(&param_1->pageWindows[7], FONT_SYSTEM, param_1->buffer, 3, 0, 0xff, 0x10200, (PrinterCallback_t)0x0);
    ScheduleWindowCopyToVram(param_1->pageWindows);
    ScheduleWindowCopyToVram(&param_1->pageWindows[1]);
    ScheduleWindowCopyToVram(&param_1->pageWindows[2]);
    ScheduleWindowCopyToVram(&param_1->pageWindows[3]);
    ScheduleWindowCopyToVram(&param_1->pageWindows[4]);
    ScheduleWindowCopyToVram(&param_1->pageWindows[5]);
    ScheduleWindowCopyToVram(&param_1->pageWindows[6]);
    ScheduleWindowCopyToVram(&param_1->pageWindows[7]);
    return;
}
