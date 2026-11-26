#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_83_battle_castle_opponent_app.h"

u8 ov83_022447E0(BattleCastleOpponentApp *app, Window *window, int messageID, u32 x, u32 y, u32 renderDelay, u8 textColor, u8 shadowColor, u8 bgColor, u8 fontID);

u8 ov83_022447E0(BattleCastleOpponentApp *app, Window *window, int messageID, u32 x, u32 y, u32 renderDelay, u8 textColor, u8 shadowColor, u8 bgColor, u8 fontID) {
    FillWindowPixelBuffer(window, bgColor);
    ReadMsgDataIntoString(app->msgLoader, messageID, app->fmtStr);
    StringExpandPlaceholders(app->strTemplate, app->displayStr, app->fmtStr);

    u8 result = AddTextPrinterParameterizedWithColor(window, (enum Font)fontID, app->displayStr, x, y, renderDelay, MAKE_TEXT_COLOR(textColor, shadowColor, bgColor), 0L);
    ScheduleWindowCopyToVram(window);
    return result;
}
