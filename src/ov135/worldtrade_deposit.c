// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov214/worldtrade_deposit.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "constants/version.h"
#include "dpw/dpw_tr.h"
#include "field/unity_tower.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "save/player_info.h"
#include "save/pokedex.h"
#include "system/bmp_menulist.h"
#include "system/bmp_winframe.h"
#include "system/str_tool.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "worldtrade_local.h"
#include "system/bmp_menuwork.h"

// The Global Trade Station's deposit screen, where the player says which Pokémon they want for the one they deposit,
// and the printing of Pokémon names, genders, levels and countries that the other screens share. The names are ours,
// guessed

// The sub-process modes the screen leaves with
#define DEPOSIT_MODE_TO_BOX 5
#define DEPOSIT_MODE_UPLOAD 7

// The number of species and one, the size of the regional Pokédex's table
#define MONSNO_TABLE_SIZE 650

// The initials of the Pokémon names
#define HEADWORD_NUM 10

#define LEVEL_SELECT_NUM 12

enum {
    DEPOSIT_SEQ_START,
    DEPOSIT_SEQ_MAIN,
    DEPOSIT_SEQ_END,
    DEPOSIT_SEQ_HEADWORD_SELECT_LIST,
    DEPOSIT_SEQ_HEADWORD_SELECT_WAIT,
    DEPOSIT_SEQ_POKENAME_SELECT_LIST,
    DEPOSIT_SEQ_POKENAME_SELECT_WAIT,
    DEPOSIT_SEQ_SEX_SELECT_MES,
    DEPOSIT_SEQ_SEX_SELECT_LIST,
    DEPOSIT_SEQ_SEX_SELECT_WAIT,
    DEPOSIT_SEQ_LEVEL_SELECT_MES,
    DEPOSIT_SEQ_LEVEL_SELECT_LIST,
    DEPOSIT_SEQ_LEVEL_SELECT_WAIT,
    DEPOSIT_SEQ_DEPOSITOK_MESSAGE,
    DEPOSIT_SEQ_DEPOSIT_YESNO,
    DEPOSIT_SEQ_DEPOSIT_YESNO_WAIT,
    DEPOSIT_SEQ_MES_WAIT,
};

// A level condition and its text
typedef struct {
    u32 msg;
    s16 min;
    s16 max;
} WorldTradeLevelTerm;

// Where the countries of each initial start in the country list
// Black 2's, not checked against Black
typedef struct {
    u8 start;
    u8 head;
} WorldTradeNationHead;

// An option of a selection list and its value
// Black 2's, not checked against Black
typedef struct {
    u32 msg;
    s32 value;
} WorldTradeSelectItem;

void func_ov135_021f7538(WorldTradeWork *wk, int msgNo, int wait, int flag, u16 dat);
void func_ov135_021f75c0(void);
void func_ov135_021f76bc(void);
void func_ov135_021f76d8(WorldTradeWork *wk);
void func_ov135_021f777c(WorldTradeWork *wk);
void func_ov135_021f7840(WorldTradeWork *wk);
void func_ov135_021f7878(WorldTradeWork *wk);
void func_ov135_021f7964(WorldTradeWork *wk);
void func_ov135_021f798c(WorldTradeWork *wk);
void func_ov135_021f79ec(WorldTradeWork *wk);
static int func_ov135_021f7a18(WorldTradeWork *wk);
static int func_ov135_021f7a4c(WorldTradeWork *wk);
int func_ov135_021f7a70(WorldTradeWork *wk);
int func_ov135_021f7a8c(WorldTradeWork *wk);
static int func_ov135_021f7b50(WorldTradeWork *wk);
int func_ov135_021f7bc8(WorldTradeWork *wk);
static int func_ov135_021f7cb4(WorldTradeWork *wk);
static int func_ov135_021f7cdc(WorldTradeWork *wk);
int func_ov135_021f7d00(WorldTradeWork *wk);
static int func_ov135_021f7d80(WorldTradeWork *wk);
int func_ov135_021f7da8(WorldTradeWork *wk);
int func_ov135_021f7dd0(WorldTradeWork *wk);
static int func_ov135_021f7e74(WorldTradeWork *wk);
static int func_ov135_021f7e9c(WorldTradeWork *wk);
int func_ov135_021f7ec0(WorldTradeWork *wk);
static int func_ov135_021f7f14(WorldTradeWork *wk);
int func_ov135_021f7f64(WorldTradeWork *wk);
u16 func_ov135_021f7f80(int sex);
u16 func_ov135_021f8078(int sex, u16 color);
void func_ov135_021f8668(Dpw_Tr_Data *dtd, WorldTradeWork *wk);
int func_ov135_021f86a8(ListMenuOption **menulist, MsgData *monsNameManager, MsgData *msgManager,
                                        u16 *table, u8 *sinou, int num, int select, PokeDexSave *zukan);
u32 func_ov135_021f8950(BmpMenuList *list, u16 *posBackup);

static int (*data_ov135_02203bbc[])(WorldTradeWork *wk) = {
    func_ov135_021f7a18,
    func_ov135_021f7a4c,
    func_ov135_021f7f14,
    func_ov135_021f7a70,
    func_ov135_021f7a8c,
    func_ov135_021f7b50,
    func_ov135_021f7bc8,
    func_ov135_021f7cb4,
    func_ov135_021f7cdc,
    func_ov135_021f7d00,
    func_ov135_021f7d80,
    func_ov135_021f7da8,
    func_ov135_021f7dd0,
    func_ov135_021f7e74,
    func_ov135_021f7e9c,
    func_ov135_021f7ec0,
    func_ov135_021f7f64,
};

static u16 data_ov135_02203b84[HEADWORD_NUM + 1] = {
    0, 112, 184, 253, 315, 392, 463, 595, 637, 649,
};

// The countries in the order of their names
static const u16 data_ov135_022030e8[] ATTRIBUTE_ALIGN(4) = {
#include "asset/worldtrade_deposit_data_ov135_022030e8.inc"
};

// The number of countries in the list, which worldtrade_search.c reads (pokebw2's WorldTrade_CountryListNum)
const u32 data_ov135_02202eac = NELEMS(data_ov135_022030e8);

// The texts of the genders, by a search's gender
const u32 data_ov135_02202eb0[] = { 0x82, 0x83, 0x84, 0x82 };

// Where the windows of the wanted and the deposited Pokémon are
static const u16 data_ov135_02202ec0[6][2] = {
#include "asset/worldtrade_deposit_data_ov135_02202ec0.inc"
};

static const BmpMenuListHeader data_ov135_02202ed8 = { NULL, NULL, NULL, NULL, 10, 6, 4, 8, 0, 2, 1, 15, 2, 0, 0, 1, 0, 0, NULL, 0, 0, NULL, NULL, NULL, NULL };

static const WorldTradeNationHead data_ov135_02202f0c[28] = {
#include "asset/worldtrade_deposit_data_ov135_02202f0c.inc"
};

void func_ov135_02202940(WorldTradePrint *print);

// pokebw2's WorldTrade_Deposit_Main

int func_ov135_021f73dc(WorldTradeWork *wk, int seq) {
    WorldTradeInputHeader header;

    func_ov135_021f798c(wk);
    func_ov135_021f75c0();
    func_ov135_021fffe8(wk, 0, 0);
    func_ov135_021f76d8(wk);
    func_ov135_021f777c(wk);
    func_ov135_021f7878(wk);
    GFL_WipeSet(3, 1, 1, 0, 6, 1, HEAPID_WORLDTRADE);
    func_ov135_021f9254(wk);
    func_ov135_021f81c8(wk->msgManager, wk->monsNameManager, wk->wordSet, &wk->infoWin[0], 0, 3, -1, &wk->print);
    func_ov135_021f8358(wk->msgManager, wk->wordSet, &wk->infoWin[3], wk->pokePtr, &wk->post, &wk->print);

    header.menuWin = wk->menuWin;
    header.backWin = &wk->backWin;
    header.cursorAct = wk->subCursorAct;
    header.arrowAct[0] = wk->boxArrowAct[0];
    header.arrowAct[1] = wk->boxArrowAct[1];
    header.searchCursorAct = NULL;
    header.msgManager = wk->msgManager;
    header.monsNameManager = wk->monsNameManager;
    header.countryNameManager = wk->countryNameManager;
    header.zukan = wk->param->pokedex;
    header.sinouTable = wk->depositWork->sinouTable;
    header.config = wk->param->config;
    wk->inputWork = func_ov135_021f9890(&header, 2, INPUT_SITUATION_DEPOSIT);

    wk->subprocessSeq = DEPOSIT_SEQ_START;
    wk->unk12E4 = 0;
    return WT_SEQ_FADEIN;
}

int func_ov135_021f74e4(WorldTradeWork *wk, int seq) {
    return data_ov135_02203bbc[wk->subprocessSeq](wk);
}

// pokebw2's WorldTrade_Deposit_End
int func_ov135_021f74f8(WorldTradeWork *wk, int seq) {
    func_ov135_021f7964(wk);
    func_ov135_021f99d0(wk->inputWork);
    func_ov135_021f79ec(wk);
    func_ov135_021f7840(wk);
    func_ov135_021f76bc();
    func_ov135_022000e4(wk);
    func_0204b3dc(wk->promptDsAct, FALSE);
    func_ov135_021f4ec8(wk);
    return 0;
}

void func_ov135_021f7538(WorldTradeWork *wk, int msgNo, int wait, int flag, u16 dat) {
    BmpWin *win;
    StrBuf *src = GFL_MsgDataLoadStrbufNew(wk->msgManager, msgNo);

    GFL_WordSetFormatStrbuf(wk->wordSet, wk->talkString, src);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->msgWin), 0xf);
    win = wk->msgWin;
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(win));
    BmpWin_DrawFrame(wk->msgWin, 0, 1, 0xe);
    func_ov135_02202858(wk->msgWin, 0, wk->talkString, 0, 0, &wk->print);
    GFL_StrBufFree(src);
}


// pokebw2's Deposit_BgInit
void func_ov135_021f75c0(void) {
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
            2,
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
            GX_BG_CHARBASE_0x04000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(2, &setup, BGMODE_TEXT);
        GFL_BGSysFillScrArea(2, 0, 0, 0, 32, 24, 0);
        GFL_BGSysLoadScr(2);
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
            GX_BG_SCRBASE_0xe000,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(3, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(3);
        GFL_BGSysSetBGEnabled(3, TRUE);
    }
    GFL_BGSysClearCharCore(0, 32, 0, HEAPID_WORLDTRADE);
    GFL_BGSysClearCharCore(3, 32, 0, HEAPID_WORLDTRADE);
}

// pokebw2's Deposit_BgExit
void func_ov135_021f76bc(void) {
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(0);
    GFL_BGSysReleaseBG(3);
}

// pokebw2's Deposit_BgGraphicSet
void func_ov135_021f76d8(WorldTradeWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_WORLDTRADE, HEAPID_WORLDTRADE);

    GFL_BGSysLoadNCLRDefault(ARCID_WORLDTRADE, 0, 0, 0, 0x40, HEAPID_WORLDTRADE);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x1a0, 0x20, HEAPID_WORLDTRADE);
    LoadSysMsgBox(0, 1, 14, 0, HEAPID_WORLDTRADE);
    LoadSysMsgBox(0, 31, 11, 0, HEAPID_WORLDTRADE);
    GFL_BGSysLoadNCGRStatic(ARCID_WORLDTRADE, 9, 1, 0, 0, TRUE, HEAPID_WORLDTRADE);
    loadBGScrToVramByNarcNoReserveNegAlign(ARCID_WORLDTRADE, 20, 1, 0, 0x600, TRUE, HEAPID_WORLDTRADE);
    GFL_BGSysLoadArcNCGRStatic(arc, 12, 2, 0, 0, TRUE, HEAPID_WORLDTRADE);
    func_ov135_02200b34(wk);
    GFL_ArcToolFree(arc);
}

void func_ov135_021f777c(WorldTradeWork *wk) {
    BmpWin *win;
    int i;

    wk->msgWin = BmpWin_CreateDynamic(0, 2, 21, 27, 2, 13, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->msgWin), 0);
    win = wk->msgWin;
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(win));

    for (i = 0; i < 6; i++) {
        wk->infoWin[i] = BmpWin_CreateDynamic(3, data_ov135_02202ec0[i][0], data_ov135_02202ec0[i][1], 12, 2, 13, TRUE);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->infoWin[i]), 0);
        win = wk->infoWin[i];
        BmpWin_FlushChar(win);
        BmpWin_FlushMap(win);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(win));
    }
    func_ov135_02200bac(wk, 3);
}


// pokebw2's Deposit_BmpWinDelete
void func_ov135_021f7840(WorldTradeWork *wk) {
    int i;

    func_ov135_02202940(&wk->print);
    BmpWin_Free(wk->explainWin);
    for (i = 0; i < 6; i++) {
        BmpWin_Free(wk->infoWin[i]);
    }
    BmpWin_Free(wk->msgWin);
}

void func_ov135_021f7878(WorldTradeWork *wk) {
    ClActor *pCVar1;
    ClActorSetup CStack_1c;

    MI_CpuFill8(&CStack_1c, 0, 8);
    CStack_1c.x = 0xa0;
    CStack_1c.y = 0x20;
    pCVar1 = func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_MAIN2][WT_CLACT_RES_CHAR],
            wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_PLTT], wk->clactRes[WT_CLACT_RES_MAIN2][WT_CLACT_RES_CELL], &CStack_1c, 0, 0x48);
    wk->subCursorAct = pCVar1;
    func_0204b878(pCVar1, 0xc);
    func_0204b92c(wk->subCursorAct, 1);
    func_0204b974(wk->subCursorAct);
    func_0204b3dc(wk->subCursorAct, 0);
    CStack_1c.x = 0xe4;
    CStack_1c.y = 0x75;
    pCVar1 = func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CHAR],
            wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_PLTT], wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CELL], &CStack_1c, 0, 0x48);
    wk->boxArrowAct[0] = pCVar1;
    func_0204b878(pCVar1, 0x26);
    func_0204b3dc(wk->boxArrowAct[0], 0);
    CStack_1c.x = 0x8c;
    pCVar1 = func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CHAR],
            wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_PLTT], wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CELL], &CStack_1c, 0, 0x48);
    wk->boxArrowAct[1] = pCVar1;
    func_0204b878(pCVar1, 0x27);
    func_0204b3dc(wk->boxArrowAct[1], 0);
}

// pokebw2's Deposit_DelCellActor
// pokebw2's Deposit_DelCellActor
// pokebw2's Deposit_DelCellActor
void func_ov135_021f7964(WorldTradeWork *wk) {
    func_0204b3b4(wk->subCursorAct);
    func_0204b3b4(wk->boxArrowAct[0]);
    func_0204b3b4(wk->boxArrowAct[1]);
}

void func_ov135_021f798c(WorldTradeWork *wk) {
    StrBuf *pSVar1;
    void *dest;

    pSVar1 = GFL_StrBufCreate(0xb4, 0x48);
    wk->talkString = pSVar1;
    dest = GFL_HeapAllocate(0x48,0x30,0,"worldtrade_deposit.c",0x2b4);
    wk->depositWork = dest;
    MIi_CpuClearFast(0, dest, 0x30);
    wk->depositWork->nameSortTable = func_ov135_021f8498(0x48, 0, &wk->depositWork->nameSortNum);
    wk->depositWork->sinouTable = func_ov135_021f84ec(0x48);
    func_ov135_021f8990(&wk->selectListPos);
}

// pokebw2's Deposit_FreeWork
void func_ov135_021f79ec(WorldTradeWork *wk) {
    GFL_HeapFree(wk->depositWork->sinouTable);
    GFL_HeapFree(wk->depositWork->nameSortTable);
    GFL_HeapFree(wk->depositWork);
    GFL_StrBufFree(wk->talkString);
}


// pokebw2's Deposit_SubSeqStart
static int func_ov135_021f7a18(WorldTradeWork *wk) {
    if (GFL_WipeIsFinished()) {
        func_ov135_021f7538(wk, 9, 1, 0, 0xf0f);
        func_ov135_021f4e98(wk, DEPOSIT_SEQ_MES_WAIT, DEPOSIT_SEQ_MAIN);
    }
    return WT_SEQ_MAIN;
}

// pokebw2's Deposit_SubSeqMain
static int func_ov135_021f7a4c(WorldTradeWork *wk) {
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        func_ov135_021f4ec0(wk, WORLDTRADE_MYBOX, DEPOSIT_MODE_TO_BOX);
        wk->subprocessSeq = DEPOSIT_SEQ_END;
    }
    // Overrides the end, so B only changes the screen that comes after the input
    wk->subprocessSeq = DEPOSIT_SEQ_HEADWORD_SELECT_LIST;
    return WT_SEQ_MAIN;
}

// pokebw2's Deposit_SubSeqHeadwordSelectList
int func_ov135_021f7a70(WorldTradeWork *wk) {
    func_ov135_021f9968(wk->inputWork, INPUT_MODE_POKEMON_NAME);
    wk->subprocessSeq = DEPOSIT_SEQ_POKENAME_SELECT_WAIT;
    return WT_SEQ_MAIN;
}

int func_ov135_021f7a8c(WorldTradeWork *wk) {
    switch (func_ov135_021f8950(wk->bmpListWork, &wk->listPos)) {
    case 0:
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        BmpMenuList_Free(wk->bmpListWork, &wk->depositWork->headwordListPos, &wk->depositWork->headwordPos);
        ListMenuCore_FreeOptionList(wk->menuList);
        wk->subprocessSeq = DEPOSIT_SEQ_POKENAME_SELECT_LIST;
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        break;
    case BMPMENULIST_CANCEL:
        BmpMenuList_Free(wk->bmpListWork, &wk->depositWork->headwordListPos, &wk->depositWork->headwordPos);
        ListMenuCore_FreeOptionList(wk->menuList);
        func_ov135_02202644(wk->menuWin[0], 0);
        func_0201f7a8(wk->msgWin, 0);
        BmpWin_Free(wk->menuWin[0]);
        BmpWin_Free(wk->menuWin[1]);
        func_ov135_021f4ec0(wk, WORLDTRADE_MYBOX, DEPOSIT_MODE_TO_BOX);
        wk->subprocessSeq = DEPOSIT_SEQ_END;
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        break;
    }
    return WT_SEQ_MAIN;
}


// pokebw2's Deposit_SubSeqPokeNameSelectList
// pokebw2's Deposit_SubSeqPokeNameSelectList
static int func_ov135_021f7b50(WorldTradeWork *wk) {
    wk->bmpListWork = func_ov135_021f875c(wk, &wk->menuList, wk->menuWin[1], wk->msgManager, wk->monsNameManager,
                                          wk->depositWork, wk->param->pokedex);
    wk->listPos = 0xffff;
    wk->subprocessSeq = DEPOSIT_SEQ_POKENAME_SELECT_WAIT;
    return WT_SEQ_MAIN;
}


// pokebw2's WorldTrade_SexSelectionCheck
BOOL func_ov135_021f7b9c(Dpw_Tr_PokemonSearchData *dtps, int sexSelection) {
    switch (sexSelection) {
    case 0:
        dtps->gender = 1;
        return TRUE;
    case 254:
        dtps->gender = 2;
        return TRUE;
    case 255:
        dtps->gender = 3;
        return TRUE;
    }
    return FALSE;
}

int func_ov135_021f7bc8(WorldTradeWork *wk) {
    u32 ret = func_ov135_021f99f0(wk->inputWork);
    int gender;
    void *personal;
    int sexSelection;

    switch (ret) {
    case BMPMENULIST_CANCEL:
        func_0201f7a8(wk->msgWin, 0);
        func_ov135_021f4ec0(wk, WORLDTRADE_MYBOX, DEPOSIT_MODE_TO_BOX);
        wk->subprocessSeq = DEPOSIT_SEQ_END;
        func_ov135_021f89a8(&wk->selectListPos, wk->depositWork->headwordListPos + wk->depositWork->headwordPos,
                            wk->depositWork->nameListPos, wk->depositWork->namePos);
        break;
    case BMPMENULIST_NULL:
        break;
    default:
        wk->want.characterNo = ret;
        personal = PML_PersonalLoad(ret, 0, HEAPID_WORLDTRADE);
        sexSelection = PML_PersonalGetParam(personal, 20);
        PML_PersonalFree(personal);
        wk->depositWork->sexSelection = sexSelection;
        if (func_ov135_021f7b9c(&wk->want, wk->depositWork->sexSelection)) {
            wk->subprocessSeq = DEPOSIT_SEQ_LEVEL_SELECT_MES;
            gender = wk->want.gender;
        } else {
            wk->subprocessSeq = DEPOSIT_SEQ_SEX_SELECT_MES;
            gender = 3;
        }
        func_ov135_021f81c8(wk->msgManager, wk->monsNameManager, wk->wordSet, &wk->infoWin[0], wk->want.characterNo,
                            gender, -1, &wk->print);
        func_ov135_021f89a8(&wk->selectListPos, wk->depositWork->headwordListPos + wk->depositWork->headwordPos,
                            wk->depositWork->nameListPos, wk->depositWork->namePos);
        break;
    }
    return WT_SEQ_MAIN;
}


// pokebw2's Deposit_SubSeqSexSelectMessage
static int func_ov135_021f7cb4(WorldTradeWork *wk) {
    func_ov135_021f7538(wk, 10, 1, 0, 0xf0f);
    func_ov135_021f4e98(wk, DEPOSIT_SEQ_MES_WAIT, DEPOSIT_SEQ_SEX_SELECT_LIST);
    return WT_SEQ_MAIN;
}

// pokebw2's Deposit_SubSeqSexSelectList
// pokebw2's Deposit_SubSeqSexSelectList
static int func_ov135_021f7cdc(WorldTradeWork *wk) {
    wk->listPos = 0xffff;
    func_ov135_021f9968(wk->inputWork, 1);
    wk->subprocessSeq = DEPOSIT_SEQ_SEX_SELECT_WAIT;
    return WT_SEQ_MAIN;
}

// pokebw2's Deposit_SubSeqSexSelectWait
int func_ov135_021f7d00(WorldTradeWork *wk) {
    u32 ret = func_ov135_021f99f0(wk->inputWork);

    switch (ret) {
    case BMPMENULIST_CANCEL:
        func_0201f7a8(wk->msgWin, 0);
        wk->subprocessSeq = DEPOSIT_SEQ_START;
        break;
    case 0:
    case 1:
    case 2:
        wk->want.gender = ret + 1;
        wk->subprocessSeq = DEPOSIT_SEQ_LEVEL_SELECT_MES;
        func_ov135_021f81c8(wk->msgManager, wk->monsNameManager, wk->wordSet, wk->infoWin, wk->want.characterNo,
                            wk->want.gender, -1, &wk->print);
        break;
    }
    return WT_SEQ_MAIN;
}


// pokebw2's Deposit_SubSeqLevelSelectMessage
static int func_ov135_021f7d80(WorldTradeWork *wk) {
    func_ov135_021f7538(wk, 11, 1, 0, 0xf0f);
    func_ov135_021f4e98(wk, DEPOSIT_SEQ_MES_WAIT, DEPOSIT_SEQ_LEVEL_SELECT_LIST);
    return WT_SEQ_MAIN;
}

// pokebw2's Deposit_SubSeqLevelSelectList
int func_ov135_021f7da8(WorldTradeWork *wk) {
    func_ov135_021f9968(wk->inputWork, 2);
    wk->listPos = 0xffff;
    wk->subprocessSeq = DEPOSIT_SEQ_LEVEL_SELECT_WAIT;
    return WT_SEQ_MAIN;
}

int func_ov135_021f7dd0(WorldTradeWork *wk) {
    u32 ret = func_ov135_021f99f0(wk->inputWork);

    switch (ret) {
    case LEVEL_SELECT_NUM:
    case BMPMENULIST_CANCEL:
        func_0201f7a8(wk->msgWin, 0);
        if (func_ov135_021f7b9c(&wk->want, wk->depositWork->sexSelection)) {
            wk->subprocessSeq = DEPOSIT_SEQ_START;
        } else {
            wk->subprocessSeq = DEPOSIT_SEQ_SEX_SELECT_MES;
        }
        break;
    case BMPMENULIST_NULL:
        break;
    default:
        func_ov135_021f882c(&wk->want, ret, 0);
        wk->subprocessSeq = DEPOSIT_SEQ_DEPOSITOK_MESSAGE;
        func_ov135_021f81c8(wk->msgManager, wk->monsNameManager, wk->wordSet, &wk->infoWin[0], wk->want.characterNo,
                            wk->want.gender, func_ov135_021f886c(wk->want.level_min, wk->want.level_max, 0),
                            &wk->print);
        break;
    }
    return WT_SEQ_MAIN;
}


// pokebw2's Deposit_SubSeqDepositOkMessage
static int func_ov135_021f7e74(WorldTradeWork *wk) {
    func_ov135_021f7538(wk, 23, 1, 0, 0xf0f);
    func_ov135_021f4e98(wk, DEPOSIT_SEQ_MES_WAIT, DEPOSIT_SEQ_DEPOSIT_YESNO);
    return WT_SEQ_MAIN;
}

// pokebw2's Deposit_SubSeqDepositOkYesNo
static int func_ov135_021f7e9c(WorldTradeWork *wk) {
    func_ov135_021f4be0(wk, 20, 0x126, 3, FALSE);
    wk->subprocessSeq = DEPOSIT_SEQ_DEPOSIT_YESNO_WAIT;
    return WT_SEQ_MAIN;
}

int func_ov135_021f7ec0(WorldTradeWork *wk) {
    u8 ret = func_ov135_021f4d14(wk);

    if (ret == 1) {
        func_ov135_021f4ce4(wk);
        func_ov135_021f4ec0(wk, WORLDTRADE_UPLOAD, DEPOSIT_MODE_UPLOAD);
        wk->subprocessSeq = DEPOSIT_SEQ_END;
        wk->subOutFlag = 1;
        func_ov135_021f8668(&wk->uploadData, wk);
    } else if (ret == 2) {
        func_ov135_021f4ce4(wk);
        func_ov135_021f4ec0(wk, WORLDTRADE_MYBOX, DEPOSIT_MODE_TO_BOX);
        wk->subprocessSeq = DEPOSIT_SEQ_END;
    }
    return WT_SEQ_MAIN;
}


// pokebw2's Deposit_SubSeqEnd
static int func_ov135_021f7f14(WorldTradeWork *wk) {
    if (wk->subNextProcess == WORLDTRADE_ENTER) {
        GFL_WipeSet(0, 0, 0, 0, 6, 1, HEAPID_WORLDTRADE);
        wk->subOutFlag = 1;
    } else {
        GFL_WipeSet(3, 0, 0, 0, 6, 1, HEAPID_WORLDTRADE);
    }
    wk->subprocessSeq = DEPOSIT_SEQ_START;
    return WT_SEQ_FADEOUT;
}

int func_ov135_021f7f64(WorldTradeWork *wk) {
    BOOL BVar1;

    BVar1 = func_ov135_02202840(&wk->print);
    if (BVar1 == 0) {
        wk->subprocessSeq = wk->subprocessNextSeq;
    }
    return 2;
}


// pokebw2's Deposit_GetSexBaseColor
u16 func_ov135_021f7f80(int sex) {
    switch (sex) {
    case 0:
        return 0x14c0;
    case 1:
        return 0xc80;
    }
    return 0;
}

// pokebw2's WorldTrade_PokeNamePrint
void func_ov135_021f7f9c(BmpWin *win, MsgData *msgManager, int msgNo, int flag, int y, u16 color, WorldTradePrint *print) {
    StrBuf *str;

    if (msgNo != 0) {
        str = GFL_MsgDataLoadStrbufNew(msgManager, msgNo);
        func_ov135_021f91a0(win, str, 0, y, flag, color, print);
        GFL_StrBufFree(str);
    }
}

// pokebw2's WorldTrade_PokeNamePrintNoPut
void func_ov135_021f7fd4(BmpWin *win, MsgData *nameManager, int monsno, int y, u16 color, WorldTradePrint *print) {
    StrBuf *str;

    if (monsno != 0) {
        str = GFL_MsgDataLoadStrbufNew(nameManager, monsno);
        func_ov135_022028c8(win, 0, str, 0, y, 0, color, print);
        GFL_StrBufFree(str);
    }
}

// pokebw2's WorldTrade_CountryPrint
void func_ov135_021f8010(BmpWin *win, MsgData *mgr, MsgData *mgr2, int a, int b, int c, u16 d, WorldTradePrint *print) {
    StrBuf *str;

    if (a != 0) {
        str = GFL_MsgDataLoadStrbufNew(mgr, a);
        func_ov135_021f91a0(win, str, 0, c, b, d, print);
        GFL_StrBufFree(str);
        return;
    }
    str = GFL_MsgDataLoadStrbufNew(mgr2, 0xbb);
    func_ov135_021f91a0(win, str, 0, c, b, d, print);
    GFL_StrBufFree(str);
}


// pokebw2's Deposit_GetSexColor
u16 func_ov135_021f8078(int sex, u16 color) {
    if (sex == 1) {
        return func_ov135_021f7f80(0);
    }
    if (sex == 2) {
        color = func_ov135_021f7f80(1);
    }
    return color;
}

// pokebw2's WorldTrade_SexPrint
void func_ov135_021f8098(BmpWin *win, MsgData *mgr, int a, int b, int c, int d, u16 e, WorldTradePrint *print) {
    StrBuf *str;

    if (b != 0 || a != SEARCH_GENDER_ANY) {
        str = GFL_MsgDataLoadStrbufNew(mgr, data_ov135_02202eb0[a]);
        if (d > 3) {
            u16 color = func_ov135_021f8078(a, e);
            func_ov135_021f91a0(win, str, d, c, 0, color, print);
        } else {
            u16 color = func_ov135_021f8078(a, e);
            func_ov135_021f91a0(win, str, 0, c, d, color, print);
        }
        GFL_StrBufFree(str);
    }
}


// pokebw2's WorldTrade_SexPrintNoPut
// pokebw2's WorldTrade_SexPrintNoPut
void func_ov135_021f8104(BmpWin *win, MsgData *msgManager, int sex, int flag, int x, int y, u16 color,
                         WorldTradePrint *print) {
    StrBuf *str;

    if (flag != 0 || sex != SEARCH_GENDER_ANY) {
        str = GFL_MsgDataLoadStrbufNew(msgManager, data_ov135_02202eb0[sex]);
        func_ov135_022028c8(win, 0, str, x, y, 0, func_ov135_021f8078(sex, color), print);
        GFL_StrBufFree(str);
    }
}


// Not referred to (pokebw2's WorldTrade_SexSelectTable)
WorldTradeSelectItem data_ov135_02203b9c[] = {
    { 0x82, 2 },
    { 0x83, 0 },
    { 0x84, 1 },
    { 0x85, BMPMENULIST_CANCEL },
};

static const WorldTradeLevelTerm data_ov135_02202fc4[SEARCH_LEVEL_SELECT_NUM] = {
#include "asset/worldtrade_deposit_data_ov135_02202fc4.inc"
};

static const WorldTradeLevelTerm data_ov135_0220301c[LEVEL_SELECT_NUM] = {
#include "asset/worldtrade_deposit_data_ov135_0220301c.inc"
};

// pokebw2's WorldTrade_WantLevelPrint
// pokebw2's WorldTrade_WantLevelPrint
void func_ov135_021f8154(BmpWin *win, MsgData *msgManager, int level, int flag, int y, u16 color, int tblSelect,
                         WorldTradePrint *print) {
    func_ov135_021f8178(win, msgManager, level, flag, 0, y, color, tblSelect, print);
}


void func_ov135_021f8178(BmpWin *win, MsgData *msgManager, int level, int flag, int x, int y, u16 color, int tblSelect, WorldTradePrint *print) {
    StrBuf *str;
    u8 *ptr;

    if (level != -1) {
        ptr = (void *)&data_ov135_0220301c;
        if (tblSelect != 0) {
            ptr = (void *)&data_ov135_02202fc4;
        }
        str = GFL_MsgDataLoadStrbufNew(msgManager, *(u32 *)(ptr + level * 8));
        func_ov135_021f91a0(win, str, x, y, flag, color, print);
        GFL_StrBufFree(str);
    }
}

void func_ov135_021f81c8(MsgData *msgManager, MsgData *monsNameManager, WordSet *wordSet, BmpWin **win, int monsno,
                         int sex, int level, WorldTradePrint *print) {
    StrBuf *str;
    BOOL printSex = TRUE;
    int i;

    str = GFL_MsgDataLoadStrbufNew(msgManager, 0x6b);

    func_ov135_021f91a0(win[0], str, 1, 0, 0, 0x440, print);
    for (i = 1; i < 3; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(win[i]), 0);
    }
    func_ov135_021f7f9c(win[1], monsNameManager, monsno, 0, 0, 0x440, print);
    if (monsno == SPECIES_NIDORAN_F && sex == 2) {
        printSex = FALSE;
    } else if (monsno == SPECIES_NIDORAN_M && sex == 1) {
        printSex = FALSE;
    }
    if (printSex && (sex == 1 || sex == 2)) {
        func_ov135_021f8098(win[1], msgManager, sex, 0, 0, 70, 0x440, print);
    }
    func_ov135_021f8154(win[2], msgManager, level, 2, 0, 0x440, 0, print);
    GFL_StrBufFree(str);
}

void func_ov135_021f8290(MsgData *msgManager, MsgData *monsNameManager, WordSet *wordSet, BmpWin **win, int monsno,
                         int sex, int level, WorldTradePrint *print) {
    StrBuf *str;
    BOOL printSex = TRUE;
    int i;

    str = GFL_MsgDataLoadStrbufNew(msgManager, 0x6b);

    func_ov135_021f91a0(win[0], str, 0, 0, 0, 0x440, print);
    for (i = 1; i < 3; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(win[i]), 0);
    }
    func_ov135_021f7f9c(win[1], monsNameManager, monsno, 0, 0, 0x440, print);
    if (monsno == SPECIES_NIDORAN_F && sex == 2) {
        printSex = FALSE;
    } else if (monsno == SPECIES_NIDORAN_M && sex == 1) {
        printSex = FALSE;
    }
    if (printSex && (sex == 1 || sex == 2)) {
        func_ov135_021f8098(win[1], msgManager, sex, 0, 0, 70, 0x440, print);
    }
    func_ov135_021f8154(win[2], msgManager, level, 0, 0, 0x440, 0, print);
    GFL_StrBufFree(str);
}

void func_ov135_021f8358(MsgData *msgManager, WordSet *wordSet, BmpWin **win, BoxPkm *pkm,
                         Dpw_Tr_PokemonDataSimple *post, WorldTradePrint *print) {
    StrBuf *title;
    StrBuf *levelStr;
    StrBuf *name = GFL_StrBufCreate(11, HEAPID_WORLDTRADE);
    StrBuf *sexStr = GFL_StrBufCreate(11, HEAPID_WORLDTRADE);
    u32 showSex;
    int level;
    int sex;
    int i;

    PML_PkmGetParam(pkm, PKM_PARAM_NICKNAME, name);
    sex = PML_PkmGetParam(pkm, PKM_PARAM_SEX, NULL) + 1;
    level = PML_PkmGetLevel(pkm);
    showSex = PML_PkmGetParam(pkm, 0xad, NULL);
    title = GFL_MsgDataLoadStrbufNew(msgManager, 0x6e);
    WordSetNumber(wordSet, 3, level, 3, 0, TRUE);
    levelStr = func_ov135_02202670(wordSet, msgManager, 0x72, HEAPID_WORLDTRADE);
    if (sex != 3) {
        GFL_MsgDataLoadStrbuf(msgManager, data_ov135_02202eb0[sex], sexStr);
    }
    for (i = 0; i < 3; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(win[i]), 0);
    }
    func_ov135_021f91a0(win[0], title, 1, 0, 0, 0x3c40, print);
    func_ov135_021f91a0(win[1], name, 0, 0, 0, 0x3c40, print);
    func_ov135_021f91a0(win[2], levelStr, 0, 0, 2, 0x3c40, print);
    if (sex != 3 && showSex) {
        func_ov135_021f91a0(win[1], sexStr, 70, 0, 0, func_ov135_021f7f80(sex - 1), print);
    }
    post->characterNo = PML_PkmGetParam(pkm, PKM_PARAM_SPECIES, NULL);
    post->gender = sex;
    post->level = level;
    GFL_StrBufFree(levelStr);
    GFL_StrBufFree(sexStr);
    GFL_StrBufFree(name);
    GFL_StrBufFree(title);
}


// pokebw2's WorldTrade_ZukanSortDataGet
u16 *func_ov135_021f8498(int heapId, int idx, int *num) {
    u32 size;
    u16 *data = GFL_ArcSysReadHeapNewLZGetLen(98, 5, FALSE, heapId, &size);

    *num = size / 2;
    return data;
}

// Where the species of each initial start and end in the sorted list. The name is the one the assert prints
// pokebw2's ZukanSortHiraTable
static const u32 data_ov135_0220307c[27] = {
#include "asset/worldtrade_deposit_data_ov135_0220307c.inc"
};

// pokebw2's WorldTrade_HeadwordRangeGet
void func_ov135_021f84bc(int select, int *start, int *end) {
    // clang-format off
    GFL_ASSERT(select < NELEMS(data_ov135_0220307c));
    // clang-format on
    *start = data_ov135_0220307c[select];
    *end = data_ov135_0220307c[select + 1];
}

// pokebw2's WorldTrade_SinouZukanDataGet
u8 *func_ov135_021f84ec(int heapId) {
    u32 i = 0;
    u8 *table = GFL_HeapAllocate(HEAPID_WORLDTRADE, MONSNO_TABLE_SIZE, FALSE, "worldtrade_deposit.c", 1942);
    u32 size;
    u16 *data;
    u32 num;

    MIi_CpuClearFast(0, table, MONSNO_TABLE_SIZE);
    data = GFL_ArcSysReadHeapNewLZGetLen(98, 4, FALSE, heapId, &size);
    num = size / 2;
    for (; i < num; i++) {
        if (data[i] < MONSNO_TABLE_SIZE) {
            table[data[i]] = 1;
        }
    }
    GFL_HeapFree(data);
    return table;
}

void func_ov135_021f8554(Dpw_Tr_Data *dtd, WorldTradeWork *wk) {
    u32 species;

    if (func_ov135_021f6b34(wk->boxTrayNo)) {
        species = PML_PkmGetParam(wk->pokePtr, PKM_PARAM_SPECIES, NULL);
        if (species == SPECIES_SHAYMIN || species == SPECIES_KYUREM) {
            PokeParty_ChangeForme((PartyPkm *)wk->pokePtr, 0);
        }
        MIi_CpuCopyFast(wk->pokePtr, dtd, PokeParty_GetPkmRawSize());
    } else {
        species = PML_PkmGetParam(wk->pokePtr, PKM_PARAM_SPECIES, NULL);
        if (species == SPECIES_SHAYMIN || species == SPECIES_KYUREM) {
            PML_PkmChangeForme(wk->pokePtr, 0);
        }
        func_ov135_022026a0(wk->pokePtr, (PartyPkm *)dtd);
    }

    wcharsncpy(GetPlayerName(wk->param->mystatus), dtd->name, 8);
    dtd->trainerID = getIDAsUInt(wk->param->mystatus);
    dtd->countryCode = UnityTowerVisitor_GetCountry(wk->param->mystatus);
    dtd->localCode = UnityTowerVisitor_GetProvince(wk->param->mystatus);
    dtd->trainerType = func_02008554(wk->param->mystatus);
    dtd->gender = getTrainerGender(wk->param->mystatus);
    dtd->versionCode = GAME_VERSION;
    dtd->langCode = 2;
    dtd->unk126 = getPlayerSurveys(wk->param->wifihistory);
    dtd->unk127 = func_02009584(wk->param->wifihistory);
    dtd->unkF7 = func_0200960c(wk->param->wifihistory);
}


// pokebw2's Deposit_DepositPokemonDataMake

void func_ov135_021f8668(Dpw_Tr_Data *dtd, WorldTradeWork *wk) {
    func_ov135_021f8554(dtd, wk);
    *(u16 *)&dtd->postSimple.characterNo = *(u16 *)&wk->post.characterNo;
    *(u16 *)&dtd->postSimple.gender = *(u16 *)&wk->post.gender;
    *(u16 *)&dtd->wantSimple.characterNo = *(u16 *)&wk->want.characterNo;
    *(u16 *)&dtd->wantSimple.gender = *(u16 *)&wk->want.gender;
    *(u16 *)&dtd->wantSimple.level_max = *(u16 *)&wk->want.level_max;
}


int func_ov135_021f86a8(ListMenuOption **menulist, MsgData *monsNameManager, MsgData *msgManager,
                                        u16 *table, u8 *sinou, int num, int select, PokeDexSave *zukan) {
    int count = 0;
    int start = data_ov135_02203b84[select];
    int max = data_ov135_02203b84[select + 1] - start;
    int i;

    for (i = 0; i < max; i++) {
        if (PokeDex_IsSeen(zukan, table[start + i])) {
            count++;
        }
    }
    *menulist = ListMenuCore_CreateOptionList(count + 1, HEAPID_WORLDTRADE);
    for (i = 0; i < max; i++) {
        if (PokeDex_IsSeen(zukan, table[start + i])) {
            ListMenuCore_AppendMsgOption(*menulist, monsNameManager, table[start + i], table[start + i],
                                         HEAPID_WORLDTRADE);
        }
    }
    ListMenuCore_AppendMsgOption(*menulist, msgManager, 0x85, BMPMENULIST_CANCEL, HEAPID_WORLDTRADE);
    return count + 1;
}

// pokebw2's WorldTrade_PokeNameListMake
// pokebw2's WorldTrade_PokeNameListMake
BmpMenuList *func_ov135_021f875c(WorldTradeWork *wk, ListMenuOption **menulist, BmpWin *win, MsgData *msgManager,
                                 MsgData *monsNameManager, WorldTradeDepositWork *dw, PokeDexSave *zukan) {
    BmpMenuListHeader header;
    int head;
    int num;

    GFL_BitmapFill(BmpWin_GetBitmap(win), 15);
    head = dw->headwordListPos + dw->headwordPos;
    num = func_ov135_021f86a8(menulist, monsNameManager, msgManager, dw->nameSortTable, dw->sinouTable,
                              dw->nameSortNum, head, zukan);
    header = data_ov135_02202ed8;
    header.count = num;
    header.options = *menulist;
    header.window = win;
    BmpWin_DrawFrame(win, 0, 31, 11);
    return BmpMenuList_Create(&header, wk->selectListPos.nameList[head], wk->selectListPos.namePos[head], HEAPID_WORLDTRADE);
}


// pokebw2's WorldTrade_LevelListAdd
int func_ov135_021f87e0(ListMenuOption **menulist, MsgData *msgManager, int tblSelect) {
    int i;
    int num;
    const WorldTradeLevelTerm *table;

    if (tblSelect == LEVEL_PRINT_TBL_DEPOSIT) {
        table = data_ov135_0220301c;
        num = LEVEL_SELECT_NUM;
    } else {
        table = data_ov135_02202fc4;
        num = SEARCH_LEVEL_SELECT_NUM;
    }
    *menulist = ListMenuCore_CreateOptionList(num, HEAPID_WORLDTRADE);
    for (i = 0; i < num; i++) {
        ListMenuCore_AppendMsgOption(*menulist, msgManager, table[i].msg, i, HEAPID_WORLDTRADE);
    }
    return num;
}

// pokebw2's WorldTrade_LevelMinMaxSet
void func_ov135_021f882c(Dpw_Tr_PokemonSearchData *dtps, int index, int tblSelect) {
    const WorldTradeLevelTerm *table;

    // clang-format off
    if (tblSelect == LEVEL_PRINT_TBL_DEPOSIT) {
        table = data_ov135_0220301c;
        GFL_ASSERT(index<(LEVEL_SELECT_NUM));
    } else {
        table = data_ov135_02202fc4;
        GFL_ASSERT(index<(SEARCH_LEVEL_SELECT_NUM));
    }
    // clang-format on
    dtps->level_min = table[index].min;
    dtps->level_max = table[index].max;
}

// pokebw2's WorldTrade_LevelTermGet
int func_ov135_021f886c(int min, int max, int tblSelect) {
    int i;
    int num;
    const WorldTradeLevelTerm *table;

    if (tblSelect == LEVEL_PRINT_TBL_DEPOSIT) {
        table = data_ov135_0220301c;
        num = LEVEL_SELECT_NUM;
    } else {
        table = data_ov135_02202fc4;
        num = SEARCH_LEVEL_SELECT_NUM;
    }
    for (i = 0; i < num; i++) {
        if (min == table[i].min && max == table[i].max) {
            return i;
        }
    }
    return 0;
}

// pokebw2's WorldTrade_CountryCodeSet
void func_ov135_021f88b0(WorldTradeWork *wk, int countryCode) {
    if (countryCode == 0) {
        wk->countryCode = 0;
    } else if (countryCode - 1 < NELEMS(data_ov135_022030e8)) {
        wk->countryCode = data_ov135_022030e8[countryCode - 1];
    }
}


// pokebw2's WorldTrade_NationSortListNumGet
int func_ov135_021f88d8(int start, int *number) {
    int i;

    for (i = 0; i < 28; i++) {
        if (start == data_ov135_02202f0c[i].head) {
            *number = data_ov135_02202f0c[i].start;
            return data_ov135_02202f0c[i + 1].start - data_ov135_02202f0c[i].start;
        }
    }
    return 0;
}

// pokebw2's WorldTrade_NationSortListMake
// pokebw2's WorldTrade_NationSortListMake
int func_ov135_021f8908(ListMenuOption **menulist, MsgData *countryNameManager, int start) {
    int first;
    int num = func_ov135_021f88d8(start, &first);
    int i;

    *menulist = ListMenuCore_CreateOptionList(num, HEAPID_WORLDTRADE);
    for (i = 0; i < num; i++) {
        ListMenuCore_AppendMsgOption(*menulist, countryNameManager, data_ov135_022030e8[first + i], first + i + 1,
                                     HEAPID_WORLDTRADE);
    }
    return num;
}

// pokebw2's Deposit_BmpListMain
u32 func_ov135_021f8950(BmpMenuList *list, u16 *posBackup) {
    u16 pos;
    u32 ret = (u32)BmpMenuList_Update(list);

    func_020202ec(list, &pos);
    if (*posBackup != pos) {
        if (*posBackup != 0xffff) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
        }
        *posBackup = pos;
    }
    return ret;
}

// pokebw2's WorldTrade_SelectListPosInit
void func_ov135_021f8990(WorldTradeSelectListPos *slp) {
    int i;

    for (i = 0; i < 10; i++) {
        slp->nameList[i] = 0;
        slp->namePos[i] = 0;
    }
    slp->headList = 0;
    slp->headPos = 0;
}

// pokebw2's WorldTrade_SelectNameListBackup
void func_ov135_021f89a8(WorldTradeSelectListPos *slp, int head, int list, int pos) {
    slp->nameList[head] = list;
    slp->namePos[head] = pos;
}
