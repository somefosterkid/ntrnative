// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/beacon_detail_local.h: the lines in use
// so far.
#ifndef BWPORT_APP_BEACON_DETAIL_LOCAL_H
#define BWPORT_APP_BEACON_DETAIL_LOCAL_H

#include "types.h"
#include "app/beacon_detail_gra.h"
#include "app/ui/touchbar.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "nnsys/g2d.h"
#include "system/game_beacon.h"
#include "system/printsys.h"
#include "struct_decls.h"

// The two panels of details: the one shown and the one that slides in when the player turns the page
#define BEACON_DETAIL_PANEL_COUNT 2

#define BEACON_DETAIL_UNION_VIEW_COUNT 16

// Black 2's; twinned code confirms the values of BEACON_DETAIL_WINDOW_FROM and BEACON_DETAIL_WINDOW_COUNTS in Black
// The windows of a panel: the labels and values of the name, the job, the hobby and the play time, the "FROM:"
// label, the country and region, and the numbers of thanks and of passersby
enum {
    BEACON_DETAIL_WINDOW_NAME,
    BEACON_DETAIL_WINDOW_JOB,
    BEACON_DETAIL_WINDOW_HOBBY,
    BEACON_DETAIL_WINDOW_TIME,
    BEACON_DETAIL_WINDOW_FROM,
    BEACON_DETAIL_WINDOW_HOME,
    BEACON_DETAIL_WINDOW_COUNTS,
    BEACON_DETAIL_WINDOW_COUNT,
};

// The map's actors: the trainer where the beacon was sent from, and the icon of what the trainer was doing
enum {
    BEACON_DETAIL_ACTOR_TRAINER,
    BEACON_DETAIL_ACTOR_RECENT,
    BEACON_DETAIL_ACTOR_COUNT,
};

// The touch bar's icons
enum {
    BEACON_DETAIL_ICON_RETURN = 1,
    BEACON_DETAIL_ICON_NEXT = 2,
    BEACON_DETAIL_ICON_PREV = 3,
};

// The sequences of the screen's main function
enum {
    BEACON_DETAIL_SEQ_INIT,
    BEACON_DETAIL_SEQ_FADE_IN,
    BEACON_DETAIL_SEQ_INPUT,
    // Until the tasks that the input started have ended
    BEACON_DETAIL_SEQ_WAIT,
    BEACON_DETAIL_SEQ_FADE_OUT,
    BEACON_DETAIL_SEQ_END,
};

// A window with its bitmap, printed into through the print queue
struct BeaconDetailWindow {
    BmpWin *window;
    GFLBitmap *bitmap;
    PrintWindow print;
};

// The cell actor resources of a set of graphics
struct BeaconDetailObjRes {
    u32 palette;
    u32 chars;
    u32 cellAnims;
};

struct BeaconDetailChars {
    NNSG2dCharacterData *data;
    void *file;
};

// Black 2's; twinned code confirms the offsets of raw and file in Black
struct BeaconDetailPalette {
    NNSG2dPaletteData *data;
    u16 *raw;
    void *file;
};

// Black 2's; twinned code confirms the offsets of windows, recordWindow, rankActor, trainerActor, rankShown and bg in
// Black
struct BeaconDetailPanel {
    BeaconDetailWindow windows[BEACON_DETAIL_WINDOW_COUNT];
    // The sender's C-Gear record
    BmpWin *recordWindow;
    ClActor *rankActor;
    ClActor *trainerActor;
    u8 trainerView;
    // Nothing sets it, so showing and hiding a panel leaves its rank icon alone
    u8 rankShown;
    u8 bg;
};

// Black 2's; matching code confirms the offsets of heapId, tailHeapId, seq, taskCount, empty, param, tcbManager,
// infoTable, beacon, time, page, count, index, logIndices, graphic, arc, trainerArc, iconRes, trainerRes, unionRes,
// unionChars, unionPalette, versionPalette, touchBar, font, printQueue, msgData, countryMsgData, wordSet, formatBuf,
// textBuf, unk144, infoWindow, pmsDraw, panels, markerX, markerY, hasPlace and actors in Black
struct BeaconDetailWork {
    HeapID heapId;
    HeapID tailHeapId;
    // The step of the sequence
    int seq;
    u32 unk8;
    // How many tasks are running
    int taskCount;
    // Set when the log holds no beacon to show, which ends the screen at once
    BOOL empty;
    BeaconDetailParam *param;
    TCBExManager *tcbManager;
    void *townMapData;
    BeaconStatus *beaconStatus;
    GameBeaconInfoTable *infoTable;
    // The beacon shown, and when it was received: the hour in the high byte and the minute in the low
    GameBeacon *beacon;
    u16 time;
    // The panel shown
    u8 page;
    // The beacons of the log that the screen shows, their offsets in the log, and the one shown
    u8 count;
    u8 index;
    u8 logIndices[GAME_BEACON_LOG_MAX];
    u8 unk_4f;
    BeaconDetailGraphic *graphic;
    ArcTool *arc;
    ArcTool *trainerArc;
    BeaconDetailObjRes iconRes;
    BeaconDetailObjRes trainerRes[BEACON_DETAIL_PANEL_COUNT];
    // The map's trainer, whose characters and palette are replaced with those of the sender's view
    BeaconDetailObjRes unionRes;
    BeaconDetailChars unionChars[BEACON_DETAIL_UNION_VIEW_COUNT];
    BeaconDetailPalette unionPalette;
    // The palettes of the versions' frames
    BeaconDetailPalette versionPalette;
    TouchBar *touchBar;
    Font *font;
    PrintQueue *printQueue;
    MsgData *msgData;
    MsgData *countryMsgData;
    WordSet *wordSet;
    StrBuf *formatBuf;
    StrBuf *textBuf;
    StrBuf *unk144;
    // What the sender was doing and when the beacon came, over the map
    BeaconDetailWindow infoWindow;
    PMSDraw *pmsDraw;
    BeaconDetailPanel panels[BEACON_DETAIL_PANEL_COUNT];
    // Where the sender is on the map, when the map has the place
    s16 markerX;
    s16 markerY;
    u8 hasPlace;
    ClActor *actors[BEACON_DETAIL_ACTOR_COUNT];
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BeaconDetailSub_Start
// Draws the beacon that the screen starts with
void func_ov216_021bcba4(BeaconDetailWork *work);
// pokebw2's BeaconDetailSub_Input
// Handles a frame's input and returns the sequence to go on with
u32 func_ov216_021bcc10(BeaconDetailWork *work);
// pokebw2's BeaconDetailSub_EndWait
// After the tasks of an input have ended
void func_ov216_021bccbc(BeaconDetailWork *work);
// Black 2's, not checked against Black
#define HEAPID_BEACON_DETAIL 0x77

// The two panels of details: the one shown and the one that slides in when the player turns the page

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BeaconDetailSub_Input
// Handles a frame's input and returns the sequence to go on with
// pokebw2's BeaconDetailSub_EndWait
// After the tasks of an input have ended
// pokebw2's BeaconDetailSub_Start
// Draws the beacon that the screen starts with

#endif // BWPORT_APP_BEACON_DETAIL_LOCAL_H
