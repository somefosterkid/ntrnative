#include "global.h"

#include "overlay_01_021F1348.h"
#include "overlay_01_simple_renderers.h"
#include "overlay_01_021FEC38.h"
#include "unk_020689C8.h"

BOOL sub_020698E8(Simple3DAnimation *anim, fx32 frameDelta, BOOL loop);
BOOL sub_02069948(const Simple3DAnimation *anim);
void sub_020699BC(Simple3DRenderObj *renderObj, const VecFx32 *position);

void ov01_021FED58(OverworldAnimManager *a0, UnkStruct_ov5_021F2C98 *arg1) {
    if (sub_02069948(&arg1->unk_00) == 1) {
        ov01_021F1640(a0);
        return;
    }
    sub_020698E8(&arg1->unk_00, 0x1000, 0);
}

void ov01_021FED80(void *a0, UnkStruct_ov5_021F2C98 *arg1) {
    VecFx32 a1;

    sub_02068DB8(a0, &a1);
    sub_020699BC(&arg1->unk_24, &a1);
}
