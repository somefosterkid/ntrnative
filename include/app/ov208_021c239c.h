#ifndef GUARD_APP_OV208_021C239C_H
#define GUARD_APP_OV208_021C239C_H

#include "types.h"
#include "app/townmap/townmap_data.h"
#include "app/zukan_detail.h"
#include "gfl/clact.h"
#include "nitro/gx.h"
#include "struct_decls.h"
#include "system/printsys.h"
#include "system/season.h"

typedef struct ZukanDetailWork ZukanDetailWork;

// The detail screen's habitat map page: the town map on the main screen with the areas where the Pokémon lives in
// each season, and a cursor to pick a place for the Pokédex's habitat list. pokebw2's ZukanDetailMapWork; Black's
// layout is the same (checked against every offset Black's code reaches), but for the windows and the actors that
// Black 2's button adds

#define ALL_PLANES                                                                                                         (GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 |                    GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD)

// Black 2's
enum {
    MAP_STATE_MAP,
    MAP_STATE_OPEN_START,
    MAP_STATE_OPEN,
    MAP_STATE_CHANGE,
    MAP_STATE_EXIT,
    MAP_STATE_PLACE,
    MAP_STATE_CLOSE_START,
    MAP_STATE_CLOSE,
    MAP_STATE_PLACE_CHANGE,
};

// No place is picked
#define NO_PLACE 0xff

// Black 2's
enum {
    MAP_SEQ_INIT,
    MAP_SEQ_FADE_IN_SUB,
    MAP_SEQ_FADE_IN,
    MAP_SEQ_WAIT_FADE_IN,
    MAP_SEQ_WAIT_STEP,
    MAP_SEQ_MAIN,
    MAP_SEQ_WAIT_EXIT,
    MAP_SEQ_WAIT_FADE_OUT,
    MAP_SEQ_FADE_OUT_SUB,
    MAP_SEQ_EXIT,
};

// Black 2's
// How the page ends
enum {
    MAP_EXIT_NONE,
    MAP_EXIT_PAGE,
    MAP_EXIT_SCREEN,
};

// Black 2's
// How the panel and the Pokémon's icon slide
enum {
    SLIDE_NONE,
    SLIDE_IN,
    SLIDE_DONE,
    SLIDE_OUT,
};

// Black 2's
// A place on the map: none, hidden as the player has not been there, or shown
enum {
    PLACE_NONE,
    PLACE_HIDDEN,
    PLACE_SHOWN,
};

// Black 2's
// What a VBlank changes
enum {
    VBLANK_NONE,
    VBLANK_DIM,
    VBLANK_HIGHLIGHT,
    VBLANK_RESET,
};

// Black's, without Black 2's ACTOR_BUTTON
// The cursor actors: the season's label and its arrows, the box saying the habitat is unknown, the cursor, the picked
// place's marker and the player's
enum {
    ACTOR_SEASON,
    ACTOR_SEASON_PREV,
    ACTOR_SEASON_NEXT,
    ACTOR_UNKNOWN,
    ACTOR_CURSOR,
    ACTOR_PLACE,
    ACTOR_PLAYER,
    ACTOR_COUNT,
};

// No season arrow is playing its animation
#define ACTOR_NONE ACTOR_COUNT

// Black's, without Black 2's WINDOW_BUTTON
// The windows: the Pokémon's name, the place's name, the season and the unknown habitat's message
enum {
    WINDOW_NAME,
    WINDOW_PLACE,
    WINDOW_SEASON,
    WINDOW_UNKNOWN,
    WINDOW_COUNT,
};

// Black 2's
// The kinds of habitat of a place in a season
#define HABITAT_LAND 0x07
#define HABITAT_WATER 0x18
#define HABITAT_FISHING 0x60

// A Pokémon's habitat: whether it is the same all year, as the first season, and by season whether it lives nowhere
// and the habitat of each place
typedef struct {
    u8 allYear;
    struct {
        u8 none;
        // Bits of HABITAT_*
        u8 places[TOWNMAP_PLACE_COUNT];
    } seasons[SEASON_COUNT];
} ZukanHabitat;

typedef struct {
    int state;
    ClActor *actor;
} MapPlace;

struct ZukanDetailWork {
    ClActUnit *unit;
    Font *font;
    ZukanDetailBackground *background;
    MsgData *msgData[2];
    BmpWin *windows[WINDOW_COUNT];
    BOOL transferPending[WINDOW_COUNT];
    PrintQueue *printQueues[WINDOW_COUNT];
    BmpWin *bgWindows[2];
    // The Pokémon's icons, two to slide one in as the other leaves
    u32 iconChars[2];
    u32 iconPalette;
    u32 iconCellAnims;
    ClActor *icons[2];
    int icon;
    u32 panelChars;
    int state;
    int panelSlide;
    int iconSlide;
    u8 season;
    u8 place;
    // Whether the panel was opened by touching the map, and where
    BOOL touched;
    u8 touchX;
    u8 touchY;
    // The step of a state's change, done over frames
    int step;
    BOOL active;
    // Whether the habitat changes with the seasons, and whether the Pokémon lives anywhere in the season shown
    BOOL bySeason;
    BOOL found;
    ZukanHabitat *habitat;
    u32 mapChars;
    u32 mapPalette;
    u32 mapCellAnims;
    u32 seasonChars;
    u32 seasonPalette;
    u32 seasonCellAnims;
    u32 areaChars;
    u32 areaPalette;
    u32 areaCellAnims;
    ClActor *actors[ACTOR_COUNT];
    // The season arrow playing its animation
    u8 arrow;
    // Where the Pokémon lives
    ClActor *areas[TOWNMAP_PLACE_COUNT];
    ClActUnit *unit2;
    ClActRenderer *renderer;
    int glowEv;
    u16 glowPhase;
    MapPlace places[TOWNMAP_PLACE_COUNT];
    void *townmap;
    // Each place's index in the town map's table
    u8 placeOrder[TOWNMAP_PLACE_COUNT];
    ArcTool *habitatArc;
    TCB *vblankTcb;
    // What the VBlank changes
    u8 vblankRequest;
    ZukanDetailBlend *blendMain;
    ZukanDetailBlend *blendSub;
    ZukanDetailPalFade *palFade;
    int exit;
    BOOL inputEnabled;
    // An arrow of the touch bar was touched, the Pokémon it moved to is shown, and the arrow's animation has played
    BOOL arrowPushed;
    BOOL arrowLoaded;
    BOOL arrowDone;
};

// pokebw2's ZukanDetailMap_ClearHighlight
void func_ov208_021c239c(ZukanDetailWork *wk);
// pokebw2's ZukanDetailMap_UpdateGlow
void func_ov208_021c23bc(ZukanDetailWork *wk);
// pokebw2's ZukanDetailMap_StartGlow
void func_ov208_021c2430(ZukanDetailWork *wk);
// pokebw2's ZukanDetailMap_SetActorsOpaque
void func_ov208_021c2458(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
// pokebw2's ZukanDetailMap_SetActorsBlended
void func_ov208_021c24a4(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
// pokebw2's ZukanDetailMap_SetIconsOpaque
void func_ov208_021c24f0(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
// pokebw2's ZukanDetailMap_SetIconsBlended
void func_ov208_021c252c(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);

#endif // GUARD_APP_OV208_021C239C_H
