#include "global.h"

#include "map_object.h"
#include "unk_02062108.h"

void ov01_02206028(LocalMapObject *arg0, LocalMapObject *arg1);

void ov01_02206028(LocalMapObject *arg0, LocalMapObject *arg1) {
    LocalMapObject *new_var2;
    int new_var;
    LocalMapObject *sp0;
    s32 temp_r6_2;
    s32 temp_r4_2;
    u32 temp_r4;
    unsigned int temp_r6;
    u32 temp_r7;
    sp0 = arg0;
    temp_r6 = MapObject_GetXCoord(arg0);
    temp_r7 = MapObject_GetZCoord(arg0);
    temp_r4 = MapObject_GetXCoord(arg1);
    new_var = temp_r7 - MapObject_GetZCoord(new_var2 = arg1);
    temp_r6_2 = temp_r6 - temp_r4;
    temp_r4_2 = new_var;
    MapObject_GetFacingDirection(arg0);
    if (temp_r6_2 < 0) {
        MapObject_SetHeldMovement(new_var2, 0xAU);
        return;
    }
    if (temp_r6_2 > 0) {
        MapObject_SetHeldMovement(new_var2, 0xBU);
        return;
    }
    if (0 > temp_r4_2) {
        MapObject_SetHeldMovement(new_var2, 8U);
        return;
    }
    if (temp_r4_2 > 0) {
        MapObject_SetHeldMovement(new_var2, 9U);
    }
}
