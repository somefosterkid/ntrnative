#include "global.h"

#include "map_object.h"
#include "overlay_01_021F944C.h"

void ov01_021F7DA8(LocalMapObject *mapObj);

void ov01_021F7DA8(LocalMapObject *mapObj) {
    MapObjectBillboardWorkA *v0 = (MapObjectBillboardWorkA *)sub_0205F3E8(mapObj, sizeof(MapObjectBillboardWorkA));
    v0->unk_00 = -1;

    ov01_021F9510(mapObj, &v0->billboard);

    if (v0->billboard != NULL) {
        sub_0205F484(mapObj);
    }
}
