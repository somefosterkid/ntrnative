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

void ov85_021E8C5C(BgConfig *param0);

extern const GraphicsModes ov85_021EA8F4;

extern const BgTemplate ov85_021EA980;

extern const BgTemplate ov85_021EA99C;

extern const BgTemplate ov85_021EA92C;

extern const BgTemplate ov85_021EA948;

extern const BgTemplate ov85_021EA964;

void ov85_021E8C5C(BgConfig *param0) {
    GraphicsModes v0 = ov85_021EA8F4;

    SetBothScreensModesAndDisable(&v0);

    BgTemplate v1 = ov85_021EA980;

    InitBgFromTemplate(param0, GF_BG_LYR_SUB_0, &v1, 0);
    BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_0);

    BgTemplate v2 = ov85_021EA99C;

    InitBgFromTemplate(param0, GF_BG_LYR_SUB_1, &v2, 0);
    BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_1);

    BgTemplate v3 = ov85_021EA92C;

    InitBgFromTemplate(param0, GF_BG_LYR_SUB_2, &v3, 0);

    BgTemplate v4 = ov85_021EA948;

    InitBgFromTemplate(param0, GF_BG_LYR_MAIN_0, &v4, 0);
    BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_0);

    BgTemplate v5 = ov85_021EA964;

    InitBgFromTemplate(param0, GF_BG_LYR_MAIN_1, &v5, 0);

    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, HEAP_ID_102);
    BG_ClearCharDataRange(4, 32, 0, HEAP_ID_102);
}
