#include "global.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "gf_gfx_planes.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void ov37_021E5D10(BgConfig *param0);

extern const GraphicsModes ov37_021E7978;

extern const BgTemplate ov37_021E7998;

extern const BgTemplate ov37_021E79D0;

extern const BgTemplate ov37_021E79EC;

extern const BgTemplate ov37_021E7A08;

extern const BgTemplate ov37_021E79B4;

void ov37_021E5D10(BgConfig *param0) {
    {
        GraphicsModes v0 = ov37_021E7978;

        SetBothScreensModesAndDisable(&v0);
    }

    {
        BgTemplate v1 = ov37_021E7998;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_0, &v1, 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_0);
    }

    {
        BgTemplate v2 = ov37_021E79D0;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_1, &v2, 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_1);
    }

    {
        BgTemplate v3 = ov37_021E79EC;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_2, &v3, 0);
    }

    {
        BgTemplate v4 = ov37_021E7A08;

        InitBgFromTemplate(param0, GF_BG_LYR_SUB_0, &v4, 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_0);
    }

    {
        BgTemplate v5 = ov37_021E79B4;

        InitBgFromTemplate(param0, GF_BG_LYR_SUB_1, &v5, 0);
    }

    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, HEAP_ID_39);
    BG_ClearCharDataRange(4, 32, 0, HEAP_ID_39);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, HEAP_ID_39);
    BG_ClearCharDataRange(5, 32, 0, HEAP_ID_39);
}
