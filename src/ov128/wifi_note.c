// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov204/wifi_note.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/codein.h"
#include "app/name_entry.h"
#include "app/ui/print_msg.h"
#include "app/ui/ui_scene.h"
#include "app/wf2dmap/wifi_2dchar.h"
#include "app/wifi_note.h"
#include "app/wificlub.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "dwc/dwc.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
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
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/math.h"
#include "nnsys/g2d.h"
#include "pml/poke_graphic.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "save/wifi_list.h"
#include "system/aeabi.h"
#include "system/app_menu_common.h"
#include "system/app_taskmenu.h"
#include "system/blink_palanm.h"
#include "system/bmp_winframe.h"
#include "system/codein_param.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/net_define.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/union_view.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "text/system/0418.h"
#include "field/beacon_view.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
extern const GameProcFunctions data_ov129_021da62c; // codein.c


// The Pal Pad. The ROM has no name for this file; "wifi_note.c" is a guess from pokeplatinum, whose Pal Pad has the
// same five screens: the menu, the friend roster, the name and code entry, the player's own code and a friend's
// record. Our names

// The archive of the screen's graphics
#define ARCID_WIFI_NOTE 63 // Black's

// The screens, the mode of the work
enum {
    WIFI_NOTE_MODE_SELECT,
    WIFI_NOTE_MODE_FRIEND_LIST,
    WIFI_NOTE_MODE_CODE_IN,
    WIFI_NOTE_MODE_MY_CODE,
    WIFI_NOTE_MODE_FRIEND_INFO,
    // Leaves the Pal Pad for the bag, or for the field
    WIFI_NOTE_MODE_RETURN,
    WIFI_NOTE_MODE_EXIT,
};

// The sequences of the menu that the other screens ask for
enum {
    MODE_SELECT_SEQ_INIT,
    MODE_SELECT_SEQ_FADE_IN,
    MODE_SELECT_SEQ_INIT_NO_FADE,
    MODE_SELECT_SEQ_MAIN,
    MODE_SELECT_SEQ_BUTTON,
    MODE_SELECT_SEQ_FADE_OUT,
    MODE_SELECT_SEQ_FADE_OUT_WAIT,
    // Back from the code entry with a name and a code to register
    MODE_SELECT_SEQ_CODE_INIT,
    MODE_SELECT_SEQ_CODE_FADE_IN,
    MODE_SELECT_SEQ_CODE_MESSAGE,
    MODE_SELECT_SEQ_CODE_YES_NO,
    MODE_SELECT_SEQ_MESSAGE,
};

// The menu's cursor: its three buttons, then the return and X buttons
enum {
    MODE_SELECT_FRIEND_LIST,
    MODE_SELECT_CODE_IN,
    MODE_SELECT_MY_CODE,
    MODE_SELECT_RETURN,
    MODE_SELECT_EXIT,
};

enum {
    FRIEND_LIST_SEQ_INIT,
    FRIEND_LIST_SEQ_INIT_KEEP_POS,
    FRIEND_LIST_SEQ_MAIN,
    FRIEND_LIST_SEQ_SCROLL_START,
    FRIEND_LIST_SEQ_SCROLL,
    FRIEND_LIST_SEQ_SCROLL_END,
    FRIEND_LIST_SEQ_BUTTON,
    FRIEND_LIST_SEQ_MENU_MESSAGE,
    FRIEND_LIST_SEQ_MENU,
    FRIEND_LIST_SEQ_INFO,
    FRIEND_LIST_SEQ_INFO_WAIT,
    // Back from a friend's record
    FRIEND_LIST_SEQ_INFO_RETURN,
    FRIEND_LIST_SEQ_NAME,
    FRIEND_LIST_SEQ_NAME_WAIT,
    // Back from the name entry, with a new name or without
    FRIEND_LIST_SEQ_NAME_RETURN,
    FRIEND_LIST_SEQ_NAME_CANCEL,
    FRIEND_LIST_SEQ_FADE_IN,
    FRIEND_LIST_SEQ_DELETE_MESSAGE,
    FRIEND_LIST_SEQ_DELETE_MESSAGE_WAIT,
    FRIEND_LIST_SEQ_DELETE_YES_NO,
    FRIEND_LIST_SEQ_DELETE_START,
    FRIEND_LIST_SEQ_DELETE,
    FRIEND_LIST_SEQ_DELETE_END,
    FRIEND_LIST_SEQ_CODE,
    FRIEND_LIST_SEQ_CODE_WAIT,
    FRIEND_LIST_SEQ_RETURN,
    FRIEND_LIST_SEQ_EXIT,
    FRIEND_LIST_SEQ_FADE_OUT,
    FRIEND_LIST_SEQ_FADE_OUT_WAIT,
};

// What the player did in the friend roster
enum {
    FRIEND_LIST_INPUT_NONE,
    FRIEND_LIST_INPUT_PREV_PAGE,
    FRIEND_LIST_INPUT_NEXT_PAGE,
    FRIEND_LIST_INPUT_RETURN,
    FRIEND_LIST_INPUT_EXIT,
    FRIEND_LIST_INPUT_DECIDE,
    FRIEND_LIST_INPUT_X,
};

#define FRIEND_LIST_PAGE_COUNT 4
#define FRIEND_LIST_PAGE_SIZE 8
// The cursor's positions after a page's friends
#define FRIEND_LIST_POS_RETURN 8
#define FRIEND_LIST_POS_EXIT 9
// The roster draws the page shown and, while it scrolls, the one that comes in from a side
#define FRIEND_LIST_AREA_COUNT 3
#define FRIEND_LIST_SCROLL_FRAMES 24

// Which page the roster scrolls to: the one before comes in from the left, in the third area
enum {
    FRIEND_LIST_SCROLL_PREV = 2,
    FRIEND_LIST_SCROLL_NEXT,
};

enum {
    CODE_IN_SEQ_NEW_INIT,
    CODE_IN_SEQ_NEW_NAME,
    CODE_IN_SEQ_NEW_CODE,
    CODE_IN_SEQ_NEW_END,
    // Only the name, of a friend of the roster
    CODE_IN_SEQ_NAME_INIT,
    CODE_IN_SEQ_NAME,
    CODE_IN_SEQ_NAME_END,
};

enum {
    MY_CODE_SEQ_INIT,
    MY_CODE_SEQ_MAIN,
    MY_CODE_SEQ_RETURN,
    MY_CODE_SEQ_EXIT,
    MY_CODE_SEQ_FADE_OUT,
    MY_CODE_SEQ_FADE_OUT_WAIT,
};

enum {
    FRIEND_INFO_SEQ_INIT,
    // The next friend's record, after one was shown
    FRIEND_INFO_SEQ_INIT_NEXT,
    FRIEND_INFO_SEQ_MAIN,
    FRIEND_INFO_SEQ_SCROLL_START,
    FRIEND_INFO_SEQ_SCROLL,
    FRIEND_INFO_SEQ_SCROLL_END,
    FRIEND_INFO_SEQ_CHANGE,
    FRIEND_INFO_SEQ_RETURN,
    FRIEND_INFO_SEQ_EXIT,
    FRIEND_INFO_SEQ_FADE_OUT,
    FRIEND_INFO_SEQ_FADE_OUT_WAIT,
};

// What the player did in a friend's record
enum {
    FRIEND_INFO_INPUT_RETURN,
    FRIEND_INFO_INPUT_EXIT,
    FRIEND_INFO_INPUT_PREV,
    FRIEND_INFO_INPUT_NEXT,
};

#define FRIEND_INFO_AREA_COUNT 3
#define FRIEND_INFO_SCREEN_COUNT 4
#define FRIEND_INFO_WINDOW_COUNT 10
#define FRIEND_INFO_SCROLL_FRAMES 8

// The keys that take the cursor back from the touch screen
#define WIFI_NOTE_KEYS (PAD_BUTTON_A | PAD_BUTTON_B | PAD_PLUS_KEY_MASK | PAD_BUTTON_X | PAD_BUTTON_Y)

#define WIFI_NOTE_PRINT_WINDOW_MAX 64
#define WIFI_NOTE_COLOR_WHITE 0x3c40
#define WIFI_NOTE_COLOR_BLACK 0x440
#define WIFI_NOTE_MENU_COLOR 0x39e3

// The friends of the Wi-Fi list that the roster shows
typedef struct {
    u32 count;
    // How many of them the player has met, and so have a record
    u32 metCount;
    // Their indices in the Wi-Fi list
    u8 index[WIFI_LIST_FRIEND_MAX];
    // Their numbers among the friends met, 0xff for the others
    u8 number[WIFI_LIST_FRIEND_MAX];
} WifiNoteFriends;

// What the screens share
typedef struct {
    // Whether the player is using the touch screen
    BOOL touch;
    GameData *gameData;
    u8 seq;
    // The screen and the sequence to go to when the current screen ends
    u8 nextMode;
    u8 nextSeq;
    // The friend picked in the roster
    s8 friendPos;
    u8 textSpeed;
    u8 frameType;
    // Whether L and R turn the roster's pages
    u8 lrEnabled;
    WifiNoteFriends friends;
    // The name and the Friend Code entered
    StrBuf *name;
    StrBuf *code;
    // The name that the name entry starts with
    const u16 *oldName;
    BOOL hasOldName;
} WifiNoteData;

// A screen of the archive
typedef struct {
    void *buffer[2];
    NNSG2dScreenData *screen[2];
} WifiNoteScreens;

// Black 2's; twinned code confirms the offsets of wordSet, msgData, arc, title, titleStr, titleTemp, str, temp,
// tcbManager, stream, streamPaused, queue, windows, unit, scrollUnit, renderer, chars, palettes, cellAnims, arrows,
// returnButton, exitButton, screens, font, heapId and blink in Black
typedef struct {
    WordSet *wordSet;
    MsgData *msgData;
    ArcTool *arc;
    PrintWindow title;
    StrBuf *titleStr;
    StrBuf *titleTemp;
    StrBuf *str;
    StrBuf *temp;
    u32 unk24;
    TCBExManager *tcbManager;
    PrintStream *stream;
    BOOL streamPaused;
    PrintQueue *queue;
    // The windows to send to VRAM once their text is printed
    PrintWindow *windows[WIFI_NOTE_PRINT_WINDOW_MAX];
    ClActUnit *unit;
    // The actors that scroll with the roster's pages
    ClActUnit *scrollUnit;
    ClActRenderer *renderer;
    // The resources of the screen's own archive, and the menus' common ones
    u32 chars[2];
    u32 palettes[2];
    u32 cellAnims[2];
    ClActor *arrows[2];
    ClActor *returnButton;
    ClActor *exitButton;
    WifiNoteScreens screens;
    Font *font;
    u16 heapId;
    // The cursor's and the page tabs' blinking palettes
    BlinkPalAnm *blink[2];
} WifiNoteDraw;

// Black 2's; twinned code confirms the offsets of cursor, buttonSeq, buttonCount, menuWindow, screenBuffer, screen,
// msgWindow, msgStr, yesNoItems, menuRes and menu in Black
typedef struct {
    u32 cursor;
    u32 unk4;
    // The flash of a picked button
    u16 buttonSeq;
    u16 buttonCount;
    PrintWindow menuWindow;
    void *screenBuffer;
    NNSG2dScreenData *screen;
    PrintWindow msgWindow;
    u32 unk24[2];
    StrBuf *msgStr;
    AppTaskMenuItem yesNoItems[2];
    AppTaskMenuRes *menuRes;
    AppTaskMenu *menu;
} WifiNoteModeSelect;

// A part of BG 2 in tiles, negative when it is to the left of the screen
typedef struct {
    s16 x;
    s16 y;
    u8 width;
    u8 height;
} WifiNoteScreenArea;

// A page of the roster
typedef struct {
    WifiNoteScreenArea area;
    PrintWindow *nameWindow;
    // The friends' trainers, or the empty frames of the friends not met and of the free places
    WF_2DCWK *chars[FRIEND_LIST_PAGE_SIZE];
    ClActor *frames[FRIEND_LIST_PAGE_SIZE];
} WifiNoteListArea;

// Black 2's; twinned code confirms the offsets of page, prevPage, pos, prevPos, buttonSeq, buttonCount, areas,
// nameWindows, scrollCount, scrollDir, charSys, msgWindow, msgStr, menuItems, yesNoItems, menuRes, menu, menuType,
// wait, eraser, pageCursor and eraserFrame in Black
typedef struct {
    s16 page;
    s16 prevPage;
    s16 pos;
    s16 prevPos;
    u16 buttonSeq;
    u16 buttonCount;
    WifiNoteListArea areas[FRIEND_LIST_AREA_COUNT];
    PrintWindow nameWindows[2];
    s16 scrollCount;
    // FRIEND_LIST_SCROLL_*
    u16 scrollDir;
    WF_2DCSYS *charSys;
    PrintWindow msgWindow;
    StrBuf *msgStr;
    u32 unk114;
    // The menus of a friend met, and of one only registered by code
    AppTaskMenuItem menuItems[2][4];
    AppTaskMenuItem yesNoItems[2];
    AppTaskMenuRes *menuRes;
    AppTaskMenu *menu;
    u32 menuType;
    s32 wait;
    ClActor *eraser;
    ClActor *pageCursor;
    fx32 eraserFrame;
} WifiNoteFriendList;

// Black 2's, not checked against Black
typedef struct {
    GameProcManager *procManager;
    NameEntryParam *nameParam;
    CodeInParam *codeParam;
} WifiNoteCodeIn;

// Black 2's; twinned code confirms the offsets of msgWindow, codeWindow, screenBuffer and screen in Black
typedef struct {
    PrintWindow msgWindow;
    PrintWindow codeWindow;
    void *screenBuffer;
    NNSG2dScreenData *screen;
    u32 unk18;
} WifiNoteMyCode;

// A window's place in tiles
// Black 2's, not checked against Black
typedef struct {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} WifiNoteWindowRect;

// A page of a friend's record
typedef struct {
    WifiNoteScreenArea area;
    u32 charOffset;
    PrintWindow *windows[8];
    u32 windowCounts[8];
} WifiNoteInfoArea;

typedef struct {
    void *buffer;
    NNSG2dScreenData *screen;
} WifiNoteInfoScreen;

typedef struct {
    s16 page;
    s32 scrollCount;
    // 0 for the friend before, 1 for the one after
    u32 scrollDir;
    u16 wait;
    // Whether a record was shown before this one
    u16 moved;
    u32 unk10;
    WifiNoteInfoArea areas[FRIEND_INFO_AREA_COUNT];
    WifiNoteInfoScreen screens[FRIEND_INFO_SCREEN_COUNT];
    PrintWindow areaWindows[2];
    PrintWindow windows[FRIEND_INFO_WINDOW_COUNT];
    ClActor *trainer;
    u32 trainerChars;
    u32 trainerPalette;
    u32 trainerCellAnims;
} WifiNoteFriendInfo;

typedef struct {
    u32 mode;
    WifiNoteDraw draw;
    WifiNoteData data;
    WifiNoteParam *param;
    WifiNoteModeSelect modeSelect;
    WifiNoteFriendList friendList;
    WifiNoteCodeIn codeIn;
    WifiNoteMyCode myCode;
    WifiNoteFriendInfo friendInfo;
    TCB *vblankTask;
    s32 wait;
} WifiNoteWork;

// An item of the roster's menu, and the sequence it goes to
// Black 2's, not checked against Black
typedef struct {
    u32 msg;
    u32 seq;
} WifiNoteListMenuItem;

// Where a friend of a page is, in tiles from the page's corner
// Black 2's, not checked against Black
typedef struct {
    s16 x;
    s16 y;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    u16 unk8;
} WifiNoteListPos;

typedef void (*WifiNoteScrollFunc)(WifiNoteFriendList *list, WifiNoteDraw *draw, s16 page, s16 prevPage);

static BOOL func_ov128_021c6180(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov128_021c6208(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov128_021c6304(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov128_021c6354(TCB *tcb, void *data);
static void func_ov128_021c6360(WifiNoteWork *wk, HeapID heapId);
static void func_ov128_021c6390(WifiNoteWork *wk);
static void func_ov128_021c63ac(WifiNoteDraw *draw, PrintWindow *window, u8 bg, u8 x, u8 y, u8 width, u8 height,
                                  u8 palette, u32 charOffset, u8 color);
static void func_ov128_021c63ec(WifiNoteData *data, WifiNoteParam *param, HeapID heapId);
static void func_ov128_021c6420(WifiNoteData *data, WifiNoteParam *param);
static void func_ov128_021c6438(WifiNoteData *data, u8 mode, u8 seq);
static void func_ov128_021c6440(WifiNoteData *data, const StrBuf *name, const StrBuf *code);
static u32 func_ov128_021c6460(WifiNoteData *data, StrBuf *code, StrBuf *name);
static void func_ov128_021c6514(WifiNoteData *data, const u16 *name);
static void func_ov128_021c651c(WifiNoteData *data, s32 pos);
static void func_ov128_021c6520(WifiNoteData *data);
static void func_ov128_021c658c(WifiNoteDraw *draw, WifiNoteData *data, HeapID heapId);
static void func_ov128_021c6638(WifiNoteDraw *draw);
static void func_ov128_021c665c(WifiNoteDraw *draw);
static void func_ov128_021c6664(WifiNoteDraw *draw);
static void func_ov128_021c6698(void);
static void func_ov128_021c66a8(WifiNoteDraw *draw, HeapID heapId);
static void func_ov128_021c6790(WifiNoteDraw *draw);
static void func_ov128_021c67a8(WifiNoteDraw *draw, HeapID heapId);
static void func_ov128_021c68c4(WifiNoteDraw *draw);
static void func_ov128_021c692c(WifiNoteDraw *draw, HeapID heapId);
static void func_ov128_021c6980(WifiNoteDraw *draw);
static void func_ov128_021c69c4(WifiNoteDraw *draw, const WifiNoteData *data, HeapID heapId);
static void func_ov128_021c6a5c(WifiNoteDraw *draw);
static void func_ov128_021c6a84(WifiNoteDraw *draw, HeapID heapId);
static void func_ov128_021c6ac0(WifiNoteDraw *draw);
static void func_ov128_021c6adc(WifiNoteDraw *draw, PrintWindow *window, u32 msg);
static void func_ov128_021c6b38(WifiNoteDraw *draw);
static void func_ov128_021c6b50(WifiNoteDraw *draw, u64 code);
static void func_ov128_021c6bcc(WifiNoteDraw *draw, WifiNoteData *data, u32 index, HeapID heapId);
static BOOL func_ov128_021c6c20(WifiNoteDraw *draw, WifiNoteData *data, u32 index);
static void func_ov128_021c6c94(WifiNoteDraw *draw, u32 number);
static void func_ov128_021c6cb0(WifiNoteDraw *draw, HeapID heapId);
static void func_ov128_021c6dac(WifiNoteDraw *draw);
static void func_ov128_021c6dd8(WifiNoteDraw *draw, BOOL visible);
static void func_ov128_021c6df8(WifiNoteDraw *draw, BOOL animating);
static void func_ov128_021c6e18(WifiNoteDraw *draw, u32 arrow, u16 sequence);
static AppTaskMenu *func_ov128_021c6e3c(AppTaskMenuItem *items, AppTaskMenuRes **res, BOOL low, HeapID heapId);
static s32 func_ov128_021c6e74(AppTaskMenu *menu);
static void func_ov128_021c6ea0(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId);
static BOOL func_ov128_021c6ec0(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId);
static void func_ov128_021c7138(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw);
static void func_ov128_021c7140(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw,
                                        HeapID heapId);
static void func_ov128_021c72a0(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw);
static void func_ov128_021c72e4(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw);
static BOOL func_ov128_021c734c(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw);
static s32 func_ov128_021c7460(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw);
static BOOL func_ov128_021c74c4(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw);
static void func_ov128_021c74e8(WifiNoteModeSelect *select, WifiNoteData *data);
static void func_ov128_021c7500(WifiNoteModeSelect *select, WifiNoteDraw *draw);
static void func_ov128_021c7558(WifiNoteModeSelect *select, WifiNoteDraw *draw, u32 msg, HeapID heapId);
static BOOL func_ov128_021c7600(PrintStream *stream, BOOL *paused);
static BOOL func_ov128_021c7674(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw);
static void func_ov128_021c7694(WifiNoteModeSelect *select, WifiNoteDraw *draw);
static BOOL func_ov128_021c76bc(WifiNoteModeSelect *select, WifiNoteData *data);
static void func_ov128_021c76f0(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId);
static BOOL func_ov128_021c76f8(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId);
static void func_ov128_021c7ac4(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw);
static void func_ov128_021c7acc(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw,
                                        HeapID heapId);
static void func_ov128_021c7d48(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw);
static void func_ov128_021c7e0c(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId);
static void func_ov128_021c7e48(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw);
static u32 func_ov128_021c7e78(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw, u32 input);
static u32 func_ov128_021c7f14(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw);
static u32 func_ov128_021c8100(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw, BOOL *touched);
static u32 func_ov128_021c81c8(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw);
static void func_ov128_021c81fc(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw,
                                           HeapID heapId);
static BOOL func_ov128_021c8240(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId);
static BOOL func_ov128_021c8354(WifiNoteFriendList *list, WifiNoteDraw *draw);
static u64 func_ov128_021c8388(WifiNoteFriendList *list, WifiNoteData *data);
static void func_ov128_021c83b0(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw,
                                                HeapID heapId);
static void func_ov128_021c83dc(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw,
                                        HeapID heapId);
static u32 func_ov128_021c8470(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw);
static BOOL func_ov128_021c84e8(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw,
                                                HeapID heapId);
static void func_ov128_021c8548(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw,
                                                  HeapID heapId);
static BOOL func_ov128_021c8574(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw,
                                         HeapID heapId);
static void func_ov128_021c85a4(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw, s32 page,
                                        s32 pos, u32 area, HeapID heapId);
static void func_ov128_021c860c(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw, s32 page,
                                       s32 pos, HeapID heapId);
static void func_ov128_021c8660(WifiNoteFriendList *list, WifiNoteDraw *draw, s16 page, s16 prevPage);
static void func_ov128_021c8664(WifiNoteFriendList *list, WifiNoteDraw *draw, s16 page, s16 prevPage);
static void func_ov128_021c8668(WifiNoteFriendList *list, WifiNoteData *data, u32 dir);
static void func_ov128_021c867c(WifiNoteFriendList *list, WifiNoteDraw *draw, u32 msg, HeapID heapId);
static BOOL func_ov128_021c873c(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw);
static void func_ov128_021c875c(WifiNoteFriendList *list, WifiNoteDraw *draw);
static void func_ov128_021c8784(WifiNoteFriendList *list);
static s32 func_ov128_021c87a0(WifiNoteFriendList *list);
static void func_ov128_021c87b0(WifiNoteFriendList *list, WifiNoteDraw *draw);
static BOOL func_ov128_021c8850(WifiNoteFriendList *list, WifiNoteDraw *draw);
static void func_ov128_021c8880(WifiNoteFriendList *list, WifiNoteDraw *draw);
static void func_ov128_021c88ac(WifiNoteListArea *area, WifiNoteDraw *draw, const WifiNoteScreenArea *screenArea,
                                  u32 charOffset, HeapID heapId);
static void func_ov128_021c88bc(WifiNoteListArea *area, WifiNoteDraw *draw);
static void func_ov128_021c88c0(WifiNoteListArea *area, WifiNoteDraw *draw, WifiNoteScreens *screens);
static void func_ov128_021c8904(WifiNoteListArea *area, WifiNoteData *data, WifiNoteDraw *draw,
                                         WF_2DCSYS *charSys, WifiNoteScreens *screens, WifiNoteFriends *friends,
                                         s32 page, HeapID heapId);
static void func_ov128_021c899c(WifiNoteListArea *area, WifiNoteData *data, WifiNoteDraw *draw, WF_2DCSYS *charSys,
                                  WifiNoteScreens *screens, WifiNoteFriends *friends, s32 page, HeapID heapId);
static void func_ov128_021c89c4(WifiNoteListArea *area, WifiNoteDraw *draw, WifiNoteScreens *screens);
static void func_ov128_021c8a50(WifiNoteListArea *area, WifiNoteDraw *draw);
static void func_ov128_021c8a80(WifiNoteListArea *area, WF_2DCSYS *charSys, WifiNoteDraw *draw,
                                        WifiNoteScreens *screens, s32 friendPos, WifiList *wifiList, u32 index,
                                        HeapID heapId);
static void func_ov128_021c8b90(WifiNoteListArea *area, WifiNoteDraw *draw, WifiNoteScreens *screens, s32 pos,
                                       HeapID heapId);
static BOOL func_ov128_021c8c04(WifiNoteCodeIn *codeIn, WifiNoteWork *wk, WifiNoteData *data, WifiNoteDraw *draw,
                                HeapID heapId);
static NameEntryParam *func_ov128_021c8da0(WifiNoteCodeIn *codeIn, WifiNoteData *data, BOOL isNew,
                                                      HeapID heapId);
static void func_ov128_021c8e24(WifiNoteMyCode *myCode, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId);
static BOOL func_ov128_021c8e2c(WifiNoteMyCode *myCode, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId);
static void func_ov128_021c8f24(WifiNoteMyCode *myCode, WifiNoteData *data, WifiNoteDraw *draw);
static s32 func_ov128_021c8f2c(WifiNoteMyCode *myCode, WifiNoteData *data);
static void func_ov128_021c8f88(WifiNoteMyCode *myCode, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId);
static void func_ov128_021c9154(WifiNoteMyCode *myCode, WifiNoteData *data, WifiNoteDraw *draw);
static void func_ov128_021c9180(WifiNoteMyCode *myCode, WifiNoteData *data, WifiNoteDraw *draw);
static void func_ov128_021c91cc(WifiNoteMyCode *myCode, WifiNoteDraw *draw);
static void func_ov128_021c9238(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId);
static BOOL func_ov128_021c9244(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId);
static u32 func_ov128_021c93c0(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw, u32 input);
static u32 func_ov128_021c9450(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw);
static u32 func_ov128_021c94dc(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw, BOOL *touched);
static u32 func_ov128_021c9544(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw);
static void func_ov128_021c958c(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw,
                                           HeapID heapId);
static BOOL func_ov128_021c95cc(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId);
static void func_ov128_021c9628(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw,
                                        HeapID heapId);
static void func_ov128_021c9730(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw);
static void func_ov128_021c97a0(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId);
static void func_ov128_021c987c(WifiNoteFriendInfo *info, WifiNoteDraw *draw);
static void func_ov128_021c98ec(WifiNoteFriendInfo *info, WifiNoteDraw *draw);
static void func_ov128_021c991c(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId);
static void func_ov128_021c9bc4(WifiNoteFriendInfo *info);
static void func_ov128_021c9bec(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw, u32 area,
                                       s32 page, HeapID heapId);
static void func_ov128_021c9bf8(WifiNoteFriendInfo *info, WifiNoteDraw *draw, u32 tab);
static void func_ov128_021c9ccc(WifiNoteFriendInfo *info, WifiNoteDraw *draw);
static BOOL func_ov128_021c9d0c(WifiNoteData *data, u32 dir);
static BOOL func_ov128_021c9d88(WifiNoteData *data, u32 dir);
static void func_ov128_021c9e10(WifiNoteInfoScreen *screens, WifiNoteDraw *draw, HeapID heapId);
static void func_ov128_021c9e54(WifiNoteInfoScreen *screens);
static void func_ov128_021c9e6c(WifiNoteFriendInfo *info, WifiNoteDraw *draw);
static void func_ov128_021c9eb4(WifiNoteFriendInfo *info, WifiNoteDraw *draw);
static void func_ov128_021c9ed0(WifiNoteInfoArea *area, WifiNoteDraw *draw, const WifiNoteScreenArea *screenArea,
                                  u32 charOffset, HeapID heapId);
static void func_ov128_021c9ef0(WifiNoteInfoArea *area);
static void func_ov128_021c9ef4(WifiNoteInfoArea *area, WifiNoteDraw *draw, u32 index, u32 count,
                                        const WifiNoteWindowRect *rects, HeapID heapId);
static void func_ov128_021c9efc(WifiNoteInfoArea *area, u32 index);
static void func_ov128_021c9f30(WifiNoteInfoArea *area, u32 index);
static void func_ov128_021c9f6c(WifiNoteInfoArea *area, WifiNoteDraw *draw);
static void func_ov128_021c9f8c(WifiNoteDraw *draw, const WifiNoteScreenArea *area);
static void func_ov128_021c9fc8(PrintWindow *window, WifiNoteData *data, WifiNoteDraw *draw, u32 msg, u32 x, u32 y,
                                     u32 color);
static u64 func_ov128_021ca014(const StrBuf *str, BOOL *valid);
static BOOL func_ov128_021ca084(WifiNoteData *data);
static void func_ov128_021ca0bc(WifiNoteDraw *draw, HeapID heapId);
static void func_ov128_021ca10c(WifiNoteDraw *draw);
static void func_ov128_021ca124(u32 button, u32 palette);
static void func_ov128_021ca154(WifiNoteDraw *draw, u32 button);
static void func_ov128_021ca16c(WifiNoteDraw *draw, u32 button);
static void func_ov128_021ca184(WifiNoteDraw *draw, u32 button, u32 prevButton);
static void func_ov128_021ca19c(WifiNoteModeSelect *select);
static BOOL func_ov128_021ca1e0(WifiNoteModeSelect *select);
static void func_ov128_021ca25c(u32 pos, u32 palette);
static void func_ov128_021ca294(WifiNoteDraw *draw, u32 pos);
static void func_ov128_021ca2ac(WifiNoteDraw *draw, u32 pos);
static void func_ov128_021ca2c4(WifiNoteDraw *draw, u32 pos, u32 prevPos);
static void func_ov128_021ca2dc(WifiNoteFriendList *list);
static BOOL func_ov128_021ca320(WifiNoteFriendList *list);
static void func_ov128_021ca3a0(WifiNoteDraw *draw);
static void func_ov128_021ca3c4(WifiNoteDraw *draw);
static void func_ov128_021ca3e8(BOOL normal);
static void func_ov128_021ca40c(WifiNoteFriendList *list, s32 page);
static void func_ov128_021ca42c(WifiNoteDraw *draw);
static void func_ov128_021ca444(WifiNoteDraw *draw);
static void func_ov128_021ca458(WifiNoteDraw *draw, PrintWindow *window);
static void func_ov128_021ca478(WifiNoteDraw *draw);
static void func_ov128_021ca4b4(BmpWin *window);
static void func_ov128_021ca4c8(WifiNoteFriendInfo *info);

// The most digits that func_ov128_021ca014 reads. The compiler folds it into the code, but only a table is
// still in the ROM's data, and only a table's element is compared as an int
static const u8 sWifiNoteFriendKeyDigitsMax[1] = { 18 };

// pokebw2's sWifiNoteVRAMConfig
static const BGSysVRAMConfig data_ov128_021ca5cc = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,        GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// The NSCRs of the roster's page and of a record's tabs
// pokebw2's sWifiNoteScreenFiles
static const u8 data_ov128_021ca4f5[2] = { 8, 9 };

// The row of a record's tabs in their screen
// pokebw2's sWifiNoteFriendInfoTabSrcY
static const u8 data_ov128_021ca4f7[2] = { 24, 16 };

// The messages of func_ov128_021c6460's results
// pokebw2's sWifiNoteModeSelectAddFriendMsgs
static const u8 data_ov128_021ca4f9[3] = {
    0,
    Bank0418_Text_056,
    Bank0418_Text_059,
};

// The windows of a record's eight parts, which nothing reads. The compiler sorts a file's data by size, and not
// stably: this order of the definitions, and the places of the VRAM config and the surface setup, are what give the
// ROM's order
// pokebw2's sWifiNoteFriendInfoRects6
static const WifiNoteWindowRect data_ov128_021ca510[1] = { { 2, 5, 20, 11 } };
// pokebw2's sWifiNoteFriendInfoRects1
static const WifiNoteWindowRect data_ov128_021ca514[1] = { { 2, 5, 22, 11 } };
// pokebw2's sWifiNoteFriendInfoRects5
static const WifiNoteWindowRect data_ov128_021ca50c[1] = { { 2, 5, 20, 11 } };
// pokebw2's sWifiNoteFriendInfoRects0
static const WifiNoteWindowRect data_ov128_021ca504[1] = { { 2, 5, 12, 2 } };
// pokebw2's sWifiNoteFriendInfoRects3
static const WifiNoteWindowRect data_ov128_021ca4fc[1] = { { 2, 1, 14, 2 } };
// pokebw2's sWifiNoteFriendInfoRects2
static const WifiNoteWindowRect data_ov128_021ca500[1] = { { 5, 5, 9, 2 } };
// pokebw2's sWifiNoteFriendInfoRects7
static const WifiNoteWindowRect data_ov128_021ca518[1] = { { 1, 5, 27, 2 } };
// pokebw2's sWifiNoteFriendInfoRects4
static const WifiNoteWindowRect data_ov128_021ca508[1] = { { 2, 5, 26, 11 } };

// The screens of the menu's buttons, then of its return and X buttons
// pokebw2's sWifiNoteModeSelectModes
static const u8 data_ov128_021ca51c[5] = {
    WIFI_NOTE_MODE_FRIEND_LIST, WIFI_NOTE_MODE_CODE_IN, WIFI_NOTE_MODE_MY_CODE,
    WIFI_NOTE_MODE_RETURN,      WIFI_NOTE_MODE_EXIT,
};

// pokebw2's sWifiNoteFriendInfoWindowCounts
static const u8 data_ov128_021ca521[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };

// The return and X buttons
// pokebw2's sWifiNoteMyCodeTouchRects
static const TouchRect data_ov128_021ca529[] = {
    { 168, 191, 232, 255 },
    { 168, 191, 200, 223 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

// pokebw2's sWifiNoteFriendInfoCharOffsets
static const u32 data_ov128_021ca538[FRIEND_INFO_AREA_COUNT] = { 1, 501, 501 };

// pokebw2's sWifiNoteFriendInfoTabX
static const u8 data_ov128_021ca544[2][8] = {
#include "asset/wifi_note_data_ov128_021ca544.inc"
};

// pokebw2's sWifiNoteFriendInfoTabWidth
static const u8 data_ov128_021ca554[2][8] = {
#include "asset/wifi_note_data_ov128_021ca554.inc"
};

// The page shown, and the pages to its right and to its left
// pokebw2's sWifiNoteScreenAreas
static const WifiNoteScreenArea data_ov128_021ca564[3] = {
#include "asset/wifi_note_data_ov128_021ca564.inc"
};

// The up and down arrows, then the return and X buttons
// pokebw2's sWifiNoteFriendInfoTouchRects
static const TouchRect data_ov128_021ca576[] = {
#include "asset/wifi_note_data_ov128_021ca576.inc"
};

// pokebw2's sWifiNoteSurfaceSetup
static ClActSurfaceSetup data_ov128_021ca7d0 = { 0, 0, 256, 192, 0, 0 };

// pokebw2's sWifiNoteModeSelectTouchRects
static const TouchRect data_ov128_021ca58a[] = {
#include "asset/wifi_note_data_ov128_021ca58a.inc"
};

// pokebw2's sWifiNoteFriendInfoWindowRects
static const WifiNoteWindowRect data_ov128_021ca5a2[FRIEND_INFO_WINDOW_COUNT] = {
#include "asset/wifi_note_data_ov128_021ca5a2.inc"
};

// A page's eight friends, the return and X buttons, and the four page tabs
// pokebw2's sWifiNoteFriendListTouchRects
static const TouchRect data_ov128_021ca5fc[] = {
#include "asset/wifi_note_data_ov128_021ca5fc.inc"
};

// pokebw2's WIFI_NOTE_PROC_FUNCTIONS
// pokebw2's WifiNote_ProcExit
// pokebw2's WifiNote_ProcInit
// pokebw2's WifiNote_ProcMain
const GameProcFunctions data_ov128_021ca658 = { func_ov128_021c6180, func_ov128_021c6208, func_ov128_021c6304 };

// pokebw2's sWifiNoteFriendListMenu
static const WifiNoteListMenuItem data_ov128_021ca688[2][4] = {
    {
        { Bank0418_Text_008, FRIEND_LIST_SEQ_INFO },
        { Bank0418_Text_009, FRIEND_LIST_SEQ_NAME },
        { Bank0418_Text_010, FRIEND_LIST_SEQ_DELETE_MESSAGE },
        { Bank0418_Text_011, FRIEND_LIST_SEQ_MAIN },
    },
    {
        { Bank0418_Text_061, FRIEND_LIST_SEQ_CODE },
        { Bank0418_Text_009, FRIEND_LIST_SEQ_NAME },
        { Bank0418_Text_010, FRIEND_LIST_SEQ_DELETE_MESSAGE },
        { Bank0418_Text_011, FRIEND_LIST_SEQ_MAIN },
    },
};

// pokebw2's sWifiNoteFriendListPos
static const WifiNoteListPos data_ov128_021ca6c8[FRIEND_LIST_PAGE_SIZE] = {
#include "asset/wifi_note_data_ov128_021ca6c8.inc"
};

// pokebw2's sWifiNoteBGSetups
static const BGSetup data_ov128_021ca718[5] = {
#include "asset/wifi_note_data_ov128_021ca718.inc"
};

// What the roster does every six frames of its scroll
// pokebw2's WifiNoteFriendList_ScrollFunc0
// pokebw2's sWifiNoteFriendListScrollFuncs
static WifiNoteScrollFunc data_ov128_021ca7c0[4] = { func_ov128_021c8660, NULL,
                                                                func_ov128_021c8664, NULL };

// The up and down arrows, then the return and X buttons, with the menus' common graphics
// pokebw2's sWifiNoteButtonSetups
static ClActorSetup data_ov128_021ca800[4] = {
    { 16, 168, 2, 8, 2 },
    { 48, 168, 3, 8, 2 },
    { 232, 168, 1, 0, 2 },
    { 200, 168, 0, 0, 2 },
};

// pokebw2's sWifiNoteFriendInfoRects
static const WifiNoteWindowRect *data_ov128_021ca7e0[8] = {
    data_ov128_021ca504, data_ov128_021ca514, data_ov128_021ca500, data_ov128_021ca4fc,
    data_ov128_021ca508, data_ov128_021ca50c, data_ov128_021ca510, data_ov128_021ca518,
};

static inline void WifiNote_HideWindow(BmpWin *window) {
    func_020456f8(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
}

static inline void WifiNote_ShowWindowNow(BmpWin *window) {
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
}

static inline void WifiNote_LoadScreen(u8 bg, const NNSG2dScreenData *screen) {
    GFL_BGSysLoadScrAreaAll(bg, screen->rawData, 0, 0, screen->screenWidth / 8, screen->screenHeight / 8);
}

static BOOL func_ov128_021c6180(GameProc *proc, u32 *state, void *param, void *work) {
    WifiNoteWork *wk;

    GFL_OvlLoad(OVERLAY_WIFICLUB);
    GFL_OvlLoad(OVERLAY_APP_UI);
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_WIFI_NOTE, 0xc0000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(WifiNoteWork), HEAPID_WIFI_NOTE);
    MI_CpuFill8(wk, 0, sizeof(WifiNoteWork));
    wk->param = param;
    wk->vblankTask = NULL;
    func_ov128_021c63ec(&wk->data, wk->param, HEAPID_WIFI_NOTE);
    func_ov128_021c658c(&wk->draw, &wk->data, HEAPID_WIFI_NOTE);
    if (wk->vblankTask == NULL) {
        wk->vblankTask = GFL_VBlankTCBAdd(func_ov128_021c6354, wk, 16);
    }
    return TRUE;
}

static BOOL func_ov128_021c6208(GameProc *proc, u32 *state, void *param, void *work) {
    WifiNoteWork *wk = work;
    BOOL done;

    switch (*state) {
    case 0:
        switch (wk->mode) {
        case WIFI_NOTE_MODE_SELECT:
            done = func_ov128_021c6ec0(&wk->modeSelect, &wk->data, &wk->draw, HEAPID_WIFI_NOTE);
            break;
        case WIFI_NOTE_MODE_FRIEND_LIST:
            done = func_ov128_021c76f8(&wk->friendList, &wk->data, &wk->draw, HEAPID_WIFI_NOTE);
            break;
        case WIFI_NOTE_MODE_CODE_IN:
            done = func_ov128_021c8c04(&wk->codeIn, wk, &wk->data, &wk->draw, HEAPID_WIFI_NOTE);
            break;
        case WIFI_NOTE_MODE_MY_CODE:
            done = func_ov128_021c8e2c(&wk->myCode, &wk->data, &wk->draw, HEAPID_WIFI_NOTE);
            break;
        case WIFI_NOTE_MODE_FRIEND_INFO:
            done = func_ov128_021c9244(&wk->friendInfo, &wk->data, &wk->draw, HEAPID_WIFI_NOTE);
            break;
        case WIFI_NOTE_MODE_RETURN:
            wk->param->result = WIFI_NOTE_RESULT_RETURN;
            return TRUE;
        case WIFI_NOTE_MODE_EXIT:
            wk->param->result = WIFI_NOTE_RESULT_EXIT;
            return TRUE;
        }
        if (done == TRUE) {
            wk->mode = wk->data.nextMode;
            wk->data.seq = wk->data.nextSeq;
            wk->wait = 4;
            *state = 1;
        }
        break;
    case 1:
        wk->wait--;
        if (wk->wait <= 0) {
            *state = 0;
        }
        break;
    }
    func_ov128_021c6638(&wk->draw);
    return FALSE;
}

static BOOL func_ov128_021c6304(GameProc *proc, u32 *state, void *param, void *work) {
    WifiNoteWork *wk = work;

    GFL_TCBRemove(wk->vblankTask);
    wk->vblankTask = NULL;
    func_ov128_021c6664(&wk->draw);
    func_ov128_021c6420(&wk->data, wk->param);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_WIFI_NOTE);
    GFL_OvlUnload(OVERLAY_APP_UI);
    GFL_OvlUnload(OVERLAY_WIFICLUB);
    return TRUE;
}

// pokebw2's WifiNote_VBlank
static void func_ov128_021c6354(TCB *tcb, void *data) {
    WifiNoteWork *wk = data;

    func_ov128_021c665c(&wk->draw);
}

// Starts the screen's graphics again after the name or code entry, and ends them before it
// pokebw2's WifiNote_InitDraw
static void func_ov128_021c6360(WifiNoteWork *wk, HeapID heapId) {
    func_ov128_021c658c(&wk->draw, &wk->data, heapId);
    if (wk->vblankTask == NULL) {
        wk->vblankTask = GFL_VBlankTCBAdd(func_ov128_021c6354, wk, 16);
    }
}

// pokebw2's WifiNote_ExitDraw
static void func_ov128_021c6390(WifiNoteWork *wk) {
    GFL_TCBRemove(wk->vblankTask);
    wk->vblankTask = NULL;
    func_ov128_021c6664(&wk->draw);
}

// charOffset is not used
static void func_ov128_021c63ac(WifiNoteDraw *draw, PrintWindow *window, u8 bg, u8 x, u8 y, u8 width, u8 height,
                                  u8 palette, u32 charOffset, u8 color) {
    window->window = BmpWin_CreateDynamic(bg, x, y, width, height, palette, 1);
    GFL_BitmapFill(BmpWin_GetBitmap(window->window), color);
    BmpWin_FlushChar(window->window);
    func_ov128_021ca458(draw, window);
}

// pokebw2's WifiNoteData_Init
static void func_ov128_021c63ec(WifiNoteData *data, WifiNoteParam *param, HeapID heapId) {
    data->gameData = param->gameData;
    data->name = GFL_StrBufCreate(128, heapId);
    data->code = GFL_StrBufCreate(128, heapId);
    data->touch = func_020355c4();
    data->textSpeed = func_02012ffc();
    data->frameType = 0;
    data->lrEnabled = TRUE;
}

// pokebw2's WifiNoteData_Exit
static void func_ov128_021c6420(WifiNoteData *data, WifiNoteParam *param) {
    func_020355d8(data->touch);
    GFL_StrBufFree(data->name);
    GFL_StrBufFree(data->code);
}

// pokebw2's WifiNoteData_SetNext
static void func_ov128_021c6438(WifiNoteData *data, u8 mode, u8 seq) {
    data->nextMode = mode;
    data->nextSeq = seq;
}

// pokebw2's WifiNoteData_SetEntered
static void func_ov128_021c6440(WifiNoteData *data, const StrBuf *name, const StrBuf *code) {
    if (name != NULL) {
        GFL_StrBufCopy(data->name, name);
    }
    if (code != NULL) {
        GFL_StrBufCopy(data->code, code);
    }
}

// Registers a friend by Friend Code in the first free place of the Wi-Fi list. Returns 0, or 1 when the code is
// wrong and 2 when it is in the list already
// pokebw2's WifiNoteData_AddFriend
static u32 func_ov128_021c6460(WifiNoteData *data, StrBuf *code, StrBuf *name) {
    int i;
    WifiList *wifiList = func_0201283c(data->gameData);
    u64 myKey = func_0205994c(func_02009860(wifiList));

    for (i = 0; i < WIFI_LIST_FRIEND_MAX; i++) {
        if (!func_02009a1c(wifiList, i)) {
            BOOL valid;
            int index;
            u64 key = func_ov128_021ca014(code, &valid);

            if (valid == TRUE && key != myKey) {
                if (!func_02059354(func_02009860(wifiList), key)) {
                    return 1;
                }
                if (!func_0200f43c(wifiList, key, &index)) {
                    return 2;
                }
                DWC_CreateFriendKeyToken(func_020099c8(wifiList, i), key);
                func_02009a00(wifiList, i, name);
                func_02009918(wifiList, i, WIFI_LIST_FRIEND_GENDER, WIFI_LIST_GENDER_UNKNOWN);
                return 0;
            }
            return 1;
        }
    }
    return 0;
}

// pokebw2's WifiNoteData_SetOldName
static void func_ov128_021c6514(WifiNoteData *data, const u16 *name) {
    data->oldName = name;
    data->hasOldName = TRUE;
}

// pokebw2's WifiNoteData_SetFriendPos
static void func_ov128_021c651c(WifiNoteData *data, s32 pos) {
    data->friendPos = pos;
}

// pokebw2's WifiNoteData_MakeFriends
static void func_ov128_021c6520(WifiNoteData *data) {
    int i;
    WifiList *wifiList;

    MI_CpuFill8(&data->friends, 0, sizeof(WifiNoteFriends));
    wifiList = func_0201283c(data->gameData);
    data->friends.count = 0;
    for (i = 0; i < WIFI_LIST_FRIEND_MAX; i++) {
        if (func_02009a1c(wifiList, i) == TRUE) {
            data->friends.index[data->friends.count] = i;
            if (func_02009864(wifiList, i, WIFI_LIST_FRIEND_GENDER) == WIFI_LIST_GENDER_UNKNOWN) {
                data->friends.number[data->friends.count] = 0xff;
            } else {
                data->friends.number[data->friends.count] = data->friends.metCount++;
            }
            data->friends.count++;
        }
    }
}

// pokebw2's WifiNoteDraw_Init
static void func_ov128_021c658c(WifiNoteDraw *draw, WifiNoteData *data, HeapID heapId) {
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    GFL_BGSysSetDisplayLayout(0);
    draw->arc = GFL_ArcSysCreateFileHandle(ARCID_WIFI_NOTE, heapId);
    draw->heapId = heapId;
    func_ov128_021c6698();
    func_ov128_021c66a8(draw, heapId);
    func_ov128_021c67a8(draw, heapId);
    func_ov128_021c692c(draw, heapId);
    func_ov128_021c69c4(draw, data, heapId);
    func_ov128_021c6cb0(draw, heapId);
    func_ov128_021c6a84(draw, heapId);
    func_ov128_021ca0bc(draw, heapId);
    func_0203d34c(TRUE, HEAPID_WIFI_NOTE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's WifiNoteDraw_Main
static void func_ov128_021c6638(WifiNoteDraw *draw) {
    if (draw->queue != NULL) {
        func_0201c4e4(draw->queue);
        func_ov128_021ca478(draw);
    }
    if (draw->tcbManager != NULL) {
        GFL_TCBExMgrUpdate(draw->tcbManager);
    }
    func_0204a600();
}

// pokebw2's WifiNoteDraw_VBlank
static void func_ov128_021c665c(WifiNoteDraw *draw) {
    func_0204a648();
}

// pokebw2's WifiNoteDraw_Exit
static void func_ov128_021c6664(WifiNoteDraw *draw) {
    func_ov128_021ca10c(draw);
    func_ov128_021c6dac(draw);
    func_ov128_021c6ac0(draw);
    func_ov128_021c6a5c(draw);
    func_ov128_021c6980(draw);
    func_ov128_021c68c4(draw);
    func_ov128_021c6790(draw);
    GFL_ArcToolFree(draw->arc);
}

// pokebw2's WifiNoteDraw_InitVRAM
static void func_ov128_021c6698(void) {
    GFL_BGSysSetVRAMBanks(&data_ov128_021ca5cc);
}

// pokebw2's WifiNoteDraw_InitBG
static void func_ov128_021c66a8(WifiNoteDraw *draw, HeapID heapId) {
    int i;

    GFL_BGSysCreate(heapId);
    {
        BGSysLCDConfig config = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

        GFL_BGSysSetLCDConfig(&config);
    }
    for (i = 0; i < 5; i++) {
        GFL_BGSysCreateBG(i, &data_ov128_021ca718[i], BGMODE_TEXT);
        GFL_BGSysClearCharCore(i, BGSYS_TILE_SIZE_16, 0, heapId);
        GFL_BGSysFillScr(i, 0);
        GFL_BGSysSetBGEnabled(i, TRUE);
    }
    GFL_G2DIOLoadArcNCLRDefault(draw->arc, 0, 0, 0, 0xc0, heapId);
    GFL_G2DIOLoadArcNCLRDefault(draw->arc, 0, 4, 0, 0xc0, heapId);
    GFL_BGSysLoadArcNCGRStatic(draw->arc, 3, 0, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(draw->arc, 3, 2, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(draw->arc, 3, 4, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(draw->arc, 5, 0, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(draw->arc, 14, 4, 0, 0, FALSE, heapId);
}

// pokebw2's WifiNoteDraw_ExitBG
static void func_ov128_021c6790(WifiNoteDraw *draw) {
    int i;

    for (i = 0; i < 5; i++) {
        GFL_BGSysReleaseBG(i);
    }
    GFL_BGSysFree();
}

// pokebw2's WifiNoteDraw_InitClAct
static void func_ov128_021c67a8(WifiNoteDraw *draw, HeapID heapId) {
    ClActSysSetup setup = data_020a1448;
    ArcTool *arc;

    setup.oamStartMain = 16;
    setup.oamCountMain = 112;
    ClActSys_Create(&setup, &data_ov128_021ca5cc, heapId);
    draw->unit = func_0204b100(32, 0, heapId);
    func_0204b270(draw->unit);
    draw->renderer = func_0204b034(&data_ov128_021ca7d0, 1, heapId);
    draw->scrollUnit = func_0204b100(18, 1, heapId);
    func_0204b258(draw->scrollUnit, draw->renderer);
    draw->chars[0] = func_0204a6c8(draw->arc, 4, FALSE, CLACT_VRAM_MAIN, heapId);
    draw->palettes[0] = func_0204abf0(draw->arc, 1, CLACT_VRAM_MAIN, 0, heapId);
    draw->cellAnims[0] = func_0204af28(draw->arc, 15, 16, heapId);
    arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), heapId);
    draw->chars[1] = func_0204a6c8(arc, func_02027618(), FALSE, CLACT_VRAM_MAIN, heapId);
    draw->palettes[1] = func_0204abf0(arc, func_02027614(), CLACT_VRAM_MAIN, 0x60, heapId);
    draw->cellAnims[1] = func_0204af28(arc, func_0202761c(2), func_02027620(2), heapId);
    GFL_ArcToolFree(arc);
}

// pokebw2's WifiNoteDraw_ExitClAct
static void func_ov128_021c68c4(WifiNoteDraw *draw) {
    u32 i;

    for (i = 0; i < 2; i++) {
        func_0204a8d4(draw->chars[i]);
    }
    for (i = 0; i < 2; i++) {
        func_0204ada4(draw->palettes[i]);
    }
    for (i = 0; i < 2; i++) {
        func_0204afd8(draw->cellAnims[i]);
    }
    func_0204b1cc(draw->scrollUnit);
    func_0204b084(draw->renderer);
    func_0204b1cc(draw->unit);
    func_0204a5ac();
}

// pokebw2's WifiNoteDraw_InitMsg
static void func_ov128_021c692c(WifiNoteDraw *draw, HeapID heapId) {
    func_0201dd68();
    draw->wordSet = GFL_WordSetSystemCreateDefault(heapId);
    draw->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_0418, heapId);
    draw->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, heapId);
    draw->tcbManager = GFL_TCBExMgrCreate(heapId, heapId, 3, 0x100);
    draw->stream = NULL;
    draw->queue = func_0201c440(heapId);
}

// pokebw2's WifiNoteDraw_ExitMsg
static void func_ov128_021c6980(WifiNoteDraw *draw) {
    func_0201c6ec(draw->queue);
    func_0201c4c0(draw->queue);
    draw->queue = NULL;
    if (draw->stream != NULL) {
        func_0201ce48(draw->stream);
        draw->stream = NULL;
    }
    GFL_TCBExMgrFree(draw->tcbManager);
    draw->tcbManager = NULL;
    GFL_FontFree(draw->font);
    GFL_MsgDataFree(draw->msgData);
    GFL_WordSetSystemFree(draw->wordSet);
}

// pokebw2's WifiNoteDraw_InitTitle
static void func_ov128_021c69c4(WifiNoteDraw *draw, const WifiNoteData *data, HeapID heapId) {
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x140, 0x20, heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 0x1c0, 0x20, heapId);
    LoadSysMsgBox(1, 70, 11, data->frameType, heapId);
    BmpWin_InitAllocator(heapId);
    func_ov128_021c63ac(draw, &draw->title, 1, 1, 0, 24, 3, 10, 100, 0);
    draw->titleStr = GFL_StrBufCreate(128, heapId);
    draw->titleTemp = GFL_StrBufCreate(128, heapId);
    draw->str = GFL_StrBufCreate(128, heapId);
    draw->temp = GFL_StrBufCreate(128, heapId);
}

// pokebw2's WifiNoteDraw_ExitTitle
static void func_ov128_021c6a5c(WifiNoteDraw *draw) {
    GFL_StrBufFree(draw->str);
    GFL_StrBufFree(draw->temp);
    GFL_StrBufFree(draw->titleStr);
    GFL_StrBufFree(draw->titleTemp);
    BmpWin_Free(draw->title.window);
    BmpWin_FreeAllocator();
}

// pokebw2's WifiNoteDraw_LoadScreens
static void func_ov128_021c6a84(WifiNoteDraw *draw, HeapID heapId) {
    int i;

    for (i = 0; i < 2; i++) {
        draw->screens.buffer[i] =
            GFL_G2DIOReadNSCRArc(draw->arc, data_ov128_021ca4f5[i], FALSE, &draw->screens.screen[i], heapId);
    }
}

// pokebw2's WifiNoteDraw_FreeScreens
static void func_ov128_021c6ac0(WifiNoteDraw *draw) {
    int i;

    for (i = 0; i < 2; i++) {
        GFL_HeapFree(draw->screens.buffer[i]);
    }
}

// pokebw2's WifiNoteDraw_PrintTitle
static void func_ov128_021c6adc(WifiNoteDraw *draw, PrintWindow *window, u32 msg) {
    GFL_BitmapFill(BmpWin_GetBitmap(window->window), 0);
    BmpWin_FlushChar(window->window);
    GFL_MsgDataLoadStrbuf(draw->msgData, msg, draw->titleTemp);
    GFL_WordSetFormatStrbuf(draw->wordSet, draw->titleStr, draw->titleTemp);
    func_ov067_021b6ae4(window, draw->queue, 0, 4, draw->titleStr, draw->font, WIFI_NOTE_COLOR_WHITE, PRINT_ALIGN_LEFT);
    func_ov128_021ca4b4(window->window);
}

// pokebw2's WifiNoteDraw_HideTitle
static void func_ov128_021c6b38(WifiNoteDraw *draw) {
    WifiNote_HideWindow(draw->title.window);
}

// Sets a Friend Code's three groups of digits as the words 1 to 3
// pokebw2's WifiNoteDraw_SetFriendCode
static void func_ov128_021c6b50(WifiNoteDraw *draw, u64 code) {
    WordSetNumber(draw->wordSet, 1, code / 100000000, 4, 2, TRUE);
    WordSetNumber(draw->wordSet, 2, (code / 10000) % 10000, 4, 2, TRUE);
    WordSetNumber(draw->wordSet, 3, code % 10000, 4, 2, TRUE);
}

// Sets a friend's name as the word 0
// pokebw2's WifiNoteDraw_SetFriendName
static void func_ov128_021c6bcc(WifiNoteDraw *draw, WifiNoteData *data, u32 index, HeapID heapId) {
    WifiList *wifiList = func_0201283c(data->gameData);
    PlayerInfo *playerInfo = func_0200846c(heapId);
    StrBuf *name = GFL_StrBufCreate(10, heapId);

    GFL_StrBufLoadString(name, func_020099d4(wifiList, index));
    copyTrainerNameFromStrbuf(playerInfo, name);
    GFL_StrBufFree(name);
    copyVarForText(draw->wordSet, 0, playerInfo);
    GFL_HeapFree(playerInfo);
}

// Sets the date the player last played with a friend, and returns whether there is one
// pokebw2's WifiNoteDraw_SetLastDate
static BOOL func_ov128_021c6c20(WifiNoteDraw *draw, WifiNoteData *data, u32 index) {
    WifiList *wifiList = func_0201283c(data->gameData);
    BOOL result = TRUE;
    u32 number;

    number = func_02009864(wifiList, index, WIFI_LIST_FRIEND_LAST_DAY);
    if (number == 0) {
        result = FALSE;
    }
    WordSetNumber(draw->wordSet, 2, number, 2, 0, TRUE);
    number = func_02009864(wifiList, index, WIFI_LIST_FRIEND_LAST_YEAR);
    WordSetNumber(draw->wordSet, 0, number, 4, 1, TRUE);
    number = func_02009864(wifiList, index, WIFI_LIST_FRIEND_LAST_MONTH);
    WordSetNumber(draw->wordSet, 1, number, 2, 0, TRUE);
    return result;
}

// pokebw2's WifiNoteDraw_SetNumber
static void func_ov128_021c6c94(WifiNoteDraw *draw, u32 number) {
    WordSetNumber(draw->wordSet, 0, number, 4, 1, TRUE);
}

// pokebw2's WifiNoteDraw_InitActors
static void func_ov128_021c6cb0(WifiNoteDraw *draw, HeapID heapId) {
    int i;

    for (i = 0; i < 2; i++) {
        draw->arrows[i] = func_0204b294(draw->unit, draw->chars[1], draw->palettes[1], draw->cellAnims[1],
                                        &data_ov128_021ca800[i], 0, heapId);
        func_0204b92c(draw->arrows[i], TRUE);
        func_0204b3dc(draw->arrows[i], FALSE);
    }
    draw->returnButton = func_0204b294(draw->unit, draw->chars[1], draw->palettes[1], draw->cellAnims[1],
                                       &data_ov128_021ca800[2], 0, heapId);
    func_0204b3dc(draw->returnButton, TRUE);
    draw->exitButton = func_0204b294(draw->unit, draw->chars[1], draw->palettes[1], draw->cellAnims[1],
                                     &data_ov128_021ca800[3], 0, heapId);
    func_0204b3dc(draw->exitButton, TRUE);
}

// pokebw2's WifiNoteDraw_ExitActors
static void func_ov128_021c6dac(WifiNoteDraw *draw) {
    int i;

    func_0204b3b4(draw->exitButton);
    func_0204b3b4(draw->returnButton);
    for (i = 0; i < 2; i++) {
        func_0204b3b4(draw->arrows[i]);
    }
}

// pokebw2's WifiNoteDraw_SetArrowsVisible
static void func_ov128_021c6dd8(WifiNoteDraw *draw, BOOL visible) {
    int i;

    for (i = 0; i < 2; i++) {
        func_0204b3dc(draw->arrows[i], visible);
    }
}

// pokebw2's WifiNoteDraw_SetArrowsAnimating
static void func_ov128_021c6df8(WifiNoteDraw *draw, BOOL animating) {
    int i;

    for (i = 0; i < 2; i++) {
        func_0204b92c(draw->arrows[i], animating);
    }
}

// pokebw2's WifiNoteDraw_StartArrowAnim
static void func_ov128_021c6e18(WifiNoteDraw *draw, u32 arrow, u16 sequence) {
    func_0204b92c(draw->arrows[arrow], TRUE);
    func_0204b878(draw->arrows[arrow], sequence);
    func_0204b9a0(draw->arrows[arrow]);
}

// A yes/no menu at the bottom right, above the message window if low
// pokebw2's WifiNote_CreateYesNoMenu
static AppTaskMenu *func_ov128_021c6e3c(AppTaskMenuItem *items, AppTaskMenuRes **res, BOOL low, HeapID heapId) {
    AppTaskMenuInit init;

    init.heapId = heapId;
    init.itemCount = 2;
    init.items = items;
    init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
    init.x = 32;
    if (low == TRUE) {
        init.y = 18;
    } else {
        init.y = 12;
    }
    init.width = 8;
    init.height = 3;
    return func_02027778(&init, *res);
}

// Returns the item picked once its flash is over, and frees the menu, or -1
// pokebw2's WifiNote_UpdateMenu
static s32 func_ov128_021c6e74(AppTaskMenu *menu) {
    func_02027974(menu);
    if (func_020279e8(menu) == TRUE) {
        s32 pos = func_02027a04(menu);

        func_02027858(menu);
        return pos;
    }
    return -1;
}

// pokebw2's WifiNoteModeSelect_Init
static void func_ov128_021c6ea0(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId) {
    data->touch = func_020355c4();
    func_ov128_021c7140(select, data, draw, heapId);
}

// pokebw2's WifiNoteModeSelect_Main
static BOOL func_ov128_021c6ec0(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId) {
    BOOL result;

    switch (data->seq) {
    case MODE_SELECT_SEQ_INIT:
        func_ov128_021c6ea0(select, data, draw, HEAPID_WIFI_NOTE);
        func_ov128_021c72e4(select, data, draw);
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, heapId);
        data->seq = MODE_SELECT_SEQ_FADE_IN;
        break;
    case MODE_SELECT_SEQ_FADE_IN:
        if (GFL_WipeIsFinished()) {
            data->seq = MODE_SELECT_SEQ_MAIN;
        }
        break;
    case MODE_SELECT_SEQ_INIT_NO_FADE:
        func_ov128_021c6ea0(select, data, draw, HEAPID_WIFI_NOTE);
        func_ov128_021c72e4(select, data, draw);
        data->seq = MODE_SELECT_SEQ_MAIN;
        break;
    case MODE_SELECT_SEQ_MAIN:
        func_0202ea40(draw->blink[0]);
        result = func_ov128_021c74c4(select, data, draw);
        if (result) {
            if (select->cursor == MODE_SELECT_RETURN) {
                func_ov128_021ca3a0(draw);
            } else if (select->cursor == MODE_SELECT_EXIT) {
                func_ov128_021ca3c4(draw);
            } else {
                select->buttonSeq = 0;
            }
            data->seq = MODE_SELECT_SEQ_BUTTON;
        }
        break;
    case MODE_SELECT_SEQ_BUTTON:
        if (select->cursor == MODE_SELECT_RETURN) {
            if (func_0204b98c(draw->returnButton) == TRUE) {
                break;
            }
        } else if (select->cursor == MODE_SELECT_EXIT) {
            if (func_0204b98c(draw->exitButton) == TRUE) {
                break;
            }
        } else if (func_ov128_021ca1e0(select) == TRUE) {
            break;
        }
        if (func_ov128_021c76bc(select, data) == TRUE) {
            func_ov128_021c74e8(select, data);
            if (select->cursor == MODE_SELECT_RETURN) {
                data->seq = MODE_SELECT_SEQ_FADE_OUT;
            } else if (select->cursor == MODE_SELECT_EXIT) {
                data->seq = MODE_SELECT_SEQ_FADE_OUT;
            } else if (select->cursor == MODE_SELECT_CODE_IN) {
                data->seq = MODE_SELECT_SEQ_FADE_OUT;
            } else {
                func_ov128_021c7500(select, draw);
                func_ov128_021c7138(select, data, draw);
                return TRUE;
            }
        } else {
            func_ov128_021c7558(select, draw, Bank0418_Text_058, heapId);
            data->seq = MODE_SELECT_SEQ_MESSAGE;
        }
        break;
    case MODE_SELECT_SEQ_FADE_OUT:
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, heapId);
        data->seq++;
        break;
    case MODE_SELECT_SEQ_FADE_OUT_WAIT:
        if (GFL_WipeIsFinished() == TRUE) {
            func_ov128_021c7138(select, data, draw);
            return TRUE;
        }
        break;
    case MODE_SELECT_SEQ_CODE_INIT:
        func_ov128_021c6ea0(select, data, draw, HEAPID_WIFI_NOTE);
        func_ov128_021c72e4(select, data, draw);
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, heapId);
        data->seq = MODE_SELECT_SEQ_CODE_FADE_IN;
        break;
    case MODE_SELECT_SEQ_CODE_FADE_IN:
        if (GFL_WipeIsFinished()) {
            PlayerInfo *playerInfo = func_0200846c(heapId);
            u64 code;

            copyTrainerNameFromStrbuf(playerInfo, data->name);
            copyVarForText(draw->wordSet, 0, playerInfo);
            GFL_HeapFree(playerInfo);
            code = func_ov128_021ca014(data->code, &result);
            if (result) {
                func_ov128_021c6b50(draw, code);
                func_ov128_021c7558(select, draw, Bank0418_Text_055, heapId);
                data->seq = MODE_SELECT_SEQ_CODE_MESSAGE;
            } else {
                func_ov128_021c7558(select, draw, Bank0418_Text_056, heapId);
                data->seq = MODE_SELECT_SEQ_MESSAGE;
            }
        }
        break;
    case MODE_SELECT_SEQ_CODE_MESSAGE:
        if (func_ov128_021c7674(select, data, draw)) {
            select->menu = func_ov128_021c6e3c(select->yesNoItems, &select->menuRes, TRUE, heapId);
            data->seq = MODE_SELECT_SEQ_CODE_YES_NO;
        }
        break;
    case MODE_SELECT_SEQ_CODE_YES_NO:
        switch (func_ov128_021c6e74(select->menu)) {
        case 0: {
            u32 error;

            data->touch = func_020355c4();
            error = func_ov128_021c6460(data, data->code, data->name);
            if (error == 0) {
                func_ov128_021c7694(select, draw);
                func_ov128_021ca19c(select);
                data->seq = MODE_SELECT_SEQ_MAIN;
            } else {
                func_ov128_021c7558(select, draw, data_ov128_021ca4f9[error], heapId);
                data->seq = MODE_SELECT_SEQ_MESSAGE;
            }
            break;
        }
        case 1:
            data->touch = func_020355c4();
            func_ov128_021c7694(select, draw);
            func_ov128_021ca19c(select);
            data->seq = MODE_SELECT_SEQ_MAIN;
            break;
        }
        break;
    case MODE_SELECT_SEQ_MESSAGE:
        if (func_ov128_021c7674(select, data, draw)) {
            if (func_ov128_021ca084(data) == TRUE) {
                func_ov128_021c7694(select, draw);
                func_ov128_021ca19c(select);
                data->seq = MODE_SELECT_SEQ_MAIN;
            }
        }
        break;
    }
    return FALSE;
}

// pokebw2's WifiNoteModeSelect_Exit
static void func_ov128_021c7138(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw) {
    func_ov128_021c72a0(select, data, draw);
}

static void func_ov128_021c7140(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw,
                                        HeapID heapId) {
    int i;
    StrBuf *str;
    StrBuf *temp;

    func_ov128_021c63ac(draw, &select->menuWindow, 3, 4, 5, 24, 20, 10, 1, 0);
    func_ov128_021c63ac(draw, &select->msgWindow, 1, 2, 19, 27, 4, 10, 172, 0);
    select->msgStr = GFL_StrBufCreate(128, heapId);
    str = GFL_StrBufCreate(128, heapId);
    temp = GFL_StrBufCreate(128, heapId);
    copyVarForText(draw->wordSet, 0, GetGameDataPlayerInfo(data->gameData));
    for (i = 0; i < 3; i++) {
        u32 width;

        GFL_MsgDataLoadStrbuf(draw->msgData, Bank0418_Text_001 + i, temp);
        GFL_WordSetFormatStrbuf(draw->wordSet, str, temp);
        width = GFL_FontGetBlockWidth(str, draw->font, 0);
        func_ov067_021b6ae4(&select->menuWindow, draw->queue, (192 - width) / 2, i * 40 + 8, str, draw->font,
                        WIFI_NOTE_COLOR_BLACK, PRINT_ALIGN_LEFT);
    }
    func_ov128_021ca4b4(select->menuWindow.window);
    GFL_StrBufFree(str);
    GFL_StrBufFree(temp);
    select->screenBuffer = GFL_G2DIOReadNSCRArc(draw->arc, 13, FALSE, &select->screen, heapId);
    select->yesNoItems[0].str = GFL_MsgDataLoadStrbufNew(draw->msgData, Bank0418_Text_066);
    select->yesNoItems[0].color = WIFI_NOTE_MENU_COLOR;
    select->yesNoItems[0].type = 0;
    select->yesNoItems[1].str = GFL_MsgDataLoadStrbufNew(draw->msgData, Bank0418_Text_067);
    select->yesNoItems[1].color = WIFI_NOTE_MENU_COLOR;
    select->yesNoItems[1].type = 0;
    select->menuRes = func_02027f60(1, 14, draw->font, draw->queue, heapId);
}

// pokebw2's WifiNoteModeSelect_ExitDraw
static void func_ov128_021c72a0(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw) {
    func_ov128_021ca444(draw);
    func_ov128_021ca458(draw, &draw->title);
    func_02027fd8(select->menuRes);
    GFL_StrBufFree(select->yesNoItems[0].str);
    GFL_StrBufFree(select->yesNoItems[1].str);
    GFL_StrBufFree(select->msgStr);
    BmpWin_Free(select->msgWindow.window);
    BmpWin_Free(select->menuWindow.window);
    GFL_HeapFree(select->screenBuffer);
}

// pokebw2's WifiNoteModeSelect_Show
static void func_ov128_021c72e4(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw) {
    func_ov128_021c6adc(draw, &draw->title, Bank0418_Text_000);
    WifiNote_LoadScreen(2, select->screen);
    GFL_BGSysMoveBG(2, 0, 0);
    GFL_BGSysMoveBG(2, 3, 0);
    if (data->touch == FALSE) {
        func_ov128_021ca16c(draw, select->cursor);
    } else {
        GFL_BGSysQueueScrLoad(2);
    }
}

// Returns whether a button was picked
// pokebw2's WifiNoteModeSelect_Keys
static BOOL func_ov128_021c734c(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw) {
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov128_021ca154(draw, select->cursor);
        func_020355d8(FALSE);
        data->touch = FALSE;
        select->cursor = MODE_SELECT_RETURN;
        return TRUE;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
        GFL_SndSEPlay(SEQ_SE_CLOSE1);
        func_ov128_021ca154(draw, select->cursor);
        func_020355d8(FALSE);
        data->touch = FALSE;
        select->cursor = MODE_SELECT_EXIT;
        return TRUE;
    }
    if (data->touch == TRUE) {
        if (GCTX_HIDGetPressedKeys() & WIFI_NOTE_KEYS) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov128_021ca184(draw, select->cursor, select->cursor);
            func_020355d8(FALSE);
            data->touch = FALSE;
        }
        return FALSE;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_0202eb04(draw->blink[0]);
        return TRUE;
    }
    if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
        if (select->cursor != 0) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov128_021ca184(draw, select->cursor - 1, select->cursor);
            select->cursor--;
            return FALSE;
        }
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
        if (select->cursor < MODE_SELECT_MY_CODE) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov128_021ca184(draw, select->cursor + 1, select->cursor);
            select->cursor++;
            return FALSE;
        }
    }
    return FALSE;
}

// pokebw2's WifiNoteModeSelect_Touch
static s32 func_ov128_021c7460(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw) {
    s32 hit = func_02035c08(data_ov128_021ca58a);

    if (hit != TOUCH_RECT_NONE) {
        if (hit == MODE_SELECT_RETURN) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov128_021ca154(draw, select->cursor);
        } else if (hit == MODE_SELECT_EXIT) {
            GFL_SndSEPlay(SEQ_SE_CLOSE1);
            func_ov128_021ca154(draw, select->cursor);
        } else {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov128_021ca184(draw, hit, select->cursor);
        }
        select->cursor = hit;
        func_020355d8(TRUE);
        data->touch = TRUE;
    }
    return hit;
}

// pokebw2's WifiNoteModeSelect_Input
static BOOL func_ov128_021c74c4(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw) {
    if (func_ov128_021c7460(select, data, draw) != TOUCH_RECT_NONE) {
        return TRUE;
    }
    return func_ov128_021c734c(select, data, draw);
}

// pokebw2's WifiNoteModeSelect_SetNext
static void func_ov128_021c74e8(WifiNoteModeSelect *select, WifiNoteData *data) {
    func_ov128_021c6438(data, data_ov128_021ca51c[select->cursor], 0);
}

// pokebw2's WifiNoteModeSelect_Hide
static void func_ov128_021c7500(WifiNoteModeSelect *select, WifiNoteDraw *draw) {
    func_ov128_021c6b38(draw);
    GFL_BGSysFillScrArea(2, 0, 0, 0, 32, 24, 16);
    GFL_BGSysQueueScrLoad(2);
    GFL_BGSysFillScrArea(3, 0, 0, 0, 32, 24, 16);
    GFL_BGSysQueueScrLoad(3);
    WifiNote_HideWindow(select->menuWindow.window);
}

// pokebw2's WifiNoteModeSelect_StartMessage
static void func_ov128_021c7558(WifiNoteModeSelect *select, WifiNoteDraw *draw, u32 msg, HeapID heapId) {
    StrBuf *temp;

    if (draw->stream != NULL) {
        func_0201ce48(draw->stream);
        draw->stream = NULL;
    }
    temp = GFL_StrBufCreate(128, heapId);
    GFL_BitmapFill(BmpWin_GetBitmap(select->msgWindow.window), 15);
    GFL_MsgDataLoadStrbuf(draw->msgData, msg, temp);
    GFL_WordSetFormatStrbuf(draw->wordSet, select->msgStr, temp);
    draw->stream = func_0201cce4(select->msgWindow.window, 0, 0, select->msgStr, draw->font, func_02012ffc(),
                                 draw->tcbManager, 10, draw->heapId, 0);
    BmpWin_DrawFrame(select->msgWindow.window, 2, 70, 11);
    WifiNote_ShowWindowNow(select->msgWindow.window);
    GFL_StrBufFree(temp);
    func_ov128_021ca3e8(FALSE);
}

// Runs a message's stream, which the A and B buttons and the touch screen speed up and continue. Returns FALSE once
// the message is over
// pokebw2's WifiNote_UpdateStream
static BOOL func_ov128_021c7600(PrintStream *stream, BOOL *paused) {
    switch (func_0201ce30(stream)) {
    case 0:
        if (func_02035c60() == TRUE || (GCTX_HIDGetHeldKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            func_0201ce5c(stream, 0);
        }
        *paused = FALSE;
        break;
    case 1:
        if (*paused == FALSE) {
            if (func_02035c60() == TRUE || (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
                GFL_SndSEPlay(SEQ_SE_MESSAGE);
                func_0201ce38(stream);
                *paused = TRUE;
            }
        }
        break;
    case 2:
        func_0201ce48(stream);
        *paused = FALSE;
        return FALSE;
    }
    return TRUE;
}

// pokebw2's WifiNoteModeSelect_IsMessageDone
static BOOL func_ov128_021c7674(WifiNoteModeSelect *select, WifiNoteData *data, WifiNoteDraw *draw) {
    if (!func_ov128_021c7600(draw->stream, &draw->streamPaused)) {
        data->touch = FALSE;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiNoteModeSelect_ClearMessage
static void func_ov128_021c7694(WifiNoteModeSelect *select, WifiNoteDraw *draw) {
    func_ov128_021ca3e8(TRUE);
    func_0201f7a8(select->msgWindow.window, 0);
    WifiNote_HideWindow(select->msgWindow.window);
}

// Whether the screen of the cursor's button can be entered: the code entry needs a free place in the Wi-Fi list
// pokebw2's WifiNoteModeSelect_CanEnter
static BOOL func_ov128_021c76bc(WifiNoteModeSelect *select, WifiNoteData *data) {
    if (select->cursor == MODE_SELECT_CODE_IN) {
        int i;
        WifiList *wifiList = func_0201283c(data->gameData);

        for (i = 0; i < WIFI_LIST_FRIEND_MAX; i++) {
            if (!func_02009a1c(wifiList, i)) {
                return TRUE;
            }
        }
    } else {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiNoteFriendList_Init
static void func_ov128_021c76f0(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId) {
    func_ov128_021c7acc(list, data, draw, heapId);
}

// pokebw2's WifiNoteFriendList_Main
static BOOL func_ov128_021c76f8(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId) {
    switch (data->seq) {
    case FRIEND_LIST_SEQ_INIT:
        list->page = 0;
        list->prevPage = 0;
        list->pos = 0;
        list->prevPos = 0;
        // fallthrough
    case FRIEND_LIST_SEQ_INIT_KEEP_POS:
        func_ov128_021c76f0(list, data, draw, HEAPID_WIFI_NOTE);
        func_ov128_021c6520(data);
        func_ov128_021c7e0c(list, data, draw, heapId);
        data->seq = FRIEND_LIST_SEQ_MAIN;
        break;
    case FRIEND_LIST_SEQ_MAIN:
        func_0202ea40(draw->blink[0]);
        func_0202ea40(draw->blink[1]);
        switch (func_ov128_021c81c8(list, data, draw)) {
        case FRIEND_LIST_INPUT_NONE:
            break;
        case FRIEND_LIST_INPUT_PREV_PAGE:
            func_ov128_021c8668(list, data, FRIEND_LIST_SCROLL_PREV);
            func_ov128_021ca40c(list, list->page);
            func_0202eb04(draw->blink[1]);
            func_0202ea40(draw->blink[1]);
            break;
        case FRIEND_LIST_INPUT_NEXT_PAGE:
            func_ov128_021c8668(list, data, FRIEND_LIST_SCROLL_NEXT);
            func_ov128_021ca40c(list, list->page);
            func_0202eb04(draw->blink[1]);
            func_0202ea40(draw->blink[1]);
            break;
        case FRIEND_LIST_INPUT_RETURN:
            data->seq = FRIEND_LIST_SEQ_RETURN;
            break;
        case FRIEND_LIST_INPUT_EXIT:
            data->seq = FRIEND_LIST_SEQ_EXIT;
            break;
        case FRIEND_LIST_INPUT_DECIDE:
            data->seq = FRIEND_LIST_SEQ_BUTTON;
            break;
        }
        break;
    case FRIEND_LIST_SEQ_SCROLL_START:
        func_ov128_021c81fc(list, data, draw, heapId);
        data->seq = FRIEND_LIST_SEQ_SCROLL;
        break;
    case FRIEND_LIST_SEQ_SCROLL:
        if (func_ov128_021c8240(list, data, draw, heapId) == TRUE) {
            data->seq = FRIEND_LIST_SEQ_SCROLL_END;
        }
        break;
    case FRIEND_LIST_SEQ_SCROLL_END:
        if (func_ov128_021c8354(list, draw) == TRUE) {
            data->seq = FRIEND_LIST_SEQ_MAIN;
        }
        break;
    case FRIEND_LIST_SEQ_BUTTON:
        if (func_ov128_021ca320(list) != TRUE) {
            func_ov128_021c651c(data, func_ov128_021c87a0(list));
            func_ov128_021c83b0(list, data, draw, heapId);
            data->seq = FRIEND_LIST_SEQ_MENU_MESSAGE;
        }
        break;
    case FRIEND_LIST_SEQ_MENU_MESSAGE:
        if (func_ov128_021c873c(list, data, draw) == TRUE) {
            func_ov128_021c83dc(list, data, draw, heapId);
            data->seq = FRIEND_LIST_SEQ_MENU;
        }
        break;
    case FRIEND_LIST_SEQ_MENU:
        data->seq = func_ov128_021c8470(list, data, draw);
        break;
    case FRIEND_LIST_SEQ_INFO:
        list->wait = 8;
        data->seq = FRIEND_LIST_SEQ_INFO_WAIT;
        break;
    case FRIEND_LIST_SEQ_INFO_WAIT:
        list->wait--;
        if (list->wait <= 0) {
            func_ov128_021c7e48(list, data, draw);
            func_ov128_021c6438(data, WIFI_NOTE_MODE_FRIEND_INFO, FRIEND_INFO_SEQ_INIT);
            func_ov128_021c7ac4(list, data, draw);
            return TRUE;
        }
        break;
    case FRIEND_LIST_SEQ_INFO_RETURN:
        list->page = data->friendPos / FRIEND_LIST_PAGE_SIZE;
        list->pos = data->friendPos % FRIEND_LIST_PAGE_SIZE;
        data->seq = FRIEND_LIST_SEQ_INIT_KEEP_POS;
        break;
    case FRIEND_LIST_SEQ_NAME: {
        s32 friend;
        WifiList *wifiList;

        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, heapId);
        friend = func_ov128_021c87a0(list);
        wifiList = func_0201283c(data->gameData);
        func_ov128_021c6514(data, func_020099d4(wifiList, data->friends.index[friend]));
        data->seq = FRIEND_LIST_SEQ_NAME_WAIT;
        break;
    }
    case FRIEND_LIST_SEQ_NAME_WAIT:
        if (GFL_WipeIsFinished()) {
            func_ov128_021c7e48(list, data, draw);
            func_ov128_021c6438(data, WIFI_NOTE_MODE_CODE_IN, CODE_IN_SEQ_NAME_INIT);
            func_ov128_021c7ac4(list, data, draw);
            return TRUE;
        }
        break;
    case FRIEND_LIST_SEQ_NAME_RETURN: {
        s32 friend = func_ov128_021c87a0(list);
        WifiList *wifiList = func_0201283c(data->gameData);

        func_02009a00(wifiList, data->friends.index[friend], data->name);
    }
        // fallthrough
    case FRIEND_LIST_SEQ_NAME_CANCEL:
        func_ov128_021c76f0(list, data, draw, HEAPID_WIFI_NOTE);
        func_ov128_021c7e0c(list, data, draw, heapId);
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, heapId);
        data->seq = FRIEND_LIST_SEQ_FADE_IN;
        break;
    case FRIEND_LIST_SEQ_FADE_IN:
        if (GFL_WipeIsFinished()) {
            data->seq = FRIEND_LIST_SEQ_MAIN;
        }
        break;
    case FRIEND_LIST_SEQ_DELETE_MESSAGE:
        func_ov128_021c8548(list, data, draw, heapId);
        data->seq = FRIEND_LIST_SEQ_DELETE_MESSAGE_WAIT;
        break;
    case FRIEND_LIST_SEQ_DELETE_MESSAGE_WAIT:
        if (func_ov128_021c8574(list, data, draw, heapId) == TRUE) {
            data->seq = FRIEND_LIST_SEQ_DELETE_YES_NO;
        }
        break;
    case FRIEND_LIST_SEQ_DELETE_YES_NO:
        switch (func_ov128_021c6e74(list->menu)) {
        case 0:
            data->touch = func_020355c4();
            func_ov128_021c875c(list, draw);
            data->seq = FRIEND_LIST_SEQ_DELETE_START;
            break;
        case 1:
            data->touch = func_020355c4();
            func_ov128_021c875c(list, draw);
            data->seq = FRIEND_LIST_SEQ_MAIN;
            break;
        }
        break;
    case FRIEND_LIST_SEQ_DELETE_START:
        func_ov128_021c87b0(list, draw);
        data->seq = FRIEND_LIST_SEQ_DELETE;
        break;
    case FRIEND_LIST_SEQ_DELETE:
        if (func_ov128_021c8850(list, draw) == TRUE) {
            WifiList *wifiList = func_0201283c(data->gameData);

            func_02009a74(wifiList, data->friends.index[func_ov128_021c87a0(list)]);
            func_ov128_021c6520(data);
            func_ov128_021c85a4(list, data, draw, list->page, list->pos, 0, heapId);
            data->seq = FRIEND_LIST_SEQ_DELETE_END;
        }
        break;
    case FRIEND_LIST_SEQ_DELETE_END:
        func_ov128_021c8880(list, draw);
        data->seq = FRIEND_LIST_SEQ_MAIN;
        break;
    case FRIEND_LIST_SEQ_CODE:
        if (func_ov128_021c84e8(list, data, draw, heapId) == TRUE) {
            data->seq = FRIEND_LIST_SEQ_CODE_WAIT;
        } else {
            data->seq = FRIEND_LIST_SEQ_MAIN;
        }
        break;
    case FRIEND_LIST_SEQ_CODE_WAIT:
        if (func_ov128_021c873c(list, data, draw)) {
            if (func_ov128_021ca084(data) == TRUE) {
                data->seq = FRIEND_LIST_SEQ_MAIN;
                func_ov128_021c875c(list, draw);
            }
        }
        break;
    case FRIEND_LIST_SEQ_RETURN:
        if (func_0204b98c(draw->returnButton) != TRUE) {
            func_ov128_021c7e48(list, data, draw);
            func_ov128_021c6438(data, WIFI_NOTE_MODE_SELECT, MODE_SELECT_SEQ_INIT_NO_FADE);
            func_ov128_021c7ac4(list, data, draw);
            return TRUE;
        }
        break;
    case FRIEND_LIST_SEQ_EXIT:
        if (func_0204b98c(draw->exitButton) != TRUE) {
            data->seq = FRIEND_LIST_SEQ_FADE_OUT;
        }
        break;
    case FRIEND_LIST_SEQ_FADE_OUT:
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, heapId);
        data->seq = FRIEND_LIST_SEQ_FADE_OUT_WAIT;
        break;
    case FRIEND_LIST_SEQ_FADE_OUT_WAIT:
        if (GFL_WipeIsFinished()) {
            func_ov128_021c6438(data, WIFI_NOTE_MODE_EXIT, 0);
            func_ov128_021c7ac4(list, data, draw);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's WifiNoteFriendList_Exit
static void func_ov128_021c7ac4(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw) {
    func_ov128_021c7d48(list, data, draw);
}

static void func_ov128_021c7acc(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw,
                                        HeapID heapId) {
    int i;

    for (i = 0; i < 2; i++) {
        func_ov128_021c63ac(draw, &list->nameWindows[i], 3, MATH_ABS(data_ov128_021ca564[i].x) + 4,
                              MATH_ABS(data_ov128_021ca564[i].y) + 4, 26, 17, 10, 0, 0);
    }
    for (i = 0; i < FRIEND_LIST_AREA_COUNT; i++) {
        func_ov128_021c88ac(&list->areas[i], draw, &data_ov128_021ca564[i], 0, heapId);
    }
    list->areas[0].nameWindow = &list->nameWindows[0];
    list->areas[2].nameWindow = &list->nameWindows[1];
    list->areas[1].nameWindow = &list->nameWindows[1];
    list->charSys = func_ov127_021c3ee0(draw->scrollUnit, NULL, 16, heapId);
    func_ov127_021c4044(list->charSys, CLACT_VRAM_MAIN, 2, 0xe0, heapId);
    for (i = 0; i < 4; i++) {
        list->menuItems[0][i].str = GFL_MsgDataLoadStrbufNew(draw->msgData, data_ov128_021ca688[0][i].msg);
        list->menuItems[0][i].color = WIFI_NOTE_MENU_COLOR;
        list->menuItems[0][i].type = 0;
        list->menuItems[1][i].str = GFL_MsgDataLoadStrbufNew(draw->msgData, data_ov128_021ca688[1][i].msg);
        list->menuItems[1][i].color = WIFI_NOTE_MENU_COLOR;
        list->menuItems[1][i].type = 0;
    }
    list->menuItems[0][3].type = APP_TASKMENU_ITEM_RETURN;
    list->menuItems[1][3].type = APP_TASKMENU_ITEM_RETURN;
    list->yesNoItems[0].str = GFL_MsgDataLoadStrbufNew(draw->msgData, Bank0418_Text_066);
    list->yesNoItems[0].color = WIFI_NOTE_MENU_COLOR;
    list->yesNoItems[0].type = 0;
    list->yesNoItems[1].str = GFL_MsgDataLoadStrbufNew(draw->msgData, Bank0418_Text_067);
    list->yesNoItems[1].color = WIFI_NOTE_MENU_COLOR;
    list->yesNoItems[1].type = 0;
    list->menuRes = func_02027f60(1, 14, draw->font, draw->queue, heapId);
    func_ov128_021c63ac(draw, &list->msgWindow, 1, 2, 1, 27, 4, 10, 0, 15);
    list->msgStr = GFL_StrBufCreate(128, heapId);
    {
        ClActorSetup setup = { 0, 0, 1, 0, 0 };

        list->eraser =
            func_0204b294(draw->unit, draw->chars[0], draw->palettes[0], draw->cellAnims[0], &setup, 0, heapId);
        func_0204b3dc(list->eraser, FALSE);
    }
    {
        ClActorSetup setup = { 56, 180, 2, 0, 0 };

        list->pageCursor =
            func_0204b294(draw->unit, draw->chars[0], draw->palettes[0], draw->cellAnims[0], &setup, 0, heapId);
    }
}

// pokebw2's WifiNoteFriendList_ExitDraw
static void func_ov128_021c7d48(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw) {
    int i;

    func_ov128_021ca444(draw);
    func_ov128_021ca458(draw, &draw->title);
    func_0204b3b4(list->pageCursor);
    func_0204b3b4(list->eraser);
    BmpWin_Free(list->msgWindow.window);
    GFL_StrBufFree(list->msgStr);
    func_02027fd8(list->menuRes);
    GFL_StrBufFree(list->yesNoItems[0].str);
    GFL_StrBufFree(list->yesNoItems[1].str);
    for (i = 0; i < 4; i++) {
        GFL_StrBufFree(list->menuItems[0][i].str);
        GFL_StrBufFree(list->menuItems[1][i].str);
    }
    func_ov127_021c4080(list->charSys);
    func_ov127_021c3f80(list->charSys);
    for (i = 0; i < FRIEND_LIST_AREA_COUNT; i++) {
        func_ov128_021c88bc(&list->areas[i], draw);
    }
    for (i = 0; i < 2; i++) {
        BmpWin_Free(list->nameWindows[i].window);
    }
}

// pokebw2's WifiNoteFriendList_Show
static void func_ov128_021c7e0c(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId) {
    func_ov128_021c6adc(draw, &draw->title, Bank0418_Text_005);
    GFL_BGSysQueueScrLoad(0);
    func_ov128_021c85a4(list, data, draw, list->page, list->pos, 0, heapId);
}

// pokebw2's WifiNoteFriendList_Hide
static void func_ov128_021c7e48(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw) {
    int i;

    func_ov128_021c6b38(draw);
    for (i = 0; i < FRIEND_LIST_AREA_COUNT; i++) {
        func_ov128_021c89c4(&list->areas[i], draw, &draw->screens);
    }
    GFL_BGSysQueueScrLoad(0);
}

// Returns the FRIEND_LIST_INPUT_* that an input comes to, with its sound
// pokebw2's WifiNoteFriendList_Decide
static u32 func_ov128_021c7e78(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw, u32 input) {
    if (input == FRIEND_LIST_INPUT_NONE) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov128_021ca3a0(draw);
        return FRIEND_LIST_INPUT_RETURN;
    }
    if (input == FRIEND_LIST_INPUT_X) {
        GFL_SndSEPlay(SEQ_SE_CLOSE1);
        func_ov128_021ca3c4(draw);
        return FRIEND_LIST_INPUT_EXIT;
    }
    if (input == FRIEND_LIST_INPUT_DECIDE) {
        if (list->pos == FRIEND_LIST_POS_RETURN) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov128_021ca3a0(draw);
            return FRIEND_LIST_INPUT_RETURN;
        }
        if (list->pos == FRIEND_LIST_POS_EXIT) {
            GFL_SndSEPlay(SEQ_SE_CLOSE1);
            func_ov128_021ca3c4(draw);
            return FRIEND_LIST_INPUT_EXIT;
        }
        if (data->friends.count <= list->page * FRIEND_LIST_PAGE_SIZE + list->pos) {
            return FRIEND_LIST_INPUT_NONE;
        }
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        list->buttonSeq = 0;
        return FRIEND_LIST_INPUT_DECIDE;
    }
    if (input == FRIEND_LIST_INPUT_PREV_PAGE) {
        return FRIEND_LIST_INPUT_PREV_PAGE;
    }
    if (input == FRIEND_LIST_INPUT_NEXT_PAGE) {
        return FRIEND_LIST_INPUT_NEXT_PAGE;
    }
    return FRIEND_LIST_INPUT_NONE;
}

// pokebw2's WifiNoteFriendList_Keys
static u32 func_ov128_021c7f14(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw) {
    u16 prevPos;
    BOOL turn;
    BOOL next;
    BOOL moved;

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        func_020355d8(FALSE);
        data->touch = FALSE;
        return func_ov128_021c7e78(list, data, draw, FRIEND_LIST_INPUT_NONE);
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
        func_020355d8(FALSE);
        data->touch = FALSE;
        return func_ov128_021c7e78(list, data, draw, FRIEND_LIST_INPUT_X);
    }
    if (data->touch == TRUE) {
        if (GCTX_HIDGetPressedKeys() & WIFI_NOTE_KEYS) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov128_021ca2c4(draw, list->pos, list->pos);
            func_020355d8(FALSE);
            data->touch = FALSE;
        }
        return FRIEND_LIST_INPUT_NONE;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        return func_ov128_021c7e78(list, data, draw, FRIEND_LIST_INPUT_DECIDE);
    }
    prevPos = list->pos;
    moved = FALSE;
    turn = FALSE;
    if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
        if (list->pos == 0) {
            list->pos = 3;
        } else if (list->pos == 4) {
            list->pos = 7;
        } else {
            list->pos--;
        }
        moved = TRUE;
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
        if (list->pos == 3) {
            list->pos = 0;
        } else if (list->pos == 7) {
            list->pos = 4;
        } else {
            list->pos++;
        }
        moved = TRUE;
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_LEFT) {
        if (list->pos < 4) {
            list->pos += 4;
            turn = TRUE;
            next = FALSE;
        } else {
            list->pos -= 4;
        }
        moved = TRUE;
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_RIGHT) {
        if (list->pos < 4) {
            list->pos += 4;
        } else {
            list->pos -= 4;
            turn = TRUE;
            next = TRUE;
        }
        moved = TRUE;
    } else if (data->lrEnabled) {
        if (data->lrEnabled && (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L)) {
            turn = TRUE;
            next = FALSE;
        } else if (data->lrEnabled && (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R)) {
            turn = TRUE;
            next = TRUE;
        }
    }
    if (turn == TRUE) {
        if (next) {
            list->prevPage = list->page;
            list->page = (list->page + 1) % FRIEND_LIST_PAGE_COUNT;
            return func_ov128_021c7e78(list, data, draw, FRIEND_LIST_INPUT_NEXT_PAGE);
        }
        list->prevPage = list->page;
        list->page--;
        if (list->page < 0) {
            list->page += FRIEND_LIST_PAGE_COUNT;
        }
        return func_ov128_021c7e78(list, data, draw, FRIEND_LIST_INPUT_PREV_PAGE);
    }
    if (moved == TRUE) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov128_021ca2c4(draw, list->pos, prevPos);
        list->prevPos = prevPos;
    }
    return FRIEND_LIST_INPUT_NONE;
}

// pokebw2's WifiNoteFriendList_Touch
static u32 func_ov128_021c8100(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw, BOOL *touched) {
    u32 hit = func_02035c08(data_ov128_021ca5fc);
    s8 turn;

    if (hit == TOUCH_RECT_NONE) {
        return FRIEND_LIST_INPUT_NONE;
    }
    if (hit <= FRIEND_LIST_PAGE_SIZE - 1) {
        *touched = TRUE;
        func_ov128_021ca294(draw, list->pos);
        list->pos = hit;
        return func_ov128_021c7e78(list, data, draw, FRIEND_LIST_INPUT_DECIDE);
    }
    if (hit == FRIEND_LIST_POS_RETURN) {
        *touched = TRUE;
        list->pos = hit;
        return func_ov128_021c7e78(list, data, draw, FRIEND_LIST_INPUT_DECIDE);
    }
    if (hit == FRIEND_LIST_POS_EXIT) {
        *touched = TRUE;
        list->pos = hit;
        return func_ov128_021c7e78(list, data, draw, FRIEND_LIST_INPUT_DECIDE);
    }
    turn = hit - 10 - list->page;
    if (turn != 0) {
        *touched = TRUE;
        list->prevPage = list->page;
        list->page += turn;
        if (turn >= 0) {
            list->page %= FRIEND_LIST_PAGE_COUNT;
            return func_ov128_021c7e78(list, data, draw, FRIEND_LIST_INPUT_NEXT_PAGE);
        }
        if (list->page < 0) {
            list->page += FRIEND_LIST_PAGE_COUNT;
        }
        return func_ov128_021c7e78(list, data, draw, FRIEND_LIST_INPUT_PREV_PAGE);
    }
    return FRIEND_LIST_INPUT_NONE;
}

// pokebw2's WifiNoteFriendList_Input
static u32 func_ov128_021c81c8(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw) {
    BOOL touched = FALSE;
    u32 input = func_ov128_021c8100(list, data, draw, &touched);

    if (touched) {
        func_020355d8(TRUE);
        data->touch = TRUE;
        return input;
    }
    return func_ov128_021c7f14(list, data, draw);
}

static void func_ov128_021c81fc(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw,
                                           HeapID heapId) {
    u32 area;

    if (list->scrollDir == FRIEND_LIST_SCROLL_PREV) {
        area = 2;
    } else {
        area = 1;
    }
    func_ov128_021c85a4(list, data, draw, list->page, list->pos, area, heapId);
    func_ov128_021ca294(draw, list->pos);
    GFL_SndSEPlay(SEQ_SE_SELECT1);
}

// pokebw2's WifiNoteFriendList_Scroll
static BOOL func_ov128_021c8240(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId) {
    ClActorPos pos;
    s32 scroll;

    if (!func_0201c6b4(draw->queue)) {
        return FALSE;
    }
    if (list->scrollCount > FRIEND_LIST_SCROLL_FRAMES) {
        u32 area;

        pos.x = data_ov128_021ca7d0.x;
        pos.y = data_ov128_021ca7d0.y;
        func_0204b09c(draw->renderer, 0, &pos);
        if (list->scrollDir == FRIEND_LIST_SCROLL_PREV) {
            area = 2;
        } else {
            area = 1;
        }
        func_ov128_021c8a50(&list->areas[area], draw);
        func_ov128_021c860c(list, data, draw, list->page, list->pos, heapId);
        return TRUE;
    }
    scroll = list->scrollCount * 256 / FRIEND_LIST_SCROLL_FRAMES;
    if (list->scrollDir == FRIEND_LIST_SCROLL_PREV) {
        scroll = -scroll;
    }
    GFL_BGSysMoveBGReq(2, 0, scroll);
    GFL_BGSysMoveBGReq(3, 0, scroll);
    pos.x = data_ov128_021ca7d0.x;
    pos.y = data_ov128_021ca7d0.y;
    pos.x += scroll;
    func_0204b09c(draw->renderer, 0, &pos);
    if (list->scrollCount % 6 != 0) {
        WifiNoteScrollFunc func = data_ov128_021ca7c0[list->scrollCount / 6];

        if (func != NULL) {
            func(list, draw, list->page, list->prevPage);
        }
    }
    list->scrollCount++;
    return FALSE;
}

// pokebw2's WifiNoteFriendList_ScrollEnd
static BOOL func_ov128_021c8354(WifiNoteFriendList *list, WifiNoteDraw *draw) {
    if (func_0201c6b4(draw->queue) == TRUE) {
        GFL_BGSysMoveBGReq(2, 0, 0);
        GFL_BGSysMoveBGReq(3, 0, 0);
        func_ov128_021ca4b4(list->areas[0].nameWindow->window);
        return TRUE;
    }
    return FALSE;
}

// The friend key of the cursor's friend, 0 for a friend met in person
// pokebw2's WifiNoteFriendList_GetFriendKey
static u64 func_ov128_021c8388(WifiNoteFriendList *list, WifiNoteData *data) {
    WifiList *wifiList = func_0201283c(data->gameData);

    return DWC_GetFriendKey(func_020099c8(wifiList, data->friends.index[func_ov128_021c87a0(list)]));
}

static void func_ov128_021c83b0(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw,
                                                HeapID heapId) {
    func_ov128_021c6bcc(draw, data, data->friends.index[func_ov128_021c87a0(list)], heapId);
    func_ov128_021c867c(list, draw, Bank0418_Text_012, heapId);
}

static void func_ov128_021c83dc(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw,
                                        HeapID heapId) {
    WifiList *wifiList = func_0201283c(data->gameData);
    u32 gender =
        func_02009864(wifiList, data->friends.index[func_ov128_021c87a0(list)], WIFI_LIST_FRIEND_GENDER);
    AppTaskMenuInit init;

    init.heapId = heapId;
    init.itemCount = 4;
    if (gender == WIFI_LIST_GENDER_UNKNOWN) {
        if (func_ov128_021c8388(list, data) != 0) {
            init.items = list->menuItems[1];
        } else {
            init.items = list->menuItems[0];
        }
        list->menuType = 1;
    } else {
        init.items = list->menuItems[0];
        list->menuType = 0;
    }
    init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
    init.x = 32;
    init.y = 24;
    init.width = 17;
    init.height = 3;
    list->menu = func_02027778(&init, list->menuRes);
}

// Returns the sequence to go to
// pokebw2's WifiNoteFriendList_UpdateMenu
static u32 func_ov128_021c8470(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw) {
    u32 pos;
    u32 seq;

    func_02027974(list->menu);
    if (func_020279e8(list->menu) == TRUE) {
        pos = func_02027a04(list->menu);
        func_02027858(list->menu);
    } else {
        return FRIEND_LIST_SEQ_MENU;
    }
    if (pos == 3) {
        data->touch = func_020355c4();
        seq = FRIEND_LIST_SEQ_MAIN;
    } else {
        data->touch = func_020355c4();
        seq = data_ov128_021ca688[list->menuType][pos].seq;
    }
    if (seq == FRIEND_LIST_SEQ_MAIN) {
        func_ov128_021c875c(list, draw);
    } else if (seq == FRIEND_LIST_SEQ_INFO || seq == FRIEND_LIST_SEQ_NAME) {
        func_ov128_021c875c(list, draw);
    } else {
        func_ov128_021c8784(list);
    }
    return seq;
}

static BOOL func_ov128_021c84e8(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw,
                                                HeapID heapId) {
    u64 key = func_ov128_021c8388(list, data);

    func_ov128_021c6bcc(draw, data, data->friends.index[func_ov128_021c87a0(list)], heapId);
    if (key != 0) {
        func_ov128_021c6b50(draw, key);
        func_ov128_021c867c(list, draw, Bank0418_Text_060, heapId);
    } else {
        func_ov128_021c867c(list, draw, Bank0418_Text_068, heapId);
    }
    return TRUE;
}

static void func_ov128_021c8548(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw,
                                                  HeapID heapId) {
    func_ov128_021c6bcc(draw, data, data->friends.index[func_ov128_021c87a0(list)], heapId);
    func_ov128_021c867c(list, draw, Bank0418_Text_057, heapId);
}

static BOOL func_ov128_021c8574(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw,
                                         HeapID heapId) {
    if (!func_ov128_021c873c(list, data, draw)) {
        return FALSE;
    }
    list->menu = func_ov128_021c6e3c(list->yesNoItems, &list->menuRes, FALSE, heapId);
    return TRUE;
}

// Draws a page of the roster in an area. With area 0, the page shown, it becomes the current page
static void func_ov128_021c85a4(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw, s32 page,
                                        s32 pos, u32 area, HeapID heapId) {
    if (area == 0) {
        list->page = page;
        list->pos = pos;
        func_ov128_021ca40c(list, page);
    }
    func_ov128_021c88c0(&list->areas[area], draw, &draw->screens);
    func_ov128_021c899c(&list->areas[area], data, draw, list->charSys, &draw->screens, &data->friends, page, heapId);
    func_ov128_021ca2dc(list);
}

static void func_ov128_021c860c(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw, s32 page,
                                       s32 pos, HeapID heapId) {
    list->pos = pos;
    list->page = page;
    func_ov128_021ca40c(list, page);
    func_ov128_021c88c0(&list->areas[0], draw, &draw->screens);
    func_ov128_021c8904(&list->areas[0], data, draw, list->charSys, &draw->screens, &data->friends, page,
                                 heapId);
    func_ov128_021ca2dc(list);
}

static void func_ov128_021c8660(WifiNoteFriendList *list, WifiNoteDraw *draw, s16 page, s16 prevPage) {
}

// pokebw2's WifiNoteFriendList_ScrollFunc2
static void func_ov128_021c8664(WifiNoteFriendList *list, WifiNoteDraw *draw, s16 page, s16 prevPage) {
}

// pokebw2's WifiNoteFriendList_RequestScroll
static void func_ov128_021c8668(WifiNoteFriendList *list, WifiNoteData *data, u32 dir) {
    data->seq = FRIEND_LIST_SEQ_SCROLL_START;
    list->scrollDir = dir;
    list->scrollCount = 0;
}

// pokebw2's WifiNoteFriendList_StartMessage
static void func_ov128_021c867c(WifiNoteFriendList *list, WifiNoteDraw *draw, u32 msg, HeapID heapId) {
    StrBuf *temp;

    if (draw->stream != NULL) {
        func_0201ce48(draw->stream);
        draw->stream = NULL;
    }
    GFL_BitmapFill(BmpWin_GetBitmap(list->msgWindow.window), 15);
    temp = GFL_StrBufCreate(128, heapId);
    GFL_MsgDataLoadStrbuf(draw->msgData, msg, temp);
    GFL_WordSetFormatStrbuf(draw->wordSet, list->msgStr, temp);
    draw->stream = func_0201cce4(list->msgWindow.window, 0, 0, list->msgStr, draw->font, func_02012ffc(),
                                 draw->tcbManager, 10, draw->heapId, 0);
    BmpWin_DrawFrame(list->msgWindow.window, 2, 70, 11);
    WifiNote_ShowWindowNow(list->msgWindow.window);
    GFL_StrBufFree(temp);
    func_ov128_021ca3e8(FALSE);
}

// pokebw2's WifiNoteFriendList_IsMessageDone
static BOOL func_ov128_021c873c(WifiNoteFriendList *list, WifiNoteData *data, WifiNoteDraw *draw) {
    if (!func_ov128_021c7600(draw->stream, &draw->streamPaused)) {
        data->touch = FALSE;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiNoteFriendList_ClearMessage
static void func_ov128_021c875c(WifiNoteFriendList *list, WifiNoteDraw *draw) {
    func_0201f7a8(list->msgWindow.window, 1);
    func_ov128_021ca42c(draw);
    func_ov128_021ca2dc(list);
    func_ov128_021ca3e8(TRUE);
}

// pokebw2's WifiNoteFriendList_EraseMessage
static void func_ov128_021c8784(WifiNoteFriendList *list) {
    GFL_BitmapFill(BmpWin_GetBitmap(list->msgWindow.window), 15);
    BmpWin_FlushChar(list->msgWindow.window);
}

// The cursor's place among the roster's friends
// pokebw2's WifiNoteFriendList_GetFriend
static s32 func_ov128_021c87a0(WifiNoteFriendList *list) {
    s32 friend = list->page * FRIEND_LIST_PAGE_SIZE;

    return friend + list->pos;
}

// The eraser that rubs a deleted friend out, in an OBJ window over the friend
// pokebw2's WifiNoteFriendList_StartErase
static void func_ov128_021c87b0(WifiNoteFriendList *list, WifiNoteDraw *draw) {
    ClActorPos pos;

    func_0204b3dc(list->eraser, TRUE);
    func_0204b878(list->eraser, 1);
    func_0204b9a0(list->eraser);
    list->eraserFrame = 0;
    func_0204b92c(list->eraser, TRUE);
    pos.x = data_ov128_021ca6c8[list->pos].x * 8;
    pos.y = data_ov128_021ca6c8[list->pos].y * 8 - 4;
    func_0204b404(list->eraser, &pos, 0);
    G2_SetWndOBJInsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2, FALSE);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 |
                              GX_WND_PLANEMASK_OBJ,
                          TRUE);
    GX_SetVisibleWnd(GX_WNDMASK_OW);
    GFL_SndSEPlay(SEQ_SE_SYS_41);
}

// Returns TRUE once the eraser's animation is over
// pokebw2's WifiNoteFriendList_UpdateErase
static BOOL func_ov128_021c8850(WifiNoteFriendList *list, WifiNoteDraw *draw) {
    fx32 frame;

    if (!func_0204b98c(list->eraser)) {
        return TRUE;
    }
    frame = func_0204b8f4(list->eraser);
    if ((u32)(frame - list->eraserFrame) >= 2) {
        list->eraserFrame = frame;
    }
    return FALSE;
}

// pokebw2's WifiNoteFriendList_EndErase
static void func_ov128_021c8880(WifiNoteFriendList *list, WifiNoteDraw *draw) {
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    func_0204b3dc(list->eraser, FALSE);
    func_0204b92c(list->eraser, FALSE);
}

static void func_ov128_021c88ac(WifiNoteListArea *area, WifiNoteDraw *draw, const WifiNoteScreenArea *screenArea,
                                  u32 charOffset, HeapID heapId) {
    area->area = *screenArea;
}

// pokebw2's WifiNoteListArea_Exit
static void func_ov128_021c88bc(WifiNoteListArea *area, WifiNoteDraw *draw) {
}

// pokebw2's WifiNoteListArea_DrawBG
static void func_ov128_021c88c0(WifiNoteListArea *area, WifiNoteDraw *draw, WifiNoteScreens *screens) {
    GFL_BGSysLoadScrAreaAll(2, screens->screen[0]->rawData, MATH_ABS(area->area.x), MATH_ABS(area->area.y),
                            screens->screen[0]->screenWidth / 8, screens->screen[0]->screenHeight / 8);
    GFL_BGSysQueueScrLoad(2);
}

static void func_ov128_021c8904(WifiNoteListArea *area, WifiNoteData *data, WifiNoteDraw *draw,
                                         WF_2DCSYS *charSys, WifiNoteScreens *screens, WifiNoteFriends *friends,
                                         s32 page, HeapID heapId) {
    int first;
    WifiList *wifiList = func_0201283c(data->gameData);
    GFLBitmap *bitmap = BmpWin_GetBitmap(area->nameWindow->window);
    BOOL printed = FALSE;
    int i;

    GFL_BitmapFill(bitmap, 0);
    func_ov128_021c8a50(area, draw);
    first = page * FRIEND_LIST_PAGE_SIZE;
    for (i = 0; i < FRIEND_LIST_PAGE_SIZE; i++) {
        if (friends->count > first + i) {
            func_ov128_021c8a80(area, charSys, draw, screens, i, wifiList, friends->index[first + i], heapId);
            printed = TRUE;
        } else {
            func_ov128_021c8b90(area, draw, screens, i, heapId);
        }
    }
    if (!printed) {
        BmpWin_FlushChar(area->nameWindow->window);
    }
}

static void func_ov128_021c899c(WifiNoteListArea *area, WifiNoteData *data, WifiNoteDraw *draw, WF_2DCSYS *charSys,
                                  WifiNoteScreens *screens, WifiNoteFriends *friends, s32 page, HeapID heapId) {
    func_ov128_021c8904(area, data, draw, charSys, screens, friends, page, heapId);
    func_ov128_021ca4b4(area->nameWindow->window);
}

// pokebw2's WifiNoteListArea_Clear
static void func_ov128_021c89c4(WifiNoteListArea *area, WifiNoteDraw *draw, WifiNoteScreens *screens) {
    func_ov128_021c8a50(area, draw);
    WifiNote_HideWindow(area->nameWindow->window);
    GFL_BGSysFillScrArea(2, 0, MATH_ABS(area->area.x), MATH_ABS(area->area.y), area->area.width, area->area.height, 0);
    GFL_BGSysQueueScrLoad(2);
    GFL_BGSysFillScrArea(3, 0, MATH_ABS(area->area.x), MATH_ABS(area->area.y), area->area.width, area->area.height, 0);
    GFL_BGSysQueueScrLoad(3);
}

// pokebw2's WifiNoteListArea_DeleteActors
static void func_ov128_021c8a50(WifiNoteListArea *area, WifiNoteDraw *draw) {
    int i;

    for (i = 0; i < FRIEND_LIST_PAGE_SIZE; i++) {
        if (area->chars[i] != NULL) {
            func_ov127_021c41bc(area->chars[i]);
            area->chars[i] = NULL;
        }
        if (area->frames[i] != NULL) {
            func_0204b3b4(area->frames[i]);
            area->frames[i] = NULL;
        }
    }
}

// A friend's trainer, or an empty frame for a friend not met, and the name in the color of the gender
static void func_ov128_021c8a80(WifiNoteListArea *area, WF_2DCSYS *charSys, WifiNoteDraw *draw,
                                        WifiNoteScreens *screens, s32 friendPos, WifiList *wifiList, u32 index,
                                        HeapID heapId) {
    WF_2DC_WKDATA charData;
    u16 colors[3];
    u32 view;
    u32 gender;
    StrBuf *name;
    u32 pos = friendPos;

    colors[0] = 0x14c0;
    colors[1] = 0xc80;
    colors[2] = WIFI_NOTE_COLOR_BLACK;
    view = func_02009864(wifiList, index, WIFI_LIST_FRIEND_TRAINER_VIEW);
    gender = func_02009864(wifiList, index, WIFI_LIST_FRIEND_GENDER);
    if (gender == WIFI_LIST_GENDER_UNKNOWN) {
        func_ov128_021c8b90(area, draw, screens, pos, heapId);
    } else {
        charData.x = (area->area.x + data_ov128_021ca6c8[pos].x) * 8 + 8;
        charData.y = (area->area.y + data_ov128_021ca6c8[pos].y) * 8 + 10;
        charData.priority = 8;
        charData.bgPriority = 1;
        area->chars[pos] = func_ov127_021c40bc(charSys, &charData, view, heapId);
        func_ov127_021c42a8(area->chars[pos], 0, 1);
    }
    name = GFL_StrBufCreate(128, heapId);
    GFL_StrBufLoadString(name, func_020099d4(wifiList, index));
    func_ov067_021b6ae4(area->nameWindow, draw->queue, data_ov128_021ca6c8[pos].x * 8,
                    data_ov128_021ca6c8[pos].y * 8 - 24, name, draw->font, colors[gender], PRINT_ALIGN_LEFT);
    GFL_StrBufFree(name);
}

static void func_ov128_021c8b90(WifiNoteListArea *area, WifiNoteDraw *draw, WifiNoteScreens *screens, s32 pos,
                                       HeapID heapId) {
    ClActorSetup setup;

    setup.x = (area->area.x + data_ov128_021ca6c8[pos].x) * 8 + 16;
    setup.y = (area->area.y + data_ov128_021ca6c8[pos].y) * 8 + 16;
    setup.sequence = 0;
    setup.priority = 8;
    setup.bgPriority = 1;
    area->frames[pos] =
        func_0204b294(draw->scrollUnit, draw->chars[0], draw->palettes[0], draw->cellAnims[0], &setup, 0, heapId);
}

// The name entry and then the code input for a new friend, or the name entry alone for a friend of the roster. The
// screen's graphics and the menus' overlay make room for them
static BOOL func_ov128_021c8c04(WifiNoteCodeIn *codeIn, WifiNoteWork *wk, WifiNoteData *data, WifiNoteDraw *draw,
                                HeapID heapId) {
    switch (data->seq) {
    case CODE_IN_SEQ_NEW_INIT: {
        int blocks[CODEIN_BLOCK_COUNT];

        GFL_OvlUnload(OVERLAY_APP_UI);
        codeIn->nameParam = func_ov128_021c8da0(codeIn, data, TRUE, heapId);
        blocks[0] = 4;
        blocks[1] = 4;
        blocks[2] = 4;
        codeIn->codeParam = func_0202e4ec(heapId, CODEIN_MODE_FRIEND_CODE, 0, 12, blocks);
        func_ov128_021c6390(wk);
        codeIn->procManager = CreateGameProcManager(heapId);
        QueueGameProc(codeIn->procManager, OVERLAY_ID(194), &data_ov194_021f5580, codeIn->nameParam);
        data->seq = CODE_IN_SEQ_NEW_NAME;
        break;
    }
    case CODE_IN_SEQ_NEW_NAME:
        if (!GFL_ProcMgrUpdate(codeIn->procManager)) {
            if (codeIn->nameParam->unk1C == 0) {
                QueueGameProc(codeIn->procManager, OVERLAY_CODEIN, &data_ov129_021da62c, codeIn->codeParam);
                data->seq = CODE_IN_SEQ_NEW_CODE;
            } else {
                FreeGameProcManager(codeIn->procManager);
                func_ov128_021c6438(data, WIFI_NOTE_MODE_SELECT, MODE_SELECT_SEQ_INIT);
                data->seq = CODE_IN_SEQ_NEW_END;
            }
        }
        break;
    case CODE_IN_SEQ_NEW_CODE:
        if (!GFL_ProcMgrUpdate(codeIn->procManager)) {
            FreeGameProcManager(codeIn->procManager);
            func_ov128_021c6440(data, codeIn->nameParam->name, codeIn->codeParam->str);
            func_ov128_021c6438(data, WIFI_NOTE_MODE_SELECT, MODE_SELECT_SEQ_CODE_INIT);
            data->seq = CODE_IN_SEQ_NEW_END;
        }
        break;
    case CODE_IN_SEQ_NEW_END:
        func_ov010_0216c0c0(codeIn->nameParam);
        func_0202e54c(codeIn->codeParam);
        GFL_OvlLoad(OVERLAY_APP_UI);
        func_ov128_021c6360(wk, heapId);
        return TRUE;
    case CODE_IN_SEQ_NAME_INIT:
        GFL_OvlUnload(OVERLAY_APP_UI);
        codeIn->nameParam = func_ov128_021c8da0(codeIn, data, FALSE, heapId);
        func_ov128_021c6390(wk);
        codeIn->procManager = CreateGameProcManager(heapId);
        QueueGameProc(codeIn->procManager, OVERLAY_ID(194), &data_ov194_021f5580, codeIn->nameParam);
        data->seq = CODE_IN_SEQ_NAME;
        break;
    case CODE_IN_SEQ_NAME:
        if (!GFL_ProcMgrUpdate(codeIn->procManager)) {
            FreeGameProcManager(codeIn->procManager);
            data->touch = func_020355c4();
            if (codeIn->nameParam->unk1C == 0) {
                func_ov128_021c6438(data, WIFI_NOTE_MODE_FRIEND_LIST, FRIEND_LIST_SEQ_NAME_RETURN);
                func_ov128_021c6440(data, codeIn->nameParam->name, NULL);
                data->seq = CODE_IN_SEQ_NAME_END;
            } else {
                func_ov128_021c6438(data, WIFI_NOTE_MODE_FRIEND_LIST, FRIEND_LIST_SEQ_NAME_CANCEL);
                data->seq = CODE_IN_SEQ_NAME_END;
            }
        }
        break;
    case CODE_IN_SEQ_NAME_END:
        func_ov010_0216c0c0(codeIn->nameParam);
        GFL_OvlLoad(OVERLAY_APP_UI);
        func_ov128_021c6360(wk, heapId);
        return TRUE;
    }
    return FALSE;
}

// The name entry of a friend, with the friend's trainer if the player has met them
static NameEntryParam *func_ov128_021c8da0(WifiNoteCodeIn *codeIn, WifiNoteData *data, BOOL isNew,
                                                      HeapID heapId) {
    TrainerGameInfoSave *gameInfo = getTrainerGameInfoAddress(GameData_GetSaveControl(data->gameData));
    WifiList *wifiList = func_0201283c(data->gameData);
    u32 index = data->friends.index[data->friendPos];
    u32 gender = func_02009864(wifiList, index, WIFI_LIST_FRIEND_GENDER);
    NameEntryParam *param;

    if (isNew == TRUE || gender == WIFI_LIST_GENDER_UNKNOWN) {
        param = setupNameEntry(heapId, NAME_ENTRY_FRIEND, 0xffff, 0, 7, NULL, gameInfo);
    } else {
        param = setupNameEntry(heapId, NAME_ENTRY_FRIEND, func_02009864(wifiList, index, WIFI_LIST_FRIEND_TRAINER_VIEW),
                               0, 7, NULL, gameInfo);
    }
    if (data->hasOldName == TRUE) {
        GFL_StrBufLoadString(param->name, data->oldName);
        data->hasOldName = FALSE;
    }
    return param;
}

// pokebw2's WifiNoteMyCode_Init
static void func_ov128_021c8e24(WifiNoteMyCode *myCode, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId) {
    func_ov128_021c8f88(myCode, data, draw, heapId);
}

// pokebw2's WifiNoteMyCode_Main
static BOOL func_ov128_021c8e2c(WifiNoteMyCode *myCode, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId) {
    switch (data->seq) {
    case MY_CODE_SEQ_INIT:
        func_ov128_021c8e24(myCode, data, draw, HEAPID_WIFI_NOTE);
        func_ov128_021c9180(myCode, data, draw);
        data->seq = MY_CODE_SEQ_MAIN;
        break;
    case MY_CODE_SEQ_MAIN: {
        s32 input = func_ov128_021c8f2c(myCode, data);

        if (input == 0) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov128_021ca3a0(draw);
            data->seq = MY_CODE_SEQ_RETURN;
        } else if (input == 1) {
            GFL_SndSEPlay(SEQ_SE_CLOSE1);
            func_ov128_021ca3c4(draw);
            data->seq = MY_CODE_SEQ_EXIT;
        }
        break;
    }
    case MY_CODE_SEQ_RETURN:
        if (func_0204b98c(draw->returnButton) != TRUE) {
            func_ov128_021c6438(data, WIFI_NOTE_MODE_SELECT, MODE_SELECT_SEQ_INIT_NO_FADE);
            func_ov128_021c91cc(myCode, draw);
            func_ov128_021c8f24(myCode, data, draw);
            return TRUE;
        }
        break;
    case MY_CODE_SEQ_EXIT:
        if (func_0204b98c(draw->exitButton) != TRUE) {
            data->seq = MY_CODE_SEQ_FADE_OUT;
        }
        break;
    case MY_CODE_SEQ_FADE_OUT:
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, heapId);
        data->seq = MY_CODE_SEQ_FADE_OUT_WAIT;
        break;
    case MY_CODE_SEQ_FADE_OUT_WAIT:
        if (GFL_WipeIsFinished() == TRUE) {
            func_ov128_021c6438(data, WIFI_NOTE_MODE_EXIT, 0);
            func_ov128_021c8f24(myCode, data, draw);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's WifiNoteMyCode_Exit
static void func_ov128_021c8f24(WifiNoteMyCode *myCode, WifiNoteData *data, WifiNoteDraw *draw) {
    func_ov128_021c9154(myCode, data, draw);
}

// Returns 0 for the return button and 1 for the X button, or -1
// pokebw2's WifiNoteMyCode_Input
static s32 func_ov128_021c8f2c(WifiNoteMyCode *myCode, WifiNoteData *data) {
    s32 hit = func_02035c08(data_ov128_021ca529);

    if (hit != TOUCH_RECT_NONE) {
        func_020355d8(TRUE);
        data->touch = TRUE;
        return hit;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        func_020355d8(FALSE);
        data->touch = FALSE;
        return 0;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
        func_020355d8(FALSE);
        data->touch = FALSE;
        return 1;
    }
    return TOUCH_RECT_NONE;
}

// Prints a Friend Code's twelve digits from the right, one digit to a tile, in groups of four
static inline void WifiNoteMyCode_PrintCode(PrintWindow *window, StrBuf *str, u64 code, PrintQueue *queue, Font *font) {
    u16 digits[10] = { L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9' };
    int gap = 0;
    u8 width = func_02045738(window->window);
    int i;

    for (i = 1; i <= 12; i++) {
        GFL_StrBufClear(str);
        GFL_StrBufAppend(str, digits[code % 10]);
        func_ov067_021b6ae4(window, queue, (width - i) * 8 - gap + 1, 0, str, font, WIFI_NOTE_COLOR_WHITE,
                        PRINT_ALIGN_LEFT);
        if (i % 4 == 0) {
            gap += 8;
        }
        code /= 10;
    }
}

// pokebw2's WifiNoteMyCode_InitDraw
static void func_ov128_021c8f88(WifiNoteMyCode *myCode, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId) {
    StrBuf *str;
    StrBuf *temp;
    u64 code;

    func_ov128_021c63ac(draw, &myCode->msgWindow, 3, 2, 10, 30, 8, 10, 1, 0);
    func_ov128_021c63ac(draw, &myCode->codeWindow, 3, 9, 6, 14, 2, 10, 241, 0);
    str = GFL_StrBufCreate(192, heapId);
    temp = GFL_StrBufCreate(128, heapId);
    code = func_0205994c(func_02009860(func_0201283c(data->gameData)));
    if (code != 0) {
        GFL_MsgDataLoadStrbuf(draw->msgData, Bank0418_Text_053, str);
        func_ov067_021b6ae4(&myCode->msgWindow, draw->queue, 4, 0, str, draw->font, WIFI_NOTE_COLOR_BLACK,
                        PRINT_ALIGN_LEFT);
        func_ov128_021ca4b4(myCode->msgWindow.window);
        WifiNoteMyCode_PrintCode(&myCode->codeWindow, str, code, draw->queue, draw->font);
        func_ov128_021ca4b4(myCode->codeWindow.window);
    } else {
        GFL_MsgDataLoadStrbuf(draw->msgData, Bank0418_Text_054, str);
        func_ov067_021b6ae4(&myCode->msgWindow, draw->queue, 4, 0, str, draw->font, WIFI_NOTE_COLOR_BLACK,
                        PRINT_ALIGN_LEFT);
        func_ov128_021ca4b4(myCode->msgWindow.window);
    }
    GFL_StrBufFree(str);
    GFL_StrBufFree(temp);
    myCode->screenBuffer = GFL_G2DIOReadNSCRArc(draw->arc, 6, FALSE, &myCode->screen, heapId);
}

// pokebw2's WifiNoteMyCode_ExitDraw
static void func_ov128_021c9154(WifiNoteMyCode *myCode, WifiNoteData *data, WifiNoteDraw *draw) {
    func_ov128_021ca444(draw);
    func_ov128_021ca458(draw, &draw->title);
    BmpWin_Free(myCode->msgWindow.window);
    BmpWin_Free(myCode->codeWindow.window);
    GFL_HeapFree(myCode->screenBuffer);
}

// pokebw2's WifiNoteMyCode_Show
static void func_ov128_021c9180(WifiNoteMyCode *myCode, WifiNoteData *data, WifiNoteDraw *draw) {
    copyVarForText(draw->wordSet, 0, GetGameDataPlayerInfo(data->gameData));
    func_ov128_021c6adc(draw, &draw->title, Bank0418_Text_050);
    WifiNote_LoadScreen(2, myCode->screen);
    GFL_BGSysQueueScrLoad(2);
}

// pokebw2's WifiNoteMyCode_Hide
static void func_ov128_021c91cc(WifiNoteMyCode *myCode, WifiNoteDraw *draw) {
    func_ov128_021c6b38(draw);
    GFL_BGSysFillScrArea(2, 0, 0, 0, 32, 24, 0);
    GFL_BGSysQueueScrLoad(2);
    WifiNote_HideWindow(myCode->msgWindow.window);
    WifiNote_HideWindow(myCode->codeWindow.window);
    GFL_BGSysFillScrArea(3, 0, 0, 0, 32, 24, 0);
    GFL_BGSysQueueScrLoad(3);
}

// pokebw2's WifiNoteFriendInfo_Init
static void func_ov128_021c9238(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId) {
    info->unk10 = 1;
    func_ov128_021c9628(info, data, draw, heapId);
}

// pokebw2's WifiNoteFriendInfo_Main
static BOOL func_ov128_021c9244(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId) {
    switch (data->seq) {
    case FRIEND_INFO_SEQ_INIT:
        info->moved = FALSE;
        info->page = 0;
        // fallthrough
    case FRIEND_INFO_SEQ_INIT_NEXT:
        func_ov128_021c9238(info, data, draw, heapId);
        func_ov128_021c97a0(info, data, draw, heapId);
        data->seq = FRIEND_INFO_SEQ_MAIN;
        break;
    case FRIEND_INFO_SEQ_MAIN:
        data->seq = func_ov128_021c9544(info, data, draw);
        break;
    case FRIEND_INFO_SEQ_SCROLL_START:
        func_ov128_021c958c(info, data, draw, heapId);
        func_ov128_021c95cc(info, data, draw, heapId);
        data->seq = FRIEND_INFO_SEQ_SCROLL;
        break;
    case FRIEND_INFO_SEQ_SCROLL:
        if (func_ov128_021c95cc(info, data, draw, heapId) == TRUE) {
            data->seq = FRIEND_INFO_SEQ_SCROLL_END;
        }
        break;
    case FRIEND_INFO_SEQ_SCROLL_END:
        if (func_0201c6b4(draw->queue) == TRUE) {
            GFL_BGSysMoveBGReq(2, 0, 0);
            GFL_BGSysMoveBGReq(3, 0, 0);
            data->seq = FRIEND_INFO_SEQ_MAIN;
        }
        break;
    case FRIEND_INFO_SEQ_CHANGE:
        // The first change is at once; holding the key repeats it every four frames
        if (info->moved) {
            info->wait--;
            if (info->wait != 0) {
                break;
            }
        }
        if (func_ov128_021c9d0c(data, info->scrollDir)) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov128_021c98ec(info, draw);
            func_ov128_021c6438(data, WIFI_NOTE_MODE_FRIEND_INFO, FRIEND_INFO_SEQ_INIT_NEXT);
            info->moved = TRUE;
            func_ov128_021c9730(info, data, draw);
            return TRUE;
        }
        data->seq = FRIEND_INFO_SEQ_MAIN;
        break;
    case FRIEND_INFO_SEQ_RETURN:
        if (func_0204b98c(draw->returnButton) != TRUE) {
            func_ov128_021c987c(info, draw);
            func_ov128_021c6438(data, WIFI_NOTE_MODE_FRIEND_LIST, FRIEND_LIST_SEQ_INFO_RETURN);
            func_ov128_021c9730(info, data, draw);
            return TRUE;
        }
        break;
    case FRIEND_INFO_SEQ_EXIT:
        if (func_0204b98c(draw->exitButton) != TRUE) {
            data->seq = FRIEND_INFO_SEQ_FADE_OUT;
        }
        break;
    case FRIEND_INFO_SEQ_FADE_OUT:
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, heapId);
        data->seq = FRIEND_INFO_SEQ_FADE_OUT_WAIT;
        break;
    case FRIEND_INFO_SEQ_FADE_OUT_WAIT:
        if (GFL_WipeIsFinished()) {
            func_ov128_021c6438(data, WIFI_NOTE_MODE_EXIT, 0);
            func_ov128_021c9730(info, data, draw);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// Returns the sequence that an input goes to, with its sound
// pokebw2's WifiNoteFriendInfo_Decide
static u32 func_ov128_021c93c0(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw, u32 input) {
    switch (input) {
    case FRIEND_INFO_INPUT_RETURN:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov128_021ca3a0(draw);
        return FRIEND_INFO_SEQ_RETURN;
    case FRIEND_INFO_INPUT_EXIT:
        GFL_SndSEPlay(SEQ_SE_CLOSE1);
        func_ov128_021ca3c4(draw);
        return FRIEND_INFO_SEQ_EXIT;
    case FRIEND_INFO_INPUT_PREV:
        if (func_ov128_021c9d88(data, 0) == TRUE) {
            info->wait = 4;
            info->scrollDir = 0;
            func_ov128_021c6e18(draw, 1, 11);
            return FRIEND_INFO_SEQ_CHANGE;
        }
        break;
    case FRIEND_INFO_INPUT_NEXT:
        if (func_ov128_021c9d88(data, 1) == TRUE) {
            info->wait = 4;
            info->scrollDir = 1;
            func_ov128_021c6e18(draw, 0, 10);
            return FRIEND_INFO_SEQ_CHANGE;
        }
        break;
    }
    return FRIEND_INFO_SEQ_MAIN;
}

// pokebw2's WifiNoteFriendInfo_Keys
static u32 func_ov128_021c9450(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw) {
    if (data->touch == TRUE) {
        if (GCTX_HIDGetPressedKeys() & WIFI_NOTE_KEYS) {
            func_020355d8(FALSE);
            data->touch = FALSE;
        }
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        return func_ov128_021c93c0(info, data, draw, FRIEND_INFO_INPUT_RETURN);
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
        return func_ov128_021c93c0(info, data, draw, FRIEND_INFO_INPUT_EXIT);
    }
    if (GCTX_HIDGetHeldKeys() & PAD_KEY_UP) {
        return func_ov128_021c93c0(info, data, draw, FRIEND_INFO_INPUT_PREV);
    }
    if (GCTX_HIDGetHeldKeys() & PAD_KEY_DOWN) {
        return func_ov128_021c93c0(info, data, draw, FRIEND_INFO_INPUT_NEXT);
    }
    return FRIEND_INFO_SEQ_MAIN;
}

// pokebw2's WifiNoteFriendInfo_Touch
static u32 func_ov128_021c94dc(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw, BOOL *touched) {
    s32 hit = func_02035c08(data_ov128_021ca576);

    if (hit == TOUCH_RECT_NONE) {
        return FRIEND_INFO_SEQ_MAIN;
    }
    *touched = TRUE;
    if (hit == 0) {
        return func_ov128_021c93c0(info, data, draw, FRIEND_INFO_INPUT_NEXT);
    }
    if (hit == 1) {
        return func_ov128_021c93c0(info, data, draw, FRIEND_INFO_INPUT_PREV);
    }
    if (hit == 2) {
        return func_ov128_021c93c0(info, data, draw, FRIEND_INFO_INPUT_RETURN);
    }
    return func_ov128_021c93c0(info, data, draw, FRIEND_INFO_INPUT_EXIT);
}

// pokebw2's WifiNoteFriendInfo_Input
static u32 func_ov128_021c9544(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw) {
    BOOL touched = FALSE;
    u32 seq;

    if (!func_0201c6b4(draw->queue)) {
        return FRIEND_INFO_SEQ_MAIN;
    }
    seq = func_ov128_021c94dc(info, data, draw, &touched);
    if (touched) {
        func_020355d8(TRUE);
        data->touch = TRUE;
        return seq;
    }
    return func_ov128_021c9450(info, data, draw);
}

static void func_ov128_021c958c(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw,
                                           HeapID heapId) {
    u32 area;

    info->scrollCount = 0;
    func_ov128_021c6df8(draw, FALSE);
    if (info->scrollDir == 2) {
        area = 2;
    } else {
        area = 1;
    }
    func_ov128_021c9bec(info, data, draw, area, info->page, heapId);
    GFL_SndSEPlay(SEQ_SE_SELECT1);
}

// pokebw2's WifiNoteFriendInfo_Scroll
static BOOL func_ov128_021c95cc(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId) {
    s32 scroll;

    if (info->scrollCount > FRIEND_INFO_SCROLL_FRAMES) {
        func_ov128_021c9bec(info, data, draw, 0, info->page, heapId);
        func_ov128_021c6df8(draw, TRUE);
        func_ov128_021c9bf8(info, draw, 0xff);
        return TRUE;
    }
    scroll = info->scrollCount * 256 / FRIEND_INFO_SCROLL_FRAMES;
    if (info->scrollDir == 2) {
        scroll = -scroll;
    }
    GFL_BGSysMoveBG(3, 0, scroll);
    info->scrollCount++;
    return FALSE;
}

static void func_ov128_021c9628(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw,
                                        HeapID heapId) {
    int i;
    int j;

    func_ov128_021c9e10(info->screens, draw, heapId);
    func_ov128_021c9e6c(info, draw);
    for (i = 0; i < 2; i++) {
        func_ov128_021c63ac(draw, &info->areaWindows[i], 3, MATH_ABS(data_ov128_021ca564[i].x) + 2,
                              MATH_ABS(data_ov128_021ca564[i].y) + 1, 26, 18, 10, 0, 0);
    }
    for (i = 0; i < FRIEND_INFO_AREA_COUNT; i++) {
        func_ov128_021c9ed0(&info->areas[i], draw, &data_ov128_021ca564[i], data_ov128_021ca538[i],
                              heapId);
        for (j = 0; j < 8; j++) {
            func_ov128_021c9ef4(&info->areas[i], draw, j, data_ov128_021ca521[j],
                                        data_ov128_021ca7e0[j], heapId);
            info->areas[0].windows[j] = &info->areaWindows[0];
            info->areas[1].windows[j] = &info->areaWindows[1];
            info->areas[2].windows[j] = &info->areaWindows[1];
        }
    }
}

// pokebw2's WifiNoteFriendInfo_ExitDraw
static void func_ov128_021c9730(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw) {
    int i;
    int j;

    func_ov128_021ca444(draw);
    func_ov128_021ca458(draw, &draw->title);
    for (i = 0; i < FRIEND_INFO_AREA_COUNT; i++) {
        for (j = 0; j < 8; j++) {
            func_ov128_021c9efc(&info->areas[i], j);
        }
        func_ov128_021c9ef0(&info->areas[i]);
    }
    for (i = 0; i < 2; i++) {
        BmpWin_Free(info->areaWindows[i].window);
    }
    func_ov128_021c9eb4(info, draw);
    func_ov128_021c9e54(info->screens);
}

// pokebw2's WifiNoteFriendInfo_Show
static void func_ov128_021c97a0(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId) {
    func_ov128_021c6bcc(draw, data, data->friends.index[data->friendPos], heapId);
    func_ov128_021c6adc(draw, &draw->title, Bank0418_Text_013);
    WifiNote_LoadScreen(0, info->screens[0].screen);
    GFL_BGSysQueueScrLoad(0);
    WifiNote_LoadScreen(2, info->screens[1].screen);
    GFL_BGSysQueueScrLoad(2);
    func_ov128_021c991c(info, data, draw, heapId);
    func_ov128_021c9bec(info, data, draw, 0, info->page, heapId);
    // The arrows are gray when there is no other friend with a record
    if (func_ov128_021c9d88(data, 0) == TRUE) {
        func_ov128_021c6e18(draw, 1, 3);
        func_ov128_021c6e18(draw, 0, 2);
    } else {
        func_ov128_021c6e18(draw, 1, 17);
        func_ov128_021c6e18(draw, 0, 16);
    }
    func_ov128_021c6dd8(draw, TRUE);
}

// pokebw2's WifiNoteFriendInfo_Hide
static void func_ov128_021c987c(WifiNoteFriendInfo *info, WifiNoteDraw *draw) {
    func_ov128_021c6dd8(draw, FALSE);
    func_ov128_021c9ccc(info, draw);
    func_ov128_021c9bc4(info);
    GFL_BGSysFillScrArea(2, 0, 0, 0, 32, 32, 0);
    GFL_BGSysQueueScrLoad(2);
    WifiNote_LoadScreen(0, info->screens[2].screen);
    GFL_BGSysQueueScrLoad(0);
    func_ov128_021c6b38(draw);
    func_ov128_021ca4c8(info);
}

// pokebw2's WifiNoteFriendInfo_HideForNext
static void func_ov128_021c98ec(WifiNoteFriendInfo *info, WifiNoteDraw *draw) {
    int i;

    for (i = 0; i < FRIEND_INFO_AREA_COUNT; i++) {
        func_ov128_021c9f6c(&info->areas[i], draw);
    }
    func_ov128_021c6b38(draw);
    func_ov128_021ca4c8(info);
}

// pokebw2's WifiNoteFriendInfo_Draw
static void func_ov128_021c991c(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw, HeapID heapId) {
    WifiList *wifiList = func_0201283c(data->gameData);
    u32 i;

    func_02009864(wifiList, data->friends.index[data->friendPos], WIFI_LIST_FRIEND_GENDER);
    for (i = 0; i < FRIEND_INFO_WINDOW_COUNT; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(info->windows[i].window), 0);
    }
    func_ov067_021b6b2c(&info->windows[0], draw->queue, draw->font, BmpWin_GetWidth1(info->windows[0].window) * 8 / 2, 0,
                  WIFI_NOTE_COLOR_WHITE, data->friends.number[data->friendPos] + 1, data->friends.metCount, heapId);
    func_ov128_021c9fc8(&info->windows[1], data, draw, Bank0418_Text_019, 0, 0, WIFI_NOTE_COLOR_WHITE);
    func_ov128_021c9fc8(&info->windows[2], data, draw, Bank0418_Text_020, 0, 0, WIFI_NOTE_COLOR_WHITE);
    func_ov128_021c9fc8(&info->windows[3], data, draw, Bank0418_Text_021, 0, 0, WIFI_NOTE_COLOR_WHITE);
    func_ov128_021c6c94(draw, func_02009864(wifiList, data->friends.index[data->friendPos], WIFI_LIST_FRIEND_WINS));
    func_ov128_021c9fc8(&info->windows[4], data, draw, Bank0418_Text_022, 0, 0, WIFI_NOTE_COLOR_WHITE);
    func_ov128_021c6c94(draw,
                           func_02009864(wifiList, data->friends.index[data->friendPos], WIFI_LIST_FRIEND_LOSSES));
    func_ov128_021c9fc8(&info->windows[5], data, draw, Bank0418_Text_022, 0, 0, WIFI_NOTE_COLOR_WHITE);
    func_ov128_021c9fc8(&info->windows[6], data, draw, Bank0418_Text_023, 0, 0, WIFI_NOTE_COLOR_WHITE);
    func_ov128_021c6c94(draw, func_02009864(wifiList, data->friends.index[data->friendPos], WIFI_LIST_FRIEND_DRAWS));
    func_ov128_021c9fc8(&info->windows[7], data, draw, Bank0418_Text_024, 0, 0, WIFI_NOTE_COLOR_WHITE);
    func_ov128_021c9fc8(&info->windows[8], data, draw, Bank0418_Text_027, 0, 0, WIFI_NOTE_COLOR_WHITE);
    if (func_ov128_021c6c20(draw, data, data->friends.index[data->friendPos])) {
        func_ov128_021c9fc8(&info->windows[9], data, draw, Bank0418_Text_028, 0, 0, WIFI_NOTE_COLOR_WHITE);
    } else {
        BmpWin_FlushChar(info->windows[9].window);
    }
    for (i = 0; i < FRIEND_INFO_WINDOW_COUNT; i++) {
        func_ov128_021ca4b4(info->windows[i].window);
    }
    {
        ClActorSetup setup = { 56, 80, 0, 0, 0 };
        u32 trainerClass =
            func_02025c88(func_02009864(wifiList, data->friends.index[data->friendPos], WIFI_LIST_FRIEND_TRAINER_VIEW));
        ArcTool *arc = func_0202da04(heapId);

        info->trainerChars = func_0202da48(arc, trainerClass, CLACT_VRAM_MAIN, heapId);
        info->trainerPalette = func_0202da18(arc, trainerClass, CLACT_VRAM_MAIN, 0xc0, heapId);
        info->trainerCellAnims = func_0202da6c(trainerClass, 2, CLACT_VRAM_MAIN, heapId);
        GFL_ArcToolFree(arc);
        info->trainer = func_0204b294(draw->unit, info->trainerChars, info->trainerPalette, info->trainerCellAnims,
                                      &setup, 0, heapId);
    }
}

// pokebw2's WifiNoteFriendInfo_HideWindows
static void func_ov128_021c9bc4(WifiNoteFriendInfo *info) {
    int i;

    for (i = 0; i < FRIEND_INFO_WINDOW_COUNT; i++) {
        WifiNote_HideWindow(info->windows[i].window);
    }
}

// With area 0, the page shown, the page becomes the current one
static void func_ov128_021c9bec(WifiNoteFriendInfo *info, WifiNoteData *data, WifiNoteDraw *draw, u32 area,
                                       s32 page, HeapID heapId) {
    if (area == 0) {
        info->page = page;
    }
}

// Draws the row of the record's tabs, and a tab over it as the current one unless it is 0xff
// pokebw2's WifiNoteFriendInfo_DrawTabs
static void func_ov128_021c9bf8(WifiNoteFriendInfo *info, WifiNoteDraw *draw, u32 tab) {
    if (info->unk10 == 1) {
        GFL_BGSysLoadScrArea(2, 0, 20, 32, 4, draw->screens.screen[1]->rawData, 0, 12,
                             draw->screens.screen[1]->screenWidth / 8, draw->screens.screen[1]->screenHeight / 8);
    } else {
        GFL_BGSysLoadScrArea(2, 0, 20, 32, 4, draw->screens.screen[1]->rawData, 0, 20,
                             draw->screens.screen[1]->screenWidth / 8, draw->screens.screen[1]->screenHeight / 8);
    }
    if (tab == 0xff) {
        GFL_BGSysQueueScrLoad(2);
        return;
    }
    GFL_BGSysLoadScrArea(2, data_ov128_021ca544[info->unk10][tab], 20,
                         data_ov128_021ca554[info->unk10][tab], 4, draw->screens.screen[1]->rawData,
                         data_ov128_021ca544[info->unk10][tab], data_ov128_021ca4f7[info->unk10],
                         draw->screens.screen[1]->screenWidth / 8, draw->screens.screen[1]->screenHeight / 8);
    GFL_BGSysQueueScrLoad(2);
}

// pokebw2's WifiNoteFriendInfo_ClearTabs
static void func_ov128_021c9ccc(WifiNoteFriendInfo *info, WifiNoteDraw *draw) {
    GFL_BGSysLoadScrArea(2, 0, 20, 32, 4, draw->screens.screen[1]->rawData, 0, 28, draw->screens.screen[1]->screenWidth / 8,
                         draw->screens.screen[1]->screenHeight / 8);
    GFL_BGSysQueueScrLoad(2);
}

// Moves to the friend with a record before (dir 0) or after the current one, if there is another
// pokebw2's WifiNoteData_MoveFriend
static BOOL func_ov128_021c9d0c(WifiNoteData *data, u32 dir) {
    WifiList *wifiList = func_0201283c(data->gameData);
    int pos = data->friendPos;
    u32 i;

    if (dir == 0) {
        for (i = 0; i < data->friends.count - 1; i++) {
            pos--;
            if (pos < 0) {
                pos += data->friends.count;
            }
            if (func_02009864(wifiList, data->friends.index[pos], WIFI_LIST_FRIEND_GENDER) !=
                WIFI_LIST_GENDER_UNKNOWN) {
                data->friendPos = pos;
                return TRUE;
            }
        }
    } else {
        for (i = 0; i < data->friends.count - 1; i++) {
            pos = (pos + 1) % data->friends.count;
            if (func_02009864(wifiList, data->friends.index[pos], WIFI_LIST_FRIEND_GENDER) !=
                WIFI_LIST_GENDER_UNKNOWN) {
                data->friendPos = pos;
                return TRUE;
            }
        }
    }
    return FALSE;
}

// pokebw2's WifiNoteData_CanMoveFriend
static BOOL func_ov128_021c9d88(WifiNoteData *data, u32 dir) {
    WifiList *wifiList = func_0201283c(data->gameData);
    int pos = data->friendPos;
    u32 i;

    if (dir == 0) {
        for (i = 0; i < data->friends.count - 1; i++) {
            pos--;
            if (pos < 0) {
                pos += data->friends.count;
            }
            if (data->friendPos == pos) {
                break;
            }
            if (func_02009864(wifiList, data->friends.index[pos], WIFI_LIST_FRIEND_GENDER) !=
                WIFI_LIST_GENDER_UNKNOWN) {
                return TRUE;
            }
        }
    } else {
        for (i = 0; i < data->friends.count - 1; i++) {
            pos = (pos + 1) % data->friends.count;
            if (data->friendPos == pos) {
                break;
            }
            if (func_02009864(wifiList, data->friends.index[pos], WIFI_LIST_FRIEND_GENDER) !=
                WIFI_LIST_GENDER_UNKNOWN) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

// pokebw2's WifiNoteFriendInfo_LoadScreens
static void func_ov128_021c9e10(WifiNoteInfoScreen *screens, WifiNoteDraw *draw, HeapID heapId) {
    u16 files[FRIEND_INFO_SCREEN_COUNT] = { 10, 11, 5, 12 };
    int i;

    for (i = 0; i < FRIEND_INFO_SCREEN_COUNT; i++) {
        screens->buffer = GFL_G2DIOReadNSCRArc(draw->arc, files[i], FALSE, &screens->screen, heapId);
        screens++;
    }
}

// pokebw2's WifiNoteFriendInfo_FreeScreens
static void func_ov128_021c9e54(WifiNoteInfoScreen *screens) {
    int i;

    for (i = 0; i < FRIEND_INFO_SCREEN_COUNT; i++) {
        GFL_HeapFree(screens->buffer);
        screens++;
    }
}

// pokebw2's WifiNoteFriendInfo_CreateWindows
static void func_ov128_021c9e6c(WifiNoteFriendInfo *info, WifiNoteDraw *draw) {
    int i;

    for (i = 0; i < FRIEND_INFO_WINDOW_COUNT; i++) {
        func_ov128_021c63ac(draw, &info->windows[i], 1, data_ov128_021ca5a2[i].x,
                              data_ov128_021ca5a2[i].y, data_ov128_021ca5a2[i].width,
                              data_ov128_021ca5a2[i].height, 10, 0, 0);
    }
}

// pokebw2's WifiNoteFriendInfo_FreeWindows
static void func_ov128_021c9eb4(WifiNoteFriendInfo *info, WifiNoteDraw *draw) {
    int i;

    for (i = 0; i < FRIEND_INFO_WINDOW_COUNT; i++) {
        BmpWin_Free(info->windows[i].window);
    }
}

static void func_ov128_021c9ed0(WifiNoteInfoArea *area, WifiNoteDraw *draw, const WifiNoteScreenArea *screenArea,
                                  u32 charOffset, HeapID heapId) {
    int i;

    area->area = *screenArea;
    area->charOffset = charOffset;
    for (i = 0; i < 8; i++) {
        area->windowCounts[i] = 0;
    }
}

// pokebw2's WifiNoteInfoArea_Exit
static void func_ov128_021c9ef0(WifiNoteInfoArea *area) {
}

static void func_ov128_021c9ef4(WifiNoteInfoArea *area, WifiNoteDraw *draw, u32 index, u32 count,
                                        const WifiNoteWindowRect *rects, HeapID heapId) {
    area->windowCounts[index] = count;
}

// pokebw2's WifiNoteInfoArea_ResetWindows
static void func_ov128_021c9efc(WifiNoteInfoArea *area, u32 index) {
    if (area->windows[index] != NULL) {
        u8 i;

        for (i = 0; i < area->windowCounts[index]; i++) {
        }
        area->windows[index] = NULL;
        area->windowCounts[index] = 0;
    }
}

// pokebw2's WifiNoteInfoArea_HideWindows
static void func_ov128_021c9f30(WifiNoteInfoArea *area, u32 index) {
    u32 i;

    for (i = 0; i < area->windowCounts[index]; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(area->windows[index]->window), 0);
        WifiNote_HideWindow(area->windows[index]->window);
    }
}

// pokebw2's WifiNoteInfoArea_Hide
static void func_ov128_021c9f6c(WifiNoteInfoArea *area, WifiNoteDraw *draw) {
    int i;

    for (i = 0; i < 8; i++) {
        func_ov128_021c9f30(area, i);
    }
    func_ov128_021c9f8c(draw, &area->area);
}

// pokebw2's WifiNote_ClearScreenArea
static void func_ov128_021c9f8c(WifiNoteDraw *draw, const WifiNoteScreenArea *area) {
    GFL_BGSysFillScrArea(3, 0, MATH_ABS(area->x), MATH_ABS(area->y), 32, 24, 0);
    GFL_BGSysQueueScrLoad(3);
}

static void func_ov128_021c9fc8(PrintWindow *window, WifiNoteData *data, WifiNoteDraw *draw, u32 msg, u32 x, u32 y,
                                     u32 color) {
    GFL_MsgDataLoadStrbuf(draw->msgData, msg, draw->temp);
    GFL_WordSetFormatStrbuf(draw->wordSet, draw->str, draw->temp);
    func_ov067_021b6ae4(window, draw->queue, x, y, draw->str, draw->font, color, PRINT_ALIGN_LEFT);
}

// Reads a Friend Code of ASCII or full-width digits. valid is whether the whole string was read
// pokebw2's WifiNote_StrToFriendKey
static u64 func_ov128_021ca014(const StrBuf *str, BOOL *valid) {
    u16 terminator = GFL_StrBufGetTerminator();
    const u16 *chars = GFL_StrBufGetStringPtr(str);
    u64 key = 0;
    u16 i;

    *valid = FALSE;
    for (i = 0; chars[i] != terminator; i++) {
        u32 digit;

        if (i >= sWifiNoteFriendKeyDigitsMax[0]) {
            return key;
        }
        digit = (u16)(chars[i] - L'0');
        if (digit >= 10) {
            digit = (u16)(chars[i] - 0xff10);
            if (digit >= 10) {
                return key;
            }
        }
        key *= 10;
        key += digit;
    }
    *valid = TRUE;
    return key;
}

// Whether the player touched the screen or pressed A or B to go on
// pokebw2's WifiNote_IsNextPressed
static BOOL func_ov128_021ca084(WifiNoteData *data) {
    if (func_02035c60() == TRUE) {
        func_020355d8(TRUE);
        data->touch = TRUE;
        return TRUE;
    }
    if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        func_020355d8(FALSE);
        data->touch = FALSE;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiNoteDraw_InitBlink
static void func_ov128_021ca0bc(WifiNoteDraw *draw, HeapID heapId) {
    draw->blink[0] = func_0202e940(0x30, 16, 0, heapId);
    func_0202e9ac(draw->blink[0], draw->arc, 0, 0x20, 0x30);
    draw->blink[1] = func_0202e940(0x10, 16, 0xfffe, heapId);
    func_0202e9ac(draw->blink[1], draw->arc, 2, 0, 0x10);
}

// pokebw2's WifiNoteDraw_ExitBlink
static void func_ov128_021ca10c(WifiNoteDraw *draw) {
    func_0202ea20(draw->blink[1]);
    func_0202ea20(draw->blink[0]);
}

// pokebw2's WifiNoteModeSelect_SetButtonPalette
static void func_ov128_021ca124(u32 button, u32 palette) {
    GFL_BGSysSetScrPaletteNo(2, 2, (button + 1) * 5, 28, 4, palette);
    GFL_BGSysQueueScrLoad(2);
}

// pokebw2's WifiNoteModeSelect_ClearCursor
static void func_ov128_021ca154(WifiNoteDraw *draw, u32 button) {
    func_ov128_021ca124(button, 1);
    func_0202eb04(draw->blink[0]);
}

// pokebw2's WifiNoteModeSelect_SetCursor
static void func_ov128_021ca16c(WifiNoteDraw *draw, u32 button) {
    func_ov128_021ca124(button, 3);
    func_0202eb04(draw->blink[0]);
}

// pokebw2's WifiNoteModeSelect_MoveCursor
static void func_ov128_021ca184(WifiNoteDraw *draw, u32 button, u32 prevButton) {
    func_ov128_021ca154(draw, prevButton);
    func_ov128_021ca16c(draw, button);
}

// No button has the cursor while the player uses the touch screen
// pokebw2's WifiNoteModeSelect_DrawCursor
static void func_ov128_021ca19c(WifiNoteModeSelect *select) {
    u32 i;

    if (func_020355c4() == TRUE) {
        for (i = 0; i < 3; i++) {
            func_ov128_021ca124(i, 1);
        }
    } else {
        for (i = 0; i < 3; i++) {
            if (i == select->cursor) {
                func_ov128_021ca124(i, 3);
            } else {
                func_ov128_021ca124(i, 1);
            }
        }
    }
}

// Flashes the picked button twice. Returns FALSE once it is over
// pokebw2's WifiNoteModeSelect_UpdateButton
static BOOL func_ov128_021ca1e0(WifiNoteModeSelect *select) {
    switch (select->buttonSeq) {
    case 0:
        func_ov128_021ca124(select->cursor, 1);
        select->buttonCount = 0;
        select->buttonSeq++;
        break;
    case 1:
        select->buttonCount++;
        if (select->buttonCount == 4) {
            func_ov128_021ca124(select->cursor, 2);
            select->buttonCount = 0;
            select->buttonSeq++;
        }
        break;
    case 2:
        select->buttonCount++;
        if (select->buttonCount == 4) {
            func_ov128_021ca124(select->cursor, 1);
            select->buttonCount = 0;
            select->buttonSeq++;
        }
        break;
    case 3:
        select->buttonCount++;
        if (select->buttonCount == 4) {
            func_ov128_021ca124(select->cursor, 2);
            select->buttonCount = 0;
            select->buttonSeq++;
        }
        break;
    case 4:
        select->buttonCount++;
        if (select->buttonCount == 4) {
            select->buttonSeq = 0;
            select->buttonCount = 0;
            return FALSE;
        }
        break;
    }
    return TRUE;
}

// pokebw2's WifiNoteFriendList_SetButtonPalette
static void func_ov128_021ca25c(u32 pos, u32 palette) {
    GFL_BGSysSetScrPaletteNo(2, pos / 4 * 14 + 2, pos % 4 * 4 + 4, 14, 4, palette);
    GFL_BGSysQueueScrLoad(2);
}

// pokebw2's WifiNoteFriendList_ClearCursor
static void func_ov128_021ca294(WifiNoteDraw *draw, u32 pos) {
    func_ov128_021ca25c(pos, 1);
    func_0202eb04(draw->blink[0]);
}

// pokebw2's WifiNoteFriendList_SetCursor
static void func_ov128_021ca2ac(WifiNoteDraw *draw, u32 pos) {
    func_ov128_021ca25c(pos, 3);
    func_0202eb04(draw->blink[0]);
}

// pokebw2's WifiNoteFriendList_MoveCursor
static void func_ov128_021ca2c4(WifiNoteDraw *draw, u32 pos, u32 prevPos) {
    func_ov128_021ca294(draw, prevPos);
    func_ov128_021ca2ac(draw, pos);
}

// pokebw2's WifiNoteFriendList_DrawCursor
static void func_ov128_021ca2dc(WifiNoteFriendList *list) {
    u32 i;

    if (func_020355c4() == TRUE) {
        for (i = 0; i < FRIEND_LIST_PAGE_SIZE; i++) {
            func_ov128_021ca25c(i, 1);
        }
    } else {
        for (i = 0; i < FRIEND_LIST_PAGE_SIZE; i++) {
            if (i == list->pos) {
                func_ov128_021ca25c(i, 3);
            } else {
                func_ov128_021ca25c(i, 1);
            }
        }
    }
}

// pokebw2's WifiNoteFriendList_UpdateButton
static BOOL func_ov128_021ca320(WifiNoteFriendList *list) {
    switch (list->buttonSeq) {
    case 0:
        func_ov128_021ca25c(list->pos, 1);
        list->buttonCount = 0;
        list->buttonSeq++;
        break;
    case 1:
        list->buttonCount++;
        if (list->buttonCount == 4) {
            func_ov128_021ca25c(list->pos, 2);
            list->buttonCount = 0;
            list->buttonSeq++;
        }
        break;
    case 2:
        list->buttonCount++;
        if (list->buttonCount == 4) {
            func_ov128_021ca25c(list->pos, 1);
            list->buttonCount = 0;
            list->buttonSeq++;
        }
        break;
    case 3:
        list->buttonCount++;
        if (list->buttonCount == 4) {
            func_ov128_021ca25c(list->pos, 2);
            list->buttonCount = 0;
            list->buttonSeq++;
        }
        break;
    case 4:
        list->buttonCount++;
        if (list->buttonCount == 4) {
            list->buttonSeq = 0;
            list->buttonCount = 0;
            return FALSE;
        }
        break;
    }
    return TRUE;
}

// pokebw2's WifiNoteDraw_StartReturnAnim
static void func_ov128_021ca3a0(WifiNoteDraw *draw) {
    func_0204b8c4(draw->returnButton, 0);
    func_0204b878(draw->returnButton, 9);
    func_0204b92c(draw->returnButton, TRUE);
}

// pokebw2's WifiNoteDraw_StartExitAnim
static void func_ov128_021ca3c4(WifiNoteDraw *draw) {
    func_0204b8c4(draw->exitButton, 0);
    func_0204b878(draw->exitButton, 8);
    func_0204b92c(draw->exitButton, TRUE);
}

// Darkens the screen behind a message, or shows it as it is
// pokebw2's WifiNote_SetBrightness
static void func_ov128_021ca3e8(BOOL normal) {
    if (normal == TRUE) {
        G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, 0);
    } else {
        G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, -8);
    }
}

// pokebw2's WifiNoteFriendList_SetPageCursor
static void func_ov128_021ca40c(WifiNoteFriendList *list, s32 page) {
    func_0204b8c4(list->pageCursor, 0);
    func_0204b878(list->pageCursor, page + 2);
}

// pokebw2's WifiNoteDraw_ShowTitle
static void func_ov128_021ca42c(WifiNoteDraw *draw) {
    BmpWin_FlushMap(draw->title.window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(draw->title.window));
}

// pokebw2's WifiNoteDraw_ClearPrintWindows
static void func_ov128_021ca444(WifiNoteDraw *draw) {
    u32 i;

    for (i = 0; i < WIFI_NOTE_PRINT_WINDOW_MAX; i++) {
        draw->windows[i] = NULL;
    }
}

// pokebw2's WifiNoteDraw_AddPrintWindow
static void func_ov128_021ca458(WifiNoteDraw *draw, PrintWindow *window) {
    u32 i;

    for (i = 0; i < WIFI_NOTE_PRINT_WINDOW_MAX; i++) {
        if (draw->windows[i] == NULL) {
            draw->windows[i] = window;
            return;
        }
    }
}

// pokebw2's WifiNoteDraw_FlushPrintWindows
static void func_ov128_021ca478(WifiNoteDraw *draw) {
    u32 i;

    for (i = 0; i < WIFI_NOTE_PRINT_WINDOW_MAX; i++) {
        if (draw->windows[i] == NULL) {
            break;
        }
        PrintWindow_Flush(draw->windows[i], draw->queue);
    }
}

// pokebw2's WifiNote_ShowWindow
static void func_ov128_021ca4b4(BmpWin *window) {
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
}

// pokebw2's WifiNoteFriendInfo_FreeTrainer
static void func_ov128_021ca4c8(WifiNoteFriendInfo *info) {
    func_0204a8d4(info->trainerChars);
    func_0204ada4(info->trainerPalette);
    func_0204afd8(info->trainerCellAnims);
    func_0204b3b4(info->trainer);
}
