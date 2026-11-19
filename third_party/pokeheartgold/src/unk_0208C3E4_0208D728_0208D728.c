#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "unk_0208C3E4.h"
#include "unk_02088288.h"
#include "pokemon_summary_app.h"

void sub_0208D728(PokemonSummary *param_1) {
    u8 cVar1;

    cVar1 = param_1->args->unk12;
    if (cVar1 != 3 && cVar1 != 4) {
        return;
    }
    FillWindowPixelBuffer(&param_1->pageWindows[1], 0);
    ReadMsgDataIntoString(param_1->msgData, 0xa5, param_1->buffer);
    sub_0208C778(param_1, &param_1->pageWindows[1], 0xe0f00, 0);
    ReadMsgDataIntoString(param_1->msgData, param_1->mon.favoriteFlavor + 0xa6, param_1->buffer);
    AddTextPrinterParameterizedWithColor(&param_1->pageWindows[1], FONT_SYSTEM, param_1->buffer, 0, 0x10, 0xff, 0x10200, (PrinterCallback_t)0x0);
    ScheduleWindowCopyToVram(&param_1->pageWindows[1]);
    return;
}
