#include "global.h"

#include "map_object.h"
#include "overlay_01_022001E4.h"

void sub_02062F48(LocalMapObject *arg0, s32 arg1);

void sub_02062F48(LocalMapObject *object, s32 a0) {
    u8 *v0;

    v0 = sub_0205F3C0(object, 8);
    *(s32 *)v0 = a0;
    *(OverworldAnimManager **)((u8 *)v0 + 4) = ov01_02200540(object, a0, 1);
    MapObject_IncrementMovementStep(object);
}
