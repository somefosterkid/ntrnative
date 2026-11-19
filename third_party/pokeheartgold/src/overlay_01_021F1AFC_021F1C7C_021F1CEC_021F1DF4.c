#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1ECC.h"

static s32 ov01_021F1E54(void *arg0, void *arg1, s32 arg2, s32 arg3);

s32 ov01_021F232C(void *, PlayerAvatar *, s32, s32);

s32 ov01_021F24F4(void *, void *, s32, s32);

s32 sub_0205DEC0(void *, s32);

s32 ov01_021F1DF4(void *arg0, void *arg1, s32 arg2, s32 arg3);

s32 ov01_021F1DF4(void *arg0, void *arg1, s32 arg2, s32 arg3) {
    s32 v0;

    v0 = sub_0205DEC0(arg1, arg2);
    if ((1 & arg3) && (ov01_021F1E54(arg0, arg1, arg2, v0) == 1)) {
        return 1;
    }
    if ((2 & arg3) && (ov01_021F24F4(arg0, arg1, arg2, v0) == 1)) {
        return 1;
    }
    if (ov01_021F232C(arg0, arg1, arg2, v0) == 1) {
        return 1;
    }
    return 0;
}

LocalMapObject *ov01_021F3094(void *, s32);

s32 sub_02060BB8(LocalMapObject *, s32);

static s32 ov01_021F1E54(void *arg0, void *arg1, s32 arg2, s32 arg3) {
    LocalMapObject *object;

    if (arg3 != 1) {
        return 0;
    }
    object = ov01_021F3094(arg1, arg2);
    if (object == NULL) {
        return 0;
    }
    if (MapObject_GetSpriteID(object) != 0x54) {
        return 0;
    }
    if (sub_02060BB8(object, arg2) & ~1) {
        return 0;
    }
    ov01_021F1EA4(arg2, arg0, arg1, object);
    return 1;
}
