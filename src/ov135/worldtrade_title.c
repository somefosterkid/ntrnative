// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov214/worldtrade_title.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/sound.h"
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
#include "gfl/sound.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "system/bmp_winframe.h"
#include "system/wipe.h"
#include "worldtrade_local.h"

// The Global Trade Station's title menu, where the player chooses to deposit or check a Pokémon, to search, or to
// leave, and the lower screen's trade room that the other screens share. The file's name is a guess, as the ROM
// doesn't give it, and so are the names of its functions

enum {
    TITLE_SEQ_OPENING,
    TITLE_SEQ_OPENING_MESSAGE,
    TITLE_SEQ_OPENING_FADE,
    TITLE_SEQ_OPENING_FADE_WAIT,
    TITLE_SEQ_OPENING_WAIT,
    TITLE_SEQ_START,
    TITLE_SEQ_MAIN,
    TITLE_SEQ_END_DEMO,
    TITLE_SEQ_END_DEMO_WAIT,
    TITLE_SEQ_END,
    TITLE_SEQ_MES_WAIT,
    TITLE_SEQ_MES_1MIN_WAIT,
    TITLE_SEQ_YESNO,
    TITLE_SEQ_YESNO_SELECT,
    TITLE_SEQ_CURSOR_ANIM_WAIT,
};

// The menu's choices
enum {
    TITLE_MENU_DEPOSIT,
    TITLE_MENU_SEARCH,
    TITLE_MENU_EXIT,
};

// Black 2's, not checked against Black
typedef struct {
    u16 x;
    u16 y;
} TitleCursorPos;

void func_ov135_021fff10(void);
void func_ov135_02200118(void);
void func_ov135_02200130(WorldTradeWork *wk);
void func_ov135_0220015c(WorldTradeWork *wk);
void func_ov135_022001f4(WorldTradeWork *wk);
void func_ov135_02200290(WorldTradeWork *wk);
void func_ov135_022002a0(WorldTradeWork *wk);
void func_ov135_022003bc(WorldTradeWork *wk);
void func_ov135_0220040c(WorldTradeWork *wk);
void func_ov135_02200434(WorldTradeWork *wk);
static int func_ov135_02200450(WorldTradeWork *wk);
static int func_ov135_02200490(WorldTradeWork *wk);
static int func_ov135_022004a0(WorldTradeWork *wk);
static int func_ov135_022004c8(WorldTradeWork *wk);
static int func_ov135_022004dc(WorldTradeWork *wk);
int func_ov135_022004f0(WorldTradeWork *wk);
int func_ov135_0220052c(WorldTradeWork *wk);
void func_ov135_0220053c(WorldTradeWork *wk, int decide);
int func_ov135_022005ec(WorldTradeWork *wk);
static int func_ov135_02200770(WorldTradeWork *wk);
static int func_ov135_0220078c(WorldTradeWork *wk);
static int func_ov135_022007a0(WorldTradeWork *wk);
static int func_ov135_022007e0(WorldTradeWork *wk);
static int func_ov135_02200800(WorldTradeWork *wk);
int func_ov135_022008b0(WorldTradeWork *wk);
int func_ov135_022008f0(WorldTradeWork *wk);
int func_ov135_0220090c(WorldTradeWork *wk);
void func_ov135_0220093c(WorldTradeWork *wk);
void func_ov135_022009a4(WorldTradeWork *wk, u32 param_2, u32 param_3, u32 param_4, u32 param_5);
void func_ov135_02200a10(WorldTradeWork *wk, u32 param_2, u32 param_3, u32 param_4, u32 param_5);
void func_ov135_02200a7c(BmpWin *win, MsgData *msgManager, int font, int msgNo, u16 dat, WorldTradePrint *print, u16 color);

// pokebw2's sTitleSubSeqTable
static int (*data_ov135_02203e58[])(WorldTradeWork *wk) = {
    func_ov135_02200450,        func_ov135_02200490,
    func_ov135_022004a0,    func_ov135_022004c8,
    func_ov135_022004dc,    func_ov135_022004f0,
    func_ov135_022005ec,           func_ov135_02200770,
    func_ov135_0220078c,    func_ov135_022007a0,
    func_ov135_022008f0,    func_ov135_0220090c,
    func_ov135_022007e0,          func_ov135_02200800,
    func_ov135_022008b0,
};

void func_ov135_02202940(WorldTradePrint *print);

// pokebw2's WorldTrade_Title_Main

void func_ov135_021fffe8(WorldTradeWork *wk, int bg1YOffset, BOOL bg2NoClear);

int func_ov135_021ffda8(WorldTradeWork *wk, int seq) {
    func_ov135_0220040c(wk);
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
    func_ov135_021fff10();
    func_ov135_0220015c(wk);
    func_ov135_022002a0(wk);
    func_ov135_021fffe8(wk, 0, FALSE);
    func_ov135_02200ab4(wk);
    if (wk->openingFlag == 0) {
        GFL_BGSysSetBGEnabled(4, FALSE);
    } else {
        func_ov135_02200b34(wk);
        GFL_BGSysSetBGEnabled(4, TRUE);
    }
    func_ov135_02200bac(wk, 0);
    wk->unk12E4 = 0;
    func_ov135_022001f4(wk);
    func_ov135_021f9254(wk);
    func_ov135_0220093c(wk);
    if (wk->openingFlag == 0) {
        func_ov135_02200130(wk);
        GFL_WipeSet(0, 1, 1, 0, 6, 1, HEAPID_WORLDTRADE);
        wk->subprocessSeq = TITLE_SEQ_OPENING;
        wk->openingFlag = 1;
        func_ov135_021ff8b0(wk);
    } else {
        if (wk->subOutFlag == 1 && GXx_GetMasterBrightness_(REG_DB_MASTER_BRIGHT_ADDR) != 0) {
            func_ov135_021ffd44(wk);
            GFL_WipeSet(0, 1, 1, 0, 6, 1, HEAPID_WORLDTRADE);
        } else {
            GFL_WipeSet(3, 1, 1, 0, 6, 1, HEAPID_WORLDTRADE);
        }
        wk->subprocessSeq = TITLE_SEQ_START;
    }
    wk->subOutFlag = 0;
    func_ov135_021ffbc8(wk);
    return WT_SEQ_FADEIN;
}

int func_ov135_021ffeb4(WorldTradeWork *wk, int seq) {
    return data_ov135_02203e58[wk->subprocessSeq](wk);
}

int func_ov135_021ffec8(WorldTradeWork *wk, int seq) {
    func_ov135_02200290(wk);
    func_ov135_02200434(wk);
    func_ov135_022003bc(wk);
    func_ov135_02200118();
    if (wk->subNextProcess == 4 || wk->subNextProcess == 5) {
        wk->unk12E4 = 1;
    }
    func_ov135_022000e4(wk);
    func_0204b3dc(wk->promptDsAct, FALSE);
    func_ov135_021f4ec8(wk);
    return 0;
}


// pokebw2's Title_BgInit
// The sub LCD's four backgrounds, as the BG setups the game takes them from the ROM
void func_ov135_021fff10(void) {
    {
        BGSysLCDConfig config = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };
        u32 enabled = GFL_BGSysGetEnabledBGsB();

        GFL_BGSysSetLCDConfig(&config);
        GFL_BGSysSetEnabledBGsB(enabled);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf800,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(0, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(0);
        GFL_BGSysSetBGEnabled(0, TRUE);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf000,
            GX_BG_CHARBASE_0x08000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(1, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(1, TRUE);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe800,
            GX_BG_CHARBASE_0x08000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(2, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(2, TRUE);
    }
    GFL_BGSysClearCharCore(0, 32, 0, HEAPID_WORLDTRADE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

void func_ov135_021fffe8(WorldTradeWork *wk, int bg1YOffset, BOOL bg2NoClear) {
    if (wk->unk12E0 == 0) {
        wk->unk12E0 = 1;
        {
            BGSetup setup = {
                0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x10000, 0x8000,
                GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE,
            };
            GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
            GFL_BGSysFillScrAsync(4, 0);
        }
        {
            BGSetup setup = {
                0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x14000, 0x8000,
                GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE,
            };
            GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
            if (!bg2NoClear) {
                GFL_BGSysFillScrAsync(6, 0);
            }
        }
        {
            BGSetup setup = {
                0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe000, GX_BG_CHARBASE_0x00000, 0x8000,
                GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE,
            };
            setup.y = bg1YOffset;
            GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
        }
        GFL_BGSysClearCharCore(4, 32, 0, HEAPID_WORLDTRADE);
        {
            BGSetup setup = {
                0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xd800, GX_BG_CHARBASE_0x10000, 0x8000,
                GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE,
            };
            GFL_BGSysCreateBG(7, &setup, BGMODE_TEXT);
            GFL_BGSysFillScrAsync(7, 0);
        }
    }
    GFL_BGSysSetBGEnabled(4, TRUE);
    GFL_BGSysSetBGEnabled(5, TRUE);
    GFL_BGSysSetBGEnabled(6, TRUE);
    GFL_BGSysSetBGEnabled(7, TRUE);
}


void func_ov135_022000e4(WorldTradeWork *wk) {
    if ((wk->unk12E0 == 1) && (wk->unk12E4 == 0)) {
        wk->unk12E0 = 0;
        GFL_BGSysReleaseBG(6);
        GFL_BGSysReleaseBG(5);
        GFL_BGSysReleaseBG(4);
        GFL_BGSysReleaseBG(7);
    }
}

// pokebw2's Title_BgExit
void func_ov135_02200118(void) {
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(0);
}

void func_ov135_02200130(WorldTradeWork *wk) {
    GFL_BGSysSetBGEnabledA(1, 0);
    GFL_BGSysSetBGEnabledA(2, 0);
    GFL_BGSysSetBGEnabledA(4, 0);
    func_0204b3dc(wk->cursorAct, 0);
}

// pokebw2's Title_BgGraphicSet
// Where the cursor sits on each choice
// pokebw2's sTitleCursorPos
static const TitleCursorPos data_ov135_022038e0[] = {
    { 128, 56 },
    { 128, 96 },
    { 128, 136 },
};

void func_ov135_0220015c(WorldTradeWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_WORLDTRADE, HEAPID_WORLDTRADE);

    GFL_G2DIOLoadArcNCLRDefault(arc, 3, 0, 0, 0x60, HEAPID_WORLDTRADE);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x1a0, 0x20, HEAPID_WORLDTRADE);
    LoadSysMsgBox(0, 1, 14, 0, HEAPID_WORLDTRADE);
    LoadSysMsgBox(0, 31, 11, 0, HEAPID_WORLDTRADE);
    GFL_BGSysLoadArcNCGRStatic(arc, 13, 1, 0, 0, TRUE, HEAPID_WORLDTRADE);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 32, 1, 0, 0x600, TRUE, HEAPID_WORLDTRADE);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 31, 2, 0, 0x600, TRUE, HEAPID_WORLDTRADE);
    GFL_ArcToolFree(arc);
}

void func_ov135_022001f4(WorldTradeWork *wk) {
    ClActorSetup setup;

    setup.x = data_ov135_022038e0[wk->titleCursorPos].x;
    setup.y = data_ov135_022038e0[wk->titleCursorPos].y;
    setup.sequence = 0;
    setup.priority = 0;
    setup.bgPriority = 1;
    wk->cursorAct = func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_MAIN2][WT_CLACT_RES_CHAR],
            wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_PLTT], wk->clactRes[WT_CLACT_RES_MAIN2][WT_CLACT_RES_CELL],
            &setup, 0, HEAPID_WORLDTRADE);
    func_0204b92c(wk->cursorAct, TRUE);
    func_0204b878(wk->cursorAct, 0);
    if (func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, FALSE);
    } else {
        func_0204b878(wk->cursorAct, 0);
        func_0204b3dc(wk->cursorAct, TRUE);
    }
    func_0203d34c(FALSE, HEAPID_WORLDTRADE);
}


void func_ov135_02200290(WorldTradeWork *wk) {
    func_0204b3b4(wk->cursorAct);
}

void func_ov135_022002a0(WorldTradeWork *wk) {
    int i;

    wk->titleWin = BmpWin_CreateDynamic(1, 2, 1, 28, 2, 1, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->titleWin), 10);
    BmpWin_TransferNow(wk->titleWin);
    func_ov135_022028c8(wk->titleWin, 0, wk->titleString, 0, 2, 0, 0x35ca, &wk->print);

    for (i = 0; i < 3; i++) {
        wk->menuWin[i] = BmpWin_CreateDynamic(1, 9, i * 5 + 6, 15, 2, 1, TRUE);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[i]), 1);
        BmpWin_TransferNow(wk->menuWin[i]);
    }

    wk->msgWin = BmpWin_CreateDynamic(0, 2, 21, 27, 2, 13, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->msgWin), 15);
    wk->talkWin = BmpWin_CreateDynamic(0, 2, 19, 27, 4, 13, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->talkWin), 15);
}


void func_ov135_022003bc(WorldTradeWork *wk) {
    s32 i;

    func_ov135_02202940(&wk->print);
    BmpWin_Free(wk->explainWin);
    BmpWin_Free(wk->talkWin);
    BmpWin_Free(wk->msgWin);
    i = 0;
    do {
        BmpWin_Free(wk->menuWin[i]);
        i = i + 1;
    } while (i < 3);
    BmpWin_Free(wk->titleWin);
}

void func_ov135_0220040c(WorldTradeWork *wk) {
    StrBuf *pSVar1;

    pSVar1 = GFL_StrBufCreate(0xb4, 0x48);
    wk->talkString = pSVar1;
    pSVar1 = GFL_MsgDataLoadStrbufNew(wk->msgManager, 0x2d);
    wk->titleString = pSVar1;
}

void func_ov135_02200434(WorldTradeWork *wk) {
    GFL_StrBufFree(wk->talkString);
    GFL_StrBufFree(wk->titleString);
}

// Waits for the player's walk, then checks the server for a trade of the deposited Pokémon
// pokebw2's Title_SubSeqOpening
static int func_ov135_02200450(WorldTradeWork *wk) {
    if (wk->demoEnd) {
        GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, FALSE);
        GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG1, FALSE);
        GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG2, FALSE);
        func_ov135_021f4ec0(wk, WORLDTRADE_UPLOAD, 11);
        wk->subReturnProcess = WORLDTRADE_TITLE;
        wk->subprocessSeq = TITLE_SEQ_END;
    }
    return WT_SEQ_MAIN;
}

// pokebw2's Title_SubSeqOpeningMessage
static int func_ov135_02200490(WorldTradeWork *wk) {
    func_ov135_021f4e98(wk, TITLE_SEQ_MES_WAIT, TITLE_SEQ_OPENING_FADE);
    return WT_SEQ_MAIN;
}

// pokebw2's Title_SubSeqOpeningFade
static int func_ov135_022004a0(WorldTradeWork *wk) {
    GFL_WipeSet(3, 1, 1, 0, 6, 1, HEAPID_WORLDTRADE);
    wk->subprocessSeq = TITLE_SEQ_OPENING_FADE_WAIT;
    return WT_SEQ_MAIN;
}

// pokebw2's Title_SubSeqOpeningFadeWait
static int func_ov135_022004c8(WorldTradeWork *wk) {
    if (GFL_WipeIsFinished()) {
        wk->subprocessSeq = TITLE_SEQ_START;
    }
    return WT_SEQ_MAIN;
}

// pokebw2's Title_SubSeqOpeningWait
static int func_ov135_022004dc(WorldTradeWork *wk) {
    if (GFL_WipeIsFinished()) {
        wk->subprocessSeq = TITLE_SEQ_START;
    }
    return WT_SEQ_MAIN;
}

int func_ov135_022004f0(WorldTradeWork *wk) {
    func_ov135_022009a4(wk, 4, 1, 0, 0xf0f);
    func_ov135_021f4e98(wk, TITLE_SEQ_MES_WAIT, TITLE_SEQ_MAIN);
    func_0204b92c(wk->cursorAct, TRUE);
    func_ov135_021f4f90(wk);
    return WT_SEQ_MAIN;
}


// pokebw2's sTitleTouchRects
static const TouchRect data_ov135_022038ec[] = {
    { 40, 71, 24, 231 },
    { 80, 111, 24, 231 },
    { 120, 151, 24, 231 },
    { TOUCH_RECT_END },
};

// pokebw2's Title_TouchPanelFunc
int func_ov135_0220052c(WorldTradeWork *wk) {
    return func_02035c08(data_ov135_022038ec);
}

void func_ov135_0220053c(WorldTradeWork *wk, int decide) {
    switch (decide) {
    case TITLE_MENU_DEPOSIT:
        if (wk->depositFlag == 0) {
            func_ov135_021f4ec0(wk, WORLDTRADE_MYBOX, 5);
            wk->subprocessSeq = TITLE_SEQ_END;
        } else if (wk->serverWaitTime == 0) {
            func_ov135_021f4ec0(wk, WORLDTRADE_UPLOAD, 11);
            wk->subReturnProcess = WORLDTRADE_MYPOKE;
            wk->subprocessSeq = TITLE_SEQ_END;
            wk->serverWaitTime = 60 * 60;
        } else {
            func_ov135_022009a4(wk, 40, 1, 0, 0xf0f);
            func_ov135_021f4e98(wk, TITLE_SEQ_MES_1MIN_WAIT, TITLE_SEQ_START);
            GFL_SndSEPlay(SEQ_SE_BEEP);
            wk->wait = 0;
        }
        break;
    case TITLE_MENU_SEARCH:
        wk->subLcdTouchOK = 0;
        func_ov135_021f4ec0(wk, WORLDTRADE_SEARCH, 13);
        wk->subprocessSeq = TITLE_SEQ_END;
        break;
    case TITLE_MENU_EXIT:
        func_ov135_02200a10(wk, 7, func_ov135_021f4ed4(wk), 0, 0xf0f);
        func_ov135_021f4e98(wk, TITLE_SEQ_MES_WAIT, TITLE_SEQ_YESNO);
        break;
    }
}

int func_ov135_022005ec(WorldTradeWork *wk) {
    int touch = func_ov135_0220052c(wk);

    if (touch != TOUCH_RECT_NONE) {
        wk->titleCursorPos = touch;
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov135_021f4ff4(wk->cursorAct, data_ov135_022038e0[wk->titleCursorPos].x,
                data_ov135_022038e0[wk->titleCursorPos].y);
        func_0204b3dc(wk->cursorAct, TRUE);
        func_0204b878(wk->cursorAct, 1);
        wk->subprocessSeq = TITLE_SEQ_CURSOR_ANIM_WAIT;
        func_020355d8(TRUE);
        if (wk->titleCursorPos == TITLE_MENU_EXIT) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
        } else {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        }
        return WT_SEQ_MAIN;
    }

    if (GCTX_HIDGetPressedKeys() != 0 && func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, TRUE);
        func_020355d8(FALSE);
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return WT_SEQ_MAIN;
    }

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        wk->titleCursorPos = TITLE_MENU_EXIT;
        func_ov135_021f4ff4(wk->cursorAct, data_ov135_022038e0[wk->titleCursorPos].x,
                data_ov135_022038e0[wk->titleCursorPos].y);
        func_0204b3dc(wk->cursorAct, TRUE);
        func_0204b878(wk->cursorAct, 1);
        wk->subprocessSeq = TITLE_SEQ_CURSOR_ANIM_WAIT;
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
    } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        func_0204b3dc(wk->cursorAct, TRUE);
        func_0204b878(wk->cursorAct, 1);
        wk->subprocessSeq = TITLE_SEQ_CURSOR_ANIM_WAIT;
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
    } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_UP) {
        if (wk->titleCursorPos != 0) {
            wk->titleCursorPos--;
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_0204b3dc(wk->cursorAct, TRUE);
            func_0204b9a0(wk->cursorAct);
            func_ov135_021f4ff4(wk->cursorAct, data_ov135_022038e0[wk->titleCursorPos].x,
                    data_ov135_022038e0[wk->titleCursorPos].y);
        }
    } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_DOWN) {
        if (wk->titleCursorPos < 2) {
            wk->titleCursorPos++;
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_0204b3dc(wk->cursorAct, TRUE);
            func_0204b9a0(wk->cursorAct);
            func_ov135_021f4ff4(wk->cursorAct, data_ov135_022038e0[wk->titleCursorPos].x,
                    data_ov135_022038e0[wk->titleCursorPos].y);
        }
    }
    return WT_SEQ_MAIN;
}



// pokebw2's Title_SubSeqEndDemo
static int func_ov135_02200770(WorldTradeWork *wk) {
    func_ov135_021ff9e0(wk);
    wk->subprocessSeq = TITLE_SEQ_END_DEMO_WAIT;
    wk->demoEnd = 0;
    return WT_SEQ_MAIN;
}

// pokebw2's Title_SubSeqEndDemoWait
static int func_ov135_0220078c(WorldTradeWork *wk) {
    if (wk->demoEnd) {
        wk->subprocessSeq = TITLE_SEQ_END;
    }
    return WT_SEQ_MAIN;
}

// pokebw2's Title_SubSeqEnd
static int func_ov135_022007a0(WorldTradeWork *wk) {
    if (wk->subNextProcess == WORLDTRADE_ENTER) {
        GFL_WipeSet(0, 0, 0, 0, 6, 1, HEAPID_WORLDTRADE);
    } else {
        GFL_WipeSet(3, 0, 0, 0, 6, 1, HEAPID_WORLDTRADE);
    }
    wk->subprocessSeq = TITLE_SEQ_OPENING;
    return WT_SEQ_FADEOUT;
}

// pokebw2's Title_SubSeqYesNo
static int func_ov135_022007e0(WorldTradeWork *wk) {
    func_ov135_021f4be0(wk, 18, 0x102, 3, TRUE);
    wk->subprocessSeq = TITLE_SEQ_YESNO_SELECT;
    return WT_SEQ_MAIN;
}

static int func_ov135_02200800(WorldTradeWork *wk) {
    u8 bg;
    BmpWin *pBVar2;

    bg = func_ov135_021f4d14(wk);
    if (bg == 1) {
        func_ov135_021f4ce4(wk);
        func_0201f7a8(wk->talkWin, 2);
        pBVar2 = wk->talkWin;
        func_020456f8(pBVar2);
        bg = BmpWin_GetBGIndex(pBVar2);
        GFL_BGSysQueueScrLoad(bg);
        pBVar2 = wk->explainWin;
        func_020456f8(pBVar2);
        bg = BmpWin_GetBGIndex(pBVar2);
        GFL_BGSysQueueScrLoad(bg);
        GFL_BGSysSetBGEnabled(6, 0);
        func_0204b3dc(wk->promptDsAct, 0);
        func_ov135_021f4ec0(wk, 0, 0x14);
        wk->subprocessSeq = 7;
    }
    else if (bg == 2) {
        func_ov135_021f4ce4(wk);
        func_0201f7a8(wk->talkWin, 2);
        pBVar2 = wk->talkWin;
        func_020456f8(pBVar2);
        bg = BmpWin_GetBGIndex(pBVar2);
        GFL_BGSysQueueScrLoad(bg);
        func_0204b92c(wk->cursorAct, 1);
        wk->subprocessSeq = 5;
        func_0204b3dc(wk->cursorAct, 1);
    }
    return 2;
}

int func_ov135_022008b0(WorldTradeWork *wk) {
    if (func_0204b98c(wk->cursorAct) == 0) {
        func_0204b878(wk->cursorAct, 0);
        if (func_020355c4() == TRUE) {
            func_0204b3dc(wk->cursorAct, FALSE);
        } else {
            func_0204b3dc(wk->cursorAct, TRUE);
        }
        func_ov135_0220053c(wk, wk->titleCursorPos);
    }
    return WT_SEQ_MAIN;
}


int func_ov135_022008f0(WorldTradeWork *wk) {
    BOOL BVar1;

    BVar1 = func_ov135_02202840(&wk->print);
    if (BVar1 == 0) {
        wk->subprocessSeq = wk->subprocessNextSeq;
    }
    return 2;
}

// The choices' texts, without and with a Pokémon deposited
// pokebw2's sTitleMenuMsgTable
static const u32 data_ov135_022038fc[][3] = {
    { 0x2f, 0x30, 0x31 },
    { 0x2e, 0x30, 0x31 },
};

int func_ov135_0220090c(WorldTradeWork *wk) {
    BOOL BVar1;
    s32 val;

    BVar1 = func_ov135_02202840(&wk->print);
    if ((BVar1 == 0) &&(val = wk->wait + 1, wk->wait = val, 0x2d < val)) {
        wk->wait = 0;
        wk->subprocessSeq = wk->subprocessNextSeq;
    }
    return 2;
}

void func_ov135_0220093c(WorldTradeWork *wk) {
    const u32 *msgs = data_ov135_022038fc[wk->depositFlag];
    int i;

    for (i = 0; i < 3; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[i]), 1);
        func_ov135_02200a7c(wk->menuWin[i], wk->msgManager, 0, msgs[i], 0, &wk->print, 0x3dc1);
    }
}


void func_ov135_022009a4(WorldTradeWork *wk, u32 param_2, u32 param_3, u32 param_4, u32 param_5) {
    u8 bg;
    GFLBitmap *bitmap;
    BmpWin *window;

    GFL_MsgDataLoadStrbuf(wk->msgManager, param_2, wk->talkString);
    bitmap = BmpWin_GetBitmap(wk->msgWin);
    GFL_BitmapFill(bitmap, 0xf);
    window = wk->msgWin;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    bg = BmpWin_GetBGIndex(window);
    GFL_BGSysLoadScr(bg);
    BmpWin_DrawFrame(wk->msgWin, 0, 1, 0xe);
    func_ov135_02202858(wk->msgWin, 0, wk->talkString, 0, 0, &wk->print);
}

void func_ov135_02200a10(WorldTradeWork *wk, u32 param_2, u32 param_3, u32 param_4, u32 param_5) {
    u8 bg;
    GFLBitmap *bitmap;
    BmpWin *window;

    GFL_MsgDataLoadStrbuf(wk->msgManager, param_2, wk->talkString);
    bitmap = BmpWin_GetBitmap(wk->talkWin);
    GFL_BitmapFill(bitmap, 0xf);
    BmpWin_DrawFrame(wk->talkWin, 0, 1, 0xe);
    func_ov135_02202858(wk->talkWin, 0, wk->talkString, 0, 0, &wk->print);
    window = wk->talkWin;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    bg = BmpWin_GetBGIndex(window);
    GFL_BGSysLoadScr(bg);
}

void func_ov135_02200a7c(BmpWin *win, MsgData *msgManager, int font, int msgNo, u16 dat, WorldTradePrint *print,
                                u16 color) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(msgManager, msgNo);

    func_ov135_022028c8(win, font, str, 0, 0, 0, color, print);
    GFL_StrBufFree(str);
}


void func_ov135_02200ab4(WorldTradeWork *wk) {
    ArcTool *arc;

    if ((wk->unk12E0 == 1) && (wk->unk12E4 == 0)) {
        arc = GFL_ArcSysCreateFileHandle(0x60, 0x48);
        GFL_G2DIOLoadArcNCLR(arc, 4, 4, 0x160, 0x160, 0xa0, 0x48);
        GFL_BGSysLoadArcNCGRStatic(arc, 0xe, 5, 0, 0x1c00, 0, 0x48);
        GFL_G2DIOLoadNSCRAsync(arc, 0x21, 5, 0, 0, 0, 0, 0x48);
        GFL_ArcToolFree(arc);
        GFL_BGSysLoadNCLRDefault(0x17, 5, 4, 0x20, 0x20, 0x48);
    }
}

void func_ov135_02200b34(WorldTradeWork *wk) {
    ArcTool *arc;

    if ((wk->unk12E0 == 1) && (wk->unk12E4 == 0)) {
        arc = GFL_ArcSysCreateFileHandle(0x60, 0x48);
        GFL_G2DIOLoadArcNCLRDefault(arc, 5, 4, 0, 0x20, 0x48);
        GFL_BGSysLoadArcNCGRStatic(arc, 0xf, 6, 0, 0, 0, 0x48);
        GFL_G2DIOLoadNSCRAsync(arc, 0x22, 6, 0, 0, 0, 0, 0x48);
        GFL_ArcToolFree(arc);
    }
    func_0204b3dc(wk->promptDsAct, 1);
}

void func_ov135_02200bac(WorldTradeWork *wk, s32 msgId) {
    u8 bg;
    BmpWin *pBVar1;
    GFLBitmap *bitmap;

    pBVar1 = BmpWin_CreateDynamic(4, 0xc, 0x13, 0x12, 4, 1, 1);
    wk->explainWin = pBVar1;
    bitmap = BmpWin_GetBitmap(pBVar1);
    GFL_BitmapFill(bitmap, 0);
    func_ov135_021f9218(wk->explainWin, wk->msgManager, msgId, &wk->print);
    pBVar1 = wk->explainWin;
    BmpWin_FlushChar(pBVar1);
    BmpWin_FlushMap(pBVar1);
    bg = BmpWin_GetBGIndex(pBVar1);
    GFL_BGSysQueueScrLoad(bg);
}

static inline void Title_ClearWin(BmpWin *win) {
    func_020456f8(win);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(win));
}


