#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"
#include "unk_0208C3E4.h"
#include "pokemon_summary_app.h"

void sub_0208DDA0(PokemonSummary *summary, enum PokemonSummaryPoffinMsg msg) {
    u32 entryID;

    switch (msg) {
    case SUMMARY_POFFIN_MSG_COOLNESS_ENHANCED:
        entryID = 172;
        break;
    case SUMMARY_POFFIN_MSG_BEAUTY_ENHANCED:
        entryID = 174;
        break;
    case SUMMARY_POFFIN_MSG_CUTENESS_ENHANCED:
        entryID = 175;
        break;
    case SUMMARY_POFFIN_MSG_SMARTNESS_ENHANCED:
        entryID = 176;
        break;
    case SUMMARY_POFFIN_MSG_TOUGHNESS_ENHANCED:
        entryID = 173;
        break;
    case SUMMARY_POFFIN_MSG_NOTHING_CHANGED:
        entryID = 177;
        break;
    default:
        entryID = 178;
    }

    Window *window = &summary->pageWindows[2];

    DrawFrameAndWindow2(window, 1, (1024 - (18 + 12)), 13);
    FillWindowPixelBuffer(window, 15);
    ReadMsgDataIntoString(summary->msgData, entryID, summary->buffer);
    AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, summary->buffer, 0, 0, 0xFF, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    ScheduleWindowCopyToVram(window);
}
