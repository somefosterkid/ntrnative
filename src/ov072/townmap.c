// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov144/townmap.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/townmap.h"
#include "app/townmap/townmap_data.h"
#include "app/ui/touchbar.h"
#include "app/ui/ui_scene.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "constants/zones.h"
#include "field/townmap_util.h"
#include "field/zone.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/calctool.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/math.h"
#include "save/records.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/town_map.h"

// The town map: the process, and the parts it is made of, each with its own work: the sequence, the touch bar with
// the zoom button, the cursor, the places and the player's marker, the map's BGs, the description on the upper
// screen, the message and the place's name, and the dragging of the map. The ROM names the file; all of the range is
// this one file, since every part is only reached from the process's functions and its .rodata is a single run. It
// ends with the empty VBlank task that the process adds, just before townmap_grh.c's func_ov072_021bb6e8

// The parameters of the process, which the field's callers allocate (they declare the layout themselves)
typedef struct {
    // TOWNMAP_MODE_*
    u32 mode;
    // TOWNMAP_RESULT_*
    u32 result;
    // The zone the player is in, then the zone picked to fly to
    u16 zoneId;
    u16 escapeZoneId;
    // Where to land in the zone picked
    u32 warpX;
    u32 warpZ;
    GameSystem *gsys;
} TownMapParam;

// A place's parameters that townmap_data.h doesn't name: where a flight to it lands
#define TOWNMAP_PARAM_WARP_X 13
#define TOWNMAP_PARAM_WARP_Z 14

// The center of the screen, which the map zooms about
#define TOWNMAP_CENTER_X 128
#define TOWNMAP_CENTER_Y 96
// Where the cursor starts when the player is nowhere on the map
#define TOWNMAP_CURSOR_X 128
#define TOWNMAP_CURSOR_Y 96
// The area the cursor moves in, and the area of the map that the touch screen drags
#define TOWNMAP_CURSOR_LEFT 0
#define TOWNMAP_CURSOR_RIGHT 248
#define TOWNMAP_CURSOR_TOP 8
#define TOWNMAP_CURSOR_BOTTOM 168
// How far the zoomed map scrolls from the center, and how many frames the zoom takes
#define TOWNMAP_SCROLL_TOP (-96)
#define TOWNMAP_SCROLL_BOTTOM 72
#define TOWNMAP_SCROLL_LEFT (-128)
#define TOWNMAP_SCROLL_RIGHT 128
#define TOWNMAP_ZOOM_FRAMES 10
// The places' markers scroll in their own limits
#define TOWNMAP_PLACES_TOP (-72)
#define TOWNMAP_PLACES_BOTTOM 96

// What the touch bar's own icons are, and the zoom button, which the bar doesn't know of
#define TOWNMAP_ICON_CLOSE 0
#define TOWNMAP_ICON_RETURN 1
#define TOWNMAP_ICON_ZOOM TOUCHBAR_ICON_CUSTOM
#define TOWNMAP_ICON_NONE 0xffffffff

// The animations of the map's cells
enum {
    TOWNMAP_ANIM_NAME_PLATE = 0,
    TOWNMAP_ANIM_CITY = 1,
    TOWNMAP_ANIM_CITY_SELECTED = 2,
    TOWNMAP_ANIM_SELECTED = 3,
    TOWNMAP_ANIM_PLAYER = 6,
    // The zoom button, at rest and pressed, when zoomed out and in
    TOWNMAP_ANIM_ZOOM_IN = 7,
    TOWNMAP_ANIM_ZOOM_OUT = 8,
    TOWNMAP_ANIM_ZOOM_IN_PRESSED = 9,
    TOWNMAP_ANIM_ZOOM_OUT_PRESSED = 10,
    TOWNMAP_ANIM_NONE = 0xff,
};

typedef struct {
    s32 x;
    s32 y;
} TownMapPoint;
// pokebw2's sTownMapCenter
static const TownMapPoint data_ov072_021bbb78 = { TOWNMAP_CENTER_X, TOWNMAP_CENTER_Y };

typedef struct TownMapSeq TownMapSeq;
typedef void (*TownMapSeqFunc)(TownMapSeq *seq, u32 *state, void *work);

struct TownMapSeq {
    TownMapSeqFunc func;
    BOOL ended;
    u32 state;
    void *work;
};

typedef struct {
    u32 pltt;
    u32 chars;
    u32 cellAnims;
    TouchBar *bar;
    GameData *gameData;
    ClActor *zoomButton;
    u32 decided;
    u32 state;
} TownMapAppBar;

typedef struct TownMapPlace TownMapPlace;

// The parameters a place keeps scaled: TOWNMAP_PARAM_X to TOWNMAP_PARAM_HIT_RADIUS, the last of which is a length
#define TOWNMAP_PLACE_PARAM_COUNT 9

typedef struct {
    ClActor *cursor;
    ClActor *ring;
    ClActorPos pos;
    BOOL decided;
    // Whether the cursor is pulled to the nearest place
    BOOL pull;
    // The place the ring is around
    TownMapPlace *place;
} TownMapCursor;

struct TownMapPlace {
    ClActor *actor;
    void *data;
    u32 index;
    s32 params[TOWNMAP_PLACE_PARAM_COUNT];
    u8 anim;
    u8 animSelected;
    u8 animNow;
    fx32 scale;
    BOOL arrived;
    BOOL canFly;
    BOOL hidden;
};

typedef struct {
    ClActor *actor;
    TownMapPlace *place;
} TownMapPlayer;

typedef struct {
    GameData *gameData;
    u32 unk4;
    TownMapPlace *places;
    u32 count;
    TownMapPoint scroll;
    s16 limitTop;
    s16 limitLeft;
    s16 limitRight;
    s16 limitBottom;
    TownMapPlayer player;
} TownMapPlaces;

typedef struct {
    u8 bgBack;
    u8 bgFront;
    u16 frame;
    s16 limitTop;
    s16 limitLeft;
    s16 limitRight;
    s16 limitBottom;
    // Set for the frame the zoom ends in
    BOOL finished;
    BOOL moving;
    fx32 scale;
    fx32 scaleEnd;
    fx32 scaleStart;
    TownMapPoint pos;
    TownMapPoint end;
    TownMapPoint start;
} TownMapMap;

typedef struct {
    BmpWin *window;
    StrBuf *strbuf;
    Font *font;
    MsgData *msgData;
} TownMapMsgWin;

typedef struct {
    TownMapMsgWin msgWin;
    MsgData *msgData;
} TownMapInfo;

typedef struct {
    TownMapMsgWin msgWin;
    ClActor *plate;
    TownMapPlace *place;
    // Waiting for the plate's animation before the name is drawn
    BOOL opening;
    BOOL requested;
} TownMapPlaceName;

typedef struct {
    VecFx32 start;
    VecFx32 now;
    u32 unk18;
    BOOL dragging;
    // Whether the next drag is measured from where this one ended
    BOOL restart;
    VecFx32 delta;
} TownMapTouch;

typedef struct {
    TownMapGraphic *graphic;
    TownMapSeq seq;
    TownMapAppBar appBar;
    void *data;
    TownMapCursor cursor;
    TownMapPlaces places;
    TownMapMap map;
    TownMapInfo info;
    TownMapMsgWin msgWin;
    TownMapPlaceName placeName;
    TownMapTouch touch;
    Font *font;
    MsgData *placeNames;
    MsgData *msgData;
    TownMapParam *param;
    TownMapPlace *selected;
    BOOL zoomed;
    TCB *vblankTask;
    u32 unk14C;
} TownMapWork;

BOOL func_ov072_021b95a0(GameProc *proc, u32 *state, void *paramPtr, void *workPtr);
static BOOL func_ov072_021b9858(GameProc *proc, u32 *state, void *paramPtr, void *workPtr);
static BOOL func_ov072_021b98f8(GameProc *proc, u32 *state, void *paramPtr, void *workPtr);
void func_ov072_021b991c(TownMapSeq *seq, void *work, TownMapSeqFunc func);
void func_ov072_021b9938(TownMapSeq *seq);
static void func_ov072_021b9944(TownMapSeq *seq);
static BOOL func_ov072_021b9958(TownMapSeq *seq);
void func_ov072_021b995c(TownMapSeq *seq, TownMapSeqFunc func);
void func_ov072_021b9964(TownMapSeq *seq);
void func_ov072_021b996c(TownMapSeq *seq, u32 *state, void *data);
void func_ov072_021b99ac(TownMapSeq *seq, u32 *state, void *data);
void func_ov072_021b99e8(TownMapSeq *seq, u32 *state, void *data);
void func_ov072_021b9e38(TownMapAppBar *appBar, u32 mode, ClActUnit *unit, u8 bg, u8 bgPalette, u8 objPalette, GameData *gameData, HeapID heapId);
void func_ov072_021b9f20(TownMapAppBar *appBar);
void func_ov072_021b9f50(TownMapAppBar *appBar, TownMapMap *map);
u32 func_ov072_021ba028(TownMapAppBar *appBar);
BOOL func_ov072_021ba040(TownMapAppBar *appBar);
void func_ov072_021ba04c(TownMapCursor *cursor, ClActor *actor, ClActor *ring, HeapID heapId);
void func_ov072_021ba0d8(TownMapCursor *cursor);
void func_ov072_021ba0e4(TownMapCursor *cursor, TownMapPlaces *places);
void func_ov072_021ba3bc(const TownMapCursor *cursor, TownMapPoint *pos);
void func_ov072_021ba3cc(TownMapCursor *cursor, const TownMapPoint *pos);
BOOL func_ov072_021ba3e8(const TownMapCursor *cursor);
u32 func_ov072_021ba3ec(const TownMapCursor *cursor);
void func_ov072_021ba3f4(TownMapCursor *cursor, TownMapPlace *place, TownMapPoint *scroll);
void func_ov072_021ba450(TownMapCursor *cursor, const TownMapPoint *scroll);
TownMapPlace *func_ov072_021ba488(const TownMapCursor *cursor);
void func_ov072_021ba48c(TownMapCursor *cursor, BOOL pull);
void func_ov072_021ba490(TownMapPlaces *places, u16 zoneId, u16 escapeZoneId, void *data, TownMapGraphic *graphic, GameData *gameData, HeapID heapId, BOOL all);
void func_ov072_021ba570(TownMapPlaces *places);
void func_ov072_021ba5ac(TownMapPlaces *places);
void func_ov072_021ba5b0(TownMapPlaces *places, fx32 scale, const TownMapPoint *center);
void func_ov072_021ba638(TownMapPlaces *places, const TownMapPoint *scroll);
void func_ov072_021ba69c(const TownMapPlaces *places, TownMapPoint *scroll);
void func_ov072_021ba6a8(TownMapPlaces *places, const TownMapPoint *move);
TownMapPlace *func_ov072_021ba6c8(const TownMapPlaces *places, const TownMapCursor *cursor);
TownMapPlace *func_ov072_021ba710(const TownMapPlaces *places, const TownMapCursor *cursor);
s32 func_ov072_021ba774(TownMapPlace *place, s32 param);
void func_ov072_021ba7a4(TownMapPlaces *places, const TownMapPlace *place);
void func_ov072_021ba7d4(TownMapPlaces *places);
TownMapPlace *func_ov072_021ba7f8(const TownMapPlaces *places, u16 zoneId, u16 escapeZoneId);
BOOL func_ov072_021ba834(const TownMapPlace *place);
void func_ov072_021ba86c(TownMapPlace *place, void *data, u32 index, ClActUnit *unit, u32 chars, u32 cellAnims, u32 pltt, GameData *gameData, HeapID heapId, BOOL all);
void func_ov072_021baa14(TownMapPlace *place);
void func_ov072_021baa30(TownMapPlace *place, BOOL visible);
static BOOL func_ov072_021baa90(const TownMapPlace *place, const TownMapPoint *pos, const TownMapPoint *scroll);
BOOL func_ov072_021bacb0(TownMapPlace *place, const TownMapPoint *pos, const TownMapPoint *scroll, u32 *distance);
void func_ov072_021bad10(TownMapPlace *place, fx32 scale, const TownMapPoint *center);
void func_ov072_021bad84(TownMapPlace *place, const TownMapPoint *scroll);
void func_ov072_021badbc(TownMapPlace *place, u8 anim);
void func_ov072_021badf4(TownMapPlace *place);
void func_ov072_021bae44(TownMapPlace *place);
BOOL func_ov010_02166cd0(GameData *gameData, u16 flag);
void func_ov072_021bb4b4(TownMapPlaceName *placeName, u8 bg, Font *font, MsgData *placeNames, ClActor *plate, HeapID heapId);
void func_ov072_021bb1bc(TownMapInfo *info, u8 bg, Font *font, MsgData *msgData, MsgData *placeNames, HeapID heapId);
void func_ov072_021bb5d4(TownMapTouch *touch, HeapID heapId);
void func_ov072_021bb5ac(TownMapPlaceName *placeName, TownMapPlace *place);
void func_ov072_021bb5b4(TownMapPlaceName *placeName, BOOL visible);
void func_ov072_021bb548(TownMapPlaceName *placeName);
void func_ov072_021bae9c(TownMapPlayer *player, TownMapPlace *place, ClActUnit *unit, u32 chars, u32 cellAnims, u32 pltt, HeapID heapId);
void func_ov072_021baf24(TownMapPlayer *player);
void func_ov072_021baf3c(TownMapPlayer *player, const TownMapPoint *scroll);
void func_ov072_021baf74(TownMapMap *map, u8 bgBack, u8 bgFront, HeapID heapId);
void func_ov072_021baf9c(TownMapMap *map);
static void func_ov072_021bafa8(TownMapMap *map, TownMapPlaces *places);
void func_ov072_021bb0b4(TownMapMap *map, fx32 scale, const TownMapPoint *start, const TownMapPoint *end);
BOOL func_ov072_021bb0dc(const TownMapMap *map);
BOOL func_ov072_021bb0e0(const TownMapMap *map);
void func_ov072_021bb0e4(TownMapMap *map, const TownMapPoint *pos);
void func_ov072_021bb190(const TownMapMap *map, TownMapPoint *pos);
void func_ov072_021bb19c(TownMapMap *map, const TownMapPoint *move);
void func_ov072_021bb204(TownMapMsgWin *msgWin);
void func_ov072_021bb220(TownMapInfo *info, TownMapPlace *place);
void func_ov072_021bb5e0(TownMapTouch *touch);
void func_ov072_021bb534(TownMapPlaceName *placeName);
void func_ov072_021bb42c(TownMapMsgWin *msgWin);
void func_ov072_021bbb34(void *data);
void func_ov072_021bb738(TownMapGraphic *graphic);
void func_ov072_021bb3d0(TownMapMsgWin *msgWin);
void func_ov072_021bb3e8(TownMapMsgWin *msgWin, u8 bg, Font *font, MsgData *msgData, u8 x, u8 y, u8 width,
                               u8 height, HeapID heapId);
void func_ov072_021bb448(TownMapMsgWin *msgWin, u32 messageId, u16 x, u16 y);
void func_ov072_021bb5ec(TownMapTouch *touch);
BOOL func_ov072_021bb6ac(const TownMapTouch *touch, VecFx32 *delta, u32 *unk18);
void func_ov072_021bb6dc(TownMapTouch *touch);
void func_ov072_021bb6e4(TCB *tcb, void *data);


// pokebw2's TOWNMAP_PROC_FUNCTIONS
// pokebw2's TownMap_ProcExit
// pokebw2's TownMap_ProcInit
// pokebw2's TownMap_ProcMain
const GameProcFunctions data_ov072_021bbb80 = { func_ov072_021b95a0, func_ov072_021b98f8, func_ov072_021b9858 };


BOOL func_ov072_021b95a0(GameProc *proc, u32 *state, void *paramPtr, void *workPtr) {
    TownMapWork *work;
    TownMapParam *param = paramPtr;
    BOOL all;
    GameData *gameData;
    TownMapPlace *place;
    TownMapPoint pos;
    TownMapPoint scroll;

    GFL_OvlLoad(OVERLAY_ID(67));
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_TOWNMAP, 0x60000);
    work = GFL_ProcInitSubsystem(proc, sizeof(TownMapWork), HEAPID_TOWNMAP);
    MI_CpuFill8(work, 0, sizeof(TownMapWork));
    work->param = param;
    all = work->param->mode == TOWNMAP_MODE_FLY_ALL;
    work->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_TOWNMAP);
    work->placeNames = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_PLACE_NAMES, HEAPID_TOWNMAP);
    work->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_TOWN_MAP, HEAPID_TOWNMAP);
    work->graphic = func_ov072_021bb6e8(HEAPID_TOWNMAP);
    work->data = func_ov072_021bbb24(HEAPID_TOWNMAP);
    func_ov072_021b991c(&work->seq, work, func_ov072_021b996c);
    gameData = GSYS_GetGameData(work->param->gsys);
    if (work->param->mode != TOWNMAP_MODE_VIEW) {
        func_ov072_021bb3e8(&work->msgWin, func_ov072_021bb764(work->graphic, TOWNMAP_BG_TEXT), work->font,
            work->msgData, 0, 21, 24, 3, HEAPID_TOWNMAP);
        GFL_TextRndUpdateColorIndexLUT(15, 1, 0);
        func_ov072_021bb448(&work->msgWin, TownMap_Text_000, 16, 0);
        func_0201dd68();
    }
    func_ov072_021b9e38(&work->appBar, work->param->mode, func_ov072_021bb778(work->graphic, TOWNMAP_UNIT_MAIN),
        func_ov072_021bb764(work->graphic, TOWNMAP_BG_BAR), 15, 8, gameData, HEAPID_TOWNMAP);
    func_ov072_021bb4b4(&work->placeName, func_ov072_021bb764(work->graphic, TOWNMAP_BG_TEXT), work->font,
        work->placeNames, func_ov072_021bb784(work->graphic, TOWNMAP_ACTOR_NAME_PLATE), HEAPID_TOWNMAP);
    GFL_BGSysMoveBG(func_ov072_021bb764(work->graphic, TOWNMAP_BG_TEXT), BG_MOVE_SET_Y, -4);
    func_ov072_021ba490(&work->places, param->zoneId, param->escapeZoneId, work->data, work->graphic,
        GSYS_GetGameData(work->param->gsys), HEAPID_TOWNMAP, all);
    func_ov072_021ba04c(&work->cursor, func_ov072_021bb784(work->graphic, TOWNMAP_ACTOR_CURSOR),
        func_ov072_021bb784(work->graphic, TOWNMAP_ACTOR_RING), HEAPID_TOWNMAP);
    place = func_ov072_021ba7f8(&work->places, param->zoneId, param->escapeZoneId);
    if (place != NULL) {
        pos.x = func_ov072_021ba774(place, 4);
        pos.y = func_ov072_021ba774(place, 5);
    } else {
        pos.x = TOWNMAP_CURSOR_X;
        pos.y = TOWNMAP_CURSOR_Y;
    }
    func_ov072_021ba3cc(&work->cursor, &pos);
    func_ov072_021baf74(&work->map, func_ov072_021bb764(work->graphic, TOWNMAP_BG_MAP_BACK),
        func_ov072_021bb764(work->graphic, TOWNMAP_BG_MAP_FRONT), HEAPID_TOWNMAP);
    func_ov072_021bb1bc(&work->info, func_ov072_021bb764(work->graphic, TOWNMAP_BG_INFO), work->font,
        work->placeNames, work->msgData, HEAPID_TOWNMAP);
    func_ov072_021bb5d4(&work->touch, HEAPID_TOWNMAP);
    place = func_ov072_021ba6c8(&work->places, &work->cursor);
    if (place != NULL) {
        func_ov072_021bb5ac(&work->placeName, place);
        func_ov072_021bb220(&work->info, place);
        work->selected = place;
        func_ov072_021ba69c(&work->places, &scroll);
        func_ov072_021ba3f4(&work->cursor, place, &scroll);
    }
    if (func_0203cd98()) {
        func_0203d34c(TRUE, HEAPID_TOWNMAP);
    }
    work->vblankTask = GFL_VBlankTCBAdd(func_ov072_021bb6e4, work, 0);
    return TRUE;
}

static BOOL func_ov072_021b9858(GameProc *proc, u32 *state, void *paramPtr, void *workPtr) {
    TownMapWork *work = workPtr;

    GFL_TCBRemove(work->vblankTask);
    func_ov072_021bb5e0(&work->touch);
    func_ov072_021bb204(&work->info.msgWin);
    func_ov072_021baf9c(&work->map);
    func_ov072_021ba570(&work->places);
    func_ov072_021ba0d8(&work->cursor);
    func_ov072_021bb534(&work->placeName);
    if (work->param->mode != TOWNMAP_MODE_VIEW) {
        func_ov072_021bb42c(&work->msgWin);
    }
    func_ov072_021b9f20(&work->appBar);
    func_ov072_021b9938(&work->seq);
    func_ov072_021bbb34(work->data);
    func_ov072_021bb738(work->graphic);
    GFL_MsgDataFree(work->msgData);
    GFL_MsgDataFree(work->placeNames);
    GFL_FontFree(work->font);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_TOWNMAP);
    GFL_OvlUnload(OVERLAY_ID(67));
    return TRUE;
}

static BOOL func_ov072_021b98f8(GameProc *proc, u32 *state, void *paramPtr, void *workPtr) {
    TownMapWork *work = workPtr;

    func_ov072_021b9944(&work->seq);
    func_ov072_021bb758(work->graphic);
    if (func_ov072_021b9958(&work->seq)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's TownMapSeq_Init
void func_ov072_021b991c(TownMapSeq *seq, void *work, TownMapSeqFunc func) {
    MI_CpuFill8(seq, 0, sizeof(TownMapSeq));
    seq->work = work;
    func_ov072_021b995c(seq, func);
}

// pokebw2's TownMapSeq_Free
void func_ov072_021b9938(TownMapSeq *seq) {
    MI_CpuFill8(seq, 0, sizeof(TownMapSeq));
}

// pokebw2's TownMapSeq_Main
static void func_ov072_021b9944(TownMapSeq *seq) {
    if (!seq->ended) {
        seq->func(seq, &seq->state, seq->work);
    }
}

// pokebw2's TownMapSeq_IsEnd
static BOOL func_ov072_021b9958(TownMapSeq *seq) {
    return seq->ended;
}

// pokebw2's TownMapSeq_SetNext
void func_ov072_021b995c(TownMapSeq *seq, TownMapSeqFunc func) {
    seq->func = func;
    seq->state = 0;
}

// pokebw2's TownMapSeq_End
void func_ov072_021b9964(TownMapSeq *seq) {
    seq->ended = TRUE;
}

// pokebw2's TownMap_SeqFadeIn
void func_ov072_021b996c(TownMapSeq *seq, u32 *state, void *data) {
    switch (*state) {
    case 0:
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 0);
        *state = 1;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            *state = 2;
        }
        break;
    case 2:
        func_ov072_021b995c(seq, func_ov072_021b99e8);
        break;
    }
}

// pokebw2's TownMap_SeqFadeOut
void func_ov072_021b99ac(TownMapSeq *seq, u32 *state, void *data) {
    switch (*state) {
    case 0:
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 0);
        *state = 1;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            *state = 2;
        }
        break;
    case 2:
        func_ov072_021b9964(seq);
        break;
    }
}

void func_ov072_021b99e8(TownMapSeq *seq, u32 *state, void *data) {
    TownMapWork *work = data;
    BOOL scrolled = FALSE;

    if (!func_ov072_021ba040(&work->appBar)) {
        if (work->zoomed && !func_ov072_021bb0e0(&work->map)) {
            TownMapPoint pos;
            TownMapPoint scroll = { 0, 0 };
            u32 keys;

            func_ov072_021ba3bc(&work->cursor, &pos);
            keys = func_ov072_021ba3ec(&work->cursor);
            if (pos.y < 16 && (keys & PAD_KEY_UP)) {
                scroll.y -= 4;
                scrolled = TRUE;
            }
            if (pos.y <= TOWNMAP_CURSOR_BOTTOM && pos.y > 136 && (keys & PAD_KEY_DOWN)) {
                scroll.y += 4;
                scrolled = TRUE;
            }
            if (pos.x < 16 && (keys & PAD_KEY_LEFT)) {
                scroll.x -= 4;
                scrolled = TRUE;
            }
            if (pos.x > 240 && (keys & PAD_KEY_RIGHT)) {
                scroll.x += 4;
                scrolled = TRUE;
            }
            if (!func_ov072_021bb0e0(&work->map)) {
                VecFx32 delta;
                u32 unk18;

                if (func_ov072_021bb6ac(&work->touch, &delta, &unk18)) {
                    scroll.x = -FX_Mul(delta.x, FX32_ONE) >> FX32_SHIFT;
                    scroll.y = -FX_Mul(delta.y, FX32_ONE) >> FX32_SHIFT;
                    func_ov072_021bb6dc(&work->touch);
                    scrolled = TRUE;
                }
            }
            if (scrolled) {
                TownMapPoint placesScroll;
                TownMapPoint ringScroll;

                func_ov072_021ba69c(&work->places, &placesScroll);
                func_ov072_021bb19c(&work->map, &scroll);
                scroll.x *= -1;
                scroll.y *= -1;
                func_ov072_021ba6a8(&work->places, &scroll);
                func_ov072_021ba69c(&work->places, &ringScroll);
                func_ov072_021ba450(&work->cursor, &ringScroll);
            }
        }
        if (!scrolled && !func_ov072_021bb0e0(&work->map)) {
            BOOL decided;

            if (work->param->mode == TOWNMAP_MODE_FLY || work->param->mode == TOWNMAP_MODE_FLY_ALL) {
                TownMapPlace *ringed = func_ov072_021ba488(&work->cursor);
                TownMapPlace *hit = func_ov072_021ba6c8(&work->places, &work->cursor);

                if (ringed != NULL && ringed == hit && func_ov072_021ba3e8(&work->cursor) &&
                    func_ov072_021ba774(ringed, 12)) {
                    work->param->result = TOWNMAP_RESULT_FLY;
                    work->param->zoneId = func_ov072_021ba774(ringed, 0);
                    work->param->warpX = func_ov072_021ba774(ringed, 13);
                    work->param->warpZ = func_ov072_021ba774(ringed, 14);
                    GFL_SndSEPlay(SEQ_SE_DECIDE1);
                    func_ov072_021b995c(seq, func_ov072_021b99ac);
                }
            }
            if (func_020355c4() == FALSE) {
                decided = TRUE;
            } else {
                decided = func_ov072_021ba3e8(&work->cursor);
            }
            if (decided) {
                TownMapPlace *place = func_ov072_021ba6c8(&work->places, &work->cursor);

                if (place != work->selected && place != NULL) {
                    TownMapPoint placesScroll;

                    GFL_SndSEPlay(SEQ_SE_SELECT1);
                    func_ov072_021bb220(&work->info, place);
                    func_ov072_021ba7a4(&work->places, place);
                    work->selected = place;
                    func_ov072_021ba69c(&work->places, &placesScroll);
                    func_ov072_021ba3f4(&work->cursor, place, &placesScroll);
                    if (!work->zoomed) {
                        func_ov072_021bb5ac(&work->placeName, place);
                    }
                } else if (place == NULL && work->selected != NULL) {
                    func_ov072_021bb3d0(&work->info.msgWin);
                    func_ov072_021bb5b4(&work->placeName, FALSE);
                    func_ov072_021ba7d4(&work->places);
                    func_ov072_021ba3f4(&work->cursor, NULL, NULL);
                    work->selected = NULL;
                }
            }
        }
        if (work->zoomed) {
            func_ov072_021bb5b4(&work->placeName, FALSE);
        }
        func_ov072_021ba48c(&work->cursor, scrolled ? FALSE : TRUE);
    }
    func_ov072_021b9f50(&work->appBar, &work->map);
    if (!func_ov072_021bb0e0(&work->map)) {
        func_ov072_021ba0e4(&work->cursor, &work->places);
    }
    func_ov072_021ba5ac(&work->places);
    func_ov072_021bafa8(&work->map, &work->places);
    func_ov072_021bb548(&work->placeName);
    func_ov072_021bb5ec(&work->touch);
    switch (func_ov072_021ba028(&work->appBar)) {
    case TOWNMAP_ICON_CLOSE:
        work->param->result = TOWNMAP_RESULT_CLOSE;
        func_ov072_021b995c(seq, func_ov072_021b99ac);
        break;
    case TOWNMAP_ICON_RETURN:
        work->param->result = TOWNMAP_RESULT_RETURN;
        func_ov072_021b995c(seq, func_ov072_021b99ac);
        break;
    case TOWNMAP_ICON_ZOOM:
        if (!work->zoomed && !func_ov072_021bb0e0(&work->map)) {
            TownMapPoint pos;

            func_ov072_021ba3bc(&work->cursor, &pos);
            func_ov072_021bb0b4(&work->map, 2 * FX32_ONE, &data_ov072_021bbb78, &pos);
            GFL_SndSEPlay(SEQ_SE_SYS_09);
            if (work->selected != NULL) {
                func_ov072_021bb3d0(&work->info.msgWin);
                func_ov072_021bb5b4(&work->placeName, FALSE);
                func_ov072_021ba7d4(&work->places);
                func_ov072_021ba3f4(&work->cursor, NULL, NULL);
                work->selected = NULL;
            }
            work->zoomed = TRUE;
        } else if (work->zoomed == TRUE && !func_ov072_021bb0e0(&work->map)) {
            TownMapPoint pos;

            func_ov072_021bb190(&work->map, &pos);
            pos.x += TOWNMAP_CENTER_X;
            pos.y += TOWNMAP_CENTER_Y;
            func_ov072_021bb0b4(&work->map, FX32_ONE, &pos, &data_ov072_021bbb78);
            GFL_SndSEPlay(SEQ_SE_SYS_10);
            if (work->selected != NULL) {
                func_ov072_021bb3d0(&work->info.msgWin);
                func_ov072_021bb5b4(&work->placeName, FALSE);
                func_ov072_021ba7d4(&work->places);
                func_ov072_021ba3f4(&work->cursor, NULL, NULL);
                work->selected = NULL;
            }
            work->zoomed = FALSE;
        }
        break;
    }
    func_ov072_021bb0dc(&work->map);
}


void func_ov072_021b9e38(TownMapAppBar *appBar, u32 mode, ClActUnit *unit, u8 bg, u8 bgPalette, u8 objPalette,
    GameData *gameData, HeapID heapId) {
    ArcTool *arc;

    MI_CpuFill8(appBar, 0, sizeof(TownMapAppBar));
    appBar->gameData = gameData;
    arc = GFL_ArcSysCreateFileHandle(ARCID_TOWNMAP_GRA, heapId);
    appBar->pltt = func_0204ac18(arc, 2, CLACT_VRAM_MAIN, 0, 0, 4, heapId);
    appBar->chars = func_0204a6c8(arc, 8, FALSE, CLACT_VRAM_MAIN, heapId);
    appBar->cellAnims = func_0204af28(arc, 12, 19, heapId);
    GFL_ArcToolFree(arc);
    {
        ClActorSetup setup;

        MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
        setup.x = 168;
        setup.y = 168;
        setup.sequence = TOWNMAP_ANIM_ZOOM_IN;
        appBar->zoomButton = func_0204b294(unit, appBar->chars, appBar->pltt, appBar->cellAnims, &setup,
            CLACT_SURFACE_MAIN, heapId);
        func_0204b92c(appBar->zoomButton, TRUE);
        func_0204b974(appBar->zoomButton);
    }
    {
        TouchBarSetup touchSetup;
        TouchBarItem items[] = {
            { TOWNMAP_ICON_RETURN, { 232, 168 } },
            { TOWNMAP_ICON_CLOSE, { 200, 168 } },
        };

        MI_CpuFill8(&touchSetup, 0, sizeof(TouchBarSetup));
        touchSetup.items = items;
        touchSetup.count = NELEMS(items);
        touchSetup.unit = unit;
        touchSetup.bg = bg;
        touchSetup.bgPalette = bgPalette;
        touchSetup.objPalette = objPalette + 4;
        touchSetup.vramType = 2;
        appBar->bar = func_ov067_021b62e0(&touchSetup, heapId);
        func_ov067_021b6520(appBar->bar, 10);
    }
}


// pokebw2's TownMapAppBar_Free
void func_ov072_021b9f20(TownMapAppBar *appBar) {
    func_ov067_021b639c(appBar->bar);
    func_0204b3b4(appBar->zoomButton);
    func_0204afd8(appBar->cellAnims);
    func_0204a8d4(appBar->chars);
    func_0204ada4(appBar->pltt);
    MI_CpuFill8(appBar, 0, sizeof(TownMapAppBar));
}

// pokebw2's TownMapAppBar_Main
void func_ov072_021b9f50(TownMapAppBar *appBar, TownMapMap *map) {
    func_ov067_021b63d0(appBar->bar);
    appBar->decided = TOWNMAP_ICON_NONE;
    switch (appBar->state) {
    case 0: {
        BOOL pressed = FALSE;
        u32 x;
        u32 y;
        ClActorPos pos;

        func_0204b454(appBar->zoomButton, &pos, CLACT_SURFACE_MAIN);
        if (func_02035d20(&x, &y)) {
            if (x - pos.x <= 24 && y - pos.y <= 24) {
                func_020355d8(TRUE);
                pressed = TRUE;
            }
        } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) {
            func_020355d8(FALSE);
            pressed = TRUE;
        }
        if (pressed && !func_0204b98c(appBar->zoomButton) && !func_ov072_021bb0e0(map)) {
            if (func_0204b898(appBar->zoomButton) == TOWNMAP_ANIM_ZOOM_IN) {
                func_0204b878(appBar->zoomButton, TOWNMAP_ANIM_ZOOM_IN_PRESSED);
            } else {
                func_0204b878(appBar->zoomButton, TOWNMAP_ANIM_ZOOM_OUT_PRESSED);
            }
            func_0204b95c(appBar->zoomButton);
            appBar->decided = TOWNMAP_ICON_ZOOM;
            appBar->state = 1;
        }
        break;
    }
    case 1:
        if (!func_0204b98c(appBar->zoomButton)) {
            if (func_0204b898(appBar->zoomButton) == TOWNMAP_ANIM_ZOOM_IN_PRESSED) {
                func_0204b878(appBar->zoomButton, TOWNMAP_ANIM_ZOOM_OUT);
            } else {
                func_0204b878(appBar->zoomButton, TOWNMAP_ANIM_ZOOM_IN);
            }
            appBar->state = 0;
        }
        break;
    }
}

// The icon decided: one of the touch bar's, or the zoom button in the frame it is pressed
// pokebw2's TownMapAppBar_GetDecided
u32 func_ov072_021ba028(TownMapAppBar *appBar) {
    u32 decided = func_ov067_021b6448(appBar->bar);

    if (decided == TOWNMAP_ICON_NONE) {
        decided = appBar->decided;
    }
    return decided;
}

BOOL func_ov072_021ba040(TownMapAppBar *appBar) {
    return func_ov067_021b6498(appBar->bar);
}


// pokebw2's TownMapCursor_Init
void func_ov072_021ba04c(TownMapCursor *cursor, ClActor *actor, ClActor *ring, HeapID heapId) {
    MI_CpuFill8(cursor, 0, sizeof(TownMapCursor));
    cursor->pos.x = TOWNMAP_CURSOR_X;
    cursor->pos.y = TOWNMAP_CURSOR_Y;
    cursor->cursor = actor;
    cursor->ring = ring;
    func_0204b404(cursor->cursor, &cursor->pos, CLACT_SURFACE_MAIN);
    func_0204b404(cursor->ring, &cursor->pos, CLACT_SURFACE_MAIN);
    func_0204b3dc(cursor->cursor, TRUE);
    func_0204b3dc(cursor->ring, FALSE);
    func_0204b84c(cursor->cursor, 0);
    func_0204b800(cursor->cursor, 0);
    func_0204b84c(cursor->ring, 2);
    func_0204b800(cursor->ring, 3);
    func_0204b92c(cursor->ring, TRUE);
    // The cursor shows only while the keys are used
    if (!func_020355c4()) {
        func_0204b3dc(cursor->cursor, TRUE);
    } else {
        func_0204b3dc(cursor->cursor, FALSE);
    }
}

// pokebw2's TownMapCursor_Free
void func_ov072_021ba0d8(TownMapCursor *cursor) {
    MI_CpuFill8(cursor, 0, sizeof(TownMapCursor));
}

void func_ov072_021ba0e4(TownMapCursor *cursor, TownMapPlaces *places) {
    VecFx32 move = { 0, 0, 0 };
    VecFx32 pull = { 0, 0, 0 };
    BOOL moved = FALSE;

    cursor->decided = FALSE;
    if (GCTX_HIDGetHeldKeys() & PAD_KEY_UP) {
        func_020355d8(FALSE);
        move.y -= FX32_ONE;
        moved = TRUE;
    }
    if (GCTX_HIDGetHeldKeys() & PAD_KEY_DOWN) {
        func_020355d8(FALSE);
        move.y += FX32_ONE;
        moved = TRUE;
    }
    if (GCTX_HIDGetHeldKeys() & PAD_KEY_LEFT) {
        func_020355d8(FALSE);
        move.x -= FX32_ONE;
        moved = TRUE;
    }
    if (GCTX_HIDGetHeldKeys() & PAD_KEY_RIGHT) {
        func_020355d8(FALSE);
        move.x += FX32_ONE;
        moved = TRUE;
    }
    if (cursor->pull && !func_020355c4()) {
        TownMapPlace *place = func_ov072_021ba710(places, cursor);

        if (place != NULL) {
            TownMapPoint scroll;
            VecFx32 to;
            VecFx32 from;

            func_ov072_021ba69c(places, &scroll);
            to.x = FX32_CONST(scroll.x + func_ov072_021ba774(place, 4));
            to.y = FX32_CONST(scroll.y + func_ov072_021ba774(place, 5));
            to.z = 0;
            from.x = FX32_CONST(cursor->pos.x);
            from.y = FX32_CONST(cursor->pos.y);
            from.z = 0;
            VEC_Subtract(&to, &from, &pull);
            if (VEC_Mag(&pull) > FX32_ONE) {
                VEC_Normalize(&pull, &pull);
                vecfx_mul(&pull, FX32_ONE, &pull);
            }
            moved = TRUE;
        }
    }
    if (moved) {
        fx32 x;
        fx32 y;

        VEC_Normalize(&move, &move);
        x = pull.x + move.x * 3;
        y = pull.y + move.y * 3;
        if (0 < x && x < FX32_ONE) {
            x = FX32_ONE;
        } else if (-FX32_ONE < x && x < 0) {
            x = -FX32_ONE;
        }
        if (0 < y && y < FX32_ONE) {
            y = FX32_ONE;
        } else if (-FX32_ONE < y && y < 0) {
            y = -FX32_ONE;
        }
        cursor->pos.x += x >> FX32_SHIFT;
        cursor->pos.y += y >> FX32_SHIFT;
        cursor->pos.x = MATH_CLAMP(cursor->pos.x, TOWNMAP_CURSOR_LEFT, TOWNMAP_CURSOR_RIGHT);
        cursor->pos.y = MATH_CLAMP(cursor->pos.y, TOWNMAP_CURSOR_TOP, TOWNMAP_CURSOR_BOTTOM);
        func_0204b404(cursor->cursor, &cursor->pos, CLACT_SURFACE_MAIN);
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        func_020355d8(FALSE);
        cursor->decided = TRUE;
    }
    {
        u32 x;
        u32 y;

        if (func_02035d20(&x, &y)) {
            func_020355d8(TRUE);
            if (x <= TOWNMAP_CURSOR_RIGHT && y - TOWNMAP_CURSOR_TOP <= TOWNMAP_CURSOR_BOTTOM - TOWNMAP_CURSOR_TOP) {
                cursor->decided = TRUE;
                cursor->pos.x = x;
                cursor->pos.y = y;
            }
        }
    }
    if (!func_020355c4()) {
        func_0204b3dc(cursor->cursor, TRUE);
    } else {
        func_0204b3dc(cursor->cursor, FALSE);
    }
}


// pokebw2's TownMapCursor_GetPos
void func_ov072_021ba3bc(const TownMapCursor *cursor, TownMapPoint *pos) {
    pos->x = cursor->pos.x;
    pos->y = cursor->pos.y;
}

// pokebw2's TownMapCursor_SetPos
void func_ov072_021ba3cc(TownMapCursor *cursor, const TownMapPoint *pos) {
    cursor->pos.x = pos->x;
    cursor->pos.y = pos->y;
    func_0204b404(cursor->cursor, &cursor->pos, CLACT_SURFACE_MAIN);
}

// pokebw2's TownMapCursor_IsDecided
BOOL func_ov072_021ba3e8(const TownMapCursor *cursor) {
    return cursor->decided;
}

// pokebw2's TownMapCursor_GetHeldKeys
u32 func_ov072_021ba3ec(const TownMapCursor *cursor) {
    return GCTX_HIDGetHeldKeys();
}

void func_ov072_021ba3f4(TownMapCursor *cursor, TownMapPlace *place, TownMapPoint *scroll) {
    if (place == NULL) {
        func_0204b3dc(cursor->ring, FALSE);
        cursor->place = NULL;
    } else if (cursor->place != place) {
        ClActorPos pos;

        pos.x = scroll->x + func_ov072_021ba774(place, 4);
        pos.y = scroll->y + func_ov072_021ba774(place, 5);
        func_0204b404(cursor->ring, &pos, CLACT_SURFACE_MAIN);
        func_0204b3dc(cursor->ring, TRUE);
        func_0204b9a0(cursor->ring);
        cursor->place = place;
    }
}


void func_ov072_021ba450(TownMapCursor *cursor, const TownMapPoint *scroll) {
    if (cursor->place != NULL) {
        ClActorPos pos;

        pos.x = scroll->x + func_ov072_021ba774(cursor->place, 4);
        pos.y = scroll->y + func_ov072_021ba774(cursor->place, 5);
        func_0204b404(cursor->ring, &pos, CLACT_SURFACE_MAIN);
    }
}


// pokebw2's TownMapCursor_GetPlace
TownMapPlace *func_ov072_021ba488(const TownMapCursor *cursor) {
    return cursor->place;
}

// pokebw2's TownMapCursor_SetPull
void func_ov072_021ba48c(TownMapCursor *cursor, BOOL pull) {
    cursor->pull = pull;
}

void func_ov072_021ba490(TownMapPlaces *places, u16 zoneId, u16 escapeZoneId, void *data,
                               TownMapGraphic *graphic, GameData *gameData, HeapID heapId, BOOL all) {
    u32 i;
    ClActUnit *unit;
    TownMapPlace *place;

    unit = func_ov072_021bb778(graphic, 0);
    MI_CpuFill8(places, 0, sizeof(TownMapPlaces));
    places->count = 61;
    places->gameData = gameData;
    places->places = GFL_HeapAllocate(heapId, places->count * sizeof(TownMapPlace), FALSE, "townmap.c", 2245);
    MI_CpuFill8(places->places, 0, places->count * sizeof(TownMapPlace));
    {
        u32 chars;
        u32 cellAnims;
        u32 pltt;

        func_ov072_021bb790(graphic, &chars, &cellAnims, &pltt);
        for (i = 0; i < places->count; i++) {
            func_ov072_021ba86c(&places->places[i], data, i, unit, chars, cellAnims, pltt, gameData, heapId, all);
        }
    }
    place = func_ov072_021ba7f8(places, zoneId, escapeZoneId);
    {
        u32 chars;
        u32 cellAnims;
        u32 pltt;

        func_ov072_021bb790(graphic, &chars, &cellAnims, &pltt);
        func_ov072_021bae9c(&places->player, place, unit, chars, cellAnims, pltt, heapId);
    }
    places->limitTop = 0;
    places->limitLeft = 0;
    places->limitRight = 0;
    places->limitBottom = 0;
    func_ov072_021ba7d4(places);
}


// pokebw2's TownMapPlaces_Free
void func_ov072_021ba570(TownMapPlaces *places) {
    u32 i;

    func_ov072_021baf24(&places->player);
    for (i = 0; i < places->count; i++) {
        func_ov072_021baa14(&places->places[i]);
    }
    GFL_HeapFree(places->places);
    MI_CpuFill8(places, 0, sizeof(TownMapPlaces));
}

// pokebw2's TownMapPlaces_Main
void func_ov072_021ba5ac(TownMapPlaces *places) {
}

// Scales the places about the center of the screen, with the map scrolled to center
// pokebw2's TownMapPlaces_SetScale
void func_ov072_021ba5b0(TownMapPlaces *places, fx32 scale, const TownMapPoint *center) {
    u32 i;
    TownMapPoint scroll;

    if (scale == FX32_ONE) {
        places->limitTop = 0;
        places->limitLeft = 0;
        places->limitRight = 0;
        places->limitBottom = 0;
    } else {
        places->limitTop = TOWNMAP_PLACES_TOP;
        places->limitLeft = TOWNMAP_SCROLL_LEFT;
        places->limitRight = TOWNMAP_SCROLL_RIGHT;
        places->limitBottom = TOWNMAP_PLACES_BOTTOM;
    }
    scroll.x = -center->x;
    scroll.y = -center->y;
    func_ov072_021ba638(places, &scroll);
    for (i = 0; i < places->count; i++) {
        func_ov072_021bad10(&places->places[i], scale, &data_ov072_021bbb78);
        func_ov072_021bad84(&places->places[i], &places->scroll);
    }
    func_ov072_021baf3c(&places->player, &places->scroll);
}

// pokebw2's TownMapPlaces_SetScroll
void func_ov072_021ba638(TownMapPlaces *places, const TownMapPoint *scroll) {
    u32 i;

    places->scroll.x = MATH_CLAMP(scroll->x, places->limitLeft, places->limitRight);
    places->scroll.y = MATH_CLAMP(scroll->y, places->limitTop, places->limitBottom);
    for (i = 0; i < places->count; i++) {
        func_ov072_021bad84(&places->places[i], &places->scroll);
    }
    func_ov072_021baf3c(&places->player, &places->scroll);
}

// pokebw2's TownMapPlaces_GetScroll
void func_ov072_021ba69c(const TownMapPlaces *places, TownMapPoint *scroll) {
    *scroll = places->scroll;
}

// pokebw2's TownMapPlaces_AddScroll
void func_ov072_021ba6a8(TownMapPlaces *places, const TownMapPoint *move) {
    places->scroll.x += move->x;
    places->scroll.y += move->y;
    func_ov072_021ba638(places, &places->scroll);
}

// The place the cursor is on, or NULL
// pokebw2's TownMapPlaces_GetHit
TownMapPlace *func_ov072_021ba6c8(const TownMapPlaces *places, const TownMapCursor *cursor) {
    u32 i;
    TownMapPoint pos;

    func_ov072_021ba3bc(cursor, &pos);
    for (i = 0; i < places->count; i++) {
        if (func_ov072_021baa90(&places->places[i], &pos, &places->scroll)) {
            return &places->places[i];
        }
    }
    return NULL;
}

// The nearest of the places whose radius the cursor is in, or NULL
// pokebw2's TownMapPlaces_GetNearest
TownMapPlace *func_ov072_021ba710(const TownMapPlaces *places, const TownMapCursor *cursor) {
    u32 i;
    TownMapPoint pos;
    u32 distance;
    u32 nearest;
    u32 nearestDistance;

    func_ov072_021ba3bc(cursor, &pos);
    nearest = 0xffffffff;
    nearestDistance = 0xffffffff;
    for (i = 0; i < places->count; i++) {
        if (func_ov072_021bacb0(&places->places[i], &pos, &places->scroll, &distance)) {
            if (nearestDistance > distance) {
                nearest = i;
                nearestDistance = distance;
            }
        }
    }
    if (nearest == 0xffffffff) {
        return NULL;
    }
    return &places->places[nearest];
}

s32 func_ov072_021ba774(TownMapPlace *place, s32 param) {
    if (param >= TOWNMAP_PARAM_X && param <= 10) {
        return place->params[param - TOWNMAP_PARAM_X];
    }
    if (param == 12) {
        return place->canFly;
    }
    return func_ov072_021bbb3c(place->data, place->index, param);
}


// pokebw2's TownMapPlaces_Select
void func_ov072_021ba7a4(TownMapPlaces *places, const TownMapPlace *place) {
    u32 i;

    for (i = 0; i < places->count; i++) {
        if (&places->places[i] == place) {
            func_ov072_021badf4(&places->places[i]);
        } else {
            func_ov072_021bae44(&places->places[i]);
        }
    }
}

// pokebw2's TownMapPlaces_Deselect
void func_ov072_021ba7d4(TownMapPlaces *places) {
    u32 i;

    for (i = 0; i < places->count; i++) {
        func_ov072_021bae44(&places->places[i]);
    }
}

// The place the town map shows for a zone, or NULL
// pokebw2's TownMapPlaces_GetByZone
TownMapPlace *func_ov072_021ba7f8(const TownMapPlaces *places, u16 zoneId, u16 escapeZoneId) {
    u32 i;
    u16 zone = func_ov010_02166c7c(places->gameData, zoneId);

    for (i = 0; i < places->count; i++) {
        TownMapPlace *place = &places->places[i];

        if (zone == func_ov072_021ba774(place, TOWNMAP_PARAM_ZONE)) {
            return place;
        }
    }
    return NULL;
}

BOOL func_ov072_021ba834(const TownMapPlace *place) {
    u16 flag = func_ov072_021bbb3c(place->data, place->index, 0xf);

    if ((flag != TOWNMAP_NO_FLAG && place->arrived && !place->hidden) || flag == TOWNMAP_NO_FLAG) {
        return TRUE;
    }
    return FALSE;
}

void func_ov072_021ba86c(TownMapPlace *place, void *data, u32 index, ClActUnit *unit, u32 chars, u32 cellAnims,
    u32 pltt, GameData *gameData, HeapID heapId, BOOL all) {
    int i;
    u16 flag;

    MI_CpuFill8(place, 0, sizeof(TownMapPlace));
    place->scale = FX32_ONE;
    place->data = data;
    place->index = index;
    flag = func_ov072_021bbb3c(data, index, 0xf);
    if (flag != TOWNMAP_NO_FLAG) {
        place->arrived = func_ov010_02166cd0(gameData, flag);
    } else {
        place->arrived = FALSE;
    }
    if (all) {
        place->arrived = TRUE;
    }
    for (i = 0; i < 9; i++) {
        place->params[i] = func_ov072_021bbb3c(place->data, place->index, i + 2);
    }
    if (all) {
        place->canFly = TRUE;
    } else {
        place->canFly = place->arrived && func_ov072_021bbb3c(place->data, place->index, 12) != 0;
    }
    flag = func_ov072_021bbb3c(place->data, place->index, TOWNMAP_PARAM_FLAG);
    if (flag != TOWNMAP_NO_FLAG) {
        place->hidden = !func_ov010_02166cd0(gameData, flag);
    } else {
        place->hidden = FALSE;
    }
    if (all) {
        place->hidden = FALSE;
    }
    switch (func_ov072_021ba774(place, 11)) {
    case 0:
        place->animSelected = TOWNMAP_ANIM_CITY_SELECTED;
        place->anim = TOWNMAP_ANIM_CITY;
        break;
    case 2:
        place->animSelected = TOWNMAP_ANIM_SELECTED;
        place->anim = TOWNMAP_ANIM_NONE;
        break;
    case 1:
        place->animSelected = TOWNMAP_ANIM_NONE;
        place->anim = TOWNMAP_ANIM_NONE;
        break;
    case 3:
        place->animSelected = TOWNMAP_ANIM_SELECTED;
        place->anim = 13;
        break;
    case 4:
        place->animSelected = 15;
        place->anim = 14;
        break;
    case 5:
        place->animSelected = 18;
        if (place->arrived) {
            place->anim = 17;
        } else {
            place->anim = 16;
        }
        break;
    }
    if (func_ov072_021ba774(place, 11) != 1) {
        ClActorSetup setup;

        MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
        setup.x = func_ov072_021ba774(place, 2);
        setup.y = func_ov072_021ba774(place, 3);
        place->actor = func_0204b294(unit, chars, pltt, cellAnims, &setup, CLACT_SURFACE_MAIN, heapId);
        func_0204b570(place->actor, 2);
        func_0204b84c(place->actor, 2);
        func_0204b800(place->actor, 4);
    }
    func_ov072_021bae44(place);
}


// pokebw2's TownMapPlace_Free
void func_ov072_021baa14(TownMapPlace *place) {
    if (place->actor != NULL) {
        func_0204b3b4(place->actor);
    }
    MI_CpuFill8(place, 0, sizeof(TownMapPlace));
}

void func_ov072_021baa30(TownMapPlace *place, BOOL visible) {
    if (place->actor == NULL || place->animNow == 0xFF) {
        return;
    }
    if (place->hidden) {
        func_0204b3dc(place->actor, FALSE);
        return;
    }
    if (func_ov072_021ba774(place, 0xb) == 4 || func_ov072_021ba774(place, 0xb) == 5) {
        func_ov072_021badbc(place, place->anim);
        func_0204b3dc(place->actor, TRUE);
    } else {
        func_0204b3dc(place->actor, visible);
    }
}

static BOOL func_ov072_021baa90(const TownMapPlace *place, const TownMapPoint *pos, const TownMapPoint *scroll) {
    CalcSphere sphere;
    CalcCapsule capsule;
    VecFx32 start;
    VecFx32 end;

    start.x = FX32_CONST(pos->x);
    start.y = FX32_CONST(pos->y);
    start.z = 0;
    func_02051f1c(&sphere, &start, 0);
    start.x = FX32_CONST(scroll->x + func_ov072_021ba774(place, 6));
    start.y = FX32_CONST(scroll->y + func_ov072_021ba774(place, 7));
    start.z = 0;
    end.x = FX32_CONST(scroll->x + func_ov072_021ba774(place, 8));
    end.y = FX32_CONST(scroll->y + func_ov072_021ba774(place, 9));
    end.z = 0;
    func_0205207c(&capsule, &start, &end, FX32_CONST(func_ov072_021ba774(place, 10)));
    if (place->hidden) {
        return FALSE;
    }
    return func_02051f34(&capsule, &sphere, NULL);
}


BOOL func_ov072_021bacb0(TownMapPlace *place, const TownMapPoint *pos, const TownMapPoint *scroll, u32 *distance) {
    s32 x = scroll->x + func_ov072_021ba774(place, 4);
    s32 y = scroll->y + func_ov072_021ba774(place, 5);
    u32 squareX = (x - pos->x) * (x - pos->x);
    u32 squareY = (y - pos->y) * (y - pos->y);

    if (place->hidden) {
        return FALSE;
    }
    if (squareX + squareY < (12 * 12 * MATH_ABS(place->scale)) >> FX32_SHIFT) {
        if (distance != NULL) {
            *distance = squareX + squareY;
        }
        return TRUE;
    }
    return FALSE;
}

void func_ov072_021bad10(TownMapPlace *place, fx32 scale, const TownMapPoint *center) {
    int i;

    for (i = 0; i < 8; i++) {
        s32 origin;
        s32 value;

        if (i % 2 == 0) {
            origin = center->x;
        } else {
            origin = center->y;
        }
        value = func_ov072_021bbb3c(place->data, place->index, i + 2);
        value -= origin;
        value *= scale;
        place->params[i] = (value >> FX32_SHIFT) + origin;
    }
    place->params[8] = (func_ov072_021bbb3c(place->data, place->index, 10) * scale) >> FX32_SHIFT;
    place->scale = scale;
    if (place->actor != NULL) {
        ClActorScale actorScale;

        actorScale.x = scale;
        actorScale.y = scale;
        func_0204b5ac(place->actor, &actorScale);
    }
}


void func_ov072_021bad84(TownMapPlace *place, const TownMapPoint *scroll) {
    ClActorPos pos;

    pos.x = scroll->x + func_ov072_021ba774(place, TOWNMAP_PARAM_X);
    pos.y = scroll->y + func_ov072_021ba774(place, TOWNMAP_PARAM_Y);
    if (place->actor != NULL) {
        func_0204b404(place->actor, &pos, CLACT_SURFACE_MAIN);
    }
}


// pokebw2's TownMapPlace_SetAnim
void func_ov072_021badbc(TownMapPlace *place, u8 anim) {
    place->animNow = anim;
    if (place->animNow != TOWNMAP_ANIM_NONE) {
        if (place->actor != NULL) {
            func_0204b3dc(place->actor, TRUE);
            func_0204b878(place->actor, place->animNow);
        }
    } else {
        if (place->actor != NULL) {
            func_0204b3dc(place->actor, FALSE);
        }
    }
}

void func_ov072_021badf4(TownMapPlace *place) {
    BOOL shown = FALSE;

    if (func_ov072_021ba774(place, 11) == TOWNMAP_PLACE_TYPE_TOWN || func_ov072_021ba774(place, 11) == 5) {
        shown = TRUE;
        if (place->hidden) {
            shown = FALSE;
        }
    }
    if (place->arrived || shown) {
        func_ov072_021baa30(place, TRUE);
        func_ov072_021badbc(place, place->animSelected);
    } else {
        func_ov072_021baa30(place, FALSE);
    }
}

void func_ov072_021bae44(TownMapPlace *place) {
    BOOL shown = FALSE;

    if (func_ov072_021ba774(place, 11) == TOWNMAP_PLACE_TYPE_TOWN || func_ov072_021ba774(place, 11) == 5) {
        shown = TRUE;
        if (place->hidden) {
            shown = FALSE;
        }
    }
    if (place->arrived || shown) {
        if (place->actor != NULL) {
            func_ov072_021baa30(place, TRUE);
            func_ov072_021badbc(place, place->anim);
        }
    } else {
        func_ov072_021baa30(place, FALSE);
    }
}

void func_ov072_021bae9c(TownMapPlayer *player, TownMapPlace *place, ClActUnit *unit, u32 chars, u32 cellAnims,
    u32 pltt, HeapID heapId) {
    ClActorSetup setup;

    MI_CpuFill8(player, 0, sizeof(TownMapPlayer));
    player->place = place;
    MI_CpuFill8(&setup, 0, 8);
    if (place != NULL) {
        setup.x = func_ov072_021ba774(place, TOWNMAP_PARAM_X);
        setup.y = func_ov072_021ba774(place, TOWNMAP_PARAM_Y);
    }
    player->actor = func_0204b294(unit, chars, pltt, cellAnims, &setup, CLACT_SURFACE_MAIN, heapId);
    func_0204b84c(player->actor, 2);
    func_0204b800(player->actor, 2);
    func_0204b878(player->actor, TOWNMAP_ANIM_PLAYER);
    func_0204b92c(player->actor, TRUE);
    if (place == NULL) {
        func_0204b3dc(player->actor, FALSE);
    }
}


// pokebw2's TownMapPlayer_Free
void func_ov072_021baf24(TownMapPlayer *player) {
    func_0204b3b4(player->actor);
    MI_CpuFill8(player, 0, sizeof(TownMapPlayer));
}

void func_ov072_021baf3c(TownMapPlayer *player, const TownMapPoint *scroll) {
    ClActorPos pos;

    if (player->place != NULL) {
        pos.x = scroll->x + func_ov072_021ba774(player->place, TOWNMAP_PARAM_X);
        pos.y = scroll->y + func_ov072_021ba774(player->place, TOWNMAP_PARAM_Y);
    }
    func_0204b404(player->actor, &pos, CLACT_SURFACE_MAIN);
}


// pokebw2's TownMapMap_Init
void func_ov072_021baf74(TownMapMap *map, u8 bgBack, u8 bgFront, HeapID heapId) {
    MI_CpuFill8(map, 0, sizeof(TownMapMap));
    map->bgBack = bgBack;
    map->bgFront = bgFront;
    map->scale = FX32_ONE;
    map->limitTop = 0;
    map->limitLeft = 0;
    map->limitRight = 0;
    map->limitBottom = 0;
}

// pokebw2's TownMapMap_Free
void func_ov072_021baf9c(TownMapMap *map) {
    MI_CpuFill8(map, 0, sizeof(TownMapMap));
}

// Moves the zoom on, and the places with it
// pokebw2's TownMapMap_Main
inline void TownMapMap_SetLimits(TownMapMap *map, s16 top, s16 left, s16 right, s16 bottom) {
    map->limitTop = top;
    map->limitLeft = left;
    map->limitRight = right;
    map->limitBottom = bottom;
}
static void func_ov072_021bafa8(TownMapMap *map, TownMapPlaces *places) {
    map->finished = FALSE;
    if (map->moving) {
        fx32 step = (map->scaleEnd - map->scaleStart) / TOWNMAP_ZOOM_FRAMES;
        fx32 moveX;
        fx32 moveY;

        map->scale = map->scaleStart + step * map->frame;
        moveY = (map->end.y - map->start.y) << FX32_SHIFT;
        moveX = (map->end.x - map->start.x) << FX32_SHIFT;
        map->pos.x = map->start.x + ((moveX * map->frame / TOWNMAP_ZOOM_FRAMES) >> FX32_SHIFT);
        map->pos.y = map->start.y + ((moveY * map->frame / TOWNMAP_ZOOM_FRAMES) >> FX32_SHIFT);
        if (map->scaleEnd == FX32_ONE) {
            map->limitTop = TOWNMAP_SCROLL_TOP - TOWNMAP_SCROLL_TOP / TOWNMAP_ZOOM_FRAMES * map->frame;
            map->limitBottom = TOWNMAP_SCROLL_BOTTOM - TOWNMAP_SCROLL_BOTTOM / TOWNMAP_ZOOM_FRAMES * map->frame;
            map->limitLeft = TOWNMAP_SCROLL_LEFT - TOWNMAP_SCROLL_LEFT / TOWNMAP_ZOOM_FRAMES * map->frame;
            map->limitRight = TOWNMAP_SCROLL_RIGHT - TOWNMAP_SCROLL_RIGHT / TOWNMAP_ZOOM_FRAMES * map->frame;
        } else {
            map->limitTop = TOWNMAP_SCROLL_TOP / TOWNMAP_ZOOM_FRAMES * map->frame;
            map->limitBottom = TOWNMAP_SCROLL_BOTTOM / TOWNMAP_ZOOM_FRAMES * map->frame;
            map->limitLeft = TOWNMAP_SCROLL_LEFT / TOWNMAP_ZOOM_FRAMES * map->frame;
            map->limitRight = TOWNMAP_SCROLL_RIGHT / TOWNMAP_ZOOM_FRAMES * map->frame;
        }
        if (map->frame++ >= TOWNMAP_ZOOM_FRAMES) {
            map->frame = 0;
            map->moving = FALSE;
            map->scale = map->scaleEnd;
            map->pos = map->end;
            if (map->scale == FX32_ONE) {
                TownMapMap_SetLimits(map, 0, 0, 0, 0);
            } else {
                TownMapMap_SetLimits(map, TOWNMAP_SCROLL_TOP, TOWNMAP_SCROLL_LEFT, TOWNMAP_SCROLL_RIGHT,
                                     TOWNMAP_SCROLL_BOTTOM);
            }
            map->finished = TRUE;
        }
        func_ov072_021ba5b0(places, map->scale, &map->pos);
        func_ov072_021bb0e4(map, &map->pos);
    }
}

// Starts a zoom to a scale, between two points of the screen that the map is centered on
// pokebw2's TownMapMap_StartZoom
void func_ov072_021bb0b4(TownMapMap *map, fx32 scale, const TownMapPoint *start, const TownMapPoint *end) {
    map->moving = TRUE;
    map->scaleEnd = scale;
    map->scaleStart = map->scale;
    map->start.x = start->x - TOWNMAP_CENTER_X;
    map->start.y = start->y - TOWNMAP_CENTER_Y;
    map->end.x = end->x - TOWNMAP_CENTER_X;
    map->end.y = end->y - TOWNMAP_CENTER_Y;
}

// pokebw2's TownMapMap_IsFinished
BOOL func_ov072_021bb0dc(const TownMapMap *map) {
    return map->finished;
}

// pokebw2's TownMapMap_IsMoving
BOOL func_ov072_021bb0e0(const TownMapMap *map) {
    return map->moving;
}

// pokebw2's TownMapMap_SetPos
void func_ov072_021bb0e4(TownMapMap *map, const TownMapPoint *pos) {
    map->pos.x = MATH_CLAMP(pos->x, map->limitLeft, map->limitRight);
    map->pos.y = MATH_CLAMP(pos->y, map->limitTop, map->limitBottom);
    GFL_BGSysMoveBGReq(map->bgBack, BG_MOVE_SET_X, map->pos.x);
    GFL_BGSysMoveBGReq(map->bgBack, BG_MOVE_SET_Y, map->pos.y);
    GFL_BGSysAdjustBGOriginReq(map->bgBack, BG_CENTER_SET_X, TOWNMAP_CENTER_X);
    GFL_BGSysAdjustBGOriginReq(map->bgBack, BG_CENTER_SET_Y, TOWNMAP_CENTER_Y);
    GFL_BGSysScaleBGReq(map->bgBack, BG_SCALE_SET_X, map->scale);
    GFL_BGSysScaleBGReq(map->bgBack, BG_SCALE_SET_Y, map->scale);
    GFL_BGSysMoveBGReq(map->bgFront, BG_MOVE_SET_X, map->pos.x);
    GFL_BGSysMoveBGReq(map->bgFront, BG_MOVE_SET_Y, map->pos.y);
    GFL_BGSysAdjustBGOriginReq(map->bgFront, BG_CENTER_SET_X, TOWNMAP_CENTER_X);
    GFL_BGSysAdjustBGOriginReq(map->bgFront, BG_CENTER_SET_Y, TOWNMAP_CENTER_Y);
    GFL_BGSysScaleBGReq(map->bgFront, BG_SCALE_SET_X, map->scale);
    GFL_BGSysScaleBGReq(map->bgFront, BG_SCALE_SET_Y, map->scale);
}

// pokebw2's TownMapMap_GetPos
void func_ov072_021bb190(const TownMapMap *map, TownMapPoint *pos) {
    *pos = map->pos;
}

// pokebw2's TownMapMap_AddPos
void func_ov072_021bb19c(TownMapMap *map, const TownMapPoint *move) {
    map->pos.x += move->x;
    map->pos.y += move->y;
    func_ov072_021bb0e4(map, &map->pos);
}

void func_ov072_021bb1bc(TownMapInfo *info, u8 bg, Font *font, MsgData *msgData, MsgData *placeNames, HeapID heapId) {
    MI_CpuFill8(info, 0, sizeof(TownMapInfo));
    info->msgWin.font = font;
    info->msgWin.msgData = msgData;
    info->msgData = placeNames;
    info->msgWin.window = BmpWin_CreateDynamic(bg, 0, 0, 32, 24, 14, 0);
    info->msgWin.strbuf = GFL_StrBufCreate(255, heapId);
}

// pokebw2's TownMapInfo_Free
void func_ov072_021bb204(TownMapMsgWin *msgWin) {
    GFL_StrBufFree(msgWin->strbuf);
    BmpWin_Free(msgWin->window);
    MI_CpuFill8(msgWin, 0, 0x14);
}

void func_ov072_021bb220(TownMapInfo *info, TownMapPlace *place) {
    GFLBitmap *bitmap = BmpWin_GetBitmap(info->msgWin.window);
    s16 center;
    s16 half;
    u16 messageId;

    GFL_BitmapFill(bitmap, 0);
    if (place != NULL) {
        u16 zoneId = func_ov072_021ba774(place, 0);

        GFL_MsgDataLoadStrbuf(info->msgWin.msgData, ZoneData_GetPlaceNameID(zoneId), info->msgWin.strbuf);
        center = func_02045738(info->msgWin.window) * 4;
        half = GFL_FontGetBlockWidth(info->msgWin.strbuf, info->msgWin.font, 0) / 2;
        center -= half;
        GFL_TextRendererDrawToBitmap(bitmap, center, 48, info->msgWin.strbuf, info->msgWin.font);
        messageId = func_ov072_021ba774(place, 0x11);
        if (messageId != 0xffff) {
            GFL_MsgDataLoadStrbuf(info->msgData, messageId, info->msgWin.strbuf);
            GFL_TextRendererDrawToBitmap(bitmap, 28, 80, info->msgWin.strbuf, info->msgWin.font);
        }
        if (func_ov072_021ba834(place)) {
            messageId = func_ov072_021ba774(place, 0x12);
            if (messageId != 0xffff) {
                GFL_MsgDataLoadStrbuf(info->msgData, messageId, info->msgWin.strbuf);
                GFL_TextRendererDrawToBitmap(bitmap, 16, 128, info->msgWin.strbuf, info->msgWin.font);
            }
            messageId = func_ov072_021ba774(place, 0x13);
            if (messageId != 0xffff) {
                GFL_MsgDataLoadStrbuf(info->msgData, messageId, info->msgWin.strbuf);
                GFL_TextRendererDrawToBitmap(bitmap, 16, 144, info->msgWin.strbuf, info->msgWin.font);
            }
            messageId = func_ov072_021ba774(place, 0x14);
            if (messageId != 0xffff) {
                GFL_MsgDataLoadStrbuf(info->msgData, messageId, info->msgWin.strbuf);
                GFL_TextRendererDrawToBitmap(bitmap, 16, 160, info->msgWin.strbuf, info->msgWin.font);
            }
            messageId = func_ov072_021ba774(place, 0x15);
            if (messageId != 0xffff) {
                GFL_MsgDataLoadStrbuf(info->msgData, messageId, info->msgWin.strbuf);
                GFL_TextRendererDrawToBitmap(bitmap, 136, 128, info->msgWin.strbuf, info->msgWin.font);
            }
            messageId = func_ov072_021ba774(place, 0x16);
            if (messageId != 0xffff) {
                GFL_MsgDataLoadStrbuf(info->msgData, messageId, info->msgWin.strbuf);
                GFL_TextRendererDrawToBitmap(bitmap, 136, 144, info->msgWin.strbuf, info->msgWin.font);
            }
            messageId = func_ov072_021ba774(place, 0x17);
            if (messageId != 0xffff) {
                GFL_MsgDataLoadStrbuf(info->msgData, messageId, info->msgWin.strbuf);
                GFL_TextRendererDrawToBitmap(bitmap, 136, 160, info->msgWin.strbuf, info->msgWin.font);
            }
        }
    }
    {
        BmpWin *window = info->msgWin.window;

        BmpWin_FlushChar(window);
        BmpWin_FlushMap(window);
        GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
    }
}


// pokebw2's TownMapInfo_Clear
void func_ov072_021bb3d0(TownMapMsgWin *msgWin) {
    BmpWin *window = msgWin->window;

    func_020456f8(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
}

void func_ov072_021bb3e8(TownMapMsgWin *msgWin, u8 bg, Font *font, MsgData *msgData, u8 x, u8 y, u8 width,
                               u8 height, HeapID heapId) {
    MI_CpuFill8(msgWin, 0, sizeof(TownMapMsgWin));
    msgWin->font = font;
    msgWin->msgData = msgData;
    msgWin->window = BmpWin_CreateDynamic(bg, x, y, width, height, 14, 0);
    msgWin->strbuf = GFL_StrBufCreate(255, heapId);
}

// pokebw2's TownMapMsgWin_Free
void func_ov072_021bb42c(TownMapMsgWin *msgWin) {
    GFL_StrBufFree(msgWin->strbuf);
    BmpWin_Free(msgWin->window);
    MI_CpuFill8(msgWin, 0, sizeof(TownMapMsgWin));
}

// pokebw2's TownMapMsgWin_Print
void func_ov072_021bb448(TownMapMsgWin *msgWin, u32 messageId, u16 x, u16 y) {
    GFL_BitmapFill(BmpWin_GetBitmap(msgWin->window), 0);
    GFL_MsgDataLoadStrbuf(msgWin->msgData, messageId, msgWin->strbuf);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(msgWin->window), x, y, msgWin->strbuf, msgWin->font);
    BmpWin_Transfer(msgWin->window);
}

void func_ov072_021bb49c(TownMapMsgWin *msgWin) {
    BmpWin *window = msgWin->window;

    func_020456f8(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
}

void func_ov072_021bb4b4(TownMapPlaceName *placeName, u8 bg, Font *font, MsgData *placeNames, ClActor *plate,
    HeapID heapId) {
    ClActorPos pos;

    MI_CpuFill8(placeName, 0, sizeof(TownMapPlaceName));
    placeName->plate = plate;
    func_ov072_021bb3e8(&placeName->msgWin, bg, font, placeNames, 0, 0, 24, 3, heapId);
    pos.x = TOWNMAP_CENTER_X;
    pos.y = TOWNMAP_CENTER_Y;
    func_0204b404(placeName->plate, &pos, CLACT_SURFACE_MAIN);
    func_0204b84c(placeName->plate, 1);
    func_0204b800(placeName->plate, 1);
    func_0204b3dc(placeName->plate, TRUE);
    func_0204b878(placeName->plate, TOWNMAP_ANIM_NAME_PLATE);
    func_0204b92c(placeName->plate, TRUE);
    func_0204b974(placeName->plate);
}


// Frees the place name's window and clears the place name (0x20 bytes on ARM, its size)
void func_ov072_021bb534(TownMapPlaceName *placeName) {
    func_ov072_021bb42c(&placeName->msgWin);
    MI_CpuFill8(placeName, 0, sizeof(TownMapPlaceName));
}

void func_ov072_021bb548(TownMapPlaceName *placeName) {
    if (placeName->requested) {
        placeName->requested = FALSE;
        func_0204b878(placeName->plate, 0);
        func_0204b95c(placeName->plate);
        func_0204b3dc(placeName->plate, TRUE);
        func_ov072_021bb49c(&placeName->msgWin);
        placeName->opening = TRUE;
    }
    if (placeName->opening && !func_0204b98c(placeName->plate)) {
        u16 zoneId = func_ov072_021ba774(placeName->place, TOWNMAP_PARAM_ZONE);

        func_ov072_021bb448(&placeName->msgWin, ZoneData_GetPlaceNameID(zoneId), 16, 2);
        func_0204b974(placeName->plate);
        placeName->opening = FALSE;
    }
}


void func_ov072_021bb5ac(TownMapPlaceName *placeName, TownMapPlace *place) {
    placeName->requested = TRUE;
    placeName->place = place;
}

void func_ov072_021bb5b4(TownMapPlaceName *placeName, BOOL visible) {
    func_0204b3dc(placeName->plate, visible);
    if (!visible) {
        placeName->requested = FALSE;
        placeName->opening = FALSE;
        func_ov072_021bb49c(&placeName->msgWin);
    }
}


void func_ov072_021bb5d4(TownMapTouch *touch, HeapID heapId) {
    MI_CpuFill8(touch, 0, sizeof(TownMapTouch));
}

void func_ov072_021bb5e0(TownMapTouch *touch) {
    MI_CpuFill8(touch, 0, sizeof(TownMapTouch));
}


// Follows a touch on the map: how far it moved since it started, or since the last drag was taken
// pokebw2's TownMapTouch_Main
void func_ov072_021bb5ec(TownMapTouch *touch) {
    u32 x;
    u32 y;

    touch->dragging = FALSE;
    if (func_02035d20(&x, &y)) {
        if (y <= TOWNMAP_CURSOR_BOTTOM) {
            touch->start.x = x << FX32_SHIFT;
            touch->start.y = y << FX32_SHIFT;
            touch->start.z = 0;
            touch->now.x = x << FX32_SHIFT;
            touch->now.y = y << FX32_SHIFT;
            touch->now.z = 0;
        }
    } else if (func_02035cb8(&x, &y)) {
        if (y <= TOWNMAP_CURSOR_BOTTOM) {
            if (touch->restart) {
                touch->start = touch->now;
                touch->restart = FALSE;
            }
            touch->now.x = x << FX32_SHIFT;
            touch->now.y = y << FX32_SHIFT;
            touch->now.z = 0;
            VEC_Subtract(&touch->now, &touch->start, &touch->delta);
            if (VEC_Mag(&touch->delta) >= FX32_ONE) {
                touch->dragging = TRUE;
            }
        } else {
            touch->start.x = x << FX32_SHIFT;
            touch->start.y = y << FX32_SHIFT;
            touch->start.z = 0;
            touch->now.x = x << FX32_SHIFT;
            touch->now.y = y << FX32_SHIFT;
            touch->now.z = 0;
        }
    }
}

// pokebw2's TownMapTouch_GetDrag
BOOL func_ov072_021bb6ac(const TownMapTouch *touch, VecFx32 *delta, u32 *unk18) {
    if (touch->dragging) {
        if (delta != NULL) {
            *delta = touch->delta;
        }
        if (unk18 != NULL) {
            *unk18 = touch->unk18;
        }
        return TRUE;
    }
    return FALSE;
}

// pokebw2's TownMapTouch_Restart
void func_ov072_021bb6dc(TownMapTouch *touch) {
    touch->restart = TRUE;
}

// pokebw2's TownMap_VBlank
void func_ov072_021bb6e4(TCB *tcb, void *data) {
}
