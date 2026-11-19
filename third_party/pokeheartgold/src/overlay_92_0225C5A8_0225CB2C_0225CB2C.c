#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02005D10.h"
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

BOOL ov92_02260428(UnkStruct_ov116_0226501C *param0, int param1, int param2, int param3, int param4, f32 param5, BOOL param6);

BOOL ov92_02260628(UnkStruct_ov116_0226501C *param0, f32 param1);

extern s32 ov92_02263878;

extern s32 ov92_02263884;

void ov92_0225CB2C(UnkStruct_ov116_02262A8C *param0, u32 param1, u32 param2);

void ov92_0225CB2C(UnkStruct_ov116_02262A8C *param0, u32 param1, u32 param2) {
    s32 r7;
    f32 f;
    VecFx32 sp18;
    VecFx32 spC;
    u32 temp_r1;
    u32 temp_r1_2;
    u32 temp_r2;
    u32 temp_r2_2;

    if ((param1 == 0) && (param2 == 0)) {
        param0->unk_1FBC.unk_00 = 0;
        param0->unk_1FBC.unk_04 = 0;
        if (param0->unk_FC.unk_1F8 != 0) {
            ov92_02260628(&param0->unk_FC, 1.0f);
            sp18 = *(VecFx32 *)((u8 *)&ov92_02263884);
            MTX_MultVec43(&sp18, &param0->unk_308[0].unk_1B0, &param0->unk_1FB0);
            return;
        }
        param0->unk_FC.unk_1FC = 0.0f;
        param0->unk_FC.unk_200 = 0.0f;
        temp_r2 = param0->unk_1FBC.unk_04;
        temp_r1 = param0->unk_1FBC.unk_00;
        ov92_02260428(&param0->unk_FC, (s32)temp_r1, (s32)temp_r2, (s32)temp_r1, (s32)temp_r2, 1.88f, 0);
        temp_r2_2 = param0->unk_1FBC.unk_04;
        temp_r1_2 = param0->unk_1FBC.unk_00;
        ov92_02260428(param0->unk_308, (s32)temp_r1_2, (s32)temp_r2_2, (s32)temp_r1_2, (s32)temp_r2_2, 0.22f, 0);
        return;
    }
    r7 = *((s32 *)&param0->unk_1FB0.y) >> 12;
    f = (f32)((100 - r7) / 40);
    if (r7 > 95) {
        f = 1.0f;
    }
    if (param0->unk_1FBC.unk_00 == 0) {
        param0->unk_1FBC.unk_00 = param1;
        param0->unk_1FBC.unk_04 = param2;
    }
    ov92_02260428(&param0->unk_FC, (s32)param1, (s32)param2, (s32)param0->unk_1FBC.unk_00, (s32)param0->unk_1FBC.unk_04, 2.0f, 1);
    spC = *(VecFx32 *)((u8 *)&ov92_02263878);
    MTX_MultVec43(&spC, &param0->unk_308[0].unk_1B0, &param0->unk_1FB0);
    if (IsSEPlaying(0x58AU) == 0) {
        PlaySE(0x58AU);
    }
    param0->unk_1FBC.unk_00 = param1;
    param0->unk_1FBC.unk_04 = param2;
}
