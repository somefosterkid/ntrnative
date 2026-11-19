#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

void ov81_02241524(BattleFactoryApp *app);

void ov81_02241524(BattleFactoryApp *app) {
    s32 width;

    FillWindowPixelBuffer(&app->windows[11], 2U);
    FillWindowPixelBuffer(&app->windows[12], 2U);
    FillWindowPixelBuffer(&app->windows[13], 2U);
    FillWindowPixelBuffer(&app->windows[14], 2U);
    width = GetWindowWidth(&app->windows[11]);
    ov81_022430B4((&app->windows[11]), app->msgLoader, 5, (width * 8) / 2, 0, 4, 0xF0E02, 2);
    width = GetWindowWidth(&app->windows[12]);
    ov81_022430B4((&app->windows[12]), app->msgLoader, 6, (width * 8) / 2, 0, 4, 0xF0E02, 2);
    width = GetWindowWidth(&app->windows[13]);
    ov81_022430B4((&app->windows[13]), app->msgLoader, 8, (width * 8) / 2, 0, 4, 0xF0E02, 2);
    width = GetWindowWidth(&app->windows[14]);
    ov81_022430B4((&app->windows[14]), app->msgLoader, 7, (width * 8) / 2, 0, 4, 0xF0E02, 2);
    CopyWindowPixelsToVram_TextMode(&app->windows[11]);
    CopyWindowPixelsToVram_TextMode(&app->windows[12]);
    CopyWindowPixelsToVram_TextMode(&app->windows[13]);
    CopyWindowPixelsToVram_TextMode(&app->windows[14]);
}
