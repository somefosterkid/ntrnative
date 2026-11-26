#include "global.h"

#include "error_handling.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"

void *sub_02064084(LocalMapObject *arg0);

extern s32 ov01_02207274;
s32 sub_020640A4(s32 arg0, s32 arg1);

void *sub_02064084(LocalMapObject *arg0) {
    if (ov01_021FA2D4(arg0) == 1) {
        GF_AssertFail();
        return 0;
    }
    return ((MapObjectBillboardWorkD *)sub_0205F40C(arg0))->billboard;
}

s32 sub_020640A4(s32 arg0, s32 arg1) {
    if (arg1 == 0) {
        return 0x106;
    }
    if (arg0 == 0) {
        GF_AssertFail();
    }
    return *(s32 *)(&ov01_02207274 + arg0);
}
