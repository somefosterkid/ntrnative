#include "global.h"

#include "map_object.h"
#include "overlay_01_021F944C.h"

void ov01_021F8F68(LocalMapObject *arg0, s8 arg1);

void ov01_021F8F68(LocalMapObject *object, s8 arg1) {
    ((MapObjectBillboardWorkB *)sub_0205F40C(object))->unk_14 = arg1;
}

void ov01_021F8F74(LocalMapObject *arg0, s32 arg1);

void ov01_021F8F74(LocalMapObject *object, s32 arg1) {
    MapObjectBillboardWorkB *work;

    work = (MapObjectBillboardWorkB *)sub_0205F40C(object);
    work->unk_14 = (s8)(work->unk_14 + arg1);
}

s32 ov01_021F8F88(LocalMapObject *arg0);

s32 ov01_021F8F88(LocalMapObject *object) {
    if (((u32)(((MapObjectBillboardWorkB *)sub_0205F40C(object))->flags << 0x1D) >> 0x1F) == 0) {
        return 1;
    }
    return 0;
}

void ov01_021F8FA0(LocalMapObject *arg0, void *arg1);

void ov01_021F8FA0(LocalMapObject *object, void *arg1) {
    MapObjectBillboardWorkB *work;
    VecFx32 *offset = arg1;

    work = (MapObjectBillboardWorkB *)sub_0205F40C(object);
    offset->x = (s32)(work->unk_12 << 0xC);
    offset->z = (s32)(work->unk_13 << 0xC);
    offset->y = 0;
}
