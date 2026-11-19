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

void ov45_02230E28(UnkStruct_ov66_02234798 *param0);

void ov45_02230978(UnkStruct_ov66_02234798 *param0, BOOL param1);

u32 ov45_02230994(u8 *arg0);

void ov45_02230978(UnkStruct_ov66_02234798 *param0, BOOL param1) {
    param0->unk_00_6 = param1;
    ov45_02230E28(param0);
}

u32 ov45_02230994(u8 *arg0) {
    return (u32)(*arg0 << 0x18) >> 0x1E;
}
