#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "list_menu_items.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_02240C48(struct BattleCastleSelfApp *, u32, u32, u32, u32);

u16 ov83_02240EC4(struct BattleCastleSelfApp *, u16, s32);

void ov83_02241DD8(struct BattleCastleSelfApp *, Window *, MsgData *, u32, u32, u32, u32, u32, u32);

void ov83_02240664(BattleCastleSelfApp *app);

void ov83_02240664(BattleCastleSelfApp *app) {
    u16 start;
    u16 i;
    u32 val;

    i = 0;
    start = app->page * 6;
    do {
        FillWindowPixelBuffer(&app->windows[i + 0x30], 0);
        if ((start + i) < app->numEntries) {
            AddTextPrinterParameterizedWithColor(&app->windows[i + 0x30], FONT_SYSTEM, app->listItems[start + i].text, 4, 4, 0xff, 0x10200, 0L);
            val = ov83_02240EC4(app, app->listItems[start + i].value, app->selectedMenuEntry);
            ov83_02240C48(app, 0, val, 2, 0);
            ov83_02241DD8(app, &app->windows[i + 0x30], app->msgLoader, 0x68, (GetWindowWidth(&app->windows[i + 0x30]) * 8) - 4, 0x14, 0, 0x10200, 1);
        }
        ScheduleWindowCopyToVram(&app->windows[i + 0x30]);
        i++;
    } while (i < 6);
}
