#include "global.h"
#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "overlay_28.h"
#include "script_manager.h"
#include "sprite.h"
#include "task.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"

extern TouchscreenHitbox ov28_0225EA88;

void ov28_0225DC2C(SysTask *task, void *env) {
    DowsingMchnApp *app = env;
    FieldSystem *fieldSystem;
    s32 v0;

    fieldSystem = app->fieldSystem;
    if ((fieldSystem->unkD2_7 != 0) && (FieldSystem_IsPlayerMovementAllowed(fieldSystem) != 0)) {
        app->fieldSystem->unkD2_7 = FALSE;
    }
    if ((app->touching == 0) && (TouchscreenHitbox_FindHitboxAtTouchNew(&ov28_0225EA88) != -1)) {
        app->touching = 1;
    }
    if ((FieldSystem_TaskIsRunning(app->fieldSystem) == 1) || (TouchscreenHitbox_FindHitboxAtTouchHeld(&ov28_0225EA88) == -1) || (v0 = app->touching, (v0 == 0))) {
        ov28_0225E31C(app, -1);
        ov28_0225E578(app, 0);
        app->needleTimerActive = 0;
        app->unk_338 = 0;
        app->unk_33A = 0;
        app->touching = 0;
        StopSE(0x93DU, 0);
    } else if (v0 == 1) {
        ov28_0225E7D4(app);
        ov28_0225E1A4(app);
        ov28_0225E31C(app, 1);
        ov28_0225E578(app, 1);
        ov28_0225E730(app);
    }
    ov28_0225E3BC(app);
    ov28_0225E43C(app);
    ov28_0225E900(app);
    ov28_0225E938(app);
    ov28_0225E9E0(app);
    SpriteList_RenderAndAnimateSprites(app->spriteList);
}

void DowsingMchn_GetHiddenItemLocs(DowsingMchnApp *app) {
    app->hiddenItems = AllocAndFetchNearbyHiddenItems(app->fieldSystem, HEAP_ID_8);
}

void DowsingMchn_FreeHiddenItemLocs(DowsingMchnApp *app) {
    void *ptr;

    ptr = app->hiddenItems;
    if (ptr != NULL) {
        Heap_Free(ptr);
        app->hiddenItems = NULL;
    }
}
