#include "global.h"

#include "overlay_92.h"
#include "constants/heap.h"
#include "field/overlay_01_021FB878.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "palette.h"
#include "sys_task.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

s32 sub_02018030(s32, s32, s32, s32);

typedef union {
    struct {
        f32 val1_00;
        f32 val1_04;
        f32 val1_08;
        f32 val1_0C;
    };
    f32 val2[4];
} UnkUnion_ov116_02266FEC;
typedef union {
    struct {
        f32 val1_00;
        f32 val1_04;
        f32 val1_08;
        f32 val1_0C;
        f32 val1_10;
        f32 val1_14;
        f32 val1_18;
        f32 val1_1C;
        f32 val1_20;
        f32 val1_24;
        f32 val1_28;
        f32 val1_2C;
        f32 val1_30;
        f32 val1_34;
        f32 val1_38;
        f32 val1_3C;
    };
    f32 val2[4][4];
    f32 val3[16];
} UnkUnion_ov116_022671EC;
enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
void sub_020181B0(Field3dObject *obj, Field3dModel *model);
void sub_020182A0(Field3dObject *obj, BOOL visible);
void sub_020182A8(Field3dObject *obj, fx32 x, fx32 y, fx32 z);
void sub_020182C4(Field3dObject *obj, fx32 x, fx32 y, fx32 z);
typedef struct {
    Field3dObject unk_00;
    Field3dModel unk_78;
    Field3DModelAnimation unk_88[10];
    UnkUnion_ov116_022671EC unk_150;
    UnkUnion_ov116_02266FEC unk_190;
    UnkUnion_ov116_02266FEC unk_1A0;
    MtxFx43 unk_1B0;
    VecFx32 unk_1E0;
    BOOL unk_1EC;
    BOOL unk_1F0;
    int unk_1F4;
    int unk_1F8;
    f32 unk_1FC;
    f32 unk_200;
    f32 unk_204;
    f32 unk_208;
} UnkStruct_ov116_0226501C;
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);
static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);
void sub_02018068(Field3dModel *model);
void ov92_0225D9B4(UnkStruct_ov116_0226501C *param0, int param1);
void ov92_0225DA2C(UnkStruct_ov116_0226501C *param0);

void ov92_0225D9A8(s32 arg0, s32 arg1, s32 arg2) {
    sub_02018030(arg0 + 0x78, arg1, arg2, 0x71);
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

void ov92_0225D9B4(UnkStruct_ov116_0226501C *param0, int param1) {
    sub_020181B0(&param0->unk_00, &param0->unk_78);
    sub_020182A8(&param0->unk_00, 0 * ((fx32)0x0000000000001000L), -20 * ((fx32)0x0000000000001000L), 0 * ((fx32)0x0000000000001000L));

    if (param1 == 0) {
        sub_020182C4(&param0->unk_00, ((fx32)(((1.00f) > 0) ? ((1.00f) * (1 << 12) + 0.5f) : ((1.00f) * (1 << 12) - 0.5f))), ((fx32)(((1.00f) > 0) ? ((1.00f) * (1 << 12) + 0.5f) : ((1.00f) * (1 << 12) - 0.5f))), ((fx32)(((1.00f) > 0) ? ((1.00f) * (1 << 12) + 0.5f) : ((1.00f) * (1 << 12) - 0.5f))));
    } else {
        sub_020182C4(&param0->unk_00, ((fx32)(((1.50f) > 0) ? ((1.50f) * (1 << 12) + 0.5f) : ((1.50f) * (1 << 12) - 0.5f))), ((fx32)(((1.50f) > 0) ? ((1.50f) * (1 << 12) + 0.5f) : ((1.50f) * (1 << 12) - 0.5f))), ((fx32)(((1.50f) > 0) ? ((1.50f) * (1 << 12) + 0.5f) : ((1.50f) * (1 << 12) - 0.5f))));
    }

    sub_020182A0(&param0->unk_00, 1);

    param0->unk_1E0.x = (0 * ((fx32)0x0000000000001000L));
    param0->unk_1E0.y = (-20 * ((fx32)0x0000000000001000L));
    param0->unk_1E0.z = (0 * ((fx32)0x0000000000001000L));
    param0->unk_1FC = 0.0f;
    param0->unk_200 = 0.0f;
    param0->unk_1EC = 0;
    param0->unk_1F0 = 1;
}

void ov92_0225DA2C(UnkStruct_ov116_0226501C *param0) {
    param0->unk_1F0 = 0;
    sub_02018068(&param0->unk_78);
}
