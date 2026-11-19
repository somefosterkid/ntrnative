#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

extern const BgTemplate ov97_0221FD20;

extern const BgTemplate ov97_0221FD3C;

extern const BgTemplate ov97_0221FD04;

extern const BgTemplate ov97_0221FD58;

void ov97_0221F1D8(BgConfig *param0);

void ov97_0221F1D8(BgConfig *param0) {
    BgTemplate v1 = ov97_0221FD20;
    InitBgFromTemplate(param0, GF_BG_LYR_SUB_0, &v1, 0);
    BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_0);
    BgTemplate v2 = ov97_0221FD3C;
    InitBgFromTemplate(param0, GF_BG_LYR_SUB_1, &v2, 0);
    BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_1);
    BgTemplate v3 = ov97_0221FD04;
    InitBgFromTemplate(param0, GF_BG_LYR_SUB_2, &v3, 0);
    BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_2);
    BgTemplate v4 = ov97_0221FD58;
    InitBgFromTemplate(param0, GF_BG_LYR_SUB_3, &v4, 0);
    BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_3);
}
