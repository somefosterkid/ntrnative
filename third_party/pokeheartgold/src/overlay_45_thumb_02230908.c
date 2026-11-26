#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "filesystem.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

struct UnkStruct_ov63_0222BEC0_t;

struct UnkStruct_ov66_02234798_t;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};


void ov45_02230908(const UnkStruct_ov66_02234798 *param0, VecFx32 *param1);

s32 ov45_022309C4(s32 arg0, s32 arg1);
s32 sub_02023EE0(Billboard *, s32);
s32 sub_02023F40(Billboard *, s32);
void ov45_02230920(void *arg0, s32 arg1);

void ov45_02230908(const UnkStruct_ov66_02234798 *param0, VecFx32 *param1) {
    const VecFx32 *v0 = sub_02023E68(param0->unk_08);
    *param1 = *v0;
}

void ov45_02230920(void *arg0, s32 arg1) {
    sub_02023EE0(*(void **)((u8 *)arg0 + 8), ov45_022309C4(1, arg1));
    sub_02023F40(*(void **)((u8 *)arg0 + 8), 0);
}
