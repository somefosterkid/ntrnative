#include "global.h"

#include "math_util.h"
#include "screen_fade.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "overlay_92_unk_struct_ov116_02265d84.h"
#include "bg_window.h"
#include "camera.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "overlay_92_unk_struct_ov116_022649e4.h"
#include "overlay_92_unk_struct_ov116_02262a8c.h"

struct ManagedSprite;

struct NARC;

struct Sprite;

struct SpriteList;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SysTask;

struct SysTaskQueue;

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

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

int ov92_022619C4(int param0);

BOOL ov92_022620D0(UnkStruct_ov116_02265D84 *param0, int param1, int param2, int param3);

BOOL ov92_022620D0(UnkStruct_ov116_02265D84 *param0, int param1, int param2, int param3) {
    fx32 v1, v2;
    fx32 v3, v4;

    switch (param0->unk_04) {
    case 0:
        param0->unk_10 = 0;
        param0->unk_08 = 0;
        param0->unk_04++;
    case 1:
        param0->unk_08 += (360 / ov92_022619C4(param1));

        if (param0->unk_08 >= 360) {
            param0->unk_08 = 0;
        }

        ManagedSprite_GetPositionFxXYWithSubscreenOffset(param0->unk_00, &v3, &v4, ((192 + 64) << 12));

        if (param2 == 0) {
            v1 = (param0->unk_14 << 12) - (GF_SinDeg(param0->unk_08) * param0->unk_18);
            v2 = (param0->unk_16 << 12) - (GF_CosDeg(param0->unk_08) * param0->unk_1C);
        } else {
            v1 = (param0->unk_14 << 12) + (GF_SinDeg(param0->unk_08) * param0->unk_18);
            v2 = (param0->unk_16 << 12) - (GF_CosDeg(param0->unk_08) * param0->unk_1C);
        }

        ManagedSprite_SetPositionFxXYWithSubscreenOffset(param0->unk_00, v1, v2, ((192 + 64) << 12));

        if (param0->unk_08 == 0) {
            param0->unk_10++;

            if (param0->unk_10 >= param3) {
                param0->unk_04++;
                return 1;
            } else {
                param0->unk_04 = 1;
            }
        }
        break;
    default:
        return 1;
    }

    return 0;
}

struct BgConfig;

struct Camera;

struct G3DPipelineBuffers;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

BOOL ov92_02261E88(UnkStruct_ov116_02265D84 *param0, int param1, int param2, int param3);

BOOL ov92_02261F60(UnkStruct_ov116_02265D84 *param0, int param1, int param2, int param3);

BOOL ov92_02262018(UnkStruct_ov116_02265D84 *param0, int param1, int param2, int param3);

void ov92_02261E80(void *arg0);

void ov92_022621A4(SysTask *param0, void *param1);

typedef struct {
    BOOL a[4];
} ov92_02263E1C_Array;

extern const ov92_02263E1C_Array ov92_02263E1C;

void ov92_022621A4(SysTask *param0, void *param1) {
    int v0;
    UnkStruct_ov116_022660A8 *v1 = param1;
    UnkStruct_ov116_02265AA4 *v2 = &v1->unk_00;
    ov92_02263E1C_Array v3 = ov92_02263E1C;
    fx32 v4, v5;
    fx32 v6, v7;

    if ((IsPaletteFadeFinished() == FALSE) || (v1->unk_30->unk_2C.unk_00 == 1)) {
        SysTask_Destroy(param0);
        return;
    }

    switch (v1->unk_1C) {
    case 0:
        for (v0 = 0; v0 < v2->unk_140; v0++) {
            v2->unk_00[v0].unk_08 = 0;
            v2->unk_00[v0].unk_14 = 128;
            v2->unk_00[v0].unk_16 = (128 + 32);
            v2->unk_00[v0].unk_18 = 64;
            v2->unk_00[v0].unk_1C = 24;

            ManagedSprite_SetDrawFlag(v2->unk_00[v0].unk_00, 1);

            v2->unk_A0[v0].unk_08 = 0;
            v2->unk_A0[v0].unk_14 = 128;
            v2->unk_A0[v0].unk_16 = (128 + 32);
            v2->unk_A0[v0].unk_18 = 64;
            v2->unk_A0[v0].unk_1C = 24;

            ManagedSprite_SetDrawFlag(v2->unk_A0[v0].unk_00, 1);

            ManagedSprite_GetPositionFxXYWithSubscreenOffset(v2->unk_00[v0].unk_00, &v6, &v7, ((192 + 64) << FX32_SHIFT));
            v4 = (v2->unk_00[v0].unk_14 << FX32_SHIFT) + (GF_SinDeg(v2->unk_00[v0].unk_08) * v2->unk_00[v0].unk_18);
            v5 = (v2->unk_00[v0].unk_16 << FX32_SHIFT) + (GF_CosDeg(v2->unk_00[v0].unk_08) * v2->unk_00[v0].unk_1C);
            ManagedSprite_SetPositionFxXYWithSubscreenOffset(v2->unk_00[v0].unk_00, v4, v5, ((192 + 64) << FX32_SHIFT));

            ManagedSprite_GetPositionFxXYWithSubscreenOffset(v2->unk_A0[v0].unk_00, &v6, &v7, ((192 + 64) << FX32_SHIFT));
            v4 = (v2->unk_A0[v0].unk_14 << FX32_SHIFT) + (GF_SinDeg(v2->unk_A0[v0].unk_08) * v2->unk_A0[v0].unk_18);
            v5 = (v2->unk_A0[v0].unk_16 << FX32_SHIFT) - (GF_CosDeg(v2->unk_A0[v0].unk_08) * v2->unk_A0[v0].unk_1C);
            ManagedSprite_SetPositionFxXYWithSubscreenOffset(v2->unk_A0[v0].unk_00, v4, v5, ((192 + 64) << FX32_SHIFT));
        }

        ov92_02261E80(&v2->unk_A0[0]);
        ov92_02261E80(&v2->unk_A0[1]);
        ov92_02261E80(&v2->unk_00[0]);
        ov92_02261E80(&v2->unk_00[1]);
        ManagedSprite_SetDrawFlag(v2->unk_00[1].unk_00, 0);
        v1->unk_1C++;
    case 1:
        v3.a[0] = ov92_02261E88(&v2->unk_A0[0], v2->unk_144, 0, 3);
        v3.a[1] = ov92_02261F60(&v2->unk_A0[1], v2->unk_144, 0, 2);
        v3.a[2] = ov92_02262018(&v2->unk_00[0], v2->unk_148, 1, 3);
        v3.a[3] = 1;

        if (v3.a[0] && v3.a[1] && v3.a[2] && v3.a[3]) {
            ov92_02261E80(&v2->unk_A0[0]);
            ov92_02261E80(&v2->unk_A0[1]);
            ov92_02261E80(&v2->unk_00[0]);
            ov92_02261E80(&v2->unk_00[1]);
            ManagedSprite_SetDrawFlag(v2->unk_00[1].unk_00, 1);
            v1->unk_1C++;
        } else {
            break;
        }

    case 2:
        v3.a[0] = 1;
        v3.a[1] = ov92_022620D0(&v2->unk_A0[1], v2->unk_144, 0, 1);
        v3.a[2] = ov92_02261E88(&v2->unk_00[0], v2->unk_148, 1, 2);
        v3.a[3] = ov92_02262018(&v2->unk_00[1], v2->unk_148, 1, 3);

        if (v3.a[0] && v3.a[1] && v3.a[2] && v3.a[3]) {
            ov92_02261E80(&v2->unk_A0[0]);
            ov92_02261E80(&v2->unk_A0[1]);
            ov92_02261E80(&v2->unk_00[0]);
            ov92_02261E80(&v2->unk_00[1]);
            v1->unk_1C++;
        } else {
            break;
        }

    case 3:
        v3.a[0] = ov92_02261F60(&v2->unk_A0[0], v2->unk_144, 0, 2);
        v3.a[1] = ov92_02261E88(&v2->unk_A0[1], v2->unk_144, 0, 3);
        v3.a[2] = 1;
        v3.a[3] = ov92_02261E88(&v2->unk_00[1], v2->unk_148, 1, 2);

        if (v3.a[0] && v3.a[1] && v3.a[2] && v3.a[3]) {
            ov92_02261E80(&v2->unk_A0[0]);
            ov92_02261E80(&v2->unk_A0[1]);
            ov92_02261E80(&v2->unk_00[0]);
            ov92_02261E80(&v2->unk_00[1]);

            v1->unk_1C++;
        } else {
            break;
        }

    case 4:
        v3.a[0] = ov92_022620D0(&v2->unk_A0[0], v2->unk_144, 0, 1);
        v3.a[1] = 1;
        v3.a[2] = 1;
        v3.a[3] = ov92_02261E88(&v2->unk_00[1], v2->unk_148, 1, 2);

        if (v3.a[0] && v3.a[1] && v3.a[2] && v3.a[3]) {
            ov92_02261E80(&v2->unk_A0[0]);
            ov92_02261E80(&v2->unk_A0[1]);
            ov92_02261E80(&v2->unk_00[0]);
            ov92_02261E80(&v2->unk_00[1]);

            if (v2->unk_14C != 1) {
                v2->unk_14C++;
                v1->unk_1C = 1;
            } else {
                v2->unk_14C = 0;
                v1->unk_1C++;
            }
        }
        break;
    default:
        SysTask_Destroy(param0);
        break;
    }
}

void ov92_02262548(SysTask *param0, void *param1);

typedef struct {
    BOOL a[4];
} ov92_02263E0C_Array;

extern const ov92_02263E0C_Array ov92_02263E0C;

void ov92_02262548(SysTask *param0, void *param1) {
    int v0;
    UnkStruct_ov116_022660A8 *v1 = param1;
    UnkStruct_ov116_02265AA4 *v2 = &v1->unk_00;
    ov92_02263E0C_Array v3 = ov92_02263E0C;
    fx32 v4, v5;
    fx32 v6, v7;

    if ((IsPaletteFadeFinished() == FALSE) || (v1->unk_30->unk_2C.unk_00 == 1)) {
        SysTask_Destroy(param0);
        return;
    }

    switch (v1->unk_1C) {
    case 0:
        for (v0 = 0; v0 < v2->unk_140; v0++) {
            v2->unk_00[v0].unk_08 = 0;
            v2->unk_00[v0].unk_14 = 128;
            v2->unk_00[v0].unk_16 = (128 + 32);
            v2->unk_00[v0].unk_18 = 64;
            v2->unk_00[v0].unk_1C = 24;

            ManagedSprite_SetDrawFlag(v2->unk_00[v0].unk_00, 1);

            v2->unk_A0[v0].unk_08 = 0;
            v2->unk_A0[v0].unk_14 = 128;
            v2->unk_A0[v0].unk_16 = (128 + 32);
            v2->unk_A0[v0].unk_18 = 64;
            v2->unk_A0[v0].unk_1C = 24;

            ManagedSprite_SetDrawFlag(v2->unk_A0[v0].unk_00, 1);

            ManagedSprite_GetPositionFxXYWithSubscreenOffset(v2->unk_00[v0].unk_00, &v6, &v7, ((192 + 64) << FX32_SHIFT));
            v4 = (v2->unk_00[v0].unk_14 << FX32_SHIFT) + (GF_SinDeg(v2->unk_00[v0].unk_08) * v2->unk_00[v0].unk_18);
            v5 = (v2->unk_00[v0].unk_16 << FX32_SHIFT) + (GF_CosDeg(v2->unk_00[v0].unk_08) * v2->unk_00[v0].unk_1C);
            ManagedSprite_SetPositionFxXYWithSubscreenOffset(v2->unk_00[v0].unk_00, v4, v5, ((192 + 64) << FX32_SHIFT));

            ManagedSprite_GetPositionFxXYWithSubscreenOffset(v2->unk_A0[v0].unk_00, &v6, &v7, ((192 + 64) << FX32_SHIFT));
            v4 = (v2->unk_A0[v0].unk_14 << FX32_SHIFT) + (GF_SinDeg(v2->unk_A0[v0].unk_08) * v2->unk_A0[v0].unk_18);
            v5 = (v2->unk_A0[v0].unk_16 << FX32_SHIFT) - (GF_CosDeg(v2->unk_A0[v0].unk_08) * v2->unk_A0[v0].unk_1C);
            ManagedSprite_SetPositionFxXYWithSubscreenOffset(v2->unk_A0[v0].unk_00, v4, v5, ((192 + 64) << FX32_SHIFT));
        }

        ov92_02261E80(&v2->unk_A0[0]);
        ov92_02261E80(&v2->unk_A0[1]);
        ov92_02261E80(&v2->unk_00[0]);
        ov92_02261E80(&v2->unk_00[1]);
        ManagedSprite_SetDrawFlag(v2->unk_00[1].unk_00, 0);
        v1->unk_1C++;
    case 1:
        v3.a[0] = ov92_02261E88(&v2->unk_A0[0], v2->unk_144, 0, 3);
        v3.a[1] = ov92_02261F60(&v2->unk_A0[1], v2->unk_144, 0, 2);
        v3.a[2] = ov92_02262018(&v2->unk_00[0], v2->unk_148, 1, 3);
        v3.a[3] = 1;
        if (v3.a[0] && v3.a[1] && v3.a[2] && v3.a[3]) {
            ov92_02261E80(&v2->unk_A0[0]);
            ov92_02261E80(&v2->unk_A0[1]);
            ov92_02261E80(&v2->unk_00[0]);
            ov92_02261E80(&v2->unk_00[1]);
            ManagedSprite_SetDrawFlag(v2->unk_00[1].unk_00, 1);
            v1->unk_1C++;
        } else {
            break;
        }
    case 2:
        v3.a[0] = 1;
        v3.a[1] = ov92_022620D0(&v2->unk_A0[1], v2->unk_144, 0, 2);
        v3.a[2] = ov92_02261E88(&v2->unk_00[0], v2->unk_148, 1, 2);
        v3.a[3] = ov92_02262018(&v2->unk_00[1], v2->unk_148, 1, 3);
        if (v3.a[0] && v3.a[1] && v3.a[2] && v3.a[3]) {
            ov92_02261E80(&v2->unk_A0[0]);
            ov92_02261E80(&v2->unk_A0[1]);
            ov92_02261E80(&v2->unk_00[0]);
            ov92_02261E80(&v2->unk_00[1]);
            v1->unk_1C++;
        } else {
            break;
        }
    case 3:
        v3.a[0] = ov92_02261F60(&v2->unk_A0[0], v2->unk_144, 0, 2);
        v3.a[1] = ov92_02261E88(&v2->unk_A0[1], v2->unk_144, 0, 3);
        v3.a[2] = 1;
        v3.a[3] = ov92_02261E88(&v2->unk_00[1], v2->unk_148, 1, 2);
        if (v3.a[0] && v3.a[1] && v3.a[2] && v3.a[3]) {
            ov92_02261E80(&v2->unk_A0[0]);
            ov92_02261E80(&v2->unk_A0[1]);
            ov92_02261E80(&v2->unk_00[0]);
            ov92_02261E80(&v2->unk_00[1]);
            v1->unk_1C++;
        } else {
            break;
        }
    case 4:
        v3.a[0] = ov92_022620D0(&v2->unk_A0[0], v2->unk_144, 0, 2);
        v3.a[1] = 1;
        v3.a[2] = 1;
        v3.a[3] = ov92_02261E88(&v2->unk_00[1], v2->unk_148, 1, 2);
        if (v3.a[0] && v3.a[1] && v3.a[2] && v3.a[3]) {
            ov92_02261E80(&v2->unk_A0[0]);
            ov92_02261E80(&v2->unk_A0[1]);
            ov92_02261E80(&v2->unk_00[0]);
            ov92_02261E80(&v2->unk_00[1]);
            if (v2->unk_14C != 1) {
                v2->unk_14C++;
                v1->unk_1C = 1;
            } else {
                v2->unk_14C = 0;
                v1->unk_1C++;
            }
        }
        break;
    default:
        SysTask_Destroy(param0);
        break;
    }
}

void ov92_022628F0(SysTask *param0, void *param1);

typedef struct {
    BOOL a[4];
} ov92_02263E2C_Array;

extern const ov92_02263E2C_Array ov92_02263E2C;

void ov92_022628F0(SysTask *param0, void *param1) {
    int v0;
    UnkStruct_ov116_022660A8 *v1 = param1;
    UnkStruct_ov116_02265AA4 *v2 = &v1->unk_00;
    ov92_02263E2C_Array v3 = ov92_02263E2C;
    fx32 v4, v5;
    fx32 v6, v7;

    if ((IsPaletteFadeFinished() == FALSE) || (v1->unk_30->unk_2C.unk_00 == 1)) {
        SysTask_Destroy(param0);
        return;
    }

    switch (v1->unk_1C) {
    case 0:
        for (v0 = 0; v0 < v2->unk_140; v0++) {
            v2->unk_00[v0].unk_08 = 0;
            v2->unk_00[v0].unk_14 = 128;
            v2->unk_00[v0].unk_16 = (128 + 32);
            v2->unk_00[v0].unk_18 = 64;
            v2->unk_00[v0].unk_1C = 24;

            ManagedSprite_SetDrawFlag(v2->unk_00[v0].unk_00, 1);

            v2->unk_A0[v0].unk_08 = 0;
            v2->unk_A0[v0].unk_14 = 128;
            v2->unk_A0[v0].unk_16 = (128 + 32);
            v2->unk_A0[v0].unk_18 = 64;
            v2->unk_A0[v0].unk_1C = 24;

            ManagedSprite_SetDrawFlag(v2->unk_A0[v0].unk_00, 1);

            ManagedSprite_GetPositionFxXYWithSubscreenOffset(v2->unk_00[v0].unk_00, &v6, &v7, ((192 + 64) << FX32_SHIFT));
            v4 = (v2->unk_00[v0].unk_14 << FX32_SHIFT) + (GF_SinDeg(v2->unk_00[v0].unk_08) * v2->unk_00[v0].unk_18);
            v5 = (v2->unk_00[v0].unk_16 << FX32_SHIFT) + (GF_CosDeg(v2->unk_00[v0].unk_08) * v2->unk_00[v0].unk_1C);
            ManagedSprite_SetPositionFxXYWithSubscreenOffset(v2->unk_00[v0].unk_00, v4, v5, ((192 + 64) << FX32_SHIFT));

            ManagedSprite_GetPositionFxXYWithSubscreenOffset(v2->unk_A0[v0].unk_00, &v6, &v7, ((192 + 64) << FX32_SHIFT));
            v4 = (v2->unk_A0[v0].unk_14 << FX32_SHIFT) + (GF_SinDeg(v2->unk_A0[v0].unk_08) * v2->unk_A0[v0].unk_18);
            v5 = (v2->unk_A0[v0].unk_16 << FX32_SHIFT) - (GF_CosDeg(v2->unk_A0[v0].unk_08) * v2->unk_A0[v0].unk_1C);
            ManagedSprite_SetPositionFxXYWithSubscreenOffset(v2->unk_A0[v0].unk_00, v4, v5, ((192 + 64) << FX32_SHIFT));
        }

        ov92_02261E80(&v2->unk_A0[0]);
        ov92_02261E80(&v2->unk_A0[1]);
        ov92_02261E80(&v2->unk_00[0]);
        ov92_02261E80(&v2->unk_00[1]);
        ManagedSprite_SetDrawFlag(v2->unk_00[1].unk_00, 0);
        v1->unk_1C++;

    case 1:
        v3.a[0] = ov92_02261E88(&v2->unk_A0[0], v2->unk_144, 0, 3);
        v3.a[1] = ov92_02261F60(&v2->unk_A0[1], v2->unk_144, 0, 2);
        v3.a[2] = ov92_02262018(&v2->unk_00[0], v2->unk_148, 1, 3);
        v3.a[3] = 1;

        if (v3.a[0] && v3.a[1] && v3.a[2] && v3.a[3]) {
            ov92_02261E80(&v2->unk_A0[0]);
            ov92_02261E80(&v2->unk_A0[1]);
            ov92_02261E80(&v2->unk_00[0]);
            ov92_02261E80(&v2->unk_00[1]);
            ManagedSprite_SetDrawFlag(v2->unk_00[1].unk_00, 1);
            v1->unk_1C++;
        } else {
            break;
        }

    case 2:
        v3.a[0] = 1;
        v3.a[1] = ov92_022620D0(&v2->unk_A0[1], v2->unk_144, 0, 1);
        v3.a[2] = ov92_02261E88(&v2->unk_00[0], v2->unk_148, 1, 2);
        v3.a[3] = ov92_02262018(&v2->unk_00[1], v2->unk_148, 1, 3);

        if (v3.a[0] && v3.a[1] && v3.a[2] && v3.a[3]) {
            ov92_02261E80(&v2->unk_A0[0]);
            ov92_02261E80(&v2->unk_A0[1]);
            ov92_02261E80(&v2->unk_00[0]);
            ov92_02261E80(&v2->unk_00[1]);

            v1->unk_1C++;
        } else {
            break;
        }

    case 3:
        v3.a[1] = ov92_022620D0(&v2->unk_A0[1], v2->unk_144, 1, 1);
        if (v3.a[1]) {
            ov92_02261E80(&v2->unk_A0[1]);
            v1->unk_1C++;
        } else {
            break;
        }

    case 4:
        v3.a[0] = ov92_02261F60(&v2->unk_A0[0], v2->unk_144, 0, 2);
        v3.a[1] = ov92_02261E88(&v2->unk_A0[1], v2->unk_144, 0, 3);
        v3.a[2] = 1;
        v3.a[3] = ov92_02261E88(&v2->unk_00[1], v2->unk_148, 1, 2);

        if (v3.a[0] && v3.a[1] && v3.a[2] && v3.a[3]) {
            ov92_02261E80(&v2->unk_A0[0]);
            ov92_02261E80(&v2->unk_A0[1]);
            ov92_02261E80(&v2->unk_00[0]);
            ov92_02261E80(&v2->unk_00[1]);
            v1->unk_1C++;
        } else {
            break;
        }

    case 5:
        v3.a[0] = ov92_022620D0(&v2->unk_A0[0], v2->unk_144, 0, 1);
        v3.a[1] = 1;
        v3.a[2] = 1;
        v3.a[3] = ov92_02261E88(&v2->unk_00[1], v2->unk_148, 1, 2);

        if (v3.a[0] && v3.a[1] && v3.a[2] && v3.a[3]) {
            ov92_02261E80(&v2->unk_A0[0]);
            ov92_02261E80(&v2->unk_A0[1]);
            ov92_02261E80(&v2->unk_00[0]);
            ov92_02261E80(&v2->unk_00[1]);
            v1->unk_1C++;
        } else {
            break;
        }

    case 6:
        v3.a[0] = ov92_022620D0(&v2->unk_A0[0], v2->unk_144, 1, 1);

        if (v3.a[0]) {
            ov92_02261E80(&v2->unk_A0[0]);
            if (v2->unk_14C != 1) {
                v2->unk_14C++;
                v1->unk_1C = 1;
            } else {
                v2->unk_14C = 0;
                v1->unk_1C++;
            }
        }
        break;
    default:
        SysTask_Destroy(param0);
        break;
    }
}

void ov92_02262CEC(SysTask *param0, void *param1);

typedef struct {
    BOOL a[4];
} ov92_02263E4C_Array;

extern const ov92_02263E4C_Array ov92_02263E4C;

void ov92_02262CEC(SysTask *param0, void *param1) {
    int v0;
    UnkStruct_ov116_022660A8 *v1 = param1;
    UnkStruct_ov116_02265AA4 *v2 = &v1->unk_00;
    ov92_02263E4C_Array v3 = ov92_02263E4C;
    fx32 v4, v5;
    fx32 v6, v7;

    if ((IsPaletteFadeFinished() == FALSE) || (v1->unk_30->unk_2C.unk_00 == 1)) {
        SysTask_Destroy(param0);
        return;
    }

    switch (v1->unk_1C) {
    case 0:
        for (v0 = 0; v0 < v2->unk_140; v0++) {
            v2->unk_00[v0].unk_08 = 0;
            v2->unk_00[v0].unk_14 = 128;
            v2->unk_00[v0].unk_16 = (128 + 32);
            v2->unk_00[v0].unk_18 = 64;
            v2->unk_00[v0].unk_1C = 24;

            ManagedSprite_SetDrawFlag(v2->unk_00[v0].unk_00, 1);

            v2->unk_A0[v0].unk_08 = 0;
            v2->unk_A0[v0].unk_14 = 128;
            v2->unk_A0[v0].unk_16 = (128 + 32);
            v2->unk_A0[v0].unk_18 = 64;
            v2->unk_A0[v0].unk_1C = 24;

            ManagedSprite_SetDrawFlag(v2->unk_A0[v0].unk_00, 1);

            ManagedSprite_GetPositionFxXYWithSubscreenOffset(v2->unk_00[v0].unk_00, &v6, &v7, ((192 + 64) << FX32_SHIFT));
            v4 = (v2->unk_00[v0].unk_14 << FX32_SHIFT) + (GF_SinDeg(v2->unk_00[v0].unk_08) * v2->unk_00[v0].unk_18);
            v5 = (v2->unk_00[v0].unk_16 << FX32_SHIFT) + (GF_CosDeg(v2->unk_00[v0].unk_08) * v2->unk_00[v0].unk_1C);
            ManagedSprite_SetPositionFxXYWithSubscreenOffset(v2->unk_00[v0].unk_00, v4, v5, ((192 + 64) << FX32_SHIFT));

            ManagedSprite_GetPositionFxXYWithSubscreenOffset(v2->unk_A0[v0].unk_00, &v6, &v7, ((192 + 64) << FX32_SHIFT));
            v4 = (v2->unk_A0[v0].unk_14 << FX32_SHIFT) + (GF_SinDeg(v2->unk_A0[v0].unk_08) * v2->unk_A0[v0].unk_18);
            v5 = (v2->unk_A0[v0].unk_16 << FX32_SHIFT) - (GF_CosDeg(v2->unk_A0[v0].unk_08) * v2->unk_A0[v0].unk_1C);
            ManagedSprite_SetPositionFxXYWithSubscreenOffset(v2->unk_A0[v0].unk_00, v4, v5, ((192 + 64) << FX32_SHIFT));
        }

        ov92_02261E80(&v2->unk_A0[0]);
        ov92_02261E80(&v2->unk_A0[1]);
        ov92_02261E80(&v2->unk_00[0]);
        ov92_02261E80(&v2->unk_00[1]);
        ManagedSprite_SetDrawFlag(v2->unk_00[1].unk_00, 0);
        v1->unk_1C++;
    case 1:
        v3.a[0] = ov92_02261E88(&v2->unk_A0[0], v2->unk_144, 0, 3);
        v3.a[1] = ov92_02261F60(&v2->unk_A0[1], v2->unk_144, 0, 2);
        v3.a[2] = ov92_02262018(&v2->unk_00[0], v2->unk_148, 1, 3);
        v3.a[3] = 1;

        if (v3.a[0] && v3.a[1] && v3.a[2] && v3.a[3]) {
            ov92_02261E80(&v2->unk_A0[0]);
            ov92_02261E80(&v2->unk_A0[1]);
            ov92_02261E80(&v2->unk_00[0]);
            ov92_02261E80(&v2->unk_00[1]);
            ManagedSprite_SetDrawFlag(v2->unk_00[1].unk_00, 1);
            v1->unk_1C++;
        } else {
            break;
        }

    case 2:
        v3.a[0] = 1;
        v3.a[1] = ov92_022620D0(&v2->unk_A0[1], v2->unk_144, 0, 1);
        v3.a[2] = ov92_02261E88(&v2->unk_00[0], v2->unk_148, 1, 2);
        v3.a[3] = ov92_02262018(&v2->unk_00[1], v2->unk_148, 1, 3);

        if (v3.a[0] && v3.a[1] && v3.a[2] && v3.a[3]) {
            ov92_02261E80(&v2->unk_A0[0]);
            ov92_02261E80(&v2->unk_A0[1]);
            ov92_02261E80(&v2->unk_00[0]);
            ov92_02261E80(&v2->unk_00[1]);

            v1->unk_1C++;
        } else {
            break;
        }

    case 3:
        v3.a[1] = ov92_022620D0(&v2->unk_A0[1], v2->unk_144, 1, 1);

        if (v3.a[1]) {
            ov92_02261E80(&v2->unk_A0[1]);
            v1->unk_1C++;
        } else {
            break;
        }

    case 4:
        v3.a[0] = ov92_02261F60(&v2->unk_A0[0], v2->unk_144, 0, 2);
        v3.a[1] = ov92_02261E88(&v2->unk_A0[1], v2->unk_144, 0, 3);
        v3.a[2] = 1;
        v3.a[3] = ov92_02261E88(&v2->unk_00[1], v2->unk_148, 1, 2);

        if (v3.a[0] && v3.a[1] && v3.a[2] && v3.a[3]) {
            ov92_02261E80(&v2->unk_A0[0]);
            ov92_02261E80(&v2->unk_A0[1]);
            ov92_02261E80(&v2->unk_00[0]);
            ov92_02261E80(&v2->unk_00[1]);

            v1->unk_1C++;
        } else {
            break;
        }

    case 5:
        v3.a[0] = ov92_022620D0(&v2->unk_A0[0], v2->unk_144, 0, 1);
        v3.a[1] = 1;
        v3.a[2] = 1;
        v3.a[3] = ov92_02261E88(&v2->unk_00[1], v2->unk_148, 1, 2);

        if (v3.a[0] && v3.a[1] && v3.a[2] && v3.a[3]) {
            ov92_02261E80(&v2->unk_A0[0]);
            ov92_02261E80(&v2->unk_A0[1]);
            ov92_02261E80(&v2->unk_00[0]);
            ov92_02261E80(&v2->unk_00[1]);

            v1->unk_1C++;
        } else {
            break;
        }

    case 6:
        v3.a[0] = ov92_022620D0(&v2->unk_A0[0], v2->unk_144, 1, 1);

        if (v3.a[0]) {
            ov92_02261E80(&v2->unk_A0[0]);

            if (v2->unk_14C != 8) {
                v2->unk_14C++;
                v1->unk_1C = 1;
            } else {
                v2->unk_14C = 0;
                v1->unk_1C++;
            }
        }

        break;
    default:
        SysTask_Destroy(param0);
        break;
    }
}
