#include "global.h"

#include "heap.h"
#include "math_util.h"
#include "screen_fade.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "bg_window.h"
#include "camera.h"
#include "overlay_92_unk_struct_ov116_02262d08.h"
#include "overlay_92_unk_struct_ov116_02265d84.h"
#include "field/overlay_01_021FB878.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "overlay_92_unk_struct_ov116_022649e4.h"
#include "overlay_92_unk_struct_ov116_02262a8c.h"
#include "overlay_92_unk_struct_ov116_02264fec.h"

struct BgConfig;

struct Camera;

struct Field3DModelAnimation;

struct Field3dModel;

struct Field3dObject;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct NARC;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

struct Window;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

void ov92_0225DF0C(UnkStruct_ov116_02262A8C *param0, BOOL param1, int param2);

void ov92_0225DF28(UnkStruct_ov116_02262A8C *param0);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

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

static inline void inline_ov116_022649E4(UnkStruct_ov116_02264FEC_sub1 *param0);

static inline void inline_ov116_022649E4_1(UnkStruct_ov116_02264FEC_sub1 *wk, fx32 x, fx32 y);

static inline void inline_ov116_022649E4(UnkStruct_ov116_02264FEC_sub1 *param0) {
    param0->unk_1C = FX_MulInline(GF_SinDeg(param0->unk_28), ((fx32)(((param0->unk_2E) > 0) ? ((param0->unk_2E) * (1 << 12) + 0.5f) : ((param0->unk_2E) * (1 << 12) - 0.5f))));
    param0->unk_20 = FX_MulInline(GF_CosDeg(param0->unk_28), ((fx32)(((param0->unk_30) > 0) ? ((param0->unk_30) * (1 << 12) + 0.5f) : ((param0->unk_30) * (1 << 12) - 0.5f))));
}

static inline void inline_ov116_022649E4_1(UnkStruct_ov116_02264FEC_sub1 *wk, fx32 x, fx32 y) {
    wk->unk_04 += x;
    wk->unk_08 += y;

    ManagedSprite_SetPositionFxXYWithSubscreenOffset(wk->unk_00, wk->unk_14 + wk->unk_0C + wk->unk_1C + wk->unk_04, wk->unk_18 + wk->unk_10 + wk->unk_20 + wk->unk_08, ((fx32)(((192 + 32) > 0) ? ((192 + 32) * (1 << 12) + 0.5f) : ((192 + 32) * (1 << 12) - 0.5f))));
}

void ov92_02260AE0(SysTask *param0, void *param1);

typedef struct {
    s16 a[5][2];
} ov92_02263E5C_Array;

extern const ov92_02263E5C_Array ov92_02263E5C;

void ov92_02260AE0(SysTask *param0, void *param1) {
    UnkStruct_ov116_02264FEC *v0 = param1;
    UnkStruct_ov116_022649E4 *v1 = v0->unk_00->unk_14;

    if ((IsPaletteFadeFinished() == 0) || (v1->unk_2C.unk_00 == 1)) {
        *v0->unk_10 = 0;
        SysTask_Destroy(param0);
        Heap_Free(v0);
        return;
    }

    switch (v0->unk_04) {
    case 0:
        v0->unk_0C = 0;
        v0->unk_04++;
    case 1: {
        int v2;
        ov92_02263E5C_Array v3 = ov92_02263E5C;
        u32 v4 = v3.a[v0->unk_0C][0];
        u32 v5 = v3.a[v0->unk_0C][1];

        v0->unk_08 = 0;

        for (v2 = 0; v2 < (3 * 12); v2++) {
            v0->unk_14[v2].unk_26 = v2 % 2;
            v0->unk_14[v2].unk_2E = 6 - (v2 % 3);
            v0->unk_14[v2].unk_30 = 6 - (v2 % 3);
            v0->unk_14[v2].unk_00 = v0->unk_00->unk_28[v2];
            v0->unk_14[v2].unk_28 = ((v2 / 3) * (360 / 12));
            v0->unk_14[v2].unk_28 %= 360;
            v0->unk_14[v2].unk_2C = 0;
            v0->unk_14[v2].unk_32 = -((v2 % 3) * 2);
            v0->unk_14[v2].unk_0C = ((fx32)(((v4) > 0) ? ((v4) * (1 << 12) + 0.5f) : ((v4) * (1 << 12) - 0.5f)));
            v0->unk_14[v2].unk_10 = ((fx32)(((v5) > 0) ? ((v5) * (1 << 12) + 0.5f) : ((v5) * (1 << 12) - 0.5f)));
            v0->unk_14[v2].unk_1C = 0;
            v0->unk_14[v2].unk_20 = 0;
            v0->unk_14[v2].unk_04 = 0;
            v0->unk_14[v2].unk_08 = 0;
            v0->unk_14[v2].unk_14 = 0;
            v0->unk_14[v2].unk_18 = 0;
            v0->unk_14[v2].unk_24 = 0;
            v0->unk_14[v2].unk_25 = 1;
            v0->unk_14[v2].unk_27 = 0;

            if (v0->unk_14[v2].unk_28 == 180) {
                v0->unk_14[v2].unk_24 = 2;
            }

            if ((v0->unk_14[v2].unk_28 >= 270) && (v0->unk_14[v2].unk_28 <= 90)) {
                v0->unk_14[v2].unk_24 = 1;
            }

            inline_ov116_022649E4(&v0->unk_14[v2]);
            inline_ov116_022649E4_1(&v0->unk_14[v2], ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))), ((fx32)(((v0->unk_14[v2].unk_2C) > 0) ? ((v0->unk_14[v2].unk_2C) * (1 << 12) + 0.5f) : ((v0->unk_14[v2].unk_2C) * (1 << 12) - 0.5f))));
        }
    }
        v0->unk_04++;
        break;

    case 2: {
        int v6;
        s16 v7, v8;
        BOOL v9 = 1;

        if (v0->unk_08 == 0) {
            v0->unk_08++;
            ov92_0225DF0C(v0->unk_00, 1, 0);
            PlaySE(1419);
        }

        if (v0->unk_08 > 30) {
            v0->unk_04 = 1;
            v0->unk_0C++;
            v0->unk_0C %= 5;
            ov92_0225DF0C(v0->unk_00, 0, 1);
        } else {
            v0->unk_08++;

            for (v6 = 0; v6 < (3 * 12); v6++) {
                if ((++v0->unk_14[v6].unk_32) < 0) {
                    continue;
                }

                if (v0->unk_14[v6].unk_32 >= (10 + v0->unk_14[v6].unk_26)) {
                    if (v0->unk_14[v6].unk_32 == (10 + v0->unk_14[v6].unk_26)) {
                        ManagedSprite_SetAnim(v0->unk_14[v6].unk_00, 0);
                    }

                    if (v0->unk_14[v6].unk_32 == (18 + v0->unk_14[v6].unk_26)) {
                        v0->unk_14[v6].unk_25 = 0;
                    } else {
                        if (v0->unk_14[v6].unk_32 % 2) {
                            v0->unk_14[v6].unk_25 ^= 1;
                        }
                    }

                    ManagedSprite_SetDrawFlag(v0->unk_14[v6].unk_00, v0->unk_14[v6].unk_25);

                    switch (v0->unk_14[v6].unk_27) {
                    case 0:
                        v0->unk_14[v6].unk_14 = ((fx32)(((-1) > 0) ? ((-1) * (1 << 12) + 0.5f) : ((-1) * (1 << 12) - 0.5f)));
                        break;
                    case 1:
                        v0->unk_14[v6].unk_14 = ((fx32)(((+0) > 0) ? ((+0) * (1 << 12) + 0.5f) : ((+0) * (1 << 12) - 0.5f)));
                        break;
                    case 2:
                        v0->unk_14[v6].unk_14 = ((fx32)(((+1) > 0) ? ((+1) * (1 << 12) + 0.5f) : ((+1) * (1 << 12) - 0.5f)));
                        break;
                    }

                    v0->unk_14[v6].unk_27++;
                    v0->unk_14[v6].unk_27 %= 3;
                }
                if (v0->unk_14[v6].unk_32 > 5) {
                    switch (v0->unk_14[v6].unk_24) {
                    case 2:
                        v0->unk_14[v6].unk_2C = (v0->unk_14[v6].unk_32 + 5) / 6;
                        break;
                    case 1:
                        v0->unk_14[v6].unk_2C = (v0->unk_14[v6].unk_32 + 5) / 6;
                        break;
                    case 0:
                        v0->unk_14[v6].unk_2C = (v0->unk_14[v6].unk_32 + 5) / 7;
                        break;
                    }
                }

                switch (v0->unk_14[v6].unk_24) {
                case 2:
                    v7 = (30 - (v0->unk_14[v6].unk_32 + 5)) / 8;
                    v8 = (30 - (v0->unk_14[v6].unk_32 + 7)) / 8;
                    break;
                case 1:
                default:
                    v7 = (30 - (v0->unk_14[v6].unk_32 + 5)) / 8;
                    v8 = (30 - (v0->unk_14[v6].unk_32 + 6)) / 8;
                    break;
                case 0:
                    v7 = (30 - (v0->unk_14[v6].unk_32 + 5)) / 8;
                    v8 = (30 - (v0->unk_14[v6].unk_32 + 4)) / 8;
                    break;
                }

                if (v7 > 0) {
                    v0->unk_14[v6].unk_2E += v7;
                } else {
                    v0->unk_14[v6].unk_2E++;
                }

                if (v8 > 0) {
                    v0->unk_14[v6].unk_30 += v8;
                } else {
                    v0->unk_14[v6].unk_30++;
                }

                inline_ov116_022649E4(&v0->unk_14[v6]);
                inline_ov116_022649E4_1(&v0->unk_14[v6], ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))), ((fx32)(((v0->unk_14[v6].unk_2C) > 0) ? ((v0->unk_14[v6].unk_2C) * (1 << 12) + 0.5f) : ((v0->unk_14[v6].unk_2C) * (1 << 12) - 0.5f))));
            }
        }
        if (*v0->unk_10) {
            v0->unk_04++;
        }
    } break;

    default:
        ov92_0225DF0C(v0->unk_00, 0, 0);
        Heap_Free(v0);
        SysTask_Destroy(param0);
        break;
    }

    ov92_0225DF28(v0->unk_00);
}
