#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "filesystem.h"
#include "overlay_42.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

struct UnkStruct_ov63_0222BEC0_t;

struct UnkStruct_ov66_02234798_t;

void ov45_022308E4(UnkStruct_ov66_02234798 *param0, const VecFx32 *param1);

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

void ov49_02258800(const UnkStruct_ov44_02232914 *param0, VecFx32 *param1);

void ov45_022308C0(UnkStruct_ov66_02234798 *param0, const UnkStruct_ov44_02232914 *param1);

void ov45_022308C0(UnkStruct_ov66_02234798 *param0, const UnkStruct_ov44_02232914 *param1) {
    VecFx32 v0;

    ov49_02258800(param1, &v0);
    v0.z += (FX32_CONST(16));
    ov45_022308E4(param0, &v0);
}
