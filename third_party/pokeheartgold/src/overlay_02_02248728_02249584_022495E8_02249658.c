#include "global.h"

#include "constants/heap.h"

#include "field/dynamic_terrain_height.h"
#include "field/map_prop_animation.h"
#include "field/model_attributes.h"

#include "bag_cursor.h"
#include "bg_window.h"
#include "camera.h"
#include "field_types_def.h"
#include "map_matrix.h"
#include "options.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_02_hmcut_in.h"
#include "sprite.h"

struct SpriteList;

s32 ov02_0224A69C(void *, s32, s32, s32, s32);

s32 ov02_02249690(HMCutIn *arg0);

s32 ov02_0224A450(Sprite *);

s32 ov02_022496D0(HMCutIn *arg0);

void ov02_0224B72C(HMCutIn *cutIn);

BOOL ov02_02249658(HMCutIn *cutIn);

BOOL ov02_02249658(HMCutIn *cutIn) {
    ov02_0224B72C(cutIn);

    cutIn->updateWindow = FALSE;
    cutIn->windowDelta = FX32_ONE * -64;
    cutIn->windowX1 = FX32_ONE * 254;
    cutIn->windowX2 = FX32_ONE * 255;
    cutIn->windowY1 = FX32_ONE * (96 - 1);
    cutIn->windowY2 = FX32_ONE * (96 + 1);
    cutIn->updateWindow = TRUE;
    cutIn->state++;

    return FALSE;
}

s32 ov02_02249690(HMCutIn *arg0) {
    s32 v0;

    arg0->updateWindow = 0;
    v0 = arg0->windowX1 + arg0->windowDelta;
    arg0->windowX1 = v0;
    if (v0 <= 0) {
        arg0->windowX1 = 0;
        arg0->windowDelta = 0x2000;
        arg0->state = arg0->state + 1;
    }
    ov02_0224A69C(arg0, arg0->windowX1, arg0->windowY1, arg0->windowX2, arg0->windowY2);
    arg0->updateWindow = 1;
    return 0;
}

s32 ov02_022496D0(HMCutIn *arg0) {
    s32 v0;

    arg0->updateWindow = 0;
    arg0->windowY1 = (s32)(arg0->windowY1 - arg0->windowDelta);
    arg0->windowY2 = (s32)(arg0->windowY2 + arg0->windowDelta);
    v0 = arg0->windowDelta + 0x2000;
    arg0->windowDelta = v0;
    if (v0 > 0x20000) {
        arg0->windowDelta = 0x20000;
    }
    if ((s32) arg0->windowY1 < 0x38000) {
        arg0->windowY1 = 0x38000;
    }
    if ((s32) arg0->windowY2 > 0x88000) {
        arg0->windowY2 = 0x88000;
    }
    ov02_0224A69C(arg0, arg0->windowX1, arg0->windowY1, arg0->windowX2, arg0->windowY2);
    arg0->updateWindow = 1;
    if ((arg0->windowY1 == 0x38000) && (arg0->windowY2 == 0x88000)) {
        ov02_0224A450(arg0->playerSprite);
        arg0->state = arg0->state + 1;
    }
    return 0;
}
