#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"

typedef enum BgPosAdjustOp BgPosAdjustOp;

typedef struct BgTemplateSet {
    BgTemplate templates[6];
} BgTemplateSet;

extern const GraphicsModes ov96_0221CC0C;

extern const BgTemplateSet ov96_0221CCF8;

extern u32 ov96_0221CC48[6];

void ov96_02209C14(BgConfig *bgConfig, enum HeapID heapID);

void ov96_02209C14(BgConfig *bgConfig, enum HeapID heapID) {
    int i;
    GraphicsModes graphicsModes;
    BgTemplateSet templates;

    graphicsModes = ov96_0221CC0C;
    SetBothScreensModesAndDisable(&graphicsModes);

    templates = ov96_0221CCF8;

    for (i = 0; i < 6; i++) {
        InitBgFromTemplate(bgConfig, (u8)ov96_0221CC48[i], &templates.templates[i], 0);
        BgClearTilemapBufferAndCommit(bgConfig, (u8)ov96_0221CC48[i]);
        BG_ClearCharDataRange((u8)ov96_0221CC48[i], 32, 0, heapID);
    }

    FontID_Alloc((enum Font)4, heapID);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 0);
    BgSetPosTextAndCommit(bgConfig, 0, (BgPosAdjustOp)3, 16);
}
