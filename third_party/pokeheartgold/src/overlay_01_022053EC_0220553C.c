#include "global.h"

#include "map_object.h"
#include "overlay_01.h"

s32 ov01_0220553C(LocalMapObject *mapObject);

s32 ov01_0220553C(LocalMapObject *mapObject) {
    u32 v0;

    v0 = MapObject_GetID(mapObject);
    if ((v0 != 0xFD) && (v0 != 0xFA) && (v0 != 0xFB)) {
        return 0;
    }
    return MapObject_GetParam(mapObject, 2U) & 1;
}

s32 ov01_02205564(LocalMapObject *arg0);

s32 ov01_02205564(LocalMapObject *arg0) {
    s32 v0;

    v0 = (s32)MapObject_GetSpriteID(arg0);
    if ((v0 >= 0x19F) && (v0 <= 0x1A4)) {
        return 1;
    }
    return 0;
}

u8 ov01_02205584(LocalMapObject *arg0);

u8 ov01_02205584(LocalMapObject *arg0) {
    u16 v0;

    v0 = (u16)MapObject_GetParam(arg0, 1U);
    if (MapObject_GetID(arg0) != 0xFD) {
        return 0U;
    }
    return (u8)(0xF & ((s32)(u8)v0 >> 4));
}

u8 ov01_022055B0(LocalMapObject *arg0);

u8 ov01_022055B0(LocalMapObject *arg0) {
    u16 v0;

    v0 = (u16)MapObject_GetParam(arg0, 1U);
    if (MapObject_GetID(arg0) != 0xFD) {
        return 0U;
    }
    return (u8)(0xF & (u8)v0);
}

u8 ov01_022055DC(LocalMapObject *object) {
    u16 v0;

#if defined(SDK_PORT) || defined(UBFIX)
    // ScrCmd_596 (and FollowMon_GetMapObject callers) pass NULL when no partner Pokemon walks (an empty party): the DS read the
    // object's fields at address 0 and the ID test below returned 0; a 64-bit host has no page there
    if (object == NULL) {
        return 0U;
    }
#endif
    v0 = (u16)MapObject_GetParam(object, 1U);
    if (MapObject_GetID(object) != 0xFD) {
        return 0U;
    }
    return (u8)(0xF & ((s32)v0 >> 8));
}
