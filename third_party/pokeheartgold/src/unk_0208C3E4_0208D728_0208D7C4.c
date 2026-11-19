#include "global.h"
#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "ribbon.h"
#include "pokemon_summary_app.h"
#include "unk_0208C3E4.h"
#include "unk_02088288.h"

void sub_0208D7C4(PokemonSummary *arg0) {
    FillWindowPixelBuffer(&arg0->pageWindows[1], 0U);
    FillWindowPixelBuffer(&arg0->pageWindows[2], 0U);
    if ((s32)(arg0->ribbonCursor + (arg0->ribbonRow * 9)) < (s32) arg0->ribbonCount) {
        ReadMsgDataIntoString(arg0->ribbonMsgData, (s32)GetRibbonAttr(arg0->ribbonId, RIBBONDAT_NAMEGMM), arg0->buffer);
        sub_0208C778(arg0, &arg0->pageWindows[1], 0xE0F00, 0);
        ReadMsgDataIntoString(arg0->ribbonMsgData, GetRibbonDescGmm(arg0->args->ribbons->ribbons, arg0->ribbonId), arg0->buffer);
        sub_0208C778(arg0, &arg0->pageWindows[2], 0x10200, 0);
    }
    ScheduleWindowCopyToVram(&arg0->pageWindows[1]);
    ScheduleWindowCopyToVram(&arg0->pageWindows[2]);
}
