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
#include "field/overlay_01_021FB878.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "overlay_92_unk_struct_ov116_022649e4.h"
#include "overlay_92_unk_struct_ov116_02262a8c.h"

struct BgConfig;

struct Camera;

struct CameraPerspective;

struct CameraPositionHistory;

struct Field3DModelAnimation;

struct Field3dModel;

struct Field3dObject;

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

BOOL ov116_0226432C(UnkStruct_ov116_0226501C *param0, int param1, int param2, int param3, int param4, f32 param5, BOOL param6);

void ov92_02260860(UnkStruct_ov116_0226469C *param0, fx32 param1, fx32 param2, s32 param3);

BOOL ov116_02264774(UnkStruct_ov116_0226469C *param0);

void ov92_022630E8(UnkUnion_ov116_02266FEC *param0);

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

BOOL PlaySE(u16 seqID);

void ov92_0225EF4C(SysTask *param0, void *param1);

BOOL ov92_02260870(UnkStruct_ov116_0226469C *param0);

BOOL ov92_02260428(UnkStruct_ov116_0226501C *param0, int param1, int param2, int param3, int param4, f32 param5, BOOL param6);

void ov92_0225EF4C(SysTask *param0, void *param1) {
    UnkStruct_ov116_02262DC0 *v0 = param1;
    s16 v3 = v0->unk_10->unk_1E0.y >> 12;
    s16 v4 = v0->unk_10->unk_1E0.z >> 12;
    s16 v5 = v0->unk_20.z >> 12;
    fx32 v6;
    fx32 v7;
    fx32 v8;

    if (v0->unk_74) {
        v6 = ((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f)));
        v7 = ((fx32)(((12) > 0) ? ((12) * (1 << 12) + 0.5f) : ((12) * (1 << 12) - 0.5f)));
        v8 = ((fx32)(((48) > 0) ? ((48) * (1 << 12) + 0.5f) : ((48) * (1 << 12) - 0.5f)));
    } else {
        v6 = ((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f)));
        v7 = ((fx32)(((12) > 0) ? ((12) * (1 << 12) + 0.5f) : ((12) * (1 << 12) - 0.5f)));
        v8 = ((fx32)(((48) > 0) ? ((48) * (1 << 12) + 0.5f) : ((48) * (1 << 12) - 0.5f)));
    }

    if ((IsPaletteFadeFinished() == 0) || (v0->unk_78->unk_2C.unk_00 == 1)) {
        SysTask_Destroy(param0);
        Heap_Free(v0);
        return;
    }

    switch (v0->unk_00) {
    case 0:
        if (v0->unk_04 == 0) {
            ov92_02260860(&v0->unk_44, v0->unk_10->unk_1E0.y, v0->unk_10->unk_1E0.y + v7, 4);

            if (v5 > 0) {
                ov92_02260860(&v0->unk_5C, v0->unk_10->unk_1E0.z, v0->unk_10->unk_1E0.z + v6, 4);
            } else {
                ov92_02260860(&v0->unk_5C, v0->unk_10->unk_1E0.z, v0->unk_10->unk_1E0.z - v6, 4);
            }
        } else {
            ov92_02260860(&v0->unk_44, v0->unk_10->unk_1E0.y, v0->unk_10->unk_1E0.y + v8, 8);

            if (v5 > 0) {
                ov92_02260860(&v0->unk_5C, v0->unk_10->unk_1E0.z, v0->unk_10->unk_1E0.z - v6, 8);
            } else {
                ov92_02260860(&v0->unk_5C, v0->unk_10->unk_1E0.z, v0->unk_10->unk_1E0.z + v6, 4);
            }
        }

        PlaySE(1546);

        v0->unk_08 = 0;
        v0->unk_00++;
        break;
    case 1: {
        BOOL v9[2];

        v9[0] = ov92_02260870(&v0->unk_44);
        v9[1] = ov92_02260870(&v0->unk_5C);
        v0->unk_10->unk_1E0.y = v0->unk_44.unk_00;
        v0->unk_10->unk_1E0.z = v0->unk_5C.unk_00;

        if (v9[0] && v9[1]) {
            if (v0->unk_04 == 0) {
                *v0->unk_0C = 1;
                ov92_022630E8(&v0->unk_10->unk_190);
                ov92_022630E8(&v0->unk_10->unk_1A0);
                ov92_02260428(v0->unk_10, 0, 0, +5, +5, (0.80), 0);
                ov92_02260428(v0->unk_10, 0, 0, -5, -5, (0.80), 0);
                ov92_02260860(&v0->unk_44, v0->unk_10->unk_1E0.y, v0->unk_10->unk_1E0.y - v8, 8);

                if (v5 > 0) {
                    ov92_02260860(&v0->unk_5C, v0->unk_10->unk_1E0.z, v0->unk_10->unk_1E0.z + v6, 8);
                } else {
                    ov92_02260860(&v0->unk_5C, v0->unk_10->unk_1E0.z, v0->unk_10->unk_1E0.z - v6, 8);
                }
            } else {
                ov92_02260860(&v0->unk_44, v0->unk_10->unk_1E0.y, v0->unk_10->unk_1E0.y - v7, 4);

                if (v5 > 0) {
                    ov92_02260860(&v0->unk_5C, v0->unk_10->unk_1E0.z, v0->unk_10->unk_1E0.z - v6, 4);
                } else {
                    ov92_02260860(&v0->unk_5C, v0->unk_10->unk_1E0.z, v0->unk_10->unk_1E0.z + v6, 4);
                }
            }

            v0->unk_00++;
        }
    } break;
    case 2: {
        BOOL v10[2];

        v10[0] = ov92_02260870(&v0->unk_44);
        v10[1] = ov92_02260870(&v0->unk_5C);
        v0->unk_10->unk_1E0.y = v0->unk_44.unk_00;
        v0->unk_10->unk_1E0.z = v0->unk_5C.unk_00;

        if (v10[0] && v10[1]) {
            if (v0->unk_04 == 0) {
                PlaySE(1584);
            }
            v0->unk_00++;
        }
    } break;
    case 3:
        if (v0->unk_04 == 0) {
            if (v0->unk_08 == ((30 * 3) - 82)) {
                PlaySE(1421);
            }

            if ((++v0->unk_08) >= ((30 * 3) - 50)) {
                *v0->unk_0C = 2;
                v0->unk_00++;
            }
        } else {
            v0->unk_00++;
        }
        break;
    default:
        if (v0->unk_04 == 0) {
            if ((++v0->unk_08) >= ((30 * 3) - 45)) {
                *v0->unk_0C = 0;
                PlaySE(1546);
                v0->unk_00 = 0;
                v0->unk_04++;
            }
        } else {
            if ((++v0->unk_08) >= 10) {
                if (v0->unk_74) {
                    v0->unk_10->unk_1E0.y = (-8 * ((fx32)0x0000000000001000L));
                } else {
                    v0->unk_10->unk_1E0.y = (-20 * ((fx32)0x0000000000001000L));
                }

                v0->unk_10->unk_1E0.z = (0 * ((fx32)0x0000000000001000L));
                SysTask_Destroy(param0);
                Heap_Free(v0);
            }
        }
        break;
    }
}
