#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FE780.h"
#include "overworld_anim_manager.h"
#include "sys_task.h"

BOOL ov01_021FE868(OverworldAnimManager *animMan, SurfMountRenderer *renderer) {
    const SurfMountUserData *userData = sub_02068D98(animMan);
    LocalMapObject *playerMapObj = userData->playerMapObj;

    renderer->userData = *userData;
    renderer->mapObjLocalID = MapObject_GetID(playerMapObj);
    renderer->mapHeaderID = MapObject_GetMapID(playerMapObj);
    renderer->dir = userData->dir;
    renderer->syncPos = sub_02068D90(animMan);
    renderer->yOffset = FX32_ONE;
    renderer->yDelta = 0x400;

    return 1;
}

void ov01_021FE8B0(OverworldAnimManager *animMan, SurfMountRenderer *renderer) {
    LocalMapObject *playerMapObj = renderer->userData.playerMapObj;
    VecFx32 spritePosOffset = { 0, 0, 0 };

    sub_0205F9A0(playerMapObj, &spritePosOffset);
}
