#include "global.h"

#include "constants/heap.h"
#include "overlay_92_unk_struct_ov116_0226139_c.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "player_data.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "bg_window.h"
#include "camera.h"
#include "overlay_92_unk_struct_ov116_02262d08.h"
#include "overlay_92_unk_struct_ov116_02265d84.h"
#include "overlay_90_unk_struct_ov114_0225c76c.h"
#include "overlay_90_unk_struct_ov114_0225c9a8.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_92_unk_struct_ov116_022649e4.h"
#include "overlay_92_unk_struct_ov116_02262a8c.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct BgConfig;

struct G3DPipelineBuffers;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

void NARC_Delete(NARC *narc);

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

BOOL SpriteSystem_LoadCellResObjFromOpenNarc(SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, int resourceID);

BOOL SpriteSystem_LoadAnimResObjFromOpenNarc(SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, int resourceID);

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

void ov92_0225E3C4(UnkStruct_ov116_0226139C *param0);

void ov92_0225E3C4(UnkStruct_ov116_0226139C *param0) {
    NARC *v0 = param0->unk_48.unk_00;
    NARC *v1 = param0->unk_48.unk_04;
    BgConfig *v2 = param0->unk_48.unk_10;
    SpriteSystem *v3 = param0->unk_48.unk_08;
    SpriteManager *v4 = param0->unk_48.unk_0C;
    PaletteData *v5 = param0->unk_48.unk_14;
    NARC *v6;
    int v7;
    int v8;
    int v9;
    int v10;
    int v11;
    int v12;

    v6 = v0;
    v7 = 82;
    v8 = 83;
    v9 = 84;
    v10 = 85;
    v11 = 9000;
    v12 = 13;

    SpriteSystem_LoadCharResObjFromOpenNarc(v3, v4, v6, v9, FALSE, NNS_G2D_VRAM_TYPE_2DSUB, v11);
    SpriteSystem_LoadCellResObjFromOpenNarc(v3, v4, v6, v8, FALSE, v11);
    SpriteSystem_LoadAnimResObjFromOpenNarc(v3, v4, v6, v7, FALSE, v11);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(v5, PLTTBUF_SUB_OBJ, v3, v4, v6, v10, FALSE, v12, NNS_G2D_VRAM_TYPE_2DSUB, v11);

    v7 = 27;
    v8 = 28;
    v9 = 29;
    v10 = 30;
    v11 = 9003;
    v12 = 1;

    SpriteSystem_LoadCharResObjFromOpenNarc(v3, v4, v6, v9, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, v11);
    SpriteSystem_LoadCellResObjFromOpenNarc(v3, v4, v6, v8, FALSE, v11);
    SpriteSystem_LoadAnimResObjFromOpenNarc(v3, v4, v6, v7, FALSE, v11);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(v5, PLTTBUF_MAIN_OBJ, v3, v4, v6, v10, FALSE, v12, NNS_G2D_VRAM_TYPE_2DMAIN, v11);

    v7 = 11;
    v8 = 12;
    v9 = 13;
    v10 = 14;
    v11 = 9004;
    v12 = 1;

    SpriteSystem_LoadCharResObjFromOpenNarc(v3, v4, v6, v9, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, v11);
    SpriteSystem_LoadCellResObjFromOpenNarc(v3, v4, v6, v8, FALSE, v11);
    SpriteSystem_LoadAnimResObjFromOpenNarc(v3, v4, v6, v7, FALSE, v11);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(v5, PLTTBUF_MAIN_OBJ, v3, v4, v6, v10, FALSE, v12, NNS_G2D_VRAM_TYPE_2DMAIN, v11);

    v7 = 19;
    v8 = 20;
    v9 = 21;
    v10 = 22;
    v11 = 9005;
    v12 = 1;

    SpriteSystem_LoadCharResObjFromOpenNarc(v3, v4, v6, v9, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, v11);
    SpriteSystem_LoadCellResObjFromOpenNarc(v3, v4, v6, v8, FALSE, v11);
    SpriteSystem_LoadAnimResObjFromOpenNarc(v3, v4, v6, v7, FALSE, v11);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(v5, PLTTBUF_MAIN_OBJ, v3, v4, v6, v10, FALSE, v12, NNS_G2D_VRAM_TYPE_2DMAIN, v11);

    v7 = 15;
    v8 = 16;
    v9 = 17;
    v10 = 18;
    v11 = 9006;
    v12 = 1;

    SpriteSystem_LoadCharResObjFromOpenNarc(v3, v4, v6, v9, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, v11);
    SpriteSystem_LoadCellResObjFromOpenNarc(v3, v4, v6, v8, FALSE, v11);
    SpriteSystem_LoadAnimResObjFromOpenNarc(v3, v4, v6, v7, FALSE, v11);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(v5, PLTTBUF_MAIN_OBJ, v3, v4, v6, v10, FALSE, v12, NNS_G2D_VRAM_TYPE_2DMAIN, v11);

    v7 = 23;
    v8 = 24;
    v9 = 25;
    v10 = 26;
    v11 = 9002;
    v12 = 3;

    SpriteSystem_LoadCharResObjFromOpenNarc(v3, v4, v6, v9, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, v11);
    SpriteSystem_LoadCellResObjFromOpenNarc(v3, v4, v6, v8, FALSE, v11);
    SpriteSystem_LoadAnimResObjFromOpenNarc(v3, v4, v6, v7, FALSE, v11);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(v5, PLTTBUF_MAIN_OBJ, v3, v4, v6, v10, FALSE, v12, NNS_G2D_VRAM_TYPE_2DMAIN, v11);

    {
        NARC *v13 = NARC_New((enum NarcId)200, HEAP_ID_113);

        v7 = 23;
        v8 = 22;
        v9 = 21;
        v10 = 20;
        v11 = 9007;
        v12 = 2;

        SpriteSystem_LoadCharResObjFromOpenNarc(v3, v4, v13, v9, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, v11);
        SpriteSystem_LoadCellResObjFromOpenNarc(v3, v4, v13, v8, FALSE, v11);
        SpriteSystem_LoadAnimResObjFromOpenNarc(v3, v4, v13, v7, FALSE, v11);
        SpriteSystem_LoadPaletteBufferFromOpenNarc(v5, PLTTBUF_MAIN_OBJ, v3, v4, v13, v10, FALSE, v12, NNS_G2D_VRAM_TYPE_2DMAIN, v11);
        NARC_Delete(v13);
    }
}
