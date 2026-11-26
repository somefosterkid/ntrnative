#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "billboard.h"
#include "overlay_01_021F944C.h"

void ov01_02205808(s32, LocalMapObject *, Billboard *);

void ov01_021F902C(s32 a0, LocalMapObject *a1);

void ov01_021F9048(LocalMapObject *arg0);

void ov01_021F902C(s32 a0, LocalMapObject *a1) {
    MapObjectBillboardWorkB *work;

    work = (MapObjectBillboardWorkB *)sub_0205F40C(a1);
    ov01_02205808(a0, a1, work->billboard);
    return;
}

void ov01_021F9048(LocalMapObject *object) {
    MapObjectBillboardWorkB *work;

    work = (MapObjectBillboardWorkB *)sub_0205F40C(object);
    work->flags = (u8)(work->flags & ~1);
}
