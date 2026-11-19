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

void ov96_021F7D30(BgConfig *bgConfig, s32 arg1);

extern const GraphicsModes ov96_0221C2B4;

typedef struct {
    BgTemplate a[6];
} ov96_0221C338_Array;

extern const ov96_0221C338_Array ov96_0221C338;

void ov96_021F7D30(BgConfig *bgConfig, s32 arg1) {
    GraphicsModes graphicsModes = ov96_0221C2B4;

    SetBothScreensModesAndDisable(&graphicsModes);

    ov96_0221C338_Array bgTemplates = ov96_0221C338;
    extern const u8 ov96_0221C290[6];
    extern const u8 ov96_0221C298[6];
    int i;

    for (i = 0; i < 6; i++) {
        InitBgFromTemplate(bgConfig, ov96_0221C290[i], &bgTemplates.a[i], ov96_0221C298[i]);
        BgClearTilemapBufferAndCommit(bgConfig, ov96_0221C290[i]);
        BG_ClearCharDataRange(ov96_0221C290[i], 0x40, 0, (enum HeapID)arg1);
    }
}
