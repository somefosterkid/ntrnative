#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "unk_0208C3E4.h"
#include "pokemon_summary_app.h"

void sub_0208C57C(PokemonSummary *summary) {
    Window *window = &summary->windows[33];

    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, summary->mon.nickname, 0, 0, 0xFF, MAKE_TEXT_COLOR(14, 15, 0), NULL);

    if (summary->mon.noPrintGender == 0) {
        if (summary->mon.gender == 0) {
            ReadMsgDataIntoString(summary->msgData, 1, summary->buffer);
            sub_0208C778(summary, window, MAKE_TEXT_COLOR(3, 4, 0), SUMMARY_ALIGN_RIGHT);
        } else if (summary->mon.gender == 1) {
            ReadMsgDataIntoString(summary->msgData, 2, summary->buffer);
            sub_0208C778(summary, window, MAKE_TEXT_COLOR(5, 6, 0), SUMMARY_ALIGN_RIGHT);
        }
    }

    ScheduleWindowCopyToVram(window);
}
