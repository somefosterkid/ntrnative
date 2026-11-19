#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"
#include "filesystem.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

struct NARC;

struct SysTask;

struct SysTaskQueue;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void ToggleBgLayer(u8 bgLayer, u8 enable);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

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

void ov65_0221CEB8(BgConfig *bgConfig);

extern const GraphicsModes ov65_0221FD58;

extern const BgTemplate ov65_0221FDF0;

extern const BgTemplate ov65_0221FDB8;

extern const BgTemplate ov65_0221FE0C;

extern const BgTemplate ov65_0221FE28;

extern const BgTemplate ov65_0221FD80;

extern const BgTemplate ov65_0221FD9C;

extern const BgTemplate ov65_0221FDD4;

void ov65_0221CEB8(BgConfig *bgConfig) {
    GraphicsModes modes = ov65_0221FD58;

    SetBothScreensModesAndDisable(&modes);

    BgTemplate mainBg0Template = ov65_0221FDF0;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &mainBg0Template, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);

    BgTemplate mainBg1Template = ov65_0221FDB8;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &mainBg1Template, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);

    BgTemplate mainBg2Template = ov65_0221FE0C;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &mainBg2Template, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);

    BgTemplate mainBg3Template = ov65_0221FE28;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &mainBg3Template, 0);

    BgTemplate subBg0Template = ov65_0221FD80;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &subBg0Template, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);

    BgTemplate subBg1Template = ov65_0221FD9C;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &subBg1Template, 0);

    BgTemplate subBg2Template = ov65_0221FDD4;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &subBg2Template, 0);

    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, HEAP_ID_26);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, HEAP_ID_26);
    BG_ClearCharDataRange(4, 32, 0, HEAP_ID_26);

    for (int i = 0; i < 4; i++) {
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_0 + i, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_0 + i, BG_POS_OP_SET_Y, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_0 + i, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_0 + i, BG_POS_OP_SET_Y, 0);
    }

    ToggleBgLayer(GF_BG_LYR_MAIN_0, 0);
    ToggleBgLayer(GF_BG_LYR_MAIN_1, 0);
    ToggleBgLayer(GF_BG_LYR_MAIN_2, 0);
    ToggleBgLayer(GF_BG_LYR_MAIN_3, 0);
    ToggleBgLayer(GF_BG_LYR_SUB_0, 0);
    ToggleBgLayer(GF_BG_LYR_SUB_1, 0);
    ToggleBgLayer(GF_BG_LYR_SUB_2, 0);

    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, FALSE);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
    G2_SetWnd0Position(240, 0, 0, 16);
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
}
