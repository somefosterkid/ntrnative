#include "global.h"

#include "bg_window.h"
#include "overlay_81.h"
#include "unk_02019BA4.h"
#include "overlay_81_battle_factory_app.h"

s32 ov81_0224141C(void *, s32);

void ov81_02242D88(BattleFactoryAppPanelSprite *panelSprite, BOOL draw);

s32 ov81_02243140(BgConfig *, NARC *, s32);

s32 ov81_022431E0(BgConfig *, s32);

s32 ov81_022433FC(GridInputHandler *, s32, s32);

void ov81_02241980(BattleFactoryApp *app) {
    ov81_022431E0(*((void **)&app->bgConfig), 2);
    ov81_02243140(*((void **)&app->bgConfig), *((void **)&app->narc), 1);
    ScheduleWindowCopyToVram(&app->windows[16]);
    ScheduleWindowCopyToVram(&app->windows[17]);
    GridInputHandler_SetNextInput(app->inputHandler, 6);
    ov81_0224141C(app, 6);
    ov81_022433FC(app->inputHandler, 1, app->challengeType);
    ov81_02242D88(app->unk_38C, 1);
}

void ov81_022419E0(BattleFactoryApp *app) {
    ov81_022431E0(*((void **)&app->bgConfig), 1);
    ov81_02243140(*((void **)&app->bgConfig), *((void **)&app->narc), 2);
    ScheduleWindowCopyToVram(&app->windows[9]);
    ScheduleWindowCopyToVram(&app->windows[10]);
    GridInputHandler_SetNextInput(app->inputHandler, (s32)(u8) app->unk_468);
    ov81_022433FC(app->inputHandler, 0, app->challengeType);
    ov81_02242D88(app->unk_38C, 0);
}

void ov81_02241A38(BattleFactoryApp *app) {
    s32 v0;

    v0 = GridInputHandler_GetNextInput(app->inputHandler);
    ov81_02243140(*((void **)&app->bgConfig), *((void **)&app->narc), 2);
    ScheduleWindowCopyToVram(&app->windows[9]);
    ScheduleWindowCopyToVram(&app->windows[10]);
    if ((u32)v0 >= 4U) {
        ov81_02242D88(app->unk_38C, 1);
    }
}

void ov81_02241A7C(BattleFactoryApp *app) {
    ov81_022431E0(*((void **)&app->bgConfig), 2);
    ov81_02242D88(app->unk_38C, 0);
}
