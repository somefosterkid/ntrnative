// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov214/worldtrade_upload.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "text/system/ability_handlers_btl_main.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "constants/version.h"
#include "dpw/dpw_tr.h"
#include "gfl/nhttp_rap.h"
#include "dwc/dwc.h"
#include "field/unity_tower.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_state.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/rtc.h"
#include "pml/item.h"
#include "pml/mail.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "save/chatter.h"
#include "save/medal_box.h"
#include "save/player_info.h"
#include "save/pokedex.h"
#include "save/records.h"
#include "save/save_control.h"
#include "save/wifi_list.h"
#include "save/worldtrade_data.h"
#include "system/bmp_winframe.h"
#include "system/country_region.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "worldtrade_local.h"
#include "app/worldtrade.h"
#include "battle/btl_main.h"
#include "app/wifi_p2pmatchroom.h"

typedef struct WorldTradeUploadPokemonData {
    char name[8];
} WorldTradeUploadPokemonData;

void func_ov135_021fffe8(WorldTradeWork *wk, int a2, BOOL a3);
void func_ov135_021ffd44(WorldTradeWork *wk);
void func_ov135_021f9254(WorldTradeWork *wk);
void func_ov135_021ffd74(WorldTradeWork *wk);
void func_ov135_022000e4(WorldTradeWork *wk);
void func_ov135_02200ab4(WorldTradeWork *wk);
void func_ov135_02200b34(WorldTradeWork *wk);
void func_ov135_02202940(WorldTradePrint *print);
void func_ov135_021f4e98(WorldTradeWork *wk, int a2, int a3);
void func_ov135_021f4fa4(WorldTradeWork *wk);

// The Global Trade Station's server exchanges: depositing a Pokémon, taking it back, trading for a Pokémon found by a
// search, checking whether the deposited Pokémon was traded, and the saves after each. The file's name is a guess, as
// the ROM doesn't give it, and so are the names of its functions

// The steps of the screen
enum {
    UPLOAD_SEQ_START,
    UPLOAD_SEQ_MAIN,
    UPLOAD_SEQ_EVIL_CHECK_START,
    UPLOAD_SEQ_EVIL_CHECK_RESULT,
    UPLOAD_SEQ_NAME_CHECK_START,
    UPLOAD_SEQ_NAME_CHECK_WAIT,
    UPLOAD_SEQ_UPLOAD_START,
    UPLOAD_SEQ_UPLOAD_RESULT,
    UPLOAD_SEQ_UPLOAD_FINISH,
    UPLOAD_SEQ_UPLOAD_FINISH_RESULT,
    UPLOAD_SEQ_UPLOAD_SUCCESS_MESSAGE,
    UPLOAD_SEQ_DOWNLOAD_START,
    UPLOAD_SEQ_DOWNLOAD_RESULT,
    UPLOAD_SEQ_DOWNLOAD_FINISH,
    UPLOAD_SEQ_DOWNLOAD_FINISH_RESULT,
    UPLOAD_SEQ_DOWNLOAD_SUCCESS_MESSAGE,
    UPLOAD_SEQ_EXCHANGE_START,
    UPLOAD_SEQ_EXCHANGE_RESULT,
    UPLOAD_SEQ_EXCHANGE_FINISH,
    UPLOAD_SEQ_EXCHANGE_FINISH_RESULT,
    UPLOAD_SEQ_EXCHANGE_SUCCESS_MESSAGE,
    UPLOAD_SEQ_EXCHANGE_FAILED_MESSAGE,
    UPLOAD_SEQ_DOWNLOAD_EX_START,
    UPLOAD_SEQ_DOWNLOAD_EX_FINISH,
    UPLOAD_SEQ_DOWNLOAD_EX_FINISH_RESULT,
    UPLOAD_SEQ_DOWNLOAD_EX_SUCCESS_MESSAGE,
    UPLOAD_SEQ_SERVER_POKE_DELETE,
    UPLOAD_SEQ_SERVER_POKE_DELETE_WAIT,
    UPLOAD_SEQ_SERVER_TRADE_CHECK,
    UPLOAD_SEQ_SERVER_TRADE_CHECK_RESULT,
    UPLOAD_SEQ_SERVER_DOWNLOAD,
    UPLOAD_SEQ_SERVER_DOWNLOAD_RESULT,
    UPLOAD_SEQ_SERVER_TRADE_CHECK_END,
    UPLOAD_SEQ_NOW_SAVE_MESSAGE,
    UPLOAD_SEQ_SAVE,
    UPLOAD_SEQ_SAVE_RANDOM_WAIT,
    UPLOAD_SEQ_SAVE_WAIT,
    UPLOAD_SEQ_SAVE_LAST,
    UPLOAD_SEQ_TIMEOUT_SAVE,
    UPLOAD_SEQ_TIMEOUT_SAVE_WAIT,
    UPLOAD_SEQ_END,
    UPLOAD_SEQ_MES_WAIT,
    UPLOAD_SEQ_MES_WAIT_BUTTON,
    UPLOAD_SEQ_ERROR_MESSAGE,
    UPLOAD_SEQ_ERROR_END,
    UPLOAD_SEQ_RETURN_TITLE_MESSAGE,
    UPLOAD_SEQ_CANCEL,
    UPLOAD_SEQ_CANCEL_WAIT,
    UPLOAD_SEQ_SERVER_SERVICE_END,
};

// What the screen was started for
#define UPLOAD_MODE_UPLOAD 7
#define UPLOAD_MODE_DOWNLOAD 8
#define UPLOAD_MODE_EXCHANGE 9
#define UPLOAD_MODE_DOWNLOAD_EX 10
#define UPLOAD_MODE_SERVER_CHECK 11
#define UPLOAD_MODE_POKEMON_EVO_SAVE 12

// How the trade demo is started
#define DEMO_MODE_UPLOAD 7
#define DEMO_MODE_DOWNLOAD 8
#define DEMO_MODE_EXCHANGE 9
#define DEMO_MODE_DOWNLOAD_EX 10

// How the connection screen is started to log in again
#define ENTER_MODE_RELOGIN 0x15

// The versions of the other color, whose trades earn a medal, and the name that replaces a bad one
#ifdef BLACK2
#define OTHER_COLOR VERSION_WHITE
#define OTHER_COLOR_2 VERSION_WHITE2
#else
#define OTHER_COLOR VERSION_BLACK
#define OTHER_COLOR_2 VERSION_BLACK2
#endif
#define DEFAULT_NAME_MSG AbilityHandlersBtlMain_Text_VersionName

// Frames to wait for the server
#define UPLOAD_TIMEOUT (30 * 60 * 2)

// The HTTP statuses of the server's check
#define HTTP_STATUS_OK 200
#define HTTP_STATUS_BAD_REQUEST 400
#define HTTP_STATUS_UNAUTHORIZED 401
#define HTTP_STATUS_REQUEST_TIMEOUT 408


static void func_ov135_02200ccc(void);
static void func_ov135_02200db8(void);
static void func_ov135_02200dd0(WorldTradeWork *wk);
static void func_ov135_02200e48(WorldTradeWork *wk);
void func_ov135_02200e94(WorldTradeWork *wk);
static void func_ov135_02200eb0(WorldTradeWork *wk);
static void func_ov135_02200ec8(WorldTradeWork *wk);
int func_ov135_02200ed8(WorldTradeWork *wk);
int func_ov135_02200ff4(WorldTradeWork *wk);
int func_ov135_022010bc(WorldTradeWork *wk);
int func_ov135_02201258(WorldTradeWork *wk);
int func_ov135_02201320(WorldTradeWork *wk);
int func_ov135_022013e4(WorldTradeWork *wk);
int func_ov135_02201414(WorldTradeWork *wk);
static int func_ov135_022014b8(WorldTradeWork *wk);
int func_ov135_022014d8(WorldTradeWork *wk);
static int func_ov135_02201554(WorldTradeWork *wk);
int func_ov135_0220157c(WorldTradeWork *wk);
static int func_ov135_0220163c(WorldTradeWork *wk);
int func_ov135_02201658(WorldTradeWork *wk);
static int func_ov135_022016cc(WorldTradeWork *wk);
static int func_ov135_0220171c(WorldTradeWork *wk);
static int func_ov135_022017f0(WorldTradeWork *wk);
int func_ov135_0220180c(WorldTradeWork *wk);
static int func_ov135_02201880(WorldTradeWork *wk);
int func_ov135_0220189c(WorldTradeWork *wk);
static int func_ov135_02201a34(WorldTradeWork *wk);
static int func_ov135_02201a48(WorldTradeWork *wk);
int func_ov135_02201a64(WorldTradeWork *wk);
void func_ov135_02201b18(WorldTradeWork *wk);
static void func_ov135_02201b44(WorldTradeWork *wk, PartyPkm *received, Dpw_Tr_Data *trData, PartyPkm *sent);
static int func_ov135_02201c08(WorldTradeWork *wk);
int func_ov135_02201c24(WorldTradeWork *wk);
static int func_ov135_02201c88(WorldTradeWork *wk);
static int func_ov135_02201cbc(WorldTradeWork *wk);
static int func_ov135_02201d14(WorldTradeWork *wk);
int func_ov135_02201d30(WorldTradeWork *wk);
static int func_ov135_02201dbc(WorldTradeWork *wk);
static int func_ov135_02201dc0(WorldTradeWork *wk);
static int func_ov135_02201dd8(WorldTradeWork *wk);
static int func_ov135_02201df0(WorldTradeWork *wk);
static int func_ov135_02201e04(WorldTradeWork *wk);
static int func_ov135_02201e1c(WorldTradeWork *wk);
int func_ov135_02201e38(WorldTradeWork *wk);
static int func_ov135_02201ed4(WorldTradeWork *wk);
void func_ov135_02201f18(WorldTradeWork *wk);
static int func_ov135_02201fa4(WorldTradeWork *wk);
static int func_ov135_02201fdc(WorldTradeWork *wk);
static int func_ov135_02202000(WorldTradeWork *wk);
static int func_ov135_02202038(WorldTradeWork *wk);
static int func_ov135_02202054(WorldTradeWork *wk);
static int func_ov135_0220207c(WorldTradeWork *wk);
static int func_ov135_02202094(WorldTradeWork *wk);
static int func_ov135_022020b8(WorldTradeWork *wk);
static int func_ov135_022020e0(WorldTradeWork *wk);
static int func_ov135_022020f8(WorldTradeWork *wk);
int func_ov135_02202148(WorldTradeWork *wk);
static int func_ov135_022021a4(WorldTradeWork *wk);
static int func_ov135_022021c0(WorldTradeWork *wk);
void func_ov135_022021f8(WorldTradeWork *wk, BOOL keep);
void func_ov135_022022b8(WorldTradeWork *wk, PartyPkm *pkm, int boxNo, BOOL traded);
void func_ov135_022023a4(WorldTradeWork *wk, PartyPkm *pkm, Dpw_Tr_Data *upload, Dpw_Tr_Data *exchange, int boxNo);
void func_ov135_0220246c(WorldTradeData *data, int type);
PartyPkm *func_ov135_02202608(HeapID heapId);
void func_ov114_021b99b8(NHttpRap *rap);
void *func_ov114_021b97f4(NHttpRap *rap);
void func_ov135_022024b0(UnityTowerSurveySave *save, Dpw_Tr_Data *trData);
int func_ov135_022024dc(WorldTradeWork *wk, Dpw_Tr_Data *trData);
static void func_ov135_0220251c(WorldTradeWork *wk, u16 nextSeq1st, u16 nextSeq2nd);
BOOL func_ov135_0220252c(WorldTradeWork *wk);
void func_ov135_0220254c(PartyPkm *pkm, HeapID heapId);

static int (*data_ov135_02203e94[])(WorldTradeWork *wk) = {
    func_ov135_02200ed8,
    func_ov135_02201dbc,
    func_ov135_02200ff4,
    func_ov135_022010bc,
    func_ov135_02201258,
    func_ov135_02201320,
    func_ov135_022013e4,
    func_ov135_02201414,
    func_ov135_022014b8,
    func_ov135_022014d8,
    func_ov135_02201dc0,
    func_ov135_02201554,
    func_ov135_0220157c,
    func_ov135_0220163c,
    func_ov135_02201658,
    func_ov135_02201dd8,
    func_ov135_022016cc,
    func_ov135_0220171c,
    func_ov135_022017f0,
    func_ov135_0220180c,
    func_ov135_02201df0,
    func_ov135_02201ed4,
    func_ov135_02201cbc,
    func_ov135_02201d14,
    func_ov135_02201d30,
    func_ov135_02201e04,
    func_ov135_02201e1c,
    func_ov135_02201e38,
    func_ov135_02201880,
    func_ov135_0220189c,
    func_ov135_02201a48,
    func_ov135_02201a64,
    func_ov135_02201a34,
    func_ov135_02202038,
    func_ov135_02202054,
    func_ov135_0220207c,
    func_ov135_02202094,
    func_ov135_022020b8,
    func_ov135_022020e0,
    func_ov135_022020f8,
    func_ov135_02202148,
    func_ov135_022021a4,
    func_ov135_022021c0,
    func_ov135_02201fa4,
    func_ov135_02201fdc,
    func_ov135_02202000,
    func_ov135_02201c08,
    func_ov135_02201c24,
    func_ov135_02201c88,
};

// pokebw2's WorldTrade_Upload_Main

typedef struct WorldTradePrint WorldTradePrint;
void func_ov135_021f4fc4(WorldTradeWork *wk);
void func_ov135_021f513c(WorldTradeWork *wk);

void func_ov135_021ffbc8(WorldTradeWork *wk);
BOOL func_ov135_02202840(WorldTradePrint *print);
PlayerInfo *func_ov135_021f72c4(Dpw_Tr_Data *dtd);

int func_ov135_02200c0c(WorldTradeWork *wk, int seq) {
    func_ov135_02200eb0(wk);
    func_ov135_02200ccc();
    func_ov135_021fffe8(wk, 0, FALSE);
    if (wk->subProcessMode == UPLOAD_MODE_SERVER_CHECK) {
        GFL_BGSysSetBGEnabled(4, FALSE);
        GFL_BGSysSetBGEnabled(6, FALSE);
    }
    func_ov135_02200dd0(wk);
    func_ov135_02200e48(wk);
    func_ov135_021ffd44(wk);
    GFL_WipeSet(3, 1, 1, 0, 6, 1, HEAPID_WORLDTRADE);
    func_ov135_021f9254(wk);
    wk->subprocessSeq = UPLOAD_SEQ_START;
    wk->unk12E4 = 0;
    return WT_SEQ_FADEIN;
}

int func_ov135_02200c80(WorldTradeWork *wk, int seq) {
    return data_ov135_02203e94[wk->subprocessSeq](wk);
}

// pokebw2's WorldTrade_Upload_End
int func_ov135_02200c94(WorldTradeWork *wk, int seq) {
    if (GXx_GetMasterBrightness_(REG_DB_MASTER_BRIGHT_ADDR) != 0) {
        func_ov135_021ffd74(wk);
    }
    func_ov135_02200ec8(wk);
    func_ov135_02200e94(wk);
    func_ov135_02200db8();
    func_ov135_022000e4(wk);
    func_ov135_021f4ec8(wk);
    return WT_SEQ_INIT;
}

// pokebw2's Upload_BgInit
static void func_ov135_02200ccc(void) {
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
            GX_BG_SCRBASE_0xe800,
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
    GFL_BGSysClearCharCore(0, 32, 0, HEAPID_WORLDTRADE);
    GFL_BGSysClearCharCore(1, 32, 0, HEAPID_WORLDTRADE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's Upload_BgExit
static void func_ov135_02200db8(void) {
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(0);
}

// pokebw2's Upload_BgGraphicSet
static void func_ov135_02200dd0(WorldTradeWork *wk) {
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x1a0, 0x20, HEAPID_WORLDTRADE);
    LoadSysMsgBox(0, 1, 14, 0, HEAPID_WORLDTRADE);
    LoadSysMsgBox(0, 31, 11, 0, HEAPID_WORLDTRADE);
    // The trade room only shows once the player walked in
    if (wk->demoEnd == 0) {
        GFL_BGSysSetBGEnabled(4, FALSE);
        GFL_BGSysSetBGEnabled(5, FALSE);
        GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, FALSE);
    }
    if (wk->oldSubProcess == WORLDTRADE_DEMO) {
        func_ov135_02200ab4(wk);
        func_ov135_02200b34(wk);
    }
}

// pokebw2's Upload_BmpWinInit
static void func_ov135_02200e48(WorldTradeWork *wk) {
    BmpWin *win;

    wk->msgWin = BmpWin_CreateDynamic(0, 2, 19, 27, 4, 13, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->msgWin), 0);
    win = wk->msgWin;
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(win));
}

void func_ov135_02200e94(WorldTradeWork *wk) {
    func_ov135_02202940(&wk->print);
    BmpWin_Free(wk->msgWin);
}


// pokebw2's Upload_InitWork
static void func_ov135_02200eb0(WorldTradeWork *wk) {
    wk->talkString = GFL_StrBufCreate(180, HEAPID_WORLDTRADE);
}

// pokebw2's Upload_FreeWork
static void func_ov135_02200ec8(WorldTradeWork *wk) {
    GFL_StrBufFree(wk->talkString);
}

int func_ov135_02200ed8(WorldTradeWork *wk) {
    switch (wk->subProcessMode) {
    case UPLOAD_MODE_UPLOAD:
        func_ov135_021f90a0(wk, wk->msgManager, 0x18, 1, 0xf0f);
        func_ov135_021f4e98(wk, UPLOAD_SEQ_MES_WAIT, UPLOAD_SEQ_EVIL_CHECK_START);
        wk->evilCheckRetry = 0;
        wk->nameCheckNextSeq = UPLOAD_SEQ_UPLOAD_START;
        break;
    case UPLOAD_MODE_DOWNLOAD:
        func_ov135_021f90a0(wk, wk->msgManager, 0x18, 1, 0xf0f);
        func_ov135_021f4e98(wk, UPLOAD_SEQ_MES_WAIT, UPLOAD_SEQ_DOWNLOAD_START);
        break;
    case UPLOAD_MODE_EXCHANGE:
        func_ov135_021f90a0(wk, wk->msgManager, 0x18, 1, 0xf0f);
        func_ov135_021f4e98(wk, UPLOAD_SEQ_MES_WAIT, UPLOAD_SEQ_EVIL_CHECK_START);
        wk->evilCheckRetry = 0;
        wk->nameCheckNextSeq = UPLOAD_SEQ_EXCHANGE_START;
        break;
    case UPLOAD_MODE_DOWNLOAD_EX:
        func_ov135_021f90a0(wk, wk->msgManager, 0x18, 1, 0xf0f);
        func_ov135_021f4e98(wk, UPLOAD_SEQ_MES_WAIT, UPLOAD_SEQ_DOWNLOAD_EX_START);
        wk->subOutFlag = 1;
        break;
    case UPLOAD_MODE_SERVER_CHECK:
        func_ov135_021f90a0(wk, wk->msgManager, 0x18, 1, 0xf0f);
        func_ov135_021f4e98(wk, UPLOAD_SEQ_MES_WAIT, UPLOAD_SEQ_SERVER_TRADE_CHECK);
        break;
    case UPLOAD_MODE_POKEMON_EVO_SAVE:
        func_ov135_021f90a0(wk, wk->msgManager, 0x9a, 1, 0xf0f);
        wk->subprocessSeq = UPLOAD_SEQ_NOW_SAVE_MESSAGE;
        wk->subNextProcess = WORLDTRADE_TITLE;
        func_0203d34c(TRUE, HEAPID_WORLDTRADE);
        break;
    default:
        GFL_ASSERT(0);
        break;
    }
    func_ov135_021f4fa4(wk);
    return WT_SEQ_MAIN;
}


int func_ov135_02200ff4(WorldTradeWork *wk) {
    Dpw_Tr_Data *upload = &wk->uploadData;
    BOOL ret;

    copyPartyPkm((PartyPkm *)upload, wk->demoPokemon);
    wk->uploadData.countryCode = func_02025c1c(wk->uploadData.countryCode, wk->uploadData.localCode, wk->uploadData.langCode);
    wk->uploadData.localCode = func_02025c30(wk->uploadData.countryCode, wk->uploadData.localCode, wk->uploadData.langCode);
    wk->evilCheck = func_ov114_021b9800(0x48, func_0200853c(wk->param->mystatus), &wk->wifiLoginBuffer);
    func_ov114_021b98a0(wk->evilCheck, 0x48, PokeParty_GetPkmRawSize(), TRUE);
    func_ov114_021b98d8(wk->evilCheck, upload, PokeParty_GetPkmRawSize());
    GFL_ASSERT(func_ov114_021b98f8(wk->evilCheck));
    ret = func_ov114_021b9748(wk->evilCheck) == 0;
    GFL_ASSERT(ret);
    wk->subprocessSeq = UPLOAD_SEQ_EVIL_CHECK_RESULT;
    return WT_SEQ_MAIN;
}

int func_ov135_022010bc(WorldTradeWork *wk) {
    int status = func_ov114_021b99dc(wk->evilCheck);
    int ret = func_ov114_021b9790(wk->evilCheck);

    if (ret == 0) {
        void *body = func_ov114_021b97f4(wk->evilCheck);

        switch (status) {
        case 400:
            func_0200fb4c(0x41);
            GFL_NetErrShow(0);
            func_ov114_021b9774(wk->evilCheck);
            func_ov114_021b99b8(wk->evilCheck);
            func_ov114_021b9838(wk->evilCheck);
            wk->subprocessSeq = UPLOAD_SEQ_ERROR_END;
            return WT_SEQ_MAIN;
        case 401:
            func_0200fb4c(0x42);
            GFL_NetErrShow(0);
            func_ov114_021b9774(wk->evilCheck);
            func_ov114_021b99b8(wk->evilCheck);
            func_ov114_021b9838(wk->evilCheck);
            wk->subprocessSeq = UPLOAD_SEQ_ERROR_END;
            return WT_SEQ_MAIN;
        case 408:
            func_0200fb4c(0x44);
            GFL_NetErrShow(0);
            func_ov114_021b9774(wk->evilCheck);
            func_ov114_021b99b8(wk->evilCheck);
            func_ov114_021b9838(wk->evilCheck);
            wk->subprocessSeq = UPLOAD_SEQ_ERROR_END;
            return WT_SEQ_MAIN;
        default:
            func_0200fb4c(0x43);
            GFL_NetErrShow(0);
            func_ov114_021b9774(wk->evilCheck);
            func_ov114_021b99b8(wk->evilCheck);
            func_ov114_021b9838(wk->evilCheck);
            wk->subprocessSeq = UPLOAD_SEQ_ERROR_END;
            return WT_SEQ_MAIN;
        case 200:
            wk->evilCheckStatus = func_ov114_021b99f0(body);
            wk->evilCheckResult = func_ov114_021b99f4(body, 0);
            MI_CpuCopy8(func_ov114_021b9a14(body, 1), wk->evilCheckSign, 0x80);
            func_ov114_021b99b8(wk->evilCheck);
            func_ov114_021b9838(wk->evilCheck);
            if (wk->evilCheckStatus == 0) {
                if (wk->evilCheckResult == 0) {
                    wk->subprocessSeq = UPLOAD_SEQ_NAME_CHECK_START;
                } else if (wk->evilCheckRetry == 0) {
                    wk->evilCheckRetry++;
                    wk->subprocessSeq = UPLOAD_SEQ_EVIL_CHECK_START;
                    func_ov135_0220254c((PartyPkm *)&wk->uploadData, HEAPID_WORLDTRADE);
                } else {
                    wk->connectErrorNo = -7;
                    wk->subprocessSeq = UPLOAD_SEQ_RETURN_TITLE_MESSAGE;
                }
            } else if (wk->evilCheckRetry == 0) {
                wk->evilCheckRetry++;
                wk->subprocessSeq = UPLOAD_SEQ_EVIL_CHECK_START;
                func_ov135_0220254c((PartyPkm *)&wk->uploadData, HEAPID_WORLDTRADE);
            } else {
                wk->connectErrorNo = -7;
                wk->subprocessSeq = UPLOAD_SEQ_RETURN_TITLE_MESSAGE;
            }
            break;
        }
    } else if (ret != 15) {
        wk->connectErrorNo = -15;
        wk->subprocessSeq = UPLOAD_SEQ_ERROR_MESSAGE;
        func_ov114_021b99b8(wk->evilCheck);
        func_ov114_021b9838(wk->evilCheck);
        func_0200fbb0();
        func_0203ca04();
        func_0200fba0();
        func_0203c968();
    }
    return WT_SEQ_MAIN;
}


int func_ov135_02201258(WorldTradeWork *wk) {
    u32 i;
    StrBuf *name = GFL_StrBufCreate(8, HEAPID_WORLDTRADE);
    const u16 *str;

    GFL_StrBufLoadFixedString(name, wk->uploadData.name, 8);
    MI_CpuFill8(&wk->nameCheck, 0, sizeof(WorldTradeNameCheck));
    wk->nameCheck.unk24 = HEAPID_WORLDTRADE;
    str = GFL_StrBufGetStringPtr(name);
    for (i = 0; i < (u32)GFL_StrBufGetCharCount(name) + 1; i++) {
        u16 c = str[i];

        if (c == GFL_StrBufGetTerminator()) {
            wk->nameCheck.name[i] = 0;
        } else {
            wk->nameCheck.name[i] = c;
        }
    }
    wk->nameCheck.words = wk->nameCheck.name;
    wk->nameCheck.badWordCount = 0;
    func_ov009_0217bc44(&wk->nameCheck.words, 1, NULL, 0, wk->nameCheck.result, &wk->nameCheck.badWordCount, 0x80);
    GFL_StrBufFree(name);
    wk->subprocessSeq = UPLOAD_SEQ_NAME_CHECK_WAIT;
    wk->timeoutCount = 0;
    return WT_SEQ_MAIN;
}

int func_ov135_02201320(WorldTradeWork *wk) {
    BOOL done = DWC_CheckProfanityProcess() == 2;
    BOOL bad;

    if (done) {
        bad = wk->nameCheck.badWordCount > 0;
    }
    if (done) {
        if (bad) {
            u16 *dest = wk->uploadData.name;
            MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_ABILITY_HANDLERS_BTL_MAIN, HEAPID_WORLDTRADE);
            StrBuf *str = GFL_MsgDataLoadStrbufNew(msgData, DEFAULT_NAME_MSG);
            const u16 *src;
            int i;

            GFL_MsgDataFree(msgData);
            src = GFL_StrBufGetStringPtr(str);
            for (i = 0; i < 7 && i < GFL_StrBufGetCharCount(str); i++) {
                dest[i] = src[i];
            }
            dest[i] = GFL_StrBufGetTerminator();
            GFL_StrBufFree(str);
        }
        wk->subprocessSeq = wk->nameCheckNextSeq;
    } else if (wk->timeoutCount++ == UPLOAD_TIMEOUT) {
        wk->connectErrorNo = -5;
        wk->subprocessSeq = UPLOAD_SEQ_ERROR_MESSAGE;
        func_0200fbb0();
        func_0203ca04();
        func_0200fba0();
        func_0203c968();
    }
    return WT_SEQ_MAIN;
}


int func_ov135_022013e4(WorldTradeWork *wk) {
    func_ov114_021c86b8(&wk->uploadData, wk->evilCheckSign, sizeof(wk->evilCheckSign));
    wk->subprocessSeq = UPLOAD_SEQ_UPLOAD_RESULT;
    wk->timeoutCount = 0;
    func_ov135_0220251c(wk, UPLOAD_SEQ_UPLOAD_FINISH, UPLOAD_SEQ_UPLOAD_SUCCESS_MESSAGE);
    return WT_SEQ_MAIN;
}

int func_ov135_02201414(WorldTradeWork *wk) {
    if (func_ov114_021c866c()) {
        s32 result = func_ov114_021c86a8();

        wk->timeoutCount = 0;
        switch (result) {
        case 0:
            func_ov135_022021f8(wk, TRUE);
            RecordAddOne(wk->param->record, 0x1c);
            wk->subprocessSeq = UPLOAD_SEQ_SAVE;
            break;
        case -1:
            wk->connectErrorNo = result;
            wk->subprocessSeq = UPLOAD_SEQ_ERROR_MESSAGE;
            func_0200fbb0();
            func_0203ca04();
            func_0200fba0();
            func_0203c968();
            break;
        case -5:
        case -6:
        case -7:
        case -8:
        case -9:
        case -10:
        case -11:
            wk->connectErrorNo = result;
            wk->subprocessSeq = UPLOAD_SEQ_RETURN_TITLE_MESSAGE;
            break;
        case -4:
        case -12:
            wk->connectErrorNo = result;
            wk->subprocessSeq = UPLOAD_SEQ_RETURN_TITLE_MESSAGE;
            break;
        case -2:
        case -14:
        case -15:
            wk->connectErrorNo = result;
            wk->subprocessSeq = UPLOAD_SEQ_ERROR_MESSAGE;
            func_0200fbb0();
            func_0203ca04();
            func_0200fba0();
            func_0203c968();
            break;
        case -13:
            func_ov135_021f513c(wk);
            break;
        }
    } else if (++wk->timeoutCount == UPLOAD_TIMEOUT) {
        func_ov135_021f513c(wk);
    }
    return WT_SEQ_MAIN;
}


static int func_ov135_022014b8(WorldTradeWork *wk) {
    func_ov114_021c8770();
    wk->subprocessSeq = 9;
    wk->timeoutCount = 0;
    wk->depositFlag = 1;
    return 2;
}

int func_ov135_022014d8(WorldTradeWork *wk) {
    if (func_ov114_021c866c()) {
        s32 result = func_ov114_021c86a8();

        wk->timeoutCount = 0;
        switch (result) {
        case 0:
            wk->serverWaitTime = 0;
            wk->subprocessSeq = UPLOAD_SEQ_SAVE_LAST;
            break;
        case -4:
        case -12:
            wk->connectErrorNo = result;
            wk->subprocessSeq = UPLOAD_SEQ_RETURN_TITLE_MESSAGE;
            break;
        case -1:
        case -2:
        case -3:
        case -5:
        case -14:
        case -15:
            func_ov135_021f513c(wk);
            break;
        case -13:
            func_ov135_021f513c(wk);
            break;
        }
    } else if (++wk->timeoutCount == UPLOAD_TIMEOUT) {
        func_ov135_021f513c(wk);
    }
    return WT_SEQ_MAIN;
}


static int func_ov135_02201554(WorldTradeWork *wk) {
    func_ov114_021c87fc(&wk->uploadData);
    wk->subprocessSeq = 0xc;
    wk->timeoutCount = 0;
    func_ov135_0220251c(wk, 0xd, 0xf);
    return 2;
}

int func_ov135_0220157c(WorldTradeWork *wk) {
    if (func_ov114_021c866c()) {
        s32 result = func_ov114_021c86a8();

        wk->timeoutCount = 0;
        switch (result) {
        case 0: {
            s8 isTrade = wk->uploadData.isTrade;
            int nextSeq;

            if (isTrade) {
                nextSeq = UPLOAD_SEQ_SERVER_TRADE_CHECK;
                wk->subprocessSeq = nextSeq;
                break;
            } else {
                PartyPkm *pkm = func_0200adb4(wk->param->worldtrade_data);
                u16 boxNo = func_0200ade0(wk->param->worldtrade_data);
                func_ov135_022022b8(wk, pkm, boxNo, isTrade);
                nextSeq = UPLOAD_SEQ_SAVE;
                wk->subprocessSeq = nextSeq;
                break;
            }
        }
        case -3:
            wk->subprocessSeq = UPLOAD_SEQ_ERROR_MESSAGE;
            func_0200fbb0();
            func_0203ca04();
            func_0200fba0();
            func_0203c968();
            break;
        case -4:
            wk->subprocessSeq = UPLOAD_SEQ_ERROR_MESSAGE;
            func_0200fbb0();
            func_0203ca04();
            func_0200fba0();
            func_0203c968();
            break;
        case -12:
            wk->connectErrorNo = result;
            wk->subprocessSeq = UPLOAD_SEQ_RETURN_TITLE_MESSAGE;
            break;
        case -2:
        case -14:
        case -15:
            wk->connectErrorNo = result;
            wk->subprocessSeq = UPLOAD_SEQ_ERROR_MESSAGE;
            func_0200fbb0();
            func_0203ca04();
            func_0200fba0();
            func_0203c968();
            break;
        case -13:
            func_ov135_021f513c(wk);
            break;
        }
    } else if (++wk->timeoutCount == UPLOAD_TIMEOUT) {
        func_ov135_021f513c(wk);
    }
    return WT_SEQ_MAIN;
}


static int func_ov135_0220163c(WorldTradeWork *wk) {
    func_ov114_021c8978();
    wk->subprocessSeq = 0xe;
    wk->timeoutCount = 0;
    return 2;
}

int func_ov135_02201658(WorldTradeWork *wk) {
    if (func_ov114_021c866c()) {
        s32 result = func_ov114_021c86a8();

        wk->timeoutCount = 0;
        switch (result) {
        case 0:
            wk->subprocessSeq = UPLOAD_SEQ_SAVE_LAST;
            break;
        case -5:
        case -4:
        case -3:
            func_ov135_021f513c(wk);
            break;
        case -15:
        case -14:
        case -12:
        case -2:
            func_ov135_021f513c(wk);
            break;
        case -13:
            func_ov135_021f513c(wk);
            break;
        }
    } else if (++wk->timeoutCount == UPLOAD_TIMEOUT) {
        func_ov135_021f513c(wk);
    }
    return WT_SEQ_MAIN;
}


static int func_ov135_022016cc(WorldTradeWork *wk) {
    func_ov114_021c8c68(wk->downloadData[wk->unkD4].id, &wk->uploadData, &wk->exchangeData,
            wk->evilCheckSign, 0x80);
    func_ov135_0220251c(wk, 0x12, 0x14);
    wk->subprocessSeq = 0x11;
    wk->timeoutCount = 0;
    return 2;
}

static int func_ov135_0220171c(WorldTradeWork *wk) {
    if (func_ov114_021c866c()) {
        s32 result = func_ov114_021c86a8();

        wk->timeoutCount = 0;
        switch (result) {
        case 0:
            wk->subprocessSeq = 0x22;
            func_ov135_022021f8(wk, FALSE);
            func_ov135_022023a4(wk, (PartyPkm *)wk->exchangeData.postData, &wk->uploadData, &wk->exchangeData, wk->boxTrayNo);
            func_ov135_022024b0(wk->param->wifihistory, &wk->exchangeData);
            func_ov135_02201b44(wk, (PartyPkm *)wk->exchangeData.postData, &wk->exchangeData, (PartyPkm *)wk->uploadData.postData);
            break;
        case -5:
            wk->connectErrorNo = result;
            wk->subprocessSeq = 0x15;
            break;
        case -6:
        case -7:
        case -8:
        case -9:
        case -10:
        case -11:
            wk->connectErrorNo = result;
            wk->subprocessSeq = 0x2d;
            break;
        case -12:
            wk->connectErrorNo = result;
            wk->subprocessSeq = 0x2d;
            break;
        case -2:
        case -14:
        case -15:
            wk->connectErrorNo = result;
            wk->subprocessSeq = 0x2b;
            func_0200fbb0();
            func_0203ca04();
            func_0200fba0();
            func_0203c968();
            break;
        case -13:
            func_ov135_021f513c(wk);
            break;
        }
    } else if (++wk->timeoutCount == 0xe10) {
        func_ov135_021f513c(wk);
    }
    return 2;
}


static int func_ov135_022017f0(WorldTradeWork *wk) {
    func_ov114_021c8d14();
    wk->subprocessSeq = 0x13;
    wk->timeoutCount = 0;
    return 2;
}

int func_ov135_0220180c(WorldTradeWork *wk) {
    if (func_ov114_021c866c()) {
        s32 result = func_ov114_021c86a8();

        wk->timeoutCount = 0;
        switch (result) {
        case 0:
            wk->subprocessSeq = UPLOAD_SEQ_SAVE_LAST;
            break;
        case -5:
            func_ov135_021f513c(wk);
            break;
        case -15:
        case -14:
        case -12:
        case -2:
            func_ov135_021f513c(wk);
            break;
        case -13:
            func_ov135_021f513c(wk);
            break;
        }
    } else if (++wk->timeoutCount == UPLOAD_TIMEOUT) {
        func_ov135_021f513c(wk);
    }
    return WT_SEQ_MAIN;
}


static int func_ov135_02201880(WorldTradeWork *wk) {
    func_ov114_021c887c(&wk->uploadData);
    wk->subprocessSeq = 0x1d;
    wk->timeoutCount = 0;
    return 2;
}

int func_ov135_0220189c(WorldTradeWork *wk) {
    if (func_ov114_021c866c()) {
        s32 result = func_ov114_021c86a8();

        wk->timeoutCount = 0;
        switch (result) {
        case 0:
            wk->subprocessSeq = UPLOAD_SEQ_SERVER_DOWNLOAD;
            break;
        case 1:
            wk->depositFlag = 1;
            switch (func_ov135_022024dc(wk, &wk->uploadData)) {
            case 1:
                func_ov135_021f4fc4(wk);
                func_ov135_021f90a0(wk, wk->msgManager, 0x23, 1, 0xf0f);
                func_ov135_021f4e98(wk, 0x2a, 0x20);
                break;
            case 2:
                func_ov135_021f4fc4(wk);
                func_ov135_021f90a0(wk, wk->msgManager, 0x29, 1, 0xf0f);
                func_ov135_021f4e98(wk, 0x2a, 0x20);
                break;
            case 0:
                wk->subprocessSeq = UPLOAD_SEQ_DOWNLOAD_EX_START;
                wk->subOutFlag = 1;
                wk->checkEvolution = 1;
                break;
            }
            break;
        case -3:
            wk->depositFlag = 0;
            if (func_0200ad8c(wk->param->worldtrade_data)) {
                PartyPkm *pkm = func_ov135_02202608(HEAPID_WORLDTRADE);

                func_0200ad9c(wk->param->worldtrade_data, pkm);
                loadPokemonNicknameToStrbuf(wk->wordSet, 0, pkm);
                wk->errorMesNo = 2;
                wk->subprocessSeq = UPLOAD_SEQ_TIMEOUT_SAVE;
                func_ov135_022022b8(wk, pkm, func_0200ade0(wk->param->worldtrade_data), FALSE);
                func_0200ad94(wk->param->worldtrade_data, 0);
                GFL_HeapFree(pkm);
            } else {
                func_ov135_02201b18(wk);
            }
            break;
        case -4:
            wk->depositFlag = 0;
            if (func_0200ad8c(wk->param->worldtrade_data)) {
                PartyPkm *pkm = func_ov135_02202608(HEAPID_WORLDTRADE);

                func_0200ad9c(wk->param->worldtrade_data, pkm);
                loadPokemonNicknameToStrbuf(wk->wordSet, 0, pkm);
                wk->errorMesNo = 3;
                wk->subprocessSeq = UPLOAD_SEQ_TIMEOUT_SAVE;
                func_0200ad94(wk->param->worldtrade_data, 0);
                GFL_HeapFree(pkm);
            }
            break;
        case -2:
        case -12:
        case -14:
        case -15:
            wk->connectErrorNo = result;
            wk->subprocessSeq = UPLOAD_SEQ_ERROR_MESSAGE;
            func_0200fbb0();
            func_0203ca04();
            func_0200fba0();
            func_0203c968();
            break;
        case -13:
            func_ov135_021f513c(wk);
            break;
        }
    } else if (++wk->timeoutCount == UPLOAD_TIMEOUT) {
        func_ov135_021f513c(wk);
    }
    return WT_SEQ_MAIN;
}


// pokebw2's Upload_SubSeqServerTradeCheckEnd
static int func_ov135_02201a34(WorldTradeWork *wk) {
    func_ov135_021f4ec0(wk, WORLDTRADE_TITLE, 0);
    wk->subprocessSeq = UPLOAD_SEQ_END;
    return WT_SEQ_MAIN;
}

static int func_ov135_02201a48(WorldTradeWork *wk) {
    func_ov114_021c87fc(&wk->uploadData);
    wk->subprocessSeq = 0x1f;
    wk->timeoutCount = 0;
    return 2;
}

int func_ov135_02201a64(WorldTradeWork *wk) {
    if (func_ov114_021c866c()) {
        s32 result = func_ov114_021c86a8();

        wk->timeoutCount = 0;
        switch (result) {
        case 0:
            if (func_ov135_0220252c(wk)) {
                wk->subprocessSeq = UPLOAD_SEQ_SERVER_POKE_DELETE;
                wk->depositFlag = 0;
                return WT_SEQ_MAIN;
            }
            wk->depositFlag = 1;
            break;
        case -3:
            wk->depositFlag = 0;
            break;
        case -2:
        case -12:
        case -14:
        case -15:
            wk->connectErrorNo = result;
            wk->subprocessSeq = UPLOAD_SEQ_ERROR_MESSAGE;
            func_0200fbb0();
            func_0203ca04();
            func_0200fba0();
            func_0203c968();
            return WT_SEQ_MAIN;
        case -13:
            func_ov135_021f513c(wk);
            return WT_SEQ_MAIN;
        case -1:
        case -4:
        case -5:
        case -6:
        case -7:
        case -8:
        case -9:
        case -10:
        case -11:
            break;
        }
        func_ov135_02201b18(wk);
    } else if (++wk->timeoutCount == UPLOAD_TIMEOUT) {
        func_ov135_021f513c(wk);
    }
    return WT_SEQ_MAIN;
}


// Goes back to the screen that started the server check
// pokebw2's Upload_ReturnProcess
void func_ov135_02201b18(WorldTradeWork *wk) {
    switch (wk->subReturnProcess) {
    case WORLDTRADE_TITLE:
        func_ov135_021f4ec0(wk, WORLDTRADE_TITLE, 0);
        wk->subprocessSeq = UPLOAD_SEQ_END;
        break;
    case WORLDTRADE_MYPOKE:
        func_ov135_021f4ec0(wk, WORLDTRADE_MYPOKE, 3);
        wk->subprocessSeq = UPLOAD_SEQ_END;
        break;
    }
}

static void func_ov135_02201b44(WorldTradeWork *wk, PartyPkm *received, Dpw_Tr_Data *trData, PartyPkm *sent) {
    PlayerInfo *partner;
    PlayerInfo *visitorInfo;
    UnityTowerVisitor visitor;

    RecordAddOne(wk->param->record, 0x10);
    func_0200ed54(getHollow_RivalData(wk->param->savedata), trData->trainerID);

    partner = func_ov135_021f72c4(trData);
    func_02009de8(getPalPadFriendListAddress(wk->param->savedata), partner);
    GFL_HeapFree(partner);

    visitorInfo = func_ov135_021f72c4(trData);
    MI_CpuFill8(&visitor, 0, 0x28);
    func_02008494(visitorInfo, &visitor.info);
    visitor.sentSpecies = sent != NULL ? (u16)PokeParty_GetParam(sent, PKM_PARAM_SPECIES, NULL) : (u16)trData->postSimple.characterNo;
    visitor.receivedSpecies = PokeParty_GetParam(received, PKM_PARAM_SPECIES, NULL);
    visitor.hobby = trData->unk126;
    visitor.unk26_0 = trData->unkF7;
    visitor.countryCount = trData->unk127;
    func_0202eb7c(wk->param->wifihistory, &visitor);
    GFL_HeapFree(visitorInfo);
}


static int func_ov135_02201c08(WorldTradeWork *wk) {
    wk->timeoutCount = 0;
    func_ov114_021c89f0();
    wk->subprocessSeq = 0x2f;
    return 2;
}

int func_ov135_02201c24(WorldTradeWork *wk) {
    if (func_ov114_021c866c()) {
        switch (func_ov114_021c86a8()) {
        case -12:
            wk->subprocessSeq = UPLOAD_SEQ_SERVER_SERVICE_END;
            break;
        case -2:
        case -14:
        case -15:
            wk->subprocessSeq = UPLOAD_SEQ_SERVER_SERVICE_END;
            break;
        case -13:
            func_ov135_021f513c(wk);
            break;
        }
    } else if (++wk->timeoutCount == UPLOAD_TIMEOUT) {
        wk->subprocessSeq = UPLOAD_SEQ_SERVER_SERVICE_END;
    }
    return WT_SEQ_MAIN;
}


static int func_ov135_02201c88(WorldTradeWork *wk) {
    func_ov114_021c8658();
    func_ov135_021f4fc4(wk);
    GFL_WipeSet(0, 0, 0, 0, 6, 1, 0x48);
    wk->subprocessSeq = 0;
    return 3;
}

static int func_ov135_02201cbc(WorldTradeWork *wk) {
    func_ov135_022022b8(wk, (PartyPkm *)wk->uploadData.postData, (u32)func_0200ade0(wk->param->worldtrade_data), (s32)wk->uploadData.isTrade);
    func_ov135_022024b0(wk->param->wifihistory, &wk->uploadData);
    func_ov135_02201b44(wk, (PartyPkm *)wk->uploadData.postData, &wk->uploadData, (PartyPkm *)0x0);
    func_0200ad94(wk->param->worldtrade_data, 0);
    wk->subprocessSeq = 0x22;
    func_ov135_0220251c(wk, 0x17, 0xf);
    return 2;
}

static int func_ov135_02201d14(WorldTradeWork *wk) {
    func_ov114_021c88fc();
    wk->subprocessSeq = 0x18;
    wk->timeoutCount = 0;
    return 2;
}

int func_ov135_02201d30(WorldTradeWork *wk) {
    if (func_ov114_021c866c()) {
        s32 result = func_ov114_021c86a8();

        wk->timeoutCount = 0;
        switch (result) {
        case 0:
            wk->subprocessSeq = UPLOAD_SEQ_SAVE_LAST;
            break;
        case -3:
            wk->connectErrorNo = result;
            wk->subprocessSeq = UPLOAD_SEQ_ERROR_MESSAGE;
            func_0200fbb0();
            func_0203ca04();
            func_0200fba0();
            func_0203c968();
            break;
        case -4:
        case -12:
            wk->connectErrorNo = result;
            /* fallthrough */
        case -2:
        case -14:
        case -15:
            func_ov135_021f513c(wk);
            break;
        case -13:
            func_ov135_021f513c(wk);
            break;
        }
    } else if (++wk->timeoutCount == UPLOAD_TIMEOUT) {
        func_ov135_021f513c(wk);
    }
    return WT_SEQ_MAIN;
}


// pokebw2's Upload_SubSeqMain
static int func_ov135_02201dbc(WorldTradeWork *wk) {
    return WT_SEQ_MAIN;
}

static int func_ov135_02201dc0(WorldTradeWork *wk) {
    wk->depositFlag = 1;
    func_ov135_021f4ec0(wk, 9, 7);
    wk->subprocessSeq = 0x28;
    return 2;
}

static int func_ov135_02201dd8(WorldTradeWork *wk) {
    wk->depositFlag = 0;
    func_ov135_021f4ec0(wk, 9, 8);
    wk->subprocessSeq = 0x28;
    return 2;
}

// pokebw2's Upload_SubSeqExchangeSuccessMessage
static int func_ov135_02201df0(WorldTradeWork *wk) {
    func_ov135_021f4ec0(wk, WORLDTRADE_DEMO, DEMO_MODE_EXCHANGE);
    wk->subprocessSeq = UPLOAD_SEQ_END;
    return WT_SEQ_MAIN;
}

static int func_ov135_02201e04(WorldTradeWork *wk) {
    wk->depositFlag = 0;
    func_ov135_021f4ec0(wk, 9, 10);
    wk->subprocessSeq = 0x22;
    return 2;
}

static int func_ov135_02201e1c(WorldTradeWork *wk) {
    func_ov114_021c8978();
    wk->subprocessSeq = 0x1b;
    wk->timeoutCount = 0;
    return 2;
}

int func_ov135_02201e38(WorldTradeWork *wk) {
    if (func_ov114_021c866c()) {
        s32 result = func_ov114_021c86a8();

        wk->timeoutCount = 0;
        switch (result) {
        case 0:
            func_ov135_02201b18(wk);
            break;
        case -3:
            func_ov135_02201b18(wk);
            /* fallthrough */
        case -4:
            func_ov135_02201b18(wk);
            /* fallthrough */
        case -5:
            wk->connectErrorNo = result;
            wk->subprocessSeq = UPLOAD_SEQ_ERROR_MESSAGE;
            func_0200fbb0();
            func_0203ca04();
            func_0200fba0();
            func_0203c968();
            break;
        case -2:
        case -12:
        case -14:
        case -15:
            func_ov135_021f513c(wk);
            break;
        case -13:
            func_ov135_021f513c(wk);
            break;
        }
    } else if (++wk->timeoutCount == UPLOAD_TIMEOUT) {
        func_ov135_021f513c(wk);
    }
    return WT_SEQ_MAIN;
}


static int func_ov135_02201ed4(WorldTradeWork *wk) {
    func_ov135_021f90a0(wk, wk->msgManager, 0x99, 1, 0xf0f);
    func_ov135_021f4e98(wk, 0x2a, 0x28);
    func_ov135_021f4ec0(wk, 1, 0);
    func_ov135_021f4fc4(wk);
    func_ov135_021ffbc8(wk);
    return 2;
}

void func_ov135_02201f18(WorldTradeWork *wk) {
    int msgNo;

    switch (wk->connectErrorNo) {
    case -1:
        msgNo = 0x9d;
        break;
    case -2:
        msgNo = 0xa9;
        break;
    case -3:
        msgNo = 0xa2;
        break;
    case -4:
        msgNo = 0xa4;
        break;
    case -5:
        msgNo = 0xa3;
        break;
    case -6:
        msgNo = 0x1b;
        break;
    case -7:
        msgNo = 0x1c;
        break;
    case -8:
        msgNo = 0x1d;
        break;
    case -9:
        msgNo = 0x1e;
        break;
    case -10:
        msgNo = 0x1f;
        break;
    case -11:
        msgNo = 0x20;
        break;
    case -12:
        msgNo = 0xa0;
        break;
    case -13:
        msgNo = 0xa5;
        break;
    case -14:
        msgNo = 0xaa;
        break;
    case -15:
        msgNo = 0xa1;
        break;
    default:
        msgNo = 0x9f;
        break;
    }
    func_ov135_021f9088(wk, wk->msgManager, msgNo, 1, 0xf0f);
}


static int func_ov135_02201fa4(WorldTradeWork *wk) {
    BOOL BVar1;
    func_ov135_02201f18(wk);
    BVar1 = func_ov114_021c866c();
    if (BVar1 != 0) {
        func_ov135_021f4e98(wk, 0x29, 0x30);
    } else {
        func_ov135_021f4e98(wk, 0x29, 0x2e);
    }
    func_ov135_021f4ec0(wk, 0, 0x15);
    func_ov135_021f4fc4(wk);
    return 2;
}

// pokebw2's Upload_SubSeqErrorEnd
static int func_ov135_02201fdc(WorldTradeWork *wk) {
    if (func_ov114_021c866c()) {
        wk->subprocessSeq = UPLOAD_SEQ_SERVER_SERVICE_END;
    } else {
        wk->subprocessSeq = UPLOAD_SEQ_CANCEL;
    }
    func_ov135_021f4ec0(wk, WORLDTRADE_ENTER, ENTER_MODE_RELOGIN);
    return WT_SEQ_MAIN;
}

static int func_ov135_02202000(WorldTradeWork *wk) {
    func_ov135_02201f18(wk);
    func_ov135_021f4e98(wk, 0x29, 0x28);
    func_ov135_021f4ec0(wk, 1, 0);
    func_ov135_021f4fc4(wk);
    if (0 < wk->searchResult) {
        func_ov135_021ffbc8(wk);
    }
    return 2;
}

// pokebw2's Upload_SubSeqNowSaveMessage
static int func_ov135_02202038(WorldTradeWork *wk) {
    func_ov135_021f4e98(wk, UPLOAD_SEQ_MES_WAIT, UPLOAD_SEQ_SAVE);
    func_ov135_0220251c(wk, UPLOAD_SEQ_SAVE_LAST, UPLOAD_SEQ_END);
    return WT_SEQ_MAIN;
}

static int func_ov135_02202054(WorldTradeWork *wk) {
    GameData *gameData;

    gameData = GSYS_GetGameData(wk->param->gsys);
    func_02012dac(gameData);
    wk->subprocessSeq = 0x23;
    wk->wait = GFL_RandomLC(0x3c) + 2;
    return 2;
}

static int func_ov135_0220207c(WorldTradeWork *wk) {
    s32 wait;

    wait = wk->wait + -1;
    wk->wait = wait;
    if (wait == 0) {
        wk->subprocessSeq = 0x24;
    }
    return 2;
}

static int func_ov135_02202094(WorldTradeWork *wk) {
    GameData *gameData;

    gameData = GSYS_GetGameData(wk->param->gsys);
    if (func_02012dd0(gameData) == 1) {
        wk->subprocessSeq = (u32)wk->saveNextSeq1st;
    }
    return 2;
}

static int func_ov135_022020b8(WorldTradeWork *wk) {
    GameData *gameData;

    gameData = GSYS_GetGameData(wk->param->gsys);
    if (func_02012dd0(gameData) == 2) {
        wk->subprocessSeq = (u32)wk->saveNextSeq2nd;
        func_ov135_021f4fc4(wk);
    }
    return 2;
}

// pokebw2's Upload_SubSeqTimeoutSave
static int func_ov135_022020e0(WorldTradeWork *wk) {
    func_02012dac(GSYS_GetGameData(wk->param->gsys));
    wk->subprocessSeq = UPLOAD_SEQ_TIMEOUT_SAVE_WAIT;
    return WT_SEQ_MAIN;
}

static int func_ov135_022020f8(WorldTradeWork *wk) {
    GameData *gameData;

    gameData = GSYS_GetGameData(wk->param->gsys);
    if (func_02012dd0(gameData) == 2) {
        func_ov135_021f4ec0(wk, 1, 0);
        func_ov135_021f4fc4(wk);
        func_ov135_021f90a0(wk, wk->msgManager, wk->errorMesNo, 1, 0xf0f);
        func_ov135_021f4e98(wk, 0x2a, 0x20);
    }
    return 2;
}

int func_ov135_02202148(WorldTradeWork *wk) {
    func_ov135_021f4fc4(wk);
    if (wk->subOutFlag == 1) {
        if (GXx_GetMasterBrightness_(REG_DB_MASTER_BRIGHT_ADDR) == -16) {
            GFL_WipeSet(3, 0, 0, 0, 6, 1, HEAPID_WORLDTRADE);
        } else {
            GFL_WipeSet(0, 0, 0, 0, 6, 1, HEAPID_WORLDTRADE);
        }
    } else {
        GFL_WipeSet(3, 0, 0, 0, 6, 1, HEAPID_WORLDTRADE);
    }
    wk->subprocessSeq = 0;
    return WT_SEQ_FADEOUT;
}


static int func_ov135_022021a4(WorldTradeWork *wk) {
    BOOL BVar1;

    BVar1 = func_ov135_02202840(&wk->print);
    if (BVar1 == 0) {
        wk->subprocessSeq = wk->subprocessNextSeq;
    }
    return 2;
}

static int func_ov135_022021c0(WorldTradeWork *wk) {
    BOOL BVar1;
    u32 result;

    BVar1 = func_ov135_02202840(&wk->print);
    if (((BVar1 == 0) && (result = GCTX_HIDGetPressedKeys(), (result & 1) != 0)) ||
            (BVar1 = func_02035c60(), BVar1 != 0)) {
        GFL_SndSEPlay(0x547);
        wk->subprocessSeq = wk->subprocessNextSeq;
    }
    return 2;
}

void func_ov135_022021f8(WorldTradeWork *wk, BOOL keep) {
    if (wk->boxTrayNo != 0xff) {
        PartyPkm *pkm = func_ov135_02202608(HEAPID_WORLDTRADE);

        func_ov135_022026a0(BoxSaveAccessor_GetPkm(wk->param->mybox, wk->boxTrayNo, wk->boxCursorPos), pkm);
        if (keep) {
            func_0200adb8(wk->param->worldtrade_data, pkm, wk->boxTrayNo);
        }
        BoxSaveAccessor_ClearPkm(wk->param->mybox, wk->boxTrayNo, wk->boxCursorPos);
        GFL_HeapFree(pkm);
    } else {
        PartyPkm *pkm = PokeParty_GetPkm(wk->param->myparty, wk->boxCursorPos);
        GameData *gameData;

        if (keep) {
            func_0200adb8(wk->param->worldtrade_data, pkm, wk->boxTrayNo);
        }
        PokeParty_RemovePkm(wk->param->myparty, wk->boxCursorPos);
        gameData = GSYS_GetGameData(wk->param->gsys);
        checkChatotInParty(getChatterDataAddress(gameData), GameData_GetParty(gameData));
    }
    if (keep) {
        func_0200ad94(wk->param->worldtrade_data, 1);
    }
}

void func_ov135_022022b8(WorldTradeWork *wk, PartyPkm *pkm, int boxNo, BOOL traded) {
    PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL);
    addPkmToDex(wk->param->pokedex, pkm);
    boxNo = 0xff;
    if (PokeParty_GetPkmCount(wk->param->myparty) == 6) {
        boxNo = 0;
    }
    if (traded) {
        PokeParty_SetParam(pkm, PKM_PARAM_HAPPINESS, 70);
        PokeParty_SetParam(pkm, PKM_PARAM_SEX,
                           PML_UtilDerivePkmSex(PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL),
                                                PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL),
                                                PokeParty_GetParam(pkm, PKM_PARAM_PID, NULL)));
        func_ov135_0220246c(wk->param->worldtrade_data, 0);
    }
    if (boxNo == 0xff) {
        int count;

        PokeParty_AddPkm(wk->param->myparty, pkm);
        count = PokeParty_GetPkmCount(wk->param->myparty);
        wk->unkDC = 0xff;
        wk->unkE0 = count - 1;
    } else {
        int pos = 0;

        BoxSaveAccessor_GetNextFreeBoxSlot(wk->param->mybox, &boxNo, &pos);
        BoxSaveAccessor_InsertPkmCore(wk->param->mybox, boxNo, func_ov135_022026bc(pkm));
        wk->unkDC = boxNo;
        wk->unkE0 = pos;
    }
    func_0200ad94(wk->param->worldtrade_data, 0);
}

void func_ov135_022023a4(WorldTradeWork *wk, PartyPkm *pkm, Dpw_Tr_Data *upload, Dpw_Tr_Data *exchange, int boxNo) {
    addPkmToDex(wk->param->pokedex, pkm);
    boxNo = 0xff;
    if (PokeParty_GetPkmCount(wk->param->myparty) == 6) {
        boxNo = 0;
    }
    PokeParty_SetParam(pkm, PKM_PARAM_HAPPINESS, 70);
    PokeParty_SetParam(pkm, PKM_PARAM_SEX,
                       PML_UtilDerivePkmSex(PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL),
                                            PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL),
                                            PokeParty_GetParam(pkm, PKM_PARAM_PID, NULL)));
    if (boxNo == 0xff) {
        int count;

        PokeParty_AddPkm(wk->param->myparty, pkm);
        count = PokeParty_GetPkmCount(wk->param->myparty);
        wk->unkDC = 0xff;
        wk->unkE0 = count - 1;
    } else {
        int pos = 0;

        BoxSaveAccessor_GetNextFreeBoxSlot(wk->param->mybox, &boxNo, &pos);
        BoxSaveAccessor_InsertPkmCore(wk->param->mybox, boxNo, func_ov135_022026bc(pkm));
        wk->unkDC = boxNo;
        wk->unkE0 = pos;
    }
    func_ov135_0220246c(wk->param->worldtrade_data, 1);
}




// Records the date of a deposit or a trade
// pokebw2's Upload_TradeDateUpDate
void func_ov135_0220246c(WorldTradeData *data, int type) {
    RTCDate date;
    RTCTime time;
    u32 value;

    DWC_GetDateTime(&date, &time);
    value = (date.year << 24) | ((date.month & 0xff) << 16) | ((date.day & 0xff) << 8) | date.week;
    if (type == 1) {
        func_0200add8(data, value);
    } else {
        func_0200add0(data, value);
    }
}

// pokebw2's Upload_WifiHistoryDataSet
void func_ov135_022024b0(UnityTowerSurveySave *save, Dpw_Tr_Data *trData) {
    u8 region = trData->localCode;
    u8 country = trData->countryCode;

    if (!func_0200949c(save, country, region)) {
        func_020094d8(save, country, region, 1);
    }
}

int func_ov135_022024dc(WorldTradeWork *wk, Dpw_Tr_Data *trData) {
    if (func_ov135_021f6de8((PartyPkm *)trData) && PokeParty_GetPkmCount(wk->param->myparty) == 6) {
        return 2;
    }
    if (wk->boxPokeNum == BOX_COUNT * BOX_POKE_NUM && PokeParty_GetPkmCount(wk->param->myparty) == 6) {
        return 1;
    }
    return 0;
}


static void func_ov135_0220251c(WorldTradeWork *wk, u16 nextSeq1st, u16 nextSeq2nd) {
    wk->saveNextSeq1st = nextSeq1st;
    wk->saveNextSeq2nd = nextSeq2nd;
}

BOOL func_ov135_0220252c(WorldTradeWork *wk) {
    if ((func_0200ad8c(wk->param->worldtrade_data) == 0) && (wk->depositFlag != 0)) {
        return 1;
    }
    return 0;
}

// Replaces the original trainer's name, on the Pokémon and its mail, with the version's default, and resets the
// nickname
// pokebw2's Upload_ReplaceBadName
void func_ov135_0220254c(PartyPkm *pkm, HeapID heapId) {
    MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_ABILITY_HANDLERS_BTL_MAIN, HEAPID_TAIL(heapId));
    StrBuf *name = GFL_MsgDataLoadStrbufNew(msgData, DEFAULT_NAME_MSG);

    GFL_MsgDataFree(msgData);
    setNicknameToNick(pkm);
    PokeParty_SetParam(pkm, PKM_PARAM_OT_NAME_RAW, (uintptr_t)GFL_StrBufGetStringPtr(name));
    if (PML_ItemIsMail(PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL))) {
        MailData *mail = CreateMailData(HEAPID_TAIL(heapId));

        PokeParty_GetParam(pkm, PKM_PARAM_MAIL, mail);
        func_02008f88(mail, GFL_StrBufGetStringPtr(name));
        PokeParty_SetParam(pkm, PKM_PARAM_MAIL, (uintptr_t)mail);
        GFL_HeapFree(mail);
    }
    GFL_StrBufFree(name);
}
