// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov214/worldtrade_search.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "dpw/dpw_tr.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_state.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "pml/personal.h"
#include "worldtrade_local.h"
#include "system/bmp_menulist.h"
#include "system/bmp_winframe.h"
#include "system/wipe.h"
#include "app/worldtrade.h"
#include "system/game_system.h"

// Missing types and constants

#define LEVEL_PRINT_TBL_SEARCH 1

// Missing function prototypes

// The Global Trade Station's search screen, where the player sets the Pokémon, gender, level and country to look for,
// asks the server, and picks one of the trainers found on the lower screen. The names are ours, guessed

enum {
    SEARCH_SEQ_START,
    SEARCH_SEQ_MAIN,
    SEARCH_SEQ_END,
    SEARCH_SEQ_END_TO_ENTER,
    SEARCH_SEQ_INPUT_POKENAME_MES,
    SEARCH_SEQ_POKENAME_SELECT_LIST,
    SEARCH_SEQ_POKENAME_SELECT_WAIT,
    SEARCH_SEQ_SEX_SELECT_MES,
    SEARCH_SEQ_SEX_SELECT_LIST,
    SEARCH_SEQ_SEX_SELECT_WAIT,
    SEARCH_SEQ_LEVEL_SELECT_MES,
    SEARCH_SEQ_LEVEL_SELECT_LIST,
    SEARCH_SEQ_LEVEL_SELECT_WAIT,
    SEARCH_SEQ_COUNTRY_SELECT_MES,
    SEARCH_SEQ_COUNTRY_SELECT_LIST,
    SEARCH_SEQ_COUNTRY_SELECT_WAIT,
    SEARCH_SEQ_SEARCH_CHECK,
    SEARCH_SEQ_SERVER_QUERY,
    SEARCH_SEQ_SERVER_RESULT,
    SEARCH_SEQ_SEARCH_RESULT_MESSAGE,
    SEARCH_SEQ_SEARCH_RESULT_MESSAGE_WAIT,
    SEARCH_SEQ_RESULT_TOUCH_MESSAGE,
    SEARCH_SEQ_SERVER_QUERY_FAILURE,
    SEARCH_SEQ_MES_WAIT,
    SEARCH_SEQ_MES_WAIT_KEY,
    SEARCH_SEQ_MES_WAIT_FRAMES,
    SEARCH_SEQ_YESNO,
    SEARCH_SEQ_YESNO_SELECT,
    SEARCH_SEQ_TO_MAIN,
    SEARCH_SEQ_SEARCH_ERROR_MES,
    SEARCH_SEQ_ERROR_DISCONNECT_MES1,
    SEARCH_SEQ_ERROR_DISCONNECT_MES2,
    SEARCH_SEQ_EXCHANGE_SCREEN1,
    SEARCH_SEQ_RETURN_SCREEN2,
    SEARCH_SEQ_EXCHANGE_MAIN,
    SEARCH_SEQ_CURSOR_DECIDE_WAIT,
    SEARCH_SEQ_CURSOR_QUIT_WAIT,
    SEARCH_SEQ_CURSOR_VIEW_WAIT,
};

// Where the cursor is: the conditions on the left, the buttons on the right
enum {
    CURSOR_POS_POKEMON,
    CURSOR_POS_SEX,
    CURSOR_POS_LEVEL,
    CURSOR_POS_NATION,
    CURSOR_POS_VIEW,
    CURSOR_POS_SEARCH,
    CURSOR_POS_BACK,
    CURSOR_POS_NUM,
};

// The sub-process modes of the screens it goes to and comes from
#define SEARCH_MODE_FROM_TITLE 13
#define SEARCH_MODE_PARTNER_RETURN 15
#define PARTNER_MODE_FROM_SEARCH 16
#define ENTER_MODE_DISCONNECT 21

// The text color of the conditions and of the labels
#define SEARCH_INFO_COLOR 0x3c40
#define SEARCH_LABEL_COLOR 0x440

// The frames to wait for the server before giving up
#define SEARCH_TIMEOUT (30 * 60 * 4)

void func_ov135_021fdbcc(void);
void func_ov135_021fdce4(void);
void func_ov135_021fdd00(WorldTradeWork *wk);
void func_ov135_021fdda8(WorldTradeWork *wk);
void func_ov135_021fdefc(WorldTradeWork *wk);
void func_ov135_021fdf2c(WorldTradeWork *wk);
void func_ov135_021fe0bc(WorldTradeWork *wk);
void func_ov135_021fe118(WorldTradeWork *wk);
void func_ov135_021fe178(WorldTradeWork *wk);
int func_ov135_021fe1a4(WorldTradeWork *wk);
int func_ov135_021fe254(WorldTradeWork *wk);
void func_ov135_021fe264(WorldTradeWork *wk, int decide);
int func_ov135_021fe338(WorldTradeWork *wk);
int func_ov135_021fe60c(WorldTradeWork *wk);
int func_ov135_021fe6a0(WorldTradeWork *wk);
static int func_ov135_021fe744(WorldTradeWork *wk);
int func_ov135_021fe7f0(WorldTradeWork *wk);
int func_ov135_021fe84c(WorldTradeWork *wk);
int func_ov135_021fe8a8(WorldTradeWork *wk);
int func_ov135_021fe8e8(WorldTradeWork *wk);
int func_ov135_021fe93c(WorldTradeWork *wk);
int func_ov135_021fe974(WorldTradeWork *wk);
int func_ov135_021fe9b0(WorldTradeWork *wk);
void func_ov135_021fe9dc(WorldTradeWork *wk);
void func_ov135_021feae0(WorldTradeWork *wk, int touch);
int func_ov135_021feb50(WorldTradeWork *wk);
int func_ov135_021feb84(WorldTradeWork *wk);
int func_ov135_021febb8(WorldTradeWork *wk);
int func_ov135_021febe0(WorldTradeWork *wk);
int func_ov135_021fec00(WorldTradeWork *wk);
int func_ov135_021fed24(WorldTradeWork *wk);
int func_ov135_021fed4c(WorldTradeWork *wk);
int func_ov135_021fed6c(WorldTradeWork *wk);
int func_ov135_021fee1c(WorldTradeWork *wk);
int func_ov135_021fee44(WorldTradeWork *wk);
int func_ov135_021fee70(WorldTradeWork *wk);
int func_ov135_021feefc(WorldTradeWork *wk);
int func_ov135_021fef24(WorldTradeWork *wk);
int func_ov135_021fef50(WorldTradeWork *wk);
int func_ov135_021ff004(WorldTradeWork *wk);
int func_ov135_021ff024(WorldTradeWork *wk);
int func_ov135_021ff070(WorldTradeWork *wk);
int func_ov135_021ff078(WorldTradeWork *wk);
int func_ov135_021ff0a0(WorldTradeWork *wk);
int func_ov135_021ff0bc(WorldTradeWork *wk);
int func_ov135_021ff0e8(WorldTradeWork *wk);
int func_ov135_021ff118(WorldTradeWork *wk);
int func_ov135_021ff174(WorldTradeWork *wk);
int func_ov135_021ff198(WorldTradeWork *wk);
int func_ov135_021ff218(WorldTradeWork *wk);
int func_ov135_021ff26c(WorldTradeWork *wk);
int func_ov135_021ff2d0(WorldTradeWork *wk);
void func_ov135_021ff318(WorldTradeWork *wk, int msgNo, int wait, int a, int b);
void func_ov135_021ff384(BmpWin **win, BmpWin **countryWin, MsgData *msgManager, WorldTradePrint *print);
void func_ov135_021ff474(BmpWin *win, MsgData *msgManager, int flag, WorldTradePrint *print);
BOOL func_ov135_021ff4d0(const Dpw_Tr_PokemonSearchData *s1, const Dpw_Tr_PokemonSearchData *s2,
                                    int countryCode1, int countryCode2);
void func_ov135_021ff50c(WorldTradeWork *wk);
void func_ov135_021ff570(int rate);

// The cursor's position, its animation and its animation when chosen, at each place
static u16 data_ov135_02203d46[CURSOR_POS_NUM][4] = {
#include "asset/worldtrade_search_data_ov135_02203d46.inc"
};

static int (*data_ov135_02203d80[])(WorldTradeWork *wk) = {
    func_ov135_021fe1a4,
    func_ov135_021fe338,
    func_ov135_021feb50,
    func_ov135_021feb84,
    func_ov135_021febb8,
    func_ov135_021febe0,
    func_ov135_021fec00,
    func_ov135_021fed24,
    func_ov135_021fed4c,
    func_ov135_021fed6c,
    func_ov135_021fee1c,
    func_ov135_021fee44,
    func_ov135_021fee70,
    func_ov135_021feefc,
    func_ov135_021fef24,
    func_ov135_021fef50,
    func_ov135_021fe60c,
    func_ov135_021fe6a0,
    func_ov135_021fe744,
    func_ov135_021fe7f0,
    func_ov135_021fe84c,
    func_ov135_021fe8a8,
    func_ov135_021fe8e8,
    func_ov135_021ff0a0,
    func_ov135_021ff0bc,
    func_ov135_021ff0e8,
    func_ov135_021ff004,
    func_ov135_021ff024,
    func_ov135_021ff070,
    func_ov135_021ff078,
    func_ov135_021fe93c,
    func_ov135_021fe974,
    func_ov135_021ff118,
    func_ov135_021ff174,
    func_ov135_021ff198,
    func_ov135_021ff218,
    func_ov135_021ff26c,
    func_ov135_021ff2d0,
};

void func_ov135_021f4fc4(WorldTradeWork *wk);

extern const u32 data_ov135_02202eac;

// pokebw2's Search_BgInit

void func_ov135_021f513c(WorldTradeWork *wk);


int func_ov135_021fd924(WorldTradeWork *wk, int seq) {
    WorldTradeInputHeader header;

    func_ov135_021fe118(wk);
    func_ov135_021fdbcc();
    func_ov135_021fdd00(wk);
    func_ov135_021fdf2c(wk);
    func_ov135_021fdda8(wk);
    func_ov135_021fffe8(wk, 0, 0);
    func_ov135_02200b34(wk);
    func_ov135_02200bac(wk, 4);
    wk->unk12E4 = 0;

    header.menuWin = wk->menuWin;
    header.backWin = &wk->backWin;
    header.cursorAct = wk->subCursorAct;
    header.arrowAct[0] = wk->boxArrowAct[0];
    header.arrowAct[1] = wk->boxArrowAct[1];
    header.searchCursorAct = wk->cursorAct;
    header.msgManager = wk->msgManager;
    header.monsNameManager = wk->monsNameManager;
    header.countryNameManager = wk->countryNameManager;
    header.zukan = wk->param->pokedex;
    header.sinouTable = wk->depositWork->sinouTable;
    wk->inputWork = func_ov135_021f9890(&header, 2, INPUT_SITUATION_SEARCH);

    func_ov135_021ff384(&wk->infoWin[0], wk->countryWin, wk->msgManager, &wk->print);
    func_ov135_021ff474(wk->infoWin[8], wk->msgManager, wk->subLcdTouchOK, &wk->print);
    func_ov135_021f7f9c(wk->infoWin[1], wk->monsNameManager, wk->search.characterNo, 0, 0, SEARCH_INFO_COLOR,
                        &wk->print);
    func_ov135_021f8098(wk->infoWin[3], wk->msgManager, wk->search.gender, 1, 0, 0, SEARCH_INFO_COLOR, &wk->print);
    func_ov135_021f8154(wk->infoWin[5], wk->msgManager,
                        func_ov135_021f886c(wk->search.level_min, wk->search.level_max, LEVEL_PRINT_TBL_SEARCH), 0, 0,
                        SEARCH_INFO_COLOR, LEVEL_PRINT_TBL_SEARCH, &wk->print);
    func_ov135_021f8010(wk->countryWin[1], wk->countryNameManager, wk->msgManager, wk->countryCode, 0, 0,
                        SEARCH_INFO_COLOR, &wk->print);

    wk->vfunc2 = func_ov135_021ff50c;
    if (wk->subProcessMode == SEARCH_MODE_FROM_TITLE) {
        GFL_WipeSet(3, 1, 1, 0, 6, 1, HEAPID_WORLDTRADE);
    }
    wk->subprocessSeq = SEARCH_SEQ_START;
    return WT_SEQ_FADEIN;
}

int func_ov135_021fdae4(WorldTradeWork *wk, int seq) {
    int ret;
    int i;

    ret = data_ov135_02203d80[wk->subprocessSeq](wk);
    for (i = 0; i < 8; i++) {
        func_ov135_021f4ea0(wk->subAct[i], wk->subActPos[i].x, wk->subActPos[i].y + wk->drawOffset);
    }
    func_ov135_021f4ff4(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][0],
                        data_ov135_02203d46[func_ov135_021fe9b0(wk)][1] - wk->drawOffset);
    func_ov135_021f4ea0(wk->promptDsAct, 55, wk->drawOffset + 168);
    return ret;
}

int func_ov135_021fdb74(WorldTradeWork *wk, int seq) {
    wk->vfunc2 = NULL;
    func_ov135_021f99d0(wk->inputWork);
    func_ov135_021fdefc(wk);
    func_ov135_021fe178(wk);
    func_ov135_021fe0bc(wk);
    func_ov135_021fdce4();
    if (wk->subNextProcess == WORLDTRADE_TITLE) {
        wk->unk12E4 = 1;
    }
    func_ov135_022000e4(wk);
    func_0204b3dc(wk->promptDsAct, FALSE);
    func_ov135_021f4ec8(wk);
    return WT_SEQ_INIT;
}

void func_ov135_021fdbcc(void) {
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
            GX_BG_SCRBASE_0x0000,
            GX_BG_CHARBASE_0x04000,
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
            GX_BG_SCRBASE_0x0800,
            GX_BG_CHARBASE_0x0c000,
            0x8000,
            GX_BG_EXTPLTT_01,
            3,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(1, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(1);
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
            GX_BG_SCRBASE_0x1000,
            GX_BG_CHARBASE_0x08000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(2, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(2);
        GFL_BGSysSetBGEnabled(2, TRUE);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0x1800,
            GX_BG_CHARBASE_0x1c000,
            0x8000,
            GX_BG_EXTPLTT_01,
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(3, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(3);
        GFL_BGSysSetBGEnabled(3, TRUE);
    }
    GFL_BGSysClearCharCore(2, 32, 0, HEAPID_WORLDTRADE);
    GFL_BGSysClearCharCore(0, 32, 0, HEAPID_WORLDTRADE);
    GFL_BGSysClearCharCore(3, 32, 0, HEAPID_WORLDTRADE);
}

// pokebw2's Search_BgExit
void func_ov135_021fdce4(void) {
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(0);
    GFL_BGSysReleaseBG(3);
}

// pokebw2's Search_BgGraphicSet
void func_ov135_021fdd00(WorldTradeWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_WORLDTRADE, HEAPID_WORLDTRADE);

    GFL_G2DIOLoadArcNCLRDefault(arc, 2, 0, 0, 0x60, HEAPID_WORLDTRADE);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x1a0, 0x20, HEAPID_WORLDTRADE);
    LoadSysMsgBox(0, 1, 14, 0, HEAPID_WORLDTRADE);
    LoadSysMsgBox(0, 31, 11, 0, HEAPID_WORLDTRADE);
    GFL_BGSysLoadArcNCGRStatic(arc, 11, 1, 0, 0, TRUE, HEAPID_WORLDTRADE);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 23, 1, 0, 0, TRUE, HEAPID_WORLDTRADE);
    GFL_BGSysLoadArcNCGRStatic(arc, 12, 2, 0, 0, TRUE, HEAPID_WORLDTRADE);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 0x20, 0x20, HEAPID_WORLDTRADE);
    GFL_ArcToolFree(arc);
}

void func_ov135_021fdda8(WorldTradeWork *wk) {
    ClActorSetup setup;

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.x = data_ov135_02203d46[0][0];
    setup.y = data_ov135_02203d46[0][1];
    wk->cursorAct = func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_MAIN2][WT_CLACT_RES_CHAR],
                                  wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_PLTT],
                                  wk->clactRes[WT_CLACT_RES_MAIN2][WT_CLACT_RES_CELL], &setup, 0, HEAPID_WORLDTRADE);
    func_0204b92c(wk->cursorAct, TRUE);
    func_0204b878(wk->cursorAct, data_ov135_02203d46[0][2]);
    func_0204b84c(wk->cursorAct, 1);
    if (func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, FALSE);
    } else {
        func_0204b3dc(wk->cursorAct, TRUE);
    }

    setup.x = 160;
    setup.y = 32;
    wk->subCursorAct = func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_MAIN2][WT_CLACT_RES_CHAR],
                                     wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_PLTT],
                                     wk->clactRes[WT_CLACT_RES_MAIN2][WT_CLACT_RES_CELL], &setup, 0, HEAPID_WORLDTRADE);
    func_0204b878(wk->subCursorAct, 4);
    func_0204b3dc(wk->subCursorAct, FALSE);

    setup.x = 228;
    setup.y = 117;
    wk->boxArrowAct[0] = func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CHAR],
                                       wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_PLTT],
                                       wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CELL], &setup, 0,
                                       HEAPID_WORLDTRADE);
    func_0204b878(wk->boxArrowAct[0], 38);
    func_0204b3dc(wk->boxArrowAct[0], FALSE);

    setup.x = 140;
    wk->boxArrowAct[1] = func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CHAR],
                                       wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_PLTT],
                                       wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CELL], &setup, 0,
                                       HEAPID_WORLDTRADE);
    func_0204b878(wk->boxArrowAct[1], 39);
    func_0204b3dc(wk->boxArrowAct[1], FALSE);

    func_0203d34c(FALSE, HEAPID_WORLDTRADE);
}


void func_ov135_021fdefc(WorldTradeWork *wk) {
    func_0204b3b4(wk->cursorAct);
    func_0204b3b4(wk->subCursorAct);
    func_0204b3b4(wk->boxArrowAct[0]);
    func_0204b3b4(wk->boxArrowAct[1]);
}

// The windows of the search's conditions, of its buttons and of its country
// pokebw2's sSearchCountryWinPos
static const u16 data_ov135_022037a8[2][2] = {
    { 1, 16 },
    { 2, 18 },
};

// pokebw2's sSearchButtonWinPos
static const u16 data_ov135_022037b0[3][2] = {
    { 22, 8 },
    { 22, 13 },
    { 22, 3 },
};

// pokebw2's sSearchInfoWinPos
static const u16 data_ov135_022037bc[6][2] = {
    { 1, 1 }, { 2, 3 }, { 1, 6 }, { 2, 8 }, { 1, 11 }, { 2, 13 },
};

void func_ov135_021fdf2c(WorldTradeWork *wk) {
    BmpWin *win;
    int i;

    wk->msgWin = BmpWin_CreateDynamic(0, 2, 21, 27, 2, 13, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->msgWin), 0);
    win = wk->msgWin;
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(win));
    GFL_BGSysFillChar(3, 0, 1, 0);

    for (i = 0; i < 6; i++) {
        wk->infoWin[i] = BmpWin_CreateDynamic(3, data_ov135_022037bc[i][0], data_ov135_022037bc[i][1], 11, 2, 13, FALSE);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->infoWin[i]), 0);
        win = wk->infoWin[i];
        BmpWin_FlushChar(win);
        BmpWin_FlushMap(win);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(win));
    }
    for (i = 0; i < 2; i++) {
        wk->countryWin[i] = BmpWin_CreateDynamic(3, data_ov135_022037a8[i][0], data_ov135_022037a8[i][1], 28, 2, 13, FALSE);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->countryWin[i]), 0);
        win = wk->countryWin[i];
        BmpWin_FlushChar(win);
        BmpWin_FlushMap(win);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(win));
    }
    for (i = 0; i < 3; i++) {
        wk->infoWin[6 + i] = BmpWin_CreateDynamic(3, data_ov135_022037b0[i][0], data_ov135_022037b0[i][1], 10, 2, 13, FALSE);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->infoWin[6 + i]), 0);
        win = wk->infoWin[6 + i];
        BmpWin_FlushChar(win);
        BmpWin_FlushMap(win);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(win));
    }
}


void func_ov135_02202940(WorldTradePrint *print);
void func_ov135_021fe0bc(WorldTradeWork *wk) {
    int i;

    func_ov135_02202940(&wk->print);
    BmpWin_Free(wk->explainWin);
    BmpWin_Free(wk->msgWin);
    for (i = 0; i < 9; i++) {
        BmpWin_Free(wk->infoWin[i]);
    }
    for (i = 0; i < 2; i++) {
        BmpWin_Free(wk->countryWin[i]);
    }
    GFL_BGSysFreeFilledChar(3, 1, 0);
}

void func_ov135_021fe118(WorldTradeWork *wk) {
    StrBuf *pSVar1;
    void *dest;

    pSVar1 = GFL_StrBufCreate(0xb4, 0x48);
    wk->talkString = pSVar1;
    pSVar1 = GFL_MsgDataLoadStrbufNew(wk->msgManager, 0x30);
    wk->titleString = pSVar1;
    dest = GFL_HeapAllocate(0x48,0x30,0,"worldtrade_search.c",0x351);
    wk->depositWork = dest;
    MIi_CpuClearFast(0, dest, 0x30);
    wk->depositWork->sinouTable = func_ov135_021f84ec(0x48);
    func_ov135_021f8990(&wk->selectListPos);
}

void func_ov135_021fe178(WorldTradeWork *wk) {
    GFL_HeapFree(wk->depositWork->sinouTable);
    GFL_HeapFree(wk->depositWork);
    GFL_StrBufFree(wk->talkString);
    GFL_StrBufFree(wk->titleString);
}

int func_ov135_021fe1a4(WorldTradeWork *wk) {
    if (func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, FALSE);
    } else {
        func_0204b3dc(wk->cursorAct, TRUE);
    }

    if (wk->subProcessMode == SEARCH_MODE_PARTNER_RETURN) {
        if (wk->searchResult != 0) {
            func_ov135_021ff318(wk, 0x26, 0, 0, 0xf0f);
        } else {
            func_ov135_021ff318(wk, 8, 1, 0, 0xf0f);
        }
        if (GXx_GetMasterBrightness_(REG_MASTER_BRIGHT_ADDR) == -16) {
            GFL_WipeSet(0, 1, 1, 0, 16, 1, HEAPID_WORLDTRADE);
            wk->subprocessSeq = SEARCH_SEQ_RETURN_SCREEN2;
        } else {
            wk->subprocessSeq = SEARCH_SEQ_MAIN;
        }
    } else {
        func_ov135_021ff318(wk, 8, 1, 0, 0xf0f);
        func_ov135_021f4e98(wk, SEARCH_SEQ_MES_WAIT, SEARCH_SEQ_MAIN);
    }
    func_0204b3dc(wk->promptDsAct, TRUE);
    return WT_SEQ_MAIN;
}


// pokebw2's sSearchTouchRects
static const TouchRect data_ov135_022037d4[] = {
    { 3, 41, 2, 99 },     { 43, 81, 2, 99 },    { 83, 121, 2, 99 },    { 123, 161, 2, 99 },
    { 19, 45, 145, 255 }, { 58, 85, 145, 255 }, { 98, 125, 145, 255 }, { TOUCH_RECT_END, 0, 0, 0 },
};

// The column and row of the cursor that each touch rectangle puts it on
static u8 data_ov135_02203d38[CURSOR_POS_NUM][2] = {
    { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 }, { 1, 0 }, { 1, 1 }, { 1, 2 },
};

// pokebw2's Search_TouchPanelFunc
int func_ov135_021fe254(WorldTradeWork *wk) {
    return func_02035c08(data_ov135_022037d4);
}

void func_ov135_021fe264(WorldTradeWork *wk, int decide) {
    void *personal;
    int sexSelection;

    switch (decide) {
    case CURSOR_POS_POKEMON:
        wk->subprocessSeq = SEARCH_SEQ_INPUT_POKENAME_MES;
        break;
    case CURSOR_POS_SEX:
        if (wk->search.characterNo != 0) {
            personal = PML_PersonalLoad(wk->search.characterNo, 0, HEAPID_WORLDTRADE);
            sexSelection = PML_PersonalGetParam(personal, 20);
            PML_PersonalFree(personal);
            wk->depositWork->sexSelection = sexSelection;
            if (func_ov135_021f7b9c(&wk->search, wk->depositWork->sexSelection)) {
                break;
            }
        }
        wk->subprocessSeq = SEARCH_SEQ_SEX_SELECT_MES;
        break;
    case CURSOR_POS_LEVEL:
        wk->subprocessSeq = SEARCH_SEQ_LEVEL_SELECT_MES;
        break;
    case CURSOR_POS_NATION:
        wk->subprocessSeq = SEARCH_SEQ_COUNTRY_SELECT_MES;
        break;
    case CURSOR_POS_VIEW:
        if (wk->searchResult != 0) {
            wk->subprocessSeq = SEARCH_SEQ_EXCHANGE_SCREEN1;
            GFL_WipeSet(0, 0, 0, 0, 16, 1, HEAPID_WORLDTRADE);
        }
        break;
    case CURSOR_POS_SEARCH:
        wk->subprocessSeq = SEARCH_SEQ_SEARCH_CHECK;
        break;
    case CURSOR_POS_BACK:
        func_ov135_021ff318(wk, 0xf, 1, 0, 0xf0f);
        func_ov135_021f4e98(wk, SEARCH_SEQ_MES_WAIT, SEARCH_SEQ_YESNO);
        break;
    }
}

int func_ov135_021fe338(WorldTradeWork *wk) {
    int touch = func_ov135_021fe254(wk);
    void *personal;
    int sexSelection;
    int pos;

    if (touch != -1) {
        func_020355d8(TRUE);
        func_ov135_021feae0(wk, touch);
        pos = func_ov135_021fe9b0(wk);
        switch (pos) {
        case CURSOR_POS_SEX:
            if (wk->search.characterNo != 0) {
                personal = PML_PersonalLoad(wk->search.characterNo, 0, HEAPID_WORLDTRADE);
                sexSelection = PML_PersonalGetParam(personal, 20);
                PML_PersonalFree(personal);
                wk->depositWork->sexSelection = sexSelection;
                if (func_ov135_021f7b9c(&wk->search, wk->depositWork->sexSelection)) {
                    func_0204b3dc(wk->cursorAct, TRUE);
                    func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][3]);
                    GFL_SndSEPlay(SEQ_SE_BEEP);
                    return WT_SEQ_MAIN;
                }
            }
            func_0204b3dc(wk->cursorAct, TRUE);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][3]);
            wk->subprocessSeq = SEARCH_SEQ_CURSOR_DECIDE_WAIT;
            break;
        case CURSOR_POS_SEARCH:
            if (wk->search.characterNo == 0 ||
                func_ov135_021ff4d0(&wk->search, &wk->searchBackup, wk->countryCode, wk->searchBackupCountryCode)) {
                GFL_SndSEPlay(SEQ_SE_BEEP);
            } else {
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
            }
            func_0204b3dc(wk->cursorAct, TRUE);
            func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][3]);
            wk->subprocessSeq = SEARCH_SEQ_CURSOR_DECIDE_WAIT;
            break;
        case CURSOR_POS_VIEW:
            if (wk->searchResult != 0) {
                func_0204b3dc(wk->cursorAct, TRUE);
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
                func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][3]);
                wk->subprocessSeq = SEARCH_SEQ_CURSOR_DECIDE_WAIT;
            } else {
                func_0204b3dc(wk->cursorAct, TRUE);
                func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][3]);
                GFL_SndSEPlay(SEQ_SE_BEEP);
                wk->subprocessSeq = SEARCH_SEQ_CURSOR_VIEW_WAIT;
            }
            break;
        default:
            func_0204b3dc(wk->cursorAct, TRUE);
            if (pos == CURSOR_POS_BACK) {
                GFL_SndSEPlay(SEQ_SE_CANCEL1);
            } else {
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
            }
            func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][3]);
            wk->subprocessSeq = SEARCH_SEQ_CURSOR_DECIDE_WAIT;
            break;
        }
    } else {
        if (GCTX_HIDGetPressedKeys() != 0 && func_020355c4() == TRUE) {
            func_0204b3dc(wk->cursorAct, TRUE);
            func_020355d8(FALSE);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return WT_SEQ_MAIN;
        }
        func_ov135_021fe9dc(wk);
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
            pos = func_ov135_021fe9b0(wk);
            switch (pos) {
            case CURSOR_POS_SEX:
                if (wk->search.characterNo != 0) {
                    personal = PML_PersonalLoad(wk->search.characterNo, 0, HEAPID_WORLDTRADE);
                    sexSelection = PML_PersonalGetParam(personal, 20);
                    PML_PersonalFree(personal);
                    wk->depositWork->sexSelection = sexSelection;
                    if (func_ov135_021f7b9c(&wk->search, wk->depositWork->sexSelection)) {
                        func_0204b3dc(wk->cursorAct, TRUE);
                        func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][3]);
                        GFL_SndSEPlay(SEQ_SE_BEEP);
                        return WT_SEQ_MAIN;
                    }
                }
                func_0204b3dc(wk->cursorAct, TRUE);
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
                func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][3]);
                wk->subprocessSeq = SEARCH_SEQ_CURSOR_DECIDE_WAIT;
                break;
            case CURSOR_POS_SEARCH:
                if (wk->search.characterNo == 0 ||
                    func_ov135_021ff4d0(&wk->search, &wk->searchBackup, wk->countryCode,
                                        wk->searchBackupCountryCode)) {
                    GFL_SndSEPlay(SEQ_SE_BEEP);
                } else {
                    GFL_SndSEPlay(SEQ_SE_DECIDE1);
                }
                func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][3]);
                wk->subprocessSeq = SEARCH_SEQ_CURSOR_DECIDE_WAIT;
                break;
            case CURSOR_POS_VIEW:
                if (wk->searchResult != 0) {
                    GFL_SndSEPlay(SEQ_SE_DECIDE1);
                    func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][3]);
                    wk->subprocessSeq = SEARCH_SEQ_CURSOR_DECIDE_WAIT;
                } else {
                    func_0204b3dc(wk->cursorAct, TRUE);
                    func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][3]);
                    GFL_SndSEPlay(SEQ_SE_BEEP);
                    wk->subprocessSeq = SEARCH_SEQ_CURSOR_VIEW_WAIT;
                }
                break;
            default:
                if (pos == CURSOR_POS_BACK) {
                    GFL_SndSEPlay(SEQ_SE_CANCEL1);
                } else {
                    GFL_SndSEPlay(SEQ_SE_DECIDE1);
                }
                func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][3]);
                wk->subprocessSeq = SEARCH_SEQ_CURSOR_DECIDE_WAIT;
                break;
            }
        } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
            wk->depositWork->cursorSide = 1;
            wk->depositWork->rightCursorPos = 2;
            func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][3]);
            wk->subprocessSeq = SEARCH_SEQ_CURSOR_QUIT_WAIT;
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
        }
    }
    return WT_SEQ_MAIN;
}



int func_ov135_021fe60c(WorldTradeWork *wk) {
    if (wk->search.characterNo == 0) {
        func_ov135_021ff318(wk, 0xc, 1, 0, 0xf0f);
        func_ov135_021f4e98(wk, SEARCH_SEQ_MES_WAIT, SEARCH_SEQ_MAIN);
    } else if (func_ov135_021ff4d0(&wk->search, &wk->searchBackup, wk->countryCode, wk->searchBackupCountryCode)) {
        func_ov135_021ff318(wk, 0x27, 1, 0, 0xf0f);
        func_ov135_021f4e98(wk, SEARCH_SEQ_MES_WAIT, SEARCH_SEQ_MAIN);
    } else {
        GFL_SndSEPlay(SEQ_SE_SYS_78);
        wk->unk12E8 = 118;
        func_ov135_021ff318(wk, 0xd, 1, 0, 0xf0f);
        func_ov135_021f4e98(wk, SEARCH_SEQ_MES_WAIT, SEARCH_SEQ_SERVER_QUERY);
        if (wk->searchResult > 0) {
            func_ov135_021ffbc8(wk);
        }
    }
    return WT_SEQ_MAIN;
}


int func_ov135_021fe6a0(WorldTradeWork *wk) {
    Dpw_Tr_PokemonSearchDataEx DStack_18;

    if (wk->countryCode == 0) {
        func_ov114_021c8b18(&wk->search, 7, wk->downloadData);
    }
    else {
        MI_CpuFill8(&DStack_18, 0, 8);
        DStack_18.characterNo = wk->search.characterNo;
        DStack_18.gender = wk->search.gender;
        DStack_18.level_min = wk->search.level_min;
        DStack_18.level_max = wk->search.level_max;
        DStack_18.unused = wk->search.unused;
        DStack_18.maxNum = 7;
        DStack_18.countryCode = (u8)wk->countryCode;
        func_ov114_021c8bc4(&DStack_18, wk->downloadData);
    }
    wk->searchBackup.characterNo = *(u16 *)&wk->search.characterNo;
    *(u16 *)&wk->searchBackup.gender = *(u16 *)&wk->search.gender;
    *(u16 *)&wk->searchBackup.level_max = *(u16 *)&wk->search.level_max;
    wk->searchBackupCountryCode = wk->countryCode;
    wk->timeoutCount = 0;
    wk->subprocessSeq = 0x12;
    wk->subLcdTouchOK = 0;
    return 2;
}

static int func_ov135_021fe744(WorldTradeWork *wk) {
    s32 result;

    if (func_ov114_021c866c()) {
        result = func_ov114_021c86a8();
        wk->timeoutCount = 0;
        switch (result) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            wk->searchResult = result;
            wk->subprocessSeq = SEARCH_SEQ_SEARCH_RESULT_MESSAGE;
            break;
        case -2:
        case -14:
            GFL_SndStop();
            wk->subprocessSeq = SEARCH_SEQ_ERROR_DISCONNECT_MES1;
            func_0200fbb0();
            func_0203ca04();
            func_0200fba0();
            func_0203c968();
            break;
        case -13:
            GFL_SndStop();
            func_ov135_021f513c(wk);
            break;
        case -12:
        case -15:
            GFL_SndStop();
            wk->subprocessSeq = SEARCH_SEQ_SERVER_QUERY_FAILURE;
            break;
        }
    } else if (++wk->timeoutCount == SEARCH_TIMEOUT) {
        GFL_SndStop();
        func_ov135_021f513c(wk);
    }
    return WT_SEQ_MAIN;
}


int func_ov135_021fe7f0(WorldTradeWork *wk) {
    if (wk->unk12E8 == 0) {
        GFL_SndStop();
        func_ov135_021ffb20(wk, wk->searchResult, 1);
        if (wk->searchResult == 0) {
            func_ov135_021ff474(wk->infoWin[8], wk->msgManager, 0, &wk->print);
        } else {
            func_ov135_021ff474(wk->infoWin[8], wk->msgManager, 1, &wk->print);
        }
        wk->subprocessSeq = SEARCH_SEQ_SEARCH_RESULT_MESSAGE_WAIT;
    }
    return 2;
}

int func_ov135_021fe84c(WorldTradeWork *wk) {
    if (wk->searchResult == 0) {
        func_ov135_021ff318(wk, 0xe, 1, 0, 0xf0f);
        func_ov135_021f4e98(wk, 0x17, 1);
        GFL_SndSEPlay(SEQ_SE_BEEP);
    } else {
        func_ov135_021ff318(wk, 0x24, 1, 0, 0xf0f);
        func_ov135_021f4e98(wk, 0x17, 0x15);
        wk->wait = 0;
    }
    return 2;
}



int func_ov135_021fe8a8(WorldTradeWork *wk) {
    if (++wk->wait > 45) {
        func_ov135_021ff318(wk, 0x26, 1, 0, 0xf0f);
        func_ov135_021f4e98(wk, 0x17, 1);
        wk->subLcdTouchOK = 1;
    }
    return 2;
}

int func_ov135_021fe8e8(WorldTradeWork *wk) {
    wk->searchBackup.characterNo = 0;
    func_ov135_021ff318(wk, 0x2c, 1, 0, 0xf0f);
    func_ov135_021f4e98(wk, 0x17, 1);
    GFL_SndSEPlay(SEQ_SE_BEEP);
    wk->searchResult = 0;
    func_ov135_021ff474(wk->infoWin[8], wk->msgManager, 0, &wk->print);
    return 2;
}



int func_ov135_021fe93c(WorldTradeWork *wk) {
    func_ov135_021ff318(wk, 0xab, 4, 0, 0xf0f);
    func_ov135_021f4e98(wk, 0x18, 0x1f);
    wk->wait = 0;
    GFL_SndSEPlay(SEQ_SE_BEEP);
    return 2;
}

int func_ov135_021fe974(WorldTradeWork *wk) {
    func_ov135_021ff318(wk, 0xac, 4, 0, 0xf0f);
    func_ov135_021f4e98(wk, 0x18, 3);
    func_ov135_021f4ec0(wk, 0, 0x15);
    GFL_SndSEPlay(SEQ_SE_BEEP);
    return 2;
}



int func_ov135_021fe9b0(WorldTradeWork *wk) {
    int pos;

    if (wk->depositWork->cursorSide == 0) {
        pos = wk->depositWork->leftCursorPos;
    } else {
        pos = wk->depositWork->rightCursorPos + 4;
    }
    GFL_ASSERT(pos < 7);
    return pos;
}

void func_ov135_021fe9dc(WorldTradeWork *wk) {
    if (GCTX_HIDGetPressedKeys() & PAD_KEY_UP) {
        if (wk->depositWork->cursorSide == 0) {
            if (wk->depositWork->leftCursorPos > 0) {
                wk->depositWork->leftCursorPos--;
                GFL_SndSEPlay(SEQ_SE_SELECT1);
                func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][2]);
            }
        } else if (wk->depositWork->rightCursorPos > 0) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            wk->depositWork->rightCursorPos--;
            func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][2]);
        }
    } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_DOWN) {
        if (wk->depositWork->cursorSide == 0) {
            if (wk->depositWork->leftCursorPos < 3) {
                wk->depositWork->leftCursorPos++;
                GFL_SndSEPlay(SEQ_SE_SELECT1);
                func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][2]);
            }
        } else if (wk->depositWork->rightCursorPos < 2) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            wk->depositWork->rightCursorPos++;
            func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][2]);
        }
    } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_RIGHT) {
        if (wk->depositWork->cursorSide != 1) {
            wk->depositWork->cursorSide = 1;
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][2]);
        } else {
            wk->depositWork->cursorSide = 1;
        }
    } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_LEFT) {
        if (wk->depositWork->cursorSide != 0) {
            wk->depositWork->cursorSide = 0;
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][2]);
        } else {
            wk->depositWork->cursorSide = 0;
        }
    }
    func_ov135_021f4ff4(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][0],
                        data_ov135_02203d46[func_ov135_021fe9b0(wk)][1]);
}

void func_ov135_021feae0(WorldTradeWork *wk, int touch) {
    if (data_ov135_02203d38[touch][0] == 0) {
        wk->depositWork->cursorSide = 0;
        wk->depositWork->leftCursorPos = data_ov135_02203d38[touch][1];
    } else {
        wk->depositWork->cursorSide = 1;
        wk->depositWork->rightCursorPos = data_ov135_02203d38[touch][1];
    }
    func_ov135_021f4ff4(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][0],
                        data_ov135_02203d46[func_ov135_021fe9b0(wk)][1]);
}




int func_ov135_021feb50(WorldTradeWork *wk) {
    if (wk->drawOffset == 0) {
        GFL_WipeSet(3, 0, 0, 0, 6, 1, 0x48);
    }
    wk->subprocessSeq = 0;
    return 3;
}

int func_ov135_021feb84(WorldTradeWork *wk) {
    func_ov114_021c8658();
    func_ov135_021f4fc4(wk);
    GFL_WipeSet(0, 0, 0, 0, 6, 1, 0x48);
    wk->subprocessSeq = 0;
    return 3;
}

int func_ov135_021febb8(WorldTradeWork *wk) {
    func_ov135_021ff318(wk, 9, 1, 0, 0xf0f);
    func_ov135_021f4e98(wk, 0x17, 5);
    return 2;
}


int func_ov135_021febe0(WorldTradeWork *wk) {
    func_ov135_021f9968(wk->inputWork, 0);
    func_ov135_021ff570(9);
    wk->subprocessSeq = 6;
    return 2;
}

int func_ov135_021fec00(WorldTradeWork *wk) {
    u32 ret = func_ov135_021f99f0(wk->inputWork);
    void *personal;
    int sexSelection;

    switch (ret) {
    case BMPMENULIST_CANCEL:
        func_ov135_021ff570(0);
        wk->subprocessSeq = SEARCH_SEQ_START;
        func_ov135_021f89a8(&wk->selectListPos, wk->depositWork->headwordListPos + wk->depositWork->headwordPos,
                            wk->depositWork->nameListPos, wk->depositWork->namePos);
        break;
    case BMPMENULIST_NULL:
        break;
    default:
        func_ov135_021ff570(0);
        wk->search.characterNo = ret;
        wk->subprocessSeq = SEARCH_SEQ_START;
        GFL_BitmapFill(BmpWin_GetBitmap(wk->infoWin[1]), 0);
        func_ov135_021f7f9c(wk->infoWin[1], wk->monsNameManager, ret, 0, 0, SEARCH_INFO_COLOR, &wk->print);
        personal = PML_PersonalLoad(ret, 0, HEAPID_WORLDTRADE);
        sexSelection = PML_PersonalGetParam(personal, 20);
        PML_PersonalFree(personal);
        wk->depositWork->sexSelection = sexSelection;
        func_ov135_021f89a8(&wk->selectListPos, wk->depositWork->headwordListPos + wk->depositWork->headwordPos,
                            wk->depositWork->nameListPos, wk->depositWork->namePos);
        if (func_ov135_021f7b9c(&wk->search, wk->depositWork->sexSelection)) {
            GFL_BitmapFill(BmpWin_GetBitmap(wk->infoWin[3]), 0);
            func_ov135_021f8098(wk->infoWin[3], wk->msgManager, wk->search.gender, 1, 0, 0, SEARCH_INFO_COLOR,
                                &wk->print);
        }
        break;
    }
    return WT_SEQ_MAIN;
}


int func_ov135_021fed24(WorldTradeWork *wk) {
    func_ov135_021ff318(wk, 10, 1, 0, 0xf0f);
    func_ov135_021f4e98(wk, 0x17, 8);
    return 2;
}


int func_ov135_021fed4c(WorldTradeWork *wk) {
    func_ov135_021f9968(wk->inputWork, 1);
    func_ov135_021ff570(9);
    wk->subprocessSeq = 9;
    return 2;
}

int func_ov135_021fed6c(WorldTradeWork *wk) {
    u32 ret = func_ov135_021f99f0(wk->inputWork);
    BmpWin *win;

    switch (ret) {
    case BMPMENULIST_CANCEL:
        func_ov135_021ff570(0);
        func_0201f7a8(wk->msgWin, 0);
        win = wk->msgWin;
        BmpWin_FlushChar(win);
        BmpWin_FlushMap(win);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(win));
        wk->subprocessSeq = SEARCH_SEQ_START;
        break;
    case 0:
    case 1:
    case 2:
        func_ov135_021ff570(0);
        wk->search.gender = ret + 1;
        wk->subprocessSeq = SEARCH_SEQ_START;
        GFL_BitmapFill(BmpWin_GetBitmap(wk->infoWin[3]), 0);
        func_ov135_021f8098(wk->infoWin[3], wk->msgManager, wk->search.gender, 1, 0, 0, SEARCH_INFO_COLOR, &wk->print);
        break;
    }
    return WT_SEQ_MAIN;
}


int func_ov135_021fee1c(WorldTradeWork *wk) {
    func_ov135_021ff318(wk, 11, 1, 0, 0xf0f);
    func_ov135_021f4e98(wk, 0x17, 11);
    return 2;
}


int func_ov135_021fee44(WorldTradeWork *wk) {
    wk->listPos = 0xffff;
    func_ov135_021f9968(wk->inputWork, 2);
    func_ov135_021ff570(9);
    wk->subprocessSeq = 0xc;
    return 2;
}

int func_ov135_021fee70(WorldTradeWork *wk) {
    u32 ret = func_ov135_021f99f0(wk->inputWork);

    switch (ret) {
    case 11:
    case BMPMENULIST_CANCEL:
        func_ov135_021ff570(0);
        wk->subprocessSeq = 0;
        break;
    case BMPMENULIST_NULL:
        break;
    default:
        func_ov135_021ff570(0);
        func_ov135_021f882c(&wk->search, ret, 1);
        wk->subprocessSeq = 0;
        GFL_BitmapFill(BmpWin_GetBitmap(wk->infoWin[5]), 0);
        func_ov135_021f8154(wk->infoWin[5], wk->msgManager, ret, 0, 0, (0xf1 << 6), 1, &wk->print);
        break;
    }
    return 2;
}


int func_ov135_021feefc(WorldTradeWork *wk) {
    func_ov135_021ff318(wk, 0xbe, 1, 0, 0xf0f);
    func_ov135_021f4e98(wk, 0x17, 0xe);
    return 2;
}


int func_ov135_021fef24(WorldTradeWork *wk) {
    wk->listPos = 0xffff;
    func_ov135_021f9968(wk->inputWork, 3);
    func_ov135_021ff570(9);
    wk->subprocessSeq = 0xf;
    return 2;
}

int func_ov135_021fef50(WorldTradeWork *wk) {
    u8 bg;
    u32 countryCode;
    GFLBitmap *bitmap;
    BmpWin *window;

    countryCode = func_ov135_021f99f0(wk->inputWork);
    if (countryCode != 0xffffffff) {
        if ((countryCode == 0xfffffffe) || (countryCode == data_ov135_02202eac + 1U)) {
            func_ov135_021ff570(0);
            func_0201f7a8(wk->msgWin, 0);
            window = wk->msgWin;
            BmpWin_FlushChar(window);
            BmpWin_FlushMap(window);
            bg = BmpWin_GetBGIndex(window);
            GFL_BGSysLoadScr(bg);
            wk->subprocessSeq = 0;
        }
        else {
            func_ov135_021ff570(0);
            func_ov135_021f88b0(wk, countryCode);
            wk->subprocessSeq = 0;
            bitmap = BmpWin_GetBitmap(wk->countryWin[1]);
            GFL_BitmapFill(bitmap, 0);
            func_ov135_021f8010(wk->countryWin[1], wk->countryNameManager,
                    wk->msgManager, wk->countryCode, 0, 0, 0x3c40, &wk->print);
        }
    }
    return 2;
}

int func_ov135_021ff004(WorldTradeWork *wk) {
    func_ov135_021f4be0(wk, 0x14, 0x120, 3, 1);
    wk->subprocessSeq = 0x1b;
    return 2;
}

int func_ov135_021ff024(WorldTradeWork *wk) {
    u8 result;

    result = func_ov135_021f4d14(wk);
    if (result == 1) {
        func_ov135_021f4ce4(wk);
        wk->subprocessSeq = 2;
        func_ov135_021f4ec0(wk, 1, 0);
        if (0 < wk->searchResult) {
            func_ov135_021ffbc8(wk);
        }
        wk->searchResult = 0;
    }
    else if (result == 2) {
        func_ov135_021f4ce4(wk);
        wk->subprocessSeq = 0;
    }
    return 2;
}

// pokebw2's Search_SubSeqToMain
int func_ov135_021ff070(WorldTradeWork *wk) {
    wk->subprocessSeq = SEARCH_SEQ_MAIN;
    return WT_SEQ_MAIN;
}

int func_ov135_021ff078(WorldTradeWork *wk) {
    func_ov135_021ff318(wk, 0x9e, 1, 0, 0xf0f);
    func_ov135_021f4e98(wk, 0x17, 1);
    return 2;
}


int func_ov135_021ff0a0(WorldTradeWork *wk) {
    BOOL BVar1;

    BVar1 = func_ov135_02202840(&wk->print);
    if (BVar1 == 0) {
        wk->subprocessSeq = wk->subprocessNextSeq;
    }
    return 2;
}

int func_ov135_021ff0bc(WorldTradeWork *wk) {
    BOOL BVar1;
    BVar1 = func_ov135_02202840(&wk->print);
    if ((BVar1 == 0) && ((GCTX_HIDGetPressedKeys() != 0 || (BVar1 = func_02035c60(), BVar1 != 0)))) {
        wk->subprocessSeq = wk->subprocessNextSeq;
    }
    return 2;
}

int func_ov135_021ff0e8(WorldTradeWork *wk) {
    BOOL BVar1;
    s32 val;

    BVar1 = func_ov135_02202840(&wk->print);
    if ((BVar1 == 0) &&(val = wk->wait + 1, wk->wait = val, 0x2d < val)) {
        wk->wait = 0;
        wk->subprocessSeq = wk->subprocessNextSeq;
    }
    return 2;
}

int func_ov135_021ff118(WorldTradeWork *wk) {
    BOOL BVar1;

    wk->drawOffset = wk->drawOffset + 1;
    BVar1 = GFL_WipeIsFinished();
    if (BVar1 != 0) {
        reg_GX_POWCNT = reg_GX_POWCNT | 0x8000;
        wk->drawOffset = 0xfffffff0;
        if (wk->subLcdTouchOK != 0) {
            wk->subprocessSeq = 2;
            func_ov135_021f4ec0(wk, 3, 0x10);
            wk->unkD4 = 0;
        }
        func_0204b3dc(wk->promptDsAct, 0);
    }
    return 2;
}

// pokebw2's Search_SubSeqReturnScreen2
int func_ov135_021ff174(WorldTradeWork *wk) {
    wk->drawOffset--;
    if (GFL_WipeIsFinished()) {
        wk->drawOffset = 0;
        wk->subprocessSeq = SEARCH_SEQ_MAIN;
    }
    return WT_SEQ_MAIN;
}

int func_ov135_021ff198(WorldTradeWork *wk) {
    int touch;

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        GFL_WipeSet(0, 0, 0, 0, 16, 1, HEAPID_WORLDTRADE);
    } else {
        touch = func_ov135_021ffb00(wk->searchResult);
        if (wk->subLcdTouchOK != 0 && touch >= 0) {
            func_0204b878(wk->subAct[touch + 1], touch * 4 + 16);
            wk->subprocessSeq = 2;
            func_ov135_021f4ec0(wk, 3, 0);
            wk->unkD4 = touch;
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        }
    }
    return 2;
}


int func_ov135_021ff218(WorldTradeWork *wk) {
    if (func_0204b98c(wk->cursorAct) == FALSE) {
        func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][2]);
        if (func_020355c4() == 1) {
            func_0204b3dc(wk->cursorAct, FALSE);
        } else {
            func_0204b3dc(wk->cursorAct, TRUE);
        }
        func_ov135_021fe264(wk, func_ov135_021fe9b0(wk));
    }
    return 2;
}

int func_ov135_021ff26c(WorldTradeWork *wk) {
    if (func_0204b98c(wk->cursorAct) == FALSE) {
        func_0204b878(wk->cursorAct, data_ov135_02203d46[func_ov135_021fe9b0(wk)][2]);
        if (func_020355c4() == 1) {
            func_0204b3dc(wk->cursorAct, FALSE);
        } else {
            func_0204b3dc(wk->cursorAct, TRUE);
        }
        func_ov135_021ff318(wk, 0xf, 1, 0, 0xf0f);
        func_ov135_021f4e98(wk, 0x17, 0x1a);
    }
    return 2;
}



int func_ov135_021ff2d0(WorldTradeWork *wk) {
    if (!func_0204b98c(wk->cursorAct)) {
        int pos = func_ov135_021fe9b0(wk);

        func_0204b878(wk->cursorAct, data_ov135_02203d46[pos][2]);
        if (func_020355c4() == TRUE) {
            func_0204b3dc(wk->cursorAct, FALSE);
        } else {
            func_0204b3dc(wk->cursorAct, TRUE);
        }
        wk->subprocessSeq = SEARCH_SEQ_MAIN;
    }
    return 2;
}


void func_ov135_021ff318(WorldTradeWork *wk, int msgNo, int wait, int a, int b) {
    u8 bg;
    GFLBitmap *bitmap;
    BmpWin *window;

    GFL_MsgDataLoadStrbuf(wk->msgManager, msgNo, wk->talkString);
    bitmap = BmpWin_GetBitmap(wk->msgWin);
    GFL_BitmapFill(bitmap, 0xf);
    BmpWin_DrawFrame(wk->msgWin, 0, 1, 0xe);
    func_ov135_02202858(wk->msgWin, 0, wk->talkString, 0, 0, &wk->print);
    window = wk->msgWin;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    bg = BmpWin_GetBGIndex(window);
    GFL_BGSysLoadScr(bg);
}

void func_ov135_021ff384(BmpWin **win, BmpWin **countryWin, MsgData *msgManager, WorldTradePrint *print) {
    StrBuf *pSVar1;

    pSVar1 = GFL_MsgDataLoadStrbufNew(msgManager, 0x43);
    func_ov135_021f91a0((BmpWin *)*win, pSVar1, 0, 0, 0, 0x440, print);
    GFL_StrBufFree(pSVar1);
    pSVar1 = GFL_MsgDataLoadStrbufNew(msgManager, 0x45);
    func_ov135_021f91a0((BmpWin *)win[2], pSVar1, 0, 0, 0, 0x440, print);
    GFL_StrBufFree(pSVar1);
    pSVar1 = GFL_MsgDataLoadStrbufNew(msgManager, 0x47);
    func_ov135_021f91a0((BmpWin *)win[4], pSVar1, 0, 0, 0, 0x440, print);
    GFL_StrBufFree(pSVar1);
    pSVar1 = GFL_MsgDataLoadStrbufNew(msgManager, 0xb9);
    func_ov135_021f91a0((BmpWin *)*countryWin, pSVar1, 0, 0, 0, 0x440, print);
    GFL_StrBufFree(pSVar1);
    pSVar1 = GFL_MsgDataLoadStrbufNew(msgManager, 0x49);
    func_ov135_021f91dc((BmpWin *)win[6], pSVar1, 0, 0, 0, 0x440, print);
    GFL_StrBufFree(pSVar1);
    pSVar1 = GFL_MsgDataLoadStrbufNew(msgManager, 0xfc);
    func_ov135_021f91dc((BmpWin *)win[7], pSVar1, 0, 0, 0, 0x440, print);
    GFL_StrBufFree(pSVar1);
}

void func_ov135_021ff474(BmpWin *win, MsgData *msgManager, int flag, WorldTradePrint *print) {
    StrBuf *str;
    u8 palette;
    u16 color;
    color = 0x3c40;
    if (flag != 0) {
        color = 0x440;
        palette = 0;
    } else {
        palette = 2;
    }
    GFL_BGSysSetScrPaletteNo(1, 0x11, 2, 0xf, 4, palette);
    GFL_BGSysLoadScr(1);
    str = GFL_MsgDataLoadStrbufNew(msgManager, 0x4b);
    func_ov135_021f91dc(win, str, 0, 0, 0, color, print);
    GFL_StrBufFree(str);
}

// Whether a search is the same as the last one
BOOL func_ov135_021ff4d0(const Dpw_Tr_PokemonSearchData *s1, const Dpw_Tr_PokemonSearchData *s2,
                                    int countryCode1, int countryCode2) {
    if (s1->characterNo == s2->characterNo && s1->gender == s2->gender && s1->level_min == s2->level_min &&
        s1->level_max == s2->level_max && countryCode1 == countryCode2) {
        return TRUE;
    }
    return FALSE;
}

void func_ov135_021ff50c(WorldTradeWork *wk) {
    GFL_BGSysMoveBG(0, 3, wk->drawOffset);
    GFL_BGSysMoveBG(1, 3, wk->drawOffset);
    GFL_BGSysMoveBG(2, 3, wk->drawOffset);
    GFL_BGSysMoveBG(3, 3, wk->drawOffset);
    GFL_BGSysMoveBG(4, 3, -wk->drawOffset);
    GFL_BGSysMoveBG(5, 3, -wk->drawOffset);
    GFL_BGSysMoveBG(6, 3, -wk->drawOffset);
    GFL_BGSysMoveBG(7, 3, -wk->drawOffset);
}

// Darkens the main screen behind the input
// pokebw2's Search_BgBlendSet
void func_ov135_021ff570(int rate) {
    if (rate) {
        G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3, rate);
    } else {
        G2_BlendNone();
    }
}
