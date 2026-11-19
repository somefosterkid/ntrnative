#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "heap.h"
#include "palette.h"
#include "sys_task.h"
#include "bg_window.h"
#include "gf_gfx_planes.h"

void GfGfx_SetBanks(const GraphicsBanks *banks);

void ov52_021E83A4(void);

extern const GraphicsBanks ov52_021E9578;

void ov52_021E83A4(void) {
    GraphicsBanks banks = ov52_021E9578;

    GfGfx_SetBanks(&banks);
}

struct BgConfig;

enum BgType {
    BG_TYPE_STATIC = 0,
    BG_TYPE_AFFINE,
    BG_TYPE_STATIC_WITH_AFFINE,
};

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void ov52_021E83C4(BgConfig *bgConfig);

extern const GraphicsModes ov52_021E94CC;

extern const BgTemplate ov52_021E9524;

extern const BgTemplate ov52_021E9540;

extern const BgTemplate ov52_021E955C;

extern const BgTemplate ov52_021E94EC;

extern const BgTemplate ov52_021E9508;

void ov52_021E83C4(BgConfig *bgConfig) {
    GraphicsModes graphicsModes = ov52_021E94CC;

    SetBothScreensModesAndDisable(&graphicsModes);

    BgTemplate main0Template = ov52_021E9524;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &main0Template, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);

    BgTemplate main1Template = ov52_021E9540;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &main1Template, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);

    BgTemplate main2Template = ov52_021E955C;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &main2Template, BG_TYPE_STATIC);

    BgTemplate sub0Template = ov52_021E94EC;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sub0Template, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);

    BgTemplate sub1Template = ov52_021E9508;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &sub1Template, BG_TYPE_STATIC);

    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, (8 * (8 / 2)), 0, HEAP_ID_39);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, (8 * (8 / 2)), 0, HEAP_ID_39);
}
