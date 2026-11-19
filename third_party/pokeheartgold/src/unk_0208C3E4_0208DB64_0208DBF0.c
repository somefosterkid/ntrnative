#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "unk_0208C3E4.h"
#include "pokemon_summary_app.h"

void sub_0208DBF0(PokemonSummary *param_1) {
    Window *window;
    if ((param_1->page) == 1) {
        ClearWindowTilemapAndScheduleTransfer(&param_1->pageWindows[13]);
        ClearWindowTilemapAndScheduleTransfer(&param_1->pageWindows[14]);
        ClearWindowTilemapAndScheduleTransfer(&param_1->pageWindows[15]);
        window = &param_1->pageWindows[15];
    } else {
        window = &param_1->pageWindows[5];
    }
    FillWindowPixelBuffer(window, 0);
    ReadMsgDataIntoString(param_1->msgData, 0x9c, param_1->buffer);
    sub_0208C778(param_1, window, 0x10200, 0);
    ScheduleWindowCopyToVram(window);
    return;
}
