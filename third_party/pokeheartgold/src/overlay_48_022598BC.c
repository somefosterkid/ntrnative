#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_0200A090.h"

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
typedef struct {
    VecFx32 unk_00;
    VecFx32 unk_0C;
    VecFx32 unk_18;
    Field3dObject unk_24;
    Field3dModel unk_9C;
} UnkStruct_ov69_0225D71C;
void ov48_02258BF4(MtxFx33 *param0, const VecFx32 *param1);

void ov48_022598BC(const UnkStruct_ov69_0225D71C *param0, MtxFx33 *param1);
void ov48_022598CC(const UnkStruct_ov69_0225D71C *param0, VecFx32 *param1);
void ov48_022598DC(UnkStruct_ov69_0225D71C *param0, const VecFx32 *param1);

void ov48_022598BC(const UnkStruct_ov69_0225D71C *param0, MtxFx33 *param1) {
    ov48_02258BF4(param1, &param0->unk_18);
}

void ov48_022598CC(const UnkStruct_ov69_0225D71C *param0, VecFx32 *param1) {
    *param1 = param0->unk_18;
}

void ov48_022598DC(UnkStruct_ov69_0225D71C *param0, const VecFx32 *param1) {
    param0->unk_18 = *param1;
}
