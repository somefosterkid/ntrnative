#include "global.h"

#include "map_object.h"
#include "overlay_01_021F944C.h"

void ov01_021F7434(LocalMapObject *arg0);

void ov01_021F7434(LocalMapObject *arg0) {
    MapObjectBillboardWorkA *work;

    work = (MapObjectBillboardWorkA *)sub_0205F40C(arg0);
    if (ov01_021FA2D4(arg0) != 1) {
        if (work->billboard == NULL) {
            ov01_021F9510(arg0, &work->billboard);
        }
        if (work->billboard != NULL) {
            ov01_021F9630(work->billboard, &work->animState);
            ov01_021FA3E8(arg0, work->billboard);
            MapObject_ClearFlagsBits(arg0, MAPOBJECTFLAG_UNK21);
        }
    }
}

void ov01_021F7478(LocalMapObject *arg0);

void ov01_021F7478(LocalMapObject *object) {
    MapObjectBillboardWorkB *work;

    work = (MapObjectBillboardWorkB *)sub_0205F3E8(object, sizeof(MapObjectBillboardWorkB));
    work->unk_10 = -1;
    ov01_021F9510(object, &work->billboard);
    work->unk_15 = 0;
    work->flags = (u8)(work->flags & ~2);
    work->flags = (u8)(work->flags & ~1);
    if (work->billboard != NULL) {
        sub_0205F484(object);
    }
}

void ov01_021F74B4(LocalMapObject *arg0);

void ov01_021F74B4(LocalMapObject *object) {
    ov01_021F95A8(object, &((MapObjectBillboardWorkB *)sub_0205F40C(object))->billboard);
}

void ov01_021F74C8(LocalMapObject *arg0);

void ov01_021F74C8(LocalMapObject *object) {
    Billboard *billboard;
    MapObjectBillboardWorkB *work;

    work = (MapObjectBillboardWorkB *)sub_0205F40C(object);
    billboard = work->billboard;
    if (billboard != NULL) {
        ov01_021F9610(billboard, &work->animState);
    }
    work->flags = (u8)(work->flags & ~2);
    work->flags = (u8)(work->flags & ~1);
    ov01_021F95A8(object, &work->billboard);
    MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK21);
}
