#include "global.h"

#define POKEMON_SUMMARY_DEFINES_C8C8

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "unk_0208C3E4.h"
#include "pokemon_summary_app.h"

// the callers (DS) were compiled against a wider prototype and the code takes no result: DS = void with u8 parameters;
// PC = the header's prototype (u8 parameters) and the result of the last call
#if defined(SDK_PORT) || defined(NONMATCHING)
#define SUMMARY_C8C8_TYPE u32
#define SUMMARY_C8C8_RETURN return
#else
#define SUMMARY_C8C8_TYPE void
#define SUMMARY_C8C8_RETURN
SUMMARY_C8C8_TYPE sub_0208C8C8(PokemonSummary *summary, u32 moveIndex, u32 slashEntryID, u32 curEntryID, u32 maxEntryID, u16 curValue, u16 maxValue, u8 digits, u8 xOffset, u8 yOffset);
#endif

SUMMARY_C8C8_TYPE sub_0208C8C8(PokemonSummary *summary, u32 moveIndex, u32 slashEntryID, u32 curEntryID, u32 maxEntryID, u16 curValue, u16 maxValue, u8 digits, u8 xOffset, u8 yOffset) {
    Window *window = &summary->pageWindows[moveIndex];

    ReadMsgDataIntoString(summary->msgData, slashEntryID, summary->buffer);
    u32 strWidth = FontID_String_GetWidth(FONT_SYSTEM, summary->buffer, 0);
    u16 curXOffset = xOffset - strWidth / 2;
    u16 maxXOffset = curXOffset + strWidth;

    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, summary->buffer, curXOffset, yOffset, 0xFF, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    sub_0208C87C(summary, curEntryID, curValue, digits, 0);

    strWidth = FontID_String_GetWidth(FONT_SYSTEM, summary->buffer, 0);

    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, summary->buffer, curXOffset - strWidth, yOffset, 0xFF, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    sub_0208C87C(summary, maxEntryID, maxValue, digits, 0);
    SUMMARY_C8C8_RETURN AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, summary->buffer, maxXOffset, yOffset, 0xFF, MAKE_TEXT_COLOR(1, 2, 0), NULL);
}
