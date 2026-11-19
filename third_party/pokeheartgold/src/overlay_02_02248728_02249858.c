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

s32 ov02_0224B768(void *);

s32 ov02_022498BC(HMCutIn *arg0);

BOOL ov02_02249858(HMCutIn *cutIn);

BOOL ov02_02249858(HMCutIn *cutIn) {
    const VecFx32 *v0;
    VecFx32 v1;

    cutIn->pokemonSpriteDeltaX *= 2;

    if (cutIn->pokemonSpriteDeltaX < (((fx32)0x0000000000001000L) * -64)) {
        cutIn->pokemonSpriteDeltaX = (((fx32)0x0000000000001000L) * -64);
    }

    v0 = Sprite_GetMatrixPtr(cutIn->pokemonSprite);
    v1 = *v0;
    v1.x += cutIn->pokemonSpriteDeltaX;

    Sprite_SetMatrix(cutIn->pokemonSprite, &v1);

    if (v1.x <= (((fx32)0x0000000000001000L) * -40)) {
        cutIn->windowDelta = 0x1000;
        cutIn->state++;
    }

    return 0;
}

s32 ov02_022498BC(HMCutIn *arg0) {
    s32 v0;

    arg0->updateWindow = 0;
    arg0->windowY1 = (s32)(arg0->windowY1 + arg0->windowDelta);
    arg0->windowY2 = (s32)(arg0->windowY2 - arg0->windowDelta);
    v0 = arg0->windowDelta + 0x4000;
    arg0->windowDelta = v0;
    if (v0 > 0x10000) {
        arg0->windowDelta = 0x10000;
    }
    if ((s32) arg0->windowY1 >= 0x5F000) {
        arg0->windowY1 = 0x5F000;
    }
    if ((s32) arg0->windowY2 <= 0x61000) {
        arg0->windowY2 = 0x61000;
    }
    ov02_0224A69C(arg0, arg0->windowX1, arg0->windowY1, arg0->windowX2, arg0->windowY2);
    arg0->updateWindow = 1;
    if ((arg0->windowY1 == 0x5F000) && (arg0->windowY2 == 0x61000)) {
        ov02_0224B768(arg0);
        arg0->planeMaskInside = 0x11;
        arg0->state = arg0->state + 1;
    }
    return 0;
}
