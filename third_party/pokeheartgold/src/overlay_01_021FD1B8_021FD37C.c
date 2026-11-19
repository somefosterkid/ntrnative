#include "global.h"
#include "simple3dmodel.h"

#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FD2EC.h"
#include "sys_task.h"
#include "unk_020689C8.h"

void sub_020699BC(Simple3DRenderObj *renderObj, const VecFx32 *position);

void ov01_021FD37C(OverworldAnimManager *param_1, UnkStruct_021F1258 *param_2) {
    BOOL BVar1;
    LocalMapObject *object;
    VecFx32 VStack_1c;
    VecFx32 VStack_28;

    object = param_2->unk_1C.unk_08;
    BVar1 = sub_0205F0A8(object, param_2->unk_04, param_2->unk_08);
    if (BVar1 == 0) {
        ov01_021F1640(param_1);
        return;
    }
    MapObject_CopyPositionVector(object, &VStack_1c);
    MapObject_CopyFacingVector(object, &VStack_28);
    VStack_1c.x = VStack_1c.x + (VStack_28.x + param_2->unk_0C.x);
    VStack_1c.y = VStack_1c.y + (VStack_28.y + param_2->unk_0C.y);
    VStack_1c.z = VStack_1c.z + (VStack_28.z + param_2->unk_0C.z);
    sub_02068DA8(param_1, &VStack_1c);
    return;
}

void ov01_021FD3E0(OverworldAnimManager *param0, UnkStruct_021F1258 *v0) {
    VecFx32 v1;

    sub_02068DB8(param0, &v1);
    sub_020699BC(v0->unk_18, &v1);
}
