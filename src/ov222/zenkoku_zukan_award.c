// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov315/zenkoku_zukan_award.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/zenkoku_zukan_award_graphic.h"
#include "app/zukan_award.h"
#include "constants/arc.h"
#include "constants/script_text_banks.h"
#include "constants/sound.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
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
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "save/config.h"
#include "save/player_info.h"
#include "save/trainer_data_savedata.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/script/title.h"
#include "constants/pokemon.h"

// The national Pokédex diploma: the certificate for completing the national Pokédex. It prints the player's name and
// the text on the top screen, slides a ribbon in and plays a jingle, and A, B or a touch closes it.
// Named after the ROM's "zenkoku_zukan_award.c" (the other file of the overlay is zenkoku_zukan_award_graphic.c)

#define BG_MAIN_BACKDROP 2
#define BG_MAIN_DIPLOMA 1
#define BG_SUB_C_GEAR 4

#define WINDOW_COUNT 5
#define WINDOW_PLAYER_NAME 2
#define WINDOW_CERTIFIES 3
#define WINDOW_STAFF 4

// The steps of the process
enum {
    STEP_WAIT_TEXT,
    STEP_WAIT_FADE_IN,
    STEP_WAIT_SLIDE,
    STEP_WAIT_JINGLE,
    STEP_WAIT_INPUT,
    STEP_WAIT_FADE_OUT,
    STEP_END,
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} ZenkokuZukanAwardBGSetup;

typedef struct {
    u8 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u8 palette;
    u8 fromEnd;
    u8 unk7;
    u8 unk8;
} ZenkokuZukanAwardWindowSetup;

typedef struct {
    u8 x;
    u8 y;
    u8 unk2;
    u8 unk3;
} ZenkokuZukanAwardWindowPos;

typedef struct {
    u16 heapId;
    ZukanAwardParam *param;
    ZenkokuZukanAwardGraphic *graphic;
    Font *font;
    PrintQueue *printQueue;
    TCB *vblankTask;
    MsgData *msgData;
    BmpWin *windows[WINDOW_COUNT];
    BOOL printed[WINDOW_COUNT];
    u8 slideWait;
    u32 chars;
    u32 palette;
    u32 cellAnims;
    ClActor *ribbon;
} ZenkokuZukanAwardWork;

static BOOL func_ov222_021b95a0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov222_021b9748(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov222_021b97c4(GameProc *proc, u32 *state, void *param, void *work);
void func_ov222_021b98fc(TCB *tcb, void *data);
void func_ov222_021b9900(ZenkokuZukanAwardWork *wk);
void func_ov222_021b9960(ZenkokuZukanAwardWork *wk);
void func_ov222_021b9964(ZenkokuZukanAwardWork *wk);
void func_ov222_021b9b84(ZenkokuZukanAwardWork *wk);
static void func_ov222_021b9bb0(ZenkokuZukanAwardWork *wk);
static BOOL func_ov222_021b9bfc(ZenkokuZukanAwardWork *wk);
void func_ov222_021b9c1c(ZenkokuZukanAwardWork *wk);
void func_ov222_021b9ce4(ZenkokuZukanAwardWork *wk);
void func_ov222_021b9d64(ZenkokuZukanAwardWork *wk);
void func_ov222_021b9d84(ZenkokuZukanAwardWork *wk);
static void func_ov222_021b9dd0(ZenkokuZukanAwardWork *wk);
static BOOL func_ov222_021b9e44(ZenkokuZukanAwardWork *wk);

// pokebw2's sZenkokuZukanAwardRibbonSetup
static const ClActorSetup data_ov222_021ba1d8[1] = { { 18, 17, 0, 0, 1 } };

// pokebw2's sZenkokuZukanAwardSubBGSetups
static const ZenkokuZukanAwardBGSetup data_ov222_021ba1f4[1] = {
    { BG_SUB_C_GEAR,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x8000, GX_BG_CHARBASE_0x00000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
};

// pokebw2's sZenkokuZukanAwardWindowSetups
static const ZenkokuZukanAwardWindowSetup data_ov222_021ba220[WINDOW_COUNT] = {
    { BG_MAIN_BACKDROP, 0, 0, 1, 1, 15, 0, 0, 0 },   { BG_MAIN_BACKDROP, 0, 24, 1, 1, 15, 0, 0, 0 },
    { BG_MAIN_BACKDROP, 7, 4, 20, 3, 15, 0, 0, 0 },  { BG_MAIN_BACKDROP, 7, 8, 20, 9, 15, 0, 0, 0 },
    { BG_MAIN_BACKDROP, 6, 17, 20, 5, 15, 0, 0, 0 },
};

// Where the windows go when the text is in kanji (the Japanese version's setting)
// pokebw2's sZenkokuZukanAwardKanjiWindowPositions
static const ZenkokuZukanAwardWindowPos data_ov222_021ba1e0[WINDOW_COUNT] = {
    { 0, 0, 0, 0 }, { 0, 24, 0, 0 }, { 7, 4, 0, 0 }, { 6, 8, 0, 0 }, { 6, 17, 0, 0 },
};

// pokebw2's ZENKOKU_ZUKAN_AWARD_PROC_FUNCTIONS
// pokebw2's ZenkokuZukanAward_Init
// pokebw2's ZenkokuZukanAward_Main
const GameProcFunctions data_ov222_021ba250 = { func_ov222_021b95a0, func_ov222_021b97c4,
                                                               func_ov222_021b9748 };

// The NCLR, NCGR and NSCR files of the background, for each gender
static u32 data_ov222_021ba360[2][3] = {
    { 0xd, 0xe, 3 },
    { 6, 7, 3 },
};


static BOOL func_ov222_021b95a0(GameProc *proc, u32 *state, void *param, void *work) {
    ZenkokuZukanAwardWork *wk;
    PlayerInfo *playerInfo;
    ArcTool *arc;
    HeapID heapId;
    u32 index;
    int i;

    GFL_HeapCreateChild(HEAPID_USER, 0x83, 0x30000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(ZenkokuZukanAwardWork), 0x83);
    MI_CpuFill8(wk, 0, sizeof(ZenkokuZukanAwardWork));
    wk->heapId = 0x83;
    wk->param = param;
    wk->graphic = func_ov222_021b9f10(1, wk->heapId);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, wk->heapId);
    wk->printQueue = func_0201c440(wk->heapId);
    GFL_BGSysSetBGPriority(1, 1);
    GFL_BGSysSetBGPriority(2, 0);
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov222_021b98fc, wk, 1);
    func_ov222_021b9900(wk);
    func_ov222_021b9964(wk);
    func_ov222_021b9c1c(wk);
    func_ov222_021b9ce4(wk);
    playerInfo = GetGameDataPlayerInfo(wk->param->gameData);
    G2S_BlendNone();
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_DispOn();
    GFL_BGSysDisableAllB();
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GFL_BGSysDisableBGsB();
    for (i = 0; i < NELEMS(data_ov222_021ba1f4); i++) {
        GFL_BGSysCreateBG(data_ov222_021ba1f4[i].bg, &data_ov222_021ba1f4[i].setup, data_ov222_021ba1f4[i].mode);
        GFL_BGSysClearBG(data_ov222_021ba1f4[i].bg);
        GFL_BGSysSetBGEnabled(data_ov222_021ba1f4[i].bg, data_ov222_021ba1f4[i].enabled);
    }
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    index = 1;
    heapId = wk->heapId;
    if (playerInfo->gender != GENDER_FEMALE) {
        index = 0;
    }
    arc = GFL_ArcSysCreateFileHandle(ARCID_C_GEAR, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, data_ov222_021ba360[index][0], PALTYPE_SUB_BG, 0, 0, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, data_ov222_021ba360[index][1], BG_SUB_C_GEAR, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, data_ov222_021ba360[index][2], BG_SUB_C_GEAR, 0, 0, FALSE, heapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysLoadScr(BG_SUB_C_GEAR);
    func_ov222_021b9d84(wk);
    GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0x10, 0x10, 0);
    func_0203d34c(FALSE, wk->heapId);
    if (wk->param->unk04 == 0) {
        GFL_SndBGMFadeOut(6);
    }
    return TRUE;
}

static BOOL func_ov222_021b9748(GameProc *proc, u32 *state, void *param, void *work) {
    ZenkokuZukanAwardWork *wk = work;
    int i;

    for (i = 0; i < NELEMS(data_ov222_021ba1f4); i++) {
        GFL_BGSysReleaseBG(data_ov222_021ba1f4[i].bg);
    }
    G2S_BlendNone();
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    func_ov222_021b9d64(wk);
    func_ov222_021b9b84(wk);
    func_ov222_021b9960(wk);
    GFL_TCBRemove(wk->vblankTask);
    func_0201c6ec(wk->printQueue);
    func_0201c4c0(wk->printQueue);
    GFL_FontFree(wk->font);
    func_ov222_021b9fb8(wk->graphic);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(0x83);
    return TRUE;
}

static BOOL func_ov222_021b97c4(GameProc *proc, u32 *state, void *param, void *work) {
    ZenkokuZukanAwardWork *wk = work;
    BOOL done;
    u32 x;
    u32 y;

    switch (*state) {
    case STEP_WAIT_TEXT:
        done = TRUE;
        if (!func_ov222_021b9bfc(wk)) {
            done = FALSE;
        }
        if (done && wk->param->unk04 == 0) {
            if (GFL_SndBGMIsFading()) {
                done = FALSE;
            } else {
                GFL_SndBGMSetPaused(TRUE);
                GFL_SndBGMPush();
                GFL_SndBGMPlay(SEQ_ME_HYOUKA6, SND_CHANNEL_MASK_ALL);
            }
        }
        if (done) {
            GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 0);
            *state = STEP_WAIT_FADE_IN;
        }
        break;
    case STEP_WAIT_FADE_IN:
        if (!GFL_FadeIsRunning()) {
            if (wk->param->unk04 != 0) {
                *state = STEP_WAIT_INPUT;
            } else {
                *state = STEP_WAIT_SLIDE;
            }
        }
        break;
    case STEP_WAIT_SLIDE:
        if (func_ov222_021b9e44(wk)) {
            *state = STEP_WAIT_JINGLE;
        }
        break;
    case STEP_WAIT_JINGLE:
        if (!GFL_SndBGMIsPlaying()) {
            GFL_SndBGMPop();
            GFL_SndBGMSetPaused(FALSE);
            GFL_SndBGMFadeIn(6);
            *state = STEP_WAIT_INPUT;
        }
        break;
    case STEP_WAIT_INPUT:
        done = FALSE;
        if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            func_020355d8(FALSE);
            done = TRUE;
        } else if (func_02035d20(&x, &y)) {
            done = TRUE;
            func_020355d8(TRUE);
        }
        if (done) {
            *state = STEP_WAIT_FADE_OUT;
            GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 0);
        }
        break;
    case STEP_WAIT_FADE_OUT:
        if (!GFL_FadeIsRunning()) {
            *state = STEP_END;
        }
        break;
    case STEP_END:
        return TRUE;
    }
    func_0201c4e4(wk->printQueue);
    func_ov222_021b9bb0(wk);
    if ((s32)*state >= STEP_WAIT_FADE_IN && (s32)*state <= STEP_WAIT_FADE_OUT) {
        func_ov222_021b9dd0(wk);
    }
    func_ov222_021ba014(wk->graphic);
    return FALSE;
}

// pokebw2's ZenkokuZukanAward_VBlank
void func_ov222_021b98fc(TCB *tcb, void *data) {
}

// pokebw2's ZenkokuZukanAward_LoadGraphics
void func_ov222_021b9900(ZenkokuZukanAwardWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_AWARD, wk->heapId);

    GFL_G2DIOLoadArcNCLRDefault(arc, 2, PALTYPE_MAIN_BG, 0, 0x1e0, wk->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 5, BG_MAIN_DIPLOMA, 0, 0, FALSE, wk->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 7, BG_MAIN_DIPLOMA, 0, 0, FALSE, wk->heapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysLoadScr(BG_MAIN_DIPLOMA);
}

// pokebw2's ZenkokuZukanAward_Unused
void func_ov222_021b9960(ZenkokuZukanAwardWork *wk) {
}

void func_ov222_021b9964(ZenkokuZukanAwardWork *wk) {
    PlayerInfo *playerInfo = GetGameDataPlayerInfo(wk->param->gameData);
    u32 kanji = func_020083e4((const Config *)getTrainerDataBlkAddress(GameData_GetSaveControl(wk->param->gameData)));
    StrBuf *strbuf;
    StrBuf *certifies;
    StrBuf *staff;
    WordSet *wordSet;
    StrBuf *format;
    u16 textWidth;
    u16 windowWidth;
    s16 x;
    u8 i;

    wk->msgData = NULL;
    for (i = 0; i < WINDOW_COUNT; i++) {
        wk->windows[i] = NULL;
        wk->printed[i] = FALSE;
    }
    wk->printed[0] = TRUE;
    wk->printed[1] = TRUE;
    GFL_G2DIOLoadNCLR(ARCID_FONT, 5, PALTYPE_MAIN_BG, 0, 0x1e0, 0x20, wk->heapId);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SCRIPT_MESSAGE, SCRIPT_TEXT_TITLE, wk->heapId);
    for (i = 0; i < WINDOW_COUNT; i++) {
        wk->windows[i] = BmpWin_CreateDynamic(data_ov222_021ba220[i].bg, data_ov222_021ba220[i].x,
                                              data_ov222_021ba220[i].y, data_ov222_021ba220[i].width,
                                              data_ov222_021ba220[i].height, data_ov222_021ba220[i].palette,
                                              data_ov222_021ba220[i].fromEnd);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[i]), 0);
        BmpWin_FlushChar(wk->windows[i]);
    }
    if (kanji == 1) {
        for (i = 0; i < WINDOW_COUNT; i++) {
            func_02045780(wk->windows[i], data_ov222_021ba1e0[i].x);
            func_02045788(wk->windows[i], data_ov222_021ba1e0[i].y);
        }
    }

    wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
    format = GFL_MsgDataLoadStrbufNew(wk->msgData, ScriptTitle_Text_004);
    strbuf = GFL_StrBufCreate(32, wk->heapId);
    x = 0;
    copyVarForText(wordSet, 0, playerInfo);
    GFL_WordSetFormatStrbuf(wordSet, strbuf, format);
    GFL_FontGetBlockWidth(strbuf, wk->font, 0);
    GFL_BitmapGetWidth(BmpWin_GetBitmap(wk->windows[WINDOW_PLAYER_NAME]));
    func_0201c724(wk->printQueue, BmpWin_GetBitmap(wk->windows[WINDOW_PLAYER_NAME]), 0, 0, strbuf, wk->font,
                  PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(strbuf);
    GFL_StrBufFree(format);
    GFL_WordSetSystemFree(wordSet);

    certifies = GFL_MsgDataLoadStrbufNew(wk->msgData, ScriptTitle_Text_006);
    textWidth = GFL_FontGetBlockWidth(certifies, wk->font, 0);
    windowWidth = GFL_BitmapGetWidth(BmpWin_GetBitmap(wk->windows[WINDOW_CERTIFIES]));
    if (windowWidth >= textWidth && kanji == 1) {
        x = (windowWidth - textWidth) / 2;
    }
    func_0201c724(wk->printQueue, BmpWin_GetBitmap(wk->windows[WINDOW_CERTIFIES]), x, 0, certifies, wk->font,
                  PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(certifies);

    staff = GFL_MsgDataLoadStrbufNew(wk->msgData, ScriptTitle_Text_005);
    GFL_FontGetBlockWidth(staff, wk->font, 0);
    GFL_BitmapGetWidth(BmpWin_GetBitmap(wk->windows[WINDOW_STAFF]));
    func_0201c724(wk->printQueue, BmpWin_GetBitmap(wk->windows[WINDOW_STAFF]), 0, 0, staff, wk->font,
                  PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(staff);

    for (i = 1; i < WINDOW_COUNT; i++) {
        BmpWin_Transfer(wk->windows[i]);
    }
    func_ov222_021b9bb0(wk);
}


// pokebw2's ZenkokuZukanAward_FreeText
void func_ov222_021b9b84(ZenkokuZukanAwardWork *wk) {
    u8 i;

    for (i = 0; i < WINDOW_COUNT; i++) {
        if (wk->windows[i] != NULL) {
            BmpWin_Free(wk->windows[i]);
        }
    }
    if (wk->msgData != NULL) {
        GFL_MsgDataFree(wk->msgData);
    }
}

// Sends the windows that have finished printing to the screen
// pokebw2's ZenkokuZukanAward_TransferPrinted
static void func_ov222_021b9bb0(ZenkokuZukanAwardWork *wk) {
    u8 i;

    for (i = 0; i < WINDOW_COUNT; i++) {
        if (!wk->printed[i] && !func_0201c6c4(wk->printQueue, BmpWin_GetBitmap(wk->windows[i]))) {
            BmpWin_Transfer(wk->windows[i]);
            wk->printed[i] = TRUE;
        }
    }
}

// pokebw2's ZenkokuZukanAward_IsAllPrinted
static BOOL func_ov222_021b9bfc(ZenkokuZukanAwardWork *wk) {
    u8 i;

    for (i = 0; i < WINDOW_COUNT; i++) {
        if (!wk->printed[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

// Fills the BG behind the diploma's text with one color
// pokebw2's ZenkokuZukanAward_InitBackdrop
void func_ov222_021b9c1c(ZenkokuZukanAwardWork *wk) {
    u16 *palette = GFL_HeapAllocate(wk->heapId, 0x20, TRUE, "zenkoku_zukan_award.c", 987);
    u16 *screen;
    u16 entry;
    u8 x;
    u8 y;
    u16 i = 0;

    palette[0] = GX_RGB(0, 0, 0);
    palette[1] = GX_RGB(9, 9, 7);
    palette[2] = GX_RGB(16, 16, 12);
    palette[3] = GX_RGB(0, 0, 0);
    GFL_BGSysUploadStdPalette(BG_MAIN_BACKDROP, palette, 0x20, 0x1e0);
    GFL_HeapFree(palette);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[1]), 3);
    BmpWin_FlushChar(wk->windows[1]);
    screen = GFL_HeapAllocate(wk->heapId, 32 * 24 * sizeof(u16), TRUE, "zenkoku_zukan_award.c", 1004);
    entry = func_02045768(wk->windows[1]);
    entry |= 15 << 12;
    for (y = 0; y < 24; y++) {
        for (x = 0; x < 32; x++) {
            screen[i] = entry;
            i++;
        }
    }
    GFL_BGSysLoadScrAreaAll(BG_MAIN_BACKDROP, screen, 0, 24, 32, 24);
    GFL_BGSysLoadScr(BG_MAIN_BACKDROP);
    GFL_HeapFree(screen);
}

// pokebw2's ZenkokuZukanAward_InitRibbon
void func_ov222_021b9ce4(ZenkokuZukanAwardWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_AWARD, wk->heapId);

    wk->palette = func_0204ac18(arc, 0, 0, 0, 0, 2, wk->heapId);
    wk->chars = func_0204a6c8(arc, 3, FALSE, 0, wk->heapId);
    wk->cellAnims = func_0204af28(arc, 8, 9, wk->heapId);
    GFL_ArcToolFree(arc);
    wk->ribbon = func_0204b294(func_ov222_021ba028(wk->graphic), wk->chars, wk->palette,
                               wk->cellAnims, data_ov222_021ba1d8, 0, wk->heapId);
    func_0204b3dc(wk->ribbon, TRUE);
    func_0204b92c(wk->ribbon, TRUE);
}

// pokebw2's ZenkokuZukanAward_FreeRibbon
void func_ov222_021b9d64(ZenkokuZukanAwardWork *wk) {
    func_0204b3b4(wk->ribbon);
    func_0204afd8(wk->cellAnims);
    func_0204a8d4(wk->chars);
    func_0204ada4(wk->palette);
}

// Puts the diploma's BGs and the ribbon below the screen, to slide up
// pokebw2's ZenkokuZukanAward_StartSlide
void func_ov222_021b9d84(ZenkokuZukanAwardWork *wk) {
    ClActorPos pos;
    u32 i;

    if (wk->param->unk04 == 0) {
        GFL_BGSysMoveBG(BG_MAIN_DIPLOMA, BG_MOVE_SET_Y, 64);
        GFL_BGSysMoveBG(BG_MAIN_BACKDROP, BG_MOVE_SET_Y, 64);
        for (i = 0; i < NELEMS(data_ov222_021ba1d8); i++) {
            pos.x = data_ov222_021ba1d8[i].x;
            pos.y = data_ov222_021ba1d8[i].y - 64;
            func_0204b404(wk->ribbon, &pos, 0);
        }
        wk->slideWait = 0;
    }
}

// pokebw2's ZenkokuZukanAward_UpdateSlide
static void func_ov222_021b9dd0(ZenkokuZukanAwardWork *wk) {
    ClActorPos pos;
    int y;

    if (wk->param->unk04 == 0) {
        y = GFL_BGSysGetBGOffsetY(BG_MAIN_DIPLOMA);
        if (y != 0) {
            if (wk->slideWait == 0) {
                if (y >= 2) {
                    y = 2;
                }
                GFL_BGSysMoveBGReq(BG_MAIN_DIPLOMA, BG_MOVE_UP, y);
                GFL_BGSysMoveBGReq(BG_MAIN_BACKDROP, BG_MOVE_UP, y);
                func_0204b454(wk->ribbon, &pos, 0);
                pos.y += y;
                func_0204b404(wk->ribbon, &pos, 0);
                wk->slideWait = 0;
            } else {
                wk->slideWait--;
            }
        }
    }
}

// pokebw2's ZenkokuZukanAward_IsSlideDone
static BOOL func_ov222_021b9e44(ZenkokuZukanAwardWork *wk) {
    BOOL done;
    BOOL result;

    if (wk->param->unk04 != 0) {
        return TRUE;
    }
    done = TRUE;
    if (GFL_BGSysGetBGOffsetY(BG_MAIN_DIPLOMA) != 0) {
        done = FALSE;
    }
    result = FALSE;
    if (done) {
        result = TRUE;
    }
    return result;
}
