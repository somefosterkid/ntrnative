#include "global.h"

#include "map_object.h"
#include "overlay_01_021F944C.h"

void ov01_021F73CC(LocalMapObject *arg0);

void ov01_021F73CC(LocalMapObject *object) {
    MapObjectBillboardWorkA *work;

    work = (MapObjectBillboardWorkA *)sub_0205F3E8(object, sizeof(MapObjectBillboardWorkA));
    work->unk_02 = -1;
    ov01_021F9510(object, &work->billboard);
    if (work->billboard != NULL) {
        sub_0205F484(object);
    }
}
