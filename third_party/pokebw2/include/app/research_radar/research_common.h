#ifndef POKEBW2_APP_RESEARCH_RADAR_RESEARCH_COMMON_H
#define POKEBW2_APP_RESEARCH_RADAR_RESEARCH_COMMON_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/touchpanel.h"
#include "struct_decls.h"

// The Research Radar's work shared by its screens (research_common.c): the return button at the bottom right of the
// touch screen, and which screen is shown and how it was left
// The names of these functions and types are ours

// The screens of the Research Radar's proc
enum {
    RESEARCH_SEQ_INIT,
    RESEARCH_SEQ_TOP,
    RESEARCH_SEQ_LIST,
    RESEARCH_SEQ_GRAPH,
    RESEARCH_SEQ_END,
};

// The BGs that the proc shows behind every screen: the main engine's frame, and the sub engine's background and its
// pattern
#define RESEARCH_BG_MAIN_FRAME 1
#define RESEARCH_BG_SUB_BACK 4
#define RESEARCH_BG_SUB_PATTERN 5

// The buttons of the common touch rectangles
#define RESEARCH_COMMON_BUTTON_RETURN 0

// A rectangle of the touch screen, as the screens' tables give them, which they copy into TouchRects
typedef struct {
    u8 left;
    u8 right;
    u8 top;
    u8 bottom;
} ResearchRect;

// A cell actor of a screen: what goes in its ClActorSetup, its unit, and the indices of its resources in the screen's
// table
typedef struct {
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
} ResearchActorSetup;

// A palette animation of a screen: the colors it animates, and how it runs when it is started
typedef struct {
    u16 *dst;
    const u16 *src;
    u8 count;
    u32 mode;
    // The color the animation blends toward
    u16 color;
} ResearchPaletteAnimeSetup;

ResearchCommon *ResearchCommon_Create(HeapID heapId, GameSystem *gsys);
void ResearchCommon_Delete(ResearchCommon *common);
HeapID ResearchCommon_GetHeapID(ResearchCommon *common);
GameSystem *ResearchCommon_GetGameSystem(ResearchCommon *common);
GameData *ResearchCommon_GetGameData(ResearchCommon *common);
const TouchRect *ResearchCommon_GetTouchRects(ResearchCommon *common);
// Records the screen being shown, and keeps the one before it
void ResearchCommon_SetSeq(ResearchCommon *common, u32 seq);
// Whether the last screen was left by touch, so that the next one starts without its cursor
void ResearchCommon_SetTouchMode(ResearchCommon *common, BOOL touch);
u32 ResearchCommon_GetPrevSeq(ResearchCommon *common);
BOOL ResearchCommon_GetTouchMode(ResearchCommon *common);
void ResearchCommon_UpdatePaletteAnime(ResearchCommon *common);
void ResearchCommon_StartPaletteAnime(ResearchCommon *common, u32 index);
void ResearchCommon_StopPaletteAnime(ResearchCommon *common);
void ResearchCommon_RestorePalette(ResearchCommon *common);
// Whether the screens must leave the Research Radar at once, which the proc sets from a flag of the game system
BOOL ResearchCommon_IsForceExit(ResearchCommon *common);
void ResearchCommon_SetForceExit(ResearchCommon *common);

#endif // POKEBW2_APP_RESEARCH_RADAR_RESEARCH_COMMON_H
