#include "global.h"

#include "map_object.h"
#include "metatile_behavior.h"

s32 sub_02060E54(LocalMapObject *object, s32 tile);

s32 sub_02060E54(LocalMapObject *object, s32 tile) {
    if (sub_0205BA54((u8)tile) != 0) {
        if (MapObject_CheckFlag28(object) == 0) {
            return 1;
        }
    } else {
        if (MetatileBehavior_IsSurfableWater((u8)tile) != 0) {
            return 1;
        }
        if ((sub_0205B78C((u8)tile) != 0) && ((u32)(MapObject_GetSpriteID(object) - 0xB2) <= 1U)) {
            return 1;
        }
    }
    return 0;
}
