#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FF6B0.h"

Billboard *ov01_021F1740(FieldEffectManager *fieldEffMan, u32 id, const VecFx32 *position);

int ov01_021FF7B0(OverworldAnimManager *param0, UnkStruct_ov5_021F3FB8 *param1) {
    VecFx32 v0;
    UnkStruct_ov5_021F3FB8 *v1;
    const UnkStruct_ov5_021F3F10 *v2;

    v1 = param1;
    v2 = sub_02068D98(param0);

    v1->unk_10 = *v2;
    v1->unk_04 = MapObject_GetID(v1->unk_10.unk_0C);
    v1->unk_08 = MapObject_GetMapID(v1->unk_10.unk_0C);

    sub_02068DB8(param0, &v0);

    v1->unk_20 = ov01_021F1740(v1->unk_10.unk_04, 8, &v0);

    return 1;
}
