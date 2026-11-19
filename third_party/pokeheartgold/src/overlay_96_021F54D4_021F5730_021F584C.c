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

void sub_02079720(BgConfig *bgConfig);

void PartyMenu_LoadMemberPanelTilemaps(enum HeapID heapID, u16 *lead, u16 *back, u16 *none);

void ov96_021F584C(BgConfig *bgConfig);

void GfGfx_EngineATogglePlanes(u8 planeMask, u8 enable);

extern const GraphicsModes ov96_0221C100;

extern const BgTemplate ov96_0221C124;

extern const BgTemplate ov96_0221C140;

extern const BgTemplate ov96_0221C15C;

extern const BgTemplate ov96_0221C178;

extern const BgTemplate ov96_0221C194;

extern const BgTemplate ov96_0221C1B0;

void ov96_021F584C(BgConfig *bgConfig) {
    GraphicsModes modes = ov96_0221C100;

    SetBothScreensModesAndDisable(&modes);

    BgTemplate mainLayer1 = ov96_0221C124;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &mainLayer1, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);

    BgTemplate mainLayer2 = ov96_0221C140;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &mainLayer2, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);

    BgTemplate mainLayer3 = ov96_0221C15C;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &mainLayer3, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);

    BgTemplate subLayer0 = ov96_0221C178;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &subLayer0, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);

    BgTemplate subLayer1 = ov96_0221C194;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &subLayer1, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);

    BgTemplate layer6 = ov96_0221C1B0;

    InitBgFromTemplate(bgConfig, 6, &layer6, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 6);
    GfGfx_EngineATogglePlanes(8, 0);
}
