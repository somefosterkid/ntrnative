#include "global.h"

#include "map_object.h"
#include "unk_02033AE0.h"
#include "unk_02037C94.h"
#include "unk_02054648.h"

s32 sub_02060CA8(LocalMapObject *, s32, s32, s32);

s32 sub_02060D94(LocalMapObject *, s32, s32, s32);

s32 sub_02060DEC(LocalMapObject *, s32, s32, s32);

s32 sub_02060AF4(LocalMapObject *arg0, VecFx32 *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

s32 sub_02060AF4(LocalMapObject *arg0, VecFx32 *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s8 spC;
    s32 sp8;
    VecFx32 *sp4;
    s32 var_r4;

    sp4 = arg1;
    sp8 = arg3;
    var_r4 = 0;
    if (sub_02060D94(arg0, arg2, arg3, arg4) == 1) {
        var_r4 |= 1;
    }
    if (sub_020549A8(MapObject_GetFieldSystem(arg0), sp4, arg2, arg4, &spC) == 1) {
        var_r4 |= 2;
        if (spC != 0) {
            var_r4 |= 8;
        }
    }
    if (sub_02060DEC(arg0, arg2, arg4, arg5) == 1) {
        var_r4 |= 2;
    }
    if (sub_0203401C((s32)sub_0203993C()) != 0) {
        if (sub_02060BFC(arg0, arg2, sp8, arg4) == 1) {
            var_r4 |= 4;
        }
    } else if (sub_02060CA8(arg0, arg2, sp8, arg4) == 1) {
        var_r4 |= 4;
    }
    return var_r4;
}
