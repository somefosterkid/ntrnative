#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_022006A8.h"

void sub_02023E50(Billboard *billboard, const VecFx32 *pos);

void ov01_022007F8(OverworldAnimManager *manager, UnkStruct_ov5_021F3FB8 *work) {
    BOOL BVar1;
    LocalMapObject *object;
    VecFx32 VStack_1c;
    VecFx32 VStack_28;

    object = work->unk_10.unk_0C;
    BVar1 = sub_0205F0A8(object, work->unk_04, work->unk_08);
    if (BVar1 == 0) {
        ov01_021F1640(manager);
        return;
    }
    MapObject_CopyPositionVector(object, &VStack_1c);
    MapObject_CopyFacingVector(object, &VStack_28);
    VStack_1c.x = VStack_1c.x + VStack_28.x;
    VStack_1c.y = VStack_1c.y + VStack_28.y;
    VStack_1c.z = VStack_1c.z + VStack_28.z;
    VStack_1c.z += 0xc000;
    sub_02023E50(work->unk_20, &VStack_1c);
    return;
}
