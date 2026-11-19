#include "global.h"
#include "overlay_83_battle_castle_self_app.h"
#include "touchscreen_list_menu.h"

u8 ov83_0223FC48(BattleCastleSelfApp *app, Window *window, int messageID, u32 x, u32 y, u32 renderDelay, u8 textColor, u8 shadowColor, u8 bgColor, u8 fontID);

extern const u16 ov83_02247D1E[];

void ov83_02242DAC(BattleCastleSelfApp *app);

void ov83_02242DAC(BattleCastleSelfApp *app) {
    if (app->listSelection != app->touchMenu->cursorPos) {
        ov83_0223FC48(app, &app->windows[SELF_APP_WINDOW_MSG_BOX], ov83_02247D1E[app->touchMenu->cursorPos], 1, 1, 0xff, 1, 2, 0xf, 1);
        app->listSelection = app->touchMenu->cursorPos;
    }
}
