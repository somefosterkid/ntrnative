// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov214/worldtrade_partner.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/species.h"
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
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "pml/poke_party.h"
#include "system/bmp_winframe.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "worldtrade_local.h"
#include "constants/sound.h"

// The Global Trade Station's screen of a Pokémon that a search found, with its trainer, where the player can offer
// a Pokémon for it. The file's name is a guess, as the ROM doesn't give it, and so are the names of its functions

enum {
    PARTNER_SEQ_START,
    PARTNER_SEQ_MAIN,
    PARTNER_SEQ_END,
    PARTNER_SEQ_MESSAGE_WAIT,
    PARTNER_SEQ_YESNO,
    PARTNER_SEQ_YESNO_SELECT,
    PARTNER_SEQ_PAGE_CHANGE,
    PARTNER_SEQ_RETURN_SCREEN1,
    PARTNER_SEQ_EXCHANGE_SCREEN2,
};

// The sub-process modes of the screens it goes to and comes from
#define SEARCH_MODE_PARTNER_RETURN 15
#define PARTNER_MODE_FROM_SEARCH 16
#define PARTNER_MODE_FROM_PARTNER 17
#define BOX_MODE_EXCHANGE_SELECT 6

// How many windows of information the screen has
#define PARTNER_INFOWIN_NUM 16

// Black 2's, not checked against Black
typedef struct {
    int x;
    int y;
    int w;
    int h;
    int bg;
} PartnerWinPos;

void func_ov135_021fcf28(int subBg1Offset);
void func_ov135_021fcfc8(void);
void func_ov135_021fcfd8(WorldTradeWork *wk);
void func_ov135_021fd08c(WorldTradeWork *wk);
void func_ov135_021fd0f4(WorldTradeWork *wk);
void func_ov135_021fd104(WorldTradeWork *wk);
void func_ov135_021fd1e0(WorldTradeWork *wk);
void func_ov135_021fd270(WorldTradeWork *wk);
void func_ov135_021fd2fc(WorldTradeWork *wk);
int func_ov135_021fd31c(WorldTradeWork *wk);
void func_ov135_021fd368(WorldTradeWork *wk, int result);
int func_ov135_021fd3cc(WorldTradeWork *wk);
int func_ov135_021fd4d4(WorldTradeWork *wk);
int func_ov135_021fd528(WorldTradeWork *wk);
int func_ov135_021fd550(WorldTradeWork *wk);
static void func_ov135_021fd5f0(WorldTradeWork *wk);
int func_ov135_021fd65c(WorldTradeWork *wk);
int func_ov135_021fd66c(WorldTradeWork *wk);
int func_ov135_021fd698(WorldTradeWork *wk);
int func_ov135_021fd6d8(WorldTradeWork *wk);
void func_ov135_021fd6f4(WorldTradeWork *wk, u32 param_2, u32 param_3, u32 param_4, u32 param_5);
void func_ov135_021fd760(MsgData *msgManager, BmpWin **win, int msg, WorldTradePrint *print, u16 color);
void func_ov135_021fd790(MsgData *msgManager, BmpWin **win, int msg, WorldTradePrint *print);
void func_ov135_021fd7dc(BmpWin **win, MsgData *msgManager, MsgData *monsNameManager, Dpw_Tr_PokemonSearchData *want,
                         WorldTradePrint *print);
void func_ov135_021fd85c(BmpWin **win, StrBuf *str1, StrBuf *str2, WorldTradePrint *print);
void func_ov135_021fd8bc(WorldTradeWork *wk);

// pokebw2's sPartnerSubSeqTable
static int (*data_ov135_02203d14[])(WorldTradeWork *wk) = {
    func_ov135_021fd31c,       func_ov135_021fd3cc,          func_ov135_021fd4d4,
    func_ov135_021fd6d8, func_ov135_021fd528,         func_ov135_021fd550,
    func_ov135_021fd65c,  func_ov135_021fd66c, func_ov135_021fd698,
};

void func_ov135_02202940(WorldTradePrint *print);

// pokebw2's Partner_BgInit



int func_ov135_021fcc40(WorldTradeWork *wk, int seq) {
    func_ov135_021fd270(wk);
    func_ov135_021fcf28(-32 - wk->drawOffset);
    if (GXx_GetMasterBrightness_(REG_DB_MASTER_BRIGHT_ADDR) == 0) {
        func_ov135_021fffe8(wk, -32 - wk->drawOffset, TRUE);
    } else {
        func_ov135_021fffe8(wk, -32 - wk->drawOffset, FALSE);
    }
    func_ov135_021fcfd8(wk);
    func_ov135_021fd104(wk);
    func_ov135_021fd08c(wk);
    func_ov135_021fc8cc(wk->msgManager, wk->monsNameManager, wk->wordSet, &wk->infoWin[0],
        func_0201d624((PartyPkm *)wk->downloadData[wk->unkD4].postData), &wk->downloadData[wk->unkD4].postSimple, &wk->print);
    func_ov135_021fca8c(wk->msgManager, &wk->infoWin[7], wk->downloadData[wk->unkD4].name,
        (PartyPkm *)wk->downloadData[wk->unkD4].postData, &wk->infoWin[12], &wk->print);
    func_ov135_021fcb54((PartyPkm *)wk->downloadData[wk->unkD4].postData);
    func_ov135_021fd760(wk->msgManager, &wk->infoWin[14], 0x53, &wk->print, 0x440);
    func_ov135_021fd760(wk->msgManager, &wk->infoWin[9], 0x57, &wk->print, 0x3c40);
    if (wk->partnerChange == 0) {
        func_ov135_021fd790(wk->msgManager, &wk->menuWin[0], 0x5e, &wk->print);
        func_ov135_021fd790(wk->msgManager, &wk->menuWin[1], 0x73, &wk->print);
    }
    func_ov135_021fd5f0(wk);
    func_ov135_021ffd1c(wk, wk->unkD4, -wk->drawOffset);
    wk->vfunc2 = func_ov135_021fd8bc;
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    func_0203d34c(TRUE, HEAPID_WORLDTRADE);
    if (wk->subProcessMode == PARTNER_MODE_FROM_PARTNER) {
        if (GXx_GetMasterBrightness_(REG_MASTER_BRIGHT_ADDR) == -16 && GXx_GetMasterBrightness_(REG_DB_MASTER_BRIGHT_ADDR) != -16) {
            GFL_WipeSet(3, 1, 1, 0, 6, 1, HEAPID_WORLDTRADE);
        } else if (GXx_GetMasterBrightness_(REG_MASTER_BRIGHT_ADDR) != -16 && GXx_GetMasterBrightness_(REG_DB_MASTER_BRIGHT_ADDR) == -16) {
            GFL_WipeSet(4, 1, 1, 0, 6, 1, HEAPID_WORLDTRADE);
        } else {
            GFL_WipeSet(0, 1, 1, 0, 6, 1, HEAPID_WORLDTRADE);
        }
    }
    wk->subprocessSeq = PARTNER_SEQ_START;
    wk->unk12E4 = 0;
    wk->partnerChange = 0;
    return WT_SEQ_FADEIN;
}

int func_ov135_021fce64(WorldTradeWork *wk, int seq) {
    int ret;
    int i;

    ret = data_ov135_02203d14[wk->subprocessSeq](wk);
    for (i = 0; i < 8; i++) {
        func_ov135_021f4ea0(wk->subAct[i], wk->subActPos[i].x, wk->subActPos[i].y + wk->drawOffset + 0x20);
    }
    func_ov135_021f4ff4(wk->pokemonAct, 0xd0, 0x3a - wk->drawOffset);
    func_ov135_021ffd1c(wk, wk->unkD4, wk->drawOffset);
    return ret;
}

int func_ov135_021fced4(WorldTradeWork *wk, int seq) {
    wk->vfunc2 = NULL;
    func_ov135_021fd0f4(wk);
    func_ov135_021fd2fc(wk);
    func_ov135_021fd1e0(wk);
    func_ov135_021fcfc8();
    func_ov135_022000e4(wk);
    func_0204b3dc(wk->promptDsAct, FALSE);
    func_ov135_021f4ec8(wk);
    if (wk->partnerChange == 0) {
        func_0203d34c(FALSE, HEAPID_WORLDTRADE);
    }
    return 0;
}

// pokebw2's sPartnerTouchRects
static const TouchRect data_ov135_022035fc[] = {
    { 162, 191, 3, 125 },
    { 162, 191, 130, 253 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

// pokebw2's sPartnerInfoWinPos
static const PartnerWinPos data_ov135_02203668[PARTNER_INFOWIN_NUM] = {
#include "asset/worldtrade_partner_data_ov135_02203668.inc"
};

// The trainer that left and right go to, from each of the found trainers
// pokebw2's sPartnerKeyTbl
static const u8 data_ov135_02203608[SEARCH_POKE_MAX][2] = {
#include "asset/worldtrade_partner_data_ov135_02203608.inc"
};

void func_ov135_021fcf28(int subBg1Offset) {
    {
        BGSysLCDConfig config = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };
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
    GFL_BGSysClearCharCore(2, 32, 0, HEAPID_WORLDTRADE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG3, FALSE);
}

// pokebw2's Partner_BgExit
void func_ov135_021fcfc8(void) {
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(2);
}

void func_ov135_021fcfd8(WorldTradeWork *wk) {
    ArcTool *arc;

    GFL_BGSysLoadNCLRDefault(0x17, 5, 0, 0x1a0, 0x20, 0x48);
    LoadSysMsgBox(4, 1, 2, 0, 0x48);
    arc = GFL_ArcSysCreateFileHandle(0x60, 0x48);
    GFL_BGSysLoadArcNCGRStatic(arc, 0x10, 1, 0, 0xc00, 1, 0x48);
    GFL_G2DIOLoadArcNCLRDefault(arc, 6, 0, 0, 0x60, 0x48);
    if (wk->partnerChange == 0) {
        GFL_BGSysLoadArcNCGRStatic(arc, 0xf, 6, 0, 0, 0, 0x48);
        loadBGScrToVramByFileNoReserveNegAlign(arc, 0x23, 6, 0, 0, 0, 0x48);
    }
    GFL_ArcToolFree(arc);
    if (wk->partnerChange == 0) {
        GFL_BGSysLoadNCLRDefault(0x17, 5, 4, 0x20, 0x20, 0x48);
    }
}

void func_ov135_021fd08c(WorldTradeWork *wk) {
    ClActor *actor;
    ClActorSetup setup;

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.x = 0xd0;
    setup.y = 0x3a;
    actor = func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CHAR],
            wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_PLTT], wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CELL],
            &setup, 0, 0x48);
    wk->pokemonAct = actor;
    func_0204b92c(actor, 1);
    func_0204b878(wk->pokemonAct, 0x24);
    func_0204b3dc(wk->pokemonAct, 1);
    func_0203d34c(0, 0x48);
}

void func_ov135_021fd0f4(WorldTradeWork *wk) {
    func_0204b3b4(wk->pokemonAct);
}

void func_ov135_021fd104(WorldTradeWork *wk) {
    int i;

    wk->msgWin = BmpWin_CreateDynamic(4, 2, 19, 27, 4, 1, TRUE);
    if (wk->partnerChange == 0) {
        wk->menuWin[0] = BmpWin_CreateDynamic(4, 1, 21, 13, 2, 1, TRUE);
        wk->menuWin[1] = BmpWin_CreateDynamic(4, 17, 21, 13, 2, 1, TRUE);
    }
    for (i = 0; i < PARTNER_INFOWIN_NUM; i++) {
        if (data_ov135_02203668[i].bg == 2) {
            wk->infoWin[i] = BmpWin_CreateDynamic(data_ov135_02203668[i].bg, data_ov135_02203668[i].x,
                data_ov135_02203668[i].y, data_ov135_02203668[i].w, data_ov135_02203668[i].h, 13, TRUE);
        } else {
            wk->infoWin[i] = BmpWin_CreateDynamic(data_ov135_02203668[i].bg, data_ov135_02203668[i].x,
                data_ov135_02203668[i].y, data_ov135_02203668[i].w, data_ov135_02203668[i].h, 1, TRUE);
        }
        BmpWin_Transfer(wk->infoWin[i]);
    }
}


void func_ov135_021fd1e0(WorldTradeWork *wk) {
    u8 bg;
    s32 i;
    BmpWin *window;

    func_ov135_02202940(&wk->print);
    BmpWin_Free(wk->msgWin);
    if (wk->partnerChange == 0) {
        window = wk->menuWin[1];
        func_020456f8(window);
        bg = BmpWin_GetBGIndex(window);
        GFL_BGSysLoadScr(bg);
        window = wk->menuWin[0];
        func_020456f8(window);
        bg = BmpWin_GetBGIndex(window);
        GFL_BGSysLoadScr(bg);
        BmpWin_Free(wk->menuWin[1]);
        BmpWin_Free(wk->menuWin[0]);
    }
    for (i = 0; i < PARTNER_INFOWIN_NUM; i++) {
        window = wk->infoWin[i];
        func_020456f8(window);
        bg = BmpWin_GetBGIndex(window);
        GFL_BGSysLoadScr(bg);
        BmpWin_Free(wk->infoWin[i]);
    }
}

void func_ov135_021fd270(WorldTradeWork *wk) {
    StrBuf *str;
    Dpw_Tr_Data *data;

    data = &wk->downloadData[wk->unkD4];
    str = GFL_StrBufCreate(0xb4, 0x48);
    wk->talkString = str;
    GFL_WordSetClearAll(wk->wordSet);
    if (data->countryCode != 0) {
        loadCountryToStrbuf(wk->wordSet, 8, data->countryCode);
    }
    if (data->localCode != 0) {
        loadCountryAreaToStrbuf(wk->wordSet, 9, data->countryCode, data->localCode);
    }
    str = func_ov135_02202670(wk->wordSet, wk->msgManager, 0x58, 0x48);
    wk->infoString[0] = str;
    str = func_ov135_02202670(wk->wordSet, wk->msgManager, 0x59, 0x48);
    wk->infoString[1] = str;
}

void func_ov135_021fd2fc(WorldTradeWork *wk) {
    GFL_StrBufFree(wk->infoString[0]);
    GFL_StrBufFree(wk->infoString[1]);
    GFL_StrBufFree(wk->talkString);
}

// The windows of the Pokémon's and its trainer's information

// pokebw2's Partner_SubSeqStart
int func_ov135_021fd31c(WorldTradeWork *wk) {
    func_0204b3dc(wk->promptDsAct, FALSE);
    if (wk->subProcessMode == PARTNER_MODE_FROM_SEARCH) {
        GFL_WipeSet(0, 1, 1, 0, 16, 1, HEAPID_WORLDTRADE);
        wk->subprocessSeq = PARTNER_SEQ_RETURN_SCREEN1;
    } else {
        wk->subprocessSeq = PARTNER_SEQ_MAIN;
        func_0204b3dc(wk->partnerCursorAct, TRUE);
    }
    return WT_SEQ_MAIN;
}

void func_ov135_021fd368(WorldTradeWork *wk, int result) {
    if (result != wk->unkD4 && result >= 0) {
        func_0204b878(wk->subAct[result + 1], result * 4 + 16);
        wk->subprocessSeq = PARTNER_SEQ_END;
        func_ov135_021f4ec0(wk, WORLDTRADE_PARTNER, 0x11);
        wk->unkD4 = result;
        func_ov135_021ffd1c(wk, result, 0);
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        wk->unk12E4 = 1;
        wk->partnerChange = 1;
    }
}

int func_ov135_021fd3cc(WorldTradeWork *wk) {
    int ret;

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        func_ov135_021fd6f4(wk, 0x10, 1, 0, 0xf0f);
        func_ov135_021f4e98(wk, PARTNER_SEQ_MESSAGE_WAIT, PARTNER_SEQ_YESNO);
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
    } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        GFL_WipeSet(0, 0, 0, 0, 16, 1, HEAPID_WORLDTRADE);
        wk->subprocessSeq = PARTNER_SEQ_EXCHANGE_SCREEN2;
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
    } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_LEFT) {
        if (wk->unkD4 != data_ov135_02203608[wk->unkD4][0] && wk->searchResult >= data_ov135_02203608[wk->unkD4][0] + 1) {
            func_ov135_021fd368(wk, data_ov135_02203608[wk->unkD4][0]);
        }
    } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_RIGHT) {
        if (wk->unkD4 != data_ov135_02203608[wk->unkD4][1] && wk->searchResult >= data_ov135_02203608[wk->unkD4][1] + 1) {
            func_ov135_021fd368(wk, data_ov135_02203608[wk->unkD4][1]);
        }
    } else {
        ret = func_ov135_021ffb00(wk->searchResult);
        if (ret != -1) {
            func_ov135_021fd368(wk, ret);
        }
        ret = func_02035c08(data_ov135_022035fc);
        if (ret == 0) {
            func_ov135_021fd6f4(wk, 0x10, 1, 0, 0xf0f);
            func_ov135_021f4e98(wk, PARTNER_SEQ_MESSAGE_WAIT, PARTNER_SEQ_YESNO);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        } else if (ret == 1) {
            GFL_WipeSet(0, 0, 0, 0, 16, 1, HEAPID_WORLDTRADE);
            wk->subprocessSeq = PARTNER_SEQ_EXCHANGE_SCREEN2;
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
        }
    }
    return WT_SEQ_MAIN;
}



// The two buttons: the Pokémon's trainer and Back

// pokebw2's Partner_SubSeqEnd
int func_ov135_021fd4d4(WorldTradeWork *wk) {
    func_0204b3dc(wk->partnerCursorAct, FALSE);
    if (wk->subProcessMode != SEARCH_MODE_PARTNER_RETURN) {
        if (wk->subProcessMode == BOX_MODE_EXCHANGE_SELECT) {
            GFL_WipeSet(0, 0, 0, 0, 6, 1, HEAPID_WORLDTRADE);
        } else {
            GFL_WipeSet(3, 0, 0, 0, 6, 1, HEAPID_WORLDTRADE);
        }
    }
    wk->subprocessSeq = PARTNER_SEQ_START;
    return WT_SEQ_FADEOUT;
}

// pokebw2's Partner_SubSeqYesNo
int func_ov135_021fd528(WorldTradeWork *wk) {
    func_ov135_021f4bf8(wk, 18, 0x12e, 3, 4, TRUE);
    wk->subprocessSeq = PARTNER_SEQ_YESNO_SELECT;
    return WT_SEQ_MAIN;
}

int func_ov135_021fd550(WorldTradeWork *wk) {
    u8 result;

    result = func_ov135_021f4d14(wk);
    if (result == 1) {
        func_ov135_021f4ce4(wk);
        wk->subprocessSeq = 2;
        func_ov135_021f4ec0(wk, 5, 6);
        func_ov135_021fd760(wk->msgManager, &wk->infoWin[14], 0x53, &wk->print, 0x440);
        func_ov135_021fd760(wk->msgManager, &wk->infoWin[9], 0x57, &wk->print, 0x3c40);
        func_ov135_021fd5f0(wk);
    }
    else if (result == 2) {
        func_ov135_021f4ce4(wk);
        func_0201f7a8(wk->msgWin, 0);
        wk->subprocessSeq = 1;
        func_ov135_021fd790(wk->msgManager, wk->menuWin, 0x5e, &wk->print);
        func_ov135_021fd790(wk->msgManager, &wk->menuWin[1], 0x73, &wk->print);
    }
    return 2;
}

static void func_ov135_021fd5f0(WorldTradeWork *wk) {
    loadBGScrToVramByNarcNoReserveNegAlign(0x60, 0x1e, 1, 0, 0x800, 1, 0x48);
    func_ov135_021fd85c(&wk->infoWin[10], wk->infoString[0], wk->infoString[1], &wk->print);
    func_ov135_021fd7dc(&wk->infoWin[15], wk->msgManager, wk->monsNameManager, &wk->downloadData[wk->unkD4].wantSimple,
            &wk->print);
}

// pokebw2's Partner_SubSeqPageChange
int func_ov135_021fd65c(WorldTradeWork *wk) {
    func_ov135_021fd5f0(wk);
    wk->subprocessSeq = PARTNER_SEQ_MAIN;
    return WT_SEQ_MAIN;
}

// Slides the screens in from the search screen
// pokebw2's Partner_SubSeqReturnScreen1
int func_ov135_021fd66c(WorldTradeWork *wk) {
    wk->drawOffset++;
    if (GFL_WipeIsFinished()) {
        wk->drawOffset = 0;
        wk->subprocessSeq = PARTNER_SEQ_MAIN;
        func_0204b3dc(wk->partnerCursorAct, TRUE);
    }
    return WT_SEQ_MAIN;
}

// Slides the screens back to the search screen
// pokebw2's Partner_SubSeqExchangeScreen2
int func_ov135_021fd698(WorldTradeWork *wk) {
    wk->drawOffset--;
    if (GFL_WipeIsFinished()) {
        GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
        wk->subprocessSeq = PARTNER_SEQ_END;
        func_ov135_021f4ec0(wk, WORLDTRADE_SEARCH, SEARCH_MODE_PARTNER_RETURN);
        wk->drawOffset = 16;
    }
    return WT_SEQ_MAIN;
}

int func_ov135_021fd6d8(WorldTradeWork *wk) {
    BOOL BVar1;

    BVar1 = func_ov135_02202840(&wk->print);
    if (BVar1 == 0) {
        wk->subprocessSeq = wk->subprocessNextSeq;
    }
    return 2;
}

void func_ov135_021fd6f4(WorldTradeWork *wk, u32 param_2, u32 param_3, u32 param_4, u32 param_5) {
    u8 bg;
    GFLBitmap *bitmap;
    BmpWin *window;

    GFL_MsgDataLoadStrbuf(wk->msgManager, param_2, wk->talkString);
    bitmap = BmpWin_GetBitmap(wk->msgWin);
    GFL_BitmapFill(bitmap, 0xf);
    BmpWin_DrawFrame(wk->msgWin, 0, 1, 2);
    func_ov135_02202858(wk->msgWin, 0, wk->talkString, 0, 0, &wk->print);
    window = wk->msgWin;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    bg = BmpWin_GetBGIndex(window);
    GFL_BGSysLoadScr(bg);
}

void func_ov135_021fd760(MsgData *msgManager, BmpWin **win, int msg, WorldTradePrint *print, u16 color) {
    StrBuf *str;

    str = GFL_MsgDataLoadStrbufNew(msgManager, msg);
    func_ov135_021f91a0(*win, str, 0, 2, 0, color, print);
    GFL_StrBufFree(str);
}

// pokebw2's Partner_TouchPrint
void func_ov135_021fd790(MsgData *msgManager, BmpWin **win, int msg, WorldTradePrint *print) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(msgManager, msg);
    BmpWin *window;

    func_ov135_021f91dc(win[0], str, 0, 0, 1, 0x3c40, print);
    window = win[0];
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
    GFL_StrBufFree(str);
}

void func_ov135_021fd7dc(BmpWin **win, MsgData *msgManager, MsgData *monsNameManager, Dpw_Tr_PokemonSearchData *want,
                         WorldTradePrint *print) {
    GFLBitmap *bitmap;

    bitmap = BmpWin_GetBitmap(*win);
    GFL_BitmapFill(bitmap, 0);
    func_ov135_021f7fd4(*win, monsNameManager, want->characterNo, 0, 0x440, print);
    func_ov135_021f8104(*win, msgManager, want->gender, 0, 0x49, 0, 0x440, print);
    func_ov135_021f8178(*win, msgManager, func_ov135_021f886c(want->level_min, want->level_max, 0), 0, 0x68, 0, 0x440, 0,
            print);
}

void func_ov135_021fd85c(BmpWin **win, StrBuf *str1, StrBuf *str2, WorldTradePrint *print) {
    GFLBitmap *bitmap;

    bitmap = BmpWin_GetBitmap(win[0]);
    GFL_BitmapFill(bitmap, 0);
    bitmap = BmpWin_GetBitmap(win[1]);
    GFL_BitmapFill(bitmap, 0);
    if (str1 != NULL) {
        func_ov135_021f91a0(win[0], str1, 0, 0, 0, 0x440, print);
    }
    if (str2 != NULL) {
        func_ov135_021f91a0(win[1], str2, 0, 0, 0, 0x440, print);
    }
}

// pokebw2's Partner_SlideScreenVFunc
void func_ov135_021fd8bc(WorldTradeWork *wk) {
    GFL_BGSysMoveBG(0, BG_MOVE_SET_Y, wk->drawOffset);
    GFL_BGSysMoveBG(1, BG_MOVE_SET_Y, wk->drawOffset);
    GFL_BGSysMoveBG(2, BG_MOVE_SET_Y, wk->drawOffset);
    GFL_BGSysMoveBG(3, BG_MOVE_SET_Y, wk->drawOffset);
    GFL_BGSysMoveBG(4, BG_MOVE_SET_Y, -wk->drawOffset);
    GFL_BGSysMoveBG(5, BG_MOVE_SET_Y, -32 - wk->drawOffset);
    GFL_BGSysMoveBG(6, BG_MOVE_SET_Y, -wk->drawOffset);
    GFL_BGSysMoveBG(7, BG_MOVE_SET_Y, -wk->drawOffset);
}
