#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FE780.h"
#include "overworld_anim_manager.h"

void ov01_021FE8C8(OverworldAnimManager *a0, SurfMountRenderer *arg1) {
    VecFx32 vector;
    VecFx32 positionVector;
    LocalMapObject *object;
    s32 v2;
    u32 v0;
    u32 v1;

    object = arg1->userData.playerMapObj;
    if (sub_0205F0A8(object, arg1->mapObjLocalID, arg1->mapHeaderID) == 0) {
        ov01_021F1640(a0);
        return;
    }
    arg1->inactive = 0;
    v0 = MapObject_GetFacingDirection(object);
    arg1->dirRaw = v0;
    if (v0 == -1U) {
        arg1->inactive = 1;
        return;
    }
    if (arg1->syncPos != 0) {
        arg1->dir = v0;
        v2 = arg1->yOffset + arg1->yDelta;
        arg1->yOffset = v2;
        if (v2 >= 0x4000) {
            arg1->yOffset = 0x4000;
            arg1->yDelta = (s32)(0 - arg1->yDelta);
        } else {
            v1 = 0x4000U >> 2;
            if (v2 <= (s32)v1) {
                arg1->yOffset = (s32)v1;
                arg1->yDelta = (s32)(0 - arg1->yDelta);
            }
        }
        vector.x = 0;
        vector.y = arg1->yOffset + 0x4000;
        vector.z = 0x4000;
        sub_0205F9A0(object, (VecFx32 *)&vector);
        MapObject_CopyPositionVector(object, &positionVector);
        positionVector.y += arg1->yOffset - 0x1000;
        sub_02068DA8(a0, &positionVector);
    }
}
