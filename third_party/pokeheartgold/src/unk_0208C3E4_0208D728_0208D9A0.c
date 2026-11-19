#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "move.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon_summary_app.h"
#include "unk_0208C3E4.h"

void sub_0208D9A0(PokemonSummary *arg0, u16 arg1) {
    MsgData *temp_r0_3;
    u32 temp_r0;
    u32 temp_r0_2;

    ScheduleWindowCopyToVram(&arg0->windows[24]);
    ScheduleWindowCopyToVram(&arg0->windows[25]);
    ScheduleWindowCopyToVram(&arg0->windows[26]);
    FillWindowPixelBuffer(&arg0->pageWindows[13], 0U);
    FillWindowPixelBuffer(&arg0->pageWindows[14], 0U);
    FillWindowPixelBuffer(&arg0->pageWindows[15], 0U);
    temp_r0 = GetMoveAttr(arg1, MOVEATTR_POWER);
    if (temp_r0 <= 1U) {
        ReadMsgDataIntoString(arg0->msgData, 0x9A, arg0->buffer);
    } else {
        sub_0208C87C(arg0, 0x96, temp_r0, 3, 0);
    }
    sub_0208C778(arg0, &arg0->pageWindows[13], 0x10200, 1);
    temp_r0_2 = GetMoveAttr(arg1, MOVEATTR_ACCURACY);
    if (temp_r0_2 == 0) {
        ReadMsgDataIntoString(arg0->msgData, 0x9A, arg0->buffer);
    } else {
        sub_0208C87C(arg0, 0x97, temp_r0_2, 3, 0);
    }
    sub_0208C778(arg0, &arg0->pageWindows[14], 0x10200, 1);
    temp_r0_3 = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x2ED, HEAP_ID_19);
    ReadMsgDataIntoString(temp_r0_3, (s32)arg1, arg0->buffer);
    sub_0208C778(arg0, &arg0->pageWindows[15], 0x10200, 0);
    DestroyMsgData(temp_r0_3);
    ScheduleWindowCopyToVram(&arg0->pageWindows[13]);
    ScheduleWindowCopyToVram(&arg0->pageWindows[14]);
    ScheduleWindowCopyToVram(&arg0->pageWindows[15]);
}
