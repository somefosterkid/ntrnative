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

void ov45_0223093C(UnkStruct_ov66_02234798 *param0, int param1);

void ov45_02230968(void *arg0);
void ov45_02230974(void *arg0, s8 arg1);

void ov45_0223093C(UnkStruct_ov66_02234798 *param0, int param1) {
    param0->unk_01_0 = 1;
    param0->unk_01_1 = param1;
    param0->unk_02 = 0;
    param0->unk_03 = 1;
}

void ov45_02230968(void *arg0) {
    *(u8 *)((u8 *)arg0 + 1) = (u8)(*(u8 *)((u8 *)arg0 + 1) & ~1);
}

void ov45_02230974(void *arg0, s8 arg1) {
    *(s8 *)((u8 *)arg0 + 3) = arg1;
}
