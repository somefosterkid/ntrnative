#include "global.h"

#include "map_object.h"

void ov01_021F8BE0(LocalMapObject *arg0, void *arg1, int arg2);

void ov01_021F8BE0(LocalMapObject *arg0, void *arg1, int arg2) {
    VecFx32 sp0 = { 0, 0, 0 };
    sp0.z += 0x800;
    MapObject_SetFacingVector(arg0, &sp0);
}
