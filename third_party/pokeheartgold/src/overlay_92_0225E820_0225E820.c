#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_92_unk_struct_ov116_02262d08.h"
#include "overlay_92_unk_struct_ov116_02265d84.h"
#include "field/overlay_01_021FB878.h"
#include "overlay_92_unk_struct_ov116_022649e4.h"
#include "overlay_92_unk_struct_ov116_02262a8c.h"

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
};

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

s32 ov92_0225DDD8(ManagedSpriteTemplate *, s16, s16, s32, s32, s32);

void ov92_0225E820(UnkStruct_ov116_02262A8C *param0);

void ov92_0225E820(UnkStruct_ov116_02262A8C *param0) {
    ManagedSpriteTemplate spC;
    s32 sp8;
    ManagedSpriteTemplate *var_r7;
    SpriteSystem *var_r6;
    SpriteManager *var_r4;
    s32 temp_r0_2;
    UnkStruct_ov116_02262A8C *var_r5;
    UnkStruct_ov116_022649E4 *temp_r0;

    var_r5 = param0;
    temp_r0 = var_r5->unk_14;
    var_r7 = &spC;
    var_r6 = temp_r0->unk_08;
    var_r4 = temp_r0->unk_0C;
    sp8 = 0;
    do {
        ov92_0225DDD8(var_r7, 0, 0, 1, 4, 0x232A);
        spC.bgPriority = 2;
        spC.drawPriority = 0x14;
        var_r5->unk_D0[0] = SpriteSystem_NewSprite(var_r6, var_r4, var_r7);
        ManagedSprite_SetAnim(var_r5->unk_D0[0], 0);
        ManagedSprite_TickFrame(var_r5->unk_D0[0]);
        ManagedSprite_SetPaletteOverrideOffset(var_r5->unk_D0[0], 4U);
        ManagedSprite_SetOamMode(var_r5->unk_D0[0], GX_OAM_MODE_XLU);
        ManagedSprite_SetDrawFlag(var_r5->unk_D0[0], 0);
        ov92_0225DDD8(var_r7, 0, 0, 2, 0, 0x2328);
        spC.bgPriority = 2;
        spC.drawPriority = 0x14;
        var_r5->unk_C8[0] = SpriteSystem_NewSprite(var_r6, var_r4, var_r7);
        ManagedSprite_SetAnim(var_r5->unk_C8[0], 6);
        ManagedSprite_TickFrame(var_r5->unk_C8[0]);
        ManagedSprite_SetPaletteOverrideOffset(var_r5->unk_C8[0], 0U);
        ManagedSprite_SetOamMode(var_r5->unk_C8[0], GX_OAM_MODE_XLU);
        ManagedSprite_SetDrawFlag(var_r5->unk_C8[0], 0);
        var_r5 = (void *)((u8 *)var_r5 + (4));
        temp_r0_2 = sp8 + 1;
        sp8 = temp_r0_2;
    } while (temp_r0_2 < 2);
}
