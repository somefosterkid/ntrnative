#include "global.h"

#include "overlay_01_021FD41C.h"
#include "unk_020689C8.h"

int sub_02068D90(const OverworldAnimManager *manager);

void sub_020699BC(Simple3DRenderObj *renderObj, const VecFx32 *position);

void ov01_021FD980(OverworldAnimManager *param0, void *param1);

void ov01_021FD980(OverworldAnimManager *param0, void *param1) {
    UnkStruct_ov5_021F15B4 *v0 = param1;

    if (v0->unk_0C == 1) {
        return;
    }

    {
        int v1 = sub_02068D90(param0);
        VecFx32 v2;
        Simple3DRenderObj *v3 = &v0->unk_14.unk_04->unk_78[v1];

        sub_02068DB8(param0, &v2);
        v2.x += (FX32_ONE * -1) / 2;
        v2.y += (FX32_ONE * -4);
        v2.z += (FX32_ONE * 1);
        sub_020699BC(v3, &v2);
    }
}

void ov01_021FD9CC(u32 arg0, VecFx32 *pos) {

    switch (arg0) {
    case 0:
        pos->z = pos->z + 0x2000;
        return;
    case 1:
        pos->z = pos->z - 0x2000;
        return;
    case 2:
        pos->x = pos->x + 0x6000;
        return;
    case 3:
        pos->x = pos->x - 0x6000;
    default:
        return;
    }
}
