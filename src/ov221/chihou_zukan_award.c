// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov314/chihou_zukan_award.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/chihou_zukan_award_graphic.h"
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
#include "save/player_info.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/script/title.h"
#include "constants/pokemon.h"

// The regional Pokédex diploma: the certificate for catching every Pokémon of the Unova Pokédex. It prints the
// player's name and the text on the top screen, slides a ribbon in and plays a jingle, and A, B or a touch closes it.
// Named after the ROM's "chihou_zukan_award.c" (the other file of the overlay is chihou_zukan_award_graphic.c)

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
} ChihouZukanAwardBGSetup;

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
} ChihouZukanAwardWindowSetup;

typedef struct {
    u16 heapId;
    ZukanAwardParam *param;
    ChihouZukanAwardGraphic *graphic;
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
} ChihouZukanAwardWork;

static BOOL func_ov221_021b95a0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov221_021b9748(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov221_021b97c4(GameProc *proc, u32 *state, void *param, void *work);
void func_ov221_021b98fc(TCB *tcb, void *data);
void func_ov221_021b9900(ChihouZukanAwardWork *wk);
void func_ov221_021b9960(ChihouZukanAwardWork *wk);
void func_ov221_021b9964(ChihouZukanAwardWork *wk);
void func_ov221_021b9aec(ChihouZukanAwardWork *wk);
static void func_ov221_021b9b18(ChihouZukanAwardWork *wk);
static BOOL func_ov221_021b9b64(ChihouZukanAwardWork *wk);
void func_ov221_021b9b84(ChihouZukanAwardWork *wk);
void func_ov221_021b9c4c(ChihouZukanAwardWork *wk);
void func_ov221_021b9ccc(ChihouZukanAwardWork *wk);
void func_ov221_021b9cec(ChihouZukanAwardWork *wk);
static void func_ov221_021b9d38(ChihouZukanAwardWork *wk);
static BOOL func_ov221_021b9dac(ChihouZukanAwardWork *wk);

// pokebw2's sChihouZukanAwardRibbonSetup
static const ClActorSetup data_ov221_021ba140[1] = { { 15, 13, 1, 0, 1 } };

// pokebw2's sChihouZukanAwardSubBGSetups
static const ChihouZukanAwardBGSetup data_ov221_021ba148[1] = {
    { BG_SUB_C_GEAR,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x8000, GX_BG_CHARBASE_0x00000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
};

// pokebw2's sChihouZukanAwardWindowSetups
static const ChihouZukanAwardWindowSetup data_ov221_021ba174[WINDOW_COUNT] = {
    { BG_MAIN_BACKDROP, 0, 0, 1, 1, 15, 0, 0, 0 },   { BG_MAIN_BACKDROP, 0, 24, 1, 1, 15, 0, 0, 0 },
    { BG_MAIN_BACKDROP, 1, 3, 30, 3, 15, 0, 0, 0 },  { BG_MAIN_BACKDROP, 1, 7, 30, 9, 15, 0, 0, 0 },
    { BG_MAIN_BACKDROP, 1, 16, 30, 5, 15, 0, 0, 0 },
};

// pokebw2's CHIHOU_ZUKAN_AWARD_PROC_FUNCTIONS
// pokebw2's ChihouZukanAward_Init
// pokebw2's ChihouZukanAward_Main
const GameProcFunctions data_ov221_021ba1a4 = { func_ov221_021b95a0, func_ov221_021b97c4,
                                                              func_ov221_021b9748 };

// The NCLR, NCGR and NSCR files of the background, for each gender
static u32 data_ov221_021ba2c0[2][3] = {
    { 0xd, 0xe, 3 },
    { 6, 7, 3 },
};


static BOOL func_ov221_021b95a0(GameProc *proc, u32 *state, void *param, void *work) {
    ChihouZukanAwardWork *wk;
    PlayerInfo *playerInfo;
    ArcTool *arc;
    HeapID heapId;
    u32 index;
    int i;

    GFL_HeapCreateChild(HEAPID_USER, 0x83, 0x30000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(ChihouZukanAwardWork), 0x83);
    MI_CpuFill8(wk, 0, sizeof(ChihouZukanAwardWork));
    wk->heapId = 0x83;
    wk->param = param;
    wk->graphic = func_ov221_021b9e78(1, wk->heapId);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, wk->heapId);
    wk->printQueue = func_0201c440(wk->heapId);
    GFL_BGSysSetBGPriority(1, 1);
    GFL_BGSysSetBGPriority(2, 0);
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov221_021b98fc, wk, 1);
    func_ov221_021b9900(wk);
    func_ov221_021b9964(wk);
    func_ov221_021b9b84(wk);
    func_ov221_021b9c4c(wk);
    playerInfo = GetGameDataPlayerInfo(wk->param->gameData);
    G2S_BlendNone();
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_DispOn();
    GFL_BGSysDisableAllB();
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GFL_BGSysDisableBGsB();
    for (i = 0; i < NELEMS(data_ov221_021ba148); i++) {
        GFL_BGSysCreateBG(data_ov221_021ba148[i].bg, &data_ov221_021ba148[i].setup, data_ov221_021ba148[i].mode);
        GFL_BGSysClearBG(data_ov221_021ba148[i].bg);
        GFL_BGSysSetBGEnabled(data_ov221_021ba148[i].bg, data_ov221_021ba148[i].enabled);
    }
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    index = 1;
    heapId = wk->heapId;
    if (playerInfo->gender != GENDER_FEMALE) {
        index = 0;
    }
    arc = GFL_ArcSysCreateFileHandle(ARCID_C_GEAR, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, data_ov221_021ba2c0[index][0], PALTYPE_SUB_BG, 0, 0, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, data_ov221_021ba2c0[index][1], BG_SUB_C_GEAR, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, data_ov221_021ba2c0[index][2], BG_SUB_C_GEAR, 0, 0, FALSE, heapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysLoadScr(BG_SUB_C_GEAR);
    func_ov221_021b9cec(wk);
    GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0x10, 0x10, 0);
    func_0203d34c(FALSE, wk->heapId);
    if (wk->param->unk04 == 0) {
        GFL_SndBGMFadeOut(6);
    }
    return TRUE;
}

static BOOL func_ov221_021b9748(GameProc *proc, u32 *state, void *param, void *work) {
    ChihouZukanAwardWork *wk = work;
    int i;

    for (i = 0; i < NELEMS(data_ov221_021ba148); i++) {
        GFL_BGSysReleaseBG(data_ov221_021ba148[i].bg);
    }
    G2S_BlendNone();
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    func_ov221_021b9ccc(wk);
    func_ov221_021b9aec(wk);
    func_ov221_021b9960(wk);
    GFL_TCBRemove(wk->vblankTask);
    func_0201c6ec(wk->printQueue);
    func_0201c4c0(wk->printQueue);
    GFL_FontFree(wk->font);
    func_ov221_021b9f20(wk->graphic);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(0x83);
    return TRUE;
}

static BOOL func_ov221_021b97c4(GameProc *proc, u32 *state, void *param, void *work) {
    ChihouZukanAwardWork *wk = work;
    BOOL done;
    u32 x;
    u32 y;

    switch (*state) {
    case STEP_WAIT_TEXT:
        done = TRUE;
        if (!func_ov221_021b9b64(wk)) {
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
        if (func_ov221_021b9dac(wk)) {
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
    func_ov221_021b9b18(wk);
    if ((s32)*state >= STEP_WAIT_FADE_IN && (s32)*state <= STEP_WAIT_FADE_OUT) {
        func_ov221_021b9d38(wk);
    }
    func_ov221_021b9f7c(wk->graphic);
    return FALSE;
}

// pokebw2's ChihouZukanAward_VBlank
void func_ov221_021b98fc(TCB *tcb, void *data) {
}

// pokebw2's ChihouZukanAward_LoadGraphics
void func_ov221_021b9900(ChihouZukanAwardWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_AWARD, wk->heapId);

    GFL_G2DIOLoadArcNCLRDefault(arc, 1, PALTYPE_MAIN_BG, 0, 0x1e0, wk->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 4, BG_MAIN_DIPLOMA, 0, 0, FALSE, wk->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 6, BG_MAIN_DIPLOMA, 0, 0, FALSE, wk->heapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysLoadScr(BG_MAIN_DIPLOMA);
}

// pokebw2's ChihouZukanAward_Unused
void func_ov221_021b9960(ChihouZukanAwardWork *wk) {
}

void func_ov221_021b9964(ChihouZukanAwardWork *wk) {
    PlayerInfo *playerInfo = GetGameDataPlayerInfo(wk->param->gameData);
    WordSet *wordSet;
    StrBuf *format;
    StrBuf *strbuf;
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
        wk->windows[i] = BmpWin_CreateDynamic(data_ov221_021ba174[i].bg, data_ov221_021ba174[i].x,
                                              data_ov221_021ba174[i].y, data_ov221_021ba174[i].width,
                                              data_ov221_021ba174[i].height, data_ov221_021ba174[i].palette,
                                              data_ov221_021ba174[i].fromEnd);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[i]), 0);
        BmpWin_FlushChar(wk->windows[i]);
    }

    wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
    format = GFL_MsgDataLoadStrbufNew(wk->msgData, ScriptTitle_Text_000);
    strbuf = GFL_StrBufCreate(32, wk->heapId);
    copyVarForText(wordSet, 0, playerInfo);
    GFL_WordSetFormatStrbuf(wordSet, strbuf, format);
    func_0201c724(wk->printQueue, BmpWin_GetBitmap(wk->windows[WINDOW_PLAYER_NAME]), 0, 0, strbuf, wk->font,
                  PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(strbuf);
    GFL_StrBufFree(format);
    GFL_WordSetSystemFree(wordSet);

    strbuf = GFL_MsgDataLoadStrbufNew(wk->msgData, ScriptTitle_Text_001);
    func_0201c724(wk->printQueue, BmpWin_GetBitmap(wk->windows[WINDOW_CERTIFIES]), 0, 0, strbuf, wk->font,
                  PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(strbuf);

    strbuf = GFL_MsgDataLoadStrbufNew(wk->msgData, ScriptTitle_Text_003);
    func_0201c724(wk->printQueue, BmpWin_GetBitmap(wk->windows[WINDOW_STAFF]), 0, 0, strbuf, wk->font,
                  PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(strbuf);

    for (i = 1; i < WINDOW_COUNT; i++) {
        BmpWin_Transfer(wk->windows[i]);
    }
    func_ov221_021b9b18(wk);
}


// pokebw2's ChihouZukanAward_FreeText
void func_ov221_021b9aec(ChihouZukanAwardWork *wk) {
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
// pokebw2's ChihouZukanAward_TransferPrinted
static void func_ov221_021b9b18(ChihouZukanAwardWork *wk) {
    u8 i;

    for (i = 0; i < WINDOW_COUNT; i++) {
        if (!wk->printed[i] && !func_0201c6c4(wk->printQueue, BmpWin_GetBitmap(wk->windows[i]))) {
            BmpWin_Transfer(wk->windows[i]);
            wk->printed[i] = TRUE;
        }
    }
}

// pokebw2's ChihouZukanAward_IsAllPrinted
static BOOL func_ov221_021b9b64(ChihouZukanAwardWork *wk) {
    u8 i;

    for (i = 0; i < WINDOW_COUNT; i++) {
        if (!wk->printed[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

// Fills the BG behind the diploma's text with one color
// pokebw2's ChihouZukanAward_InitBackdrop
void func_ov221_021b9b84(ChihouZukanAwardWork *wk) {
    u16 *palette = GFL_HeapAllocate(wk->heapId, 0x20, TRUE, "chihou_zukan_award.c", 913);
    u16 *screen;
    u16 entry;
    u8 x;
    u8 y;
    u16 i = 0;

    palette[0] = GX_RGB(0, 0, 0);
    palette[1] = GX_RGB(12, 11, 9);
    palette[2] = GX_RGB(20, 19, 18);
    palette[3] = GX_RGB(0, 0, 0);
    GFL_BGSysUploadStdPalette(BG_MAIN_BACKDROP, palette, 0x20, 0x1e0);
    GFL_HeapFree(palette);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[1]), 3);
    BmpWin_FlushChar(wk->windows[1]);
    screen = GFL_HeapAllocate(wk->heapId, 32 * 24 * sizeof(u16), TRUE, "chihou_zukan_award.c", 930);
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

// pokebw2's ChihouZukanAward_InitRibbon
void func_ov221_021b9c4c(ChihouZukanAwardWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_AWARD, wk->heapId);

    wk->palette = func_0204ac18(arc, 0, 0, 0, 0, 2, wk->heapId);
    wk->chars = func_0204a6c8(arc, 3, FALSE, 0, wk->heapId);
    wk->cellAnims = func_0204af28(arc, 8, 9, wk->heapId);
    GFL_ArcToolFree(arc);
    wk->ribbon = func_0204b294(func_ov221_021b9f90(wk->graphic), wk->chars, wk->palette, wk->cellAnims,
                               data_ov221_021ba140, 0, wk->heapId);
    func_0204b3dc(wk->ribbon, TRUE);
    func_0204b92c(wk->ribbon, TRUE);
}

// pokebw2's ChihouZukanAward_FreeRibbon
void func_ov221_021b9ccc(ChihouZukanAwardWork *wk) {
    func_0204b3b4(wk->ribbon);
    func_0204afd8(wk->cellAnims);
    func_0204a8d4(wk->chars);
    func_0204ada4(wk->palette);
}

// Puts the diploma's BGs and the ribbon below the screen, to slide up
// pokebw2's ChihouZukanAward_StartSlide
void func_ov221_021b9cec(ChihouZukanAwardWork *wk) {
    ClActorPos pos;
    u32 i;

    if (wk->param->unk04 == 0) {
        GFL_BGSysMoveBG(BG_MAIN_DIPLOMA, BG_MOVE_SET_Y, 64);
        GFL_BGSysMoveBG(BG_MAIN_BACKDROP, BG_MOVE_SET_Y, 64);
        for (i = 0; i < NELEMS(data_ov221_021ba140); i++) {
            pos.x = data_ov221_021ba140[i].x;
            pos.y = data_ov221_021ba140[i].y - 64;
            func_0204b404(wk->ribbon, &pos, 0);
        }
        wk->slideWait = 0;
    }
}

// pokebw2's ChihouZukanAward_UpdateSlide
static void func_ov221_021b9d38(ChihouZukanAwardWork *wk) {
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

// pokebw2's ChihouZukanAward_IsSlideDone
static BOOL func_ov221_021b9dac(ChihouZukanAwardWork *wk) {
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
