// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov141/bsubway_records.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/bsubway_records.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
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
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "save/bsubway_save.h"
#include "system/app_menu_common.h"
#include "system/bgwinfrm.h"
#include "system/cursor_move.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/pms_data.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "text/system/title_6.h"

// bsubway_records.c, a descriptive name: the Battle Subway's records screen, overlay 141. It lists the records that the
// Battle Subway's save holds, ten to a page on the main screen's frames, and shows the one under the cursor below

// The steps of the screen
enum {
    STATE_FADE_IN,
    STATE_INPUT,
    STATE_WAIT_BACK,
    STATE_FADE_OUT,
    STATE_WAIT_FADE_OUT,
};

#define RECORDS_PER_PAGE 10
#define MESSAGE_COUNT 7
#define WINDOW_COUNT 18
// The windows that draw the list's frames come first, then the ones for the record under the cursor
#define FRAME_WINDOW_COUNT RECORDS_PER_PAGE
#define INFO_WINDOW_COUNT 8
#define BG_COUNT 5

// The windows of the record under the cursor
enum {
    INFO_WINDOW_TITLE = FRAME_WINDOW_COUNT,
    INFO_WINDOW_RANK,
    INFO_WINDOW_TRAIN_NUMBER,
    INFO_WINDOW_NAME,
    INFO_WINDOW_LOCATION_LABEL,
    INFO_WINDOW_LOCATION,
    INFO_WINDOW_QUOTE_LABEL,
    INFO_WINDOW_QUOTE,
};

// The actors: the cursor, the back button and the page arrows
enum {
    ACTOR_CURSOR,
    ACTOR_BACK,
    ACTOR_ARROW_LEFT,
    ACTOR_ARROW_RIGHT,
    ACTOR_COUNT,
};

// The pages' arrows
#define PAGE_PREV 0
#define PAGE_NEXT 1

// What the touch and key input gives besides the cursor's choices
#define INPUT_PREV_PAGE 0
#define INPUT_NEXT_PAGE 1
#define INPUT_BACK 2
#define INPUT_NONE (-1)

// A record of the Battle Subway's save, which func_0200df58 copies: a player's name and place, and the message that
// they left
typedef struct {
    u16 name[8];
    u8 unk10[2];
    u8 country;
    u8 area;
    u8 unk14[4];
    PMSData message;
    u8 unk20_0 : 1;
    u8 female : 1;
    u8 unk20_2 : 6;
    u8 unk21;
} BSubwayRecord;

typedef struct {
    u32 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u16 palette;
    u16 baseChar;
} BSubwayRecordsWindow;

typedef struct {
    int x;
    int y;
    int sequence;
    u32 charIndex;
    u32 palIndex;
    u32 cellIndex;
} BSubwayRecordsActor;

typedef struct {
    ArcTool *arc;
    BGWinFrame *frames;
    BSubwayRecord *records;
    Font *font;
    BmpWin *windows[WINDOW_COUNT];
    PrintWindow printWindows[WINDOW_COUNT];
    PrintQueue *printQueue;
    u8 unkEC[0x8];
    MsgData *msgData;
    WordSet *wordSet;
    StrBuf *strbuf;
    StrBuf *messages[MESSAGE_COUNT];
    ClActUnit *clUnits[2];
    ClActor *actors[ACTOR_COUNT];
    u32 objRes[6];
    CursorMove *cursor;
    TCB *vblankTcb;
    BSubwayRecordsParam *param;
    int state;
    u32 unk15C;
    u16 page;
    u16 pageCount;
    int count;
} BSubwayRecordsWork;

BOOL func_ov069_021b95a0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov069_021b9658(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov069_021b9680(GameProc *proc, u32 *state, void *param, void *work);
void func_ov069_021b96c8(TCB *tcb, void *data);
void func_ov069_021b96d4(BSubwayRecordsWork *wk, BSubwayRecordsParam *param);
void func_ov069_021b9784(BSubwayRecordsWork *wk);
void func_ov069_021b97d4(void);
void func_ov069_021b97f4(BSubwayRecordsWork *wk);
void func_ov069_021b9888(BSubwayRecordsWork *wk);
void func_ov069_021b98ac(BSubwayRecordsWork *wk);
void func_ov069_021b9a44(BSubwayRecordsWork *wk);
void func_ov069_021b9a50(BSubwayRecordsWork *wk);
static void func_ov069_021b9b44(BSubwayRecordsWork *wk);
void func_ov069_021b9bf0(BSubwayRecordsWork *wk);
void func_ov069_021b9c44(BSubwayRecordsWork *wk);
void func_ov069_021b9cd8(BSubwayRecordsWork *wk);
void func_ov069_021b9d08(BSubwayRecordsWork *wk);
void func_ov069_021b9de8(BSubwayRecordsWork *wk);
static void func_ov069_021b9dfc(BSubwayRecordsWork *wk);
static BOOL func_ov069_021b9e44(BSubwayRecordsWork *wk);
static BOOL func_ov069_021b9e5c(BSubwayRecordsWork *wk);
static BOOL func_ov069_021b9e74(BSubwayRecordsWork *wk);
static BOOL func_ov069_021b9ec0(BSubwayRecordsWork *wk);
BOOL func_ov069_021b9edc(BSubwayRecordsWork *wk);
static BOOL func_ov069_021b9f08(BSubwayRecordsWork *wk);
void func_ov069_021b9f24(BSubwayRecordsWork *wk);
void func_ov069_021b9f58(BSubwayRecordsWork *wk);
void func_ov069_021b9f68(void *work, int pos, int prevPos);
void func_ov069_021b9f6c(void *work, int pos, int prevPos);
void func_ov069_021b9f70(void *work, int pos, int prevPos);
void func_ov069_021b9f9c(void *work, int pos, int prevPos);
void func_ov069_021b9fc8(BSubwayRecordsWork *wk, int pos);
void func_ov069_021b9ff0(BSubwayRecordsWork *wk, BSubwayRecord *records, int index);
int func_ov069_021ba2a4(BSubwayRecordsWork *wk);
void func_ov069_021ba2d8(BSubwayRecordsWork *wk, int input);
void func_ov069_021ba350(BSubwayRecordsWork *wk);
static void func_ov069_021ba384(BSubwayRecordsWork *wk, int page, int pageCount);
static BOOL func_ov069_021ba400(BSubwayRecordsWork *wk, int direction);
static void func_ov069_021ba438(BSubwayRecordsWork *wk, int page);
int func_ov069_021ba470(int page, int pageCount, int count);
void func_ov069_021ba480(BSubwayRecordsWork *wk);
void func_ov069_021ba520(BSubwayRecordsWork *wk, int index, StrBuf *name, BOOL female);

// Declared in an order that gives the original layout of the data (tools/decomp/rodata_order.py)

// pokebw2's sBSubwayRecordsWindow5
static const BSubwayRecordsWindow data_ov069_021ba5d4 = { 4, 2, 12, 28, 4, 1, 0xc8 };
// pokebw2's sBSubwayRecordsWindow4
static const BSubwayRecordsWindow data_ov069_021ba5c8 = { 4, 2, 10, 28, 2, 1, 0x90 };
// pokebw2's sBSubwayRecordsWindow1
static const BSubwayRecordsWindow data_ov069_021ba58c = { 4, 2, 3, 11, 2, 1, 0x44 };
// pokebw2's sBSubwayRecordsWindow6
static const BSubwayRecordsWindow data_ov069_021ba5a4 = { 4, 2, 16, 16, 2, 1, 0x138 };
// pokebw2's sBSubwayRecordsWindow7
static const BSubwayRecordsWindow data_ov069_021ba5bc = { 4, 2, 18, 28, 4, 1, 0x158 };
// pokebw2's sBSubwayRecordsWindow3
static const BSubwayRecordsWindow data_ov069_021ba5e0 = { 4, 2, 8, 10, 2, 1, 0x7c };
// pokebw2's sBSubwayRecordsWindow2
static const BSubwayRecordsWindow data_ov069_021ba598 = { 4, 13, 3, 17, 2, 1, 0x5a };
// pokebw2's sBSubwayRecordsWindow0
static const BSubwayRecordsWindow data_ov069_021ba5b0 = { 4, 2, 1, 24, 2, 1, 0x14 };

// pokebw2's sBSubwayRecordsCursorCallbacks
static const CursorMoveCallbacks data_ov069_021ba5ec = {
    func_ov069_021b9f68,
    func_ov069_021b9f6c,
    func_ov069_021b9f70,
    func_ov069_021b9f9c,
};

// The buttons under the list: the left and right arrows and the back button
// pokebw2's sBSubwayRecordsTouchRects
static const TouchRect data_ov069_021ba5fc[] = {
#include "asset/bsubway_records_data_ov069_021ba5fc.inc"
};

// pokebw2's sBSubwayRecordsBGs
static const u32 data_ov069_021ba60c[BG_COUNT] = { 4, 5, 0, 1, 2 };

// pokebw2's sBSubwayRecordsVRAMConfig
static const BGSysVRAMConfig data_ov069_021ba620 = {
    GX_VRAM_BG_128_B, GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_64_E, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_A,  GX_VRAM_TEXPLTT_01_FG,   GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's BSUBWAY_RECORDS_PROC_FUNCTIONS
const GameProcFunctions data_ov069_021ba650 = {
    func_ov069_021b95a0,
    func_ov069_021b9658,
    func_ov069_021b9680,
};

// Where the list's frames go on their BG: two columns of five
// pokebw2's sBSubwayRecordsFramePositions
static const int data_ov069_021ba66c[RECORDS_PER_PAGE][2] = {
#include "asset/bsubway_records_data_ov069_021ba66c.inc"
};

// pokebw2's sBSubwayRecordsActors
static const BSubwayRecordsActor data_ov069_021ba6bc[ACTOR_COUNT] = {
#include "asset/bsubway_records_data_ov069_021ba6bc.inc"
};
// The cursor's positions: the ten frames, then the end
// pokebw2's sBSubwayRecordsCursorData
static const CursorMoveData data_ov069_021ba71c[RECORDS_PER_PAGE + 1] = {
#include "asset/bsubway_records_data_ov069_021ba71c.inc"
};

// pokebw2's sBSubwayRecordsStateFuncs
static BOOL (*data_ov069_021ba860[])(BSubwayRecordsWork *wk) = {
    func_ov069_021b9e5c,  func_ov069_021b9e74,       func_ov069_021b9ec0,
    func_ov069_021b9edc, func_ov069_021b9f08,
};

// pokebw2's sBSubwayRecordsWindows
static const BSubwayRecordsWindow *data_ov069_021ba874[INFO_WINDOW_COUNT] = {
    &data_ov069_021ba5b0, &data_ov069_021ba58c, &data_ov069_021ba598, &data_ov069_021ba5e0,
    &data_ov069_021ba5c8, &data_ov069_021ba5d4, &data_ov069_021ba5a4, &data_ov069_021ba5bc,
};

// pokebw2's BSubwayRecords_Main

#define HEAPID_BSUBWAY_RECORDS 0x80

BOOL func_ov069_021b95a0(GameProc *proc, u32 *state, void *param, void *work) {
    BSubwayRecordsWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_BSUBWAY_RECORDS, 0x20000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(BSubwayRecordsWork), HEAPID_BSUBWAY_RECORDS);
    MIi_CpuClearFast(0, wk, sizeof(BSubwayRecordsWork));
    func_0202223c(0, 0);
    func_0202223c(1, 0);
    func_ov069_021b96d4(wk, param);
    func_ov069_021b97d4();
    func_ov069_021b97f4(wk);
    func_ov069_021b98ac(wk);
    func_ov069_021b9a50(wk);
    func_ov069_021b9b44(wk);
    func_ov069_021b9c44(wk);
    func_ov069_021b9d08(wk);
    func_ov069_021b9f24(wk);
    func_ov069_021ba384(wk, wk->page, wk->pageCount);
    func_ov069_021ba438(wk, wk->page);
    GFL_WipeSet(0, 1, 1, 0, 16, TRUE, HEAPID_BSUBWAY_RECORDS);
    func_0203d34c(TRUE, HEAPID_BSUBWAY_RECORDS);
    return TRUE;
}

static BOOL func_ov069_021b9658(GameProc *proc, u32 *state, void *param, void *work) {
    BSubwayRecordsWork *wk = work;

    if (!func_ov069_021b9e44(wk)) {
        return TRUE;
    }
    func_ov069_021b9dfc(wk);
    func_0202cd94(wk->frames);
    func_0204a600();
    return FALSE;
}

BOOL func_ov069_021b9680(GameProc *proc, u32 *state, void *param, void *work) {
    BSubwayRecordsWork *wk = work;

    func_ov069_021b9f58(wk);
    func_ov069_021b9de8(wk);
    func_ov069_021b9cd8(wk);
    func_ov069_021b9bf0(wk);
    func_ov069_021b9a44(wk);
    func_ov069_021b9888(wk);
    func_ov069_021b9784(wk);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapStatusValidate(0x80);
    GFL_HeapDelete(0x80);
    return TRUE;
}


// pokebw2's BSubwayRecords_VBlank
void func_ov069_021b96c8(TCB *tcb, void *data) {
    func_0204a648();
    GFL_BGSysUpdate();
}

void func_ov069_021b96d4(BSubwayRecordsWork *wk, BSubwayRecordsParam *param) {
    int i;

    wk->param = param;
    wk->unk15C = 0;
    wk->arc = GFL_ArcSysCreateFileHandle(ARCID_BSUBWAY_RECORDS, HEAPID_BSUBWAY_RECORDS);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_TITLE_6, HEAPID_BSUBWAY_RECORDS);
    for (i = 0; i < MESSAGE_COUNT; i++) {
        wk->messages[i] = GFL_MsgDataLoadStrbufNew(wk->msgData, i);
        GFL_StrBufGetStringPtr(wk->messages[i]);
    }
    wk->strbuf = GFL_StrBufCreate(0x40, HEAPID_BSUBWAY_RECORDS);
    wk->wordSet = GFL_WordSetSystemCreate(8, 0x40, HEAPID_BSUBWAY_RECORDS);
    wk->vblankTcb = GFL_VBlankTCBAdd(func_ov069_021b96c8, wk, 0);
    wk->records = func_0200df58(func_02012efc(wk->param->gameData), HEAPID_BSUBWAY_RECORDS);
    wk->count = func_0200df94(wk->records);
    func_ov069_021ba350(wk);
}


// pokebw2's BSubwayRecords_FreeData
void func_ov069_021b9784(BSubwayRecordsWork *wk) {
    int i;

    GFL_HeapFree(wk->records);
    for (i = 0; i < MESSAGE_COUNT; i++) {
        GFL_StrBufFree(wk->messages[i]);
    }
    GFL_StrBufFree(wk->strbuf);
    GFL_WordSetSystemFree(wk->wordSet);
    GFL_MsgDataFree(wk->msgData);
    GFL_TCBRemove(wk->vblankTcb);
    GFL_ArcToolFree(wk->arc);
}

// pokebw2's BSubwayRecords_InitVRAM
void func_ov069_021b97d4(void) {
    GFL_BGSysSetVRAMBanks(&data_ov069_021ba620);
    reg_GX_POWCNT = reg_GX_POWCNT & ~REG_GX_POWCNT_DSEL_MASK;
}

void func_ov069_021b97f4(BSubwayRecordsWork *wk) {
    GFL_BGSysCreate(0x80);
    {
        BGSysLCDConfig lcdConfig = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

        GFL_BGSysSetLCDConfig(&lcdConfig);
    }
    {
        BGSetup setups[BG_COUNT] = {
            { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, 0x8000,
              GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
            { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x10000, 0x4000,
              GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
            { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, 0x4000,
              GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
            { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x10000, 0x8000,
              GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
            { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x08000, 0x4000,
              GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
        };
        u32 i;

        for (i = 0; i < BG_COUNT; i++) {
            u32 bg = data_ov069_021ba60c[i];

            GFL_BGSysCreateBG(bg, &setups[i], BGMODE_TEXT);
            GFL_BGSysClearScr(bg);
            GFL_BGSysClearCharCore(bg, BGSYS_TILE_SIZE_16, 0, 0x80);
            GFL_BGSysSetBGEnabled(bg, TRUE);
        }
    }
    GFL_BGSysSetBGEnabled(6, FALSE);
    GFL_BGSysSetBGEnabled(7, FALSE);
    GFL_BGSysSetBGEnabled(3, FALSE);
}


// pokebw2's BSubwayRecords_FreeBG
void func_ov069_021b9888(BSubwayRecordsWork *wk) {
    u32 i;

    for (i = 0; i < BG_COUNT; i++) {
        GFL_BGSysReleaseBG(data_ov069_021ba60c[i]);
    }
    GFL_BGSysFree();
}

void func_ov069_021b98ac(BSubwayRecordsWork *wk) {
    ArcTool *arc = wk->arc;
    ArcTool *uiArc;
    u32 offset;
    int i;

    GFL_BGSysLoadArcNCGRStatic(arc, 5, 5, 0, 0, TRUE, HEAPID_BSUBWAY_RECORDS);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 4, 5, 0, 0, TRUE, HEAPID_BSUBWAY_RECORDS);
    GFL_G2DIOLoadArcNCLRDefault(arc, 6, 4, 0, 0, HEAPID_BSUBWAY_RECORDS);
    GFL_BGSysLoadArcNCGRStatic(arc, 1, 2, 0, 0, TRUE, HEAPID_BSUBWAY_RECORDS);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0, 2, 0, 0, TRUE, HEAPID_BSUBWAY_RECORDS);
    GFL_G2DIOLoadArcNCLRDefault(arc, 2, 0, 0, 0, HEAPID_BSUBWAY_RECORDS);
    GFL_BGSysLoadArcNCGRStatic(arc, 1, 1, 0, 0, TRUE, HEAPID_BSUBWAY_RECORDS);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0, 1, 0, 0, TRUE, HEAPID_BSUBWAY_RECORDS);
    uiArc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), HEAPID_BSUBWAY_RECORDS);
    GFL_BGSysLoadArcNCGRStatic(uiArc, func_02027628(), 1, 0, 0, FALSE, HEAPID_BSUBWAY_RECORDS);
    loadBGScrToVramByFileNoReserveNegAlign(uiArc, func_0202762c(), 1, 0, 0, FALSE, HEAPID_BSUBWAY_RECORDS);
    offset = 0x1c0;
    GFL_G2DIOLoadArcNCLR(uiArc, func_02027624(), 0, 0, offset, 0x20, HEAPID_BSUBWAY_RECORDS);
    GFL_BGSysSetScrPaletteNo(1, 0, 21, 32, 3, 14);
    GFL_BGSysLoadScr(1);
    GFL_ArcToolFree(uiArc);
    offset += 0x20;
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, offset, 0x20, HEAPID_BSUBWAY_RECORDS);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, offset, 0x20, HEAPID_BSUBWAY_RECORDS);
    wk->frames = func_0202c9b4(BGWINFRAME_TRANSFER_VBLANK, RECORDS_PER_PAGE, HEAPID_BSUBWAY_RECORDS);
    for (i = 0; i < RECORDS_PER_PAGE; i++) {
        func_0202ca3c(wk->frames, i, 1, 14, 4);
        func_0202cb18(wk->frames, i, arc, 3, TRUE);
        func_0202cb4c(wk->frames, i, data_ov069_021ba66c[i][0], data_ov069_021ba66c[i][1]);
    }
}


// pokebw2's BSubwayRecords_FreeFrames
void func_ov069_021b9a44(BSubwayRecordsWork *wk) {
    func_0202ca0c(wk->frames);
}

void func_ov069_021b9a50(BSubwayRecordsWork *wk) {
    ArcTool *uiArc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), HEAPID_BSUBWAY_RECORDS);

    ClActSys_Create(&data_020a1448, &data_ov069_021ba620, HEAPID_BSUBWAY_RECORDS);
    wk->clUnits[0] = func_0204b100(4, 1, HEAPID_BSUBWAY_RECORDS);
    wk->clUnits[1] = func_0204b100(2, 1, HEAPID_BSUBWAY_RECORDS);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    wk->objRes[1] = func_0204a6c8(wk->arc, 7, TRUE, CLACT_VRAM_BOTH, HEAPID_BSUBWAY_RECORDS);
    wk->objRes[0] = func_0204abf0(wk->arc, 10, CLACT_VRAM_BOTH, 0, HEAPID_BSUBWAY_RECORDS);
    wk->objRes[2] = func_0204af28(wk->arc, 8, 9, HEAPID_BSUBWAY_RECORDS);
    wk->objRes[4] = func_0204a6c8(uiArc, func_02027618(), FALSE, CLACT_VRAM_MAIN, HEAPID_BSUBWAY_RECORDS);
    wk->objRes[3] = func_0204ac18(uiArc, func_02027614(), CLACT_VRAM_MAIN, 0x160, 0, 3, HEAPID_BSUBWAY_RECORDS);
    wk->objRes[5] = func_0204af28(uiArc, func_0202761c(0), func_02027620(0), HEAPID_BSUBWAY_RECORDS);
    GFL_ArcToolFree(uiArc);
}


static void func_ov069_021b9b44(BSubwayRecordsWork *wk) {
    int i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        const BSubwayRecordsActor *def = &data_ov069_021ba6bc[i];
        ClActorSetup setup;

        setup.x = def->x;
        setup.y = def->y;
        setup.sequence = def->sequence;
        setup.bgPriority = 0;
        setup.priority = 0;
        wk->actors[i] = func_0204b294(wk->clUnits[0], wk->objRes[def->charIndex], wk->objRes[def->palIndex],
                                      wk->objRes[def->cellIndex], &setup, 0, HEAPID_BSUBWAY_RECORDS);
        func_0204b92c(wk->actors[i], TRUE);
    }
}


// pokebw2's BSubwayRecords_FreeObjRes
void func_ov069_021b9bf0(BSubwayRecordsWork *wk) {
    func_0204a8d4(wk->objRes[4]);
    func_0204ada4(wk->objRes[3]);
    func_0204afd8(wk->objRes[5]);
    func_0204a8d4(wk->objRes[1]);
    func_0204ada4(wk->objRes[0]);
    func_0204afd8(wk->objRes[2]);
    func_0204b1cc(wk->clUnits[1]);
    func_0204b1cc(wk->clUnits[0]);
    func_0204a5ac();
}

void func_ov069_021b9c44(BSubwayRecordsWork *wk) {
    int i;
    int j;

    BmpWin_InitAllocator(0x80);
    j = 0;
    for (i = 0; i < INFO_WINDOW_COUNT; i++) {
        BmpWin *win;

        wk->windows[FRAME_WINDOW_COUNT + i] = BmpWin_CreateDynamic(data_ov069_021ba874[i]->bg, data_ov069_021ba874[i]->x, data_ov069_021ba874[i]->y, data_ov069_021ba874[i]->width, data_ov069_021ba874[i]->height, data_ov069_021ba874[i]->palette, TRUE);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[FRAME_WINDOW_COUNT + i]), 0);
        win = wk->windows[FRAME_WINDOW_COUNT + i];
        BmpWin_FlushChar(win);
        BmpWin_FlushMap(win);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(win));
    }
    for (j = 0; j < FRAME_WINDOW_COUNT; j++) {
        wk->windows[j] = BmpWin_CreateDynamic(1, 2, 1, 10, 2, 1, TRUE);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[j]), 4);
    }
}


// pokebw2's BSubwayRecords_FreeWindows
void func_ov069_021b9cd8(BSubwayRecordsWork *wk) {
    int i;

    for (i = 0; i < FRAME_WINDOW_COUNT; i++) {
        BmpWin_Free(wk->windows[i]);
    }
    for (i = 0; i < INFO_WINDOW_COUNT; i++) {
        BmpWin_Free(wk->windows[FRAME_WINDOW_COUNT + i]);
    }
    BmpWin_FreeAllocator();
}

void func_ov069_021b9d08(BSubwayRecordsWork *wk) {
    int i;

    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_BSUBWAY_RECORDS);
    wk->printQueue = func_0201c440(HEAPID_BSUBWAY_RECORDS);
    for (i = 0; i < WINDOW_COUNT; i++) {
        PrintWindow_Init(&wk->printWindows[i], wk->windows[i]);
    }
    PrintWindow_Print(&wk->printWindows[INFO_WINDOW_TITLE], wk->printQueue, 0, 0,
                      wk->messages[Title6_Text_000], wk->font, PRINT_COLOR(15, 14, 0));
    PrintWindow_Print(&wk->printWindows[INFO_WINDOW_LOCATION_LABEL], wk->printQueue, 0, 0,
                      wk->messages[Title6_Text_003], wk->font, PRINT_COLOR(15, 14, 0));
    PrintWindow_Print(&wk->printWindows[INFO_WINDOW_QUOTE_LABEL], wk->printQueue, 0, 0,
                      wk->messages[Title6_Text_006], wk->font, PRINT_COLOR(15, 14, 0));
}


// pokebw2's BSubwayRecords_FreeText
void func_ov069_021b9de8(BSubwayRecordsWork *wk) {
    func_0201c4c0(wk->printQueue);
    GFL_FontFree(wk->font);
}

// pokebw2's BSubwayRecords_FlushText
static void func_ov069_021b9dfc(BSubwayRecordsWork *wk) {
    int i;

    func_0201c4e4(wk->printQueue);
    for (i = 0; i < WINDOW_COUNT; i++) {
        PrintWindow_Flush(&wk->printWindows[i], wk->printQueue);
    }
}

// pokebw2's BSubwayRecords_RunState
static BOOL func_ov069_021b9e44(BSubwayRecordsWork *wk) {
    return data_ov069_021ba860[wk->state](wk);
}

// pokebw2's BSubwayRecords_StateFadeIn
static BOOL func_ov069_021b9e5c(BSubwayRecordsWork *wk) {
    if (GFL_WipeIsFinished()) {
        wk->state = STATE_INPUT;
    }
    return TRUE;
}

// pokebw2's BSubwayRecords_StateInput
static BOOL func_ov069_021b9e74(BSubwayRecordsWork *wk) {
    int input;

    if (!func_0201c6b4(wk->printQueue)) {
        return TRUE;
    }
    input = func_ov069_021ba2a4(wk);
    if (input == INPUT_NONE) {
        input = func_02025d2c(wk->cursor);
        if (input == CURSOR_MOVE_EDGE_LEFT) {
            func_ov069_021ba2d8(wk, INPUT_PREV_PAGE);
        } else if (input == CURSOR_MOVE_EDGE_RIGHT) {
            func_ov069_021ba2d8(wk, INPUT_NEXT_PAGE);
        }
    }
    return TRUE;
}

// pokebw2's BSubwayRecords_StateWaitBack
static BOOL func_ov069_021b9ec0(BSubwayRecordsWork *wk) {
    if (!func_0204b98c(wk->actors[ACTOR_BACK])) {
        wk->state = STATE_FADE_OUT;
    }
    return TRUE;
}

BOOL func_ov069_021b9edc(BSubwayRecordsWork *wk) {
    GFL_WipeSet(0, 0, 0, 0, 0x10, TRUE, 0x80);
    wk->state = STATE_WAIT_FADE_OUT;
    return TRUE;
}


// pokebw2's BSubwayRecords_StateWaitFadeOut
static BOOL func_ov069_021b9f08(BSubwayRecordsWork *wk) {
    if (GFL_WipeIsFinished()) {
        wk->state = STATE_FADE_IN;
        return FALSE;
    }
    return TRUE;
}

void func_ov069_021b9f24(BSubwayRecordsWork *wk) {
    wk->cursor = func_02025cd0(data_ov069_021ba71c, &data_ov069_021ba5ec, wk, TRUE, 0, 0x80);
    func_ov069_021b9fc8(wk, 0);
}


// pokebw2's BSubwayRecords_FreeCursor
void func_ov069_021b9f58(BSubwayRecordsWork *wk) {
    func_02025d14(wk->cursor);
}

// pokebw2's BSubwayRecords_OnCursorOn
void func_ov069_021b9f68(void *work, int pos, int prevPos) {
}

// pokebw2's BSubwayRecords_OnCursorOff
void func_ov069_021b9f6c(void *work, int pos, int prevPos) {
}

// pokebw2's BSubwayRecords_OnCursorMove
void func_ov069_021b9f70(void *work, int pos, int prevPos) {
    BSubwayRecordsWork *wk = work;

    func_ov069_021b9ff0(wk, wk->records, wk->page * RECORDS_PER_PAGE + pos);
    func_ov069_021b9fc8(wk, pos);
    GFL_SndSEPlay(SEQ_SE_SELECT1);
}

// pokebw2's BSubwayRecords_OnCursorTouch
void func_ov069_021b9f9c(void *work, int pos, int prevPos) {
    BSubwayRecordsWork *wk = work;

    func_ov069_021b9ff0(wk, wk->records, wk->page * RECORDS_PER_PAGE + pos);
    func_ov069_021b9fc8(wk, pos);
    GFL_SndSEPlay(SEQ_SE_SELECT1);
}

// pokebw2's BSubwayRecords_MoveCursorActor
void func_ov069_021b9fc8(BSubwayRecordsWork *wk, int pos) {
    const CursorMoveData *data = func_0202616c(wk->cursor, pos);
    ClActorPos actorPos;

    actorPos.x = data->x;
    actorPos.y = data->y;
    func_0204b404(wk->actors[ACTOR_CURSOR], &actorPos, 0);
}

void func_ov069_021b9ff0(BSubwayRecordsWork *wk, BSubwayRecord *records, int index) {
    u32 country = records[index].country;
    u32 area = records[index].area;
    PMSData *message = &records[index].message;
    StrBuf *name = GFL_StrBufCreate(0x10, HEAPID_BSUBWAY_RECORDS);

    WordSetNumber(wk->wordSet, 0, wk->param->rank, 2, 0, TRUE);
    WordSetNumber(wk->wordSet, 1, wk->param->trainNumber, 3, 0, TRUE);
    loadCountryToStrbuf(wk->wordSet, 2, country);
    loadCountryAreaToStrbuf(wk->wordSet, 3, country, area);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[INFO_WINDOW_RANK]), 0);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[INFO_WINDOW_TRAIN_NUMBER]), 0);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[INFO_WINDOW_NAME]), 0);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[INFO_WINDOW_LOCATION]), 0);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[INFO_WINDOW_QUOTE]), 0);
    if (index < wk->count) {
        u16 color = PRINT_COLOR(15, 14, 0);
        StrBuf *text;

        GFL_StrBufLoadFixedString(name, records[index].name, 8);
        PrintWindow_Print(&wk->printWindows[INFO_WINDOW_NAME], wk->printQueue, 0, 0, name, wk->font, color);
        GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, wk->messages[Title6_Text_001]);
        PrintWindow_Print(&wk->printWindows[INFO_WINDOW_RANK], wk->printQueue, 0, 0, wk->strbuf, wk->font, color);
        GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, wk->messages[Title6_Text_002]);
        PrintWindow_Print(&wk->printWindows[INFO_WINDOW_TRAIN_NUMBER], wk->printQueue, 0, 0, wk->strbuf, wk->font,
                          color);
        GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, wk->messages[Title6_Text_004]);
        PrintWindow_Print(&wk->printWindows[INFO_WINDOW_LOCATION], wk->printQueue, 0, 0, wk->strbuf, wk->font, color);
        GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, wk->messages[Title6_Text_005]);
        PrintWindow_Print(&wk->printWindows[INFO_WINDOW_LOCATION], wk->printQueue, 0, 16, wk->strbuf, wk->font, color);
        text = func_0202435c(message, HEAPID_BSUBWAY_RECORDS);
        PrintWindow_Print(&wk->printWindows[INFO_WINDOW_QUOTE], wk->printQueue, 0, 0, text, wk->font, color);
        GFL_StrBufFree(text);
    } else {
        BmpWin_Transfer(wk->windows[INFO_WINDOW_NAME]);
        BmpWin_Transfer(wk->windows[INFO_WINDOW_LOCATION]);
        BmpWin_Transfer(wk->windows[INFO_WINDOW_QUOTE]);
    }
    GFL_StrBufFree(name);
}


int func_ov069_021ba2a4(BSubwayRecordsWork *wk) {
    int input = func_02035c08(data_ov069_021ba5fc);

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        GFL_SndSEPlay(0x551);
        input = INPUT_BACK;
    }
    func_ov069_021ba2d8(wk, input);
    return input;
}


// pokebw2's BSubwayRecords_HandleInput
void func_ov069_021ba2d8(BSubwayRecordsWork *wk, int input) {
    switch (input) {
    case INPUT_PREV_PAGE:
        if (func_ov069_021ba400(wk, input)) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_0204b878(wk->actors[ACTOR_ARROW_LEFT], 12);
            func_ov069_021ba438(wk, wk->page);
        }
        break;
    case INPUT_NEXT_PAGE:
        if (func_ov069_021ba400(wk, input)) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_0204b878(wk->actors[ACTOR_ARROW_RIGHT], 13);
            func_ov069_021ba438(wk, wk->page);
        }
        break;
    case INPUT_BACK:
        wk->state = STATE_WAIT_BACK;
        func_0204b878(wk->actors[ACTOR_BACK], 9);
        break;
    }
}

// pokebw2's BSubwayRecords_InitPages
void func_ov069_021ba350(BSubwayRecordsWork *wk) {
    wk->page = 0;
    wk->pageCount = wk->count / RECORDS_PER_PAGE;
    if (wk->count % RECORDS_PER_PAGE != 0) {
        wk->pageCount++;
    }
}

// Shows the page's arrows when there is a page to go to
// pokebw2's BSubwayRecords_UpdateArrows
static void func_ov069_021ba384(BSubwayRecordsWork *wk, int page, int pageCount) {
    if (pageCount == 1) {
        func_0204b3dc(wk->actors[ACTOR_ARROW_LEFT], FALSE);
        func_0204b3dc(wk->actors[ACTOR_ARROW_RIGHT], FALSE);
        return;
    }
    if (page == 0) {
        func_0204b3dc(wk->actors[ACTOR_ARROW_LEFT], FALSE);
        func_0204b3dc(wk->actors[ACTOR_ARROW_RIGHT], TRUE);
    } else if (page != 0 && page < pageCount - 1) {
        func_0204b3dc(wk->actors[ACTOR_ARROW_LEFT], TRUE);
        func_0204b3dc(wk->actors[ACTOR_ARROW_RIGHT], TRUE);
    } else if (page == pageCount - 1) {
        func_0204b3dc(wk->actors[ACTOR_ARROW_LEFT], TRUE);
        func_0204b3dc(wk->actors[ACTOR_ARROW_RIGHT], FALSE);
    }
}

// Turns the page back or forward if there is one, and returns whether it did
// pokebw2's BSubwayRecords_StepPage
static BOOL func_ov069_021ba400(BSubwayRecordsWork *wk, int direction) {
    BOOL moved = FALSE;

    if (direction == PAGE_PREV) {
        if (wk->page != 0) {
            wk->page--;
            moved = TRUE;
        }
    } else if (direction == PAGE_NEXT) {
        if (wk->page < wk->pageCount - 1) {
            wk->page++;
            moved = TRUE;
        }
    }
    return moved;
}

// pokebw2's BSubwayRecords_ShowPage
static void func_ov069_021ba438(BSubwayRecordsWork *wk, int page) {
    wk->page = page;
    func_ov069_021ba384(wk, page, wk->pageCount);
    func_ov069_021ba480(wk);
    func_ov069_021b9ff0(wk, wk->records, wk->page * RECORDS_PER_PAGE + func_020260e0(wk->cursor));
}

// The number of records on a page, which is ten but for the last page
// pokebw2's BSubwayRecords_GetPageSize
int func_ov069_021ba470(int page, int pageCount, int count) {
    int size = count - page * RECORDS_PER_PAGE;

    if (size > RECORDS_PER_PAGE) {
        size = RECORDS_PER_PAGE;
    }
    return size;
}

void func_ov069_021ba480(BSubwayRecordsWork *wk) {
    StrBuf *name = GFL_StrBufCreate(0x10, HEAPID_BSUBWAY_RECORDS);
    int size = func_ov069_021ba470(wk->page, wk->pageCount, wk->count);
    int i;

    for (i = 0; i < size; i++) {
        GFL_StrBufLoadFixedString(name, wk->records[wk->page * RECORDS_PER_PAGE + i].name, 8);
        func_ov069_021ba520(wk, i, name, wk->records[wk->page * RECORDS_PER_PAGE + i].female);
    }
    for (; i < RECORDS_PER_PAGE; i++) {
        func_0202cc70(wk->frames, i);
    }
    GFL_StrBufFree(name);
}


// pokebw2's BSubwayRecords_DrawFrame
void func_ov069_021ba520(BSubwayRecordsWork *wk, int index, StrBuf *name, BOOL female) {
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[index]), 4);
    if (female == FALSE) {
        GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(wk->windows[index]), 0, 0, name, wk->font,
                                       PRINT_COLOR(13, 12, 4));
    } else {
        GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(wk->windows[index]), 0, 0, name, wk->font,
                                       PRINT_COLOR(11, 10, 4));
    }
    BmpWin_FlushChar(wk->windows[index]);
    func_0202cebc(wk->frames, index, wk->windows[index]);
    func_0202cc58(wk->frames, index);
}
