// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov258/waza_oshie_main.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "pml/move_reminder.h"
#include "app/p_status.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
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
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "pml/poke_graphic.h"
#include "pml/poke_party.h"
#include "pml/waza.h"
#include "system/app_keycursor.h"
#include "system/app_menu_common.h"
#include "system/app_taskmenu.h"
#include "system/bmp_menuwork.h"
#include "system/bmp_winframe.h"
#include "system/cursor_move.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "text/system/shinka_demo_btl_main.h"
#include "field/beacon_view.h"

// The move reminder's screen, which the move tutors' scripts show too: the Pokémon and its moves on the top screen,
// and on the touch screen the list of moves to pick from with the picked move's data. Replacing a move goes through
// the summary screen, which ends this screen's graphics and builds them again on return. The ROM has no name for this
// file; "waza_oshie_main.c" is a guess from "waza_oshie.c", the file of overlay 12 that allocates its parameter

#define HEAPID_WAZA_OSHIE 0x4f
// The screen's graphics
#define ARCID_WAZA_OSHIE 124

#define WINDOW_COUNT 16
#define ACTOR_COUNT 14
#define RESOURCE_COUNT 9
#define TYPE_ICON_COUNT 8
#define LIST_ROWS 4
#define MOVE_END 0xffff
// What the info windows are drawn for when the cursor's row has no move
#define NO_MOVE 0xfffffffe

// The windows
#define WINDOW_POWER_LABEL 2
#define WINDOW_ACCURACY_LABEL 3
#define WINDOW_POWER 4
#define WINDOW_ACCURACY 5
#define WINDOW_DESCRIPTION 6
#define WINDOW_MESSAGE 7
#define WINDOW_TEACH 9
#define WINDOW_LIST 11
#define WINDOW_TITLE 13
#define WINDOW_KNOWN_MOVES 14
#define WINDOW_POKEMON 15

// The actors
#define ACTOR_ARROW_DOWN 0
#define ACTOR_ARROW_UP 1
#define ACTOR_CURSOR 2
#define ACTOR_CATEGORY 3
#define ACTOR_LIST_TYPE 4
#define ACTOR_KNOWN_TYPE 8
#define ACTOR_POKEMON 12
#define ACTOR_RETURN 13

// The cursor's positions: the list's rows, then
#define POS_ARROW_DOWN 4
#define POS_ARROW_UP 5
#define POS_RETURN 6
#define POS_TEACH 7
#define POS_QUIT 8

// The buttons shown once a move is picked by touch
#define BUTTON_TEACH 0
#define BUTTON_QUIT 1

// The sequences of the process
enum {
    SEQ_FADE_WAIT,
    SEQ_SELECT,
    SEQ_MESSAGE,
    SEQ_YES_NO_INIT,
    SEQ_YES_NO,
    SEQ_SET_MOVE,
    SEQ_FORGOT,
    SEQ_STATUS_RESULT,
    SEQ_FADE_OUT,
    SEQ_END,
    SEQ_STATUS_FADE_OUT,
    SEQ_STATUS_CALL,
    SEQ_STATUS_RETURN,
    SEQ_DECIDE_WAIT,
    SEQ_QUIT,
    SEQ_MOVE_CHOSEN,
};

// The messages, which index a row of data_ov173_021b8350
enum {
    MSG_SELECT,
    MSG_LEARN,
    MSG_QUIT,
    MSG_LEARNED,
    MSG_REPLACE,
    // "1, 2, and... Ta-da!"
    MSG_FORGET,
    MSG_FORGOT,
    MSG_GIVE_UP,
    MSG_NOT_LEARNED,
    MSG_PAID,
    MSG_FORGET_CONFIRM,
    MESSAGE_COUNT,
};

// The questions, which index data_ov173_021b8228
enum {
    YES_NO_LEARN,
    YES_NO_QUIT,
    YES_NO_REPLACE,
    YES_NO_GIVE_UP,
    YES_NO_FORGET,
};

// What SEQ_DECIDE_WAIT waits for
enum {
    DECIDE_ACTOR,
    DECIDE_BUTTON,
};

// How func_ov173_021b6bf8 places the text in its window
enum {
    ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_CENTER,
};

typedef struct {
    u32 type;
    BOOL dirty;
} WazaOshieTypeIcon;

// What SEQ_DECIDE_WAIT waits for: an actor's animation or a button's flash, and the sequence that follows
// Black 2's; twinned code confirms the offsets of index, seq and type in Black
typedef struct {
    u8 unk00[6];
    u16 index;
    u16 seq;
    u16 type;
} WazaOshieDecide;

// Black 2's; twinned code confirms the offsets of param, windows, msgData, wordSet, strbuf, ppLabel, ppFormat,
// moveList, font, tcbManager, vblankTcb, stream, chars, palettes, cellAnims, unit, actors, commonArc, typeIcons,
// status, nextSeq, moveCount, yesNo, screenBuffer, screen, cursor, prevPos, buttonsShown, decide, menuRes, menu,
// buttonItems, yesNoItems, printQueue, keyCursor and buttons in Black
typedef struct {
    MoveReminderProcessData *param;
    BmpWin *windows[WINDOW_COUNT];
    MsgData *msgData;
    WordSet *wordSet;
    StrBuf *strbuf;
    StrBuf *ppLabel;
    StrBuf *ppFormat;
    ListMenuOption *moveList;
    u32 unk5C;
    Font *font;
    TCBExManager *tcbManager;
    TCB *vblankTcb;
    PrintStream *stream;
    u32 chars[RESOURCE_COUNT];
    u32 palettes[RESOURCE_COUNT];
    u32 cellAnims[RESOURCE_COUNT];
    u32 unkDC[RESOURCE_COUNT];
    ClActUnit *unit;
    ClActor *actors[ACTOR_COUNT];
    ArcTool *commonArc;
    // The type icons whose characters the next VBlank loads
    WazaOshieTypeIcon typeIcons[TYPE_ICON_COUNT];
    PStatusParam status;
    u8 unk1A8[8];
    u32 nextSeq;
    u32 unk1B4;
    u8 moveCount;
    u8 unk1B9;
    u8 yesNo;
    u8 unk1BB;
    u8 unk1BC;
    void *screenBuffer;
    NNSG2dScreenData *screen;
    CursorMove *cursor;
    u16 prevPos;
    u16 buttonsShown;
    WazaOshieDecide decide;
    AppTaskMenuRes *menuRes;
    AppTaskMenu *menu;
    AppTaskMenuItem buttonItems[2];
    AppTaskMenuItem yesNoItems[2];
    u32 unk214;
    PrintQueue *printQueue;
    KeyCursor *keyCursor;
    AppTaskMenuWin *buttons[2];
} WazaOshieWork;

// Black 2's, not checked against Black
typedef struct {
    u32 msg;
    u32 type;
} WazaOshieMenuItem;

typedef u32 (*WazaOshieYesNoFunc)(WazaOshieWork *work);

// Black 2's, not checked against Black
typedef struct {
    WazaOshieYesNoFunc yes;
    WazaOshieYesNoFunc no;
} WazaOshieYesNoFuncs;

// Black 2's, not checked against Black
typedef struct {
    u32 bg;
    u32 x;
    u32 y;
    u32 width;
    u32 height;
    u32 palette;
    u32 charOffset;
} WazaOshieWindowData;

// Black 2's, not checked against Black
typedef struct {
    s16 x;
    s16 y;
    u16 unk04;
    u16 sequence;
    u32 priority;
    u32 unk0C;
    u32 surface;
    // Indices of the work's resources
    u32 chars;
    u32 palette;
    u32 cellAnims;
    u32 unk20;
    u32 bgPriority;
    u32 unk28;
} WazaOshieActorData;

BOOL func_ov173_021b6100(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov173_021b6174(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov173_021b6250(GameProc *proc, u32 *state, void *param, void *work);
void func_ov173_021b6278(WazaOshieWork *work);
static void func_ov173_021b6348(WazaOshieWork *work);
static void func_ov173_021b6390(WazaOshieWork *work);
void func_ov173_021b63ac(WazaOshieWork *work);
static void func_ov173_021b6460(WazaOshieWork *work);
void func_ov173_021b6498(TCB *tcb, void *data);
void func_ov173_021b64c0(void);
void func_ov173_021b64d0(void);
static void func_ov173_021b6678(void);
void func_ov173_021b66bc(WazaOshieWork *work, ArcTool *arc);
static void func_ov173_021b6794(WazaOshieWork *work);
void func_ov173_021b67a4(WazaOshieWork *work);
static void func_ov173_021b6828(WazaOshieWork *work);
void func_ov173_021b6850(WazaOshieWork *work);
static void func_ov173_021b688c(WazaOshieWork *work);
static u32 func_ov173_021b68b0(WazaOshieWork *work);
u32 func_ov173_021b68c8(WazaOshieWork *work);
static u32 func_ov173_021b69f8(WazaOshieWork *work);
static u32 func_ov173_021b6a64(WazaOshieWork *work);
static u32 func_ov173_021b6aac(WazaOshieWork *work);
static u32 func_ov173_021b6b0c(WazaOshieWork *work);
static u32 func_ov173_021b6b54(WazaOshieWork *work);
static u32 func_ov173_021b6b6c(WazaOshieWork *work);
static u32 func_ov173_021b6ba0(WazaOshieWork *work);
void func_ov173_021b6bcc(WazaOshieWork *work);
void func_ov173_021b6bf8(WazaOshieWork *work, u32 window, Font *font, u16 color, u32 align, u8 y);
void func_ov173_021b6c84(WazaOshieWork *work, u32 msg, u32 number, u32 digits, u8 pad);
void func_ov173_021b6cc0(WazaOshieWork *work);
u32 func_ov173_021b6fb4(WazaOshieWork *work);
void func_ov173_021b6fdc(WazaOshieWork *work, u8 scroll, u8 row);
void func_ov173_021b70d8(WazaOshieWork *work);
void func_ov173_021b7128(WazaOshieWork *work);
static void func_ov173_021b71b8(WazaOshieWork *work);
void func_ov173_021b71c4(WazaOshieWork *work, u32 move);
void func_ov173_021b7378(WazaOshieWork *work, u32 msg);
static u16 func_ov173_021b7438(WazaOshieWork *work);
u16 func_ov173_021b7448(WazaOshieWork *work);
static void func_ov173_021b7460(WazaOshieWork *work, u32 msg);
static void func_ov173_021b74d8(WazaOshieWork *work);
static BOOL func_ov173_021b74f8(u32 event);
static u8 func_ov173_021b7568(WazaOshieWork *work);
u32 func_ov173_021b7590(WazaOshieWork *work);
static u32 func_ov173_021b75b0(WazaOshieWork *work);
static u32 func_ov173_021b75bc(WazaOshieWork *work);
static u32 func_ov173_021b75c8(WazaOshieWork *work);
static u32 func_ov173_021b75d4(WazaOshieWork *work);
static u32 func_ov173_021b75e0(WazaOshieWork *work);
static u32 func_ov173_021b75fc(WazaOshieWork *work);
static u32 func_ov173_021b7618(WazaOshieWork *work);
static u32 func_ov173_021b7634(WazaOshieWork *work);
static u32 func_ov173_021b764c(WazaOshieWork *work);
void func_ov173_021b7658(WazaOshieWork *work);
static void func_ov173_021b7694(WazaOshieWork *work);
void func_ov173_021b76c8(WazaOshieWork *work, ArcTool *arc);
void func_ov173_021b78a0(ArcTool *arc, u32 chars, u32 fileId, BOOL compressed, HeapID heapId);
void func_ov173_021b78cc(WazaOshieWork *work, u16 move, u16 index);
void func_ov173_021b7908(WazaOshieWork *work);
void func_ov173_021b7968(WazaOshieWork *work);
void func_ov173_021b79e0(WazaOshieWork *work, u16 move);
void func_ov173_021b7a00(WazaOshieWork *work, ArcTool *arc);
void func_ov173_021b7adc(WazaOshieWork *work, u8 pos, u32 unused);
static u32 func_ov173_021b7b60(WazaOshieWork *work);
u32 func_ov173_021b7b8c(WazaOshieWork *work);
u32 func_ov173_021b7c00(WazaOshieWork *work);
void func_ov173_021b7c44(WazaOshieWork *work);
static void func_ov173_021b7c7c(WazaOshieWork *work);
void func_ov173_021b7c8c(void *data, int pos, int prevPos);
void func_ov173_021b7c90(void *data, s32 pos, s32 prevPos);
void func_ov173_021b7da0(void *data, s32 pos, s32 prevPos);
static void func_ov173_021b7eb0(WazaOshieWork *work, BOOL show);
BOOL func_ov173_021b7f50(WazaOshieWork *work);
void func_ov173_021b7f64(WazaOshieWork *work);
void func_ov173_021b7fd0(WazaOshieWork *work, s32 direction);
void func_ov173_021b8040(WazaOshieWork *work);
u32 func_ov173_021b80a4(WazaOshieWork *work, u16 seq);
u32 func_ov173_021b80c4(WazaOshieWork *work, u16 seq);
u32 func_ov173_021b80ec(WazaOshieWork *work, u16 seq);
static u32 func_ov173_021b8110(WazaOshieWork *work);
static u32 func_ov173_021b815c(WazaOshieWork *work);
static u32 func_ov173_021b817c(WazaOshieWork *work);
static void func_ov173_021b81b0(BOOL dimmed);

// BmpWin_ClearScreenNow with the screen copied at the next VBlank
static inline void WazaOshie_ClearWindow(BmpWin *window) {
    func_020456f8(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
}

// pokebw2's WazaOshieProcFunctions
// pokebw2's WazaOshieProc_Exit
// pokebw2's WazaOshieProc_Init
// pokebw2's WazaOshieProc_Main
const GameProcFunctions data_ov173_021b81d0 = { func_ov173_021b6100, func_ov173_021b6174, func_ov173_021b6250 };

// The tables are declared in the order that gives the ROM's .rodata, with those of the actors and the cursor ahead
// of the functions that read them

// pokebw2's sWazaOshieVRAMConfig
static const BGSysVRAMConfig data_ov173_021b8250 = {
    GX_VRAM_BG_128_B,
    GX_VRAM_BGEXTPLTT_NONE,
    GX_VRAM_SUB_BG_128_C,
    GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_64_E,
    GX_VRAM_OBJEXTPLTT_NONE,
    GX_VRAM_SUB_OBJ_128_D,
    GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_A,
    GX_VRAM_TEXPLTT_0_F,
    GX_OBJVRAMMODE_CHAR_1D_64K,
    GX_OBJVRAMMODE_CHAR_1D_64K,
};

// pokebw2's sWazaOshieButtonItems
static const WazaOshieMenuItem data_ov173_021b81fc[2] = {
    { ShinkaDemoBtlMain_Text_035, 0 },
    { ShinkaDemoBtlMain_Text_034, APP_TASKMENU_ITEM_RETURN },
};

// pokebw2's sWazaOshieYesNoItems
static const WazaOshieMenuItem data_ov173_021b81ec[2] = {
    { ShinkaDemoBtlMain_Text_040, 0 },
    { ShinkaDemoBtlMain_Text_041, 0 },
};

// pokebw2's sWazaOshieWindows
static const WazaOshieWindowData data_ov173_021b8420[WINDOW_COUNT] = {
#include "asset/waza_oshie_main_data_ov173_021b8420.inc"
};

// The messages, by the parameter's mode
// pokebw2's sWazaOshieMessages
static const u32 data_ov173_021b8350[2][MESSAGE_COUNT] = {
    {
        ShinkaDemoBtlMain_Text_000,
        ShinkaDemoBtlMain_Text_001,
        ShinkaDemoBtlMain_Text_002,
        ShinkaDemoBtlMain_Text_003,
        ShinkaDemoBtlMain_Text_004,
        ShinkaDemoBtlMain_Text_005,
        ShinkaDemoBtlMain_Text_006,
        ShinkaDemoBtlMain_Text_007,
        ShinkaDemoBtlMain_Text_008,
        ShinkaDemoBtlMain_Text_009,
        ShinkaDemoBtlMain_Text_010,
    },
    {
        ShinkaDemoBtlMain_Text_011,
        ShinkaDemoBtlMain_Text_012,
        ShinkaDemoBtlMain_Text_013,
        ShinkaDemoBtlMain_Text_014,
        ShinkaDemoBtlMain_Text_015,
        ShinkaDemoBtlMain_Text_016,
        ShinkaDemoBtlMain_Text_017,
        ShinkaDemoBtlMain_Text_018,
        ShinkaDemoBtlMain_Text_019,
        ShinkaDemoBtlMain_Text_020,
        ShinkaDemoBtlMain_Text_021,
    },
};

// pokebw2's sWazaOshieYesNoFuncs
static const WazaOshieYesNoFuncs data_ov173_021b8228[5] = {
    { func_ov173_021b7590, func_ov173_021b75b0 },
    { func_ov173_021b75bc, func_ov173_021b75c8 },
    { func_ov173_021b75d4, func_ov173_021b75e0 },
    { func_ov173_021b75fc, func_ov173_021b7618 },
    { func_ov173_021b7634, func_ov173_021b764c },
};

// pokebw2's sWazaOshieActorSysSetup
static const ClActSysSetup data_ov173_021b820c = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 9, 5, 6, 32, 16, 16 };


BOOL func_ov173_021b6100(GameProc *proc, u32 *state, void *param, void *work) {
    WazaOshieWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_WAZA_OSHIE, 0x50000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(WazaOshieWork), HEAPID_WAZA_OSHIE);
    MI_CpuFill8(wk, 0, sizeof(WazaOshieWork));
    wk->param = param;
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_WAZA_OSHIE);
    wk->tcbManager = GFL_TCBExMgrCreate(HEAPID_WAZA_OSHIE, HEAPID_WAZA_OSHIE, 32, 32);
    func_ov173_021b63ac(wk);
    func_ov173_021b7adc(wk, wk->param->unk14, 3);
    wk->nextSeq = 1;
    func_0203d34c(TRUE, HEAPID_WAZA_OSHIE);
    return TRUE;
}

static BOOL func_ov173_021b6174(GameProc *proc, u32 *state, void *param, void *work) {
    WazaOshieWork *wk = work;

    switch (*state) {
    case SEQ_FADE_WAIT:
        *state = func_ov173_021b68b0(wk);
        break;
    case SEQ_SELECT:
        *state = func_ov173_021b68c8(wk);
        break;
    case SEQ_MESSAGE:
        *state = func_ov173_021b69f8(wk);
        break;
    case SEQ_YES_NO_INIT:
        *state = func_ov173_021b6a64(wk);
        break;
    case SEQ_YES_NO:
        *state = func_ov173_021b6aac(wk);
        break;
    case SEQ_SET_MOVE:
        *state = func_ov173_021b6b0c(wk);
        break;
    case SEQ_FORGOT:
        *state = func_ov173_021b6b54(wk);
        break;
    case SEQ_STATUS_RESULT:
        *state = func_ov173_021b6b6c(wk);
        break;
    case SEQ_FADE_OUT:
        *state = func_ov173_021b6ba0(wk);
        break;
    case SEQ_END:
        return TRUE;
    case SEQ_STATUS_FADE_OUT:
        *state = func_ov173_021b7b60(wk);
        break;
    case SEQ_STATUS_CALL:
        *state = func_ov173_021b7b8c(wk);
        return FALSE;
    case SEQ_STATUS_RETURN:
        *state = func_ov173_021b7c00(wk);
        return FALSE;
    case SEQ_DECIDE_WAIT:
        *state = func_ov173_021b8110(wk);
        break;
    case SEQ_QUIT:
        *state = func_ov173_021b815c(wk);
        break;
    case SEQ_MOVE_CHOSEN:
        *state = func_ov173_021b817c(wk);
        break;
    }
    func_ov173_021b6390(wk);
    func_0204a600();
    GFL_TCBExMgrUpdate(wk->tcbManager);
    func_0201c4e4(wk->printQueue);
    return FALSE;
}

static BOOL func_ov173_021b6250(GameProc *proc, u32 *state, void *param, void *work) {
    WazaOshieWork *wk = work;

    func_ov173_021b6460(wk);
    GFL_TCBExMgrFree(wk->tcbManager);
    GFL_FontFree(wk->font);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_WAZA_OSHIE);
    return TRUE;
}

// pokebw2's WazaOshie_InitMenu
void func_ov173_021b6278(WazaOshieWork *work) {
    int i;

    work->printQueue = func_0201c440(HEAPID_WAZA_OSHIE);
    work->menuRes = func_02027f60(4, 10, work->font, work->printQueue, HEAPID_WAZA_OSHIE);
    for (i = 0; i < 2; i++) {
        work->buttonItems[i].str = GFL_MsgDataLoadStrbufNew(work->msgData, data_ov173_021b81fc[i].msg);
        work->buttonItems[i].color = PRINT_COLOR(14, 15, 3);
        work->buttonItems[i].type = data_ov173_021b81fc[i].type;
    }
    for (i = 0; i < 2; i++) {
        work->yesNoItems[i].str = GFL_MsgDataLoadStrbufNew(work->msgData, data_ov173_021b81ec[i].msg);
        work->yesNoItems[i].color = PRINT_COLOR(14, 15, 3);
        work->yesNoItems[i].type = data_ov173_021b81ec[i].type;
    }
    work->buttons[BUTTON_TEACH] = NULL;
    work->buttons[BUTTON_QUIT] = NULL;
}

// pokebw2's WazaOshie_FreeMenu
static void func_ov173_021b6348(WazaOshieWork *work) {
    func_ov173_021b7eb0(work, FALSE);
    GFL_StrBufFree(work->buttonItems[0].str);
    GFL_StrBufFree(work->buttonItems[1].str);
    GFL_StrBufFree(work->yesNoItems[0].str);
    GFL_StrBufFree(work->yesNoItems[1].str);
    func_02027fd8(work->menuRes);
    func_0201c4c0(work->printQueue);
}

// pokebw2's WazaOshie_UpdateButtons
static void func_ov173_021b6390(WazaOshieWork *work) {
    if (work->buttons[BUTTON_TEACH] != NULL) {
        func_02028178(work->buttons[BUTTON_TEACH]);
        func_02028178(work->buttons[BUTTON_QUIT]);
    }
}

// pokebw2's WazaOshie_InitScreen
void func_ov173_021b63ac(WazaOshieWork *work) {
    ArcTool *arc;

    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    arc = GFL_ArcSysCreateFileHandle(ARCID_WAZA_OSHIE, HEAPID_WAZA_OSHIE);
    func_ov173_021b64c0();
    func_ov173_021b64d0();
    func_ov173_021b66bc(work, arc);
    func_ov173_021b7a00(work, arc);
    func_ov173_021b6850(work);
    func_ov173_021b67a4(work);
    func_ov173_021b7128(work);
    func_ov173_021b6bcc(work);
    func_ov173_021b6cc0(work);
    func_ov173_021b7c44(work);
    func_ov173_021b8040(work);
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, HEAPID_WAZA_OSHIE);
    GFL_BGSysSetDisplayLayout(1);
    work->vblankTcb = GFL_VBlankTCBAdd(func_ov173_021b6498, work, 16);
    GFL_ArcToolFree(arc);
    func_ov173_021b6278(work);
}

// pokebw2's WazaOshie_FreeScreen
static void func_ov173_021b6460(WazaOshieWork *work) {
    func_ov173_021b6348(work);
    func_ov173_021b7c7c(work);
    func_ov173_021b71b8(work);
    func_ov173_021b6828(work);
    func_ov173_021b6794(work);
    func_ov173_021b6678();
    func_ov173_021b688c(work);
    func_ov173_021b7694(work);
    GFL_TCBRemove(work->vblankTcb);
}

// pokebw2's WazaOshie_VBlank
void func_ov173_021b6498(TCB *tcb, void *data) {
    func_ov173_021b7908(data);
    GFL_BGSysUpdate();
    func_0204a648();
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

// pokebw2's WazaOshie_SetVRAMBanks
void func_ov173_021b64c0(void) {
    GFL_BGSysSetVRAMBanks(&data_ov173_021b8250);
}

// pokebw2's WazaOshie_InitBG
void func_ov173_021b64d0(void) {
    GFL_BGSysCreate(HEAPID_WAZA_OSHIE);
    BmpWin_InitAllocator(HEAPID_WAZA_OSHIE);
    {
        BGSysLCDConfig config = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };

        GFL_BGSysSetLCDConfig(&config);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800,
                          GX_BG_CHARBASE_0x00000, 0x8000, GX_BG_EXTPLTT_01, 2, 0, FALSE };

        GFL_BGSysCreateBG(2, &setup, 0);
        GFL_BGSysClearScr(2);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000,
                          GX_BG_CHARBASE_0x08000, 0x8000, GX_BG_EXTPLTT_01, 3, 0, FALSE };

        GFL_BGSysCreateBG(3, &setup, 0);
        GFL_BGSysClearScr(3);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800,
                          GX_BG_CHARBASE_0x00000, 0x8000, GX_BG_EXTPLTT_01, 0, 0, FALSE };

        GFL_BGSysCreateBG(4, &setup, 0);
        GFL_BGSysClearScr(4);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000,
                          GX_BG_CHARBASE_0x18000, 0x8000, GX_BG_EXTPLTT_01, 2, 0, FALSE };

        GFL_BGSysCreateBG(6, &setup, 0);
        GFL_BGSysClearScr(6);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xc000,
                          GX_BG_CHARBASE_0x08000, 0x4000, GX_BG_EXTPLTT_01, 2, 0, FALSE };

        GFL_BGSysCreateBG(5, &setup, 0);
        GFL_BGSysClearScr(5);
    }
    {
        BGSetup setup = { 0, 0, 0x1000, 0, BGRES_512x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe000,
                          GX_BG_CHARBASE_0x10000, 0x2000, GX_BG_EXTPLTT_01, 3, 0, FALSE };

        GFL_BGSysCreateBG(7, &setup, 0);
        GFL_BGSysClearScr(7);
    }
    GFL_BGSysSetBGEnabled(2, TRUE);
    GFL_BGSysSetBGEnabled(3, TRUE);
    GFL_BGSysSetBGEnabled(4, TRUE);
    GFL_BGSysSetBGEnabled(5, TRUE);
    GFL_BGSysSetBGEnabled(6, TRUE);
    GFL_BGSysSetBGEnabled(7, TRUE);
    GFL_BGSysClearCharCore(2, 0x20, 0, HEAPID_WAZA_OSHIE);
    GFL_BGSysClearCharCore(4, 0x20, 0, HEAPID_WAZA_OSHIE);
    GFL_BGSysClearCharCore(5, 0x20, 0, HEAPID_WAZA_OSHIE);
    GFL_BGSysClearCharCore(2, 0x20, 0, HEAPID_WAZA_OSHIE);
    GFL_BGSysClearCharCore(4, 0x20, 0, HEAPID_WAZA_OSHIE);
    G2_BlendNone();
    G2S_BlendNone();
}

// pokebw2's WazaOshie_FreeBG
static void func_ov173_021b6678(void) {
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, FALSE);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(7);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(2);
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    GFL_BGSysFree();
}

// pokebw2's WazaOshie_LoadBG
void func_ov173_021b66bc(WazaOshieWork *work, ArcTool *arc) {
    GFL_BGSysLoadArcNCGRStatic(arc, 1, 3, 0, 0, FALSE, HEAPID_WAZA_OSHIE);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 4, 3, 0, 0x800, FALSE, HEAPID_WAZA_OSHIE);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, 0, 0, 0, HEAPID_WAZA_OSHIE);
    GFL_BGSysLoadArcNCGRStatic(arc, 1, 7, 0, 0, FALSE, HEAPID_WAZA_OSHIE);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 2, 7, 0, 0x800, FALSE, HEAPID_WAZA_OSHIE);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, 4, 0, 0, HEAPID_WAZA_OSHIE);
    work->screenBuffer = GFL_G2DIOReadNSCRArc(arc, 3, FALSE, &work->screen, HEAPID_WAZA_OSHIE);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 14 * 0x20, 0x20, HEAPID_WAZA_OSHIE);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 15 * 0x20, 0x20, HEAPID_WAZA_OSHIE);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 15 * 0x20, 0x20, HEAPID_WAZA_OSHIE);
    LoadSysMsgBox(4, 1, 13, 0, HEAPID_WAZA_OSHIE);
}

// pokebw2's WazaOshie_FreeScreenBuffer
static void func_ov173_021b6794(WazaOshieWork *work) {
    GFL_HeapFree(work->screenBuffer);
}

// pokebw2's WazaOshie_InitWindows
void func_ov173_021b67a4(WazaOshieWork *work) {
    u32 i;

    for (i = 0; i < WINDOW_COUNT; i++) {
        work->windows[i] =
            BmpWin_CreateDynamic(data_ov173_021b8420[i].bg, data_ov173_021b8420[i].x, data_ov173_021b8420[i].y,
                                 data_ov173_021b8420[i].width, data_ov173_021b8420[i].height, data_ov173_021b8420[i].palette, 1);
        GFL_BitmapFill(BmpWin_GetBitmap(work->windows[i]), 0);
    }
    WazaOshie_ClearWindow(work->windows[WINDOW_TEACH]);
    work->keyCursor = func_02028590(15, TRUE, TRUE, HEAPID_WAZA_OSHIE);
}

// pokebw2's WazaOshie_FreeWindows
static void func_ov173_021b6828(WazaOshieWork *work) {
    u32 i;

    func_02028604(work->keyCursor);
    for (i = 0; i < WINDOW_COUNT; i++) {
        BmpWin_Free(work->windows[i]);
    }
    BmpWin_FreeAllocator();
}

// pokebw2's WazaOshie_InitMsg
void func_ov173_021b6850(WazaOshieWork *work) {
    work->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_SHINKA_DEMO_BTL_MAIN, HEAPID_WAZA_OSHIE);
    work->wordSet = GFL_WordSetSystemCreateDefault(HEAPID_WAZA_OSHIE);
    work->strbuf = GFL_StrBufCreate(0x100, HEAPID_WAZA_OSHIE);
    work->ppLabel = GFL_MsgDataLoadStrbufNew(work->msgData, ShinkaDemoBtlMain_Text_025);
    work->ppFormat = GFL_MsgDataLoadStrbufNew(work->msgData, ShinkaDemoBtlMain_Text_031);
}

// pokebw2's WazaOshie_FreeMsg
static void func_ov173_021b688c(WazaOshieWork *work) {
    GFL_MsgDataFree(work->msgData);
    GFL_WordSetSystemFree(work->wordSet);
    GFL_StrBufFree(work->strbuf);
    GFL_StrBufFree(work->ppLabel);
    GFL_StrBufFree(work->ppFormat);
}

// pokebw2's WazaOshie_SeqFadeWait
static u32 func_ov173_021b68b0(WazaOshieWork *work) {
    if (GFL_WipeIsFinished() == TRUE) {
        return work->nextSeq;
    }
    return SEQ_FADE_WAIT;
}

u32 func_ov173_021b68c8(WazaOshieWork *work) {
    u32 pos;

    work->prevPos = func_020260e0(work->cursor);
    pos = func_02025de8(work->cursor);
    switch (pos) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (!func_02035c60()) {
            if (work->param->unk16 + pos < work->moveCount) {
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
                return func_ov173_021b817c(work);
            }
        } else {
            if (work->param->unk16 + pos < work->moveCount) {
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
            }
        }
        break;
    case POS_ARROW_DOWN:
    case POS_ARROW_UP:
    case CURSOR_MOVE_NONE:
    case CURSOR_MOVE_MOVED:
    case CURSOR_MOVE_CURSOR_ON:
        break;
    case POS_RETURN:
    case POS_QUIT:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        if (func_ov173_021b7f50(work)) {
            return func_ov173_021b80ec(work, 14);
        }
        return func_ov173_021b80a4(work, 14);
    case CURSOR_MOVE_CANCEL:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov173_021b7eb0(work, FALSE);
        return func_ov173_021b80a4(work, 14);
    case POS_TEACH:
        if (func_ov173_021b7f50(work)) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            return func_ov173_021b80c4(work, 15);
        }
        break;
    case CURSOR_MOVE_EDGE_UP:
        if (work->param->unk16 != 0) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            work->param->unk16--;
            func_ov173_021b71c4(work, func_ov173_021b7438(work));
            func_ov173_021b70d8(work);
            func_ov173_021b7f64(work);
            func_ov173_021b7fd0(work, -1);
        }
        break;
    }
    return 1;
}


// pokebw2's WazaOshie_SeqMessage
static u32 func_ov173_021b69f8(WazaOshieWork *work) {
    u32 state = func_0201ce30(work->stream);

    func_020286c4(work->keyCursor, work->stream, work->windows[WINDOW_MESSAGE]);
    if (state == PRINT_STREAM_RUNNING) {
        if (GCTX_HIDGetHeldKeys() & PAD_BUTTON_A) {
            func_0201ce5c(work->stream, 0);
        }
    } else if (state == PRINT_STREAM_DONE) {
        func_0201ce48(work->stream);
        return work->nextSeq;
    } else if (state == PRINT_STREAM_PAUSED) {
        if ((GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c60()) {
            func_0201ce38(work->stream);
            GFL_SndSEPlay(SEQ_SE_MESSAGE);
        }
    }
    return SEQ_MESSAGE;
}

// pokebw2's WazaOshie_SeqYesNoInit
static u32 func_ov173_021b6a64(WazaOshieWork *work) {
    AppTaskMenuInit init;

    init.heapId = HEAPID_WAZA_OSHIE;
    init.itemCount = 2;
    init.items = work->yesNoItems;
    init.posType = APP_TASKMENU_POS_TOP_LEFT;
    init.x = 21;
    init.y = 8;
    init.width = 10;
    init.height = 3;
    work->menu = func_02027778(&init, work->menuRes);
    func_ov173_021b81b0(TRUE);
    return SEQ_YES_NO;
}

// pokebw2's WazaOshie_SeqYesNo
static u32 func_ov173_021b6aac(WazaOshieWork *work) {
    u32 seq = SEQ_YES_NO;

    if (func_020279e8(work->menu)) {
        if (func_02027a04(work->menu) == 0) {
            seq = data_ov173_021b8228[work->yesNo].yes(work);
        } else {
            seq = data_ov173_021b8228[work->yesNo].no(work);
        }
        func_ov173_021b81b0(FALSE);
    }
    func_02027974(work->menu);
    if (seq != SEQ_YES_NO) {
        func_02027858(work->menu);
    }
    return seq;
}

static u32 func_ov173_021b6b0c(WazaOshieWork *work) {
    MoveReminderProcessData *param = work->param;

    PokeParty_SetParam(param->pkm, PKM_PARAM_MOVE1 + param->unk1B, func_ov173_021b7438(work));
    PokeParty_SetParam(work->param->pkm, PKM_PARAM_MOVE1_PP_UP + work->param->unk1B, 0);
    param = work->param;
    PokeParty_SetParam(param->pkm, PKM_PARAM_MOVE1_PP + param->unk1B,
        PML_MoveGetMaxPP(func_ov173_021b7438(work), 0));
    work->param->status = MOVE_REMINDER_STATUS_LEARNED;
    return 8;
}


// pokebw2's WazaOshie_SeqForgot
static u32 func_ov173_021b6b54(WazaOshieWork *work) {
    func_ov173_021b7460(work, MSG_FORGOT);
    work->nextSeq = SEQ_SET_MOVE;
    return SEQ_MESSAGE;
}

static u32 func_ov173_021b6b6c(WazaOshieWork *work) {
    if (work->param->unk1B < 4) {
        func_ov173_021b7460(work, 10);
        work->yesNo = 4;
    } else {
        func_ov173_021b7460(work, 7);
        work->yesNo = 3;
    }
    work->nextSeq = 3;
    return 2;
}


// pokebw2's WazaOshie_SeqFadeOut
static u32 func_ov173_021b6ba0(WazaOshieWork *work) {
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, HEAPID_WAZA_OSHIE);
    work->nextSeq = SEQ_END;
    return SEQ_FADE_WAIT;
}

// pokebw2's WazaOshie_DrawCursorMoveInfo
void func_ov173_021b6bcc(WazaOshieWork *work) {
    u32 move = func_ov173_021b7438(work);

    if (move != MOVE_END) {
        func_ov173_021b71c4(work, move);
    } else {
        func_ov173_021b71c4(work, NO_MOVE);
    }
    func_ov173_021b7968(work);
}

// Draws the work's string buffer into a window
// pokebw2's WazaOshie_PrintText
void func_ov173_021b6bf8(WazaOshieWork *work, u32 window, Font *font, u16 color, u32 align, u8 y) {
    u8 x;
    u8 width;
    u8 windowWidth;

    switch (align) {
    case ALIGN_LEFT:
        x = 0;
        break;
    case ALIGN_RIGHT:
        width = GFL_FontGetBlockWidth(work->strbuf, font, 0);
        windowWidth = func_02045738(work->windows[window]) * 8;
        x = windowWidth - width;
        break;
    case ALIGN_CENTER:
        width = GFL_FontGetBlockWidth(work->strbuf, font, 0);
        windowWidth = func_02045738(work->windows[window]) * 8;
        x = (windowWidth - width) / 2;
        break;
    }
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(work->windows[window]), x, y, work->strbuf, font, color);
}

// Formats a message with a number into the work's string buffer
// pokebw2's WazaOshie_FormatNumber
void func_ov173_021b6c84(WazaOshieWork *work, u32 msg, u32 number, u32 digits, u8 pad) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(work->msgData, msg);

    WordSetNumber(work->wordSet, 0, number, digits, pad, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strbuf, str);
    GFL_StrBufFree(str);
}

// Draws the labels, and on the top screen the Pokémon's name, level and moves
// pokebw2's WazaOshie_DrawText
void func_ov173_021b6cc0(WazaOshieWork *work) {
    StrBuf *str;
    MsgData *moveNames;
    BoxPkm *boxPkm;
    u32 i;
    u16 move;

    GFL_MsgDataLoadStrbuf(work->msgData, ShinkaDemoBtlMain_Text_035, work->strbuf);
    func_ov173_021b6bf8(work, WINDOW_TEACH, work->font, PRINT_COLOR(15, 2, 0), ALIGN_CENTER, 4);
    GFL_MsgDataLoadStrbuf(work->msgData, ShinkaDemoBtlMain_Text_023, work->strbuf);
    func_ov173_021b6bf8(work, WINDOW_POWER_LABEL, work->font, PRINT_COLOR(1, 2, 0), ALIGN_LEFT, 0);
    BmpWin_Transfer(work->windows[WINDOW_POWER_LABEL]);
    GFL_MsgDataLoadStrbuf(work->msgData, ShinkaDemoBtlMain_Text_024, work->strbuf);
    func_ov173_021b6bf8(work, WINDOW_ACCURACY_LABEL, work->font, PRINT_COLOR(1, 2, 0), ALIGN_LEFT, 0);
    BmpWin_Transfer(work->windows[WINDOW_ACCURACY_LABEL]);

    str = GFL_StrBufCreate(0x100, HEAPID_WAZA_OSHIE);
    GFL_MsgDataLoadStrbuf(work->msgData, ShinkaDemoBtlMain_Text_039, str);
    loadPokemonNicknameToStrbuf(work->wordSet, 0, work->param->pkm);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strbuf, str);
    func_ov173_021b6bf8(work, WINDOW_TITLE, work->font, PRINT_COLOR(15, 2, 0), ALIGN_LEFT, 4);
    BmpWin_Transfer(work->windows[WINDOW_TITLE]);

    moveNames = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_MOVE_NAMES, HEAPID_WAZA_OSHIE);
    boxPkm = func_0201d624(work->param->pkm);
    for (i = 0; i < 4; i++) {
        move = PML_PkmGetParam(boxPkm, PKM_PARAM_MOVE1 + i, NULL);
        if (move == 0) {
            func_0204b3dc(work->actors[ACTOR_KNOWN_TYPE + i], FALSE);
        } else {
            func_0204b3dc(work->actors[ACTOR_KNOWN_TYPE + i], TRUE);
            func_ov173_021b78cc(work, move, i);
            GFL_MsgDataLoadStrbuf(moveNames, move, work->strbuf);
            func_ov173_021b6bf8(work, WINDOW_KNOWN_MOVES, work->font, PRINT_COLOR(15, 2, 0), ALIGN_LEFT, i * 32);
            GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(work->windows[WINDOW_KNOWN_MOVES]), 8, i * 32 + 16,
                                           work->ppLabel, work->font, PRINT_COLOR(1, 2, 0));
            BmpWin_Transfer(work->windows[WINDOW_KNOWN_MOVES]);
            WordSetNumber(work->wordSet, 0, PML_PkmGetParam(boxPkm, PKM_PARAM_MOVE1_PP + i, NULL), 2, 1, TRUE);
            WordSetNumber(work->wordSet, 1, PML_PkmGetParam(boxPkm, PKM_PARAM_MOVE1_MAX_PP + i, NULL), 2, 0, TRUE);
            GFL_WordSetFormatStrbuf(work->wordSet, work->strbuf, work->ppFormat);
            GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(work->windows[WINDOW_KNOWN_MOVES]), 32, i * 32 + 16,
                                           work->strbuf, work->font, PRINT_COLOR(1, 2, 0));
            BmpWin_Transfer(work->windows[WINDOW_KNOWN_MOVES]);
        }
    }
    BmpWin_Transfer(work->windows[WINDOW_KNOWN_MOVES]);

    GFL_MsgDataLoadStrbuf(work->msgData, ShinkaDemoBtlMain_Text_037, str);
    setPartyPokemonSpeciesNameToStrbuf(work->wordSet, 0, work->param->pkm);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strbuf, str);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(work->windows[WINDOW_POKEMON]), 0, 0, work->strbuf, work->font,
                                   PRINT_COLOR(1, 2, 0));
    BmpWin_Transfer(work->windows[WINDOW_POKEMON]);
    GFL_MsgDataLoadStrbuf(work->msgData, ShinkaDemoBtlMain_Text_038, str);
    WordSetNumber(work->wordSet, 0, PML_PkmGetParam(boxPkm, PKM_PARAM_LEVEL, NULL), 3, 1, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strbuf, str);
    func_ov173_021b6bf8(work, WINDOW_POKEMON, work->font, PRINT_COLOR(1, 2, 0), ALIGN_RIGHT, 16);
    BmpWin_Transfer(work->windows[WINDOW_POKEMON]);

    GFL_MsgDataFree(moveNames);
    GFL_StrBufFree(str);
}

// pokebw2's WazaOshie_CountMoves
u32 func_ov173_021b6fb4(WazaOshieWork *work) {
    u32 i;

    for (i = 0; i < 256; i++) {
        if (work->param->moves[i] == MOVE_END) {
            break;
        }
    }
    return i;
}

// pokebw2's WazaOshie_DrawListRow
void func_ov173_021b6fdc(WazaOshieWork *work, u8 scroll, u8 row) {
    int y = row * 24;
    int index = scroll + row;
    u32 pp;

    if (index >= work->moveCount) {
        return;
    }
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(work->windows[WINDOW_LIST]), 0, y, work->moveList[index].text,
                                   work->font, PRINT_COLOR(15, 2, 0));
    BmpWin_Transfer(work->windows[WINDOW_LIST]);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(work->windows[WINDOW_LIST]), 96, y, work->ppLabel, work->font,
                                   PRINT_COLOR(1, 2, 0));
    BmpWin_Transfer(work->windows[WINDOW_LIST]);
    pp = PML_MoveGetMaxPP(work->moveList[index].value, 0);
    WordSetNumber(work->wordSet, 0, pp, 2, 1, TRUE);
    WordSetNumber(work->wordSet, 1, pp, 2, 0, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strbuf, work->ppFormat);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(work->windows[WINDOW_LIST]), 120, y, work->strbuf, work->font,
                                   PRINT_COLOR(1, 2, 0));
    BmpWin_Transfer(work->windows[WINDOW_LIST]);
}

void func_ov173_021b70d8(WazaOshieWork *work) {
    BmpWin *window;
    u32 i;

    GFL_BitmapFill(BmpWin_GetBitmap(work->windows[11]), 0);
    for (i = 0; i < 4; i++) {
        func_ov173_021b6fdc(work, work->param->unk16, i);
    }
    window = work->windows[11];
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
    GFL_BGSysQueueScrLoad(7);
    func_ov173_021b7968(work);
}

void func_ov173_021b7128(WazaOshieWork *work) {
    MsgData *moveNames;
    u32 i;

    work->moveCount = func_ov173_021b6fb4(work);
    work->moveList = ListMenuCore_CreateOptionList(work->moveCount, HEAPID_WAZA_OSHIE);
    moveNames = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_MOVE_NAMES, HEAPID_WAZA_OSHIE);
    for (i = 0; i < work->moveCount; i++) {
        if (work->param->moves[i] != MOVE_END) {
            ListMenuCore_AppendMsgOption(work->moveList, moveNames, work->param->moves[i], work->param->moves[i],
                HEAPID_WAZA_OSHIE);
        } else {
            ListMenuCore_AppendMsgOption(work->moveList, work->msgData, ShinkaDemoBtlMain_Text_032, -2,
                HEAPID_WAZA_OSHIE);
            break;
        }
    }
    GFL_MsgDataFree(moveNames);
    work->unk1BB = 0;
    work->unk1BC = work->param->unk14;
    func_ov173_021b70d8(work);
}



// pokebw2's WazaOshie_FreeList
static void func_ov173_021b71b8(WazaOshieWork *work) {
    ListMenuCore_FreeOptionList(work->moveList);
}

// Draws a move's power, accuracy, description and category, or clears them for NO_MOVE
// pokebw2's WazaOshie_DrawMoveInfo
void func_ov173_021b71c4(WazaOshieWork *work, u32 move) {
    u32 power;
    u32 accuracy;
    MsgData *descriptions;

    GFL_BitmapFill(BmpWin_GetBitmap(work->windows[WINDOW_DESCRIPTION]), 0);
    GFL_BitmapFill(BmpWin_GetBitmap(work->windows[WINDOW_POWER]), 0);
    GFL_BitmapFill(BmpWin_GetBitmap(work->windows[WINDOW_ACCURACY]), 0);
    func_0204b3dc(work->actors[ACTOR_CATEGORY], FALSE);
    if (move != NO_MOVE) {
        power = PML_MoveGetParam(move, MOVE_PARAM_POWER);
        if (power <= 1) {
            GFL_MsgDataLoadStrbuf(work->msgData, ShinkaDemoBtlMain_Text_033, work->strbuf);
        } else {
            func_ov173_021b6c84(work, ShinkaDemoBtlMain_Text_029, power, 3, 0);
        }
        func_ov173_021b6bf8(work, WINDOW_POWER, work->font, PRINT_COLOR(1, 2, 0), ALIGN_RIGHT, 0);
        accuracy = PML_MoveGetParam(move, MOVE_PARAM_ACCURACY);
        if (accuracy == 0 || PML_MoveIsAlwaysHit(move)) {
            GFL_MsgDataLoadStrbuf(work->msgData, ShinkaDemoBtlMain_Text_033, work->strbuf);
        } else {
            func_ov173_021b6c84(work, ShinkaDemoBtlMain_Text_030, accuracy, 3, 0);
        }
        func_ov173_021b6bf8(work, WINDOW_ACCURACY, work->font, PRINT_COLOR(1, 2, 0), ALIGN_RIGHT, 0);
        descriptions = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_MAIN_13, HEAPID_WAZA_OSHIE);
        GFL_MsgDataLoadStrbuf(descriptions, move, work->strbuf);
        func_ov173_021b6bf8(work, WINDOW_DESCRIPTION, work->font, PRINT_COLOR(1, 2, 0), ALIGN_LEFT, 0);
        GFL_MsgDataFree(descriptions);
        func_ov173_021b79e0(work, move);
        func_0204b3dc(work->actors[ACTOR_CATEGORY], TRUE);
        BmpWin_Transfer(work->windows[WINDOW_POWER_LABEL]);
        BmpWin_Transfer(work->windows[WINDOW_ACCURACY_LABEL]);
    } else {
        WazaOshie_ClearWindow(work->windows[WINDOW_POWER_LABEL]);
        WazaOshie_ClearWindow(work->windows[WINDOW_ACCURACY_LABEL]);
    }
    BmpWin_Transfer(work->windows[WINDOW_DESCRIPTION]);
    BmpWin_Transfer(work->windows[WINDOW_POWER]);
    BmpWin_Transfer(work->windows[WINDOW_ACCURACY]);
}

void func_ov173_021b7378(WazaOshieWork *work, u32 msg) {
    StrBuf *str;

    switch (msg) {
    case MSG_SELECT:
        loadPokemonNicknameToStrbuf(work->wordSet, 0, work->param->pkm);
        break;
    case MSG_LEARN:
        loadMoveNameToStrbuf(work->wordSet, 1, func_ov173_021b7438(work));
        break;
    case MSG_QUIT:
        loadPokemonNicknameToStrbuf(work->wordSet, 0, work->param->pkm);
        break;
    case MSG_LEARNED:
        loadPokemonNicknameToStrbuf(work->wordSet, 0, work->param->pkm);
        loadMoveNameToStrbuf(work->wordSet, 1, func_ov173_021b7438(work));
        break;
    case MSG_REPLACE:
        loadPokemonNicknameToStrbuf(work->wordSet, 0, work->param->pkm);
        loadMoveNameToStrbuf(work->wordSet, 1, func_ov173_021b7438(work));
        break;
    case MSG_FORGET:
        loadPokemonNicknameToStrbuf(work->wordSet, 0, work->param->pkm);
        loadMoveNameToStrbuf(work->wordSet, 1, func_ov173_021b7448(work));
        break;
    case MSG_FORGOT:
        loadPokemonNicknameToStrbuf(work->wordSet, 0, work->param->pkm);
        loadMoveNameToStrbuf(work->wordSet, 1, func_ov173_021b7438(work));
        break;
    case MSG_GIVE_UP:
        loadMoveNameToStrbuf(work->wordSet, 1, func_ov173_021b7438(work));
        break;
    case MSG_NOT_LEARNED:
        loadPokemonNicknameToStrbuf(work->wordSet, 0, work->param->pkm);
        loadMoveNameToStrbuf(work->wordSet, 1, func_ov173_021b7438(work));
        break;
    case MSG_PAID:
        copyVarForText(work->wordSet, 2, work->param->playerInfo);
        break;
    case MSG_FORGET_CONFIRM:
        loadMoveNameToStrbuf(work->wordSet, 0, func_ov173_021b7448(work));
        break;
    }
    str = GFL_MsgDataLoadStrbufNew(work->msgData, data_ov173_021b8350[work->param->unk19][msg]);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strbuf, str);
    GFL_StrBufFree(str);
}


static u16 func_ov173_021b7438(WazaOshieWork *work) {
    MoveReminderProcessData *param = work->param;
    return param->moves[param->unk16 + param->unk14];
}

u16 func_ov173_021b7448(WazaOshieWork *work) {
    MoveReminderProcessData *param = work->param;
    return PokeParty_GetParam(param->pkm, PKM_PARAM_MOVE1 + param->unk1B, NULL);
}



// pokebw2's WazaOshie_PrintMessage
static void func_ov173_021b7460(WazaOshieWork *work, u32 msg) {
    GFL_BitmapFill(BmpWin_GetBitmap(work->windows[WINDOW_MESSAGE]), 15);
    BmpWin_DrawFrame(work->windows[WINDOW_MESSAGE], 0, 1, 13);
    func_ov173_021b7378(work, msg);
    work->stream = func_0201cd10(work->windows[WINDOW_MESSAGE], 0, 0, work->strbuf, work->font, func_02012ffc(),
                                 work->tcbManager, 0, HEAPID_WAZA_OSHIE, 0xffff, func_ov173_021b74f8);
    BmpWin_TransferNow(work->windows[WINDOW_MESSAGE]);
}

// pokebw2's WazaOshie_ClearMessage
static void func_ov173_021b74d8(WazaOshieWork *work) {
    func_0201f7a8(work->windows[WINDOW_MESSAGE], 2);
    WazaOshie_ClearWindow(work->windows[WINDOW_MESSAGE]);
}

// The messages' events: wait for the sounds, play the forgetting sound, and play the fanfare and wait for it
// pokebw2's WazaOshie_StreamCallback
static BOOL func_ov173_021b74f8(u32 event) {
    switch (event) {
    case 1:
        return GFL_SndPlayerIsActiveAny();
    case 2:
        return GFL_SndPlayerIsActiveAny();
    case 3:
        GFL_SndSEPlay(SEQ_SE_KON);
        break;
    case 4:
        GFL_SndBGMSetPaused(TRUE);
        GFL_SndBGMPush();
        GFL_SndBGMPlay(SEQ_ME_LVUP, 0xffff);
        break;
    case 5:
        if (!GFL_SndBGMIsPlaying()) {
            GFL_SndBGMPop();
            GFL_SndBGMSetPaused(FALSE);
            break;
        }
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WazaOshie_CountKnownMoves
static u8 func_ov173_021b7568(WazaOshieWork *work) {
    u8 i;

    for (i = 0; i < 4; i++) {
        if (PokeParty_GetParam(work->param->pkm, PKM_PARAM_MOVE1 + i, NULL) == 0) {
            break;
        }
    }
    return i;
}

u32 func_ov173_021b7590(WazaOshieWork *work) {
    func_ov173_021b7460(work, 3);
    work->param->unk1B = func_ov173_021b7568(work);
    work->nextSeq = 5;
    return 2;
}


// pokebw2's WazaOshie_LearnNo
static u32 func_ov173_021b75b0(WazaOshieWork *work) {
    func_ov173_021b74d8(work);
    return SEQ_SELECT;
}

// pokebw2's WazaOshie_QuitYes
static u32 func_ov173_021b75bc(WazaOshieWork *work) {
    work->param->status = MOVE_REMINDER_STATUS_CANCELLED;
    return SEQ_FADE_OUT;
}

// pokebw2's WazaOshie_QuitNo
static u32 func_ov173_021b75c8(WazaOshieWork *work) {
    func_ov173_021b74d8(work);
    return SEQ_SELECT;
}

// pokebw2's WazaOshie_ReplaceYes
static u32 func_ov173_021b75d4(WazaOshieWork *work) {
    work->nextSeq = SEQ_STATUS_FADE_OUT;
    return SEQ_FADE_WAIT;
}

// pokebw2's WazaOshie_ReplaceNo
static u32 func_ov173_021b75e0(WazaOshieWork *work) {
    func_ov173_021b7460(work, MSG_GIVE_UP);
    work->yesNo = YES_NO_GIVE_UP;
    work->nextSeq = SEQ_YES_NO_INIT;
    return SEQ_MESSAGE;
}

// pokebw2's WazaOshie_GiveUpYes
static u32 func_ov173_021b75fc(WazaOshieWork *work) {
    func_ov173_021b7460(work, MSG_NOT_LEARNED);
    work->nextSeq = SEQ_FADE_OUT;
    work->param->status = MOVE_REMINDER_STATUS_CANCELLED;
    return SEQ_MESSAGE;
}

// pokebw2's WazaOshie_GiveUpNo
static u32 func_ov173_021b7618(WazaOshieWork *work) {
    func_ov173_021b7460(work, MSG_REPLACE);
    work->yesNo = YES_NO_REPLACE;
    work->nextSeq = SEQ_YES_NO_INIT;
    return SEQ_MESSAGE;
}

// pokebw2's WazaOshie_ForgetYes
static u32 func_ov173_021b7634(WazaOshieWork *work) {
    func_ov173_021b7460(work, MSG_FORGET);
    work->nextSeq = SEQ_FORGOT;
    return SEQ_MESSAGE;
}

// pokebw2's WazaOshie_ForgetNo
static u32 func_ov173_021b764c(WazaOshieWork *work) {
    work->nextSeq = SEQ_STATUS_FADE_OUT;
    return SEQ_FADE_WAIT;
}

// pokebw2's WazaOshie_InitActorSys
void func_ov173_021b7658(WazaOshieWork *work) {
    ClActSys_Create(&data_ov173_021b820c, &data_ov173_021b8250, HEAPID_WAZA_OSHIE);
    work->unit = func_0204b100(ACTOR_COUNT, 0, HEAPID_WAZA_OSHIE);
    func_0204b270(work->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's WazaOshie_FreeActors
static void func_ov173_021b7694(WazaOshieWork *work) {
    u32 i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        func_0204b3b4(work->actors[i]);
    }
    func_0204b1cc(work->unit);
    func_0204a5ac();
    GFL_ArcToolFree(work->commonArc);
}

// pokebw2's WazaOshie_LoadActorResources
void func_ov173_021b76c8(WazaOshieWork *work, ArcTool *arc) {
    ArcTool *commonArc;
    ArcTool *pokeArc;
    BoxPkm *boxPkm;
    BOOL wasEncrypted;
    u32 i;

    commonArc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), HEAPID_WAZA_OSHIE);
    work->chars[0] = func_0204a6c8(commonArc, func_02027618(), FALSE, CLACT_VRAM_SUB, HEAPID_WAZA_OSHIE);
    work->chars[1] = func_0204a6c8(arc, 17, FALSE, CLACT_VRAM_SUB, HEAPID_WAZA_OSHIE);
    work->chars[2] = func_0204a6c8(arc, 26, FALSE, CLACT_VRAM_SUB, HEAPID_WAZA_OSHIE);
    for (i = 4; i < 8; i++) {
        work->chars[i] = func_0204a6c8(commonArc, func_020275f8(0), FALSE, CLACT_VRAM_MAIN, HEAPID_WAZA_OSHIE);
    }
    work->chars[3] = func_0204a6c8(arc, 23, FALSE, CLACT_VRAM_SUB, HEAPID_WAZA_OSHIE);
    work->palettes[0] = func_0204ac18(commonArc, func_02027614(), CLACT_VRAM_SUB, 0, 0, 4, HEAPID_WAZA_OSHIE);
    work->palettes[1] = func_0204ac18(arc, 5, CLACT_VRAM_SUB, 4 * 0x20, 0, 1, HEAPID_WAZA_OSHIE);
    work->palettes[2] = func_0204ac18(commonArc, func_020275e8(), CLACT_VRAM_SUB, 6 * 0x20, 0, 3, HEAPID_WAZA_OSHIE);
    work->palettes[3] = func_0204abf0(commonArc, func_020275e8(), CLACT_VRAM_MAIN, 0, HEAPID_WAZA_OSHIE);
    work->cellAnims[0] = func_0204af28(commonArc, func_0202761c(1), func_02027620(1), HEAPID_WAZA_OSHIE);
    work->cellAnims[1] = func_0204af28(arc, 7, 8, HEAPID_WAZA_OSHIE);
    work->cellAnims[2] = func_0204af28(commonArc, func_020275fc(1), func_02027600(1), HEAPID_WAZA_OSHIE);
    work->cellAnims[3] = func_0204af28(arc, 24, 25, HEAPID_WAZA_OSHIE);
    work->cellAnims[4] = func_0204af28(arc, 27, 28, HEAPID_WAZA_OSHIE);
    GFL_ArcToolFree(commonArc);

    pokeArc = MakePokeGraArcHandle(HEAPID_WAZA_OSHIE);
    boxPkm = func_020185f0(work->param->pkm);
    wasEncrypted = PML_PkmDecrypt(boxPkm);
    work->chars[8] = func_0202d934(pokeArc, boxPkm, 0, CLACT_VRAM_MAIN, HEAPID_WAZA_OSHIE);
    work->palettes[4] =
        func_0202d8d0(pokeArc, boxPkm, 0, CLACT_VRAM_MAIN, 4 * 0x20, HEAPID_WAZA_OSHIE);
    work->cellAnims[5] = func_0202d9a4(boxPkm, 0, 1, CLACT_VRAM_MAIN, HEAPID_WAZA_OSHIE);
    PML_PkmReEncrypt(boxPkm, wasEncrypted);
    GFL_ArcToolFree(pokeArc);
}

// pokebw2's WazaOshie_ReplaceChars
void func_ov173_021b78a0(ArcTool *arc, u32 chars, u32 fileId, BOOL compressed, HeapID heapId) {
    NNSG2dCharacterData *character;
    void *buffer = GFL_G2DIOReadOBJNCGRArc(arc, fileId, compressed, &character, heapId);

    func_0204a9d0(chars, character);
    GFL_HeapFree(buffer);
}

// Gives the icon of one of the Pokémon's moves its type, loaded at the next VBlank
// pokebw2's WazaOshie_SetKnownMoveType
void func_ov173_021b78cc(WazaOshieWork *work, u16 move, u16 index) {
    u32 type = PML_MoveGetParam(move, MOVE_PARAM_TYPE);

    work->typeIcons[index].type = type;
    work->typeIcons[index].dirty = TRUE;
    func_0204b6f4(work->actors[ACTOR_KNOWN_TYPE + index], func_020275ec(type), 1);
}

// pokebw2's WazaOshie_LoadTypeIcons
void func_ov173_021b7908(WazaOshieWork *work) {
    int i;

    for (i = 0; i < TYPE_ICON_COUNT; i++) {
        if (work->typeIcons[i].dirty) {
            func_ov173_021b78a0(work->commonArc, work->chars[4 + i], func_020275f8(work->typeIcons[i].type), FALSE,
                                   HEAPID_WAZA_OSHIE);
            work->typeIcons[i].dirty = FALSE;
        }
    }
}

void func_ov173_021b7968(WazaOshieWork *work) {
    ClActorPos pos;
    u32 i;

    for (i = 0; i < 4; i++) {
        pos.x = 47;
        pos.y = i * 24 + 80;
        func_0204b404(work->actors[4 + i], &pos, CLACT_SURFACE_SUB);
        if (work->param->unk16 + i >= work->moveCount) {
            func_0204b3dc(work->actors[4 + i], FALSE);
        } else {
            u16 move = work->param->moves[work->param->unk16 + i];
            s32 type = PML_MoveGetParam(move, MOVE_PARAM_TYPE);
            func_0204b3dc(work->actors[4 + i], TRUE);
            func_0204b878(work->actors[4 + i], type);
        }
    }
}


// pokebw2's WazaOshie_SetCategory
void func_ov173_021b79e0(WazaOshieWork *work, u16 move) {
    func_0204b878(work->actors[ACTOR_CATEGORY], PML_MoveGetParam(move, MOVE_PARAM_CATEGORY));
}

// pokebw2's sWazaOshieActors
static const WazaOshieActorData data_ov173_021b85e0[ACTOR_COUNT] = {
    { 8, 168, 0, 2, 1, 2, CLACT_SURFACE_SUB, 0, 0, 0, 0, 2, 0 },
    { 40, 168, 0, 3, 1, 2, CLACT_SURFACE_SUB, 0, 0, 0, 0, 2, 0 },
    { 130, 84, 0, 0, 0, 3, CLACT_SURFACE_SUB, 1, 1, 1, 1, 2, 0 },
    { 33, 56, 0, 0, 0, 6, CLACT_SURFACE_SUB, 2, 2, 4, 4, 2, 0 },
    { 47, 80, 0, 0, 1, 6, CLACT_SURFACE_SUB, 3, 2, 3, 3, 2, 0 },
    { 47, 104, 0, 0, 1, 6, CLACT_SURFACE_SUB, 3, 2, 3, 3, 2, 0 },
    { 47, 128, 0, 0, 1, 6, CLACT_SURFACE_SUB, 3, 2, 3, 3, 2, 0 },
    { 47, 152, 0, 0, 1, 6, CLACT_SURFACE_SUB, 3, 2, 3, 3, 2, 0 },
    { 38, 48, 0, 0, 1, 0, CLACT_SURFACE_MAIN, 4, 3, 2, 2, 2, 0 },
    { 38, 80, 0, 0, 1, 0, CLACT_SURFACE_MAIN, 5, 3, 2, 2, 2, 0 },
    { 38, 112, 0, 0, 1, 0, CLACT_SURFACE_MAIN, 6, 3, 2, 2, 2, 0 },
    { 38, 144, 0, 0, 1, 0, CLACT_SURFACE_MAIN, 7, 3, 2, 2, 2, 0 },
    { 204, 124, 0, 0, 0, 0, CLACT_SURFACE_MAIN, 8, 4, 5, 5, 2, 0 },
    { 224, 168, 0, 1, 0, 0, CLACT_SURFACE_SUB, 0, 0, 0, 0, 2, 0 },
};

// pokebw2's WazaOshie_InitActors
void func_ov173_021b7a00(WazaOshieWork *work, ArcTool *arc) {
    u32 i;
    ClActorSetup setup;

    func_ov173_021b7658(work);
    func_ov173_021b76c8(work, arc);
    work->commonArc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), HEAPID_WAZA_OSHIE);
    for (i = 0; i < ACTOR_COUNT; i++) {
        setup.x = data_ov173_021b85e0[i].x;
        setup.y = data_ov173_021b85e0[i].y;
        setup.sequence = data_ov173_021b85e0[i].sequence;
        setup.priority = data_ov173_021b85e0[i].priority;
        setup.bgPriority = data_ov173_021b85e0[i].bgPriority;
        work->actors[i] = func_0204b294(work->unit, work->chars[data_ov173_021b85e0[i].chars],
                                        work->palettes[data_ov173_021b85e0[i].palette],
                                        work->cellAnims[data_ov173_021b85e0[i].cellAnims], &setup,
                                        data_ov173_021b85e0[i].surface, HEAPID_WAZA_OSHIE);
        func_0204b92c(work->actors[i], FALSE);
    }
    func_0204b92c(work->actors[ACTOR_RETURN], TRUE);
    func_0204b92c(work->actors[ACTOR_CURSOR], TRUE);
    func_0204b92c(work->actors[ACTOR_ARROW_UP], TRUE);
    func_0204b92c(work->actors[ACTOR_ARROW_DOWN], TRUE);
}

// Moves the cursor's actor to a row of the list, an arrow or the buttons, with the frame for each
// pokebw2's WazaOshie_SetCursorActor
void func_ov173_021b7adc(WazaOshieWork *work, u8 pos, u32 unused) {
    ClActorPos actorPos;

    if (pos < LIST_ROWS) {
        actorPos.x = 130;
        actorPos.y = pos * 24 + 84;
        func_0204b404(work->actors[ACTOR_CURSOR], &actorPos, CLACT_SURFACE_SUB);
        func_0204b8c4(work->actors[ACTOR_CURSOR], 0);
    } else if (pos < POS_RETURN) {
        actorPos.x = (pos - POS_ARROW_DOWN) * 40;
        actorPos.y = 168;
        func_0204b404(work->actors[ACTOR_CURSOR], &actorPos, CLACT_SURFACE_SUB);
        func_0204b8c4(work->actors[ACTOR_CURSOR], 1);
    } else {
        actorPos.x = 192;
        actorPos.y = 160;
        func_0204b404(work->actors[ACTOR_CURSOR], &actorPos, CLACT_SURFACE_SUB);
        func_0204b8c4(work->actors[ACTOR_CURSOR], 2);
    }
}

// pokebw2's WazaOshie_SeqStatusFadeOut
static u32 func_ov173_021b7b60(WazaOshieWork *work) {
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, HEAPID_WAZA_OSHIE);
    work->nextSeq = SEQ_STATUS_CALL;
    return SEQ_FADE_WAIT;
}

u32 func_ov173_021b7b8c(WazaOshieWork *work) {
    func_ov173_021b6460(work);
    work->status.party = work->param->pkm;
    work->status.trainerData = work->param->unk08;
    work->status.gameData = GSYS_GetGameData(work->param->gsys);
    work->status.dataType = PSTATUS_DATA_PARTY_PKM;
    work->status.partyIndex = 0;
    work->status.partyCount = 1;
    work->status.move = func_ov173_021b7438(work);
    work->status.mode = PSTATUS_MODE_FORGET_MOVE;
    work->status.page = PSTATUS_PAGE_INFO;
    work->status.isNationalDex = FALSE;
    GSYS_QueueProc(work->param->gsys, OVERLAY_PSTATUS, &data_ov131_021dc800, &work->status);
    return 12;
}


u32 func_ov173_021b7c00(WazaOshieWork *work) {
    u8 slot;

    func_ov173_021b63ac(work);
    func_ov173_021b7adc(work, work->param->unk14, 3);
    func_0203d34c(TRUE, HEAPID_WAZA_OSHIE);
    if (work->status.result == PSTATUS_RESULT_FORGET) {
        slot = work->status.slot;
    } else {
        slot = 4;
    }
    work->param->unk1B = slot;
    work->nextSeq = 7;
    return 0;
}


// The list's rows, the arrows, the return icon and the two buttons
// pokebw2's sWazaOshieCursorData
static const CursorMoveData data_ov173_021b83a8[] = {
#include "asset/waza_oshie_main_data_ov173_021b83a8.inc"
};

// pokebw2's sWazaOshieCursorCallbacks
static const CursorMoveCallbacks data_ov173_021b81dc = {
    func_ov173_021b7c8c,
    func_ov173_021b7c8c,
    func_ov173_021b7c90,
    func_ov173_021b7da0,
};

// pokebw2's WazaOshie_InitCursor
void func_ov173_021b7c44(WazaOshieWork *work) {
    work->cursor =
        func_02025cd0(data_ov173_021b83a8, &data_ov173_021b81dc, work, TRUE, 0, HEAPID_WAZA_OSHIE);
    func_02026124(work->cursor, POS_QUIT);
    func_ov173_021b7f64(work);
}

// pokebw2's WazaOshie_FreeCursor
static void func_ov173_021b7c7c(WazaOshieWork *work) {
    func_02025d14(work->cursor);
}

// pokebw2's WazaOshie_CursorDummy
void func_ov173_021b7c8c(void *data, int pos, int prevPos) {
}

void func_ov173_021b7c90(void *data, s32 pos, s32 prevPos) {
    WazaOshieWork *work = data;

    func_ov173_021b7eb0(work, FALSE);
    if (pos <= 3) {
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        work->param->unk14 = pos;
        if (work->param->unk16 + work->param->unk14 < work->moveCount) {
            func_ov173_021b71c4(work, func_ov173_021b7438(work));
        } else {
            func_ov173_021b71c4(work, NO_MOVE);
        }
    } else if (pos == 6) {
        if (prevPos == 3 && work->param->unk16 + 4 < work->moveCount) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            pos = 3;
            func_020260e4(work->cursor, pos);
            work->param->unk16++;
            func_ov173_021b70d8(work);
            func_ov173_021b71c4(work, func_ov173_021b7438(work));
            func_ov173_021b7f64(work);
            func_ov173_021b7fd0(work, 1);
        } else if (prevPos == 0) {
            if (work->param->unk16 != 0) {
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
                work->param->unk16--;
                func_ov173_021b70d8(work);
                func_ov173_021b71c4(work, func_ov173_021b7438(work));
                func_ov173_021b7f64(work);
                func_ov173_021b7fd0(work, -1);
            }
            pos = 0;
            func_020260e4(work->cursor, pos);
        } else {
            pos = 3;
            func_020260e4(work->cursor, pos);
        }
    } else {
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
    }
    func_ov173_021b7adc(work, pos, 3);
}

void func_ov173_021b7da0(void *data, s32 pos, s32 prevPos) {
    WazaOshieWork *work = data;

    if (pos <= 3) {
        work->param->unk14 = pos;
        if (work->param->unk16 + work->param->unk14 < work->moveCount) {
            func_ov173_021b7eb0(work, TRUE);
            func_ov173_021b71c4(work, func_ov173_021b7438(work));
        } else {
            func_ov173_021b7eb0(work, FALSE);
            func_ov173_021b71c4(work, NO_MOVE);
        }
    } else if (pos == POS_ARROW_DOWN) {
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        pos = work->param->unk14;
        work->param->unk16++;
        func_ov173_021b71c4(work, func_ov173_021b7438(work));
        func_ov173_021b70d8(work);
        func_ov173_021b7eb0(work, FALSE);
        func_ov173_021b7f64(work);
        func_ov173_021b7fd0(work, 1);
        func_020260e4(work->cursor, pos);
    } else if (pos == POS_ARROW_UP) {
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        pos = work->param->unk14;
        work->param->unk16--;
        func_ov173_021b71c4(work, func_ov173_021b7438(work));
        func_ov173_021b70d8(work);
        func_ov173_021b7eb0(work, FALSE);
        func_ov173_021b7f64(work);
        func_ov173_021b7fd0(work, -1);
        func_020260e4(work->cursor, pos);
    } else if (pos == POS_RETURN) {
        if (work->prevPos != POS_RETURN) {
            pos = work->param->unk14;
            func_020260e4(work->cursor, pos);
        }
    } else if (pos == POS_TEACH) {
        pos = work->param->unk14;
        func_020260e4(work->cursor, pos);
    } else if (pos == POS_QUIT) {
        if (work->prevPos != POS_RETURN) {
            pos = work->param->unk14;
            func_020260e4(work->cursor, pos);
        }
    }
    func_ov173_021b7adc(work, pos, 3);
}



// Shows or removes the Teach and Quit buttons
// pokebw2's WazaOshie_ShowButtons
static void func_ov173_021b7eb0(WazaOshieWork *work, BOOL show) {
    if (show == TRUE) {
        if (work->buttons[BUTTON_TEACH] == NULL) {
            work->buttons[BUTTON_TEACH] =
                func_02027fec(work->menuRes, &work->buttonItems[BUTTON_TEACH], 13, 21, 9, HEAPID_WAZA_OSHIE);
            work->buttons[BUTTON_QUIT] =
                func_02027fec(work->menuRes, &work->buttonItems[BUTTON_QUIT], 22, 21, 9, HEAPID_WAZA_OSHIE);
            func_0202614c(work->cursor, POS_QUIT);
            work->buttonsShown = TRUE;
        }
    } else {
        if (work->buttons[BUTTON_TEACH] != NULL) {
            func_02028148(work->buttons[BUTTON_TEACH]);
            func_02028148(work->buttons[BUTTON_QUIT]);
            GFL_BGSysLoadScr(4);
            work->buttons[BUTTON_TEACH] = NULL;
            work->buttons[BUTTON_QUIT] = NULL;
        }
        func_02026124(work->cursor, POS_QUIT);
        work->buttonsShown = FALSE;
    }
}

// pokebw2's WazaOshie_AreButtonsShown
BOOL func_ov173_021b7f50(WazaOshieWork *work) {
    if (work->buttons[BUTTON_TEACH] != NULL) {
        return TRUE;
    }
    return FALSE;
}

void func_ov173_021b7f64(WazaOshieWork *work) {
    if (work->moveCount < 4) {
        func_02026124(work->cursor, 4);
        func_02026124(work->cursor, 5);
        return;
    }
    if (work->param->unk16 == 0) {
        func_02026124(work->cursor, 5);
    } else {
        func_0202614c(work->cursor, 5);
    }
    if (work->param->unk16 + 4 >= work->moveCount) {
        func_02026124(work->cursor, 4);
    } else {
        func_0202614c(work->cursor, 4);
    }
}

void func_ov173_021b7fd0(WazaOshieWork *work, s32 direction) {
    func_0204b8c4(work->actors[1], 0);
    func_0204b8c4(work->actors[0], 0);
    if (direction > 0) {
        if (work->param->unk16 + 4 < work->moveCount) {
            func_0204b878(work->actors[0], 10);
        } else {
            func_0204b878(work->actors[0], 23);
        }
        func_0204b878(work->actors[1], 3);
    } else {
        if (work->param->unk16 == 0) {
            func_0204b878(work->actors[1], 24);
        } else {
            func_0204b878(work->actors[1], 11);
        }
        func_0204b878(work->actors[0], 2);
    }
}

void func_ov173_021b8040(WazaOshieWork *work) {
    if (work->moveCount <= 4) {
        func_0204b878(work->actors[1], 17);
        func_0204b878(work->actors[0], 16);
    } else if (work->param->unk16 + 4 < work->moveCount) {
        func_0204b878(work->actors[1], 17);
        func_0204b878(work->actors[0], 2);
    } else if (work->param->unk16 == 0) {
        func_0204b878(work->actors[1], 3);
        func_0204b878(work->actors[0], 16);
    }
}




// Start the animation of the return icon or the flash of a button, after which the process goes to seq
// pokebw2's WazaOshie_DecideActor
u32 func_ov173_021b80a4(WazaOshieWork *work, u16 seq) {
    WazaOshieDecide *decide = &work->decide;

    decide->type = DECIDE_ACTOR;
    decide->index = ACTOR_RETURN;
    decide->seq = seq;
    func_0204b878(work->actors[ACTOR_RETURN], 9);
    return SEQ_DECIDE_WAIT;
}

// pokebw2's WazaOshie_DecideTeach
u32 func_ov173_021b80c4(WazaOshieWork *work, u16 seq) {
    WazaOshieDecide *decide = &work->decide;

    decide->type = DECIDE_BUTTON;
    decide->index = BUTTON_TEACH;
    decide->seq = seq;
    func_0202822c(work->buttons[decide->index], TRUE);
    return SEQ_DECIDE_WAIT;
}

// pokebw2's WazaOshie_DecideQuit
u32 func_ov173_021b80ec(WazaOshieWork *work, u16 seq) {
    WazaOshieDecide *decide = &work->decide;

    decide->type = DECIDE_BUTTON;
    decide->index = BUTTON_QUIT;
    decide->seq = seq;
    func_0202822c(work->buttons[decide->index], TRUE);
    return SEQ_DECIDE_WAIT;
}

// pokebw2's WazaOshie_SeqDecideWait
static u32 func_ov173_021b8110(WazaOshieWork *work) {
    WazaOshieDecide *decide = &work->decide;

    if (decide->type == DECIDE_ACTOR) {
        if (!func_0204b98c(work->actors[decide->index])) {
            return decide->seq;
        }
    } else if (decide->type == DECIDE_BUTTON) {
        if (func_02028230(work->buttons[decide->index])) {
            func_ov173_021b7eb0(work, FALSE);
            return decide->seq;
        }
    }
    return SEQ_DECIDE_WAIT;
}

// pokebw2's WazaOshie_SeqQuit
static u32 func_ov173_021b815c(WazaOshieWork *work) {
    func_ov173_021b7460(work, MSG_QUIT);
    work->yesNo = YES_NO_QUIT;
    work->nextSeq = SEQ_YES_NO_INIT;
    return SEQ_MESSAGE;
}

// pokebw2's WazaOshie_SeqMoveChosen
static u32 func_ov173_021b817c(WazaOshieWork *work) {
    u8 yesNo;

    if (func_ov173_021b7568(work) < 4) {
        func_ov173_021b7460(work, MSG_LEARN);
        yesNo = YES_NO_LEARN;
    } else {
        func_ov173_021b7460(work, MSG_REPLACE);
        yesNo = YES_NO_REPLACE;
    }
    work->yesNo = yesNo;
    work->nextSeq = SEQ_YES_NO_INIT;
    return SEQ_MESSAGE;
}

// Darkens the touch screen behind the yes/no menu
// pokebw2's WazaOshie_SetDimmed
static void func_ov173_021b81b0(BOOL dimmed) {
    if (dimmed == TRUE) {
        G2S_SetBlendBrightness(
            GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, -8);
    } else {
        G2S_BlendNone();
    }
}
