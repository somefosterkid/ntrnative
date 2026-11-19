#include "global.h"
struct BattleFactoryApp;

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "overlay_81.h"
#include "render_window.h"
#include "unk_02019BA4.h"
#include "overlay_81_battle_factory_app.h"

typedef struct UnkStruct_ov81_022413F4 {
    u8 unk_0[0x388];
    void *unk_388;
    u8 unk_38C[0xD8];
    void *inputHandler;
} UnkStruct_ov81_022413F4;

s32 ov81_02242FDC(BgConfig *, void *);

s32 ov81_022430E8(void *);

void ov81_02242DAC(BattleFactoryAppPanelSprite *panelSprite, int x, int y);
s32 ov81_02243228(GridInputHandler *, s32, s32 *, s32 *);
void ov81_022413F4(UnkStruct_ov81_022413F4 *arg0, s32 arg1);
void ov81_02242DE4(BattleFactoryAppPanelSprite *panelSprite, s32 seq);
void ov81_0224141C(struct BattleFactoryApp *arg0, s32 arg1);
void ov81_02242D88(BattleFactoryAppPanelSprite *panelSprite, BOOL draw);
void ov81_02242E08(BattleFactoryAppPanelSprite *panelSprite, int palIndex);
u8 ov81_02242F40(BattleFactoryAppPokeballSprite *ballSprite);
s32 ov81_02243140(BgConfig *, NARC *, s32);
s32 ov81_02243270(GridInputHandler *, s32);
s32 ov81_022431E0(BgConfig *, s32);

void ov81_02241398(BattleFactoryApp *app) {
    SetBgPriority(1U, 1U);
    BG_ClearCharDataRange(1U, 0x20U, 0U, HEAP_ID_100);
    BgClearTilemapBufferAndCommit(app->bgConfig, 1U);
    ov81_02242FDC(app->bgConfig, &app->windows[0]);
    if (*((s32 *)&app->numMonsToFade) == 2) {
        ov81_022430E8(&app->windows[2]);
    }
    GfGfx_EngineATogglePlanes(2U, 1U);
}

void ov81_022413E0(Window *window) {
    ClearWindowPixelBuffer(window, 1);
    ClearWindowTilemapAndScheduleTransfer(window);
}

void ov81_022413F4(UnkStruct_ov81_022413F4 *arg0, s32 arg1) {
    s32 v1;
    s32 v0;

    ov81_02243228(arg0->inputHandler, arg1, &v1, &v0);
    ov81_02242DAC(arg0->unk_388, v1, v0);
}

void ov81_0224141C(BattleFactoryApp *arg0, s32 arg1) {
    s32 v1;
    s32 v0;

    ov81_02243228(arg0->inputHandler, arg1, &v1, &v0);
    ov81_02242DE4(arg0->unk_38C, 3);
    ov81_02242DAC(arg0->unk_38C, v1, v0);
}

void ov81_02241450(BattleFactoryApp *app) {
    ov81_02243140(*((void **)&app->bgConfig), *((void **)&app->narc), 0);
    ScheduleWindowCopyToVram(&app->windows[11]);
    ScheduleWindowCopyToVram(&app->windows[14]);
    if (ov81_02242F40(*((void **)&app->ballSprites[(app->unk_468)])) == 0) {
        ScheduleWindowCopyToVram(&app->windows[12]);
    } else {
        ScheduleWindowCopyToVram(&app->windows[13]);
    }
    GridInputHandler_SetNextInput(app->inputHandler, 6);
    ov81_0224141C(app, 6);
    ov81_02243270(app->inputHandler, 1);
    ov81_02242D88(app->unk_38C, 1);
    ov81_02242E08(app->unk_388, 1);
}

void ov81_022414E0(BattleFactoryApp *app) {
    ov81_022431E0(*((void **)&app->bgConfig), 0);
    GridInputHandler_SetNextInput(*((GridInputHandler * *)&app->inputHandler), (s32)(u8) app->unk_468);
    ov81_02243270(*((GridInputHandler * *)&app->inputHandler), 0);
    ov81_02242D88(app->unk_38C, 0);
    ov81_02242E08(app->unk_388, 0);
}
