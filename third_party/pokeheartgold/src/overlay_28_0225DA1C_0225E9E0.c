#include "global.h"
#include "bg_window.h"
#include "menu_input_state.h"
#include "overlay_28.h"
#include "sprite.h"
#include "task.h"
#include "touchscreen.h"

extern const TouchscreenHitbox ov28_0225EA80;

void ov28_0225E9E0(DowsingMchnApp *app) {
    if (ov28_0225EA58(app) == 1) {
        ClearWindowTilemapAndScheduleTransfer(&app->windows[4]);
        Sprite_SetAnimCtrlSeq(app->sprites[3], 8);
        Sprite_SetDrawFlag(app->sprites[3], 0);
        return;
    }
    ScheduleWindowCopyToVram(&app->windows[4]);
    Sprite_SetDrawFlag(app->sprites[3], 1);
    if (ov28_0225E8B8(&ov28_0225EA80, app->sprites[3], 8, 9) != 0) {
        app->fieldSystem->lastTouchMenuInput = 0xB;
        MenuInputStateMgr_SetState(&app->fieldSystem->menuInputState, MENU_INPUT_STATE_TOUCH);
    }
}

s32 ov28_0225EA58(DowsingMchnApp *app) {
    if ((FieldSystem_TaskIsRunning(app->fieldSystem) == 1) && (app->fieldSystem->unkD2_7 == 0)) {
        return 1;
    }
    return 0;
}
