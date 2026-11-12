// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov214/worldtrade_mypoke.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
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
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "pml/item.h"
#include "pml/poke_graphic.h"
#include "pml/poke_party.h"
#include "save/player_info.h"
#include "save/worldtrade_data.h"
#include "system/bmp_menulist.h"
#include "system/bmp_winframe.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "worldtrade_local.h"
#include "app/worldtrade.h"
#include "system/bmp_menuwork.h"
#include "dpw/dpw_tr.h"

// The Global Trade Station's screen of the Pokémon the player deposited, where they can take it back. The file's name
// is a guess, as the ROM doesn't give it, and so are the names of its functions

enum {
    MYPOKE_SEQ_START,
    MYPOKE_SEQ_MAIN,
    MYPOKE_SEQ_END,
    MYPOKE_SEQ_MESSAGE_WAIT,
    MYPOKE_SEQ_MESSAGE_1MIN_WAIT,
    MYPOKE_SEQ_YESNO,
    MYPOKE_SEQ_YESNO_SELECT,
    MYPOKE_SEQ_SELECT_LIST,
    MYPOKE_SEQ_SELECT_WAIT,
};

// How many windows of information the screen has
#define MYPOKE_INFOWIN_NUM 14

// The upload screen's mode that takes the Pokémon back
#define UPLOAD_MODE_DOWNLOAD 8

// Black's
typedef struct {
    int x;
    int y;
    int w;
    int h;
} MyPokeWinPos;

void func_ov135_021fc1c0(void);
static void func_ov135_021fc2a4(void);
void func_ov135_021fc2c0(WorldTradeWork *wk);
void func_ov135_021fc360(WorldTradeWork *wk);
static void func_ov135_021fc3d0(WorldTradeWork *wk);
void func_ov135_021fc3e0(WorldTradeWork *wk);
static void func_ov135_021fc4e4(WorldTradeWork *wk);
void func_ov135_021fc528(WorldTradeWork *wk);
static void func_ov135_021fc568(WorldTradeWork *wk);
static int func_ov135_021fc598(WorldTradeWork *wk);
static int func_ov135_021fc5a0(WorldTradeWork *wk);
static int func_ov135_021fc610(WorldTradeWork *wk);
static int func_ov135_021fc638(WorldTradeWork *wk);
static int func_ov135_021fc668(WorldTradeWork *wk);
static int func_ov135_021fc6b8(WorldTradeWork *wk);
int func_ov135_021fc70c(WorldTradeWork *wk);
static int func_ov135_021fc7c4(WorldTradeWork *wk);
static int func_ov135_021fc7e0(WorldTradeWork *wk);
static void func_ov135_021fc810(WorldTradeWork *wk, int msgNo, int wait, int flag, u16 dat, PartyPkm *pkm);
u16 func_ov135_021fc8a8(int sex);
void func_ov135_021fcbf0(WorldTradeWork *wk);


static const MyPokeWinPos data_ov135_0220351c[MYPOKE_INFOWIN_NUM] = {
#include "asset/worldtrade_mypoke_data_ov135_0220351c.inc"
};

static int (*data_ov135_02203cf0[9])(WorldTradeWork *wk);

typedef struct WorldTradePrint WorldTradePrint;
void func_ov135_02202940(WorldTradePrint *print);
void func_ov135_021f5060(void);
void func_ov135_021f5034(s32 param_1);
BOOL func_ov135_02202840(WorldTradePrint *print);

// pokebw2's WorldTrade_MyPoke_Main

int func_ov135_021fc06c(WorldTradeWork *wk, int seq) {
    PartyPkm *pkm = func_0200adb4(wk->param->worldtrade_data);

    func_ov135_021fc528(wk);
    func_ov135_021fc1c0();
    func_ov135_021fffe8(wk, 0, FALSE);
    func_ov135_021fc2c0(wk);
    func_ov135_021fc3e0(wk);
    func_ov135_021fc360(wk);
    func_ov135_021fc8cc(wk->msgManager, wk->monsNameManager, wk->wordSet, &wk->infoWin[0], func_020185f0(pkm),
        &wk->uploadData.postSimple, &wk->print);
    func_ov135_021fca8c(wk->msgManager, &wk->infoWin[7], GetPlayerName(wk->param->mystatus), pkm,
        &wk->infoWin[12], &wk->print);
    func_ov135_021f8290(wk->msgManager, wk->monsNameManager, wk->wordSet, &wk->infoWin[9],
        wk->uploadData.wantSimple.characterNo, wk->uploadData.wantSimple.gender,
        func_ov135_021f886c(wk->uploadData.wantSimple.level_min, wk->uploadData.wantSimple.level_max, 0), &wk->print);
    func_ov135_021fcb54(pkm);
    GFL_WipeSet(3, 1, 1, 0, 6, 1, HEAPID_WORLDTRADE);
    wk->subprocessSeq = MYPOKE_SEQ_START;
    wk->unk12E4 = 0;
    return WT_SEQ_FADEIN;
}

int func_ov135_021fc174(WorldTradeWork *wk, int seq) {
    return data_ov135_02203cf0[wk->subprocessSeq](wk);
}

void func_ov135_022000e4(WorldTradeWork *wk);
int func_ov135_021fc188(WorldTradeWork *wk, int seq) {
    func_ov135_021fc3d0(wk);
    func_ov135_021fc568(wk);
    func_ov135_021fc4e4(wk);
    func_ov135_021fc2a4();
    func_ov135_022000e4(wk);
    func_0204b3dc(wk->promptDsAct, FALSE);
    func_ov135_021f4ec8(wk);
    return WT_SEQ_INIT;
}


// pokebw2's MyPoke_BgInit
void func_ov135_021fc1c0(void) {
    {
        BGSysLCDConfig config = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };
        u32 enabled = GFL_BGSysGetEnabledBGsB();

        GFL_BGSysSetLCDConfig(&config);
        GFL_BGSysSetEnabledBGsB(enabled);
    }
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ, TRUE);
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
    GFL_BGSysClearCharCore(2, 32, 0, HEAPID_WORLDTRADE);
    GFL_BGSysClearCharCore(3, 32, 0, HEAPID_WORLDTRADE);
}

// pokebw2's sMyPokeSubSeqTable
static int (*data_ov135_02203cf0[])(WorldTradeWork *wk) = {
    func_ov135_021fc598,           func_ov135_021fc5a0,  func_ov135_021fc610,         func_ov135_021fc7c4,
    func_ov135_021fc7e0, func_ov135_021fc638, func_ov135_021fc668, func_ov135_021fc6b8,
    func_ov135_021fc70c,
};

// pokebw2's MyPoke_BgExit
static void func_ov135_021fc2a4(void) {
    GFL_BGSysReleaseBG(0);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(3);
}

void func_ov135_021fc2c0(WorldTradeWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0x60, 0x48);

    GFL_G2DIOLoadArcNCLRDefault(arc, 6, 0, 0, 0x60, 0x48);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x1a0, 0x20, 0x48);
    LoadSysMsgBox(2, 1, 14, 0, 0x48);
    LoadSysMsgBox(2, 31, 11, 0, 0x48);
    GFL_BGSysLoadArcNCGRStatic(arc, 16, 1, 0, 0, TRUE, 0x48);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 22, 1, 0, 0x600, TRUE, 0x48);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 0x20, 0x20, 0x48);
    func_ov135_02200b34(wk);
    GFL_ArcToolFree(arc);
}

void func_ov135_021fc360(WorldTradeWork *wk) {
    ClActor *actor;
    ClActorSetup CStack_1c;

    MI_CpuFill8(&CStack_1c, 0, 8);
    CStack_1c.x = 0xd0;
    CStack_1c.y = 0x3a;
    actor = func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CHAR],
            wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_PLTT], wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CELL], &CStack_1c, 0, 0x48);
    wk->pokemonAct = actor;
    func_0204b92c(actor, 1);
    func_0204b878(wk->pokemonAct, 0x24);
    func_0204b84c(wk->pokemonAct, 1);
    func_0204b3dc(wk->pokemonAct, 1);
    func_0203d34c(0, 0x48);
}

static void func_ov135_021fc3d0(WorldTradeWork *wk) {
    func_0204b3b4(wk->pokemonAct);
}

void func_ov135_021fc3e0(WorldTradeWork *wk) {
    BmpWin *win;
    int i;

    wk->msgWin = BmpWin_CreateDynamic(2, 2, 21, 27, 2, 13, 1);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->msgWin), 0);
    win = wk->msgWin;
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(win));

    wk->menuWin[0] = BmpWin_CreateDynamic(2, 21, 15, 10, 4, 13, 1);
    win = wk->menuWin[0];
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(win));

    for (i = 0; i < MYPOKE_INFOWIN_NUM; i++) {
        wk->infoWin[i] = BmpWin_CreateDynamic(3, data_ov135_0220351c[i].x, data_ov135_0220351c[i].y,
            data_ov135_0220351c[i].w, data_ov135_0220351c[i].h, 13, 1);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->infoWin[i]), 0);
        win = wk->infoWin[i];
        BmpWin_FlushChar(win);
        BmpWin_FlushMap(win);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(win));
    }

    func_ov135_02200bac(wk, 2);
}


static void func_ov135_021fc4e4(WorldTradeWork *wk) {
    s32 i;

    func_ov135_02202940(&wk->print);
    BmpWin_Free(wk->explainWin);
    BmpWin_Free(wk->msgWin);
    BmpWin_Free(wk->menuWin[0]);
    i = 0;
    do {
        BmpWin_Free(wk->infoWin[i]);
        i = i + 1;
    } while (i < 0xe);
}

void func_ov135_021fc528(WorldTradeWork *wk) {
    StrBuf *pSVar1;
    s32 i;

    pSVar1 = GFL_StrBufCreate(0xb4, 0x48);
    wk->talkString = pSVar1;
    pSVar1 = GFL_MsgDataLoadStrbufNew(wk->msgManager, 0x2d);
    wk->titleString = pSVar1;
    i = 0;
    do {
        pSVar1 = GFL_StrBufCreate(0x14, 0x48);
        wk->infoString[i] = pSVar1;
        i = i + 1;
    } while (i < 10);
}

static void func_ov135_021fc568(WorldTradeWork *wk) {
    s32 i;

    i = 0;
    do {
        GFL_StrBufFree(wk->infoString[i]);
        i = i + 1;
    } while (i < 10);
    GFL_StrBufFree(wk->talkString);
    GFL_StrBufFree(wk->titleString);
}

// The windows of the Pokémon's information
// pokebw2's sMyPokeInfoWinPos

// pokebw2's MyPoke_SubSeqStart
static int func_ov135_021fc598(WorldTradeWork *wk) {
    wk->subprocessSeq = MYPOKE_SEQ_MAIN;
    return WT_SEQ_MAIN;
}

static int func_ov135_021fc5a0(WorldTradeWork *wk) {
    PartyPkm *pkm = func_0200adb4(wk->param->worldtrade_data);

    if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) || func_02035c60()) {
        func_ov135_021fc810(wk, 5, 1, 0, 0xf0f, pkm);
        func_ov135_021f4e98(wk, 3, 7);
        GFL_SndSEPlay(0x54c);
    } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        wk->subprocessSeq = MYPOKE_SEQ_END;
        func_ov135_021f4ec0(wk, 1, 0);
        GFL_SndSEPlay(0x551);
    }
    return WT_SEQ_MAIN;
}


// pokebw2's MyPoke_SubSeqEnd
static int func_ov135_021fc610(WorldTradeWork *wk) {
    GFL_WipeSet(3, 0, 0, 0, 6, 1, HEAPID_WORLDTRADE);
    wk->subprocessSeq = MYPOKE_SEQ_START;
    return WT_SEQ_FADEOUT;
}

static int func_ov135_021fc638(WorldTradeWork *wk) {
    func_ov135_021f4bf8(wk, 0x14, 0x16a, 8, 2, 1);
    func_ov135_021f5060();
    func_ov135_021f5034(1);
    wk->subprocessSeq = 6;
    return 2;
}

static int func_ov135_021fc668(WorldTradeWork *wk) {
    int ret = func_ov135_021f4d14(wk);

    if (ret == 1) {
        func_ov135_021f4ce4(wk);
        wk->subprocessSeq = MYPOKE_SEQ_END;
        wk->subOutFlag = 1;
        func_ov135_021f4ec0(wk, 7, 8);
        func_ov135_021fcbf0(wk);
    } else if (ret == 2) {
        func_ov135_021f4ce4(wk);
        wk->subprocessSeq = MYPOKE_SEQ_START;
        func_0201f7a8(wk->msgWin, 0);
        func_ov135_021fcbf0(wk);
    }
    return WT_SEQ_MAIN;
}


static int func_ov135_021fc6b8(WorldTradeWork *wk) {
    ListMenuOption *pLVar1;

    pLVar1 = ListMenuCore_CreateOptionList(2, 0x48);
    wk->menuList = pLVar1;
    ListMenuCore_AppendMsgOption(wk->menuList, wk->msgManager, 0x3d, 1, 0x48);
    ListMenuCore_AppendMsgOption(wk->menuList, wk->msgManager, 0x3e, 2, 0x48);
    func_ov135_021f4d50(wk, 2, 2, 0x14);
    func_ov135_021f5034(1);
    wk->subprocessSeq = 8;
    return 2;
}

int func_ov135_021fc70c(WorldTradeWork *wk) {
    PartyPkm *pkm;
    int ret = func_ov135_021f4e30(wk);

    if (ret == 1) {
        func_ov135_021f4e64(wk);
        ListMenuCore_FreeOptionList(wk->menuList);
        pkm = func_0200adb4(wk->param->worldtrade_data);
        if (func_ov135_021f6de8(pkm) && PokeParty_GetPkmCount(wk->param->myparty) == 6) {
            func_ov135_021fc810(wk, 0x2a, 1, 0, 0xf0f, pkm);
            func_ov135_021f4e98(wk, MYPOKE_SEQ_MESSAGE_WAIT, MYPOKE_SEQ_MAIN);
            return WT_SEQ_MAIN;
        }
        func_ov135_021fc810(wk, 6, 1, 0, 0xf0f, pkm);
        func_ov135_021f4e98(wk, MYPOKE_SEQ_MESSAGE_WAIT, MYPOKE_SEQ_YESNO);
        func_ov135_021fcbf0(wk);
    } else if (ret == 2 || ret == BMPMENULIST_CANCEL) {
        func_ov135_021f4e64(wk);
        ListMenuCore_FreeOptionList(wk->menuList);
        wk->subprocessSeq = MYPOKE_SEQ_END;
        func_ov135_021f4ec0(wk, 1, 0);
        func_ov135_021fcbf0(wk);
    }
    return WT_SEQ_MAIN;
}


static int func_ov135_021fc7c4(WorldTradeWork *wk) {
    BOOL BVar1;

    BVar1 = func_ov135_02202840(&wk->print);
    if (BVar1 == 0) {
        wk->subprocessSeq = wk->subprocessNextSeq;
    }
    return 2;
}

static int func_ov135_021fc7e0(WorldTradeWork *wk) {
    BOOL BVar1;
    s32 val;

    BVar1 = func_ov135_02202840(&wk->print);
    if ((BVar1 == 0) &&(val = wk->wait + 1, wk->wait = val, 0x2d < val)) {
        wk->wait = 0;
        wk->subprocessSeq = wk->subprocessNextSeq;
    }
    return 2;
}

static void func_ov135_021fc810(WorldTradeWork *wk, int msgNo, int wait, int flag, u16 dat, PartyPkm *pkm) {
    StrBuf *str;
    BmpWin *win;

    setPartyPokemonSpeciesNameToStrbuf(wk->wordSet, 0, pkm);
    str = GFL_MsgDataLoadStrbufNew(wk->msgManager, msgNo);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->talkString, str);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->msgWin), 15);
    BmpWin_DrawFrame(wk->msgWin, 0, 1, 14);
    win = wk->msgWin;
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(win));
    func_ov135_02202858(wk->msgWin, 0, wk->talkString, 0, 0, &wk->print);
    GFL_StrBufFree(str);
}


// The color of the gender mark
// pokebw2's MyPoke_SexMarkColor
u16 func_ov135_021fc8a8(int sex) {
    switch (sex) {
    case 0:
        return 0;
    case 1:
        return 0x14c0;
    case 2:
        return 0xc80;
    }
    return 0;
}

void func_ov135_021fc8cc(MsgData *msgManager, MsgData *monsNameManager, WordSet *wordSet, BmpWin **win,
                                BoxPkm *pkm, Dpw_Tr_PokemonDataSimple *post, WorldTradePrint *print) {
    StrBuf *strbuf;
    StrBuf *sexbuf;
    StrBuf *levelLabel;
    StrBuf *levelbuf;
    StrBuf *itemLabel;
    StrBuf *nameLabel;
    StrBuf *namebuf = GFL_StrBufCreate(26, HEAPID_WORLDTRADE);
    StrBuf *itembuf = GFL_StrBufCreate(38, HEAPID_WORLDTRADE);
    int sex;
    int level;
    int itemNo;
    int i;
    int monsno;
    int showSex;

    PML_PkmGetParam(pkm, PKM_PARAM_NICKNAME, namebuf);
    monsno = post->characterNo;
    sex = post->gender;
    level = post->level;
    itemNo = PML_PkmGetParam(pkm, PKM_PARAM_ITEM, NULL);
    showSex = PML_PkmGetParam(pkm, 0xad, NULL);

    itemLabel = GFL_MsgDataLoadStrbufNew(msgManager, 0x4f);
    sexbuf = GFL_MsgDataLoadStrbufNew(msgManager, data_ov135_02202eb0[sex]);
    levelLabel = GFL_MsgDataLoadStrbufNew(msgManager, 0x70);
    WordSetNumber(wordSet, 3, level, 3, 0, TRUE);
    levelbuf = func_ov135_02202670(wordSet, msgManager, 0x71, HEAPID_WORLDTRADE);
    strbuf = GFL_MsgDataLoadStrbufNew(monsNameManager, monsno);
    setItemNameToStrbuf(itembuf, itemNo, HEAPID_WORLDTRADE);
    nameLabel = GFL_MsgDataLoadStrbufNew(msgManager, 0x41);

    for (i = 0; i < 6; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(win[i]), 0);
    }

    func_ov135_021f91a0(win[0], namebuf, 0, 0, 0, 0x440, print);
    if (sex != 3 && showSex) {
        func_ov135_021f91a0(win[0], sexbuf, 64, 0, 0, func_ov135_021fc8a8(sex), print);
    }
    func_ov135_021f91a0(win[1], strbuf, 1, 0, 0, 0x440, print);
    func_ov135_021f91a0(win[2], levelLabel, 0, 0, 0, 0x440, print);
    func_ov135_021f91a0(win[3], levelbuf, 0, 0, 0, 0x440, print);
    func_ov135_021f91a0(win[4], itemLabel, 0, 0, 0, 0x3c40, print);
    func_ov135_021f91a0(win[5], itembuf, 1, 0, 0, 0x440, print);
    func_ov135_021f91a0(win[6], nameLabel, 0, 0, 0, 0x3c40, print);

    GFL_StrBufFree(itemLabel);
    GFL_StrBufFree(itembuf);
    GFL_StrBufFree(levelLabel);
    GFL_StrBufFree(levelbuf);
    GFL_StrBufFree(sexbuf);
    GFL_StrBufFree(namebuf);
    GFL_StrBufFree(strbuf);
    GFL_StrBufFree(nameLabel);
}


void func_ov135_021fca8c(MsgData *param_1, BmpWin **param_2, u16 *param_3, PartyPkm *param_4, BmpWin **param_5, WorldTradePrint *param_6) {
    StrBuf *strbuf;
    StrBuf *str;
    StrBuf *str_00;
    StrBuf *str_01;

    strbuf = GFL_StrBufCreate(0x22, 0x48);
    str = GFL_StrBufCreate(0x22, 0x48);
    str_00 = GFL_MsgDataLoadStrbufNew(param_1, 0x37);
    GFL_StrBufLoadString(strbuf, param_3);
    str_01 = GFL_MsgDataLoadStrbufNew(param_1, 0xc2);
    PokeParty_GetParam(param_4, 0x8d, str);
    func_ov135_021f91a0(*param_2, str_00, 0, 0, 0, 0x3c40, param_6);
    func_ov135_021f91a0(param_2[1], strbuf, 0, 0, 0, 0x440, param_6);
    func_ov135_021f91a0(*param_5, str_01, 0, 0, 0, 0x3c40, param_6);
    func_ov135_021f91a0(param_5[1], str, 1, 0, 0, 0x440, param_6);
    GFL_StrBufFree(str_00);
    GFL_StrBufFree(strbuf);
    GFL_StrBufFree(str_01);
    GFL_StrBufFree(str);
}

void func_ov135_021fcb54(PartyPkm *param_1) {
    BoxPkm *pkm;
    void *ptr;
    u32 arcId;
    u32 species;
    u32 form;
    u32 sex;
    BOOL rare;
    NNSG2dCharacterData *pNStack_18;

    pkm = func_020185f0(param_1);
    ptr = func_0202d6f4(&pNStack_18, pkm, 0, 0x48);
    DC_FlushRange(pNStack_18->pRawData, 0x1200);
    GX_LoadOBJ(pNStack_18->pRawData, 0x3f80, 0x1200);
    GFL_HeapFree(ptr);
    arcId = GetPokemonGraphicsARCID();
    species = PokeParty_GetParam(param_1, 5, (void *)0x0);
    form = PokeParty_GetParam(param_1, 0x6f, (void *)0x0);
    sex = PokeParty_GetParam(param_1, 0x6e, (void *)0x0);
    rare = PokeParty_IsRare(param_1);
    arcId = GetPokemonPaletteDataNo(arcId, species, form, sex, rare, 0, 0);
    species = GetPokemonGraphicsARCID();
    GFL_BGSysLoadNCLRDefault(species, arcId, 1, 0x1a0, 0x20, 0x48);
}

void func_ov135_021fcbf0(WorldTradeWork *wk) {
    func_ov135_021f8290(wk->msgManager, wk->monsNameManager, wk->wordSet, &wk->infoWin[9],
            (s32)wk->uploadData.wantSimple.characterNo, (s32)wk->uploadData.wantSimple.gender, func_ov135_021f886c((s32)wk->uploadData.wantSimple.level_min, (s32)wk->uploadData.wantSimple.level_max, 0),
            &wk->print);
}
