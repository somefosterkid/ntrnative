#include "global.h"

#include "constants/heap.h"
#include "overlay_90_unk_struct_ov114_0225c76c.h"
#include "overlay_90_unk_struct_ov114_0225c9a8.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "field/field_3d_model.h"

struct BgConfig;

struct Camera;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct NARC;

struct PaletteData;

struct SaveData;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct SysTaskManager;

struct UnkStruct_ov114_02025CAD4_t;

struct UnkStruct_ov114_0225D678_t;

struct UnkStruct_ov114_0225E854_t;

struct Window;

typedef struct {
    UnkStruct_ov66_02230DBC unk_00;
    SaveData *saveData;
    BOOL unk_38;
    u32 unk_3C;
} UnkStruct_ov66_02231134;

typedef struct UnkStruct_ov114_02025CAD4_t UnkStruct_ov114_0225CAD4;

typedef struct UnkStruct_ov114_0225D678_t UnkStruct_ov114_0225D678;

typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;

typedef struct {
    u8 unk_00;
    u8 padding_01[3];
    u32 unk_04;
} UnkStruct_ov116_0226048C;

typedef struct {
    u32 unk_00;
    u8 padding_04[4];
} UnkStruct_ov116_02260494;

typedef struct {
    UnkStruct_ov116_02260494 unk_00[4];
} UnkStruct_ov116_02260498;

typedef struct {
    u8 unk_00;
    u8 padding_01[3];
} UnkStruct_ov116_022649E4_sub1;

typedef struct BgConfig BgConfig;

typedef struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
} Window;

typedef struct Camera Camera;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

typedef struct ManagedSprite ManagedSprite;

typedef struct SpriteSystem SpriteSystem;

typedef struct SpriteManager SpriteManager;

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
    u8 padding_00[4];
    int unk_04;
    s16 unk_08;
    u8 padding_0A;
    UnkStruct_ov116_022649E4 *unk_0C;
} UnkStruct_ov116_02262A44;

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
    int unk_00;
    int unk_04;
    u8 padding_08[4];
    ManagedSprite *unk_0C;
    UnkStruct_ov116_0226469C unk_10;
    int unk_28;
    int *unk_2C;
    BOOL unk_30;
    UnkStruct_ov116_0226501C *unk_34;
    UnkStruct_ov116_0226501C *unk_38;
    UnkStruct_ov116_022649E4 *unk_3C;
    int unk_40;
} UnkStruct_ov116_02262D08;

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

typedef struct SysTask SysTask;

typedef struct SysTaskManager SysTaskManager;

typedef void (*SysTaskFunc)(SysTask *, void *);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

struct SysTask {
    SysTaskManager *manager;
    SysTask *prevTask;
    SysTask *nextTask;
    u32 priority;
    void *param;
    SysTaskFunc callback;
    enum TaskState state;
};

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
    ManagedSprite *unk_00;
    int unk_04;
    s16 unk_08;
    u8 padding_0A[6];
    int unk_10;
    s16 unk_14;
    s16 unk_16;
    int unk_18;
    int unk_1C;
    UnkStruct_ov116_0226469C unk_20;
    UnkStruct_ov116_0226469C unk_38;
} UnkStruct_ov116_02265D84;

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

typedef struct {
    UnkStruct_ov116_02262A44 *unk_00;
    UnkStruct_ov116_02262A8C *unk_04;
    BOOL unk_08;
    BOOL unk_0C;
    BOOL unk_10;
    UnkStruct_ov116_0226048C unk_14;
    UnkStruct_ov116_02260494 unk_1C;
    UnkStruct_ov116_02260498 unk_24;
    u32 unk_44;
    UnkStruct_ov116_022649E4 unk_48;
    u32 unk_78;
    UnkStruct_ov114_0225CAD4 *unk_7C;
    UnkStruct_ov66_02231134 *unk_80;
    UnkStruct_ov114_0225C76C unk_84;
    UnkStruct_ov114_0225D678 *unk_94;
    UnkStruct_ov114_0225C9A8 unk_98;
    UnkStruct_ov114_0225E854 *unk_B0;
    int unk_B4[4];
    BOOL unk_C4;
} UnkStruct_ov116_0226139C;

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

void Heap_Free(void *ptr);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;

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

void ov92_0225E130(UnkStruct_ov116_0226139C *param0, int param1);

void ov92_0225E130(UnkStruct_ov116_0226139C *param0, int param1) {
    PaletteData *v0 = param0->unk_48.unk_14;
    extern int _02264020[][4][4 + 1];

    for (int v2 = 0; v2 < 4; v2++) {
        PaletteData_LoadFromNarc(v0, (enum NarcId)193, 73, HEAP_ID_113, PLTTBUF_SUB_BG, ((16) * sizeof(u16)), ((v2 + 1) * (16)), ((_02264020[param0->unk_04->unk_04 - 1][param1][v2 + 1]) * (16)));
    }
}
