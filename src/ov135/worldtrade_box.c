// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov214/worldtrade_box.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "dpw/dpw_tr.h"
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
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "pml/item.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "system/bmp_menulist.h"
#include "system/bmp_winframe.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "worldtrade_local.h"
#include "app/worldtrade.h"
#include "system/bmp_menuwork.h"


void func_ov135_021ffd74(WorldTradeWork *wk);

// The Global Trade Station's box screen, where the player picks a Pokémon of the party or the boxes to deposit or to
// offer in a trade. The names are ours, guessed

// The cursor's positions past the page's 30 Pokémon
#define BOX_CURSOR_END_POS 30
#define BOX_CURSOR_TRAY_POS 31

// The sub-process modes the screen is entered with
#define BOX_MODE_DEPOSIT_SELECT 5
#define BOX_MODE_EXCHANGE_SELECT 6

// The box page of the party
#define BOX_TRAY_PARTY 0xff

// A search's gender that takes either
#define SEARCH_GENDER_ANY 3

enum {
    BOX_SEQ_START,
    BOX_SEQ_MAIN,
    BOX_SEQ_END,
    BOX_SEQ_MESSAGE_WAIT,
    BOX_SEQ_MESSAGE_CLEAR_WAIT,
    BOX_SEQ_YESNO,
    BOX_SEQ_YESNO_SELECT,
    BOX_SEQ_SELECT_LIST,
    BOX_SEQ_SELECT_WAIT,
    BOX_SEQ_EXCHANGE_SELECT_LIST,
    BOX_SEQ_EXCHANGE_SELECT_WAIT,
    BOX_SEQ_CBALL_YESNO_MESSAGE,
    BOX_SEQ_CBALL_YESNO,
    BOX_SEQ_CBALL_YESNO_SELECT,
    BOX_SEQ_CBALL_DEPOSIT_YESNO_MESSAGE,
    BOX_SEQ_CBALL_DEPOSIT_YESNO,
    BOX_SEQ_CBALL_DEPOSIT_YESNO_SELECT,
    BOX_SEQ_CANCEL_WAIT,
};

// Black 2's, not checked against Black
typedef struct {
    u16 x;
    u16 y;
} BoxPos;

void func_ov135_021f5250(void);
static void func_ov135_021f5358(void);
void func_ov135_021f5374(WorldTradeWork *wk);
void func_ov135_021f5420(WorldTradeWork *wk);
static void func_ov135_021f577c(WorldTradeWork *wk);
void func_ov135_021f57ec(WorldTradeWork *wk);
static void func_ov135_021f5940(WorldTradeWork *wk);
void func_ov135_021f5980(WorldTradeWork *wk);
static void func_ov135_021f59e8(WorldTradeWork *wk);
int func_ov135_021f5a18(WorldTradeWork *wk);
void func_ov135_021f5a7c(WorldTradeWork *wk);
void func_ov135_021f5b8c(WorldTradeWork *wk);
int func_ov135_021f5ca4(WorldTradeWork *wk);
static void func_ov135_021f5cd4(WorldTradeWork *wk, int mode);
int func_ov135_021f5cfc(WorldTradeWork *wk);
void func_ov135_021f5e44(WorldTradeWork *wk);
void func_ov135_021f5f60(ClActor *cursor, int pos);
static int func_ov135_021f5f9c(int num, int max, int move);
static int func_ov135_021f5fc0(WorldTradeWork *wk);
int func_ov135_021f601c(WorldTradeWork *wk);
static int func_ov135_021f6178(WorldTradeWork *wk);
static int func_ov135_021f61d4(WorldTradeWork *wk);
static int func_ov135_021f632c(WorldTradeWork *wk);
static int func_ov135_021f6384(WorldTradeWork *wk);
static int func_ov135_021f63a8(WorldTradeWork *wk);
static int func_ov135_021f63ec(WorldTradeWork *wk);
static int func_ov135_021f6418(WorldTradeWork *wk);
int func_ov135_021f643c(WorldTradeWork *wk);
int func_ov135_021f6474(WorldTradeWork *wk);
static int func_ov135_021f651c(WorldTradeWork *wk);
static int func_ov135_021f6548(WorldTradeWork *wk);
int func_ov135_021f656c(WorldTradeWork *wk);
static int func_ov135_021f65c8(WorldTradeWork *wk);
static int func_ov135_021f65e8(WorldTradeWork *wk);
static int func_ov135_021f6604(WorldTradeWork *wk);
void func_ov135_021f663c(WorldTradeWork *wk, int msgNo, int wait, int flag, u16 dat, int winFlag);
static void *func_ov135_021f66c8(ArcTool *handle, u32 dataIdx, NNSG2dCharacterData **charData, u32 heapId);
void func_ov135_021f66f0(int species, int form, int sex, int egg, int no, ClActor *icon, ArcTool *handle, WorldTradePokeBuf *pbuf);
void func_ov135_021f6748(BoxPkm *pkm, Dpw_Tr_PokemonDataSimple *dat);
void func_ov135_021f6754(WorldTradeWork *wk);
void func_ov135_021f67a8(BoxPkm *pkm, ClActor *icon, ClActor *itemAct, u16 *no, int pos, ArcTool *handle, Dpw_Tr_PokemonDataSimple *dat, WorldTradePokeBuf *pbuf);
void func_ov135_021f68a4(WorldTradeWork *wk, int now);
static BOOL func_ov135_021f6ba4(PokeParty *party, BoxSaveAccessor *box, int tray, int pos);
u32 func_ov135_021f6bc4(BoxPkm *pkm);
BOOL func_ov135_021f6c28(BoxPkm *pkm);
static int func_ov135_021f6c48(PokeParty *party, BoxSaveAccessor *box, int tray, int pos);
BOOL func_ov135_021f6c80(Dpw_Tr_PokemonDataSimple *poke, Dpw_Tr_PokemonSearchData *search);
BOOL func_ov135_021f6cdc(BoxPkm *pkm, Dpw_Tr_PokemonSearchData *want);
void func_ov135_021f6d14(Dpw_Tr_Data *dtd, WorldTradeWork *wk);
void func_ov135_021f6db4(Dpw_Tr_PokemonDataSimple *box, ClActor **icons, Dpw_Tr_PokemonSearchData *want, WorldTradePokeBuf *pbuf);
static void func_ov135_021f6e08(WorldTradeWork *wk);

static int (*data_ov135_02203b0c[])(WorldTradeWork *wk) = {
    func_ov135_021f5a18,
    func_ov135_021f5cfc,
    func_ov135_021f632c,
    func_ov135_021f65e8,
    func_ov135_021f6604,
    func_ov135_021f6384,
    func_ov135_021f63a8,
    func_ov135_021f5fc0,
    func_ov135_021f601c,
    func_ov135_021f6178,
    func_ov135_021f61d4,
    func_ov135_021f63ec,
    func_ov135_021f6418,
    func_ov135_021f643c,
    func_ov135_021f651c,
    func_ov135_021f6548,
    func_ov135_021f656c,
    func_ov135_021f65c8,
};

// Where the cursor and the Pokémon icons sit, for each position of the cursor
// pokebw2's sCursorPos
static const BoxPos data_ov135_02202d24[] = {
#include "asset/worldtrade_box_data_ov135_02202d24.inc"
};

typedef struct WorldTradePrint WorldTradePrint;
void func_ov135_02202940(WorldTradePrint *print);
BOOL func_ov135_02202840(WorldTradePrint *print);

void func_ov135_022025e4(WordSet *wordSet, u32 index, BoxPkm *pkm);
u32 func_ov135_021f6bfc(BoxPkm *param_1);

// pokebw2's WorldTrade_Box_Main

#define HEAPID_WORLDTRADE 0x48
int func_ov135_021f5154(WorldTradeWork *wk, int seq) {
    func_ov135_021f5980(wk);
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
    func_ov135_021f5250();
    func_ov135_021fffe8(wk, 0, 0);
    func_ov135_021f5374(wk);
    func_ov135_021f57ec(wk);
    func_ov135_021f5420(wk);
    func_ov135_021ffd44(wk);

    if (GXx_GetMasterBrightness_(REG_DB_MASTER_BRIGHT_ADDR) == 0) {
        GFL_WipeSet(WIPE_MODE_MAIN, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, 0, 6, 1, HEAPID_WORLDTRADE);
    } else {
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, 0, 6, 1, HEAPID_WORLDTRADE);
    }

    func_ov135_021f68a4(wk, wk->boxTrayNo);
    func_ov135_021f9254(wk);
    wk->subprocessSeq = BOX_SEQ_START;
    wk->unk12E4 = 0;
    return WT_SEQ_FADEIN;
}

int func_ov135_021f51f0(WorldTradeWork *wk, int seq) {
    return data_ov135_02203b0c[wk->subprocessSeq](wk);
}

// pokebw2's WorldTrade_Box_End
int func_ov135_021f5204(WorldTradeWork *wk, int seq) {
    if (GXx_GetMasterBrightness_(REG_DB_MASTER_BRIGHT_ADDR) != 0) {
        func_ov135_021ffd74(wk);
    }
    func_ov135_021f577c(wk);
    func_ov135_021f59e8(wk);
    func_ov135_021f5940(wk);
    func_ov135_021f5358();
    func_ov135_022000e4(wk);
    func_0204b3dc(wk->promptDsAct, FALSE);
    func_ov135_021f4ec8(wk);
    return WT_SEQ_INIT;
}

// pokebw2's Box_BgInit
void func_ov135_021f5250(void) {
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
            2,
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

// The arrows by the box name
// pokebw2's sBoxArrowPos
static const BoxPos data_ov135_02202c50[] = {
    { 154, 12 },
    { 14, 12 },
};

// pokebw2's Box_BgExit
static void func_ov135_021f5358(void) {
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(0);
    GFL_BGSysReleaseBG(3);
}

// pokebw2's Box_BgGraphicSet
void func_ov135_021f5374(WorldTradeWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_WORLDTRADE, HEAPID_WORLDTRADE);

    GFL_G2DIOLoadArcNCLRDefault(arc, 1, 0, 0, 0x60, HEAPID_WORLDTRADE);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x1a0, 0x20, HEAPID_WORLDTRADE);
    LoadSysMsgBox(0, 1, 14, 0, HEAPID_WORLDTRADE);
    LoadSysMsgBox(0, 31, 11, 0, HEAPID_WORLDTRADE);
    GFL_BGSysLoadArcNCGRStatic(arc, 10, 1, 0, 0xa00, TRUE, HEAPID_WORLDTRADE);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 21, 1, 0, 0x600, TRUE, HEAPID_WORLDTRADE);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 31, 2, 0, 0x600, TRUE, HEAPID_WORLDTRADE);
    GFL_ArcToolFree(arc);

    func_ov135_02200ab4(wk);
    func_ov135_02200b34(wk);
}

void func_ov135_021f5420(WorldTradeWork *wk) {
    ClActorSetup setup;
    int i;

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.x = data_ov135_02202d24[wk->boxCursorPos].x;
    setup.y = data_ov135_02202d24[wk->boxCursorPos].y;
    wk->cursorAct = func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CHAR],
                                  wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_PLTT],
                                  wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CELL], &setup, 0, HEAPID_WORLDTRADE);
    func_0204b92c(wk->cursorAct, TRUE);
    func_0204b878(wk->cursorAct, 4);
    if (func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, FALSE);
    } else {
        func_0204b3dc(wk->cursorAct, TRUE);
    }
    if (wk->boxCursorPos == BOX_CURSOR_TRAY_POS || (wk->boxCursorPos >= 0 && wk->boxCursorPos <= 5)) {
        func_0204b84c(wk->cursorAct, 0);
    } else {
        func_0204b84c(wk->cursorAct, 1);
    }

    for (i = 0; i < BOX_POKE_NUM; i++) {
        setup.x = data_ov135_02202d24[i].x;
        setup.y = data_ov135_02202d24[i].y;
        setup.priority = 30 - i % 6;
        wk->pokeIconAct[i] =
            func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CHAR],
                          wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_PLTT],
                          wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CELL], &setup, 0, HEAPID_WORLDTRADE);
        func_0204b878(wk->pokeIconAct[i], i + 6);
        func_0204b84c(wk->pokeIconAct[i], 1);
    }
    for (i = 0; i < BOX_POKE_NUM; i++) {
        setup.x = data_ov135_02202d24[i].x;
        setup.y = data_ov135_02202d24[i].y + 6;
        setup.priority = 10;
        wk->itemIconAct[i] =
            func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CHAR],
                          wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_PLTT],
                          wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CELL], &setup, 0, HEAPID_WORLDTRADE);
        func_0204b878(wk->itemIconAct[i], 40);
        func_0204b84c(wk->itemIconAct[i], 1);
    }
    for (i = 0; i < 6; i++) {
        setup.x = data_ov135_02202d24[i].x + 8;
        setup.y = data_ov135_02202d24[i].y + 6;
        setup.priority = 10;
        wk->cballAct[i] =
            func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CHAR],
                          wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_PLTT],
                          wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CELL], &setup, 0, HEAPID_WORLDTRADE);
        func_0204b878(wk->cballAct[i], 42);
        func_0204b84c(wk->cballAct[i], 1);
    }
    for (i = 0; i < 2; i++) {
        setup.x = data_ov135_02202c50[i].x;
        setup.y = data_ov135_02202c50[i].y;
        wk->boxArrowAct[i] =
            func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CHAR],
                          wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_PLTT],
                          wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CELL], &setup, 0, HEAPID_WORLDTRADE);
        func_0204b878(wk->boxArrowAct[i], i + 38);
        func_0204b84c(wk->boxArrowAct[i], 1);
    }

    setup.x = 239;
    setup.y = 136;
    setup.priority = 100;
    setup.bgPriority = 1;
    wk->fingerAct = func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_MAIN2][WT_CLACT_RES_CHAR],
                                  wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_PLTT],
                                  wk->clactRes[WT_CLACT_RES_MAIN2][WT_CLACT_RES_CELL], &setup, 0, HEAPID_WORLDTRADE);
    func_0204b878(wk->fingerAct, 12);
    func_0204b92c(wk->fingerAct, TRUE);
    func_0204b974(wk->fingerAct);
    func_0204b3dc(wk->fingerAct, FALSE);

    func_0204b3dc(wk->promptDsAct, TRUE);
    func_ov135_021f4ea0(wk->promptDsAct, 55, 168);
}


static void func_ov135_021f577c(WorldTradeWork *wk) {
    s32 i;

    func_0204b3b4(wk->fingerAct);
    i = 0;
    do {
        func_0204b3b4(wk->boxArrowAct[i]);
        i = i + 1;
    } while (i < 2);
    func_0204b3b4(wk->cursorAct);
    i = 0;
    do {
        func_0204b3b4(wk->pokeIconAct[i]);
        func_0204b3b4(wk->itemIconAct[i]);
        i = i + 1;
    } while (i < 0x1e);
    i = 0;
    do {
        func_0204b3b4(wk->cballAct[i]);
        i = i + 1;
    } while (i < 6);
}

void func_ov135_021f57ec(WorldTradeWork *wk) {
    u8 bg;
    BmpWin *pBVar2;
    GFLBitmap *pGVar3;

    pBVar2 = BmpWin_CreateDynamic(3, 4, 0, 0xd, 3, 0xd, 1);
    wk->subWin = pBVar2;
    pGVar3 = BmpWin_GetBitmap(pBVar2);
    GFL_BitmapFill(pGVar3, 0);
    pBVar2 = wk->subWin;
    BmpWin_FlushChar(pBVar2);
    BmpWin_FlushMap(pBVar2);
    bg = BmpWin_GetBGIndex(pBVar2);
    GFL_BGSysLoadScr(bg);
    pBVar2 = BmpWin_CreateDynamic(0, 2, 0x15, 0x1b, 2, 0xd, 1);
    wk->msgWin = pBVar2;
    pGVar3 = BmpWin_GetBitmap(pBVar2);
    GFL_BitmapFill(pGVar3, 0);
    pBVar2 = wk->msgWin;
    BmpWin_FlushChar(pBVar2);
    BmpWin_FlushMap(pBVar2);
    bg = BmpWin_GetBGIndex(pBVar2);
    GFL_BGSysLoadScr(bg);
    pBVar2 = BmpWin_CreateDynamic(0, 2, 0x13, 0x1b, 4, 0xd, 1);
    wk->talkWin = pBVar2;
    pGVar3 = BmpWin_GetBitmap(pBVar2);
    GFL_BitmapFill(pGVar3, 0);
    pBVar2 = wk->msgWin;
    BmpWin_FlushChar(pBVar2);
    BmpWin_FlushMap(pBVar2);
    bg = BmpWin_GetBGIndex(pBVar2);
    GFL_BGSysLoadScr(bg);
    pBVar2 = BmpWin_CreateDynamic(1, 0x18, 0x10, 6, 2, 0, 1);
    wk->menuWin[1] = pBVar2;
    pGVar3 = BmpWin_GetBitmap(pBVar2);
    GFL_BitmapFill(pGVar3, 4);
    pBVar2 = wk->menuWin[1];
    BmpWin_FlushChar(pBVar2);
    BmpWin_FlushMap(pBVar2);
    bg = BmpWin_GetBGIndex(pBVar2);
    GFL_BGSysLoadScr(bg);
    func_ov135_021f91a0(wk->menuWin[1], wk->endString, 0, 1, 1, 0x3dc4,
            &wk->print);
    if (wk->subProcessMode == 5) {
        func_ov135_02200bac(wk, 3);
        return;
    }
    func_ov135_02200bac(wk, 1);
}

static void func_ov135_021f5940(WorldTradeWork *wk) {
    func_ov135_02202940(&wk->print);
    BmpWin_Free(wk->explainWin);
    BmpWin_Free(wk->talkWin);
    BmpWin_Free(wk->menuWin[1]);
    BmpWin_Free(wk->msgWin);
    BmpWin_Free(wk->subWin);
}

void func_ov135_021f5980(WorldTradeWork *wk) {
    func_ov135_021f6e08(wk);
    wk->boxTrayNameString = GFL_StrBufCreate(0x12U, 0x48U);
    wk->talkString = GFL_StrBufCreate(0xB4U, 0x48U);
    wk->endString = GFL_MsgDataLoadStrbufNew(wk->msgManager, 0x73U);
    if (wk->boxCursorPos == 0x1E) {
        wk->boxCursorPos = 0U;
    }
    wk->boxWork = GFL_HeapAllocate(0x48U, 0x78U, 0, "worldtrade_box.c", 0x344U);
}

static void func_ov135_021f59e8(WorldTradeWork *wk) {
    GFL_HeapFree(wk->boxWork);
    GFL_StrBufFree(wk->boxTrayNameString);
    GFL_StrBufFree(wk->talkString);
    GFL_StrBufFree(wk->endString);
}

int func_ov135_021f5a18(WorldTradeWork *wk) {
    int msgNo;

    if (GFL_WipeIsFinished()) {
        if (wk->subProcessMode == BOX_MODE_DEPOSIT_SELECT) {
            msgNo = 0x15;
        } else if (wk->subProcessMode == BOX_MODE_EXCHANGE_SELECT) {
            msgNo = 0x11;
        }
        func_ov135_021f663c(wk, msgNo, 1, 0, 0xf0f, 0);
        func_ov135_021f4e98(wk, BOX_SEQ_MESSAGE_WAIT, BOX_SEQ_MAIN);
        if (func_020355c4() == TRUE) {
            func_0204b3dc(wk->cursorAct, FALSE);
        } else {
            func_0204b3dc(wk->cursorAct, TRUE);
        }
    }
    return WT_SEQ_MAIN;
}


void func_ov135_021f5a7c(WorldTradeWork *wk) {
    u16 boxCursorPos;
    WorldTradeParam *ptr;
    WorldTradeParam *ptr2;
    WorldTradeParam *ptr3;

    boxCursorPos = wk->boxCursorPos;
    if (boxCursorPos == 0x1E) {
        func_0204b3dc(wk->fingerAct, 1);
        func_0204b9a0(wk->fingerAct);
        wk->subprocessSeq = 0x11;
        GFL_SndSEPlay(0x551U);
        return;
    }
    if (boxCursorPos != 0x1F) {
        ptr = wk->param;
        switch (func_ov135_021f6c48(ptr->myparty, ptr->mybox, (u32) wk->boxTrayNo, (u32) boxCursorPos)) {
        case 1:
            GFL_SndSEPlay(0x54CU);
            ptr2 = wk->param;
            if (func_ov135_021f6ba4(ptr2->myparty, ptr2->mybox, (s32) wk->boxTrayNo, (u32) wk->boxCursorPos) != 0) {
                ptr3 = wk->param;
                func_ov135_022025e4(wk->wordSet, 0U, func_ov135_021f6b40(ptr3->myparty, ptr3->mybox, (s32) wk->boxTrayNo, (s32) wk->boxCursorPos));
                func_ov135_021f663c(wk, 0x16U, 1U, 0U, 0xF0F, 0);
                func_ov135_021f4e98(wk, 3, 7);
                return;
            }
            func_ov135_021f663c(wk, 0x1AU, 1U, 0U, 0xF0F, 1);
            func_ov135_021f4e98(wk, 4, 1);
            return;
        case 2:
            GFL_SndSEPlay(0x54CU);
            func_ov135_021f663c(wk, 0x21U, 1U, 0U, 0xF0F, 1);
            func_ov135_021f4e98(wk, 4, 1);
            break;
        }
    }
}

void func_ov135_021f5b8c(WorldTradeWork *wk) {
    BoxPkm *pkm;
    u16 boxCursorPos;
    WorldTradeParam *ptr;
    WorldTradeParam *ptr2;
    WorldTradeParam *ptr3;

    boxCursorPos = wk->boxCursorPos;
    if (boxCursorPos == 0x1E) {
        func_0204b3dc(wk->fingerAct, 1);
        func_0204b9a0(wk->fingerAct);
        wk->subprocessSeq = 0x11;
        return;
    }
    if (boxCursorPos != 0x1F) {
        ptr = wk->param;
        switch (func_ov135_021f6c48(ptr->myparty, ptr->mybox, (s32) wk->boxTrayNo, (s32) boxCursorPos)) {
        case 1:
            ptr2 = wk->param;
            pkm = func_ov135_021f6b40(ptr2->myparty, ptr2->mybox, (s32) wk->boxTrayNo, (s32) wk->boxCursorPos);
            if (func_ov135_021f6cdc((BoxPkm *)pkm, &wk->downloadData[wk->unkD4].wantSimple) != 0) {
                ptr3 = wk->param;
                if (func_ov135_021f6ba4(ptr3->myparty, ptr3->mybox, (s32) wk->boxTrayNo, (s32) wk->boxCursorPos) != 0) {
                    func_ov135_022025e4(wk->wordSet, 0U, pkm);
                    func_ov135_021f663c(wk, 0x12, 1, 0, 0xF0F, 0);
                    func_ov135_021f4e98(wk, 3, 9);
                    GFL_SndSEPlay(0x54CU);
                    return;
                }
                func_ov135_021f663c(wk, 0x1A, 1, 0, 0xF0F, 1);
                func_ov135_021f4e98(wk, 4, 1);
                return;
            }
            GFL_SndSEPlay(0x551U);
            return;
        case 0:
        case 2:
            GFL_SndSEPlay(0x551U);
            break;
        }
    }
}

// The touch areas of the page's Pokémon, then the Quit button and the arrows
// pokebw2's sBoxTouchRects
static const TouchRect data_ov135_02202e24[] = {
#include "asset/worldtrade_box_data_ov135_02202e24.inc"
};

int func_ov135_021f5ca4(WorldTradeWork *wk) {
    s32 result;

    result = func_02035c08((void *)&data_ov135_02202e24);
    if (result != -1) {
        func_0204b3dc(wk->cursorAct, 0);
        func_020355d8(1);
    }
    return result;
}

// pokebw2's Box_CancelFunc
static void func_ov135_021f5cd4(WorldTradeWork *wk, int mode) {
    if (mode == BOX_MODE_DEPOSIT_SELECT) {
        func_ov135_021f4ec0(wk, WORLDTRADE_TITLE, 0);
        wk->subprocessSeq = BOX_SEQ_END;
    } else if (mode == BOX_MODE_EXCHANGE_SELECT) {
        func_ov135_021f4ec0(wk, WORLDTRADE_PARTNER, 0x11);
        wk->subprocessSeq = BOX_SEQ_END;
    }
}

int func_ov135_021f5cfc(WorldTradeWork *wk) {
    int touch = func_ov135_021f5ca4(wk);

    if (touch != -1) {
        switch (touch) {
        case BOX_CURSOR_TRAY_POS:
            func_0204b92c(wk->boxArrowAct[0], TRUE);
            func_0204b878(wk->boxArrowAct[0], 38);
            wk->boxTrayNo = func_ov135_021f5f9c(wk->boxTrayNo, wk->unkC8, 1);
            func_ov135_021f68a4(wk, wk->boxTrayNo);
            GFL_SndSEPlay(0x548);
            break;
        case 32:
            func_0204b92c(wk->boxArrowAct[1], TRUE);
            func_0204b878(wk->boxArrowAct[1], 39);
            wk->boxTrayNo = func_ov135_021f5f9c(wk->boxTrayNo, wk->unkC8, -1);
            func_ov135_021f68a4(wk, wk->boxTrayNo);
            GFL_SndSEPlay(0x548);
            break;
        case BOX_CURSOR_END_POS:
            func_0204b3dc(wk->fingerAct, TRUE);
            func_0204b9a0(wk->fingerAct);
            wk->subprocessSeq = 0x11;
            wk->boxCursorPos = touch;
            GFL_SndSEPlay(0x551);
            func_ov135_021f5f60(wk->cursorAct, wk->boxCursorPos);
            break;
        default:
            wk->boxCursorPos = touch;
            func_ov135_021f5f60(wk->cursorAct, wk->boxCursorPos);
            if (wk->subProcessMode == BOX_MODE_DEPOSIT_SELECT) {
                func_ov135_021f5a7c(wk);
            } else if (wk->subProcessMode == BOX_MODE_EXCHANGE_SELECT) {
                func_ov135_021f5b8c(wk);
            }
            break;
        }
    } else {
        func_ov135_021f5e44(wk);
        if (wk->subProcessMode == BOX_MODE_DEPOSIT_SELECT) {
            if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
                func_0204b3dc(wk->fingerAct, TRUE);
                func_0204b9a0(wk->fingerAct);
                wk->subprocessSeq = 0x11;
                GFL_SndSEPlay(0x551);
            } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
                func_ov135_021f5a7c(wk);
            }
        } else if (wk->subProcessMode == BOX_MODE_EXCHANGE_SELECT) {
            if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
                func_0204b3dc(wk->fingerAct, TRUE);
                func_0204b9a0(wk->fingerAct);
                wk->subprocessSeq = 0x11;
                GFL_SndSEPlay(0x551);
            } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
                func_ov135_021f5b8c(wk);
            }
        }
    }
    return 2;
}


// Where the cursor goes from each position, up, down, left and right. 99 and 101 turn the page
// pokebw2's sCursorMoveTable
static const u8 data_ov135_02202da4[][4] = {
#include "asset/worldtrade_box_data_ov135_02202da4.inc"
};

void func_ov135_021f5e44(WorldTradeWork *wk) {
    s32 sp4;
    ClActor **sp0;
    s32 val;
    s32 val2;
    s32 flag;
    u16 boxCursorPos;
    u8 boxCursorPos2;
    u8 val3;
    u32 flag2;

    flag = 0;
    val2 = 0;
    if ((GCTX_HIDGetPressedKeys() != 0) && (func_020355c4() == 1)) {
        func_0204b3dc(wk->cursorAct, 1);
        func_020355d8(0);
        GFL_SndSEPlay(0x548U);
        return;
    }
    if (GCTX_HIDGetPressedKeys() & 0x40) {
        val2 = 1;
    } else if (GCTX_HIDGetPressedKeys() & 0x80) {
        val2 = 2;
    } else if (GCTX_HIDGetPressedKeys() & 0x20) {
        val2 = 3;
    } else if (GCTX_HIDGetPressedKeys() & 0x10) {
        val2 = 4;
    }
    if (val2 != 0) {
        boxCursorPos = wk->boxCursorPos;
        boxCursorPos2 = data_ov135_02202da4[boxCursorPos][val2 - 1];
        if (boxCursorPos2 != boxCursorPos) {
            if ((boxCursorPos2 == 0x63) || (boxCursorPos2 == 0x65)) {
                flag2 = 1;
                if (boxCursorPos2 == 0x65) {
                    flag2 = 0;
                }
                val3 = flag2;
                sp0 = wk->boxArrowAct;
                val = val3 * 4;
                sp4 = val;
                func_0204b92c(sp0[val3], 1);
                func_0204b878(sp0[val3], (u16) (val3 + 0x26));
                wk->boxTrayNo = (u16) func_ov135_021f5f9c((s32) wk->boxTrayNo, wk->unkC8, boxCursorPos2 - 0x64);
                func_ov135_021f68a4(wk, wk->boxTrayNo);
                GFL_SndSEPlay(0x548U);
            } else {
                flag = 1;
                wk->boxCursorPos = (u16) boxCursorPos2;
            }
        }
    }
    if (flag != 0) {
        GFL_SndSEPlay(0x548U);
    }
    func_ov135_021f5f60(wk->cursorAct, wk->boxCursorPos);
}

void func_ov135_021f5f60(ClActor *cursor, int pos) {
    func_ov135_021f4ff4(cursor, data_ov135_02202d24[pos].x, data_ov135_02202d24[pos].y);
    if (pos == BOX_CURSOR_TRAY_POS || (pos >= 0 && pos <= 5)) {
        func_0204b84c(cursor, 0);
    } else {
        func_0204b84c(cursor, 1);
    }
}


// Turns the page from num by move, through the party's page between the last box and the first
// pokebw2's Box_RoundWork
static int func_ov135_021f5f9c(int num, int max, int move) {
    if (num == BOX_TRAY_PARTY) {
        if (move > 0) {
            return 0;
        } else if (move < 0) {
            return max - 1;
        }
    } else {
        num += move;
        if (num < 0) {
            return BOX_TRAY_PARTY;
        }
        if (num == max) {
            return BOX_TRAY_PARTY;
        }
    }
    return num;
}

static int func_ov135_021f5fc0(WorldTradeWork *wk) {
    ListMenuOption *pLVar1;

    pLVar1 = ListMenuCore_CreateOptionList(3, 0x48);
    wk->menuList = pLVar1;
    ListMenuCore_AppendMsgOption(wk->menuList, wk->msgManager, 0x68, 1, 0x48);
    ListMenuCore_AppendMsgOption(wk->menuList, wk->msgManager, 0x69, 2, 0x48);
    ListMenuCore_AppendMsgOption(wk->menuList, wk->msgManager, 0x6a, 3, 0x48);
    func_ov135_021f4d50(wk, 0, 3, 0x14);
    wk->subprocessSeq = 8;
    return 2;
}

int func_ov135_021f601c(WorldTradeWork *wk) {
    u32 ret = func_ov135_021f4e30(wk);

    switch (ret) {
    case 1:
        func_ov135_021f4e64(wk);
        ListMenuCore_FreeOptionList(wk->menuList);
        func_0201f7a8(wk->msgWin, 0);
        wk->subprocessSeq = 2;
        func_ov135_021f4ec0(wk, 8, 5);
        break;
    case 2: {
        BoxPkm *pkm;

        func_ov135_021f4e64(wk);
        ListMenuCore_FreeOptionList(wk->menuList);
        pkm = func_ov135_021f6b40(wk->param->myparty, wk->param->mybox, wk->boxTrayNo, wk->boxCursorPos);
        if (func_ov135_021f6bc4(pkm) != 0) {
            func_ov135_021f663c(wk, 0x2b, 1, 0, 0xf0f, 1);
            func_ov135_021f4e98(wk, 4, 1);
        } else if (func_ov135_021f6bfc(pkm) != 0) {
            if (func_ov135_021f6bfc(pkm) == 1) {
                func_ov135_021f663c(wk, 0xbf, 1, 0, 0xf0f, 1);
            } else {
                func_ov135_021f663c(wk, 0xc0, 1, 0, 0xf0f, 1);
            }
            func_ov135_021f4e98(wk, 4, 1);
        } else if (func_ov135_021f6c28(pkm) != 0) {
            func_ov135_021f663c(wk, 0xc1, 1, 0, 0xf0f, 1);
            func_ov135_021f4e98(wk, 4, 1);
        } else {
            if (func_ov135_021f6b34(wk->boxTrayNo) != 0) {
                PokeParty_GetPkm(wk->param->myparty, wk->boxCursorPos);
            }
            wk->pokePtr = func_ov135_021f6b40(wk->param->myparty, wk->param->mybox, wk->boxTrayNo, wk->boxCursorPos);
            wk->subprocessSeq = 2;
            func_ov135_021f4ec0(wk, 6, 0);
        }
        break;
    }
    case 3:
    case BMPMENULIST_CANCEL:
        func_ov135_021f4e64(wk);
        ListMenuCore_FreeOptionList(wk->menuList);
        func_0201f7a8(wk->msgWin, 0);
        wk->subprocessSeq = 0;
        break;
    }
    return 2;
}


static int func_ov135_021f6178(WorldTradeWork *wk) {
    ListMenuOption *pLVar1;

    pLVar1 = ListMenuCore_CreateOptionList(3, 0x48);
    wk->menuList = pLVar1;
    ListMenuCore_AppendMsgOption(wk->menuList, wk->msgManager, 0x5d, 1, 0x48);
    ListMenuCore_AppendMsgOption(wk->menuList, wk->msgManager, 0x5e, 2, 0x48);
    ListMenuCore_AppendMsgOption(wk->menuList, wk->msgManager, 0x5f, 3, 0x48);
    func_ov135_021f4d50(wk, 0, 3, 0x14);
    wk->subprocessSeq = 10;
    return 2;
}

static int func_ov135_021f61d4(WorldTradeWork *wk) {
    u32 ret = func_ov135_021f4e30(wk);

    switch (ret) {
    case 1:
        func_ov135_021f4e64(wk);
        ListMenuCore_FreeOptionList(wk->menuList);
        wk->subprocessSeq = 2;
        func_ov135_021f4ec0(wk, 8, 6);
        break;
    case 2: {
        BoxPkm *pkm;

        func_ov135_021f4e64(wk);
        ListMenuCore_FreeOptionList(wk->menuList);
        func_0201f7a8(wk->msgWin, 0);
        pkm = func_ov135_021f6b40(wk->param->myparty, wk->param->mybox, wk->boxTrayNo, wk->boxCursorPos);
        if (func_ov135_021f6bc4(pkm) != 0) {
            func_ov135_021f663c(wk, 0x2b, 1, 0, 0xf0f, 1);
            func_ov135_021f4e98(wk, 4, 1);
        } else if (func_ov135_021f6bfc(pkm) != 0) {
            if (func_ov135_021f6bfc(pkm) == 1) {
                func_ov135_021f663c(wk, 0xbf, 1, 0, 0xf0f, 1);
            } else {
                func_ov135_021f663c(wk, 0xc0, 1, 0, 0xf0f, 1);
            }
            func_ov135_021f4e98(wk, 4, 1);
        } else if (func_ov135_021f6c28(pkm) != 0) {
            func_ov135_021f663c(wk, 0xc1, 1, 0, 0xf0f, 1);
            func_ov135_021f4e98(wk, 4, 1);
        } else {
            if (func_ov135_021f6b34(wk->boxTrayNo) != 0) {
                PokeParty_GetPkm(wk->param->myparty, wk->boxCursorPos);
            }
            if (PML_PkmGetParam(pkm, PKM_PARAM_SPECIES, NULL) == SPECIES_SHAYMIN) {
                PML_PkmChangeForme(pkm, 0);
            }
            func_ov135_021f6474(wk);
        }
        break;
    }
    case 3:
    case BMPMENULIST_CANCEL:
        func_ov135_021f4e64(wk);
        ListMenuCore_FreeOptionList(wk->menuList);
        func_0201f7a8(wk->msgWin, 0);
        wk->subprocessSeq = 0;
        break;
    }
    return 2;
}


// pokebw2's Box_SubSeqEnd
static int func_ov135_021f632c(WorldTradeWork *wk) {
    if (wk->subNextProcess == WORLDTRADE_ENTER || wk->subNextProcess == WORLDTRADE_STATUS ||
        wk->subNextProcess == WORLDTRADE_PARTNER) {
        GFL_WipeSet(0, 0, 0, 0, 6, 1, HEAPID_WORLDTRADE);
        wk->subOutFlag = 1;
    } else {
        GFL_WipeSet(3, 0, 0, 0, 6, 1, HEAPID_WORLDTRADE);
    }
    wk->subprocessSeq = BOX_SEQ_START;
    return WT_SEQ_FADEOUT;
}

static int func_ov135_021f6384(WorldTradeWork *wk) {
    func_ov135_021f4be0(wk, 0x14, 0x1ad, 8, 1);
    wk->subprocessSeq = 6;
    return 2;
}

static int func_ov135_021f63a8(WorldTradeWork *wk) {
    u8 result;

    result = func_ov135_021f4d14(wk);
    GFL_DebugAssertFail("", 0, "");
    if (result == 1) {
        func_ov135_021f4ce4(wk);
        wk->subprocessSeq = 2;
        func_ov135_021f4ec0(wk, 0, 0);
    }
    else if (result == 2) {
        func_ov135_021f4ce4(wk);
        wk->subprocessSeq = 0;
    }
    return 2;
}

static int func_ov135_021f63ec(WorldTradeWork *wk) {
    func_ov135_021f663c(wk, 0x19U, 1U, 0U, 0xF0F, 1);
    func_ov135_021f4e98(wk, 3, 0xC);
    return 2;
}

static int func_ov135_021f6418(WorldTradeWork *wk) {
    func_ov135_021f4be0(wk, 0x12, 0x1ad, 8, 1);
    wk->subprocessSeq = 0xd;
    return 2;
}

int func_ov135_021f643c(WorldTradeWork *wk) {
    u32 ret = func_ov135_021f4d14(wk);

    if (ret == 1) {
        func_ov135_021f4ce4(wk);
        func_ov135_021f6474(wk);
    } else if (ret == 2) {
        func_ov135_021f4ce4(wk);
        func_0201f7a8(wk->talkWin, 0);
        wk->subprocessSeq = BOX_SEQ_MAIN;
    }
    return WT_SEQ_MAIN;
}


int func_ov135_021f6474(WorldTradeWork *wk) {
    WorldTradeParam *ptr;

    if ((func_ov135_021f6de8((PartyPkm *)wk->downloadData[wk->unkD4].postData) != 0) && (wk->boxTrayNo != 0xFF) && (PokeParty_GetPkmCount(wk->param->myparty) == 6)) {
        func_ov135_021f663c(wk, 0x29U, 1U, 0U, 0xF0F, 1);
        func_ov135_021f4e98(wk, 4, 1);
        return 0;
    }
    ptr = wk->param;
    wk->pokePtr = func_ov135_021f6b40(ptr->myparty, ptr->mybox, (s32) wk->boxTrayNo, (s32) wk->boxCursorPos);
    wk->subprocessSeq = 2;
    wk->subOutFlag = 1;
    func_ov135_021f4ec0(wk, 7, 9);
    func_ov135_021f6d14(&wk->uploadData, wk);
    wk->searchResult = 0;
    return 1;
}

static int func_ov135_021f651c(WorldTradeWork *wk) {
    func_ov135_021f663c(wk, 0x19U, 1U, 0U, 0xF0F, 1);
    func_ov135_021f4e98(wk, 3, 0xF);
    return 2;
}

static int func_ov135_021f6548(WorldTradeWork *wk) {
    func_ov135_021f4be0(wk, 0x12, 0x1ad, 8, 1);
    wk->subprocessSeq = 0x10;
    return 2;
}

int func_ov135_021f656c(WorldTradeWork *wk) {
    u8 ret = func_ov135_021f4d14(wk);

    if (ret == 1) {
        func_ov135_021f4ce4(wk);
        wk->pokePtr = func_ov135_021f6b40(wk->param->myparty, wk->param->mybox, wk->boxTrayNo, wk->boxCursorPos);
        wk->subprocessSeq = 2;
        func_ov135_021f4ec0(wk, 6, 0);
    } else if (ret == 2) {
        func_ov135_021f4ce4(wk);
        func_0201f7a8(wk->talkWin, 0);
        wk->subprocessSeq = 1;
    }
    return 2;
}


static int func_ov135_021f65c8(WorldTradeWork *wk) {
    BOOL BVar1;

    BVar1 = func_0204b98c(wk->fingerAct);
    if (BVar1 == 0) {
        func_ov135_021f5cd4(wk, wk->subProcessMode);
    }
    return 2;
}

static int func_ov135_021f65e8(WorldTradeWork *wk) {
    BOOL BVar1;

    BVar1 = func_ov135_02202840(&wk->print);
    if (BVar1 == 0) {
        wk->subprocessSeq = wk->subprocessNextSeq;
    }
    return 2;
}

static int func_ov135_021f6604(WorldTradeWork *wk) {
    BOOL BVar1;
    BVar1 = func_ov135_02202840(&wk->print);
    if ((BVar1 == 0) && ((GCTX_HIDGetPressedKeys() != 0 || (BVar1 = func_02035c60(), BVar1 != 0)))) {
        func_0201f7a8(wk->talkWin, 0);
        wk->subprocessSeq = wk->subprocessNextSeq;
    }
    return 2;
}

void func_ov135_021f663c(WorldTradeWork *wk, int msgNo, int wait, int flag, u16 dat, int winFlag) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(wk->msgManager, msgNo);
    BmpWin *win;

    GFL_WordSetFormatStrbuf(wk->wordSet, wk->talkString, str);
    if (winFlag == 0) {
        win = wk->msgWin;
    } else {
        win = wk->talkWin;
    }
    GFL_BitmapFill(BmpWin_GetBitmap(win), 15);
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(win));
    BmpWin_DrawFrame(win, 0, 1, 14);
    func_ov135_02202858(win, 0, wk->talkString, 0, 0, &wk->print);
    GFL_StrBufFree(str);
}


// pokebw2's Box_CharDataGetbyHandle
static void *func_ov135_021f66c8(ArcTool *handle, u32 dataIdx, NNSG2dCharacterData **charData, u32 heapId) {
    void *file = GFL_ArcToolReadHeapNew(handle, dataIdx, heapId);

    if (file != NULL) {
        if (!NNS_G2dGetUnpackedBGCharacterData(file, charData)) {
            GFL_HeapFree(file);
            return NULL;
        }
    }
    return file;
}

void func_ov135_021f66f0(int species, int form, int sex, int egg, int no, ClActor *icon, ArcTool *handle, WorldTradePokeBuf *pbuf) {
    u32 dataIdx;
    void *ptr;
    NNSG2dCharacterData *pNStack_18;

    dataIdx = PokeParty_GetIconIndex(species, form, sex, egg);
    ptr = func_ov135_021f66c8(handle, dataIdx, &pNStack_18, 0x48);
    MIi_CpuCopyFast(pNStack_18->pRawData, pbuf->charData, 0x200);
    pbuf->charOffset = (no * 0x10 + 0x1c) * 0x20;
    pbuf->icon = icon;
    dataIdx = func_0201bb5c(species, form, sex, egg);
    pbuf->palNo = dataIdx + 5;
    GFL_HeapFree(ptr);
}

// pokebw2's Box_PokemonLevelSet
void func_ov135_021f6748(BoxPkm *pkm, Dpw_Tr_PokemonDataSimple *dat) {
    dat->level = (s8)PML_PkmGetLevel(pkm);
}

void func_ov135_021f6754(WorldTradeWork *wk) {
    s32 i;
    WorldTradePokeBuf *ptr;

    ptr = wk->boxIcon;
    i = 0;
    do {
        if (ptr->icon != NULL) {
            DC_FlushRange(ptr->charData, 0x200);
            GX_LoadOBJ(ptr->charData, ptr->charOffset, 0x200);
            func_0204b6f4(ptr->icon, ptr->palNo & 0xff, 1);
        }
        i = i + 1;
        ptr = ptr + 1;
    } while (i < 0x1e);
    GFL_HeapFree(wk->boxIcon);
}

void func_ov135_021f67a8(BoxPkm *pkm, ClActor *icon, ClActor *itemAct, u16 *no, int pos, ArcTool *handle, Dpw_Tr_PokemonDataSimple *dat, WorldTradePokeBuf *pbuf) {
    u32 sp20;
    u32 sp1C;
    u32 egg;
    u32 sp14;

    PML_PkmDecrypt(pkm);
    sp20 = PML_PkmGetParam(pkm, 0xA9U, NULL);
    *no = (u16) PML_PkmGetParam(pkm, 5U, NULL);
    sp14 = PML_PkmGetParam(pkm, 0x6FU, NULL);
    egg = PML_PkmGetParam(pkm, 0x4CU, NULL);
    sp1C = PML_PkmGetParam(pkm, 6U, NULL);
    dat->characterNo = *no;
    dat->gender = (s8) (PML_PkmGetParam(pkm, 0x6EU, NULL) + 1);
    if (egg != 0) {
        dat->level = 0;
    }
    PML_PkmReEncrypt(pkm, 1);
    if (sp20 != 0) {
        func_ov135_021f66f0((s32) *no, (s32) sp14, dat->gender - 1, (s32) egg, pos, icon, handle, pbuf);
        func_0204b3dc(icon, 1);
        if (sp1C != 0) {
            func_0204b3dc(itemAct, 1);
            if (PML_ItemIsMail((u16) sp1C) != 0) {
                func_0204b878(itemAct, 0x29U);
                return;
            }
            func_0204b878(itemAct, 0x28U);
            return;
        }
        func_0204b3dc(itemAct, 0);
        return;
    }
    func_0204b3dc(icon, 0);
    func_0204b3dc(itemAct, 0);
    pbuf->icon = NULL;
}

void func_ov135_021f68a4(WorldTradeWork *wk, int now) {
    BoxSaveAccessor *box = wk->param->mybox;
    u16 no[BOX_POKE_NUM];
    ArcTool *handle;
    WorldTradePokeBuf *pbuf;
    BoxPkm *pkm;
    int num;
    u16 i = 0;

    pbuf = GFL_HeapAllocate(HEAPID_TAIL(HEAPID_USER), sizeof(WorldTradePokeBuf) * BOX_POKE_NUM, FALSE,
                            "worldtrade_box.c", 2194);
    wk->boxIcon = pbuf;
    handle = GFL_ArcSysCreateFileHandle(7, HEAPID_WORLDTRADE);

    if (now >= 0 && now < wk->unkC8) {
        for (i = 0; i < BOX_POKE_NUM; i++) {
            func_ov135_021f6748(BoxSaveAccessor_GetPkm(box, now, i), &wk->boxWork[i]);
        }
        for (i = 0; i < BOX_POKE_NUM; i++) {
            wk->boxWork[i].characterNo = 0;
            func_ov135_021f67a8(BoxSaveAccessor_GetPkm(box, now, i), wk->pokeIconAct[i], wk->itemIconAct[i], &no[i], i,
                                handle, &wk->boxWork[i], &pbuf[i]);
            if (i < 6) {
                func_0204b3dc(wk->cballAct[i], FALSE);
            }
        }
        loadBoxNameToStrbuf(box, now, wk->boxTrayNameString);
    } else if (now == BOX_TRAY_PARTY) {
        num = PokeParty_GetPkmCount(wk->param->myparty);
        for (i = 0; i < num; i++) {
            pkm = func_0201d624(PokeParty_GetPkm(wk->param->myparty, i));
            func_ov135_021f6748(pkm, &wk->boxWork[i]);
            func_ov135_021f67a8(pkm, wk->pokeIconAct[i], wk->itemIconAct[i], &no[i], i, handle, &wk->boxWork[i],
                                &pbuf[i]);
            func_0204b3dc(wk->cballAct[i], FALSE);
        }
        for (; i < BOX_POKE_NUM; i++) {
            wk->boxWork[i].characterNo = 0;
            func_0204b3dc(wk->pokeIconAct[i], FALSE);
            func_0204b3dc(wk->itemIconAct[i], FALSE);
            pbuf[i].icon = NULL;
            if (i < 6) {
                func_0204b3dc(wk->cballAct[i], FALSE);
            }
        }
        GFL_MsgDataLoadStrbuf(wk->msgManager, 0x62, wk->boxTrayNameString);
    } else {
        GFL_ASSERT(FALSE);
    }
    GFL_ArcToolFree(handle);

    GFL_BitmapFill(BmpWin_GetBitmap(wk->subWin), 0);
    func_ov135_021f91a0(wk->subWin, wk->boxTrayNameString, 0, 5, 1, 0x440, &wk->print);
    if (wk->subProcessMode == BOX_MODE_EXCHANGE_SELECT) {
        func_ov135_021f6db4(wk->boxWork, wk->pokeIconAct, &wk->downloadData[wk->unkD4].wantSimple, pbuf);
    }
    wk->vfunc = func_ov135_021f6754;
}


// pokebw2's WorldTrade_GetPPorPPP
BOOL func_ov135_021f6b34(int tray) {
    if (tray == BOX_TRAY_PARTY) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WorldTrade_GetPokePtr
BoxPkm *func_ov135_021f6b40(PokeParty *party, BoxSaveAccessor *box, int tray, int pos) {
    if (func_ov135_021f6b34(tray)) {
        if (pos > PokeParty_GetPkmCount(party) - 1) {
            return NULL;
        }
        return func_0201d624(PokeParty_GetPkm(party, pos));
    }
    return BoxSaveAccessor_GetPkm(box, tray, pos);
}

// pokebw2's WorldTrade_GetBoxPokeNum
int func_ov135_021f6b80(PokeParty *party, BoxSaveAccessor *box, int tray) {
    if (func_ov135_021f6b34(tray)) {
        return PokeParty_GetPkmCount(party);
    }
    return howManyPokesInGeneralAreInBox(box, tray);
}

// Whether the Pokémon may leave: not the party's last one
// pokebw2's Box_CheckPocket
static BOOL func_ov135_021f6ba4(PokeParty *party, BoxSaveAccessor *box, int tray, int pos) {
    if (func_ov135_021f6b34(tray) && PokeParty_GetPkmCount(party) < 2) {
        return FALSE;
    }
    return TRUE;
}

// The ribbons that keep a Pokémon from the Global Trade Station
// pokebw2's sRibbonCheckTable
static const u32 data_ov135_02202c58[] = {
#include "asset/worldtrade_box_data_ov135_02202c58.inc"
};

// pokebw2's Box_PokeRibbonCheck
u32 func_ov135_021f6bc4(BoxPkm *pkm) {
    BOOL flag = PML_PkmDecrypt(pkm);
    int i;
    u32 ret;

    for (i = 0; i < NELEMS(data_ov135_02202c58); i++) {
        ret = PML_PkmGetParam(pkm, data_ov135_02202c58[i], NULL);
        if (ret == TRUE) {
            break;
        }
    }
    PML_PkmReEncrypt(pkm, flag);
    return ret;
}

u32 func_ov135_021f6bfc(BoxPkm *param_1) {
    BOOL wasEncrypted;

    wasEncrypted = PML_PkmDecrypt(param_1);
    PML_PkmGetParam(param_1, 5, (void *)0x0);
    PML_PkmGetParam(param_1, 0x6f, (void *)0x0);
    PML_PkmReEncrypt(param_1, wasEncrypted);
    return 0;
}

// pokebw2's Box_PokeNewItemCheck
BOOL func_ov135_021f6c28(BoxPkm *pkm) {
    BOOL wasEncrypted;

    wasEncrypted = PML_PkmDecrypt(pkm);
    PML_PkmGetParam(pkm, 6, (void *)0x0);
    PML_PkmReEncrypt(pkm, wasEncrypted);
    return 0;
}

// 0 for no Pokémon, 2 for an egg, 1 for a Pokémon
// pokebw2's Box_PokemonCheck
static int func_ov135_021f6c48(PokeParty *party, BoxSaveAccessor *box, int tray, int pos) {
    BoxPkm *pkm = func_ov135_021f6b40(party, box, tray, pos);

    if (pkm == NULL) {
        return 0;
    }
    if (PML_PkmGetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL) == 0) {
        return 0;
    }
    if (PML_PkmGetParam(pkm, 0xaa, NULL)) {
        return 2;
    }
    return 1;
}

// pokebw2's Box_CompareSearchData
BOOL func_ov135_021f6c80(Dpw_Tr_PokemonDataSimple *poke, Dpw_Tr_PokemonSearchData *search) {
    s8 level;

    if (poke->characterNo != search->characterNo) {
        return 0;
    }
    if ((search->gender != 3) && (search->gender != poke->gender)) {
        return 0;
    }
    level = poke->level;
    if (level == 0) {
        return 0;
    }
    if ((search->level_min != 0) && (search->level_min > level)) {
        return 0;
    }
    if ((search->level_max != 0) && (search->level_max < level)) {
        return 0;
    }
    return 1;
}

// pokebw2's Box_WantPokeCheck
BOOL func_ov135_021f6cdc(BoxPkm *pkm, Dpw_Tr_PokemonSearchData *want) {
    Dpw_Tr_PokemonDataSimple sp0;

    sp0.characterNo = (s16) PML_PkmGetParam(pkm, 5U, NULL);
    sp0.gender = PML_PkmGetParam(pkm, 0x6EU, NULL) + 1;
    sp0.level = (s8) PML_PkmGetLevel(pkm);
    return func_ov135_021f6c80(&sp0, want);
}

void func_ov135_021f6d14(Dpw_Tr_Data *dtd, WorldTradeWork *wk) {
    Dpw_Tr_PokemonDataSimple post;
    Dpw_Tr_PokemonSearchData want;
    BoxPkm *pkm;

    post.characterNo = PML_PkmGetParam(wk->pokePtr, PKM_PARAM_SPECIES, NULL);
    post.gender = PML_PkmGetParam(wk->pokePtr, PKM_PARAM_SEX, NULL) + 1;
    post.level = PML_PkmGetLevel(wk->pokePtr);
    dtd->postSimple = post;
    func_ov135_021f8554(dtd, wk);

    pkm = func_0201d624((PartyPkm *)wk->downloadData[wk->unkD4].postData);
    want.characterNo = PML_PkmGetParam(pkm, PKM_PARAM_SPECIES, NULL);
    want.gender = PML_PkmGetParam(pkm, PKM_PARAM_SEX, NULL) + 1;
    want.level_min = 0;
    want.level_max = 0;
    dtd->wantSimple = want;
}


void func_ov135_021f6db4(Dpw_Tr_PokemonDataSimple *box, ClActor **icons, Dpw_Tr_PokemonSearchData *want, WorldTradePokeBuf *pbuf) {
    BOOL BVar1;
    s32 i;

    i = 0;
    do {
        if ((box[i].characterNo != 0) &&
                (BVar1 = func_ov135_021f6c80(&box[i], want), BVar1 == 0)) {
            pbuf[i].palNo = pbuf[i].palNo + 3;
        }
        i = i + 1;
    } while (i < 0x1e);
}

// pokebw2's WorldTrade_PokemonMailCheck
BOOL func_ov135_021f6de8(PartyPkm *pkm) {
    BOOL ret = FALSE;

    if (PML_ItemIsMail(PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL))) {
        ret = TRUE;
    }
    return ret;
}

static void func_ov135_021f6e08(WorldTradeWork *wk) {
    u32 val;
    u32 availableBoxCount;
    u32 box;

    availableBoxCount = (u32) BoxSaveAccessor_GetAvailableBoxCount(wk->param->mybox);
    wk->unkC8 = availableBoxCount;
    if (availableBoxCount < 0x18U) {
        box = 0;
        val = 0U;
        if (availableBoxCount > val) {
loop_2:
            if (howManyPokesInGeneralAreInBox(wk->param->mybox, box) != 0) {
                box += 1;
                if (box < (u32) wk->unkC8) {
                    goto loop_2;
                }
            }
        }
        if (box == wk->unkC8) {
            wk->unkC8 = (u32) BoxSaveAccessor_UnlockMoreBoxes(wk->param->mybox);
        }
    }
}
