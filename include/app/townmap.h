// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/townmap.h: the lines in use so far.
#ifndef BWPORT_APP_TOWNMAP_H
#define BWPORT_APP_TOWNMAP_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/clact.h"
#include "gfl/overlay.h"

// Black 2's, not checked against Black
// The town map's heap, a child of HEAPID_USER
#define HEAPID_TOWNMAP 0x44

// Black 2's, not checked against Black
// What the town map is opened for, the mode of its parameters
enum {
    // Picks where to fly
    TOWNMAP_MODE_FLY,
    // Shows the map
    TOWNMAP_MODE_VIEW,
    // Picks where to fly among all the places, arrived at or not
    TOWNMAP_MODE_FLY_ALL,
};

// Black 2's, not checked against Black
// How the town map was left, the result in its parameters
enum {
    // With the return button
    TOWNMAP_RESULT_RETURN,
    // With the close button
    TOWNMAP_RESULT_CLOSE,
    // With a place to fly to
    TOWNMAP_RESULT_FLY,
};

typedef struct TownMapGraphic TownMapGraphic;

// Black 2's; twinned code confirms the values of TOWNMAP_BG_COUNT in Black
// The BGs, as TownMapGraphic_GetBG numbers them
enum {
    // The touch bar's, BG0
    TOWNMAP_BG_BAR,
    // The place's name's and the message's, BG1
    TOWNMAP_BG_TEXT,
    // The two layers of the map, BG2 and BG3, which zoom and scroll together
    TOWNMAP_BG_MAP_FRONT,
    TOWNMAP_BG_MAP_BACK,
    // The upper screen's: BG6, which nothing draws on, the place's description on BG4, and the background on BG5
    TOWNMAP_BG_SUB_FRONT,
    TOWNMAP_BG_INFO,
    TOWNMAP_BG_SUB_BACK,
    TOWNMAP_BG_COUNT
};

// Black 2's; twinned code confirms the values of TOWNMAP_UNIT_MAIN in Black
enum {
    TOWNMAP_UNIT_MAIN,
    TOWNMAP_UNIT_COUNT
};

// Black 2's; twinned code confirms the values of TOWNMAP_ACTOR_COUNT in Black
// The actors the graphics create, all hidden at first
enum {
    // The plate behind the place's name
    TOWNMAP_ACTOR_NAME_PLATE,
    TOWNMAP_ACTOR_CURSOR,
    // The ring around the place the cursor points at
    TOWNMAP_ACTOR_RING,
    // In the player marker's animation; townmap.c makes its own
    TOWNMAP_ACTOR_PLAYER,
    TOWNMAP_ACTOR_COUNT
};

// pokebw2's TownMapGraphic_Create
TownMapGraphic *func_ov072_021bb6e8(HeapID heapId);
// pokebw2's TownMapGraphic_GetBG
// The BG of a TOWNMAP_BG_*
u8 func_ov072_021bb764(TownMapGraphic *graphic, u32 index);
// pokebw2's TownMapGraphic_Free
void func_ov072_021bb738(TownMapGraphic *graphic);
// pokebw2's TownMapGraphic_Update
void func_ov072_021bb758(TownMapGraphic *graphic);
// pokebw2's TownMapGraphic_GetRes
// The resources of the map's actors, for more actors of them
void func_ov072_021bb790(TownMapGraphic *graphic, u32 *chars, u32 *cellAnims, u32 *pltt);

// Black 2's, not checked against Black
#define OVERLAY_TOWNMAP OVERLAY_ID(144)

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's TownMapGraphic_GetUnit
ClActUnit *func_ov072_021bb778(TownMapGraphic *graphic, u32 index);
// pokebw2's TownMapGraphic_GetActor
ClActor *func_ov072_021bb784(TownMapGraphic *graphic, u32 index);

// The town map's proc, from the field menu
extern const GameProcFunctions data_ov072_021bbb80;

#endif // BWPORT_APP_TOWNMAP_H
