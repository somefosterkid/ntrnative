#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "gf_3d_vramman.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02005D10.h"
#include "overlay_92_unk_struct_ov116_02262d08.h"
#include "overlay_92_unk_struct_ov116_02265d84.h"
#include "overlay_92_unk_struct_ov116_022649e4.h"
#include "overlay_92_unk_struct_ov116_02262a8c.h"

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

void ov92_0225F8B0(UnkStruct_ov116_02262A8C *param0);

s32 ov92_0225EE60(UnkStruct_ov116_02262A8C *, u32);

s32 ov92_0225F8EC(UnkStruct_ov116_02262A8C *);

s32 ov92_0225F9C4(UnkStruct_ov116_02262A8C *);

s32 ov92_0225FAB8(UnkStruct_ov116_02262A8C *);

s32 ov92_0225FEE4(UnkStruct_ov116_02262A8C *);

void ov92_0225FC2C(UnkStruct_ov116_02262A8C *param0);

void ov92_0225FC2C(UnkStruct_ov116_02262A8C *param0) {
    if ((s32)param0->unk_279C.unk_04 < 0x4B0) {
        ov92_0225FAB8(param0);
        ov92_0225F9C4(param0);
        if (ov92_0225F8EC(param0) == 1) {
            ov92_0225EE60(param0, param0->unk_00);
            ov92_0225F8B0(param0);
            PlaySE(0x58CU);
        }
        ov92_0225FEE4(param0);
        param0->unk_279C.unk_38 += 1;
        param0->unk_279C.unk_04 += 1;
        param0->unk_279C.unk_1E += 1;
    }
}
