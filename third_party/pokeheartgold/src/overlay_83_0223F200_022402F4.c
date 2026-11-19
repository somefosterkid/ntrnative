#include "global.h"

#include "bg_window.h"
#include "options.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_02241354(Window *window);

void ov83_022402F4(BattleCastleSelfApp *app);

void ov83_022402F4(BattleCastleSelfApp *app) {
    ov83_02241354(&app->windows[SELF_APP_WINDOW_MON_SELECTION_MSG_BOX]);
}

void ov83_02242BAC(BattleCastleSelfApp *app);

void ov83_02242DAC(BattleCastleSelfApp *app);

void ov83_02247944(Window *window, u32 frame);

void ov83_02240300(BattleCastleSelfApp *app);

void ov83_02240300(BattleCastleSelfApp *app) {
    ov83_02247944(&app->windows[SELF_APP_WINDOW_MSG_BOX], Options_GetFrame(app->options));
    app->listSelection = -2;
    ov83_02242BAC(app);
    ov83_02242DAC(app);
}

void ov83_02242D5C(BattleCastleSelfApp *app);

void ov83_02240334(BattleCastleSelfApp *app);

void ov83_02240334(BattleCastleSelfApp *app) {
    ov83_02241354(&app->windows[SELF_APP_WINDOW_MSG_BOX]);
    ov83_02242D5C(app);
}

void ov83_02242BF0(BattleCastleSelfApp *app);

void ov83_02242DFC(BattleCastleSelfApp *app);

void ov83_02240348(BattleCastleSelfApp *app);

void ov83_02240348(BattleCastleSelfApp *app) {
    ov83_02247944(&app->windows[SELF_APP_WINDOW_MSG_BOX], Options_GetFrame(app->options));
    app->listSelection = -2;
    ov83_02242BF0(app);
    ov83_02242DFC(app);
}
