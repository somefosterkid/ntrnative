#include "global.h"

#include "map_object.h"
#include "overlay_01_021F944C.h"

void ov01_021F9140(LocalMapObject *arg0);

void sub_02023EE0(Billboard *, u32);
void sub_02023F04(Billboard *, u32);
void sub_02023F40(Billboard *, u32);
void ov01_021F9154(void *param_1, Billboard *billboard, MapObjectBillboardWorkC *work);
void ov01_021F917C(void *param_1, Billboard *billboard, MapObjectBillboardWorkC *work);

void ov01_021F9140(LocalMapObject *object) {
    MapObjectBillboardWorkC *work;

    work = (MapObjectBillboardWorkC *)sub_0205F3E8(object, sizeof(MapObjectBillboardWorkC));
    ov01_021F9510(object, &work->billboard);
}

void ov01_021F9154(void *param_1, Billboard *billboard, MapObjectBillboardWorkC *work) {
    if (work->unk_10 != 0) {
        sub_02023EE0(billboard, 0);
        sub_02023F40(billboard, 0);
    }
    sub_02023F04(billboard, 0x1000);
    return;
}

void ov01_021F917C(void *param_1, Billboard *billboard, MapObjectBillboardWorkC *work) {
    if (work->unk_10 != 1) {
        sub_02023EE0(billboard, 1);
        sub_02023F40(billboard, 0);
    }
    sub_02023F04(billboard, 0x1000);
    return;
}
