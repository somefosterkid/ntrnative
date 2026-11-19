#include "global.h"

#include "bg_window.h"
#include "overlay_81.h"
#include "unk_02019BA4.h"
#include "overlay_81_battle_factory_app.h"

void ov81_02242D88(BattleFactoryAppPanelSprite *panelSprite, BOOL draw);

void ov81_02242DAC(BattleFactoryAppPanelSprite *panelSprite, int x, int y);

void ov81_02242DE4(BattleFactoryAppPanelSprite *panelSprite, s32 seq);

s32 ov81_02243228(GridInputHandler *, s32, s32 *, s32 *);

void ov81_02241650(BattleFactoryApp *arg0, s32 arg1);

void ov81_02241650(BattleFactoryApp *arg0, s32 arg1) {
    s32 v1;
    s32 v0;

    ov81_02243228(arg0->inputHandler, arg1, &v1, &v0);
    if (arg1 == 3) {
        ov81_02242D88(arg0->unk_388, 0);
        ov81_02242D88(arg0->unk_38C, 1);
        ov81_02242DE4(arg0->unk_38C, 6);
        ov81_02242DAC(arg0->unk_38C, v1, v0);
        return;
    }
    ov81_02242D88(arg0->unk_388, 1);
    ov81_02242D88(arg0->unk_38C, 0);
    ov81_02242DAC(arg0->unk_388, v1, v0);
}

void ov81_022416CC(BattleFactoryApp *arg0, s32 arg1);

void ov81_022416CC(BattleFactoryApp *arg0, s32 arg1) {
    s32 v1;
    s32 v0;

    ov81_02243228(arg0->inputHandler, arg1, &v1, &v0);
    if ((u32)(arg1 - 4) <= 1U) {
        ov81_02242D88(arg0->unk_388, 0);
        ov81_02242D88(arg0->unk_38C, 1);
        ov81_02242DE4(arg0->unk_38C, 6);
        ov81_02242DAC(arg0->unk_38C, v1, v0);
        return;
    }
    ov81_02242D88(arg0->unk_388, 1);
    ov81_02242D88(arg0->unk_38C, 0);
    ov81_02242DAC(arg0->unk_388, v1, v0);
}

s32 ov81_0224141C(void *, s32);

s32 ov81_02243140(BgConfig *, NARC *, s32);

s32 ov81_022431E0(BgConfig *, s32);

s32 ov81_02243314(GridInputHandler *, s32, s32);

void ov81_0224174C(BattleFactoryApp *app) {
    ov81_022431E0(*((void **)&app->bgConfig), 3);
    ov81_02243140(*((void **)&app->bgConfig), *((void **)&app->narc), 0);
    ScheduleWindowCopyToVram(&app->windows[11]);
    ScheduleWindowCopyToVram(&app->windows[15]);
    ScheduleWindowCopyToVram(&app->windows[14]);
    GridInputHandler_SetNextInput(app->inputHandler, 4);
    ov81_0224141C(app, 4);
    ov81_02243314(app->inputHandler, 1, app->challengeType);
    ov81_02242D88(app->unk_38C, 1);
}

void ov81_022417B4(BattleFactoryApp *app) {
    ov81_022431E0(*((void **)&app->bgConfig), 0);
    ov81_02243140(*((void **)&app->bgConfig), *((void **)&app->narc), 3);
    ScheduleWindowCopyToVram(&app->windows[10]);
    GridInputHandler_SetNextInput(app->inputHandler, (s32)(u8) app->unk_468);
    ov81_02243314(app->inputHandler, 0, app->challengeType);
    ov81_02242D88(app->unk_38C, 0);
}

void ov81_02241804(void *arg0);

void ov81_02241804(void *arg0) {
    s32 v0;

    v0 = GridInputHandler_GetNextInput(((BattleFactoryApp *)arg0)->inputHandler);
    ov81_02243140(*((void **)&((BattleFactoryApp *)arg0)->bgConfig), *((void **)&((BattleFactoryApp *)arg0)->narc), 3);
    ScheduleWindowCopyToVram(&((BattleFactoryApp *)arg0)->windows[10]);
    if (v0 == 3) {
        ov81_02242D88(((BattleFactoryApp *)arg0)->unk_38C, 1);
    }
}

void ov81_02241840(BattleFactoryApp *app) {
    ov81_022431E0(*((void **)&app->bgConfig), 3);
    ov81_02242D88(app->unk_38C, 0);
}
