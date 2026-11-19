#include "global.h"

#include "overlay_01_021FD41C.h"
#include "unk_020689C8.h"

void sub_020699AC(Simple3DRenderObj *renderObj, VecFx32 *position, VecFx32 *scale, MtxFx33 *matrix);

void ov01_021FD7D4(OverworldAnimManager *param0, void *param1);

void ov01_021FD7D4(OverworldAnimManager *param0, void *param1) {
    UnkStruct_ov5_021F15B4 *v0 = param1;

    if (v0->unk_0C == 0) {
        VecFx32 v1, v2;
        MtxFx33 v3 = { FX32_ONE, 0, 0, 0, FX32_ONE, 0, 0, 0, FX32_ONE };
        Simple3DRenderObj *v4 = &v0->unk_14.unk_04->unk_78[0];

        ov01_021FD488(v0->unk_14.unk_04, &v2);
        sub_02068DB8(param0, &v1);

        v1.x += (FX32_ONE * -1) / 2;
        v1.y += (FX32_ONE * -4);
        v1.z += (FX32_ONE * 1);

        sub_020699AC(v4, &v1, &v2, &v3);
    }
}
