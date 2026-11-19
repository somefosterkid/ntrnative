#include "global.h"
#include "sys_task.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "bg_window.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "gf_gfx_planes.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

struct NARC;

struct SysTask;

void Heap_Free(void *ptr);

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

void GfGfx_SetBanks(const GraphicsBanks *banks);

void GfGfx_DisableEngineAPlanes(void);

void GfGfx_DisableEngineBPlanes(void);

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

void ov75_0224725C(BgConfig *param0);

extern const GraphicsBanks ov75_022499FC;

extern const GraphicsModes ov75_0224997C;

typedef struct {
    BgTemplate a[4];
} ov75_02249A5C_Array;

extern const ov75_02249A5C_Array ov75_02249A5C;

typedef struct {
    BgTemplate a[2];
} ov75_02249A24_Array;

extern const ov75_02249A24_Array ov75_02249A24;

void ov75_0224725C(BgConfig *param0) {
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();

    {
        GraphicsBanks v0 = ov75_022499FC;

        GfGfx_SetBanks(&v0);

        MI_CpuClear32((void *)HW_BG_VRAM, HW_BG_VRAM_SIZE);
        MI_CpuClear32((void *)HW_DB_BG_VRAM, HW_DB_BG_VRAM_SIZE);
        MI_CpuClear32((void *)HW_OBJ_VRAM, HW_OBJ_VRAM_SIZE);
        MI_CpuClear32((void *)HW_DB_OBJ_VRAM, HW_DB_OBJ_VRAM_SIZE);
    }

    {
        GraphicsModes v1 = ov75_0224997C;

        SetBothScreensModesAndDisable(&v1);
    }

    {
        ov75_02249A5C_Array v2 = ov75_02249A5C;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_0, &v2.a[0], 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_0);
        BgSetPosTextAndCommit(param0, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(param0, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_Y, 0);

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_1, &v2.a[1], 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_1);
        BgSetPosTextAndCommit(param0, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(param0, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 0);

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_2, &v2.a[2], 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_2);
        BgSetPosTextAndCommit(param0, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(param0, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_3, &v2.a[3], 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_3);
        BgSetPosTextAndCommit(param0, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(param0, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
    }

    {
        ov75_02249A24_Array v3 = ov75_02249A24;

        InitBgFromTemplate(param0, GF_BG_LYR_SUB_0, &v3.a[0], 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_0);
        BgSetPosTextAndCommit(param0, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(param0, GF_BG_LYR_SUB_0, BG_POS_OP_SET_Y, 0);

        InitBgFromTemplate(param0, GF_BG_LYR_SUB_1, &v3.a[1], 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_1);
        BgSetPosTextAndCommit(param0, GF_BG_LYR_SUB_1, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(param0, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, 0);
    }

    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, HEAP_ID_116);
    BG_ClearCharDataRange(4, 32, 0, HEAP_ID_116);
}
