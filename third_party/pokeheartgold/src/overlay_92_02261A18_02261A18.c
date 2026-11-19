#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "overlay_92_unk_struct_ov116_02265d84.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct BgConfig;

struct Camera;

struct CameraPerspective;

struct CameraPositionHistory;

struct G3DPipelineBuffers;

struct NARC;

struct PaletteData;

struct SelectedPaletteData;

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

typedef struct {
    u8 unk_00;
    u8 padding_01[3];
} UnkStruct_ov116_022649E4_sub1;

typedef struct CameraPositionHistory {
    int historySize;
    int currentHistoryIndex;
    int nextHistoryIndex;
    int delay;
    BOOL delayReached;
    BOOL delayX;
    BOOL delayY;
    BOOL delayZ;
    VecFx32 *positions;
} CameraPositionHistory;

typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

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
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_ov116_0226469C;

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

void Heap_Free(void *ptr);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

SysTask * SysTask_CreateOnMainQueue(SysTaskFunc func, void *data, u32 priority);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

void SysTask_Destroy(SysTask *task);

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

BOOL IsPaletteFadeFinished(void);

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C[3];
    int unk_18[3];
    void *unk_24;
} UnkStruct_ov116_02267DA4;

void ov92_022616D8(UnkStruct_ov116_022660A8 *param0);

void ov92_02261850(UnkStruct_ov116_022660A8 *param0);

void ov116_02265AA4(SysTask *param0, void *param1);

void ov116_022660A8(SysTask *param0, void *param1);

void ov116_0226644C(SysTask *param0, void *param1);

void ov116_022667F4(SysTask *param0, void *param1);

void ov116_02266BF0(SysTask *param0, void *param1);

extern const UnkStruct_ov116_02267DA4 ov92_02263EA0[9];

void ov92_02261A18(SysTask *param0, void *param1);

void ov92_02261A18(SysTask *param0, void *param1) {
    int v0;
    int v2;
    UnkStruct_ov116_022660A8 *v3 = param1;
    UnkStruct_ov116_02265AA4 *v4 = &v3->unk_00;

    v2 = *v3->unk_0C;

    if ((IsPaletteFadeFinished() == 0) || (v3->unk_30->unk_2C.unk_00 == 1)) {
        SysTask_Destroy(param0);
        return;
    }

    if (v3->unk_28) {
        if (v3->unk_2C == 0) {
            ov92_022616D8(v3);
        }

        if (v3->unk_18 == 0) {
            if (v2 && (v2 < (8 - 0))) {
                v3->unk_18 = ov92_02263EA0[v2].unk_00;
                v3->unk_1C = 0;
                v4->unk_140 = ov92_02263EA0[v2].unk_04 - 1;
                v4->unk_144 = ov92_02263EA0[v2].unk_08;
                v4->unk_148 = ov92_02263EA0[v2].unk_08 + 5;

                for (v0 = 0; v0 < 2; v0++) {
                    int v5 = ov92_02263EA0[v2].unk_0C[v0];

                    ManagedSprite_SetPaletteOverrideOffset(v4->unk_00[v0].unk_00, 4 + v5);
                    v5 = ov92_02263EA0[v2].unk_18[v0];
                    ManagedSprite_SetPaletteOverrideOffset(v4->unk_A0[v0].unk_00, 0 + v5);
                }

                SysTask_CreateOnMainQueue(ov92_02263EA0[v2].unk_24, v3, 4097);
            }
        }
    } else if ((v3->unk_28 == 0) && (v3->unk_18 == 0)) {
        if (v3->unk_2C == 1) {
            ov92_02261850(v3);
        }
    }

    if (v3->unk_18) {
        v3->unk_18--;
    }
}
