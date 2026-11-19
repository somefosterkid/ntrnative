#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FEA0C.h"
#include "overworld_anim_manager.h"

void ov01_021FEB8C(OverworldAnimManager *param0, UnkStruct_ov5_021F2974 *v0) {
    LocalMapObject *v1 = v0->unk_10.unk_10;

    if (v0->unk_0C == 0) {
        return;
    }

    {
        VecFx32 v2 = { 0, 0, 0 };
        sub_0205F9A0(v1, &v2);
    }

    {
        VecFx32 v3;

        MapObject_CopyPositionVector(v1, &v3);
        sub_02068DA8(param0, &v3);
    }
}
