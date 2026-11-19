#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"
#include "filesystem.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "overlay_64_hall_of_fame_showcase.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

struct NARC;

struct SysTask;

struct SysTaskQueue;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 mode);

void ToggleBgLayer(u8 bgId, u8 toggle);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

void NARC_Delete(NARC *narc);

void GfGfxLoader_GXLoadPalFromOpenNarc(NARC *narc, u32 memberIdx, u32 arg2, u32 arg3, u32 size, u32 heapID);

void GfGfxLoader_LoadCharDataFromOpenNarc(NARC *narc, u32 memberIdx, BgConfig *bgConfig, u8 bgLayer, u32 offset, u32 size, BOOL compressed, u32 heapID);

void GfGfxLoader_LoadScrnDataFromOpenNarc(NARC *narc, u32 memberIdx, BgConfig *bgConfig, u8 bgLayer, u32 offset, u32 size, BOOL compressed, u32 heapID);

void LoadFontPal0(u32 arg0, u32 arg1, u32 heapID);

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

void ov64_021E5B10(HallOfFameShowcase *param0);

extern const GraphicsModes ov64_021E6E88;

extern const BgTemplate ov64_021E6F50;

extern const BgTemplate ov64_021E6EFC;

extern const BgTemplate ov64_021E6F18;

extern const BgTemplate ov64_021E6F34;

void ov64_021E5B10(HallOfFameShowcase *param0) {
    param0->bgConfig = BgConfig_Alloc(HEAP_ID_59);

    GraphicsModes modes = ov64_021E6E88;

    SetBothScreensModesAndDisable(&modes);

    BgTemplate mainBg0Template = ov64_021E6F50;

    InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_MAIN_0, &mainBg0Template, 0);
    BgClearTilemapBufferAndCommit(param0->bgConfig, GF_BG_LYR_MAIN_0);

    BG_ClearCharDataRange(0, 32, 0, HEAP_ID_59);

    BgTemplate mainBg1Template = ov64_021E6EFC;

    InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_MAIN_1, &mainBg1Template, 0);

    BgTemplate subBg0Template = ov64_021E6F18;

    InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_SUB_0, &subBg0Template, 0);
    BgClearTilemapBufferAndCommit(param0->bgConfig, GF_BG_LYR_SUB_0);

    BG_ClearCharDataRange(4, 32, 0, HEAP_ID_59);

    BgTemplate subBg1Template = ov64_021E6F34;

    InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_SUB_1, &subBg1Template, 0);

    NARC *narc = NARC_New(NARC_a_0_9_7, HEAP_ID_59);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0, param0->bgConfig, 1, 0, 0, 1, 59);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, param0->bgConfig, 1, 0, 0, 1, 59);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 2, 0, 0, 0x20, 59);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 3, param0->bgConfig, 5, 0, 0, 1, 59);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 4, param0->bgConfig, 5, 0, 0, 1, 59);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 5, 4, 0, 0x20, 59);
    NARC_Delete(narc);

    LoadFontPal0(0, 0x1e << 4, 59);
    LoadFontPal0(4, 0x1e << 4, 59);
}
