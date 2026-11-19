#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_98.h"
#include "player_data.h"
#include "pm_string.h"
#include "text.h"

void ov98_0221EDC4(Ov98TextSys *textSys, s32 windowIdx, s32 msgId, u32 idx, PlayerProfile *profile) {
    String *src;

    BufferPlayersName(textSys->msgFmt, idx, profile);
    src = NewString_ReadMsgData(textSys->msgData, msgId);
    StringExpandPlaceholders(textSys->msgFmt, textSys->string, src);
    ov98_0221EF14(textSys, windowIdx);
    AddTextPrinterParameterizedWithColor(&textSys->windows[windowIdx], FONT_SYSTEM, textSys->string, 0, 0, 0xff, 0x10200, (PrinterCallback_t)0x0);
    ScheduleWindowCopyToVram(&textSys->windows[windowIdx]);
    String_Delete(src);
}
