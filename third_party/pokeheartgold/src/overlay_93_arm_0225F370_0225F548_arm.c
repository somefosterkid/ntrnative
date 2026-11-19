#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct ManagedSprite;

struct NARC;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SysTask;

struct SysTaskQueue;

struct UnkStruct_ov117_022626B0_t;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

void Heap_Free(void *ptr);

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

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
} UnkStruct_ov117_022626B0_sub1_sub1;

typedef struct {
    UnkStruct_ov117_022626B0_sub1_sub1 unk_00;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_08;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_10;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_18;
} UnkStruct_ov117_022626B0_sub1;

typedef struct {
    UnkStruct_ov117_022626B0_sub1_sub1 unk_00;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_08;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_10;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_18;
} UnkStruct_ov117_022626B0_sub2;

struct UnkStruct_ov117_022626B0_t {
    NNSG3dResFileHeader *unk_00;
    NNSG3dResTex *unk_04;
    u16 unk_08;
    UnkStruct_ov117_022626B0_sub1 unk_0C[8];
    UnkStruct_ov117_022626B0_sub2 unk_10C[8];
    int unk_20C;
    int unk_210;
    int unk_214;
    int unk_218;
    int unk_21C;
    int unk_220;
    int unk_224;
    int unk_228;
    int unk_22C;
    fx32 unk_230;
    ManagedSprite *unk_234;
    int unk_238;
    int unk_23C;
    int unk_240;
    int unk_244;
    fx32 unk_248;
    u16 *unk_24C;
    u16 *unk_250;
    u32 unk_254;
    u32 unk_258;
    s32 unk_25C;
    fx32 unk_260;
    fx32 unk_264;
    s16 unk_268;
    s16 unk_26A;
    s16 unk_26C;
    int unk_270;
    u8 unk_274;
    u8 unk_275;
    u8 unk_276;
};

int ov93_0225F8AC(UnkStruct_ov117_022626B0 *param0, int param1);

int ov93_0225F8E4(UnkStruct_ov117_022626B0 *param0, fx32 param1, fx32 param2, fx32 param3, fx32 *param4, fx32 *param5);

int ov93_0225F548(UnkStruct_ov117_022626B0 *param0, int param1, fx32 param2);

int ov93_0225F548(UnkStruct_ov117_022626B0 *param0, int param1, fx32 param2) {
    fx32 v0, v3, v4, v5, v6;
    fx32 v7, v8;
    u16 v11, v12;
    int v13 = 0;

    if ((param1 == 0) && (param2 < 0)) {
        (void)0;
    }

    v0 = param0->unk_210 << 12;
    v3 = -param0->unk_0C[0].unk_00.unk_04;
    v4 = -param0->unk_0C[0].unk_10.unk_04;
    v7 = v4 + (v3 - v4) / 2;

    switch (ov93_0225F8AC(param0, param1)) {
    case 0:
        v5 = v3 + param2;
        v6 = v4 + param2;
        break;
    case 1:
        v13 = ov93_0225F8E4(param0, param2, v3, v4, &v5, &v6);
        break;
    case 2:
        v13 = ov93_0225F8E4(param0, param2, v4, v3, &v6, &v5);
        break;
    }

    v8 = v6 + (v5 - v6) / 2;

    if ((v7 == v8) && (v3 == v5) && (v4 == v6)) {
        return 0;
    }

    if (v8 < 0) {
        v8 = 0;
    }

    if (v5 < 0) {
        v5 = 0;
    }

    if (v6 < 0) {
        v6 = 0;
    }

    if (v8 > (((128 * ((fx32)0x0000000000001000L)) / 8 * (8 - 1)) - 12 * ((fx32)0x0000000000001000L))) {
        v8 = (((128 * ((fx32)0x0000000000001000L)) / 8 * (8 - 1)) - 12 * ((fx32)0x0000000000001000L));
        v13 = 1;
    }

    if (v5 > (((128 * ((fx32)0x0000000000001000L)) / 8 * (8 - 1)) - 12 * ((fx32)0x0000000000001000L))) {
        v5 = (((128 * ((fx32)0x0000000000001000L)) / 8 * (8 - 1)) - 12 * ((fx32)0x0000000000001000L));
        v13 = 1;
    }

    if (v6 > (((128 * ((fx32)0x0000000000001000L)) / 8 * (8 - 1)) - 12 * ((fx32)0x0000000000001000L))) {
        v6 = (((128 * ((fx32)0x0000000000001000L)) / 8 * (8 - 1)) - 12 * ((fx32)0x0000000000001000L));
        v13 = 1;
    }

    {
        fx32 v14, v15;
        int v16;

        v16 = 64;

        switch (ov93_0225F8AC(param0, param1)) {
        case 0:
        default:
            if (v5 < v6) {
                v14 = v16 * ((fx32)0x0000000000001000L);
            } else if (v5 > v6) {
                v14 = -v16 * ((fx32)0x0000000000001000L);
            } else {
                v14 = 0;
            }

            v14 = v16 * ((fx32)0x0000000000001000L);
            v15 = v6 - v8;
            v11 = FX_Atan2Idx(v15, v14);
            v12 = v11 + 0x8000;
            break;
        case 2:
            v14 = v16 * ((fx32)0x0000000000001000L);
            v15 = (v6 - v8);
            v11 = FX_Atan2Idx(v15, v14);
            v12 = v11 + 0x8000;
            break;
        case 1:
            v14 = -v16 * ((fx32)0x0000000000001000L);
            v15 = v5 - v8;
            v12 = FX_Atan2Idx(v15, v14);
            v11 = v12 + 0x8000;
            break;
        }
    }

    {
        fx32 v17 = ((128 * ((fx32)0x0000000000001000L)) / 2) / ((fx32)0x0000000000001000L);

        param0->unk_0C[0].unk_00.unk_00 = v17 * FX_CosIdx(v12) + ((128 * ((fx32)0x0000000000001000L)) / 2);
        param0->unk_0C[0].unk_00.unk_04 = -(v17 * FX_SinIdx(v12) + v8);
        param0->unk_0C[0].unk_10.unk_00 = v17 * FX_CosIdx(v11) + ((128 * ((fx32)0x0000000000001000L)) / 2);
        param0->unk_0C[0].unk_10.unk_04 = -(v17 * FX_SinIdx(v11) + v8);

        {
            int v18;
            fx32 v19;
            s32 v20;

            v19 = (((128 * ((fx32)0x0000000000001000L)) / 8 * (8 - 1)) - v8) / (8 - 1);

            if (v11 > 0x8000) {
                v20 = -(0x10000 - v11) / (8 - 1);
            } else {
                v20 = v11 / (8 - 1);
            }

            for (v18 = 1; v18 < (8 - 1); v18++) {
                param0->unk_0C[v18].unk_00.unk_00 = v17 * FX_CosIdx(v12 - v20 * v18) + ((128 * ((fx32)0x0000000000001000L)) / 2);
                param0->unk_0C[v18].unk_00.unk_04 = -(v17 * FX_SinIdx(v12 - v20 * v18) + v8 + v19 * v18);
                param0->unk_0C[v18].unk_10.unk_00 = v17 * FX_CosIdx(v11 - v20 * v18) + ((128 * ((fx32)0x0000000000001000L)) / 2);
                param0->unk_0C[v18].unk_10.unk_04 = -(v17 * FX_SinIdx(v11 - v20 * v18) + v8 + v19 * v18);
                param0->unk_0C[v18 - 1].unk_08.unk_00 = param0->unk_0C[v18].unk_00.unk_00;
                param0->unk_0C[v18 - 1].unk_08.unk_04 = param0->unk_0C[v18].unk_00.unk_04;
                param0->unk_0C[v18 - 1].unk_18.unk_00 = param0->unk_0C[v18].unk_10.unk_00;
                param0->unk_0C[v18 - 1].unk_18.unk_04 = param0->unk_0C[v18].unk_10.unk_04;
            }
        }
    }
    if (v13 == 1) {
        return 2;
    }

    return 0;
}
