#include "global.h"
#include "constants/heap.h"
#include "palette.h"
#include "bg_window.h"
#include "camera.h"
#include "sprite.h"
#include "sprite_system.h"
#include "field/overlay_01_021FB878.h"
#include "overlay_92_unk_struct_ov116_02262d08.h"
#include "overlay_92_unk_struct_ov116_02265d84.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct BgConfig;
struct Camera;
struct G3DPipelineBuffers;
struct ManagedSprite;
struct PaletteData;
struct SpriteManager;
struct SpriteSystem;
struct SysTask;
struct SysTaskManager;
typedef struct {
    u8 unk_00;
    u8 padding_01[3];
} UnkStruct_ov116_022649E4_sub1;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);
void ManagedSprite_SetPositionXY(ManagedSprite *managedSprite, s16 x, s16 y);
void ManagedSprite_GetPositionXY(ManagedSprite *managedSprite, s16 *outX, s16 *outY);
typedef struct {
    NARC *unk_00;
    NARC *unk_04;
    SpriteSystem *unk_08;
    SpriteManager *unk_0C;
    BgConfig *unk_10;
    PaletteData *unk_14;
    G3DPipelineBuffers *unk_18;
    Camera *camera;
    u8 padding_20[4];
    NNSFndAllocator unk_24;
    UnkStruct_ov116_022649E4_sub1 unk_2C;
} UnkStruct_ov116_022649E4;
typedef struct {
    u32 unk_00;
} UnkStruct_ov116_02260490;
typedef struct {
    u32 unk_00;
    u32 unk_04;
    u8 padding_08[4];
} UnkStruct_ov116_02262A8C_sub1;
typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_ov116_0226469C;
typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    s16 unk_1C;
    s16 unk_1E;
    s16 unk_20[4];
    int unk_28[4];
    int unk_38;
    s16 unk_3C;
    s16 unk_3E;
    s16 unk_40;
    s16 unk_42;
    UnkStruct_ov116_0226469C unk_44;
    UnkStruct_ov116_0226469C unk_5C;
    UnkStruct_ov116_0226469C unk_74;
    UnkStruct_ov116_0226469C unk_8C;
    BOOL unk_A4;
    BOOL unk_A8;
    BOOL unk_AC;
    BOOL unk_B0;
    BOOL unk_B4;
    s16 unk_B8;
    s16 unk_BA;
} UnkStruct_ov116_02262A8C_sub2;
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
typedef struct {
    BOOL unk_00;
    int unk_04;
    ManagedSprite *unk_08[3];
    UnkStruct_ov116_0226469C unk_14[3][2];
    UnkStruct_ov116_022649E4 *unk_A4;
} UnkStruct_ov116_022647BC;
typedef struct {
    int unk_00;
    BOOL unk_04;
    void *unk_08[8];
    int unk_28[10];
    BOOL unk_50[10];
} UnkStruct_ov116_02265470;
typedef struct SysTaskManager SysTaskManager;
struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};
typedef struct {
    int unk_00;
    int *unk_04;
    UnkStruct_ov116_02265470 unk_08[5];
    SysTask *unk_80;
    UnkStruct_ov116_022649E4 *unk_84;
} UnkStruct_ov116_0226534C;
typedef struct {
    UnkStruct_ov116_02265D84 unk_00[2];
    UnkStruct_ov116_02265D84 unk_A0[2];
    int unk_140;
    int unk_144;
    int unk_148;
    int unk_14C;
} UnkStruct_ov116_02265AA4;
typedef struct {
    UnkStruct_ov116_02265AA4 unk_00;
    u8 padding_04[4];
    SysTask *unk_08;
    int *unk_0C;
    int *unk_10;
    int *unk_14;
    int unk_18;
    int unk_1C;
    u8 padding_20[4];
    int unk_24;
    BOOL unk_28;
    BOOL unk_2C;
    UnkStruct_ov116_022649E4 *unk_30;
    UnkStruct_ov116_0226469C unk_34[5];
} UnkStruct_ov116_022660A8;
typedef struct {
    u32 unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    u8 padding_10[4];
    UnkStruct_ov116_022649E4 *unk_14;
    u8 padding_18[4];
    ManagedSprite *unk_1C[3];
    ManagedSprite *unk_20[3];
    ManagedSprite *unk_24[3];
    ManagedSprite *unk_28[36];
    ManagedSprite *unk_B8[4];
    ManagedSprite *unk_C8[2];
    ManagedSprite *unk_D0[2];
    ManagedSprite *unk_D8;
    u16 unk_DC[4];
    u8 padding_E4[12];
    int unk_F0;
    int unk_F4;
    int unk_F8;
    UnkStruct_ov116_0226501C unk_FC;
    UnkStruct_ov116_0226501C unk_308[3];
    UnkStruct_ov116_0226501C unk_92C;
    UnkStruct_ov116_0226501C unk_B38[8];
    UnkStruct_ov116_0226501C unk_1B98;
    UnkStruct_ov116_0226501C unk_1DA4;
    VecFx32 unk_1FB0;
    UnkStruct_ov116_02262A8C_sub1 unk_1FBC;
    Window unk_1FC8;
    Window unk_1FD8[4];
    UnkStruct_ov116_022660A8 unk_2018;
    UnkStruct_ov116_0226534C unk_20C4;
    UnkStruct_ov116_022647BC unk_214C[8];
    UnkStruct_ov116_02262D08 unk_268C[4];
    UnkStruct_ov116_02262A8C_sub2 unk_279C;
    UnkStruct_ov116_02260490 unk_2858[4];
    u8 padding_2868[4];
    u32 unk_286C;
    int unk_2870;
    BOOL unk_2874;
} UnkStruct_ov116_02262A8C;
void Heap_Free(void *ptr);
void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);
SysTask * SysTask_CreateOnMainQueue(SysTaskFunc func, void *data, u32 priority);
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);
static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);
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
void ov92_02261448(SysTask *param0, void *param1);
void PaletteData_BlendPalettes(PaletteData *paletteData, enum PaletteBufferId bufferID, u16 toBlend, u8 fraction, u16 target);
void ov92_02260860(UnkStruct_ov116_0226469C *param0, fx32 param1, fx32 param2, s32 param3);
BOOL ov92_02260870(UnkStruct_ov116_0226469C *param0);

void ov92_0226156C(UnkStruct_ov116_02262A8C *param0);
void ov92_022616D8(UnkStruct_ov116_022660A8 *param0);
void ov92_02261850(UnkStruct_ov116_022660A8 *param0);
int ov92_022619C4(int param0);

void ov92_0226156C(UnkStruct_ov116_02262A8C *param0) {
    int v0;

    param0->unk_20C4.unk_84 = param0->unk_14;
    param0->unk_20C4.unk_04 = &param0->unk_279C.unk_08;
    param0->unk_20C4.unk_00 = *param0->unk_20C4.unk_04;

    for (v0 = 0; v0 < 5; v0++) {
        UnkStruct_ov116_02265470 *v1 = &param0->unk_20C4.unk_08[v0];

        v1->unk_00 = 0;
        v1->unk_04 = 0;

        switch (v0) {
        case 0:
            v1->unk_08[0] = &param0->unk_92C;
            v1->unk_08[1] = &param0->unk_F0;
            v1->unk_08[3] = &param0->unk_F4;
            v1->unk_08[2] = param0->unk_14->unk_14;
            break;
        case 1:
            v1->unk_08[0] = param0->unk_B8[0];
            v1->unk_08[1] = param0->unk_B8[1];
            v1->unk_08[2] = 0L;
            v1->unk_08[3] = 0L;
            {
                s16 v2, v3;

                ManagedSprite_GetPositionXY(param0->unk_B8[0], &v2, &v3);
                ManagedSprite_SetPositionXY(param0->unk_B8[0], v2, (192 + 32));
                ManagedSprite_GetPositionXY(param0->unk_B8[1], &v2, &v3);
                ManagedSprite_SetPositionXY(param0->unk_B8[1], v2, (192 + 32));
            }
            break;
        case 2: {
            int v4;

            for (v4 = 0; v4 < 8; v4++) {
                v1->unk_08[v4] = &param0->unk_B38[v4];
            }
        } break;
        case 3:
            v1->unk_08[0] = 0L;
            v1->unk_08[1] = 0L;
            v1->unk_08[2] = param0->unk_B8[2];
            v1->unk_08[3] = param0->unk_B8[3];
            {
                s16 v5, v6;

                ManagedSprite_GetPositionXY(param0->unk_B8[2], &v5, &v6);
                ManagedSprite_SetPositionXY(param0->unk_B8[2], v5, (192 + 32));
                ManagedSprite_GetPositionXY(param0->unk_B8[3], &v5, &v6);
                ManagedSprite_SetPositionXY(param0->unk_B8[3], v5, (192 + 32));
            }
            break;
        case 4:
            v1->unk_08[0] = param0;
            break;
        }
    }

    param0->unk_20C4.unk_80 = SysTask_CreateOnMainQueue(ov92_02261448, &param0->unk_20C4, 4096);
}

void ov92_022616D8(UnkStruct_ov116_022660A8 *param0) {
    int *v0 = param0->unk_10;
    int *v1 = param0->unk_14;
    BOOL v2[5];

    switch (param0->unk_24) {
    case 0:
        ov92_02260860(&param0->unk_34[0], ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))), ((fx32)(((+40) > 0) ? ((+40) * (1 << 12) + 0.5f) : ((+40) * (1 << 12) - 0.5f))), 10);
        ov92_02260860(&param0->unk_34[1], ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))), ((fx32)(((+80) > 0) ? ((+80) * (1 << 12) + 0.5f) : ((+80) * (1 << 12) - 0.5f))), 10);
        ov92_02260860(&param0->unk_34[2], ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))), ((fx32)(((-80) > 0) ? ((-80) * (1 << 12) + 0.5f) : ((-80) * (1 << 12) - 0.5f))), 10);

        param0->unk_24++;
        break;
    case 1: {
        int v3;

        for (v3 = 0; v3 < 3; v3++) {
            v2[v3] = ov92_02260870(&param0->unk_34[v3]);
        }

        ScheduleSetBgPosText(param0->unk_30->unk_10, 7, BG_POS_OP_SET_Y, param0->unk_34[0].unk_00 >> 12);
        ScheduleSetBgPosText(param0->unk_30->unk_10, 5, BG_POS_OP_SET_X, param0->unk_34[1].unk_00 >> 12);
        ScheduleSetBgPosText(param0->unk_30->unk_10, 6, BG_POS_OP_SET_X, param0->unk_34[2].unk_00 >> 12);
        ScheduleSetBgPosText(param0->unk_30->unk_10, 1, BG_POS_OP_SET_X, param0->unk_34[1].unk_00 >> 12);
        ScheduleSetBgPosText(param0->unk_30->unk_10, 2, BG_POS_OP_SET_X, param0->unk_34[2].unk_00 >> 12);

        if (v2[0] && v2[1] && v2[2]) {
            param0->unk_24++;
        }
    } break;
    case 2:
        param0->unk_2C = 1;
        param0->unk_24 = 0;
        break;
    }

    if (*v1 < 6) {
        *v1 += 1;
    } else {
        *v1 = 6;
    }

    PaletteData_BlendPalettes(param0->unk_30->unk_14, PLTTBUF_MAIN_BG, 0x1, *v0 + *v1, 0x0);
    PaletteData_BlendPalettes(param0->unk_30->unk_14, PLTTBUF_SUB_BG, 0x1, *v0 + *v1, 0x0);
}

void ov92_02261850(UnkStruct_ov116_022660A8 *param0) {
    int *v0 = param0->unk_10;
    int *v1 = param0->unk_14;
    BOOL v2[5];

    switch (param0->unk_24) {
    case 0:
        ov92_02260860(&param0->unk_34[0], ((fx32)(((+40) > 0) ? ((+40) * (1 << 12) + 0.5f) : ((+40) * (1 << 12) - 0.5f))), ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))), 10);
        ov92_02260860(&param0->unk_34[1], ((fx32)(((+80) > 0) ? ((+80) * (1 << 12) + 0.5f) : ((+80) * (1 << 12) - 0.5f))), ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))), 10);
        ov92_02260860(&param0->unk_34[2], ((fx32)(((-80) > 0) ? ((-80) * (1 << 12) + 0.5f) : ((-80) * (1 << 12) - 0.5f))), ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))), 10);
        param0->unk_24++;
        break;
    case 1: {
        int v3;

        for (v3 = 0; v3 < 3; v3++) {
            v2[v3] = ov92_02260870(&param0->unk_34[v3]);
        }

        ScheduleSetBgPosText(param0->unk_30->unk_10, 7, BG_POS_OP_SET_Y, param0->unk_34[0].unk_00 >> 12);
        ScheduleSetBgPosText(param0->unk_30->unk_10, 5, BG_POS_OP_SET_X, param0->unk_34[1].unk_00 >> 12);
        ScheduleSetBgPosText(param0->unk_30->unk_10, 6, BG_POS_OP_SET_X, param0->unk_34[2].unk_00 >> 12);
        ScheduleSetBgPosText(param0->unk_30->unk_10, 1, BG_POS_OP_SET_X, param0->unk_34[1].unk_00 >> 12);
        ScheduleSetBgPosText(param0->unk_30->unk_10, 2, BG_POS_OP_SET_X, param0->unk_34[2].unk_00 >> 12);

        if (v2[0] && v2[1] && v2[2]) {
            param0->unk_24++;
        }
    } break;
    case 2:
        param0->unk_2C = 0;
        param0->unk_24 = 0;
        break;
    }

    if (*v1 > 4) {
        *v1 -= 1;
    } else {
        *v1 = 0;
    }

    PaletteData_BlendPalettes(param0->unk_30->unk_14, PLTTBUF_MAIN_BG, 0x1, *v0 + *v1, 0x0);
    PaletteData_BlendPalettes(param0->unk_30->unk_14, PLTTBUF_SUB_BG, 0x1, *v0 + *v1, 0x0);
}

int ov92_022619C4(int param0) {
    switch (param0) {
    case 1:
        return 28;
    case 2:
        return 14;
    case 3:
        return 12;
    case 4:
        return 10;
    case 5:
        return 9;
    case 6:
        return 32 * 1;
    case 7:
        return 14 * 1;
    case 8:
        return 12 * 1;
    case 9:
        return 11 * 1;
    case 10:
        return 11 * 1;
    default:
        return 9;
    }
}
