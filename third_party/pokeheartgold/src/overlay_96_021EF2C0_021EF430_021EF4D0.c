#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"

struct BgTemplate;

struct GraphicsModes;

typedef struct BgTemplateSet {
    BgTemplate t[7];
} BgTemplateSet;

extern const GraphicsModes ov96_0221BA44;

extern const BgTemplateSet ov96_0221BB98;

extern const u8 ov96_0221BA10[7];

extern const u8 ov96_0221BA18[7];

void ov96_021EF4D0(BgConfig *bgConfig, enum HeapID heapID);

void ov96_021EF4D0(BgConfig *bgConfig, enum HeapID heapID) {
    GraphicsModes graphicsModes = ov96_0221BA44;
    BgTemplateSet templates;
    int i;

    SetBothScreensModesAndDisable(&graphicsModes);
    templates = ov96_0221BB98;
    for (i = 0; i < 7; i++) {
        InitBgFromTemplate(bgConfig, ov96_0221BA18[i], &templates.t[i], ov96_0221BA10[i]);
        BgClearTilemapBufferAndCommit(bgConfig, ov96_0221BA18[i]);
        BG_ClearCharDataRange(ov96_0221BA18[i], 0x40, 0, heapID);
    }
}
