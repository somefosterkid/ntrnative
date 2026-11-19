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

enum BgType {
    BG_TYPE_STATIC = 0,
    BG_TYPE_AFFINE,
    BG_TYPE_STATIC_WITH_AFFINE,
};

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void Heap_Free(void *ptr);

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

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

void ov96_02204F40(BgConfig *bgConfig);

void GfGfx_EngineATogglePlanes(u8 planeMask, u8 enable);

extern const GraphicsModes ov96_0221CAF0;

extern const BgTemplate ov96_0221CB64;

extern const BgTemplate ov96_0221CB80;

extern const BgTemplate ov96_0221CB10;

extern const BgTemplate ov96_0221CB2C;

extern const BgTemplate ov96_0221CB48;

void ov96_02204F40(BgConfig *bgConfig) {
    GraphicsModes graphicsModes = ov96_0221CAF0;
    SetBothScreensModesAndDisable(&graphicsModes);
    BgTemplate main0Template = ov96_0221CB64;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &main0Template, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    BgTemplate main1Template = ov96_0221CB80;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &main1Template, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    BgTemplate main2Template = ov96_0221CB10;
    InitBgFromTemplate(bgConfig, (enum GFBgLayer)4, &main2Template, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, (enum GFBgLayer)4);
    BgTemplate sub0Template = ov96_0221CB2C;
    InitBgFromTemplate(bgConfig, (enum GFBgLayer)5, &sub0Template, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, (enum GFBgLayer)5);
    BgTemplate sub1Template = ov96_0221CB48;
    InitBgFromTemplate(bgConfig, (enum GFBgLayer)6, &sub1Template, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, (enum GFBgLayer)6);
    GfGfx_EngineATogglePlanes((enum GFBgLayer)8, 0);
}
