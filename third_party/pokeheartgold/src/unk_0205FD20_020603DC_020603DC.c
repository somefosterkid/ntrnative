#include "global.h"

#include "map_object.h"

s32 sub_020603F8(LocalMapObject *, s32, u8, u16 *);

void sub_020603DC(LocalMapObject *arg0, s32 arg1, s32 arg2, void *arg3);

void sub_020603DC(LocalMapObject *object, s32 arg1, s32 arg2, void *arg3) {
    u8 new_var;
    new_var = (u8)sub_0205F524(object);
    sub_020603F8(object, arg1, new_var, arg3);
}
