#include "global.h"

#include "overlay_01_021FE780.h"
#include "overworld_anim_manager.h"

void ov01_021FE9F4(OverworldAnimManager *animMan, BOOL syncPos);

void ov01_021FE9F4(OverworldAnimManager *animMan, BOOL syncPos) {
    SurfMountRenderer *renderer = sub_02068D74(animMan);

    renderer->syncPos = syncPos;
    renderer->yOffset = FX32_ONE;
    renderer->yDelta = 0x400;
}
