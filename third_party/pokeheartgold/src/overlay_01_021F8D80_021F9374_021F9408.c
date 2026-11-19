#include "global.h"

#include "map_object.h"
#include "overlay_01.h"

void ov01_021F9408(LocalMapObject *object, u32 dir) {
    MapObject_SetFacingDirection(object, dir);
    if (MapObject_CheckFlag14(object) == 1) {
        sub_0205F484(object);
    }
}

void ov01_021F9424(LocalMapObject *arg0);

void ov01_021F9424(LocalMapObject *arg0) {
    MapObject_SetVisible(arg0, 1);
    MapObject_SetFlagsBits(arg0, MAPOBJECTFLAG_UNK20);
}

void ov01_021F943C(void);

void ov01_021F943C(void) {
}

void ov01_021F9440(void);

void ov01_021F9440(void) {
}

void ov01_021F9444(void);

void ov01_021F9444(void) {
}

void ov01_021F9448(void);

void ov01_021F9448(void) {
}
