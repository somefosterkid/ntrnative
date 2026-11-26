#include "global.h"

#include "overlay_86_frontier_records_app.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

enum BgType {
    BG_TYPE_STATIC = 0,
    BG_TYPE_AFFINE,
    BG_TYPE_STATIC_WITH_AFFINE,
};

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

extern const GraphicsModes ov86_021E7EC8;

extern const BgTemplate ov86_021E7F04;

extern const BgTemplate ov86_021E7F20;

extern const BgTemplate ov86_021E7F3C;

extern const BgTemplate ov86_021E7F58;

void ov86_021E5D30(BgConfig *bgConfig) {
    GraphicsModes graphicsModes = ov86_021E7EC8;

    SetBothScreensModesAndDisable(&graphicsModes);

    BgTemplate bgTemplate1 = ov86_021E7F04;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &bgTemplate1, BG_TYPE_STATIC);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, (enum HeapID)0x79);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);

    BgTemplate bgTemplate2 = ov86_021E7F20;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &bgTemplate2, BG_TYPE_STATIC);

    BgTemplate bgTemplate3 = ov86_021E7F3C;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &bgTemplate3, BG_TYPE_STATIC);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 32, 0, (enum HeapID)0x79);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);

    BgTemplate bgTemplate4 = ov86_021E7F58;

    InitBgFromTemplate(bgConfig, (enum GFBgLayer)6, &bgTemplate4, BG_TYPE_STATIC);
}
