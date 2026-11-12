// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov186/trainer_card.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/trainer_card.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "constants/version.h"
#include "field/trcard_sys.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
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
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "nnsys/gfd.h"
#include "save/player_info.h"
#include "save/timesig_savedata.h"
#include "system/app_menu_common.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/pms_draw.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wipe.h"
#include "system/wordset.h"

// The trainer card screen of overlay 186: the card's front and back on the touch screen, which the player flips,
// scrolls and signs. The ROM has no name for this file; trainer_card.c is a guess

// The proc's states
enum {
    TRCARD_SEQ_FADE_IN,
    TRCARD_SEQ_MAIN,
    TRCARD_SEQ_RETURN,
    TRCARD_SEQ_EXIT,
    TRCARD_SEQ_FADE_OUT,
    TRCARD_SEQ_FLIP,
    TRCARD_SEQ_GREETING,
    TRCARD_SEQ_BADGES,
    TRCARD_SEQ_8,
    TRCARD_SEQ_ZOOM,
    TRCARD_SEQ_MEDALS,
};

// What the input asks for
enum {
    TRCARD_INPUT_NONE,
    TRCARD_INPUT_FLIP = 3,
    TRCARD_INPUT_RETURN,
    TRCARD_INPUT_EXIT,
    TRCARD_INPUT_UNION_VIEW,
    TRCARD_INPUT_PERSONALITY,
    TRCARD_INPUT_ZOOM,
    TRCARD_INPUT_GREETING,
    TRCARD_INPUT_BADGES,
    TRCARD_INPUT_MEDALS,
};

// The card's scale while it flips and before the signature is zoomed
#define TRCARD_SCALE 0x103d

// A union room appearance's picture and palette in ARCID_TRAINER_CARD_UNION_GRA
typedef struct {
    u32 chars;
    u32 palette;
} TrainerCardUnionGra;

void func_ov111_021c3ef8(TrainerCardData *data);
static void func_ov111_021c3f18(TrainerCardWork *wk, int count);
void func_ov111_021c41c4(TrainerCardWork *wk);
static void func_ov111_021c42c0(TrainerCardWork *wk);
void func_ov111_021c436c(TrainerCardWork *wk, int stars, u32 hasPokedex);
void func_ov111_021c43c4(TrainerCardWork *wk, u32 view);
void func_ov111_021c43e8(void);
u32 func_ov111_021c4524(u32 version);
u32 func_ov111_021c4530(u32 version);
void func_ov111_021c453c(TrainerCardWork *wk);
static void func_ov111_021c46f0(void);
static void func_ov111_021c4754(TrainerCardWork *wk, u32 version);
void func_ov111_021c479c(TrainerCardWork *wk);
void func_ov111_021c47ec(TrainerCardWork *wk);
static void func_ov111_021c4858(TrainerCardWork *wk, int side);
static void func_ov111_021c49d8(TrainerCardWork *wk, int side);
static void func_ov111_021c4ab0(TrainerCardWork *wk, int side);
static void func_ov111_021c4b74(TrainerCardWork *wk, int side);
BOOL func_ov111_021c4b9c(TrainerCardWork *wk);
static void func_ov111_021c4d48(int side);
static BOOL func_ov111_021c4e0c(TrainerCardWork *wk);
void func_ov111_021c4f80(TrainerCardWork *wk);
static BOOL func_ov111_021c5008(TrainerCardWork *wk);
static int func_ov111_021c5054(TrainerCardWork *wk);
static void func_ov111_021c513c(TrainerCardWork *wk);
static void func_ov111_021c51b4(TrainerCardWork *wk, int eraser, int visible);
static void func_ov111_021c51e4(TrainerCardWork *wk, int eraser);
static void func_ov111_021c523c(TrainerCardWork *wk, int eraser);
static BOOL func_ov111_021c5264(TrainerCardWork *wk, int button);
int func_ov111_021c56e8(TrainerCardWork *wk);
void func_ov111_021c5718(TrainerCardWork *wk, int input, u32 *state);
void func_ov111_021c581c(TCB *tcb, void *data);
static void func_ov111_021c5874(void);
void func_ov111_021c58e0(TrainerCardWork *wk);
void func_ov111_021c5938(TrainerCardWork *wk);
void func_ov111_021c5958(const u8 *src, u8 *dest);
static void func_ov111_021c59b0(const u8 *src, u8 *dest);
static void func_ov111_021c59dc(u32 bg, int mode);
void func_ov111_021c5acc(u32 bg, const u8 *pixels, u8 mode);
static void func_ov111_021c5aec(TrainerCardWork *wk);
void func_ov111_021c5b34(TrainerCardWork *wk);
static void func_ov111_021c5b80(TrainerCardWork *wk);
static void func_ov111_021c5bcc(TrainerCardWork *wk, int zoomed);
static void func_ov111_021c5cf4(u8 *pixels, const u8 *brush, int x, int y, int width, int height, int zoomed,
                                  int zoomSide);
static int func_ov111_021c5db8(u8 *pixels, const u8 *brush, int x, int y, int *lastX, int *lastY, int first, int on,
                                int zoomed, int zoomSide);
static void func_ov111_021c5f50(TrainerCardPen *src, TrainerCardPen *dest);
static int func_ov111_021c5f64(u8 *pixels, TrainerCardPen *pen, TrainerCardPen *old, BOOL load, const u8 *loadSrc,
                               u8 zoomed, u8 zoomSide);

static const TrainerCardUnionGra data_ov111_021c86e8[16] = {
#include "asset/trainer_card_data_ov111_021c86e8.inc"
};

static const BGSysVRAMConfig data_ov111_021c8564 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// The card's palette for each number of stars, by the version's color, and the one of a card from another game
// pokebw2's sTrainerCardPalettes
static u32 data_ov111_021c8e20[7][2] = {
#include "asset/trainer_card_data_ov111_021c8e20.inc"
};

// Steps the union room appearance to the next of the trainer's gender
// pokebw2's TrainerCard_NextUnionView

BOOL func_ov111_021c3cc0(GameProc *proc, u32 *state, void *param, void *work) {
    TrainerCardWork *wk;
    TrainerCardParam *cardParam = param;
    int i;
    u16 bit;

    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    func_0202223c(0, 0);
    func_0202223c(1, 0);
    func_02022224(0);
    func_02022224(1);
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_TRAINER_CARD_SCREEN, 0x50000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(TrainerCardWork), HEAPID_TRAINER_CARD_SCREEN);
    MI_CpuFill8(wk, 0, sizeof(TrainerCardWork));
    wk->heapId = HEAPID_TRAINER_CARD_SCREEN;
    wk->obj.heapId = HEAPID_TRAINER_CARD_SCREEN;
    wk->param = cardParam;
    wk->data = cardParam->data;
    wk->touchMode = func_020355c4();
    wk->msgSpeed = func_02012ffc();
    wk->frameType = 0;
    if (wk->data->clearMonth != 0) {
        wk->isCleared = TRUE;
    } else {
        wk->isCleared = FALSE;
    }
    bit = 1;
    for (i = 0; i < 16; i++) {
        if (wk->data->badges & bit) {
            wk->badges[i] = 1;
        } else {
            wk->badges[i] = 0;
        }
        bit <<= 1;
    }
    if (func_0203d1b4() > 0) {
        wk->isOnline = TRUE;
    }
    GFL_BGSysCreate(wk->heapId);
    func_ov111_021c41c4(wk);
    GFL_BGSysSetVRAMBanks(&data_ov111_021c8564);
    func_ov111_021c43e8();
    func_ov111_021c453c(wk);
    func_ov111_021c6d9c(&wk->obj, &data_ov111_021c8564);
    func_ov111_021c6804(wk, wk->param->data);
    func_ov111_021c6e8c(&wk->obj);
    func_ov111_021c6000(wk);
    wk->pmsDraw =
        func_0202db0c(wk->obj.unit, CLACT_VRAM_SUB, wk->printQueue, wk->font, 8, 1, HEAPID_TRAINER_CARD_SCREEN);
    func_ov111_021c6d98(wk, wk->win[TRCARD_WIN_MONEY], 1, wk->colonBuf);
    if (wk->param->appParam == 2) {
        wk->param->appParam = 0;
        wk->side = 1;
    } else {
        wk->side = 0;
    }
    wk->param->next = 0;
    wk->unk32E8 = 0;
    wk->unk31E0 = 0;
    if (wk->data->unk04_4) {
        wk->blinkMode = 0;
    } else {
        wk->blinkMode = 2;
    }
    wk->centerX = 128;
    wk->centerY = 96;
    wk->offsetX = 0;
    wk->offsetY = 0;
    func_ov111_021c5874();
    func_ov111_021c47ec(wk);
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov111_021c581c, wk, 1);
    wk->tcbl = GFL_TCBExMgrCreate(wk->heapId, wk->heapId, 3, 0);
    func_0203d34c(TRUE, wk->heapId);
    GFL_WipeSet(1, 3, 3, 0, 6, 1, wk->heapId);
    return TRUE;
}

void func_ov111_021c3ef8(TrainerCardData *data) {
    data->unk06++;
    if (data->unk06 > (data->gender + 1) * 8 - 1) {
        data->unk06 = data->gender * 8;
    }
}

// pokebw2's TrainerCard_PlayPenSE
static void func_ov111_021c3f18(TrainerCardWork *wk, int count) {
    if (wk->seWait != 0) {
        wk->seWait--;
        return;
    }
    if (count > 5) {
        wk->seWait = 5;
        if (wk->brush == 0) {
            GFL_SndSEPlay(0x65c);
        } else {
            GFL_SndSEPlay(0x7cf);
        }
    }
}

// pokebw2's TrainerCard_Main
BOOL func_ov111_021c3f54(GameProc *proc, u32 *state, void *param, void *work) {
    TrainerCardWork *wk = work;

    switch (*state) {
    case TRCARD_SEQ_FADE_IN:
        if (GFL_WipeIsFinished()) {
            *state = TRCARD_SEQ_MAIN;
        }
        break;
    case TRCARD_SEQ_MAIN:
        func_ov111_021c5718(wk, func_ov111_021c56e8(wk), state);
        func_ov111_021c5aec(wk);
        if (wk->param->data->canEdit) {
            func_ov111_021c3f18(wk, func_ov111_021c5f64(wk->signPixels, &wk->penNew, &wk->penOld, TRUE,
                                                          wk->signPixels, wk->zoomed, wk->zoomSide));
        }
        break;
    case TRCARD_SEQ_RETURN:
        if (!func_0204b98c(wk->obj.actors[TRCARD_ACTOR_RETURN])) {
            GFL_WipeSet(2, 4, 4, 0, 6, 1, wk->heapId);
            *state = TRCARD_SEQ_FADE_OUT;
        }
        break;
    case TRCARD_SEQ_EXIT:
        if (!func_0204b98c(wk->obj.actors[TRCARD_ACTOR_EXIT])) {
            GFL_WipeSet(2, 4, 4, 0, 6, 1, wk->heapId);
            *state = TRCARD_SEQ_FADE_OUT;
        }
        break;
    case TRCARD_SEQ_FADE_OUT:
        if (GFL_WipeIsFinished()) {
            return TRUE;
        }
        break;
    case TRCARD_SEQ_FLIP:
        if (func_ov111_021c4b9c(wk)) {
            *state = TRCARD_SEQ_MAIN;
        }
        break;
    case TRCARD_SEQ_ZOOM:
        if (func_ov111_021c4e0c(wk)) {
            *state = TRCARD_SEQ_MAIN;
        }
        break;
    case TRCARD_SEQ_GREETING:
        GFL_WipeSet(2, 4, 4, 0, 6, 1, wk->heapId);
        wk->param->next = 1;
        *state = TRCARD_SEQ_FADE_OUT;
        break;
    case TRCARD_SEQ_BADGES:
        if (!func_0204b98c(wk->obj.actors[TRCARD_ACTOR_BADGES])) {
            GFL_WipeSet(2, 4, 4, 0, 6, 1, wk->heapId);
            wk->param->next = 2;
            *state = TRCARD_SEQ_FADE_OUT;
        }
        break;
    case TRCARD_SEQ_8:
        break;
    }
    wk->frame++;
    if (wk->frame >= 0x100) {
        wk->frame = 0;
    }
    func_0204a600();
    func_0202dbc4(wk->pmsDraw);
    func_0201c4e4(wk->printQueue);
    if (wk->tcbl != NULL) {
        GFL_TCBExMgrUpdate(wk->tcbl);
    }
    return FALSE;
}

// pokebw2's TrainerCard_Exit
void func_ov111_021c59b0(const u8 *src, u8 *dest);
void *getTimeSigSaveBlock(GameData *gameData);
void *func_020089cc(void *timeSig);
void MI_CpuCopy8(const void *src, void *dest, u32 size);
PlayerInfo *GetGameDataPlayerInfo(GameData *gameData);
void func_02008558(PlayerInfo *info, u8 value);
void func_02008a14(TimeSigSave *timeSig, u32 value);
void func_02008a1c(TimeSigSave *timeSig, u8 value);
void func_0202dc08(PMSDraw *draw);
void func_ov111_021c5874(void);
void func_ov111_021c6f4c(TrainerCardObjWork *obj);
void GFL_HeapFree(void *ptr);
void func_ov111_021c60a0(TrainerCardWork *wk);
void func_ov111_021c46f0(void);
BOOL GFL_TCBRemove(TCB *tcb);
void GFL_TCBExMgrFree(TCBExManager *manager);
void func_020355d8(BOOL touch);
void GFL_ProcReleaseSubsystem(GameProc *proc);
void GFL_HeapDelete(HeapID heapId);

BOOL func_ov111_021c40dc(GameProc *proc, u32 *state, void *param, void *work) {
    TrainerCardWork *wk = work;
    TrainerCardParam *cardParam = param;
    PlayerInfo *info;
    TimeSigSave *timeSig;

    if (!wk->data->unk04_5) {
        func_ov111_021c59b0(wk->signPixels, wk->data->unk74);
        MI_CpuCopy8(wk->data->unk74, func_020089cc(getTimeSigSaveBlock(wk->param->gameData)), TRCARD_SIGN_BYTES);
    }
    if (cardParam->canEdit) {
        info = GetGameDataPlayerInfo(wk->param->gameData);
        timeSig = getTimeSigSaveBlock(wk->param->gameData);
        func_02008558(info, wk->data->unk06);
        func_02008a14(timeSig, wk->data->unk48);
        func_02008a1c(timeSig, wk->data->unk04_4);
    }
    func_0202dc08(wk->pmsDraw);
    func_ov111_021c5874();
    func_ov111_021c42c0(wk);
    func_ov111_021c6f4c(&wk->obj);
    GFL_HeapFree(wk->charRes);
    GFL_HeapFree(wk->scrRes);
    GFL_HeapFree(wk->palRes);
    func_ov111_021c60a0(wk);
    func_ov111_021c46f0();
    GFL_TCBRemove(wk->vblankTask);
    GFL_TCBExMgrFree(wk->tcbl);
    func_020355d8(wk->touchMode);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_TRAINER_CARD_SCREEN);
    return TRUE;
}

// pokebw2's TrainerCard_LoadMessages
void func_ov111_021c41c4(TrainerCardWork *wk) {
    int i;

    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, wk->heapId);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_SERVER_FLOW_4, wk->heapId);
    wk->numBuf = GFL_StrBufCreate(4, wk->heapId);
    wk->nameBuf = GFL_StrBufCreate(32, wk->heapId);
    wk->tmpBuf = GFL_StrBufCreate(33, wk->heapId);
    wk->colonBuf = GFL_StrBufCreate(5, wk->heapId);
    GFL_MsgDataLoadStrbuf(wk->msgData, 23, wk->colonBuf);
    for (i = 0; i < 3; i++) {
        wk->unk3234[i] = GFL_MsgDataLoadStrbufNew(wk->msgData, i + 26);
    }
    for (i = 0; i < 2; i++) {
        wk->unk3240[i] = GFL_MsgDataLoadStrbufNew(wk->msgData, i + 29);
    }
    for (i = 0; i < 26; i++) {
        wk->labels[i] = GFL_MsgDataLoadStrbufNew(wk->msgData, i);
    }
    for (i = 0; i < 5; i++) {
        wk->formats[i] = GFL_MsgDataLoadStrbufNew(wk->msgData, i + 56);
    }
    wk->wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
}

// pokebw2's TrainerCard_FreeMessages
static void func_ov111_021c42c0(TrainerCardWork *wk) {
    int i;

    GFL_WordSetSystemFree(wk->wordSet);
    for (i = 0; i < 5; i++) {
        GFL_StrBufFree(wk->formats[i]);
    }
    for (i = 0; i < 26; i++) {
        GFL_StrBufFree(wk->labels[i]);
    }
    GFL_StrBufFree(wk->unk3240[1]);
    GFL_StrBufFree(wk->unk3240[0]);
    GFL_StrBufFree(wk->unk3234[2]);
    GFL_StrBufFree(wk->unk3234[1]);
    GFL_StrBufFree(wk->unk3234[0]);
    GFL_StrBufFree(wk->colonBuf);
    GFL_StrBufFree(wk->tmpBuf);
    GFL_StrBufFree(wk->nameBuf);
    GFL_StrBufFree(wk->numBuf);
    GFL_MsgDataFree(wk->msgData);
    GFL_FontFree(wk->font);
}

// pokebw2's TrainerCard_LoadCardPalette
void func_ov111_021c436c(TrainerCardWork *wk, int stars, u32 hasPokedex) {
    int color = 1;
    u32 file;

    if (wk->data->version == VERSION_WHITE) {
        color = 0;
    } else if (wk->data->version == VERSION_BLACK) {
        color = 1;
    } else {
        stars = 6;
    }
    file = data_ov111_021c8e20[stars][color];
    GFL_BGSysLoadNCLRDefault(ARCID_TRAINER_CARD_GRA, file, 4, 0, 0x200, wk->heapId);
    wk->palRes = GFL_G2DIOReadNCLR(ARCID_TRAINER_CARD_GRA, file, &wk->palData, wk->heapId);
}

// pokebw2's TrainerCard_LoadUnionPalette
void func_ov111_021c43c4(TrainerCardWork *wk, u32 view) {
    GFL_BGSysLoadNCLRDefault(ARCID_TRAINER_CARD_UNION_GRA, data_ov111_021c86e8[view].palette, 4, 0x80, 0x20,
                             wk->heapId);
}

// pokebw2's TrainerCard_SetupBG
void func_ov111_021c43e8(void) {
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    {
        BGSysLCDConfig lcdConfig = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_5, FALSE };

        GFL_BGSysSetLCDConfig(&lcdConfig);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, 1, 1, 15, 4, 0x10000, 0, 1, 0, 0 };

        GFL_BGSysCreateBG(7, &setup, 2);
        GFL_BGSysClearBG(7);
        GFL_BGSysSetBGEnabled(7, TRUE);
    }
    {
        BGSetup setups[2] = {
            { 0, 0, 0x800, 0, 1, 0, 12, 0, 0x6000, 0, 0, 0, 0 },
            { 0, 0, 0x800, 0, 1, 0, 13, 1, 0x6000, 0, 3, 0, 0 },
        };

        GFL_BGSysCreateBG(4, &setups[0], 0);
        GFL_BGSysClearBG(4);
        GFL_BGSysSetBGEnabled(4, TRUE);
        GFL_BGSysCreateBG(5, &setups[1], 0);
        GFL_BGSysClearBG(5);
        GFL_BGSysSetBGEnabled(5, TRUE);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, 1, 1, 14, 2, 0x8000, 0, 2, 0, 0 };

        GFL_BGSysCreateBG(6, &setup, 2);
        GFL_BGSysClearBG(6);
        GFL_BGSysSetBGEnabled(6, TRUE);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, 1, 0, 28, 0, 0x8000, 0, 2, 0, 0 };

        GFL_BGSysCreateBG(2, &setup, 0);
        GFL_BGSysClearBG(2);
        GFL_BGSysSetBGEnabled(2, FALSE);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, 1, 0, 29, 4, 0x8000, 0, 3, 0, 0 };

        GFL_BGSysCreateBG(3, &setup, 0);
        GFL_BGSysClearBG(3);
        GFL_BGSysSetBGEnabled(3, TRUE);
    }
}

// pokebw2's TrainerCard_GetFrontCharFile
u32 func_ov111_021c4524(u32 version) {
    if (version == VERSION_WHITE) {
        return 32;
    }
    return 28;
}

// pokebw2's TrainerCard_GetFrontScreenFile
u32 func_ov111_021c4530(u32 version) {
    if (version == VERSION_WHITE) {
        return 42;
    }
    return 41;
}

// pokebw2's TrainerCard_LoadGraphics
void func_ov111_021c453c(TrainerCardWork *wk) {
    func_ov111_021c436c(wk, wk->data->stars, wk->data->unk04_2);
    if (!wk->data->unk04_7) {
        wk->charRes = GFL_G2DIOReadBGNCGR(ARCID_TRAINER_CARD_GRA, 31, FALSE, &wk->charData, wk->heapId);
        if (!wk->data->gender) {
            wk->scrRes = GFL_G2DIOReadNSCR(ARCID_TRAINER_CARD_GRA, 43, FALSE, &wk->scrData, wk->heapId);
        } else {
            wk->scrRes = GFL_G2DIOReadNSCR(ARCID_TRAINER_CARD_GRA, 44, FALSE, &wk->scrData, wk->heapId);
        }
    } else {
        wk->charRes = GFL_G2DIOReadBGNCGR(ARCID_TRAINER_CARD_UNION_GRA, data_ov111_021c86e8[wk->data->unk06].chars,
                                          FALSE, &wk->charData, wk->heapId);
        wk->scrRes = GFL_G2DIOReadNSCR(ARCID_TRAINER_CARD_UNION_GRA, 32, FALSE, &wk->scrData, wk->heapId);
        func_ov111_021c43c4(wk, wk->data->unk06);
    }
    func_ov111_021c58e0(wk);
    GFL_BGSysLoadNCGRStatic(ARCID_TRAINER_CARD_GRA, func_ov111_021c4524(wk->data->version), 6, 0, 0, FALSE,
                            wk->heapId);
    loadBGScrToVramByNarcNoReserveNegAlign(ARCID_TRAINER_CARD_GRA, func_ov111_021c4530(wk->data->version), 6,
                                           0, 0, FALSE, wk->heapId);
    GFL_BGSysLoadNCGRStatic(ARCID_TRAINER_CARD_GRA, 30, 5, 0, 0, FALSE, wk->heapId);
    loadBGScrToVramByNarcNoReserveNegAlign(ARCID_TRAINER_CARD_GRA, 40, 5, 0, 0, FALSE, wk->heapId);
    GFL_BGSysLoadNCGRStatic(ARCID_TRAINER_CARD_GRA, 30, 3, 0, 0, FALSE, wk->heapId);
    loadBGScrToVramByNarcNoReserveNegAlign(ARCID_TRAINER_CARD_GRA, 40, 3, 0, 0, FALSE, wk->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_TRAINER_CARD_GRA, 15, 0, 0, 0x20, wk->heapId);
    func_ov111_021c5958(wk->data->unk74, wk->signPixels);
    GFL_BGSysLoadNCGRStatic(getUINarcIdx(), func_02027628(), 4, 0, 0, FALSE, wk->heapId);
    loadBGScrToVramByNarcNoReserveNegAlign(getUINarcIdx(), func_0202762c(), 4, 0, 0, FALSE, wk->heapId);
    GFL_BGSysSetScrPaletteNo(4, 0, 21, 32, 3, 12);
    GFL_BGSysLoadScr(4);
}

// pokebw2's TrainerCard_ReleaseBG
static void func_ov111_021c46f0(void) {
    GFL_BGSysSetBGEnabledA(0x1f, FALSE);
    GFL_BGSysSetBGEnabledB(0x1f, FALSE);
    GFL_BGSysMoveBG(5, BG_MOVE_SET_X, 0);
    GFL_BGSysMoveBG(5, BG_MOVE_SET_Y, 0);
    GFL_BGSysMoveBG(3, BG_MOVE_SET_X, 0);
    GFL_BGSysMoveBG(3, BG_MOVE_SET_Y, 0);
    GFL_BGSysReleaseBG(7);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(3);
    GFL_BGSysFree();
}

// The back's screen, with the pen or the eraser chosen, in the version's color
// pokebw2's TrainerCard_LoadBackScreen
static void func_ov111_021c4754(TrainerCardWork *wk, u32 version) {
    int files[2][2] = { { 39, 36 }, { 38, 37 } };

    // The version becomes the screen's color
    if (version == VERSION_WHITE) {
        version = 0;
    } else {
        version = 1;
    }
    loadBGScrToVramByNarcNoReserveNegAlign(ARCID_TRAINER_CARD_GRA, files[wk->data->unk04_4][version], 6, 0, 0, FALSE,
                                           wk->heapId);
}

// The tables below are declared here, and in this order, for the order of the file's .rodata: MWCC's sort of them
// by size depends on where each one is declared

static const TouchRect data_ov111_021c8544[] = {
    { 16, 80, 16, 240 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

// The buttons of the player's card, and of another player's
static const TouchRect data_ov111_021c8594[] = {
#include "asset/trainer_card_data_ov111_021c8594.inc"
};

static const TouchRect data_ov111_021c8554[] = {
#include "asset/trainer_card_data_ov111_021c8554.inc"
};

static const TouchRect data_ov111_021c854c[] = {
    { 8, 168, 16, 240 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

static const TouchRect data_ov111_021c852c[] = {
    { 88, 152, 32, 225 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

static const TouchRect data_ov111_021c853c[] = {
    { 24, 152, 32, 224 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

static const TouchRect data_ov111_021c8534[] = {
    { 72, 168, 16, 240 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

// The pen and the eraser, normally and on the zoomed signature: 1 draws a pixel and 2 erases it
static const u8 data_ov111_021c86a8[2][2][16] = {
#include "asset/trainer_card_data_ov111_021c86a8.inc"
};

// pokebw2's TrainerCard_LoadSideScreen
void func_ov111_021c479c(TrainerCardWork *wk) {
    if (wk->side == 0) {
        loadBGScrToVramByNarcNoReserveNegAlign(
            ARCID_TRAINER_CARD_GRA, func_ov111_021c4530(wk->data->version), 6, 0, 0, FALSE, wk->heapId);
        GFL_BGSysClearScr(7);
    } else {
        func_ov111_021c4754(wk, wk->data->version);
        func_ov111_021c5938(wk);
        GFL_BGSysClearScr(7);
    }
}

void func_ov111_021c47ec(TrainerCardWork *wk) {
    func_ov111_021c479c(wk);
    func_ov111_021c60cc(wk, wk->win, wk->data);
    func_0202dbc4(wk->pmsDraw);
    if (wk->side == 0) {
        func_ov111_021c6874(wk, wk->scroll, FALSE, 0);
        func_ov111_021c58e0(wk);
    } else {
        func_ov111_021c636c(wk, wk->win, wk->data);
        func_ov111_021c5acc(7, wk->signPixels, wk->blinkMode);
    }
    func_ov111_021c4b74(wk, wk->side);
    func_ov111_021c631c(wk, wk->win);
}


// The buttons of a card that its owner can edit
// pokebw2's TrainerCard_SetButtonsEdit
static void func_ov111_021c4858(TrainerCardWork *wk, int side) {
    if (side == 0) {
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_RETURN, 1, TRUE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_ZOOM, 18, FALSE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_BRUSH, 6, FALSE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_BADGES, 16, TRUE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_SHORTCUT,
                           GameData_IsShortcutRegistered(wk->param->gameData, SHORTCUT_TRAINER_CARD_FRONT) + 6, TRUE);
        return;
    }
    if (wk->zoomed == 0) {
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_RETURN, 1, TRUE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_EXIT, 0, TRUE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_FLIP, 22, TRUE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_ZOOM, 18, wk->data->unk04_4 ^ 1);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_BRUSH, 8 - wk->brush * 2, wk->data->unk04_4 ^ 1);
        func_ov111_021c51b4(wk, wk->data->unk04_4, TRUE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_SHORTCUT,
                           GameData_IsShortcutRegistered(wk->param->gameData, SHORTCUT_TRAINER_CARD_BACK) + 6, TRUE);
    } else {
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_RETURN, 15, TRUE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_EXIT, 14, TRUE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_ZOOM, 20, TRUE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_BRUSH, 8 - wk->brush * 2, TRUE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_FLIP, 22, FALSE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_BADGES, 10, FALSE);
    }
}

// The buttons of the player's card where it can't be edited
// pokebw2's TrainerCard_SetButtonsLocked
static void func_ov111_021c49d8(TrainerCardWork *wk, int side) {
    if (side == 0) {
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_RETURN, 1, TRUE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_ZOOM, 18, FALSE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_BRUSH, 6, FALSE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_BADGES, 16, TRUE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_SHORTCUT,
                           GameData_IsShortcutRegistered(wk->param->gameData, SHORTCUT_TRAINER_CARD_FRONT) + 6, TRUE);
    } else {
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_RETURN, 1, TRUE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_EXIT, 0, TRUE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_ZOOM, 18, FALSE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_FLIP, 22, TRUE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_BRUSH, 8 - wk->brush * 2, FALSE);
        func_ov111_021c51b4(wk, wk->data->unk04_4, FALSE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_SHORTCUT,
                           GameData_IsShortcutRegistered(wk->param->gameData, SHORTCUT_TRAINER_CARD_BACK) + 6, TRUE);
    }
}

// The buttons of another player's card
// pokebw2's TrainerCard_SetButtonsOther
static void func_ov111_021c4ab0(TrainerCardWork *wk, int side) {
    if (side == 0) {
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_RETURN, 1, TRUE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_EXIT, 14, FALSE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_BADGES, 16, FALSE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_BRUSH, 6, FALSE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_SHORTCUT, 6, FALSE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_ZOOM, 0, FALSE);
    } else {
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_RETURN, 1, TRUE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_EXIT, 0, FALSE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_ZOOM, 18, FALSE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_FLIP, 22, TRUE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_BRUSH, 8 - wk->brush * 2, FALSE);
        func_ov111_021c51b4(wk, wk->data->unk04_4, FALSE);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_SHORTCUT, 6, FALSE);
    }
}

// pokebw2's TrainerCard_SetButtons
static void func_ov111_021c4b74(TrainerCardWork *wk, int side) {
    if (wk->data->unk04_7 == TRUE) {
        func_ov111_021c4ab0(wk, side);
    } else if (wk->param->canEdit == FALSE) {
        func_ov111_021c49d8(wk, side);
    } else {
        func_ov111_021c4858(wk, side);
    }
}

BOOL func_ov111_021c4b9c(TrainerCardWork *wk) {
    BOOL done = FALSE;

    switch (wk->subSeq) {
    case 0:
        wk->flipAngle = 0;
        wk->scaleX = TRCARD_SCALE;
        wk->scaleY = TRCARD_SCALE;
        GFL_SndSEPlay(0x65a);
        if (wk->side == 0) {
            func_0202dcd4(wk->pmsDraw, 0, FALSE);
        }
        wk->subSeq++;
        break;
    case 1:
        wk->scaleX = FX_Mul(FX_CosIdx(wk->flipAngle), TRCARD_SCALE);
        if (wk->scaleX <= 0x80) {
            wk->scaleX = 0x80;
            wk->subSeq++;
        }
        wk->flipAngle += 0x400;
        if (wk->flipAngle >= 0x4000) {
            wk->flipAngle = 0x4000;
        }
        break;
    case 2:
        wk->side ^= 1;
        func_ov111_021c479c(wk);
        func_ov111_021c4b74(wk, wk->side);
        wk->subSeq++;
        break;
    case 3:
        if (wk->side == 0) {
            func_ov111_021c58e0(wk);
        } else {
            func_ov111_021c5acc(7, wk->signPixels, wk->blinkMode);
        }
        wk->subSeq++;
        break;
    case 4:
        func_ov111_021c631c(wk, wk->win);
        wk->subSeq++;
        break;
    case 5:
        wk->flipAngle -= 0x400;
        if (wk->flipAngle <= 0) {
            wk->flipAngle = 0;
        }
        wk->scaleX = FX_Mul(FX_CosIdx(wk->flipAngle), TRCARD_SCALE);
        if (wk->scaleX <= 0x80) {
            wk->scaleX = 0x80;
        }
        if (wk->flipAngle == 0) {
            wk->scaleX = FX32_ONE;
            wk->scaleY = FX32_ONE;
            done = TRUE;
            if (wk->side == 0) {
                func_0202dcb0(wk->pmsDraw, 0, TRUE);
                func_0202dcd4(wk->pmsDraw, 0, TRUE);
            }
        }
        break;
    }
    wk->affineDirty = TRUE;
    return done;
}


// Darkens the strip at the edge of the zoomed signature that the other half is reached through: at the right for the
// left half (0), at the left for the right half (1), or nowhere (-1)
// pokebw2's TrainerCard_SetSignWindow
static void func_ov111_021c4d48(int side) {
    if (side < 0) {
        G2S_BlendNone();
        GXS_SetVisibleWnd(0);
        return;
    }
    if (side == 0) {
        G2S_SetBlendBrightness(0xc, -8);
        G2S_SetWnd0Position(224, 0, 0, 168);
        G2S_SetWnd0InsidePlane(0x1c, TRUE);
        G2S_SetWndOutsidePlane(0x1f, FALSE);
        GXS_SetVisibleWnd(1);
    } else if (side == 1) {
        G2S_SetBlendBrightness(0xc, -8);
        G2S_SetWnd0Position(0, 0, 32, 168);
        G2S_SetWnd0InsidePlane(0x1c, TRUE);
        G2S_SetWndOutsidePlane(0x1f, FALSE);
        GXS_SetVisibleWnd(1);
    }
}

// Zooms in to a half of the signature, out again, and slides between the halves. TRUE when a step that ends one is
// done
// pokebw2's TrainerCard_ZoomSeq
static BOOL func_ov111_021c4e0c(TrainerCardWork *wk) {
    switch (wk->subSeq) {
    case 0:
        if (wk->zoomSide == 0) {
            wk->centerX = 32;
            wk->centerY = 152;
        } else {
            wk->centerX = 224;
            wk->centerY = 152;
        }
        wk->scaleX = TRCARD_SCALE;
        wk->scaleY = TRCARD_SCALE;
        wk->subSeq = 1;
        break;
    case 1:
        wk->scaleX += 0x100;
        wk->scaleY += 0x100;
        if (wk->scaleX >= 2 * FX32_ONE) {
            wk->scaleX = 2 * FX32_ONE;
            wk->scaleY = 2 * FX32_ONE;
            wk->subSeq = 2;
        }
        break;
    case 2:
        wk->zoomed = 1;
        func_ov111_021c4b74(wk, wk->side);
        func_ov111_021c4d48(wk->zoomSide);
        return TRUE;
    case 3:
        wk->subSeq = 4;
        break;
    case 4:
        wk->scaleX -= 0x100;
        wk->scaleY -= 0x100;
        if (wk->scaleX <= FX32_ONE) {
            wk->scaleX = FX32_ONE;
            wk->scaleY = FX32_ONE;
            wk->subSeq = 5;
        }
        break;
    case 5:
        wk->centerX = 128;
        wk->centerY = 96;
        wk->zoomed = 0;
        func_ov111_021c4b74(wk, wk->side);
        func_ov111_021c4d48(-1);
        return TRUE;
    case 6:
        wk->subSeq = 7;
        break;
    case 7:
        wk->centerX -= 8;
        if (wk->centerX < 32) {
            wk->centerX = 32;
            wk->subSeq = 8;
        }
        break;
    case 8:
        wk->zoomSide ^= 1;
        func_ov111_021c4d48(wk->zoomSide);
        return TRUE;
    case 9:
        wk->subSeq = 10;
        break;
    case 10:
        wk->centerX += 8;
        if (wk->centerX > 224) {
            wk->centerX = 224;
            wk->subSeq = 11;
        }
        break;
    case 11:
        wk->zoomSide ^= 1;
        func_ov111_021c4d48(wk->zoomSide);
        return TRUE;
    }
    wk->affineDirty = TRUE;
    return FALSE;
}

// pokebw2's TrainerCard_UpdateAffine
void func_ov111_021c4f80(TrainerCardWork *wk) {
    MtxFx22 mtx;

    mtx._00 = FX_Inv(wk->scaleX);
    mtx._01 = 0;
    mtx._10 = 0;
    mtx._11 = FX_Inv(wk->scaleY);
    G2S_SetBG2Affine(&mtx, wk->centerX, wk->centerY, wk->offsetX, wk->offsetY);
    G2S_SetBG3Affine(&mtx, wk->centerX, wk->centerY, wk->offsetX, wk->offsetY);
    wk->affineDirty = FALSE;
}

// Switches between the keys and the touch screen when the other is used
// pokebw2's TrainerCard_CheckInputMode
static BOOL func_ov111_021c5008(TrainerCardWork *wk) {
    if (wk->touchMode == TRUE) {
        if (func_02035c38()) {
            return FALSE;
        }
        if (GCTX_HIDGetHeldKeys()) {
            wk->touchMode = FALSE;
            return TRUE;
        }
    } else {
        if (GCTX_HIDGetHeldKeys()) {
            return FALSE;
        }
        if (func_02035c38()) {
            wk->touchMode = TRUE;
            return FALSE;
        }
    }
    return FALSE;
}

// pokebw2's TrainerCard_KeyInput
static int func_ov111_021c5054(TrainerCardWork *wk) {
    u32 keys = GCTX_HIDGetPressedKeys();

    if (keys & PAD_BUTTON_B) {
        if (wk->zoomed == 0) {
            func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_RETURN, 9, TRUE);
            GFL_SndSEPlay(0x551);
            func_020355d8(FALSE);
            return TRCARD_INPUT_RETURN;
        }
    } else if (keys & PAD_BUTTON_X) {
        if (wk->zoomed == 0 && !wk->data->unk04_7) {
            func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_EXIT, 8, TRUE);
            GFL_SndSEPlay(0x556);
            return TRCARD_INPUT_EXIT;
        }
    } else if (keys & (PAD_KEY_LEFT | PAD_KEY_RIGHT)) {
        if (wk->zoomed == 0) {
            func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_FLIP, 0x17, TRUE);
            return TRCARD_INPUT_FLIP;
        }
    } else if (keys & PAD_BUTTON_Y) {
        if (!wk->data->unk04_7) {
            func_ov111_021c513c(wk);
        }
    } else if (keys & PAD_BUTTON_A) {
        if (!wk->data->unk04_7 && wk->side == 0) {
            GFL_SndSEPlay(0x54c);
            func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_BADGES, 0x11, TRUE);
            return TRCARD_INPUT_BADGES;
        }
    }
    return TRCARD_INPUT_NONE;
}

// Registers the side shown with the Y button's shortcuts, or takes it off them
// pokebw2's TrainerCard_ToggleShortcut
static void func_ov111_021c513c(TrainerCardWork *wk) {
    BOOL registered;

    GFL_SndSEPlay(SEQ_SE_SYS_07);
    if (wk->side == 0) {
        registered = GameData_IsShortcutRegistered(wk->param->gameData, SHORTCUT_TRAINER_CARD_FRONT) ^ 1;
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_SHORTCUT, registered + 6, TRUE);
        GameData_SetKeyItemRegistration(wk->param->gameData, SHORTCUT_TRAINER_CARD_FRONT, registered);
    } else {
        registered = GameData_IsShortcutRegistered(wk->param->gameData, SHORTCUT_TRAINER_CARD_BACK) ^ 1;
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_SHORTCUT, registered + 6, TRUE);
        GameData_SetKeyItemRegistration(wk->param->gameData, SHORTCUT_TRAINER_CARD_BACK, registered);
    }
}

// The button that switches between the pen and the eraser, which shares its actor with the badge case's on the front
// pokebw2's TrainerCard_SetPenButton
static void func_ov111_021c51b4(TrainerCardWork *wk, int eraser, int visible) {
    if (eraser == 0) {
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_BADGES, 10, visible);
    } else {
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_BADGES, 12, visible);
    }
}

// pokebw2's TrainerCard_PressPenButton
static void func_ov111_021c51e4(TrainerCardWork *wk, int eraser) {
    if (eraser == 0) {
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_BADGES, 11, TRUE);
        func_0204b3dc(wk->obj.actors[TRCARD_ACTOR_ZOOM], FALSE);
        func_0204b3dc(wk->obj.actors[TRCARD_ACTOR_BRUSH], FALSE);
    } else {
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_BADGES, 13, TRUE);
        func_0204b3dc(wk->obj.actors[TRCARD_ACTOR_ZOOM], TRUE);
        func_0204b3dc(wk->obj.actors[TRCARD_ACTOR_BRUSH], TRUE);
    }
}

// pokebw2's TrainerCard_SetSignBlink
static void func_ov111_021c523c(TrainerCardWork *wk, int eraser) {
    if (eraser) {
        wk->blinkMode = 0;
        wk->blinkCount = 0;
    } else {
        wk->blinkMode = 2;
    }
    func_ov111_021c59dc(7, wk->blinkMode);
}

// pokebw2's TrainerCard_IsLocked
static BOOL func_ov111_021c5264(TrainerCardWork *wk, int button) {
    if (wk->data->unk04_5 && button >= 3 && button <= 9) {
        return TRUE;
    }
    return FALSE;
}

int func_ov111_021c5280(TrainerCardWork *wk, int button) {
    if (func_ov111_021c5264(wk, button) == TRUE) {
        return TRCARD_INPUT_NONE;
    }
    switch (button) {
    case 0:
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_RETURN, 9, TRUE);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_020355d8(TRUE);
        return TRCARD_INPUT_RETURN;
    case 1:
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_EXIT, 8, TRUE);
        GFL_SndSEPlay(SEQ_SE_CLOSE1);
        return TRCARD_INPUT_EXIT;
    case 2:
        func_02035cb8(&wk->touchX, &wk->touchY);
        func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_FLIP, 23, TRUE);
        return TRCARD_INPUT_FLIP;
    case 3:
        if (wk->side != 0) {
            if (wk->param->data->canEdit) {
                func_ov111_021c51e4(wk, wk->data->unk04_4);
                wk->data->unk04_4 ^= 1;
                func_ov111_021c4754(wk, wk->data->version);
                func_ov111_021c523c(wk, wk->data->unk04_4);
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
        } else {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_BADGES, 17, TRUE);
            return TRCARD_INPUT_BADGES;
        }
        break;
    case 4:
        if (wk->side != 0 && !wk->data->unk04_4 && wk->param->data->canEdit) {
            if (wk->zoomed == 0) {
                func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_ZOOM, 19, TRUE);
                GFL_SndSEPlay(SEQ_SE_SYS_09);
                wk->subSeq = 0;
            } else {
                func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_ZOOM, 21, TRUE);
                GFL_SndSEPlay(SEQ_SE_SYS_10);
                wk->subSeq = 3;
            }
            return TRCARD_INPUT_ZOOM;
        }
        break;
    case 5:
        if (wk->side != 0 && wk->param->data->canEdit && !wk->data->unk04_4) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_BRUSH, 9 - wk->brush * 2, TRUE);
            wk->brush ^= 1;
        }
        break;
    case 6:
        func_ov111_021c513c(wk);
        break;
    case 7:
        if (wk->side == 0) {
            return TRCARD_INPUT_UNION_VIEW;
        }
        break;
    case 8:
        if (wk->side == 0) {
            return TRCARD_INPUT_PERSONALITY;
        }
        break;
    case 9:
        if (wk->side == 0) {
            return TRCARD_INPUT_GREETING;
        }
        break;
    }
    return TRCARD_INPUT_NONE;
}


int func_ov111_021c5484(TrainerCardWork *wk, int button) {
    switch (button) {
    case 4:
        if (wk->side != 0) {
            func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_ZOOM, 21, TRUE);
            GFL_SndSEPlay(SEQ_SE_SYS_10);
            wk->subSeq = 3;
            return TRCARD_INPUT_ZOOM;
        }
        break;
    case 5:
        if (wk->side != 0) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov111_021c6fa0(&wk->obj, TRCARD_ACTOR_BRUSH, 9 - wk->brush * 2, TRUE);
            wk->brush ^= 1;
        }
        break;
    case 6:
        func_ov111_021c513c(wk);
        break;
    case 10:
        if (wk->zoomSide == 0) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            wk->subSeq = 9;
            return TRCARD_INPUT_ZOOM;
        }
        break;
    case 11:
        if (wk->zoomSide == 1) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            wk->subSeq = 6;
            return TRCARD_INPUT_ZOOM;
        }
        break;
    }
    return TRCARD_INPUT_NONE;
}


void func_ov111_021c555c(TrainerCardWork *wk) {
    if (wk->side != 0 && func_02035ba4(data_ov111_021c8544) != 0 && wk->scrolled == 1) {
        func_ov111_021c6874(wk, wk->scroll, TRUE, 0);
        wk->scrolled = 0;
        wk->unk331C = 0;
        wk->scrollTouch = 0;
    }
}

static void func_ov111_021c55a4(TrainerCardWork *wk) {
    u32 x, y;

    if (func_02035c08(data_ov111_021c8544) == 0) {
        wk->scrollTouch = 1;
    }
    if (wk->scrollTouch == 0 || func_02035ba4(data_ov111_021c8544) != 0) {
        return;
    }
    if (func_02035c60()) {
        func_02035d20(&x, &y);
        wk->dragY = y;
        wk->dragScroll = wk->scroll;
    }
    if (func_02035c38()) {
        func_02035cb8(&x, &y);
        wk->scroll = wk->dragScroll - (wk->dragY - y);
    }
    if (wk->scroll < -(wk->rowCount - 4) * 16) {
        wk->scroll = -(wk->rowCount - 4) * 16;
    } else if (wk->scroll > 0) {
        wk->scroll = 0;
    }
    if (wk->oldScroll != wk->scroll) {
        wk->scrolled = 1;
        func_ov111_021c6874(wk, wk->scroll, TRUE, 1);
        GFL_SndSEPlay(SEQ_SE_SYS_06);
    }
    wk->oldScroll = wk->scroll;
}


int func_ov111_021c567c(TrainerCardWork *wk) {
    const TouchRect *rects;
    s32 hit;
    int ret;

    if (wk->data->unk04_7 == 0) {
        rects = data_ov111_021c8594;
    } else {
        rects = data_ov111_021c8554;
    }
    hit = func_02035c08(rects);
    if (wk->zoomed == 0) {
        ret = func_ov111_021c5280(wk, hit);
    } else {
        ret = func_ov111_021c5484(wk, hit);
    }
    if (ret == 0) {
        if (wk->side != 0) {
            if (wk->zoomed == 0) {
                func_ov111_021c55a4(wk);
                func_ov111_021c5bcc(wk, 0);
            } else {
                func_ov111_021c5bcc(wk, 1);
            }
        }
        ret = 0;
    }
    return ret;
}



int func_ov111_021c56e8(TrainerCardWork *wk) {
    func_ov111_021c5008(wk);
    func_ov111_021c555c(wk);
    func_ov111_021c5b80(wk);
    if (wk->touchMode == 1) {
        return func_ov111_021c567c(wk);
    }
    return func_ov111_021c5054(wk);
}

void func_ov111_021c5718(TrainerCardWork *wk, int input, u32 *state) {
    switch (input) {
    case TRCARD_INPUT_FLIP:
        wk->subSeq = 0;
        *state = TRCARD_SEQ_FLIP;
        break;
    case TRCARD_INPUT_ZOOM:
        if (wk->param->data->canEdit) {
            *state = TRCARD_SEQ_ZOOM;
        }
        break;
    case TRCARD_INPUT_GREETING:
        if (wk->param->data->canEdit) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            wk->subSeq = 0;
            *state = TRCARD_SEQ_GREETING;
        }
        break;
    case TRCARD_INPUT_RETURN:
        *state = TRCARD_SEQ_RETURN;
        wk->param->next = 0;
        wk->param->result = 0;
        break;
    case TRCARD_INPUT_EXIT:
        *state = TRCARD_SEQ_EXIT;
        wk->param->next = 0;
        wk->param->result = 1;
        break;
    case TRCARD_INPUT_UNION_VIEW:
        if (wk->param->data->canEdit) {
            func_02008a00(getTimeSigSaveBlock(wk->param->gameData));
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov111_021c3ef8(wk->data);
            func_ov111_021c61ec(wk, wk->data->unk06, TRUE);
        }
        break;
    case TRCARD_INPUT_PERSONALITY:
        if (wk->param->data->canEdit) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            wk->data->unk48++;
            if (wk->data->unk48 > 24) {
                wk->data->unk48 = 0;
            }
            func_ov111_021c628c(wk, wk->data->unk48, TRUE);
        }
        break;
    case TRCARD_INPUT_BADGES:
        wk->subSeq = 0;
        *state = TRCARD_SEQ_BADGES;
        break;
    }
}


void func_ov111_021c581c(TCB *tcb, void *data) {
    TrainerCardWork *wk = data;
    int y;

    func_ov111_021c5b34(wk);
    y = -(wk->frame / 2);
    GFL_BGSysMoveBG(5, BG_MOVE_SET_X, y);
    GFL_BGSysMoveBG(5, BG_MOVE_SET_Y, y);
    GFL_BGSysMoveBG(3, BG_MOVE_SET_X, y);
    GFL_BGSysMoveBG(3, BG_MOVE_SET_Y, y);
    if (wk->affineDirty) {
        func_ov111_021c4f80(wk);
    }
    GFL_BGSysUpdate();
    func_0204a648();
}
// pokebw2's TrainerCard_ResetAffine
static void func_ov111_021c5874(void) {
    MtxFx22 mtx;
    fx32 x = FX_Inv(FX32_ONE);
    fx32 y = FX_Inv(FX32_ONE);

    mtx._00 = x;
    mtx._01 = 0;
    mtx._10 = 0;
    mtx._11 = y;
    OS_WaitVBlankIntr();
    G2S_SetBG2Affine(&mtx, 128, 96, 0, 0);
    G2S_SetBG3Affine(&mtx, 128, 96, 0, 0);
    OS_WaitVBlankIntr();
    G2_SetBG3Affine(&mtx, 128, 0, 0, 0);
}

// The trainer's picture on the front
// pokebw2's TrainerCard_DrawTrainer
void func_ov111_021c58e0(TrainerCardWork *wk) {
    GFL_BGSysLoadChar(7, wk->charData->pRawData, wk->charData->szByte, 0);
    GFL_BGSysLoadScrArea(7, 21, 2, 10, 11, wk->scrData->rawData, 21, 2, wk->scrData->screenWidth / 8,
                         wk->scrData->screenHeight / 8);
    GFL_BGSysLoadScr(7);
}

// pokebw2's TrainerCard_ClearSignArea
void func_ov111_021c5938(TrainerCardWork *wk) {
    GFL_BGSysFillScrArea(7, 0, 20, 6, 6, 9, 16);
}

// The signature of the save, a bit for each pixel, as 8-bit characters
// pokebw2's TrainerCard_UnpackSign
void func_ov111_021c5958(const u8 *src, u8 *dest) {
    int i;

    for (i = 0; i < TRCARD_SIGN_PIXELS; i++) {
        u8 row = (i / 8) % 8;
        u8 bit = i % 8;

        if ((src[(i / 64) * 8 + row] >> bit) & 1) {
            dest[i] = 7;
        }
    }
}

// pokebw2's TrainerCard_PackSign
static void func_ov111_021c59b0(const u8 *src, u8 *dest) {
    int i;
    int bit;
    int byte;

    for (i = 0; i < TRCARD_SIGN_BYTES; i++) {
        byte = 0;
        for (bit = 0; bit < 8; bit++) {
            byte |= (src[i * 8 + bit] & 1) << bit;
        }
        dest[i] = byte;
    }
}

// The screen of the signature's 24x8 characters: all of it (2), or only one of the two frames of the eraser's
// blinking (0 and 1)
// pokebw2's TrainerCard_SetSignScreen
static void func_ov111_021c59dc(u32 bg, int mode) {
    u16 *screen = GFL_BGSysIsScrHeapExists(bg);
    u8 x;
    u8 y;
    u16 tile;

    for (y = 0; y < 8; y++) {
        for (x = 0; x < 24; x++) {
            screen[(y + 11) * 32 + x + 4] = 0;
        }
    }
    tile = 0;
    switch (mode) {
    case 2:
        for (y = 0; y < 8; y++) {
            for (x = 0; x < 24; x++) {
                tile++;
                screen[(y + 11) * 32 + x + 4] = tile;
            }
        }
        break;
    case 0:
        for (y = 0; y < 8; y++) {
            for (x = 0; x < 12; x++) {
                tile++;
                screen[(y + 11) * 32 + x + 10] = tile;
            }
            tile += 12;
        }
        break;
    case 1:
        tile += 12;
        for (y = 0; y < 8; y++) {
            for (x = 0; x < 12; x++) {
                tile++;
                screen[(y + 11) * 32 + x + 10] = tile;
            }
            tile += 12;
        }
        break;
    }
    GFL_BGSysQueueScrLoad(bg);
}

// pokebw2's TrainerCard_LoadSign
void func_ov111_021c5acc(u32 bg, const u8 *pixels, u8 mode) {
    GFL_BGSysLoadChar(bg, pixels, TRCARD_SIGN_PIXELS, 1);
    func_ov111_021c59dc(bg, mode);
}

// pokebw2's TrainerCard_UpdateSignBlink
static void func_ov111_021c5aec(TrainerCardWork *wk) {
    if (wk->side != 0 && wk->data->unk04_4) {
        wk->blinkCount++;
        if (wk->blinkCount > 20) {
            wk->blinkMode ^= 1;
            func_ov111_021c59dc(7, wk->blinkMode);
            wk->blinkCount = 0;
        }
    }
}

// pokebw2's TrainerCard_AnimatePalette
void func_ov111_021c5b34(TrainerCardWork *wk) {
    u16 *colors = wk->palData->pRawData;

    wk->palFrame++;
    if (wk->palFrame > 8) {
        wk->palFrame = 0;
    }
    if (wk->palFrame == 0) {
        GXS_LoadBGPltt(colors + 3, 6, 4);
    } else if (wk->palFrame == 4) {
        GXS_LoadBGPltt(colors + 5, 6, 4);
    }
}

// Ends a stroke when the pen leaves the signature's surroundings
// pokebw2's TrainerCard_CheckPenRelease
static void func_ov111_021c5b80(TrainerCardWork *wk) {
    if ((wk->side != 0 && wk->zoomed == 0 && func_02035ba4(data_ov111_021c8534) != 0) ||
        (wk->zoomed == 1 && func_02035ba4(data_ov111_021c854c) != 0)) {
        wk->penTouch = 0;
    }
}

// pokebw2's TrainerCard_ReadPen
static void func_ov111_021c5bcc(TrainerCardWork *wk, int zoomed) {
    u32 x;
    u32 y;

    if (wk->data->unk04_5 || wk->data->unk04_4 == TRUE) {
        return;
    }
    if (func_02035cb8(&x, &y)) {
        if (zoomed == 0) {
            if (func_02035c08(data_ov111_021c852c) == 0) {
                wk->penTouch = 1;
            }
            if (wk->penTouch != 0 && func_02035d40(data_ov111_021c852c, x, y) == 0) {
                wk->pen.x = x;
                wk->pen.y = y;
                wk->pen.brush = wk->brush;
                wk->pen.on = 1;
                if (x < 128) {
                    wk->zoomSide = 0;
                } else {
                    wk->zoomSide = 1;
                }
            } else {
                wk->pen.on = 0;
            }
        } else {
            if (func_02035c08(data_ov111_021c853c) == 0) {
                wk->penTouch = 1;
            }
            if (wk->penTouch != 0 && func_02035d40(data_ov111_021c853c, x, y) == 0) {
                wk->pen.x = (x - 32 + wk->zoomSide * 192) / 2 + 32;
                wk->pen.y = (y - 24) / 2 + 88;
                wk->pen.brush = wk->brush;
                wk->pen.on = 1;
            } else {
                wk->pen.on = 0;
            }
        }
    } else {
        wk->pen.on = 0;
    }
    wk->penNew.x = wk->pen.x;
    wk->penNew.y = wk->pen.y;
    wk->penNew.brush = wk->pen.brush;
    wk->penNew.on = wk->pen.on;
}

// Stamps a 4x4 brush on the signature's characters. On the zoomed signature only the half shown is drawn on
static void func_ov111_021c5cf4(u8 *pixels, const u8 *brush, int x, int y, int width, int height, int zoomed,
                                  int zoomSide) {
    int px;
    int py;
    int i;
    int j;
    int n = 0;
    int offset;

    for (j = 0; j < 4; j++) {
        py = y + j;
        for (i = 0; i < 4; i++) {
            px = x + i;
            if (px < 0 || py < 0) {
                n++;
                continue;
            }
            if (px >= width || py >= height) {
                n++;
                continue;
            }
            if (zoomed == 1) {
                if ((zoomSide == 0 && px >= TRCARD_SIGN_WIDTH / 2) ||
                    (zoomSide == 1 && px <= TRCARD_SIGN_WIDTH / 2 - 1)) {
                    n++;
                    continue;
                }
            }
            offset = px % 8 + ((py % 8) * 8 + (px / 8 + (py / 8) * (TRCARD_SIGN_WIDTH / 8)) * 64);
            if (brush[n] == 1) {
                pixels[offset] = 7;
            } else if (brush[n] == 2) {
                pixels[offset] = 0;
            }
            n++;
        }
    }
}

// Draws the stroke from the last point to (x, y) and returns how many brushes it took
static int func_ov111_021c5db8(u8 *pixels, const u8 *brush, int x, int y, int *lastX, int *lastY, int first, int on,
                                int zoomed, int zoomSide) {
    int count = 0;
    int x0 = *lastX;
    int y0 = *lastY;
    int endX = x;
    int dx;
    int dy;
    int endY = y;
    int stepY;
    int stepX;
    int error;

    if (first == 0 && on == 0) {
        *lastX = x;
        *lastY = y;
        return count;
    }
    dx = x - x0;
    if (dx < 0) {
        dx = -dx;
    }
    dy = y - y0;
    if (dy < 0) {
        dy = -dy;
    }
    if (dx > dy) {
        if (x0 > x) {
            if (y0 > y) {
                stepY = 1;
            } else {
                stepY = -1;
            }
            endX = x0;
            x0 = x;
            y0 = y;
        } else {
            if (y0 < y) {
                stepY = 1;
            } else {
                stepY = -1;
            }
        }
        func_ov111_021c5cf4(pixels, brush, x0, y0, TRCARD_SIGN_WIDTH, TRCARD_SIGN_HEIGHT, zoomed, zoomSide);
        count++;
        error = dx >> 1;
        for (x0++; x0 <= endX; x0++) {
            error -= dy;
            if (error < 0) {
                error += dx;
                y0 += stepY;
            }
            func_ov111_021c5cf4(pixels, brush, x0, y0, TRCARD_SIGN_WIDTH, TRCARD_SIGN_HEIGHT, zoomed, zoomSide);
            count++;
        }
    } else {
        if (y0 > y) {
            if (x0 > x) {
                stepX = 1;
            } else {
                stepX = -1;
            }
            endY = y0;
            y0 = y;
            x0 = x;
        } else {
            if (x0 < x) {
                stepX = 1;
            } else {
                stepX = -1;
            }
        }
        func_ov111_021c5cf4(pixels, brush, x0, y0, TRCARD_SIGN_WIDTH, TRCARD_SIGN_HEIGHT, zoomed, zoomSide);
        count++;
        error = dy >> 1;
        for (y0++; y0 <= endY; y0++) {
            error -= dx;
            if (error < 0) {
                error += dy;
                x0 += stepX;
            }
            func_ov111_021c5cf4(pixels, brush, x0, y0, TRCARD_SIGN_WIDTH, TRCARD_SIGN_HEIGHT, zoomed, zoomSide);
            count++;
        }
    }
    *lastX = x;
    *lastY = y;
    return count;
}

// pokebw2's TrainerCard_CopyPen
static void func_ov111_021c5f50(TrainerCardPen *src, TrainerCardPen *dest) {
    dest->x = src->x;
    dest->y = src->y;
    dest->brush = src->brush;
    dest->on = src->on;
}

// Draws the stroke up to where the pen is now, and returns how many brushes it took
static int func_ov111_021c5f64(u8 *pixels, TrainerCardPen *pen, TrainerCardPen *old, BOOL load, const u8 *loadSrc,
                               u8 zoomed, u8 zoomSide) {
    int lastX;
    int lastY;
    BOOL drawn = FALSE;
    int count = 0;
    int offset = zoomed == 0 ? 4 : 2;
    if (pen->on) {
        if (old->on) {
            lastX = old->x - 32 - offset;
            lastY = old->y - 90;
        }
        count = func_ov111_021c5db8(pixels, data_ov111_021c86a8[zoomed][pen->brush], pen->x - 32 - offset, pen->y - 90,
                                     &lastX, &lastY, 0, old->on, zoomed, zoomSide);
        drawn = TRUE;
    }
    if (drawn && load) {
        GFL_BGSysLoadChar(7, loadSrc, TRCARD_SIGN_PIXELS, 1);
    }
    func_ov111_021c5f50(pen, old);
    pen->on = 0;
    return count;
}
