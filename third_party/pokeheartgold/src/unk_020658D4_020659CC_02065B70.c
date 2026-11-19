#include "global.h"

#include "follow_mon.h"
#include "map_object.h"
#include "overlay_01_022031C0.h"

s32 sub_02065D24(LocalMapObject *, u8 *);

s32 sub_02065D58(LocalMapObject *, u8 *);

s32 sub_02065F44(LocalMapObject *);

s32 sub_020664D8(LocalMapObject *);

s32 sub_02065B70(LocalMapObject *arg0, u8 *arg1);

s32 sub_02065B70(LocalMapObject *mapObject, u8 *arg1) {
    MapObject_ClearSingleMovement(mapObject);
    MapObject_ClearEndMovement(mapObject);
    if (sub_02065D24(mapObject, arg1) == 1) {
        sub_02065D58(mapObject, arg1);
        if (sub_02069E14(mapObject) != 0) {
            if (sub_02069EAC(mapObject) != 0) {
                ov01_0220329C(mapObject, 0);
                sub_02069E84(mapObject, 0U);
            } else {
                sub_02069DC8(mapObject, 0);
            }
            sub_020664D8(mapObject);
        }
        if (sub_02065F44(mapObject) == 1) {
            MapObject_SetSingleMovement(mapObject);
            *arg1 += 1;
            return 1;
        }
        goto error;
    }
error:
    return 0;
}
