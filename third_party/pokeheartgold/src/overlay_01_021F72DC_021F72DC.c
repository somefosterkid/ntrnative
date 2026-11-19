#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"
#include "sprite.h"

void *sub_02064084(LocalMapObject *);

Sprite *ov01_021F72DC(LocalMapObject *obj);

Sprite *ov01_021F72DC(LocalMapObject *obj) {
    s32 id;
    u8 *p;

    // The billboard slot sits after the work area's four bytes, so it is 4 bytes in on the DS and 8 on the host.
    id = MapObject_GetSpriteID(obj);
    switch (id) {
    case 0:
    case 0x15:
    case 0x61:
    case 0x62:
    case 0xb0:
    case 0xb1:
    case 0xb2:
    case 0xb3:
    case 0xb4:
    case 0xb5:
    case 0xbc:
    case 0xbd:
    case 0xc4:
    case 0xc5:
    case 0xc6:
    case 0xc7:
    case 0xc8:
    case 0xc9:
    case 0xf8:
    case 0xf9:
    case 0x102:
    case 0x103:
    case 0x104:
    case 0x105:
        p = sub_0205F40C(obj);
        return (Sprite *)((MapObjectBillboardWorkA *)p)->billboard;
    }
    if (id >= 0x1AC && id <= 0x3E1) {
        p = sub_0205F40C(obj);
        return (Sprite *)((MapObjectBillboardWorkB *)p)->billboard;
    }
    if (id >= 0x106 && id <= 0x10D) {
        return (Sprite *)sub_02064084(obj);
    }
    p = sub_0205F40C(obj);
    return (Sprite *)((MapObjectBillboardWorkA *)p)->billboard;
}
