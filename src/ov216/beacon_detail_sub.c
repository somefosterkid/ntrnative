// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov308/beacon_detail_sub.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/beacon_detail_local.h"
#include "app/townmap/townmap_data.h"
#include "app/ui/touchbar.h"
#include "constants/sound.h"
#include "constants/version.h"
#include "constants/zones.h"
#include "field/zone.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "pml/poke_graphic.h"
#include "system/game_beacon.h"
#include "system/game_system.h"
#include "system/palanm.h"
#include "system/pms_data.h"
#include "system/pms_draw.h"
#include "system/printsys.h"
#include "system/union_view.h"
#include "system/wordset.h"
#include "text/system/startmenu.h"
#include "app/beacon_detail.h"

// The drawing, the input and the tasks of the screen of a beacon's details. The ROM has no name for this file;
// "beacon_detail_sub.c" is a guess from its graphic file's, "beacon_detail_gra.c", which the linker placed between
// this file and the screen's own

// The zones are below this
#define ZONE_COUNT 615

#define SCREEN_HEIGHT 192

// How fast and for how long a panel slides when the page turns
#define PANEL_SLIDE_SPEED 8
#define PANEL_SLIDE_FRAMES (SCREEN_HEIGHT / PANEL_SLIDE_SPEED)

// How far and how fast the map slides up for the pop-up, and how long the pop-up stays
#define POPUP_SLIDE_Y 64
#define POPUP_SLIDE_SPEED 8
#define POPUP_SLIDE_FRAMES (POPUP_SLIDE_Y / POPUP_SLIDE_SPEED)
#define POPUP_FRAMES 150

// Where a panel that slides in starts from
enum {
    PANEL_SLIDE_FROM_ABOVE,
    PANEL_SLIDE_OUT,
    PANEL_SLIDE_FROM_BELOW,
};

// The directions of turning the page
enum {
    PAGE_NEXT,
    PAGE_PREV,
};

// Where LoadPalette loads colors
enum {
    PALETTE_BG_MAIN,
    PALETTE_BG_SUB,
    PALETTE_OBJ_MAIN,
    PALETTE_OBJ_SUB,
};

// The two looks of a panel's rows
typedef struct {
    u16 color;
    u8 background;
} BeaconDetailRowStyle;

// How the screen shows a kind of recent event: its message, and the animation of its icon on the map plus 1, or 0
// for no icon
typedef struct {
    u8 kind;
    u8 message;
    u8 icon;
} BeaconDetailRecent;

typedef struct {
    u8 out;
    u8 frames;
    u8 frame;
    s8 y;
    s8 speed;
    int *taskCount;
} BeaconDetailMapSlideTask;

typedef struct {
    u8 seq;
    u8 timer;
    int taskCount;
    BeaconDetailWork *work;
    int *parentTaskCount;
} BeaconDetailPopupTask;

typedef struct {
    u8 page;
    u8 frames;
    u16 unk2;
    s16 y;
    s16 speed;
    u32 unk8;
    BeaconDetailPanel *panel;
    BeaconDetailWork *work;
    int *taskCount;
} BeaconDetailPanelSlideTask;

typedef struct {
    u8 seq;
    u8 unk1;
    u8 unk2;
    u8 index;
    u8 direction;
    u8 decided;
    int taskCount;
    u32 unkC;
    BeaconDetailWork *work;
    int *parentTaskCount;
} BeaconDetailPageTask;

typedef struct {
    BeaconDetailWindow *window;
    BeaconDetailWork *work;
    int *taskCount;
} BeaconDetailFlushTask;

static BOOL func_ov216_021bccc4(BeaconDetailWork *work);
static void func_ov216_021bcd00(u32 se);
static void func_ov216_021bcd08(const void *src, u32 dest, u16 offset, u32 count);
static void func_ov216_021bcd60(BeaconDetailWork *work);
static u32 func_ov216_021bcdc0(const GameBeacon *beacon);
const BeaconDetailRecent *func_ov216_021bcddc(u32 kind);
void func_ov216_021bce04(const GameBeacon *beacon, WordSet *wordSet);
void loadTrainerNamesToStrbuf(WordSet *wordSet, u32 index, u32 trainerId);
static void func_ov216_021bce7c(ClActor *actor, u8 anim);
static void func_ov216_021bce8c(ClActor *actor, s16 x, s16 y, u16 surface);
static void func_ov216_021bcea4(BeaconDetailWork *work, u8 messageId);
void func_ov216_021bced0(BeaconDetailWork *work, u8 view);
static void func_ov216_021bcf08(BeaconDetailWork *work);
static void func_ov216_021bd07c(BeaconDetailWork *work, BeaconDetailWindow *window, StrBuf *strbuf, u8 clearX, u8 x,
                                    u8 width, u8 height, u8 style);
static void func_ov216_021bd0e4(BeaconDetailWork *work, u8 page, BOOL visible);
static void func_ov216_021bd13c(BeaconDetailPanel *panel, s16 y);
static void func_ov216_021bd174(BeaconDetailWork *work, GameBeacon *beacon, u16 time, u8 page);
void func_ov216_021bd448(BeaconDetailWork *work);
static void func_ov216_021bd668(BeaconDetailWork *work, u8 out, int *taskCount);
static void func_ov216_021bd6bc(TCBEx *task, void *data);
static void func_ov216_021bd6f0(BeaconDetailWork *work);
static void func_ov216_021bd6fc(BeaconDetailWork *work, int *taskCount);
static BOOL func_ov216_021bd73c(BeaconDetailPopupTask *popup);
static void func_ov216_021bd768(TCBEx *task, void *data);
static void func_ov216_021bd7d4(BeaconDetailWork *work, u8 page, int from, u8 direction, int *taskCount);
static void func_ov216_021bd854(TCBEx *task, void *data);
static void func_ov216_021bd89c(BeaconDetailWork *work, u8 direction);
static void func_ov216_021bd8a8(BeaconDetailWork *work, u8 direction, int *taskCount);
static void func_ov216_021bd948(TCBEx *task, void *data);
static void func_ov216_021bda54(BeaconDetailWork *work, BeaconDetailWindow *window, s16 x, s16 y, StrBuf *strbuf, u16 color);
void func_ov216_021bdaa0(BeaconDetailWork *work, BeaconDetailWindow *window, int *taskCount);
void func_ov216_021bdad8(TCBEx *task, void *data);
static BOOL BeaconDetailSub_IsOffMap(const GameBeacon *beacon);

// Where the panel that slides in comes from, by the direction of turning the page
static const u8 data_ov216_021bdd88[] = { PANEL_SLIDE_FROM_BELOW, PANEL_SLIDE_FROM_ABOVE };

// The zones of Black and White that Black 2 and White 2's map has no place for
static const u16 sBeaconDetailOffMapZones[] = { 191, 264 };

static const BeaconDetailRowStyle data_ov216_021bdd8a[] = {
    { PRINT_COLOR(15, 14, 3), 3 },
    { PRINT_COLOR(15, 14, 4), 4 },
};

// The palette of the Union Room's trainers that each view's is
static const u8 data_ov216_021bdd92[BEACON_DETAIL_UNION_VIEW_COUNT] = {
    2, 3, 6, 5, 4, 5, 2, 0, 1, 3, 6, 5, 4, 7, 7, 0,
};

static const BeaconDetailRecent data_ov216_021bdda2[] = {
    { 0, Startmenu_Text_015, 1 },
    { 1, Startmenu_Text_016, 1 },
    { 2, Startmenu_Text_017, 2 },
    { 3, Startmenu_Text_018, 2 },
    { 4, Startmenu_Text_019, 2 },
    { GAME_BEACON_RECENT_NONE, Startmenu_Text_020, 0 },
};

// pokebw2's BeaconDetailSub_Start
void func_ov216_021bcba4(BeaconDetailWork *work) {
    func_ov216_021bcf08(work);
    func_ov010_02165ac4(work->infoTable, work->beacon, &work->time, work->logIndices[work->index]);
    func_ov216_021bd174(work, work->beacon, work->time, work->page);
    func_ov216_021bd0e4(work, work->page, TRUE);
    func_ov216_021bd0e4(work, work->page ^ 1, FALSE);
    func_ov216_021bd448(work);
    func_ov216_021bcd60(work);
    func_0203d34c(FALSE, work->tailHeapId);
}

// pokebw2's BeaconDetailSub_Input
u32 func_ov216_021bcc10(BeaconDetailWork *work) {
    u32 touched = func_ov067_021b6470(work->touchBar);
    u32 keys = GCTX_HIDGetPressedKeys();

    if (func_02012204(work->param->gsys)) {
        work->param->unk09 = TRUE;
        return BEACON_DETAIL_SEQ_FADE_OUT;
    }
    if (touched == BEACON_DETAIL_ICON_RETURN) {
        work->param->unk09 = FALSE;
        return BEACON_DETAIL_SEQ_FADE_OUT;
    }
    if (touched == BEACON_DETAIL_ICON_PREV && work->index != 0) {
        func_ov216_021bd89c(work, PAGE_PREV);
        return BEACON_DETAIL_SEQ_WAIT;
    }
    if (touched == BEACON_DETAIL_ICON_NEXT && work->index < work->count - 1) {
        func_ov216_021bd89c(work, PAGE_NEXT);
        return BEACON_DETAIL_SEQ_WAIT;
    }
    if (!work->hasPlace) {
        return BEACON_DETAIL_SEQ_INPUT;
    }
    if (func_ov216_021bccc4(work) || (keys & PAD_BUTTON_A)) {
        func_ov216_021bcd00(SEQ_SE_DECIDE1);
        func_ov216_021bd6f0(work);
        return BEACON_DETAIL_SEQ_WAIT;
    }
    return BEACON_DETAIL_SEQ_INPUT;
}

// pokebw2's BeaconDetailSub_EndWait
void func_ov216_021bccbc(BeaconDetailWork *work) {
    func_ov216_021bcd60(work);
}

// Whether the map's trainer was touched this frame
// pokebw2's BeaconDetailSub_IsMarkerTouched
static BOOL func_ov216_021bccc4(BeaconDetailWork *work) {
    u32 x;
    u32 y;

    if (func_02035d20(&x, &y)) {
        if (x - (work->markerX - 12) <= 24 && y - (work->markerY - 16) <= 32) {
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's BeaconDetailSub_PlaySE
static void func_ov216_021bcd00(u32 se) {
    GFL_SndSEPlay(se);
}

// pokebw2's BeaconDetailSub_LoadPalette
static void func_ov216_021bcd08(const void *src, u32 dest, u16 offset, u32 count) {
    u32 destOffset = offset * 2;

    DC_FlushRange(src, count * 2);
    switch (dest) {
    case PALETTE_BG_MAIN:
        GX_LoadBGPltt(src, destOffset, count * 2);
        break;
    case PALETTE_BG_SUB:
        GXS_LoadBGPltt(src, destOffset, count * 2);
        break;
    case PALETTE_OBJ_MAIN:
        GX_LoadOBJPltt(src, destOffset, count * 2);
        break;
    case PALETTE_OBJ_SUB:
        GXS_LoadOBJPltt(src, destOffset, count * 2);
        break;
    }
}

// The arrows are active when the log has a beacon that way
// pokebw2's BeaconDetailSub_UpdateTouchBar
static void func_ov216_021bcd60(BeaconDetailWork *work) {
    func_ov067_021b6548(work->touchBar, BEACON_DETAIL_ICON_RETURN, TRUE);
    if (work->index != 0) {
        func_ov067_021b6548(work->touchBar, BEACON_DETAIL_ICON_PREV, TRUE);
    } else {
        func_ov067_021b6548(work->touchBar, BEACON_DETAIL_ICON_PREV, FALSE);
    }
    if (work->index < work->count - 1) {
        func_ov067_021b6548(work->touchBar, BEACON_DETAIL_ICON_NEXT, TRUE);
    } else {
        func_ov067_021b6548(work->touchBar, BEACON_DETAIL_ICON_NEXT, FALSE);
    }
}

// The palette of the panel's frame: Black's, White's, or the one of Black 2 and White 2
// pokebw2's BeaconDetailSub_GetVersionPalette
static u32 func_ov216_021bcdc0(const GameBeacon *beacon) {
    switch (func_ov010_02165c48(beacon)) {
    case VERSION_BLACK:
        return 0;
    case VERSION_WHITE:
        return 1;
    default:
        return 2;
    }
}

// pokebw2's BeaconDetailSub_FindRecent
const BeaconDetailRecent *func_ov216_021bcddc(u32 kind) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov216_021bdda2); i++) {
        if (kind == data_ov216_021bdda2[i].kind) {
            return &data_ov216_021bdda2[i];
        }
    }
    return &data_ov216_021bdda2[0];
}

void func_ov216_021bce04(const GameBeacon *beacon, WordSet *wordSet) {
    switch (func_ov010_02165dac(beacon)) {
    case 0:
    case 1:
        WordSet_LoadSpeciesName(wordSet, 0, func_ov010_02165df0(beacon));
        break;
    case 2: {
        u16 trainer = func_ov010_02165dbc(beacon);

        loadTrainerTypeToStrbuf(wordSet, 0, trainer);
        loadTrainerNamesToStrbuf(wordSet, 1, trainer);
        break;
    }
    case 3:
    case 4:
        break;
    default: {
        u16 zone = func_ov010_02165d28(beacon);

        if (zone >= ZONE_ASPERTIA_CITY) {
            zone = ZONE_ROUTE_1;
        }
        loadLocationNameToStrbuf(wordSet, 0, ZoneData_GetPlaceNameID(zone));
        break;
    }
    }
}


// pokebw2's BeaconDetailSub_SetActorAnim
static void func_ov216_021bce7c(ClActor *actor, u8 anim) {
    func_0204b878(actor, anim);
    func_0204b9a0(actor);
}

// pokebw2's BeaconDetailSub_SetActorPos
static void func_ov216_021bce8c(ClActor *actor, s16 x, s16 y, u16 surface) {
    ClActorPos pos;

    pos.x = x;
    pos.y = y;
    func_0204b404(actor, &pos, surface);
}

// Loads a message into the text buffer, with the words set
// pokebw2's BeaconDetailSub_FormatMessage
static void func_ov216_021bcea4(BeaconDetailWork *work, u8 messageId) {
    GFL_MsgDataLoadStrbuf(work->msgData, messageId, work->formatBuf);
    GFL_WordSetFormatStrbuf(work->wordSet, work->textBuf, work->formatBuf);
}

// Gives the map's trainer the characters and the palette of a view
// pokebw2's BeaconDetailSub_SetMapTrainer
void func_ov216_021bced0(BeaconDetailWork *work, u8 view) {
    u8 color;

    func_0204a9d0(work->unionRes.chars, work->unionChars[view].data);
    color = data_ov216_021bdd92[view] * 16;
    func_ov216_021bcd08(&work->unionPalette.raw[color], PALETTE_OBJ_MAIN, 10 * 16, 16);
}

// pokebw2's BeaconDetailSub_DrawLabels
static void func_ov216_021bcf08(BeaconDetailWork *work) {
    int i;
    int j;

    for (i = 0; i < 4; i++) {
        GFL_MsgDataLoadStrbuf(work->msgData, Startmenu_Text_000 + i, work->formatBuf);
        for (j = 0; j < BEACON_DETAIL_PANEL_COUNT; j++) {
            BeaconDetailPanel *panel = &work->panels[j];

            GFL_BitmapFill(panel->windows[i].bitmap, data_ov216_021bdd8a[i % 2].background);
            func_ov216_021bda54(work, &panel->windows[i], 0, 0, work->formatBuf, data_ov216_021bdd8a[i % 2].color);
            BmpWin_TransferNow(panel->windows[i].window);
        }
    }
    GFL_MsgDataLoadStrbuf(work->msgData, Startmenu_Text_007, work->formatBuf);
    for (i = 0; i < BEACON_DETAIL_PANEL_COUNT; i++) {
        BeaconDetailPanel *panel = &work->panels[i];

        GFL_BitmapFill(panel->windows[BEACON_DETAIL_WINDOW_FROM].bitmap, 3);
        func_ov216_021bda54(work, &panel->windows[BEACON_DETAIL_WINDOW_FROM], 0, 0, work->formatBuf,
                              PRINT_COLOR(15, 14, 3));
        BmpWin_TransferNow(panel->windows[BEACON_DETAIL_WINDOW_FROM].window);
    }
    GFL_MsgDataLoadStrbuf(work->msgData, Startmenu_Text_009, work->formatBuf);
    for (i = 0; i < BEACON_DETAIL_PANEL_COUNT; i++) {
        BeaconDetailPanel *panel = &work->panels[i];

        GFL_BitmapFill(panel->windows[BEACON_DETAIL_WINDOW_COUNTS].bitmap, 3);
        func_ov216_021bda54(work, &panel->windows[BEACON_DETAIL_WINDOW_COUNTS], 0, 0, work->formatBuf,
                              PRINT_COLOR(15, 14, 3));
        BmpWin_TransferNow(panel->windows[BEACON_DETAIL_WINDOW_COUNTS].window);
    }
}

// Clears a window from a column on, width by height characters, and prints a string there
static void func_ov216_021bd07c(BeaconDetailWork *work, BeaconDetailWindow *window, StrBuf *strbuf, u8 clearX, u8 x,
                                    u8 width, u8 height, u8 style) {
    GFL_BitmapFillArea(window->bitmap, clearX * 8, 0, width * 8, height * 8, data_ov216_021bdd8a[style].background);
    func_ov216_021bda54(work, window, x, 0, strbuf, data_ov216_021bdd8a[style].color);
    BmpWin_TransferNow(window->window);
}

// pokebw2's BeaconDetailSub_SetPanelVisible
static void func_ov216_021bd0e4(BeaconDetailWork *work, u8 page, BOOL visible) {
    BeaconDetailPanel *panel = &work->panels[page];

    func_0204b3dc(panel->trainerActor, visible);
    if (panel->rankShown) {
        func_0204b3dc(panel->rankActor, visible);
    }
    if (func_0202dcc4(work->pmsDraw, page)) {
        func_0202dcb0(work->pmsDraw, page, visible);
    }
    GFL_BGSysSetBGEnabled(panel->bg, visible);
}

// pokebw2's BeaconDetailSub_ScrollPanel
static void func_ov216_021bd13c(BeaconDetailPanel *panel, s16 y) {
    GFL_BGSysMoveBG(panel->bg, BG_MOVE_SET_Y, -y);
    func_ov216_021bce8c(panel->trainerActor, 220, y + 44, CLACT_SURFACE_SUB);
    func_ov216_021bce8c(panel->rankActor, 200, y + 80, CLACT_SURFACE_SUB);
}

// Draws a beacon's profile into a panel
// pokebw2's BeaconDetailSub_DrawPanel
static void func_ov216_021bd174(BeaconDetailWork *work, GameBeacon *beacon, u16 time, u8 page) {
    BeaconDetailPanel *panel = &work->panels[page];
    u16 colors[2];
    PMSData record;
    u16 hours;
    u16 minutes;
    int palette;
    u16 country;
    u8 answer;
    u8 rank;

    // The sender's favorite color and a darker shade of it
    func_ov010_02165cc0(&colors[0], beacon);
    func_02021f64(&colors[0], &colors[1], 1, 3, 0);
    palette = page * 16;
    func_ov216_021bcd08(colors, PALETTE_BG_SUB, palette + 1, 2);
    func_ov216_021bcd08(&work->versionPalette.raw[func_ov216_021bcdc0(beacon) * 16],
                                PALETTE_BG_SUB, palette + 10, 2);

    func_ov010_02165c34(beacon, work->formatBuf);
    func_ov216_021bd07c(work, &panel->windows[BEACON_DETAIL_WINDOW_NAME], work->formatBuf, 6, 48, 13, 2, 0);

    answer = func_ov010_02165c60(beacon);
    if (answer == 0xff) {
        GFL_StrBufClear(work->textBuf);
    } else {
        loadJobAnswerToStrbuf(work->wordSet, 0, answer);
        func_ov216_021bcea4(work, Startmenu_Text_005);
    }
    func_ov216_021bd07c(work, &panel->windows[BEACON_DETAIL_WINDOW_JOB], work->textBuf, 6, 48, 13, 2, 1);

    answer = func_ov010_02165c78(beacon);
    if (answer == 0xff) {
        GFL_StrBufClear(work->textBuf);
    } else {
        loadHobbyNameToStrbuf(work->wordSet, 0, answer);
        func_ov216_021bcea4(work, Startmenu_Text_004);
    }
    func_ov216_021bd07c(work, &panel->windows[BEACON_DETAIL_WINDOW_HOBBY], work->textBuf, 6, 48, 13, 2, 0);

    func_ov010_02165d10(beacon, &hours, &minutes);
    WordSetNumber(work->wordSet, 2, hours, 3, 0, TRUE);
    WordSetNumber(work->wordSet, 3, minutes, 2, 2, TRUE);
    func_ov216_021bcea4(work, Startmenu_Text_006);
    func_ov216_021bd07c(work, &panel->windows[BEACON_DETAIL_WINDOW_TIME], work->textBuf, 6, 48, 13, 2, 1);

    country = func_ov010_02165c58(beacon);
    loadCountryToStrbuf(work->wordSet, 0, country);
    loadCountryAreaToStrbuf(work->wordSet, 1, country, func_ov010_02165c5c(beacon));
    func_ov216_021bcea4(work, Startmenu_Text_008);
    func_ov216_021bd07c(work, &panel->windows[BEACON_DETAIL_WINDOW_HOME], work->textBuf, 0, 0, 28, 4, 0);

    WordSetNumber(work->wordSet, 0, func_ov010_02165d40(beacon), 5, 1, TRUE);
    WordSetNumber(work->wordSet, 1, func_ov010_02165d48(beacon), 5, 1, TRUE);
    func_ov216_021bcea4(work, Startmenu_Text_010);
    func_ov216_021bd07c(work, &panel->windows[BEACON_DETAIL_WINDOW_COUNTS], work->textBuf, 21, 0, 6, 4, 0);

    if (func_0202dcc4(work->pmsDraw, page)) {
        func_0202dc9c(work->pmsDraw, page, TRUE);
    }
    func_ov010_02165c90(beacon, &record);
    func_0202dc48(work->pmsDraw, panel->recordWindow, &record, page);

    panel->trainerView = func_ov010_02165ce8(beacon);
    func_0202daa4(work->trainerArc, func_02025c88(panel->trainerView), work->trainerRes[page].chars,
                                      work->trainerRes[page].palette, work->tailHeapId);

    rank = func_ov010_02165cb4(beacon);
    if (rank == 0) {
        func_0204b3dc(panel->rankActor, FALSE);
    } else {
        func_ov216_021bce7c(panel->rankActor, rank + 1);
        func_0204b3dc(panel->rankActor, TRUE);
    }
}

void func_ov216_021bd448(BeaconDetailWork *work) {
    const BeaconDetailRecent *recent;
    u16 zone;
    u16 place;
    u8 hour;

    func_ov010_02165ac4(work->infoTable, work->beacon, &work->time, work->logIndices[work->index]);
    recent = func_ov216_021bcddc(func_ov010_02165dac(work->beacon));
    func_ov216_021bced0(work, func_ov010_02165ce8(work->beacon));
    zone = func_ov010_02165d28(work->beacon);
    place = func_ov072_021bbb48(work->townMapData, func_ov010_02165d34(work->beacon));
    if (place == TOWNMAP_PLACE_NONE ||
        func_ov072_021bbb3c(work->townMapData, place, TOWNMAP_PARAM_FLAG) != TOWNMAP_NO_FLAG ||
        zone >= ZONE_ASPERTIA_CITY) {
        work->hasPlace = FALSE;
        work->markerY = 0;
        work->markerX = work->markerY;
        func_0204b3dc(work->actors[BEACON_DETAIL_ACTOR_TRAINER], FALSE);
        func_0204b3dc(work->actors[BEACON_DETAIL_ACTOR_RECENT], FALSE);
    } else {
        work->hasPlace = TRUE;
        work->markerX = func_ov072_021bbb3c(work->townMapData, place, TOWNMAP_PARAM_X);
        work->markerY = func_ov072_021bbb3c(work->townMapData, place, TOWNMAP_PARAM_Y) - 16;
        if (work->markerY < 32) {
            work->markerY = 32;
        } else if (work->markerY > 152) {
            work->markerY = 152;
        }
        if (work->markerX < 16) {
            work->markerX = 16;
        } else if (work->markerX > 240) {
            work->markerX = 240;
        }
        func_ov216_021bce8c(work->actors[BEACON_DETAIL_ACTOR_TRAINER], work->markerX, work->markerY, CLACT_SURFACE_MAIN);
        func_0204b3dc(work->actors[BEACON_DETAIL_ACTOR_TRAINER], TRUE);
        if (recent->icon != 0) {
            func_ov216_021bce8c(work->actors[BEACON_DETAIL_ACTOR_RECENT], work->markerX - 24, work->markerY,
                                CLACT_SURFACE_MAIN);
            func_ov216_021bce7c(work->actors[BEACON_DETAIL_ACTOR_RECENT], recent->icon - 1);
        }
        func_0204b3dc(work->actors[BEACON_DETAIL_ACTOR_RECENT], recent->icon != 0);
    }

    GFL_BitmapFill(work->infoWindow.bitmap, 15);
    hour = work->time >> 8;
    WordSetNumber(work->wordSet, 0, hour, 2, 0, TRUE);
    WordSetNumber(work->wordSet, 1, (u8)work->time, 2, 2, TRUE);
    func_ov216_021bcea4(work, Startmenu_Text_013 + hour / 12);
    func_ov216_021bda54(work, &work->infoWindow, 0, 0, work->textBuf, PRINT_COLOR(7, 8, 15));
    func_ov216_021bce04(work->beacon, work->wordSet);
    func_ov216_021bcea4(work, recent->message);
    func_ov216_021bda54(work, &work->infoWindow, 0, 16, work->textBuf, PRINT_COLOR(7, 8, 15));
    BmpWin_TransferNow(work->infoWindow.window);
}


// Slides the touch screen's info window up over the map, or back down
// pokebw2's BeaconDetailSub_StartMapSlide
static void func_ov216_021bd668(BeaconDetailWork *work, u8 out, int *taskCount) {
    BeaconDetailMapSlideTask *slide =
        GFL_TCBExGetData(GFL_TCBExMgrAddTask(work->tcbManager, func_ov216_021bd6bc, sizeof(*slide), 0));

    MI_CpuFill8(slide, 0, sizeof(*slide));
    slide->out = out;
    if (!slide->out) {
        slide->y = 0;
        slide->speed = POPUP_SLIDE_SPEED;
    } else {
        slide->y = POPUP_SLIDE_Y;
        slide->speed = -POPUP_SLIDE_SPEED;
    }
    slide->frames = POPUP_SLIDE_FRAMES;
    func_ov216_021bcd00(SEQ_SE_DECIDE2);
    slide->taskCount = taskCount;
    (*taskCount)++;
}

// pokebw2's BeaconDetailSub_MapSlideTask
static void func_ov216_021bd6bc(TCBEx *task, void *data) {
    BeaconDetailMapSlideTask *slide = data;

    if (slide->frame++ < slide->frames) {
        slide->y += slide->speed;
        GFL_BGSysMoveBG(1, BG_MOVE_SET_Y, slide->y);
    } else {
        (*slide->taskCount)--;
        GFL_TCBExRequestEnd(task);
    }
}

// pokebw2's BeaconDetailSub_StartPopup
static void func_ov216_021bd6f0(BeaconDetailWork *work) {
    func_ov216_021bd6fc(work, &work->taskCount);
}

// pokebw2's BeaconDetailSub_StartPopupTask
static void func_ov216_021bd6fc(BeaconDetailWork *work, int *taskCount) {
    BeaconDetailPopupTask *popup =
        GFL_TCBExGetData(GFL_TCBExMgrAddTask(work->tcbManager, func_ov216_021bd768, sizeof(*popup), 0));

    MI_CpuFill8(popup, 0, sizeof(*popup));
    popup->work = work;
    popup->timer = POPUP_FRAMES;
    func_ov067_021b64a8(work->touchBar, FALSE);
    popup->parentTaskCount = taskCount;
    (*taskCount)++;
}

// Whether the pop-up stays: until its time is up, a touch or the A Button
// pokebw2's BeaconDetailSub_WaitPopup
static BOOL func_ov216_021bd73c(BeaconDetailPopupTask *popup) {
    if (func_02035c60() || (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A)) {
        popup->timer = 0;
        return FALSE;
    }
    if (popup->timer-- == 0) {
        return FALSE;
    }
    return TRUE;
}

// pokebw2's BeaconDetailSub_PopupTask
static void func_ov216_021bd768(TCBEx *task, void *data) {
    BeaconDetailPopupTask *popup = data;

    switch (popup->seq) {
    case 0:
        func_ov216_021bd668(popup->work, FALSE, &popup->taskCount);
        popup->seq++;
        break;
    case 1:
        if (popup->taskCount == 0) {
            popup->seq++;
        }
        break;
    case 2:
        if (!func_ov216_021bd73c(popup)) {
            func_ov216_021bd668(popup->work, TRUE, &popup->taskCount);
            popup->seq++;
        }
        break;
    case 3:
        if (popup->taskCount) {
            break;
        }
    default:
        (*popup->parentTaskCount)--;
        GFL_TCBExRequestEnd(task);
        break;
    }
}

// pokebw2's BeaconDetailSub_StartPanelSlide
static void func_ov216_021bd7d4(BeaconDetailWork *work, u8 page, int from, u8 direction, int *taskCount) {
    BeaconDetailPanelSlideTask *slide =
        GFL_TCBExGetData(GFL_TCBExMgrAddTask(work->tcbManager, func_ov216_021bd854, sizeof(*slide), 0));

    MI_CpuFill8(slide, 0, sizeof(*slide));
    slide->panel = &work->panels[page];
    slide->page = page;
    slide->y = (PANEL_SLIDE_OUT - from) * -SCREEN_HEIGHT;
    if (direction == PAGE_NEXT) {
        slide->speed -= PANEL_SLIDE_SPEED;
    } else {
        slide->speed += PANEL_SLIDE_SPEED;
    }
    slide->frames = PANEL_SLIDE_FRAMES;
    func_ov216_021bd13c(slide->panel, slide->y);
    if (from != PANEL_SLIDE_OUT) {
        func_ov216_021bd0e4(work, slide->page, TRUE);
    }
    slide->work = work;
    slide->taskCount = taskCount;
    (*taskCount)++;
}

// pokebw2's BeaconDetailSub_PanelSlideTask
static void func_ov216_021bd854(TCBEx *task, void *data) {
    BeaconDetailPanelSlideTask *slide = data;

    if (slide->frames-- != 0) {
        slide->y += slide->speed;
        func_ov216_021bd13c(slide->panel, slide->y);
        return;
    }
    // The panel that slid off the screen
    if (slide->y != 0) {
        func_ov216_021bd0e4(slide->work, slide->page, FALSE);
    }
    (*slide->taskCount)--;
    GFL_TCBExRequestEnd(task);
}

// pokebw2's BeaconDetailSub_TurnPage
static void func_ov216_021bd89c(BeaconDetailWork *work, u8 direction) {
    func_ov216_021bd8a8(work, direction, &work->taskCount);
}

// Draws the next or the previous beacon into the hidden panel, to slide it in once it is printed
// pokebw2's BeaconDetailSub_StartPageTask
static void func_ov216_021bd8a8(BeaconDetailWork *work, u8 direction, int *taskCount) {
    BeaconDetailPageTask *page =
        GFL_TCBExGetData(GFL_TCBExMgrAddTask(work->tcbManager, func_ov216_021bd948, sizeof(*page), 0));

    MI_CpuFill8(page, 0, sizeof(*page));
    page->direction = direction;
    if (direction == PAGE_NEXT) {
        page->index = work->index + 1;
        func_ov067_021b6548(work->touchBar, BEACON_DETAIL_ICON_PREV, FALSE);
    } else {
        page->index = work->index - 1;
        func_ov067_021b6548(work->touchBar, BEACON_DETAIL_ICON_NEXT, FALSE);
    }
    func_ov067_021b6548(work->touchBar, BEACON_DETAIL_ICON_RETURN, FALSE);
    func_ov010_02165ac4(work->infoTable, work->beacon, &work->time, work->logIndices[page->index]);
    func_ov216_021bd174(work, work->beacon, work->time, work->page ^ 1);
    page->work = work;
    page->parentTaskCount = taskCount;
    (*taskCount)++;
}

// pokebw2's BeaconDetailSub_PageTask
static void func_ov216_021bd948(TCBEx *task, void *data) {
    BeaconDetailPageTask *page = data;

    // The arrow's animation ends before the panels slide
    if (!page->decided && func_ov067_021b6448(page->work->touchBar) != -1) {
        page->decided = TRUE;
    }
    if (!func_0201c6b4(page->work->printQueue)) {
        return;
    }
    switch (page->seq) {
    case 0:
        if (!page->decided) {
            return;
        }
        func_ov067_021b64a8(page->work->touchBar, FALSE);
        page->seq++;
        break;
    case 1:
        func_ov216_021bd7d4(page->work, page->work->page, PANEL_SLIDE_OUT, page->direction,
                                        &page->taskCount);
        func_ov216_021bd7d4(page->work, page->work->page ^ 1, data_ov216_021bdd88[page->direction],
                                        page->direction, &page->taskCount);
        func_0204b3dc(page->work->actors[BEACON_DETAIL_ACTOR_TRAINER], FALSE);
        func_0204b3dc(page->work->actors[BEACON_DETAIL_ACTOR_RECENT], FALSE);
        page->seq++;
        break;
    case 2:
        if (page->taskCount) {
            return;
        }
        page->work->page ^= 1;
        page->work->index = page->index;
        func_ov216_021bd448(page->work);
        page->seq++;
        break;
    case 3:
    default:
        (*page->parentTaskCount)--;
        GFL_TCBExRequestEnd(task);
        break;
    }
}

static void func_ov216_021bda54(BeaconDetailWork *work, BeaconDetailWindow *window, s16 x, s16 y, StrBuf *strbuf,
                                  u16 color) {
    PrintQueue *queue = work->printQueue;

    func_0201c724(queue, BmpWin_GetBitmap(window->print.window), x, y, strbuf, work->font, color);
    window->print.flushPending = TRUE;
    func_ov216_021bdaa0(work, window, &work->taskCount);
}


// pokebw2's BeaconDetailSub_StartFlushTask
void func_ov216_021bdaa0(BeaconDetailWork *work, BeaconDetailWindow *window, int *taskCount) {
    BeaconDetailFlushTask *flush =
        GFL_TCBExGetData(GFL_TCBExMgrAddTask(work->tcbManager, func_ov216_021bdad8, sizeof(*flush), 0));

    MI_CpuFill8(flush, 0, sizeof(*flush));
    flush->work = work;
    flush->window = window;
    if (taskCount != NULL) {
        flush->taskCount = taskCount;
        (*taskCount)++;
    }
}

// pokebw2's BeaconDetailSub_FlushTask
void func_ov216_021bdad8(TCBEx *task, void *data) {
    BeaconDetailFlushTask *flush = data;
    PrintWindow *print = &flush->window->print;
    PrintQueue *queue = flush->work->printQueue;

    PrintWindow_Flush(print, queue);
    if (!print->flushPending) {
        if (flush->taskCount != NULL) {
            (*flush->taskCount)--;
        }
        GFL_TCBExRequestEnd(task);
    }
}

