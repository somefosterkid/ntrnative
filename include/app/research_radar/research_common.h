// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/research_radar/research_common.h: the
// lines in use so far.
#ifndef BWPORT_APP_RESEARCH_RADAR_RESEARCH_COMMON_H
#define BWPORT_APP_RESEARCH_RADAR_RESEARCH_COMMON_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/touchpanel.h"
#include "struct_decls.h"

// The screens of the Research Radar's proc
enum {
    RESEARCH_SEQ_INIT,
    RESEARCH_SEQ_TOP,
    RESEARCH_SEQ_LIST,
    RESEARCH_SEQ_GRAPH,
    RESEARCH_SEQ_END,
};

// Black 2's, not checked against Black
// A rectangle of the touch screen, as the screens' tables give them, which they copy into TouchRects
struct ResearchRect {
    u8 left;
    u8 right;
    u8 top;
    u8 bottom;
};

// A cell actor of a screen: what goes in its ClActorSetup, its unit, and the indices of its resources in the screen's
// table
struct ResearchActorSetup {
    s16 x;
    s16 y;
    s16 sequence;
    u8 priority;
    u8 bgPriority;
    u32 unit;
    u32 chars;
    u32 palette;
    u32 cellAnims;
    u16 surface;
};

// Black 2's, not checked against Black
// A palette animation of a screen: the colors it animates, and how it runs when it is started
struct ResearchPaletteAnimeSetup {
    u16 *dst;
    const u16 *src;
    u8 count;
    u32 mode;
    // The color the animation blends toward
    u16 color;
};

// pokebw2's ResearchCommon_Create
ResearchCommon *func_ov217_021c097c(HeapID heapId, GameSystem *gsys);
// pokebw2's ResearchCommon_SetSeq
// Records the screen being shown, and keeps the one before it
void func_ov217_021c09bc(ResearchCommon *common, u32 seq);
// pokebw2's ResearchCommon_SetForceExit
void func_ov217_021c09fc(ResearchCommon *common);

// The BGs that the proc shows behind every screen: the main engine's frame, and the sub engine's background and its
// pattern
#define RESEARCH_BG_MAIN_FRAME 1

#define RESEARCH_BG_SUB_BACK 4
#define RESEARCH_BG_SUB_PATTERN 5

// The buttons of the common touch rectangles
#define RESEARCH_COMMON_BUTTON_RETURN 0

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ResearchCommon_Delete
void func_ov217_021c099c(ResearchCommon *common);
// pokebw2's ResearchCommon_GetHeapID
HeapID func_ov217_021c09ac(ResearchCommon *common);
// pokebw2's ResearchCommon_GetGameSystem
GameSystem *func_ov217_021c09b0(ResearchCommon *common);
// pokebw2's ResearchCommon_GetGameData
GameData *func_ov217_021c09b4(ResearchCommon *common);
// pokebw2's ResearchCommon_GetTouchRects
const TouchRect *func_ov217_021c09b8(ResearchCommon *common);
// pokebw2's ResearchCommon_SetTouchMode
// Whether the last screen was left by touch, so that the next one starts without its cursor
void func_ov217_021c09c4(ResearchCommon *common, BOOL touch);
// pokebw2's ResearchCommon_GetPrevSeq
u32 func_ov217_021c09c8(ResearchCommon *common);
// pokebw2's ResearchCommon_GetTouchMode
BOOL func_ov217_021c09cc(ResearchCommon *common);
// pokebw2's ResearchCommon_UpdatePaletteAnime
void func_ov217_021c09d0(ResearchCommon *common);
// pokebw2's ResearchCommon_StartPaletteAnime
void func_ov217_021c09d8(ResearchCommon *common, u32 index);
// pokebw2's ResearchCommon_StopPaletteAnime
void func_ov217_021c09e0(ResearchCommon *common);
// pokebw2's ResearchCommon_RestorePalette
void func_ov217_021c09ec(ResearchCommon *common);
// pokebw2's ResearchCommon_IsForceExit
// Whether the screens must leave the Research Radar at once, which the proc sets from a flag of the game system
BOOL func_ov217_021c09f8(ResearchCommon *common);

#endif // BWPORT_APP_RESEARCH_RADAR_RESEARCH_COMMON_H
