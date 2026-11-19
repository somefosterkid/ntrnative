#include "global.h"
#include "unk_struct_02089688.h"
#include "sys_task.h"
#include "constants/heap.h"
#include "options.h"
#include "pm_string.h"
#include "unk_02013534.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "gf_gfx_planes.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct BgConfig;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct TouchHitboxController;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

BOOL SpriteSystem_LoadCellResObjFromOpenNarc(SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, int resourceID);

BOOL SpriteSystem_LoadAnimResObjFromOpenNarc(SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, int resourceID);

typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchscreenHitbox;

typedef struct TouchHitboxController TouchHitboxController;

u32 GfGfxLoader_LoadCharDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, u32 heapID);

void GfGfxLoader_LoadScrnDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, enum HeapID heapID);

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

u32 sub_0200E640(u32 messageBoxFrame);

void LoadUserFrameGfx2(BgConfig *bgConfig, GFBgLayer bgLayer, u16 tileOffset, u8 palOffset, u8 messageBoxFrame, u32 heapID);

void sub_02086490(UnkStruct_02089688 *param0);

void sub_02086490(UnkStruct_02089688 *param0) {
    NARC *v0;
    BgConfig *v1;
    SpriteSystem *v2 = param0->unk_2EC.unk_04;
    SpriteManager *v3 = param0->unk_2EC.unk_08;
    PaletteData *v4 = param0->unk_2EC.unk_10;
    v1 = param0->unk_2EC.unk_0C;
    v0 = param0->unk_2EC.unk_00;

    GfGfxLoader_LoadCharDataFromOpenNarc(v0, 12, v1, 1, 0, 0, 0, HEAP_ID_108);
    GfGfxLoader_LoadScrnDataFromOpenNarc(v0, 14, v1, 1, 0, 0, 0, HEAP_ID_108);
    PaletteData_LoadNarc(v4, (enum NarcId)190, 13, HEAP_ID_108, PLTTBUF_MAIN_BG, ((16) * sizeof(u16)), ((0) * (16)));

    GfGfxLoader_LoadCharDataFromOpenNarc(v0, 15, v1, 5, 0, 0, 0, HEAP_ID_108);
    GfGfxLoader_LoadScrnDataFromOpenNarc(v0, 17, v1, 5, 0, 0, 0, HEAP_ID_108);
    PaletteData_LoadNarc(v4, (enum NarcId)190, 16, HEAP_ID_108, PLTTBUF_SUB_BG, ((16) * sizeof(u16)), ((0) * (16)));

    SpriteSystem_LoadPaletteBufferFromOpenNarc(v4, PLTTBUF_MAIN_OBJ, v2, v3, v0, 1, 0, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 1000);
    SpriteSystem_LoadCharResObjFromOpenNarc(v2, v3, v0, 0, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 1000);
    SpriteSystem_LoadCellResObjFromOpenNarc(v2, v3, v0, 2, 0, 1000);
    SpriteSystem_LoadAnimResObjFromOpenNarc(v2, v3, v0, 3, 0, 1000);

    SpriteSystem_LoadPaletteBufferFromOpenNarc(v4, PLTTBUF_MAIN_OBJ, v2, v3, v0, 5, 0, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 1001);
    SpriteSystem_LoadCharResObjFromOpenNarc(v2, v3, v0, 4, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 1001);
    SpriteSystem_LoadCellResObjFromOpenNarc(v2, v3, v0, 6, 0, 1001);
    SpriteSystem_LoadAnimResObjFromOpenNarc(v2, v3, v0, 7, 0, 1001);

    SpriteSystem_LoadPaletteBufferFromOpenNarc(v4, PLTTBUF_MAIN_OBJ, v2, v3, v0, 9, 0, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 1002);
    SpriteSystem_LoadCharResObjFromOpenNarc(v2, v3, v0, 8, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 1002);
    SpriteSystem_LoadCellResObjFromOpenNarc(v2, v3, v0, 10, 0, 1002);
    SpriteSystem_LoadAnimResObjFromOpenNarc(v2, v3, v0, 11, 0, 1002);

    int v5 = Options_GetFrame(param0->unk_3B8.options);

    LoadUserFrameGfx2(v1, GF_BG_LYR_SUB_0, 1, 10, v5, HEAP_ID_108);
    PaletteData_LoadNarc(v4, NARC_a_0_3_8, sub_0200E640(v5), HEAP_ID_108, PLTTBUF_SUB_BG, ((16) * sizeof(u16)), ((11) * (16)));
    PaletteData_LoadNarc(v4, (enum NarcId)16, 8, HEAP_ID_108, PLTTBUF_SUB_BG, ((16) * sizeof(u16)), ((12) * (16)));
}
