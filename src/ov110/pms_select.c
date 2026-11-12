// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov185/pms_select.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/pms_select.h"
#include "types.h"
#include "app/ui/ui_scene.h"
#include "app/ui/touchbar.h"
#include "app/pms_input.h"
#include "app/pms_select_graphic.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/ioreg.h"
#include "nnsys/g2d.h"
#include "nnsys/gfd.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/brightness.h"
#include "system/gf_font.h"
#include "system/pms.h"
#include "system/pms_data.h"
#include "system/pms_draw.h"
#include "system/pms_word.h"
#include "system/pmsi_param.h"
#include "system/printsys.h"
#include "text/system/pms_input.h"

#define HEAPID_PMS_SELECT 0x51

// The phrase select: a list of the saved phrases on plates, from which the trainer card picks its greeting, or opens
// the phrase input to edit one. Named descriptively, after its graphics' "pms_select_graphic.c". The names are ours

// The plates shown at once, and the phrases saved
#define PLATE_COUNT 4
#define SENTENCE_MAX 20

// No phrase under the cursor
#define POS_NONE 0xff

// The scroll bar's range of y
#define SCROLL_BAR_TOP 16
#define SCROLL_BAR_BOTTOM 152
#define SCROLL_BAR_RANGE 137

enum {
    SEQ_START,
    SEQ_TOUCHBAR_ON,
    SEQ_FADE_IN,
    SEQ_FADE_IN_WAIT,
    SEQ_SELECT,
    SEQ_FADE_OUT,
    SEQ_FADE_OUT_WAIT,
    SEQ_TOUCHBAR_OFF,
    SEQ_MENU_OPEN,
    SEQ_MENU,
    SEQ_MENU_CLOSE,
    SEQ_INPUT_FADE_OUT,
    SEQ_INPUT_FADE_OUT_WAIT,
    SEQ_INPUT_RELEASE,
    SEQ_INPUT_CALL,
    SEQ_INPUT_RETURN,
    SEQ_END,
};

// The BGs: the plates the phrases are on, whose colors fade back and forth under the cursor, and the title window
// Black 2's; twinned code confirms the offsets of chars, screenFile, screen, fadeAngle, fadeColors, decided,
// blinkCount, titleWindow and titleFlushPending in Black
typedef struct {
    u32 chars;
    void *screenFile;
    NNSG2dScreenData *screen;
    u16 fadeAngle;
    u16 fadeColors[3];
    u16 colors[4];
    // Set once a phrase is chosen, when the plate blinks
    BOOL decided;
    u8 blinkCount;
    BmpWin *titleWindow;
    BOOL titleFlushPending;
} PMSSelectBG;

// Black 2's; twinned code confirms the offsets of param, heapId, scrollBar, objRes, save, wordSave, bg, graphic,
// touchBar, font, queue, msgData, menuRes, menu, windows, flushPending, pmsDraw, inputParam, prevPos, pos, top,
// sentenceCount, lineCount, scrollTouch, nextSeq, listActive, touchMode and procMgr in Black
typedef struct {
    PMSSelectParam *param;
    HeapID heapId;
    ClActor *scrollBar;
    UIObjRes objRes;
    SaveControl *save;
    PMSWordSave *wordSave;
    PMSSelectBG bg;
    PMSSelectGraphic *graphic;
    TouchBar *touchBar;
    Font *font;
    PrintQueue *queue;
    MsgData *msgData;
    AppTaskMenuRes *menuRes;
    AppTaskMenu *menu;
    BmpWin *windows[PLATE_COUNT];
    BOOL flushPending[PLATE_COUNT];
    PMSDraw *pmsDraw;
    PMSIParam *inputParam;
    u8 prevPos;
    u8 pos;
    // The phrase on the first plate
    u8 top;
    u8 sentenceCount;
    // The plates of the list, with the empty one after the phrases when there is room
    u8 lineCount;
    u8 scrollTouch : 1;
    u32 subSeq;
    u32 nextSeq;
    BOOL listActive;
    BOOL touchMode;
    GameProcManager *procMgr;
} PMSSelectWork;

// A color of a palette that fades from start to end and back
typedef struct {
    u16 palette;
    u16 color;
    GXRgb start;
    GXRgb end;
} PMSSelectColorFade;

static BOOL func_ov110_021c0134(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov110_021c025c(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov110_021c01e4(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov110_021c022c(PMSSelectWork *wk);
static void func_ov110_021c0250(u32 *state, PMSSelectWork *wk, u32 seq);
static void func_ov110_021c0490(PMSSelectWork *wk);
static void func_ov110_021c05b0(PMSSelectWork *wk);
static void func_ov110_021c060c(PMSSelectBG *bg, HeapID heapId, Font *font, PrintQueue *queue, MsgData *msgData);
static void func_ov110_021c07e8(PMSSelectBG *bg);
static void func_ov110_021c080c(PMSSelectBG *bg, u8 line, u32 palette, BOOL active);
static void func_ov110_021c088c(u16 *angle, GXRgb *colors, const PMSSelectColorFade *fades, u8 count);
static void func_ov110_021c098c(PMSSelectBG *bg);
static void func_ov110_021c09e0(PMSSelectBG *bg);
static void func_ov110_021c09e8(PMSSelectBG *bg, u32 palette);
TouchBar *func_ov110_021c0a54(PMSSelectWork *wk, ClActUnit *unit, HeapID heapId);
static void func_ov110_021c0ab4(PMSSelectWork *wk);
static void func_ov110_021c0ac0(TouchBar *touchBar);
static AppTaskMenu *func_ov110_021c0ac8(AppTaskMenuRes *res, MsgData *msgData, HeapID heapId);
static void func_ov110_021c0b44(AppTaskMenu *menu);
static void func_ov110_021c0b4c(AppTaskMenu *menu);
static void func_ov110_021c0b54(PMSSelectWork *wk);
static void func_ov110_021c0c18(PMSSelectWork *wk);
static void func_ov110_021c0c38(PMSSelectWork *wk);
static void func_ov110_021c0c44(PMSSelectWork *wk);
static void func_ov110_021c0c50(PMSSelectWork *wk);
static void func_ov110_021c0c64(PMSSelectWork *wk, BOOL waitPrint);
static void func_ov110_021c0d34(PMSSelectWork *wk, BOOL force);
static BOOL func_ov110_021c0de4(PMSSelectWork *wk);
static void func_ov110_021c0e3c(u32 y, u8 lineCount, u16 *top, s16 *barY);
static BOOL func_ov110_021c0eb0(PMSSelectWork *wk);
static BOOL func_ov110_021c0f14(PMSSelectWork *wk);
static void func_ov110_021c1154(PMSSelectWork *wk);
static BOOL func_ov110_021c1174(PrintQueue *queue, BmpWin *window, BOOL *flushPending);
static void func_ov110_021c11b8(PMSSelectWork *wk, u8 line, u8 index, BOOL active);
static void func_ov110_021c124c(PMSSelectWork *wk, u8 line, u8 index, BOOL active);
static int func_ov110_021c12f8(void);
BOOL func_ov110_021c132c(u32 *seq, PMSSelectWork *wk);
static BOOL func_ov110_021c133c(u32 *seq, PMSSelectWork *wk);
BOOL func_ov110_021c1440(u32 *seq, PMSSelectWork *wk);
static BOOL func_ov110_021c1450(u32 *seq, PMSSelectWork *wk);
static BOOL func_ov110_021c1484(u32 *seq, PMSSelectWork *wk);
static BOOL func_ov110_021c14e0(u32 *seq, PMSSelectWork *wk);
static BOOL func_ov110_021c154c(u32 *seq, PMSSelectWork *wk);
static BOOL func_ov110_021c15b4(u32 *seq, PMSSelectWork *wk);

// pokebw2's PMS_SELECT_PROC_FUNCTIONS
const GameProcFunctions data_ov110_021c39d4 = {
    func_ov110_021c0134,
    func_ov110_021c025c,
    func_ov110_021c01e4,
};

// The colors of the plate under the cursor
// pokebw2's sPMSSelectPlateFades
static const PMSSelectColorFade data_ov110_021c39e0[] = {
    { 7, 5, 0x7e80, 0x7fef },
    { 7, 9, 0x5164, 0x7f22 },
    { 7, 10, 0x69ea, 0x7fa5 },
};

// The menu of a phrase: choose it, edit it, or back
// pokebw2's sPMSSelectMenuMessages
static u8 data_ov110_021c3c94[] = { 13, 16, 15 };

// pokebw2's PMSSelect_Init
static BOOL func_ov110_021c0134(GameProc *proc, u32 *state, void *param, void *work) {
    PMSSelectWork *wk;
    PMSSelectParam *selectParam = param;

    GFL_OvlLoad(OVERLAY_APP_UI);
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_PMS_SELECT, 0x1c000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(PMSSelectWork), HEAPID_PMS_SELECT);
    MI_CpuFill8(wk, 0, sizeof(PMSSelectWork));
    wk->heapId = HEAPID_PMS_SELECT;
    wk->save = selectParam->save;
    wk->wordSave = getDexBlkAddress(selectParam->save);
    wk->param = selectParam;
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, wk->heapId);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_PMS_INPUT, wk->heapId);
    wk->queue = func_0201c440(wk->heapId);
    wk->procMgr = CreateGameProcManager(wk->heapId);
    wk->prevPos = POS_NONE;
    wk->pos = POS_NONE;
    func_ov110_021c0490(wk);
    return TRUE;
}

// pokebw2's PMSSelect_Exit
static BOOL func_ov110_021c01e4(GameProc *proc, u32 *state, void *param, void *work) {
    PMSSelectWork *wk = work;

    func_ov110_021c05b0(wk);
    FreeGameProcManager(wk->procMgr);
    func_0201c6ec(wk->queue);
    func_0201c4c0(wk->queue);
    GFL_MsgDataFree(wk->msgData);
    GFL_FontFree(wk->font);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_PMS_SELECT);
    GFL_OvlUnload(OVERLAY_APP_UI);
    return TRUE;
}

// pokebw2's PMSSelect_IsPrintEnd
static BOOL func_ov110_021c022c(PMSSelectWork *wk) {
    if (func_0201c6b4(wk->queue) && func_0202dc98(wk->pmsDraw)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's PMSSelect_SetSeq
static void func_ov110_021c0250(u32 *state, PMSSelectWork *wk, u32 seq) {
    *state = seq;
    wk->subSeq = 0;
}

// pokebw2's PMSSelect_Main
static BOOL func_ov110_021c025c(GameProc *proc, u32 *state, void *param, void *work) {
    PMSSelectWork *wk = work;
    BOOL running = GFL_ProcMgrUpdate(wk->procMgr);

    if (running == TRUE) {
        return FALSE;
    }

    switch (*state) {
    case SEQ_START:
        if (func_02024464(func_02024910(wk->wordSave, 0), wk->heapId) == FALSE) {
            wk->pos = 0;
            func_ov110_021c0250(state, wk, SEQ_INPUT_RELEASE);
        } else {
            func_ov110_021c0250(state, wk, SEQ_TOUCHBAR_ON);
        }
        break;
    case SEQ_TOUCHBAR_ON:
        if (func_ov110_021c132c(&wk->subSeq, wk)) {
            func_ov110_021c0250(state, wk, SEQ_FADE_IN);
        }
        break;
    case SEQ_FADE_IN:
        GFL_FadeSet(3, 16, 0, 0);
        func_ov110_021c0250(state, wk, SEQ_FADE_IN_WAIT);
        break;
    case SEQ_FADE_IN_WAIT:
        if (!GFL_FadeIsRunning()) {
            func_ov110_021c0250(state, wk, SEQ_SELECT);
        }
        break;
    case SEQ_SELECT:
        if (func_ov110_021c022c(wk) && func_ov110_021c133c(&wk->subSeq, wk)) {
            func_ov110_021c0250(state, wk, wk->nextSeq);
        }
        break;
    case SEQ_FADE_OUT:
        GFL_FadeSet(3, 0, 16, 0);
        func_ov110_021c0250(state, wk, SEQ_FADE_OUT_WAIT);
        break;
    case SEQ_FADE_OUT_WAIT:
        if (!GFL_FadeIsRunning()) {
            func_ov110_021c0250(state, wk, SEQ_TOUCHBAR_OFF);
        }
        break;
    case SEQ_TOUCHBAR_OFF:
        if (func_ov110_021c1440(&wk->subSeq, wk)) {
            func_ov110_021c0250(state, wk, SEQ_END);
        }
        break;
    case SEQ_MENU_OPEN:
        if (func_ov110_021c1450(&wk->subSeq, wk)) {
            func_ov110_021c0250(state, wk, SEQ_MENU);
        }
        break;
    case SEQ_MENU:
        if (func_ov110_021c1484(&wk->subSeq, wk)) {
            func_ov110_021c0250(state, wk, SEQ_MENU_CLOSE);
        }
        break;
    case SEQ_MENU_CLOSE:
        if (func_ov110_021c14e0(&wk->subSeq, wk)) {
            func_ov110_021c0250(state, wk, wk->nextSeq);
        }
        break;
    case SEQ_INPUT_FADE_OUT:
        if (func_ov110_021c022c(wk)) {
            GFL_FadeSet(3, 0, 16, 0);
            func_ov110_021c0250(state, wk, SEQ_INPUT_FADE_OUT_WAIT);
        }
        break;
    case SEQ_INPUT_FADE_OUT_WAIT:
        if (!GFL_FadeIsRunning()) {
            func_ov110_021c0250(state, wk, SEQ_INPUT_RELEASE);
        }
        break;
    case SEQ_INPUT_RELEASE:
        func_ov110_021c05b0(wk);
        GFL_OvlUnload(OVERLAY_APP_UI);
        func_ov110_021c0250(state, wk, SEQ_INPUT_CALL);
        return FALSE;
    case SEQ_INPUT_CALL:
        if (func_ov110_021c154c(&wk->subSeq, wk)) {
            func_ov110_021c0250(state, wk, SEQ_INPUT_RETURN);
        }
        return FALSE;
    case SEQ_INPUT_RETURN:
        if (running != TRUE && func_ov110_021c15b4(&wk->subSeq, wk)) {
            GFL_OvlLoad(OVERLAY_APP_UI);
            func_ov110_021c0490(wk);
            func_ov110_021c0250(state, wk, SEQ_TOUCHBAR_ON);
        }
        return FALSE;
    case SEQ_END:
        return TRUE;
    }

    func_0201c4e4(wk->queue);
    if (wk->bg.titleFlushPending && !func_0201c6c4(wk->queue, BmpWin_GetBitmap(wk->bg.titleWindow))) {
        BmpWin_Transfer(wk->bg.titleWindow);
        wk->bg.titleFlushPending = FALSE;
    }
    func_ov110_021c1154(wk);
    func_ov110_021c0c38(wk);
    func_ov110_021c1704(wk->graphic);
    return FALSE;
}

// pokebw2's PMSSelect_SetupScreen
static void func_ov110_021c0490(PMSSelectWork *wk) {
    ClActUnit *unit;
    UIObjResSetup setup;

    wk->scrollTouch = FALSE;
    wk->listActive = TRUE;
    wk->touchMode = func_020355c4() == TRUE;
    wk->prevPos = wk->pos;
    if (wk->pos == POS_NONE) {
        if (wk->touchMode) {
            wk->top = 0;
        } else {
            wk->pos = 0;
            wk->top = 0;
        }
    } else if (wk->touchMode) {
        wk->pos = POS_NONE;
    }

    wk->graphic = func_ov110_021c1610(0, wk->heapId);
    unit = func_ov110_021c1718(wk->graphic);
    setup.arcId = ARCID_PMSI;
    setup.paletteFile = 7;
    setup.charFile = 13;
    setup.cellFile = 35;
    setup.animFile = 41;
    setup.vramType = CLACT_VRAM_MAIN;
    setup.flags = 0;
    setup.paletteOffset = 0;
    setup.paletteStart = 0;
    setup.paletteCount = 5;
    func_ov067_021b6208(&wk->objRes, &setup, unit, wk->heapId);
    wk->scrollBar = func_ov067_021b629c(&wk->objRes, unit, 240, SCROLL_BAR_TOP, 16, wk->heapId);
    func_0204b84c(wk->scrollBar, 1);
    func_0204b92c(wk->scrollBar, TRUE);
    func_ov110_021c060c(&wk->bg, wk->heapId, wk->font, wk->queue, wk->msgData);
    func_ov110_021c09e0(&wk->bg);
    wk->bg.blinkCount = 0;
    wk->bg.decided = FALSE;
    wk->touchBar = func_ov110_021c0a54(wk, func_ov110_021c1718(wk->graphic), wk->heapId);
    wk->menuRes = func_02027f60(0, 12, wk->font, wk->queue, wk->heapId);
    func_ov110_021c0b54(wk);
    func_0203d34c(TRUE, wk->heapId);
}

// pokebw2's PMSSelect_ReleaseScreen
static void func_ov110_021c05b0(PMSSelectWork *wk) {
    u8 i;

    func_0201c6ec(wk->queue);
    for (i = 0; i < PLATE_COUNT; i++) {
        wk->flushPending[i] = FALSE;
    }
    wk->bg.titleFlushPending = FALSE;
    func_ov067_021b6284(&wk->objRes);
    func_ov110_021c0ab4(wk);
    func_02027fd8(wk->menuRes);
    func_ov110_021c0c18(wk);
    func_ov110_021c07e8(&wk->bg);
    func_ov110_021c16b0(wk->graphic);
    func_020355d8(wk->touchMode ? TRUE : FALSE);
}

// pokebw2's PMSSelect_BGLoad
static void func_ov110_021c060c(PMSSelectBG *bg, HeapID heapId, Font *font, PrintQueue *queue, MsgData *msgData) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_PMSI, heapId);
    StrBuf *strbuf;

    GFL_G2DIOLoadArcNCLRDefault(arc, 0, 0, 0, 9 * 0x20, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, 4, 0, 8 * 0x20, heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 9 * 0x20, 0x20, heapId);
    bg->colors[0] = 0x7fff;
    NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, 9 * 0x20 + 14 * sizeof(GXRgb), &bg->colors[0],
                                       sizeof(GXRgb));
    bg->colors[1] = 0x7fff;
    NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, 9 * 0x20 + 13 * sizeof(GXRgb), &bg->colors[1],
                                       sizeof(GXRgb));
    GFL_BGSysLoadArcNCGRStatic(arc, 10, 4, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 21, 4, 0, 0, FALSE, heapId);
    bg->chars = GFL_BGSysLoadArcNCGRDynamic(arc, 10, 3, 0, FALSE, heapId);
    GFL_G2DIOLoadNSCRSync(arc, 22, 3, 0, CHAR_POS(bg->chars), 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 11, 2, 0, 0, FALSE, heapId);
    bg->screenFile = GFL_G2DIOReadNSCRArc(arc, 23, FALSE, &bg->screen, heapId);
    GFL_G2DIOLoadArcNCLR(arc, 4, 4, 0x80, 8 * 0x20, 0x20, heapId);
    bg->colors[2] = 0x294b;
    NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_SUB, 8 * 0x20 + 14 * sizeof(GXRgb), &bg->colors[2],
                                       sizeof(GXRgb));
    bg->colors[3] = 0x5694;
    NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_SUB, 8 * 0x20 + 15 * sizeof(GXRgb), &bg->colors[3],
                                       sizeof(GXRgb));
    GFL_BGSysLoadArcNCGRStatic(arc, 18, 5, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 24, 5, 0, 0, FALSE, heapId);
    GFL_BGSysSetScrPaletteNo(5, 0, 0, 32, 32, 8);
    bg->titleWindow = BmpWin_CreateDynamic(5, 3, 5, 26, 7, 8, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(bg->titleWindow), 7);
    strbuf = GFL_MsgDataLoadStrbufNew(msgData, 21);
    func_0201c724(queue, BmpWin_GetBitmap(bg->titleWindow), 2, 4, strbuf, font, PRINT_COLOR(14, 15, 7));
    bg->titleFlushPending = TRUE;
    GFL_StrBufFree(strbuf);
    GFL_ArcToolFree(arc);
}

// pokebw2's PMSSelect_BGUnload
static void func_ov110_021c07e8(PMSSelectBG *bg) {
    BmpWin_Free(bg->titleWindow);
    GFL_BGSysFreeCharMemory(3, CHAR_POS(bg->chars), CHAR_SIZE(bg->chars));
    GFL_HeapFree(bg->screenFile);
}

// pokebw2's PMSSelect_BGDrawPlate
static void func_ov110_021c080c(PMSSelectBG *bg, u8 line, u32 palette, BOOL active) {
    int i;
    NNSG2dScreenData *screen = bg->screen;
    int width = screen->screenWidth / 8;
    int height = screen->screenHeight / 8;
    u16 *entries = (u16 *)screen->rawData;

    if (active) {
        palette = 5;
    }
    for (i = 0; i < width * height; i++) {
        entries[i] = (entries[i] & 0xfff) + (palette + 2) * 0x1000;
    }
    GFL_BGSysLoadScrArea(2, 0, line * 6 + 1, width, 6, bg->screen->rawData, 0, 0, width, height);
    GFL_BGSysQueueScrLoad(2);
}

// pokebw2's PMSSelect_FadeColors
static void func_ov110_021c088c(u16 *angle, GXRgb *colors, const PMSSelectColorFade *fades, u8 count) {
    u8 i;
    fx16 t;

    if (*angle + 0x400 >= 0x10000) {
        *angle = *angle - 0x10000 + 0x400;
    } else {
        *angle += 0x400;
    }
    t = (FX_CosIdx(*angle) + FX16_ONE) / 2;
    for (i = 0; i < count; i++) {
        u8 r1 = (fades[i].start & GX_RGB_R_MASK) >> GX_RGB_R_SHIFT;
        u8 g1 = (fades[i].start & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
        u8 b1 = (fades[i].start & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;
        u8 r2 = (fades[i].end & GX_RGB_R_MASK) >> GX_RGB_R_SHIFT;
        u8 g2 = (fades[i].end & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
        u8 b2 = (fades[i].end & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;
        u8 r;
        u8 g;
        u8 b;

        r = r1 + ((r2 - r1) * t >> FX32_SHIFT);
        g = g1 + ((g2 - g1) * t >> FX32_SHIFT);
        b = b1 + ((b2 - b1) * t >> FX32_SHIFT);
        colors[i] = GX_RGB(r, g, b);
        NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN,
                                           fades[i].palette * 0x20 + fades[i].color * sizeof(GXRgb), &colors[i],
                                           sizeof(GXRgb));
    }
}

// pokebw2's PMSSelect_BGUpdatePlateFade
static void func_ov110_021c098c(PMSSelectBG *bg) {
    if (bg->decided) {
        BOOL on = TRUE;

        if (bg->blinkCount < 16) {
            on = (bg->blinkCount / 4) % 2;
            bg->blinkCount++;
        }
        bg->fadeAngle = (on ? 0x3f : 0x1f) << 10;
    }
    func_ov110_021c088c(&bg->fadeAngle, bg->fadeColors, data_ov110_021c39e0, NELEMS(data_ov110_021c39e0));
}

// pokebw2's PMSSelect_BGResetPlateFade
static void func_ov110_021c09e0(PMSSelectBG *bg) {
    bg->fadeAngle = 0;
}

// pokebw2's PMSSelect_BGDrawPlateEdge
static void func_ov110_021c09e8(PMSSelectBG *bg, u32 palette) {
    int i;
    NNSG2dScreenData *screen = bg->screen;
    int width = screen->screenWidth / 8;
    int height = screen->screenHeight / 8;
    u16 *entries = (u16 *)screen->rawData;

    for (i = 0; i < width * height; i++) {
        entries[i] = (entries[i] & 0xfff) + (palette + 2) * 0x1000;
    }
    GFL_BGSysLoadScrArea(2, 0, 0, width, 1, bg->screen->rawData, 0, 5, width, height);
    GFL_BGSysQueueScrLoad(2);
}

TouchBar *func_ov110_021c0a54(PMSSelectWork *wk, ClActUnit *unit, HeapID heapId) {
    TouchBarSetup setup = { 0 };
    TouchBarItem items[1] = {
        { 1, { 232, 168 } },
    };
    TouchBar *touchBar;

    setup.items = items;
    setup.count = 1;
    setup.unit = unit;
    setup.bg = 1;
    setup.bgPalette = 14;
    setup.objPalette = 5;
    setup.vramType = 1;
    touchBar = func_ov067_021b62e0(&setup, heapId);
    func_ov067_021b6588(touchBar, 1, 1);
    return touchBar;
}


// pokebw2's PMSSelect_DeleteTouchBar
static void func_ov110_021c0ab4(PMSSelectWork *wk) {
    func_ov067_021b639c(wk->touchBar);
}

// pokebw2's PMSSelect_UpdateTouchBar
static void func_ov110_021c0ac0(TouchBar *touchBar) {
    func_ov067_021b63d0(touchBar);
}

// pokebw2's PMSSelect_CreateMenu
static AppTaskMenu *func_ov110_021c0ac8(AppTaskMenuRes *res, MsgData *msgData, HeapID heapId) {
    AppTaskMenuInit init;
    AppTaskMenuItem items[3];
    AppTaskMenu *menu;
    u32 i;

    for (i = 0; i < 3; i++) {
        items[i].str = GFL_MsgDataLoadStrbufNew(msgData, data_ov110_021c3c94[i]);
        items[i].color = PRINT_COLOR(14, 15, 3);
        items[i].type = 0;
    }
    items[2].type = APP_TASKMENU_ITEM_RETURN;
    init.heapId = heapId;
    init.itemCount = 3;
    init.items = items;
    init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
    init.x = 32;
    init.y = 24;
    init.width = 13;
    init.height = 3;
    menu = func_02027778(&init, res);
    for (i = 0; i < 3; i++) {
        GFL_StrBufFree(items[i].str);
    }
    return menu;
}

// pokebw2's PMSSelect_DeleteMenu
static void func_ov110_021c0b44(AppTaskMenu *menu) {
    func_02027858(menu);
}

// pokebw2's PMSSelect_UpdateMenu
static void func_ov110_021c0b4c(AppTaskMenu *menu) {
    func_02027974(menu);
}

// pokebw2's PMSSelect_SetupList
static void func_ov110_021c0b54(PMSSelectWork *wk) {
    int i;

    wk->pmsDraw = func_0202db0c(func_ov110_021c1718(wk->graphic), CLACT_VRAM_MAIN, wk->queue, wk->font, 8,
                                 PLATE_COUNT, wk->heapId);
    wk->sentenceCount = 0;
    for (i = 0; i < SENTENCE_MAX; i++) {
        if (func_02024464(func_02024910(wk->wordSave, i), wk->heapId) == FALSE) {
            break;
        }
        wk->sentenceCount++;
    }
    wk->lineCount = wk->sentenceCount;
    if (wk->sentenceCount < SENTENCE_MAX) {
        wk->lineCount++;
    }
    func_0204b3dc(wk->scrollBar, wk->lineCount > 3);
    for (i = 0; i < PLATE_COUNT; i++) {
        wk->windows[i] = BmpWin_CreateDynamic(2, 2, i * 6 + 2, 25, 4, 9, TRUE);
    }
    func_ov110_021c0c44(wk);
    func_ov110_021c0d34(wk, TRUE);
}

// pokebw2's PMSSelect_DeleteList
static void func_ov110_021c0c18(PMSSelectWork *wk) {
    int i;

    func_0202dc08(wk->pmsDraw);
    for (i = 0; i < PLATE_COUNT; i++) {
        BmpWin_Free(wk->windows[i]);
    }
}

// pokebw2's PMSSelect_UpdateList
static void func_ov110_021c0c38(PMSSelectWork *wk) {
    func_0202dbc4(wk->pmsDraw);
}

// pokebw2's PMSSelect_DrawList
static void func_ov110_021c0c44(PMSSelectWork *wk) {
    func_ov110_021c0c64(wk, FALSE);
}

// pokebw2's PMSSelect_RedrawList
static void func_ov110_021c0c50(PMSSelectWork *wk) {
    func_ov110_021c0c64(wk, TRUE);
    func_ov110_021c09e0(&wk->bg);
}

// pokebw2's PMSSelect_DrawListEx
static void func_ov110_021c0c64(PMSSelectWork *wk, BOOL waitPrint) {
    int i;
    u8 index;

    if (waitPrint && !func_0202dc98(wk->pmsDraw)) {
        return;
    }
    GFL_BGSysFillScrAsync(2, 0);
    if (wk->top != 0) {
        func_ov110_021c09e8(&wk->bg, func_0202452c(func_02024910(wk->wordSave, (u8)(wk->top - 1))));
    }
    for (i = 0; i < PLATE_COUNT; i++) {
        if (func_0202dcc4(wk->pmsDraw, i)) {
            func_0202dc9c(wk->pmsDraw, i, FALSE);
        }
    }
    for (i = 0; wk->top + i < wk->sentenceCount && i < PLATE_COUNT; i++) {
        index = wk->top + i;
        func_ov110_021c124c(wk, i, index, index == wk->pos);
    }
    if (wk->sentenceCount < SENTENCE_MAX && i < PLATE_COUNT) {
        index = wk->top + i;
        func_ov110_021c11b8(wk, i, 0, index == wk->pos);
    }
}

// pokebw2's PMSSelect_UpdateScrollBar
static void func_ov110_021c0d34(PMSSelectWork *wk, BOOL force) {
    int max;
    int top;
    s16 y;

    if (!func_0204b3f4(wk->scrollBar)) {
        return;
    }
    max = wk->lineCount - 2;
    top = wk->top;
    if (!force) {
        int barY = func_0204b560(wk->scrollBar, 1);
        int start = top * SCROLL_BAR_RANGE / max + SCROLL_BAR_TOP;
        int end = (top + 1) * SCROLL_BAR_RANGE / max + SCROLL_BAR_TOP;

        if (top == 0) {
            if (barY < end) {
                return;
            }
        } else if (top == max - 1) {
            if (barY >= start) {
                return;
            }
        } else if (start <= barY && barY < end) {
            return;
        }
    }
    if (top == 0) {
        y = SCROLL_BAR_TOP;
    } else if (top == max - 1) {
        y = SCROLL_BAR_BOTTOM;
    } else {
        y = wk->top * SCROLL_BAR_RANGE / max + SCROLL_BAR_TOP + SCROLL_BAR_RANGE / (max * 2);
        if (y > SCROLL_BAR_BOTTOM) {
            y = SCROLL_BAR_BOTTOM;
        }
    }
    func_0204b550(wk->scrollBar, y, 1);
}

// pokebw2's PMSSelect_CheckScrollBarTouch
static BOOL func_ov110_021c0de4(PMSSelectWork *wk) {
    u32 x, y;

    if (!func_0204b3f4(wk->scrollBar)) {
        return FALSE;
    }
    if (!func_02035d20(&x, &y)) {
        return FALSE;
    }
    if (x >= 240 && x <= 247 && y <= 159 && y >= 8) {
        wk->scrollTouch = TRUE;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's PMSSelect_GetScrollPos
static void func_ov110_021c0e3c(u32 y, u8 lineCount, u16 *top, s16 *barY) {
    int max = lineCount - 2;
    int i;

    for (i = 0; i < max; i++) {
        u32 start = i * SCROLL_BAR_RANGE / max + SCROLL_BAR_TOP;
        u32 end = (i * SCROLL_BAR_RANGE + SCROLL_BAR_RANGE) / max + SCROLL_BAR_TOP;

        if (i == 0) {
            if (y < end) {
                *top = i;
                break;
            }
        } else if (i == max - 1) {
            if (y >= start) {
                *top = i;
                break;
            }
        } else if (start <= y && y < end) {
            *top = i;
            break;
        }
    }
    if (y > SCROLL_BAR_BOTTOM) {
        y = SCROLL_BAR_BOTTOM;
    } else if (y < SCROLL_BAR_TOP) {
        y = SCROLL_BAR_TOP;
    }
    *barY = y;
}

// pokebw2's PMSSelect_ScrollByTouch
static BOOL func_ov110_021c0eb0(PMSSelectWork *wk) {
    u32 x, y;
    u16 top;
    s16 barY;

    if (!func_02035cb8(&x, &y)) {
        wk->scrollTouch = FALSE;
        return FALSE;
    }
    func_ov110_021c0e3c(y, wk->lineCount, &top, &barY);
    func_0204b550(wk->scrollBar, barY, 1);
    if (top != wk->top) {
        wk->top = top;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's PMSSelect_ListInput
static BOOL func_ov110_021c0f14(PMSSelectWork *wk) {
    BOOL decided = FALSE;
    BOOL cont = TRUE;

    if (!wk->scrollTouch) {
        BOOL key = FALSE;

        if (GCTX_HIDGetPressedKeys() & ~PAD_BUTTON_B) {
            key = TRUE;
        }
        if (key && wk->pos == POS_NONE) {
            if (wk->prevPos == POS_NONE) {
                wk->pos = wk->top;
            } else if (wk->top <= wk->prevPos && wk->prevPos < wk->top + 3) {
                wk->pos = wk->prevPos;
            } else {
                wk->pos = wk->top;
            }
            wk->prevPos = POS_NONE;
            cont = FALSE;
            wk->touchMode = FALSE;
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov110_021c0c50(wk);
        }
    }

    if (cont && wk->pos != POS_NONE && !wk->scrollTouch) {
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
            decided = TRUE;
            cont = FALSE;
            wk->touchMode = FALSE;
        } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
            if (wk->pos != 0) {
                wk->prevPos = wk->pos;
                wk->pos--;
                if (wk->pos + 1 == wk->top) {
                    wk->top--;
                }
                func_ov110_021c0d34(wk, FALSE);
                func_ov110_021c0c50(wk);
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
            cont = FALSE;
            wk->touchMode = FALSE;
        } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
            if (wk->pos < wk->lineCount - 1) {
                wk->prevPos = wk->pos;
                wk->pos++;
                if (wk->pos == wk->top + 3) {
                    wk->top++;
                }
                func_ov110_021c0d34(wk, FALSE);
                func_ov110_021c0c50(wk);
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
            cont = FALSE;
            wk->touchMode = FALSE;
        }
    }

    if (cont) {
        if (wk->scrollTouch) {
            if (func_ov110_021c0eb0(wk)) {
                func_ov110_021c0c50(wk);
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
            wk->touchMode = TRUE;
        } else {
            int plate = func_ov110_021c12f8();

            if (plate != -1 && plate > wk->lineCount - 1) {
                plate = -1;
            }
            if (plate != -1) {
                wk->prevPos = wk->pos;
                wk->pos = wk->top + plate;
                func_ov110_021c0c50(wk);
                decided = TRUE;
                wk->touchMode = TRUE;
            } else {
                if (!wk->scrollTouch) {
                    func_ov110_021c0de4(wk);
                }
                if (wk->scrollTouch == TRUE) {
                    if (func_ov110_021c0eb0(wk) || wk->pos != POS_NONE) {
                        wk->prevPos = wk->pos;
                        wk->pos = POS_NONE;
                        func_ov110_021c0c50(wk);
                    }
                    GFL_SndSEPlay(SEQ_SE_SELECT1);
                    wk->touchMode = TRUE;
                }
            }
        }
    }
    return decided;
}

// pokebw2's PMSSelect_FlushWindows
static void func_ov110_021c1154(PMSSelectWork *wk) {
    int i;

    for (i = 0; i < PLATE_COUNT; i++) {
        func_ov110_021c1174(wk->queue, wk->windows[i], &wk->flushPending[i]);
    }
}

// pokebw2's PMSSelect_FlushWindow
static BOOL func_ov110_021c1174(PrintQueue *queue, BmpWin *window, BOOL *flushPending) {
    GFLBitmap *bitmap = BmpWin_GetBitmap(window);

    if (*flushPending == TRUE && !func_0201c6c4(queue, bitmap)) {
        BmpWin_Transfer(window);
        *flushPending = FALSE;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's PMSSelect_DrawNewPlate
static void func_ov110_021c11b8(PMSSelectWork *wk, u8 line, u8 index, BOOL active) {
    StrBuf *strbuf = GFL_MsgDataLoadStrbufNew(wk->msgData, PmsInput_Text_017);
    u16 color;

    func_ov110_021c080c(&wk->bg, line, 6, active);
    if (active) {
        GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[line]), 14);
        color = PRINT_COLOR(1, 2, 14);
    } else {
        GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[line]), 13);
        color = PRINT_COLOR(1, 2, 13);
    }
    func_0201c724(wk->queue, BmpWin_GetBitmap(wk->windows[line]), 0, 0, strbuf, wk->font, color);
    GFL_StrBufFree(strbuf);
    BmpWin_FlushMap(wk->windows[line]);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(wk->windows[line]));
    wk->flushPending[line] = TRUE;
}

// pokebw2's PMSSelect_DrawSentencePlate
static void func_ov110_021c124c(PMSSelectWork *wk, u8 line, u8 index, BOOL active) {
    PMSData *sentence = func_02024910(wk->wordSave, index);

    func_ov110_021c080c(&wk->bg, line, func_0202452c(sentence), active);
    if (active) {
        GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[line]), 14);
        func_0202de64(wk->pmsDraw, PRINT_COLOR(1, 2, 14));
        func_0202de5c(wk->pmsDraw, 14);
    } else {
        GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[line]), 13);
        func_0202de64(wk->pmsDraw, PRINT_COLOR(1, 2, 13));
        func_0202de5c(wk->pmsDraw, 13);
    }
    if (func_02024464(sentence, wk->heapId) == TRUE) {
        func_0202dc48(wk->pmsDraw, wk->windows[line], sentence, line);
        wk->flushPending[line] = TRUE;
    }
}

// pokebw2's PMSSelect_GetTouchedPlate
static int func_ov110_021c12f8(void) {
    u32 x, y;

    if (func_02035d20(&x, &y) && x < 232 && y >= 8) {
        u32 plate = (y / 8 - 1) / 6;

        if (plate < 3) {
            return plate;
        }
    }
    return -1;
}

BOOL func_ov110_021c132c(u32 *seq, PMSSelectWork *wk)
{
    func_ov067_021b64d0(wk->touchBar, TRUE);
    return TRUE;
}


// pokebw2's PMSSelect_SeqSelect
static BOOL func_ov110_021c133c(u32 *seq, PMSSelectWork *wk) {
    BOOL end;
    int touch;

    touch = -1;
    end = FALSE;
    func_ov110_021c098c(&wk->bg);
    if (wk->bg.decided == FALSE) {
        BOOL decided = FALSE;

        if (!wk->scrollTouch) {
            func_020355d8(wk->touchMode ? TRUE : FALSE);
            func_ov110_021c0ac0(wk->touchBar);
            if (func_ov067_021b6470(wk->touchBar) != -1) {
                wk->listActive = FALSE;
            }
            touch = func_ov067_021b6448(wk->touchBar);
            if (touch != -1) {
                wk->listActive = TRUE;
            }
            wk->touchMode = func_020355c4() == TRUE;
        }
        if (touch != -1) {
            switch (touch) {
            case 1:
                wk->param->cancel = TRUE;
                wk->param->result = NULL;
                wk->nextSeq = SEQ_FADE_OUT;
                return TRUE;
            case 7:
                break;
            }
        }
        if (wk->listActive) {
            decided = func_ov110_021c0f14(wk);
        }
        if (decided) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            wk->bg.decided = TRUE;
        }
    } else if (wk->bg.blinkCount == 16) {
        end = TRUE;
    }
    if (end) {
        if (func_02024464(func_02024910(wk->wordSave, wk->pos), wk->heapId)) {
            wk->nextSeq = SEQ_MENU_OPEN;
        } else {
            wk->nextSeq = SEQ_INPUT_FADE_OUT;
        }
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov110_021c1440(u32 *seq, PMSSelectWork *wk) {
    func_ov067_021b64d0(wk->touchBar, FALSE);
    return TRUE;
}


// pokebw2's PMSSelect_SeqMenuOpen
static BOOL func_ov110_021c1450(u32 *seq, PMSSelectWork *wk) {
    func_020355d8(wk->touchMode ? TRUE : FALSE);
    wk->menu = func_ov110_021c0ac8(wk->menuRes, wk->msgData, wk->heapId);
    func_02023f60(-8, 30, 1);
    return TRUE;
}

// pokebw2's PMSSelect_SeqMenu
static BOOL func_ov110_021c1484(u32 *seq, PMSSelectWork *wk) {
    func_ov110_021c0b4c(wk->menu);
    if (func_020279e8(wk->menu)) {
        switch (func_02027a04(wk->menu)) {
        case 0:
            wk->nextSeq = SEQ_FADE_OUT;
            wk->param->result = func_02024910(wk->wordSave, wk->pos);
            wk->param->cancel = FALSE;
            break;
        case 1:
            wk->nextSeq = SEQ_INPUT_FADE_OUT;
            break;
        case 2:
            wk->nextSeq = SEQ_SELECT;
            break;
        }
        return TRUE;
    }
    return FALSE;
}

// pokebw2's PMSSelect_SeqMenuClose
static BOOL func_ov110_021c14e0(u32 *seq, PMSSelectWork *wk) {
    func_ov110_021c0b44(wk->menu);
    wk->touchMode = func_020355c4() == TRUE;
    if (wk->nextSeq == SEQ_SELECT) {
        func_02023f60(0, 30, 1);
        func_ov110_021c09e0(&wk->bg);
        wk->bg.blinkCount = 0;
        wk->bg.decided = FALSE;
        if (wk->touchMode) {
            wk->prevPos = wk->pos;
            wk->pos = POS_NONE;
            func_ov110_021c0c50(wk);
        }
    }
    return TRUE;
}

// pokebw2's PMSSelect_SeqCallInput
static BOOL func_ov110_021c154c(u32 *seq, PMSSelectWork *wk) {
    PMSIParam *param;

    switch (*seq) {
    case 0:
        (*seq)++;
        break;
    case 1:
        param = func_02024044(2, 0, NULL, TRUE, wk->save, wk->heapId);
        func_02024108(param, func_02024910(wk->wordSave, wk->pos));
        QueueGameProc(wk->procMgr, OVERLAY_NONE, &PMS_INPUT_PROC_FUNCTIONS, param);
        wk->inputParam = param;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's PMSSelect_SeqInputReturn
static BOOL func_ov110_021c15b4(u32 *seq, PMSSelectWork *wk) {
    PMSData sentence;

    switch (*seq) {
    case 0:
        if (func_02024120(wk->inputParam)) {
            func_02024134(wk->inputParam, &sentence);
            func_02024918(wk->wordSave, wk->pos, &sentence);
        }
        func_020240fc(wk->inputParam);
        (*seq)++;
        break;
    case 1:
        return TRUE;
    }
    return FALSE;
}
