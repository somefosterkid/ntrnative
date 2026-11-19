#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "unk_0208C3E4.h"
#include "pokemon_summary_app.h"

void sub_0208C614(PokemonSummary *summary) {
    Window *window = &summary->windows[32];
    FillWindowPixelBuffer(window, 0);

    String *buf;
    if (summary->mon.isEgg == 0) {
        sub_0200CDAC(summary->numberPrinter, 1, window, 0, 5);

        buf = NewString_ReadMsgData(summary->msgData, 3);

        BufferIntegerAsString(summary->msgFormat, 0, summary->mon.level, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
        StringExpandPlaceholders(summary->msgFormat, summary->buffer, buf);
        String_Delete(buf);
        AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, summary->buffer, 16, 0, 0xFF, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    }

    ScheduleWindowCopyToVram(window);
}
