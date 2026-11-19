#include "global.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "overlay_28.h"
#include "gf_gfx_planes.h"

struct BgConfig;

struct BgTemplate;

extern const BgTemplate ov28_0225EAC0;

extern const BgTemplate ov28_0225EADC;

extern const BgTemplate ov28_0225EAF8;

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void ov28_0225D650(BgConfig *param0) {
    GXS_SetGraphicsMode(GX_BGMODE_0);

    BgTemplate v1 = ov28_0225EAC0;
    InitBgFromTemplate(param0, GF_BG_LYR_SUB_0, &v1, 0);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_8);
    BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_0);

    BgTemplate v2 = ov28_0225EADC;
    InitBgFromTemplate(param0, GF_BG_LYR_SUB_1, &v2, 0);

    BgTemplate v3 = ov28_0225EAF8;
    InitBgFromTemplate(param0, GF_BG_LYR_SUB_2, &v3, 0);
}
