#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

void ov81_0224185C(BattleFactoryApp *app);

void ov81_0224185C(BattleFactoryApp *app) {
    s32 width;

    FillWindowPixelBuffer(&app->windows[11], 2U);
    FillWindowPixelBuffer(&app->windows[15], 2U);
    FillWindowPixelBuffer(&app->windows[14], 2U);
    FillWindowPixelBuffer(&app->windows[10], 2U);
    width = GetWindowWidth(&app->windows[11]);
    ov81_022430B4((&app->windows[11]), app->msgLoader, 0x14, (width * 8) / 2, 0, 4, 0xF0E02, 2);
    width = GetWindowWidth(&app->windows[15]);
    ov81_022430B4((&app->windows[15]), app->msgLoader, 0x15, (width * 8) / 2, 0, 4, 0xF0E02, 2);
    width = GetWindowWidth(&app->windows[14]);
    ov81_022430B4((&app->windows[14]), app->msgLoader, 0x16, (width * 8) / 2, 0, 4, 0xF0E02, 2);
    width = GetWindowWidth(&app->windows[10]);
    ov81_022430B4((&app->windows[10]), app->msgLoader, 0x13, (width * 8) / 2, 0, 4, 0xF0E02, 2);
    CopyWindowPixelsToVram_TextMode(&app->windows[11]);
    CopyWindowPixelsToVram_TextMode(&app->windows[15]);
    CopyWindowPixelsToVram_TextMode(&app->windows[14]);
    CopyWindowPixelsToVram_TextMode(&app->windows[10]);
}
