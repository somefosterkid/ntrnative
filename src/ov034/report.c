// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov087/report.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "field/report.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "field/player_state.h"
#include "field/zone.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "pml/poke_party.h"
#include "save/event_work.h"
#include "save/pokedex.h"
#include "save/playtime.h"
#include "save/save_control.h"
#include "save/trainer_data_savedata.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/rtc.h"
#include "system/str_tool.h"
#include "system/wordset.h"
#include "save/playtime.h"
#include "save/trainer_data_savedata.h"

// The windows, in the order of the lines of message file 359
enum {
    REPORT_WINDOW_NAME,
    REPORT_WINDOW_DATE,
    REPORT_WINDOW_TIME,
    REPORT_WINDOW_LOCATION,
    REPORT_WINDOW_BADGES,
    REPORT_WINDOW_POKEDEX,
    REPORT_WINDOW_PLAY_TIME,
    REPORT_WINDOW_LAST_SAVE,
    REPORT_WINDOW_MAX,
};

// The actors: the party's icons, then the bar's
#define REPORT_BAR_FIRST 6
#define REPORT_BAR_COUNT 10
#define REPORT_ACTOR_MAX (REPORT_BAR_FIRST + REPORT_BAR_COUNT)

// doneFlags
#define REPORT_DONE_ICONS (1 << 0)
#define REPORT_DONE_WINDOWS (1 << 1)

// Frames between two of the bar's icons, and frames without one before the bar is filled at once
#define REPORT_BAR_WAIT 8
#define REPORT_BAR_TIMEOUT 1200

// Black 2's; twinned code confirms the offsets of gsys, save, printQueue, windows, font, actUnit, actors, chars,
// palettes, cellAnims, vblankTcb, barActive, changedSize, totalSize, barThreshold, barStep, barCount and barWait in
// Black
struct ReportScreen {
    GameSystem *gsys;
    SaveControl *save;
    PrintQueue *printQueue;
    PrintWindow windows[REPORT_WINDOW_MAX];
    Font *font;
    ClActUnit *actUnit;
    ClActor *actors[REPORT_ACTOR_MAX];
    u32 chars[REPORT_BAR_FIRST + 1];
    u8 unused_b0[0x20];
    u32 palettes[2];
    u8 unused_d8[0x2c];
    u32 cellAnims[2];
    TCB *vblankTcb;
    BOOL barActive;
    u32 changedSize;
    u32 totalSize;
    u32 barThreshold;
    u32 barStep;
    u16 barCount;
    u16 barWait;
    u32 doneFlags;
    u16 heapId;
    u16 stallFrames;
};

// Black 2's, not checked against Black
typedef struct {
    u8 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u8 palette;
} ReportWindowSetup;

static void func_ov034_021f0114(HeapID heapId);
static void func_ov034_021f01f8(void);
static void func_ov034_021f0228(HeapID heapId);
void func_ov034_021f0284(ReportScreen *wk);
static void func_ov034_021f0700(ReportScreen *wk);
static void func_ov034_021f0724(ReportScreen *wk);
static void func_ov034_021f0930(ReportScreen *wk);
void func_ov034_021f09bc(TCB *tcb, void *data);

// pokebw2's Report_Create
ReportScreen *func_ov034_021efec0(GameSystem *gsys, HeapID heapId) {
    ReportScreen *wk = GFL_HeapAllocate(heapId, sizeof(ReportScreen), FALSE, "report.c", 276);

    wk->gsys = gsys;
    wk->heapId = heapId;
    wk->barActive = FALSE;
    wk->doneFlags = 0;
    func_ov034_021f0114(wk->heapId);
    func_ov034_021f0228(wk->heapId);
    func_ov034_021f0724(wk);
    func_ov034_021f0284(wk);
    func_0203d34c(FALSE, heapId);
    wk->vblankTcb = GFL_VBlankTCBAdd(func_ov034_021f09bc, wk, 0);
    return wk;
}

// pokebw2's Report_Free
void func_ov034_021eff34(ReportScreen *wk) {
    GFL_TCBRemove(wk->vblankTcb);
    func_ov034_021f0700(wk);
    func_ov034_021f0930(wk);
    func_ov034_021f01f8();
    GFL_HeapFree(wk);
}

// pokebw2's Report_Update
void func_ov034_021eff5c(ReportScreen *wk) {
    PokeParty *party;
    u32 i;

    if (!(wk->doneFlags & REPORT_DONE_ICONS) && func_0201c6b4(wk->printQueue) == TRUE) {
        party = GameData_GetParty(GSYS_GetGameData(wk->gsys));
        for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
            func_0204b3dc(wk->actors[i], TRUE);
        }
        wk->doneFlags |= REPORT_DONE_ICONS;
    }
}

// pokebw2's Report_UpdateAltFrame
void func_ov034_021effb4(ReportScreen *wk) {
    u32 i;

    if (!(wk->doneFlags & REPORT_DONE_WINDOWS) && func_0201c6b4(wk->printQueue) == TRUE) {
        GFL_BGSysSetBGEnabled(4, TRUE);
        GFL_BGSysSetBGEnabled(5, TRUE);
        GFL_BGSysSetBGEnabled(6, TRUE);
        for (i = 0; i < REPORT_WINDOW_MAX; i++) {
            BmpWin_FlushMap(wk->windows[i].window);
        }
        GFL_BGSysQueueScrLoad(5);
        wk->doneFlags |= REPORT_DONE_WINDOWS;
    }
    func_0201c4e4(wk->printQueue);
    for (i = 0; i < REPORT_WINDOW_MAX; i++) {
        PrintWindow_Flush(&wk->windows[i], wk->printQueue);
    }
}

// pokebw2's Report_IsReady
BOOL func_ov034_021f0044(ReportScreen *wk) {
    if (wk->doneFlags == (REPORT_DONE_ICONS | REPORT_DONE_WINDOWS)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's Report_InitSave
void func_ov034_021f0058(ReportScreen *wk) {
    wk->save = GameData_GetSaveControl(GSYS_GetGameData(wk->gsys));
    func_0200723c(wk->save, &wk->changedSize, &wk->totalSize);
    wk->barThreshold = wk->barStep = (wk->changedSize << 9) / REPORT_BAR_COUNT;
    wk->barCount = 0;
    wk->barWait = 0;
}

// pokebw2's Report_IsLargeSave
BOOL func_ov034_021f009c(ReportScreen *wk) {
    if (wk->changedSize >= wk->totalSize / 3) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's Report_StartBar
void func_ov034_021f00bc(ReportScreen *wk) {
    BmpWin *window = wk->windows[REPORT_WINDOW_LAST_SAVE].window;
    u32 i;

    func_020456f8(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
    for (i = REPORT_BAR_FIRST; i <= REPORT_ACTOR_MAX - 1; i++) {
        func_0204b3dc(wk->actors[i], TRUE);
    }
    wk->barActive = TRUE;
}

// pokebw2's Report_IsBarFull
BOOL func_ov034_021f00f0(ReportScreen *wk) {
    if (wk->barCount == REPORT_BAR_COUNT) {
        wk->barActive = FALSE;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's Report_StopBar
void func_ov034_021f0108(ReportScreen *wk) {
    wk->barActive = FALSE;
}

// pokebw2's Report_SetupBGs
static void func_ov034_021f0114(HeapID heapId) {
    GFL_BGSysSetBGEnabled(4, FALSE);
    GFL_BGSysSetBGEnabled(5, FALSE);
    GFL_BGSysSetBGEnabled(6, FALSE);
    GFL_BGSysSetBGEnabled(7, FALSE);
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0x3800,
            GX_BG_CHARBASE_0x00000,
            0x2800,
            GX_BG_EXTPLTT_01,
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(4);
        GFL_BGSysClearCharCore(4, 0x20, 0, heapId);
        GFL_BGSysLoadScr(4);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0x3000,
            GX_BG_CHARBASE_0x04000,
            0x4000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(5);
        GFL_BGSysClearCharCore(5, 0x20, 0, heapId);
        GFL_BGSysLoadScr(5);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0x2800,
            GX_BG_CHARBASE_0x04000,
            0x4000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(6);
        GFL_BGSysClearCharCore(6, 0x20, 0, heapId);
        GFL_BGSysLoadScr(6);
    }
}

// pokebw2's Report_ReleaseBGs
static void func_ov034_021f01f8(void) {
    GFL_BGSysSetBGEnabled(6, FALSE);
    GFL_BGSysSetBGEnabled(5, FALSE);
    GFL_BGSysSetBGEnabled(4, FALSE);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(4);
}

// pokebw2's Report_LoadGraphics
static void func_ov034_021f0228(HeapID heapId) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(196, heapId);

    // Each version has its own palette
#ifdef BLACK2
    GFL_G2DIOLoadArcNCLRDefault(arc, 3, PALTYPE_SUB_BG, 0, 0x60, heapId);
#else
    GFL_G2DIOLoadArcNCLRDefault(arc, 2, PALTYPE_SUB_BG, 0, 0x60, heapId);
#endif
    GFL_BGSysResetStdPalette(4, 0);
    GFL_BGSysLoadArcNCGRStatic(arc, 1, 4, 0, 0, TRUE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0, 4, 0, 0, TRUE, heapId);
    GFL_ArcToolFree(arc);
}

// pokebw2's sWindowSetups
static const ReportWindowSetup data_ov034_021f0a70[REPORT_WINDOW_MAX] = {
#include "asset/report_data_ov034_021f0a70.inc"
};

void func_ov034_021f0284(ReportScreen *wk) {
    MsgData *msgData;
    WordSet *wordSet;
    StrBuf *strbuf;
    StrBuf *fmt;
    GameData *gameData;
    SaveControl *save;
    PlayTime *playTime;
    u32 i;

    wk->printQueue = func_0201c440(wk->heapId);
    for (i = 0; i < REPORT_WINDOW_MAX; i++) {
        wk->windows[i].window = BmpWin_CreateDynamic(data_ov034_021f0a70[i].bg, data_ov034_021f0a70[i].x,
            data_ov034_021f0a70[i].y, data_ov034_021f0a70[i].width, data_ov034_021f0a70[i].height,
            data_ov034_021f0a70[i].palette, TRUE);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[i].window), 0);
    }

    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, wk->heapId);
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_0359, wk->heapId);
    wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
    strbuf = GFL_StrBufCreate(0x100, wk->heapId);
    gameData = GSYS_GetGameData(wk->gsys);
    save = GameData_GetSaveControl(gameData);

    fmt = GFL_MsgDataLoadStrbufNew(msgData, REPORT_WINDOW_NAME);
    copyVarForText(wordSet, 0, GetGameDataPlayerInfo(gameData));
    GFL_WordSetFormatStrbuf(wordSet, strbuf, fmt);
    PrintWindow_Print(&wk->windows[REPORT_WINDOW_NAME], wk->printQueue, 0, 4, strbuf, wk->font, PRINT_COLOR(13, 12, 0));
    GFL_StrBufFree(fmt);

    {
        RTCDate date;

        fmt = GFL_MsgDataLoadStrbufNew(msgData, REPORT_WINDOW_DATE);
        RTC_GetDate(&date);
        WordSetNumber(wordSet, 0, date.year, 2, NUM_PAD_ZERO, TRUE);
        WordSetNumber(wordSet, 1, date.month, 2, NUM_PAD_NONE, TRUE);
        WordSetNumber(wordSet, 2, date.day, 2, NUM_PAD_NONE, TRUE);
        GFL_WordSetFormatStrbuf(wordSet, strbuf, fmt);
        PrintWindow_Print(&wk->windows[REPORT_WINDOW_DATE], wk->printQueue, 0, 0, strbuf, wk->font, PRINT_COLOR(15, 14, 0));
        GFL_StrBufFree(fmt);
    }

    {
        RTCTime time;

        fmt = GFL_MsgDataLoadStrbufNew(msgData, REPORT_WINDOW_TIME);
        RTC_GetTime(&time);
        WordSetNumber(wordSet, 0, time.hour, 2, NUM_PAD_NONE, TRUE);
        WordSetNumber(wordSet, 1, time.minute, 2, NUM_PAD_ZERO, TRUE);
        GFL_WordSetFormatStrbuf(wordSet, strbuf, fmt);
        PrintWindow_Print(&wk->windows[REPORT_WINDOW_TIME], wk->printQueue, 0, 0, strbuf, wk->font, PRINT_COLOR(15, 14, 0));
        GFL_StrBufFree(fmt);
    }

    fmt = GFL_MsgDataLoadStrbufNew(msgData, REPORT_WINDOW_LOCATION);
    loadLocationNameToStrbuf(wordSet, 0,
        ZoneData_GetPlaceNameID(PlayerState_GetZoneID(GSYS_GetPlayerState(wk->gsys))));
    GFL_WordSetFormatStrbuf(wordSet, strbuf, fmt);
    PrintWindow_Print(&wk->windows[REPORT_WINDOW_LOCATION], wk->printQueue, 0, 0, strbuf, wk->font, PRINT_COLOR(15, 14, 0));
    GFL_StrBufFree(fmt);

    fmt = GFL_MsgDataLoadStrbufNew(msgData, REPORT_WINDOW_BADGES);
    WordSetNumber(wordSet, 0, getBadgeCount(getTrainerCardDataBlkAddress(gameData)), 2, NUM_PAD_NONE, TRUE);
    GFL_WordSetFormatStrbuf(wordSet, strbuf, fmt);
    PrintWindow_Print(&wk->windows[REPORT_WINDOW_BADGES], wk->printQueue, 0, 0, strbuf, wk->font, PRINT_COLOR(15, 14, 0));
    GFL_StrBufFree(fmt);

    if (EventWork_FlagGet(GameData_GetEventWork(GSYS_GetGameData(wk->gsys)), 0x962) == TRUE) {
        fmt = GFL_MsgDataLoadStrbufNew(msgData, REPORT_WINDOW_POKEDEX);
        WordSetNumber(wordSet, 0, func_0200d0ac(GameData_GetPokedex(gameData), wk->heapId), 3, NUM_PAD_NONE, TRUE);
        GFL_WordSetFormatStrbuf(wordSet, strbuf, fmt);
        PrintWindow_Print(&wk->windows[REPORT_WINDOW_POKEDEX], wk->printQueue, 0, 0, strbuf, wk->font, PRINT_COLOR(15, 14, 0));
        GFL_StrBufFree(fmt);
    }

    fmt = GFL_MsgDataLoadStrbufNew(msgData, REPORT_WINDOW_PLAY_TIME);
    playTime = func_02012fcc(gameData);
    WordSetNumber(wordSet, 0, func_0200864c(playTime), 3, NUM_PAD_NONE, TRUE);
    WordSetNumber(wordSet, 1, func_02008650(playTime), 2, NUM_PAD_ZERO, TRUE);
    GFL_WordSetFormatStrbuf(wordSet, strbuf, fmt);
    PrintWindow_Print(&wk->windows[REPORT_WINDOW_PLAY_TIME], wk->printQueue, 0, 0, strbuf, wk->font, PRINT_COLOR(15, 14, 0));
    GFL_StrBufFree(fmt);

    if (!func_020071f0(save) && SaveControl_IsDataAlreadyPresent(save) == TRUE) {
        PlayTime *saveTime;

        saveTime = func_02008748(save);
        fmt = GFL_MsgDataLoadStrbufNew(msgData, REPORT_WINDOW_LAST_SAVE);
        WordSetNumber(wordSet, 0, func_020086c8(saveTime), 2, NUM_PAD_ZERO, TRUE);
        WordSetNumber(wordSet, 1, func_020086d0(saveTime), 2, NUM_PAD_NONE, TRUE);
        WordSetNumber(wordSet, 2, func_020086d8(saveTime), 2, NUM_PAD_NONE, TRUE);
        WordSetNumber(wordSet, 3, func_020086e0(saveTime), 2, NUM_PAD_NONE, TRUE);
        WordSetNumber(wordSet, 4, func_020086e8(saveTime), 2, NUM_PAD_ZERO, TRUE);
        GFL_WordSetFormatStrbuf(wordSet, strbuf, fmt);
        PrintWindow_Print(&wk->windows[REPORT_WINDOW_LAST_SAVE], wk->printQueue, 0, 4, strbuf, wk->font, PRINT_COLOR(13, 12, 0));
        GFL_StrBufFree(fmt);
    } else {
        BmpWin_FlushChar(wk->windows[REPORT_WINDOW_LAST_SAVE].window);
    }

    GFL_StrBufFree(strbuf);
    GFL_WordSetSystemFree(wordSet);
    GFL_MsgDataFree(msgData);
}


// pokebw2's Report_FreeWindows
static void func_ov034_021f0700(ReportScreen *wk) {
    u32 i;

    GFL_FontFree(wk->font);
    for (i = 0; i < REPORT_WINDOW_MAX; i++) {
        BmpWin_Free(wk->windows[i].window);
    }
    func_0201c4c0(wk->printQueue);
}

// The first of the party's icons, and the first of the bar's
// pokebw2's sReportActorSetups
static const ClActorSetup data_ov034_021f0a60[2] = {
    { 40, 104, 0, 0, 0 },
    { 74, 180, 0, 0, 0 },
};

// pokebw2's Report_CreateActors
static void func_ov034_021f0724(ReportScreen *wk) {
    PokeParty *party = GameData_GetParty(GSYS_GetGameData(wk->gsys));
    u32 count = PokeParty_GetPkmCount(party);
    ArcTool *arc;
    ClActorSetup setup;
    u32 i;

    wk->actUnit = func_0204b100(REPORT_ACTOR_MAX, 0, wk->heapId);

    arc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, wk->heapId);
    for (i = 0; i < count; i++) {
        wk->chars[i] = func_0204a6c8(arc, func_0201ba68(func_0201d624(PokeParty_GetPkm(party, i))), FALSE,
                                     CLACT_VRAM_SUB, wk->heapId);
    }
    wk->palettes[0] = func_0204acec(arc, func_0201bc3c(), CLACT_VRAM_SUB, 0, wk->heapId);
    wk->cellAnims[0] = func_0204af28(arc, func_0201bc40(), getOBJTileMapping_MainEng(), wk->heapId);
    GFL_ArcToolFree(arc);

    arc = GFL_ArcSysCreateFileHandle(196, wk->heapId);
    wk->chars[REPORT_BAR_FIRST] = func_0204a6c8(arc, 3, TRUE, CLACT_VRAM_SUB, wk->heapId);
    wk->palettes[1] = func_0204ac18(arc, 4, CLACT_VRAM_SUB, 0x60, 0, 1, wk->heapId);
    wk->cellAnims[1] = func_0204af28(arc, 5, 6, wk->heapId);
    GFL_ArcToolFree(arc);

    setup = data_ov034_021f0a60[0];
    for (i = 0; i < count; i++) {
        PartyPkm *pkm = PokeParty_GetPkm(party, i);

        wk->actors[i] = func_0204b294(wk->actUnit, wk->chars[i], wk->palettes[0], wk->cellAnims[0], &setup,
                                      CLACT_SURFACE_SUB, wk->heapId);
        func_0204b3dc(wk->actors[i], FALSE);
        setup.x += 32;
        func_0204b6f4(wk->actors[i], func_0201bbe8(func_0201d624(pkm)), TRUE);
    }

    setup = data_ov034_021f0a60[1];
    for (i = REPORT_BAR_FIRST; i <= REPORT_ACTOR_MAX - 1; i++) {
        wk->actors[i] = func_0204b294(wk->actUnit, wk->chars[REPORT_BAR_FIRST], wk->palettes[1], wk->cellAnims[1],
                                      &setup, CLACT_SURFACE_SUB, wk->heapId);
        func_0204b3dc(wk->actors[i], FALSE);
        setup.x += 12;
    }

    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's Report_FreeActors
static void func_ov034_021f0930(ReportScreen *wk) {
    PokeParty *party = GameData_GetParty(GSYS_GetGameData(wk->gsys));
    u32 i;

    for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
        func_0204b3b4(wk->actors[i]);
        func_0204a8d4(wk->chars[i]);
    }
    for (i = REPORT_BAR_FIRST; i <= REPORT_ACTOR_MAX - 1; i++) {
        func_0204b3b4(wk->actors[i]);
    }
    func_0204a8d4(wk->chars[REPORT_BAR_FIRST]);
    func_0204ada4(wk->palettes[0]);
    func_0204ada4(wk->palettes[1]);
    func_0204afd8(wk->cellAnims[0]);
    func_0204afd8(wk->cellAnims[1]);
    func_0204b1cc(wk->actUnit);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, FALSE);
}

void func_ov034_021f09bc(TCB *tcb, void *data) {
    ReportScreen *wk = data;
    u32 written;

    if (wk->barActive) {
        written = func_020071c0(wk->save) << 8;
        if (wk->barWait != REPORT_BAR_WAIT) {
            wk->barWait++;
        }
        if (written >= wk->barThreshold && wk->barWait == REPORT_BAR_WAIT && wk->barCount < REPORT_BAR_COUNT) {
            wk->barThreshold += wk->barStep;
            wk->barWait = 0;
            func_0204b8c4(wk->actors[REPORT_BAR_FIRST + wk->barCount], 0);
            func_0204b878(wk->actors[REPORT_BAR_FIRST + wk->barCount], 1);
            func_0204b92c(wk->actors[REPORT_BAR_FIRST + wk->barCount], 1);
            wk->barCount++;
        }
        func_0204a600();
        func_0204a648();
    }
}

