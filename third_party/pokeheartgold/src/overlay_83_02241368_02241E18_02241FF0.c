#include "global.h"
#include "msgdata.h"
#include "font.h"

#include "bg_window.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_022479E4(Window *window, MsgData *msgData, s32 strno, u32 x, u32 y, u32 fontId, u32 color, s32 alignment);

void ov83_02241FF0(BattleCastleSelfApp *app);

void ov83_02241FF0(BattleCastleSelfApp *app) {
    FillWindowPixelBuffer(&app->windows[20], 0);
    FillWindowPixelBuffer(&app->windows[22], 0);
    FillWindowPixelBuffer(&app->windows[24], 0);
    FillWindowPixelBuffer(&app->windows[26], 0);
    FillWindowPixelBuffer(&app->windows[28], 0);
    FillWindowPixelBuffer(&app->windows[30], 0);
    FillWindowPixelBuffer(&app->windows[32], 0);
    FillWindowPixelBuffer(&app->windows[34], 0);
    FillWindowPixelBuffer(&app->windows[36], 0);
    FillWindowPixelBuffer(&app->windows[38], 0);
    ov83_022479E4(&app->windows[20], app->msgLoader, 0x58, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[22], app->msgLoader, 0x4a, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[24], app->msgLoader, 0x48, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[26], app->msgLoader, 0x46, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[28], app->msgLoader, 0x59, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[30], app->msgLoader, 0x4c, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[32], app->msgLoader, 0x50, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[34], app->msgLoader, 0x4e, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[36], app->msgLoader, 0x52, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[38], app->msgLoader, 0x54, 0, 0, 0, 0x10200, 0);
    CopyWindowPixelsToVram_TextMode(&app->windows[20]);
    CopyWindowPixelsToVram_TextMode(&app->windows[22]);
    CopyWindowPixelsToVram_TextMode(&app->windows[24]);
    CopyWindowPixelsToVram_TextMode(&app->windows[26]);
    CopyWindowPixelsToVram_TextMode(&app->windows[28]);
    CopyWindowPixelsToVram_TextMode(&app->windows[30]);
    CopyWindowPixelsToVram_TextMode(&app->windows[32]);
    CopyWindowPixelsToVram_TextMode(&app->windows[34]);
    CopyWindowPixelsToVram_TextMode(&app->windows[36]);
    CopyWindowPixelsToVram_TextMode(&app->windows[38]);
    return;
}
