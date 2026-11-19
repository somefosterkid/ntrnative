#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "sys_task_api.h"
#include "field/overlay_01_021FB878.h"
#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_92_unk_struct_ov116_02262d08.h"
#include "overlay_92_unk_struct_ov116_02265d84.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "overlay_92_unk_struct_ov116_02262cb8.h"
#include "overlay_92_unk_struct_ov116_02262a8c.h"

void ov92_0225F254(SysTask *param0, void *param1);

void ov92_0225EDB4(UnkStruct_ov116_02262A8C *param0, int param1);

struct G3DPipelineBuffers;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);
static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);
void ov92_0225F338(SysTask *param0, void *param1);
void ov92_0225F530(SysTask *param0, void *param1);
void ov92_0225EE04(UnkStruct_ov116_02262A8C *param0, int param1);
void ov92_0225EE60(UnkStruct_ov116_02262A8C *param0, int param1);

void sub_02018198(Field3DModelAnimation *anim, fx32 frame);
void ov92_0225EF4C(SysTask *param0, void *param1);
void ov92_0225DD48(UnkStruct_ov116_02262A8C *param0, int param1);
typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int *unk_0C;
    UnkStruct_ov116_0226501C *unk_10;
    UnkStruct_ov116_0226501C *unk_14;
    u8 padding_18[8];
    VecFx32 unk_20;
    u8 padding_2C[24];
    UnkStruct_ov116_0226469C unk_44;
    UnkStruct_ov116_0226469C unk_5C;
    int unk_74;
    UnkStruct_ov116_022649E4 *unk_78;
} UnkStruct_ov116_02262DC0;
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);
static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);
void ov92_0225EEBC(UnkStruct_ov116_02262A8C *param0);

void ov92_0225EDB4(UnkStruct_ov116_02262A8C *param0, int param1) {
    UnkStruct_ov116_02262CB8 *data;

    data = Heap_Alloc(HEAP_ID_113, sizeof(UnkStruct_ov116_02262CB8));
    memset(data, 0, sizeof(UnkStruct_ov116_02262CB8));
    data->unk_0C = param0->unk_1C[param0->unk_DC[param1]];
    data->unk_10 = param0->unk_20[param0->unk_DC[param1]];
    data->unk_00 = 0;
    data->unk_04 = 0;
    data->unk_2C = param0->unk_14;
    SysTask_CreateOnMainQueue(ov92_0225F254, data, 0x1000);
    return;
}

static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

void ov92_0225EE04(UnkStruct_ov116_02262A8C *param0, int param1) {
    UnkStruct_ov116_02262D08 *v0 = &param0->unk_268C[param1];

    if (v0->unk_30 == 1) {
        return;
    }

    v0->unk_40 = param0->unk_2870;
    v0->unk_30 = 1;
    v0->unk_2C = &param0->unk_279C.unk_28[param1];
    v0->unk_28 = *v0->unk_2C;
    v0->unk_0C = param0->unk_24[param0->unk_DC[param1]];
    v0->unk_00 = 0;
    v0->unk_04 = 0;
    v0->unk_3C = param0->unk_14;

    SysTask_CreateOnMainQueue(ov92_0225F338, v0, 4096);
}

void ov92_0225EE60(UnkStruct_ov116_02262A8C *param0, int param1) {
    UnkStruct_ov116_02262D08 *v0 = &param0->unk_268C[param1];

    if (v0->unk_30 == 1) {
        return;
    }

    v0->unk_40 = param0->unk_2870;
    v0->unk_30 = 1;
    v0->unk_2C = &param0->unk_279C.unk_0C;
    v0->unk_28 = *v0->unk_2C;
    v0->unk_34 = &param0->unk_1B98;
    v0->unk_38 = &param0->unk_1DA4;
    v0->unk_00 = 0;
    v0->unk_04 = 0;
    v0->unk_3C = param0->unk_14;

    SysTask_CreateOnMainQueue(ov92_0225F530, v0, 4096);
}

void ov92_0225EEBC(UnkStruct_ov116_02262A8C *param0) {
    UnkStruct_ov116_02262DC0 *v0 = Heap_Alloc(HEAP_ID_113, sizeof(UnkStruct_ov116_02262DC0));

    memset(v0, 0, sizeof(UnkStruct_ov116_02262DC0));

    v0->unk_0C = &param0->unk_F8;
    v0->unk_10 = &param0->unk_308[0];
    v0->unk_14 = &param0->unk_FC;
    v0->unk_20 = param0->unk_1FB0;
    v0->unk_74 = param0->unk_2870;
    v0->unk_78 = param0->unk_14;

    ov92_0225DD48(param0, 0);

    sub_02018198(&param0->unk_308[0].unk_88[0], 0);
    sub_02018198(&param0->unk_308[1].unk_88[0], 0);
    sub_02018198(&param0->unk_308[2].unk_88[0], 0);
    SysTask_CreateOnMainQueue(ov92_0225EF4C, v0, 4096);
}
