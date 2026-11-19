#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

void ov81_02241A98(BattleFactoryApp *app);

void ov81_02241A98(BattleFactoryApp *app) {
    s32 var_r5;

    FillWindowPixelBuffer(&app->windows[16], 2U);
    FillWindowPixelBuffer(&app->windows[17], 2U);
    FillWindowPixelBuffer(&app->windows[9], 2U);
    FillWindowPixelBuffer(&app->windows[10], 2U);
    var_r5 = GetWindowWidth(&app->windows[16]);
    var_r5 <<= 3;
    ov81_022430B4((&app->windows[16]), app->msgLoader, 0x19, var_r5 / 2, 0, 4, 0xF0E02, 2);
    var_r5 = GetWindowWidth(&app->windows[17]);
    var_r5 <<= 3;
    ov81_022430B4((&app->windows[17]), app->msgLoader, 0x16, var_r5 / 2, 0, 4, 0xF0E02, 2);
    var_r5 = GetWindowWidth(&app->windows[9]);
    var_r5 <<= 3;
    ov81_022430B4((&app->windows[9]), app->msgLoader, 0x17, var_r5 / 2, 0, 4, 0xF0E02, 2);
    var_r5 = GetWindowWidth(&app->windows[10]);
    var_r5 <<= 3;
    ov81_022430B4((&app->windows[10]), app->msgLoader, 0x18, var_r5 / 2, 0, 4, 0xF0E02, 2);
    CopyWindowPixelsToVram_TextMode(&app->windows[16]);
    CopyWindowPixelsToVram_TextMode(&app->windows[17]);
    CopyWindowPixelsToVram_TextMode(&app->windows[9]);
    CopyWindowPixelsToVram_TextMode(&app->windows[10]);
}
