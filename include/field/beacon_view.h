// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/beacon_view.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_BEACON_VIEW_H
#define BWPORT_FIELD_BEACON_VIEW_H

#include "types.h"
#include "field/beacon_view_common.h"
#include "field/festival.h"
#include "gfl/clact.h"
#include "nnsys/g2d.h"
#include "system/app_taskmenu.h"
#include "system/app_keycursor.h"
#include "system/game_beacon.h"
#include "system/printsys.h"
#include "struct_decls.h"

#define BEACON_VIEW_PANEL_MAX 5

// Black 2's, not checked against Black
// The log's lines on the screen
#define BEACON_VIEW_LINE_MAX 4

// Black 2's, not checked against Black
#define BEACON_VIEW_PANEL_DIGITS 3
#define BEACON_VIEW_TRAINER_CHARS 16
#define BEACON_VIEW_ICON_CHARS 12
// The events the view asks the field for (BeaconView_RequestEvent), which BeaconView_CheckEvent creates
#define BEACON_VIEW_EVENT_NONE 0

#define BEACON_VIEW_EVENT_MENU 1
#define BEACON_VIEW_EVENT_MENU_COMM 2
#define BEACON_VIEW_EVENT_MENU_WIFI 3

// Black 2's, not checked against Black
#define BEACON_VIEW_EVENT_DETAIL 4
#define BEACON_VIEW_EVENT_MEDAL 5
#define BEACON_VIEW_EVENT_PASS_POWER 6
#define BEACON_VIEW_EVENT_PASS_POWER_USE 7
#define BEACON_VIEW_EVENT_PASS_POWER_LIST 8
#define BEACON_VIEW_EVENT_PASS_POWER_LIST_2 9
#define BEACON_VIEW_EVENT_GREETING 10
#define BEACON_VIEW_EVENT_MISSION_START 11
#define BEACON_VIEW_EVENT_MISSION_MESSAGE 12
#define BEACON_VIEW_EVENT_MISSION_INFO 13
// The actors of the screen itself
#define BEACON_VIEW_ACTOR_PASS_POWER 0

// Black 2's, not checked against Black
#define BEACON_VIEW_ACTOR_GREETING 1
#define BEACON_VIEW_ACTOR_RETURN 3
#define BEACON_VIEW_ACTOR_UP 4
#define BEACON_VIEW_ACTOR_COUNT 6
#define BEACON_VIEW_WIN_MESSAGE 0
#define BEACON_VIEW_WIN_TOTAL 1
#define BEACON_VIEW_WIN_COUNT 3
// The sets of OAM resources: the screen's own, the trainers' and the icons'
#define BEACON_VIEW_RES_COMMON 0

// Black 2's, not checked against Black
#define BEACON_VIEW_RES_TRAINER 1
#define BEACON_VIEW_RES_ICON 2
#define BEACON_VIEW_RES_COUNT 3

#define BEACON_VIEW_RES_PALETTE 0
#define BEACON_VIEW_RES_CHARS 1
#define BEACON_VIEW_RES_CELLS 2
#define BEACON_VIEW_RES_KINDS 3

// The registered resources of one kind
typedef struct {
    u32 count;
    u32 *ids;
} BeaconViewResList;

typedef struct {
    BeaconViewResList lists[BEACON_VIEW_RES_KINDS];
} BeaconViewRes;

// Black 2's, not checked against Black
typedef struct {
    BeaconViewWin win;
    u32 unused;
} BeaconViewWinSlot;

// Black 2's, not checked against Black
// A character file kept in memory, to change an actor's characters from
typedef struct {
    NNSG2dCharacterData *data;
    void *file;
} BeaconViewChars;

// Black 2's, not checked against Black
typedef struct {
    NNSG2dPaletteData *data;
    u16 *colors;
    void *file;
} BeaconViewPalette;

// A line of the log on the screen: a plate with the player's sprite, name or message and icon. One more than the
// screen shows, for the line that scrolls in
// The plate's text: the OAM actor and the bitmap it shows
typedef struct {
    BmpOamActor *actor;
    GFLBitmap *bitmap;
} BeaconViewPanelOam;

typedef struct {
    u8 id;
    // The log entry it shows, or 0xff when it is free
    u8 dataIndex;
    // 1 to 4 on the screen, 0 above it and 5 below
    u8 line;
    u8 rank;
    u8 gender;
    u8 unk5;
    u16 trainerId;
    s16 x;
    s16 y;
    ClActor *base;
    ClActor *trainer;
    ClActor *icon;
    BeaconViewPanelOam oam;
    StrBuf *greeting;
    StrBuf *name;
    PrintQueue *queue;
    // The task that waits for the plate's text to be printed
    TCBEx *printTask;
    // The task that shows the plate's icon for a while
    TCBEx *iconTask;
} BeaconViewPanel;

// A button of the task menu: its window and its item
typedef struct {
    AppTaskMenuWin *window;
    AppTaskMenuItem item;
} BeaconViewButton;

typedef struct BeaconViewWork {
    GameSystem *gsys;
    GameData *gameData;
    Field *field;
    void *parent;
    FieldDispControl *dispControl;
    TrainerCardSave *trainerCard;
    BagSave *bag;
    GameCommSys *commSys;
    WifiList *wifiList;
    void *passPowerData;
    u32 unk_28;
    u32 trainerId;
    u8 gender;
    u8 version;
    u8 unk_32;
    BeaconStatus *status;
    // The beacons received since the last frame
    GameBeaconInfoTable *newTable;
    // The game's log
    GameBeaconInfoTable *log;
    GameBeacon *beacon;
    GameBeacon *countBeacon;
    u16 time;
    u16 newTime;
    GameBeacon *newBeacon;
    BeaconViewPanel *newPanel;
    u8 newOffset;
    u8 newIndex;
    u8 unk56;
    u8 oldCount;
    // How many of the log are of the other gender, and of another version
    u8 otherGenderCount;
    u8 otherVersionCount;
    BOOL isNew;
    BOOL active;
    u32 unk64;
    int seq;
    int subSeq;
    u16 eventRequest;
    u16 eventRunning;
    int wait;
    u8 unk78;
    // Set while an event the screen asked for runs
    u8 eventWait;
    // Set while the screen is first drawn: text is printed at once, not through the queue
    u8 printNow;
    u8 logCount;
    u8 top;
    u8 viewCount;
    u8 selected;
    u8 selectedPanel;
    u8 barDragging;
    u8 scrolling;
    u8 scrollEnd;
    u8 scrollReturnSeq;
    int scrollDir;
    int scrollFrames;
    // The records reached while the screen is shown, each once
    u8 recordCount;
    u8 records[7];
    u16 resHeapId;
    u16 tmpHeapId;
    u16 heapId;
    ArcTool *arc;
    TCBExManager *tcbManager;
    TCB *vblankTask;
    // The tasks still running
    int pending;
    PaletteFade *paletteFade;
    Font *font;
    PrintQueue *printQueue;
    AppTaskMenuRes *taskMenuRes;
    KeyCursor *keyCursor;
    BeaconViewButton buttons[3];
    BeaconViewButton buttons2[2];
    WordSet *wordSet;
    MsgData *msgData;
    StrBuf *strTemplate;
    StrBuf *strExpand;
    StrBuf *strMessage;
    BmpOamSys *bmpOam;
    ClActUnit *clUnit;
    ClActRenderer *renderer;
    u32 unk130;
    BeaconViewRes res[BEACON_VIEW_RES_COUNT];
    BeaconViewWinSlot wins[BEACON_VIEW_WIN_COUNT];
    BeaconViewPanel panels[BEACON_VIEW_PANEL_MAX];
    ClActor *actors[BEACON_VIEW_ACTOR_COUNT];
    // How many players the player has passed by
    u32 passerbyCount;
    BeaconViewChars iconChars[BEACON_VIEW_ICON_CHARS];
    BeaconViewChars trainerChars[BEACON_VIEW_TRAINER_CHARS];
    BeaconViewPalette trainerPalette;
    BeaconViewPalette commonPalette;
} BeaconViewWork;

// Black 2's, not checked against Black
#define BEACON_VIEW_ACTOR_THANKS 2
// The four buttons at once, for BeaconView_SetButtonAnim
#define BEACON_VIEW_BUTTON_ALL 4

// A point of the touch screen
typedef struct {
    int x;
    int y;
} BeaconViewTouch;

// pokebw2's BeaconViewPanel_Assign
void func_ov032_021f0f30(BeaconViewPanel *panel, u8 dataIndex, u8 line);

// Black 2's, not checked against Black
#define BEACON_VIEW_ACTOR_DOWN 5

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BeaconView_SetButtonAnim
// Sets the animation of one of the four buttons, or of all with BEACON_VIEW_BUTTON_ALL: 0 idle, 1 touched, 2 off
void func_ov032_021f05e8(BeaconViewWork *work, int button, int state);
// pokebw2's BeaconView_IsButtonAnimating
BOOL func_ov032_021f0668(BeaconViewWork *work, int button);
// pokebw2's BeaconView_RequestEvent
void func_ov032_021f253c(BeaconViewWork *work, u16 request);
// pokebw2's BeaconView_HitTestArrow
int func_ov032_021f096c(BeaconViewWork *work, BeaconViewTouch *touch);
// pokebw2's BeaconView_CountOthers
// beacon_view_sub.c
void func_ov032_021f06e0(BeaconViewWork *work, const GameBeacon *beacon, u8 oldCount);
// Notes a record reached, once
void func_ov032_021f079c(BeaconViewWork *work, u8 record);
// pokebw2's BeaconView_GetRecordPassPower
u32 func_ov032_021f07d4(BeaconViewWork *work);
// pokebw2's BeaconView_FindPanel
BeaconViewPanel *func_ov032_021f0f44(BeaconViewWork *work, u8 dataIndex);
// Puts the plate at x and y
void func_ov032_021f1218(BeaconViewPanel *panel, s16 x, s16 y);
// pokebw2's BeaconView_ScrollLine
void func_ov032_021f12b8(BeaconViewWork *work, int dir);
// pokebw2's BeaconView_UpdateNameColors
void func_ov032_021f13a0(BeaconViewWork *work, const GameBeacon *beacon, u8 dataIndex, u8 dir, int mode);
// pokebw2's BeaconView_ScrollLine
// pokebw2's BeaconView_StartScroll
// pokebw2's BeaconView_DragBar
// pokebw2's BeaconView_AddLine
void func_ov032_021f155c(BeaconViewWork *work, BeaconViewPanel *panel, u8 dir, u8 delay, int *pending);
// pokebw2's BeaconView_StartJump
void func_ov032_021f18c4(BeaconViewWork *work, u8 top);
// pokebw2's BeaconView_ShowBeaconMessage
// pokebw2's BeaconView_ShowMessage
void func_ov032_021f1a08(BeaconViewWork *work);
// pokebw2's BeaconView_OfferRecordPassPower
// pokebw2's BeaconView_OfferPassPower
void func_ov032_021f1e40(BeaconViewWork *work, BeaconViewPanel *panel, const GameBeacon *beacon, BOOL greeting);
// pokebw2's BeaconView_OfferMission
void func_ov032_021f1f00(BeaconViewWork *work, u8 selected);

void func_ov032_021efec0(BeaconViewWork *work);
void func_ov032_021f0208(BeaconViewWork *work, BOOL flag);
// Makes the beacon view, and frees it
BeaconViewWork *func_ov032_021f2234(GameSystem *gsys, void *parent);
void func_ov032_021f22fc(BeaconViewWork *work);
int func_ov032_021eff7c(BeaconViewWork *work);
BOOL func_ov032_021f0078(BeaconViewWork *work);
BOOL func_ov032_021f00c8(BeaconViewWork *work);
int func_ov032_021f0114(BeaconViewWork *work);
BOOL func_ov032_021f0140(BeaconViewWork *work);
BOOL func_ov032_021f02c4(BeaconViewWork *work);
void func_ov032_021f067c(BeaconViewWork *work);
int func_ov032_021f0050(BeaconViewWork *work);
BOOL func_ov032_021f03ec(BeaconViewWork *work);
int func_ov032_021f0438(BeaconViewWork *work);
void *func_02012a54(GameData *gameData);
s32 func_02012ffc(void);
void func_020275b8(BeaconStatus *status, u8 top);
StrBuf *func_020275c4(BeaconStatus *status);
u16 *func_020275d0(BeaconStatus *status);
void *func_ov010_02165b2c(HeapID heapId);
void *func_ov010_02165b10(HeapID heapId);
void func_ov032_021f0684(BeaconViewWork *work, u8 *genderCount, u8 *versionCount);
void func_ov032_021f028c(BeaconViewWork *work);
void func_ov032_021f029c(BeaconViewWork *work, int param);
u32 func_ov032_021f08e4(BeaconViewWork *work, BeaconViewTouch *touch);
int func_ov032_021f0a18(BeaconViewWork *work, BeaconViewTouch *touch);
void func_ov032_021f1340(BeaconViewWork *work, int dir, u8 returnSeq);
void func_ov032_021f0ac0(BeaconViewWork *work);
void func_ov032_021f0b10(BeaconViewWork *work, u8 messageId, int *pending);
void func_ov032_021f0df4(ClActor *actor, u16 anim);
void func_ov032_021f0e24(BeaconViewWork *work);
void func_ov032_021f0e7c(BeaconViewWork *work);
void func_ov032_021f0ea4(BeaconViewPanel *panel, BOOL visible);
void func_ov032_021f1144(BeaconViewWork *work, BeaconViewPanel *panel, const GameBeacon *beacon, int mode, BOOL quiet,
                         int *pending);

void func_ov032_021f2344(BeaconViewWork *work, BOOL active);
void func_ov032_021f23f8(BeaconViewWork *work);
GameEvent *func_ov032_021f2404(BeaconViewWork *work, BOOL active);

#endif // BWPORT_FIELD_BEACON_VIEW_H
