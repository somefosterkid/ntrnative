#include "global.h"

#include "constants/global_fieldmap.h"

#include "fx_util.h"
#include "overlay_01_021FE780.h"
#include "overworld_anim_manager.h"
#include "simple3dmodel.h"

void sub_020699AC(Simple3DRenderObj *renderObj, const VecFx32 *position, const VecFx32 *scale, const MtxFx33 *rotation);

void ov01_021FE970(OverworldAnimManager *animMan, SurfMountRenderer *renderer) {
    if (renderer->inactive == TRUE || renderer->dir == DIR_NONE) {
        return;
    }

    renderer->counter++;

    u32 rotAngle = 0;
    MtxFx33 rotation;
    VecFx32 pos;
    VecFx32 scale = { FX32_ONE, FX32_ONE, FX32_ONE };
    Simple3DRenderObj *surfMountRenderObj = &renderer->userData.resources->unk_18;

    switch (renderer->dir) {
    case DIR_NORTH:
        rotAngle = 180;
        break;

    case DIR_SOUTH:
        break;

    case DIR_WEST:
        rotAngle = 270;
        break;

    case DIR_EAST:
        rotAngle = 90;
        break;
    }

    MTX_Rot33Angles(&rotation, 0, rotAngle, 0);
    sub_02068DB8(animMan, &pos);
    sub_020699AC(surfMountRenderObj, &pos, &scale, &rotation);
}
