#include "global.h"
#include "map_object.h"
#include "sys_task.h"

s32 sub_020632B0(void *, s32, s32, s16, s8, s16, u16);

s32 MapObjectMovementCmd112_Step0(void *arg0);

void sub_02063A40(LocalMapObject *mapObj);
void sub_02063A14(LocalMapObject *mapObj);

s32 sub_02063A5C(LocalMapObject *mapObj);
s32 sub_02063A78(LocalMapObject *mapObj);
s32 sub_02063A1C(void *arg0);

s32 MapObjectMovementCmd112_Step0(void *arg0) {
    sub_020632B0(arg0, 4, 0, 0, 3, 9, 3);
    return 1;
}

void sub_02063A14(LocalMapObject *mapObj) {
    sub_02063A40(mapObj);
}

s32 sub_02063A1C(void *arg0) {
    if (sub_02063A5C(arg0) == 0) {
        return 0;
    }
    if (sub_02063A78(arg0) != 0) {
        return 1;
    }
    return 0;
}
