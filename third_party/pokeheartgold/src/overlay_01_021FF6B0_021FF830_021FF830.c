#include "global.h"

#include "overlay_01_021FF6B0.h"
#include "unk_020689C8.h"

void sub_02023E50(Billboard *billboard, const VecFx32 *pos);

void ov01_021FF830(OverworldAnimManager *manager, UnkStruct_ov5_021F3FB8 *work) {
    VecFx32 a1;

    sub_02068DB8(manager, &a1);
    a1.z += 0x8000;
    sub_02023E50(work->unk_20, &a1);
}
