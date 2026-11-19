#include "global.h"

#include "billboard.h"
#include "map_object.h"

s32 ov01_021F9744(MapObjectManager *, u32, s32 *);

s32 ov01_021FA44C(s32 arg0);

void ov01_02205870(s32, LocalMapObject *, Billboard *, s32 *);

void sub_02023EC8(Billboard *, s32 *);

void sub_02023EE0(Billboard *, s32);

void sub_02023F40(Billboard *, s32);

void ov01_02205808(s32 arg0, LocalMapObject *arg1, Billboard *arg2);

void ov01_02205808(s32 arg0, LocalMapObject *arg1, Billboard *arg2) {
    s32 sp0[10];
    MapObjectManager *temp_r7;
    u32 temp_r0;

    temp_r7 = MapObject_GetManager(arg1);
    ov01_021F9744(temp_r7, MapObject_GetSpriteID(arg1), sp0);
    temp_r0 = MapObject_GetID(arg1);
    if (temp_r0 != 0xFD && temp_r0 != 0xFA && temp_r0 != 0xFB) {
        return;
    }
    ov01_02205870(arg0, arg1, arg2, sp0);
    sub_02023EC8(arg2, sp0);
    sub_02023EE0(arg2, ov01_021FA44C(MapObject_GetFacingDirection(arg1)));
    sub_02023F40(arg2, 0);
}
