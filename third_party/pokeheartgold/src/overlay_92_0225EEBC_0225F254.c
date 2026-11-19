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
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "overlay_92_unk_struct_ov116_02262cb8.h"

struct BgConfig;

struct Camera;

struct CameraPerspective;

struct CameraPositionHistory;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct NARC;

struct PaletteData;

struct SelectedPaletteData;

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

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

u16 ManagedSprite_GetAnimationFrame(ManagedSprite *managedSprite);

void ManagedSprite_SetPositonFxXY(ManagedSprite *managedSprite, fx32 x, fx32 y);

void ManagedSprite_GetSpritePositionFxXY(ManagedSprite *managedSprite, fx32 *outX, fx32 *outY);

void ov92_02260860(UnkStruct_ov116_0226469C *param0, fx32 param1, fx32 param2, s32 param3);

BOOL ov92_02260870(UnkStruct_ov116_0226469C *param0);

void Heap_Free(void *ptr);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

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

void ov92_0225F254(SysTask *param0, void *param1);

void ov92_0225F254(SysTask *param0, void *param1) {
    UnkStruct_ov116_02262CB8 *v0 = param1;
    int v1 = ManagedSprite_GetAnimationFrame(v0->unk_0C);

    if ((IsPaletteFadeFinished() == 0) || (v0->unk_2C->unk_2C.unk_00 == 1)) {
        SysTask_Destroy(param0);
        Heap_Free(v0);
        return;
    }

    switch (v0->unk_00) {
    case 0: {
        fx32 v2, v3;

        v0->unk_08 = 0;
        ManagedSprite_GetSpritePositionFxXY(v0->unk_0C, &v2, &v3);

        if (v0->unk_04 == 0) {
            ov92_02260860(&v0->unk_14, v3, v3 + ((fx32)(((32) > 0) ? ((32) * (1 << 12) + 0.5f) : ((32) * (1 << 12) - 0.5f))), 16);
        } else {
            ov92_02260860(&v0->unk_14, v3, v3 - ((fx32)(((32) > 0) ? ((32) * (1 << 12) + 0.5f) : ((32) * (1 << 12) - 0.5f))), 4);
        }

        v0->unk_00++;
    } break;
    case 1: {
        fx32 v5, v6;
        BOOL v7;

        v7 = ov92_02260870(&v0->unk_14);
        ManagedSprite_GetSpritePositionFxXY(v0->unk_0C, &v5, &v6);
        ManagedSprite_SetPositonFxXY(v0->unk_0C, v5, v0->unk_14.unk_00);

        if (v7) {
            v0->unk_00++;
        }
    } break;
    default:

        if (v0->unk_04 == 0) {
            if ((++v0->unk_08) >= 35) {
                v0->unk_04++;
                v0->unk_00 = 0;
            }
        } else {
            if ((++v0->unk_08) >= 10) {
                SysTask_Destroy(param0);
                Heap_Free(v0);
            }
        }
        break;
    }
}
