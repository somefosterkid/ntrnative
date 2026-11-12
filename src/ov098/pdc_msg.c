// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov172/pdc_msg.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Entree Forest capture's message window (overlay 172): the messages of the scene, in the box on the bottom
// screen's BG 1, which fades in and out by alpha blending. The name is the ROM's string, from GFL_HeapAllocate's assert

#include "types.h"
#include "app/pdc_msg.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "battle/btlv_effect.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "save/config.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wordset.h"
#include "text/system/btl_main_10.h"
#include "save/timesig_savedata.h"

// The BG of the message window
#define PDC_MSG_BG 1
// The window's fade, in 1/4096ths of a blend coefficient per frame
enum {
    PDC_MSG_FADE_SHOWN,
    PDC_MSG_FADE_OUT,
    PDC_MSG_FADE_HIDDEN,
    PDC_MSG_FADE_IN,
    PDC_MSG_FADE_IN_START,
};
// The steps of func_ov098_021b6a3c
enum {
    PDC_MSG_STEP_FADE_IN_START,
    PDC_MSG_STEP_FADE_IN,
    PDC_MSG_STEP_PRINT,
    PDC_MSG_STEP_WAIT_KEY,
    PDC_MSG_STEP_WAIT,
    PDC_MSG_STEP_NEXT_PAGE,
    PDC_MSG_STEP_DONE,
};

typedef struct {
    BmpWin *win;
    s32 evA;       // the blend coefficient of the window, with 12 fractional bits
    s32 evB;       // and of what is behind it
    s32 evADelta;
    s32 evBDelta;
    u8 state;
    u8 frames;
} PdcMsgFade;

struct PdcMsg {
    PrintStream *stream;
    PdcMsgFade fade;
    BmpWin *win;
    MsgData *msgData;
    Font *font;
    TCBExManager *tcbManager;
    StrBuf *strbuf;
    WordSet *wordSet;
    s32 printWait;
    u8 instant; // printed at once, not a character at a time
    u8 step;
    u8 finished; // set once when the stream is done, for func_ov098_021b6b74
    u32 streamState;
    u16 timer;
    u16 timerReset;
    HeapID heapId;
};

static void func_ov098_021b6b8c(PdcMsg *msg, int waitFrames);
void func_ov098_021b6c34(PdcMsgFade *fade, BmpWin *win);
static void func_ov098_021b6c60(PdcMsgFade *fade);
static void func_ov098_021b6c90(PdcMsgFade *fade, u8 instant);
static BOOL func_ov098_021b6cdc(PdcMsgFade *fade);

// pokebw2's sMsgBGSetup
static const BGSetup data_ov098_021b6e24 = {
    0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x4800, GX_BG_CHARBASE_0x10000, 0x8000,
    GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, 0,
};

// pokebw2's PdcMsg_Free

PdcMsg *func_ov098_021b67d4(Font *font, const Config *config, TCBExManager *tcbManager, HeapID heapId) {
    PdcMsg *msg = GFL_HeapAllocate(heapId, sizeof(PdcMsg), TRUE, "pdc_msg.c", 130);
    u32 frameChar;

    GFL_BGSysCreateBG(PDC_MSG_BG, &data_ov098_021b6e24, BGMODE_TEXT);
    msg->heapId = heapId;
    msg->tcbManager = tcbManager;
    msg->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_MAIN_10, msg->heapId);
    msg->font = font;
    msg->printWait = func_02013080(func_02008374(config));
    msg->strbuf = GFL_StrBufCreate(256, heapId);
    msg->wordSet = GFL_WordSetSystemCreateDefault(heapId);
    GFL_BGSysFillScrArea(PDC_MSG_BG, 0, 0, 0, 32, 32, 17);
    GFL_BGSysFillChar(PDC_MSG_BG, 0, 1, 0);
    frameChar = GFL_BGSysLoadNCGRDynamic(11, 0x59, PDC_MSG_BG, 0, FALSE, msg->heapId);
    func_020215ec(func_ov094_021f86e0(), 11, 0x5a, msg->heapId, 0, 0x20, 0, 0);
    msg->win = BmpWin_CreateDynamic(PDC_MSG_BG, 1, 19, 30, 4, 0, 0);
    BmpWin_FlushMap(msg->win);
    func_020454e0(msg->win, frameChar, 0);
    GFL_BGSysLoadScr(PDC_MSG_BG);
    GFL_BGSysSetBGEnabled(PDC_MSG_BG, FALSE);
    func_ov098_021b6c34(&msg->fade, msg->win);
    return msg;
}

void func_ov098_021b68dc(PdcMsg *msg) {
    GFL_StrBufFree(msg->strbuf);
    GFL_WordSetSystemFree(msg->wordSet);
    if (msg->stream != NULL) {
        func_0201ce48(msg->stream);
    }
    GFL_BGSysReleaseBG(PDC_MSG_BG);
    GFL_MsgDataFree(msg->msgData);
    BmpWin_Free(msg->win);
    GFL_HeapFree(msg);
}

// pokebw2's PdcMsg_StartSawDream
void func_ov098_021b6910(PdcMsg *msg, PdcWork *pdc) {
    StrBuf *src = GFL_MsgDataLoadStrbufNew(msg->msgData, BtlMain10_Text_000);

    loadPokemonNicknameToStrbuf(msg->wordSet, 0, func_ov098_021b6140(pdc));
    GFL_WordSetFormatStrbuf(msg->wordSet, msg->strbuf, src);
    func_ov098_021b6b8c(msg, 80);
    GFL_StrBufFree(src);
}

// pokebw2's PdcMsg_StartWhatWill
void func_ov098_021b694c(PdcMsg *msg, PdcWork *pdc) {
    StrBuf *src = GFL_MsgDataLoadStrbufNew(msg->msgData, BtlMain10_Text_003);

    copyVarForText(msg->wordSet, 0, func_ov098_021b6148(pdc));
    GFL_WordSetFormatStrbuf(msg->wordSet, msg->strbuf, src);
    func_ov098_021b6b8c(msg, 0);
    GFL_StrBufFree(src);
}

// pokebw2's PdcMsg_StartComeHere
void func_ov098_021b6988(PdcMsg *msg, PdcWork *pdc) {
    StrBuf *src = GFL_MsgDataLoadStrbufNew(msg->msgData, BtlMain10_Text_001);

    loadPokemonNicknameToStrbuf(msg->wordSet, 0, func_ov098_021b6140(pdc));
    GFL_WordSetFormatStrbuf(msg->wordSet, msg->strbuf, src);
    func_ov098_021b6b8c(msg, 80);
    GFL_StrBufFree(src);
}

// pokebw2's PdcMsg_StartJoinedParty
void func_ov098_021b69c4(PdcMsg *msg, PdcWork *pdc) {
    StrBuf *src = GFL_MsgDataLoadStrbufNew(msg->msgData, BtlMain10_Text_002);

    loadPokemonNicknameToStrbuf(msg->wordSet, 0, func_ov098_021b6140(pdc));
    GFL_WordSetFormatStrbuf(msg->wordSet, msg->strbuf, src);
    func_ov098_021b6b8c(msg, 80);
    GFL_StrBufFree(src);
}

// pokebw2's PdcMsg_StartLeft
void func_ov098_021b6a00(PdcMsg *msg, PdcWork *pdc) {
    StrBuf *src = GFL_MsgDataLoadStrbufNew(msg->msgData, BtlMain10_Text_004);

    copyVarForText(msg->wordSet, 0, func_ov098_021b6148(pdc));
    GFL_WordSetFormatStrbuf(msg->wordSet, msg->strbuf, src);
    func_ov098_021b6b8c(msg, 80);
    GFL_StrBufFree(src);
}

// pokebw2's PdcMsg_Step
BOOL func_ov098_021b6a3c(PdcMsg *msg) {
    switch (msg->step) {
    case PDC_MSG_STEP_FADE_IN_START:
        func_ov098_021b6c90(&msg->fade, msg->instant);
        msg->step++;
        // fallthrough
    case PDC_MSG_STEP_FADE_IN:
        if (!func_ov098_021b6cdc(&msg->fade)) {
            break;
        }
        if (msg->instant) {
            msg->step = PDC_MSG_STEP_DONE;
            break;
        }
        if (msg->stream != NULL) {
            func_0201ce20(msg->stream);
        }
        msg->step++;
        // fallthrough
    case PDC_MSG_STEP_PRINT:
        if (msg->stream != NULL) {
            msg->streamState = func_0201ce30(msg->stream);
            if (msg->streamState != PRINT_STREAM_RUNNING) {
                msg->step = PDC_MSG_STEP_WAIT_KEY;
                if (msg->streamState == PRINT_STREAM_DONE) {
                    msg->finished = TRUE;
                }
            }
            break;
        }
        return TRUE;
    case PDC_MSG_STEP_WAIT_KEY:
        if ((GCTX_HIDGetPressedKeys() & 3) || func_02035c60()) {
            msg->timer = 0;
        }
        if (msg->timer != 0) {
            msg->timer--;
        } else {
            msg->step = PDC_MSG_STEP_NEXT_PAGE;
        }
        break;
    case PDC_MSG_STEP_WAIT:
        if (msg->timer != 0) {
            msg->timer--;
        } else {
            msg->step = PDC_MSG_STEP_NEXT_PAGE;
        }
        break;
    case PDC_MSG_STEP_NEXT_PAGE:
        if (msg->streamState == PRINT_STREAM_DONE) {
            func_0201dd68();
            func_0201ce48(msg->stream);
            msg->stream = NULL;
            msg->step = PDC_MSG_STEP_DONE;
            return TRUE;
        }
        func_0201ce38(msg->stream);
        msg->step = PDC_MSG_STEP_PRINT;
        msg->timer = msg->timerReset;
        break;
    case PDC_MSG_STEP_DONE:
        return TRUE;
    }
    return FALSE;
}

// pokebw2's PdcMsg_StartHide
void func_ov098_021b6b5c(PdcMsg *msg) {
    func_ov098_021b6c60(&msg->fade);
}

// pokebw2's PdcMsg_IsHideDone
BOOL func_ov098_021b6b68(PdcMsg *msg) {
    return func_ov098_021b6cdc(&msg->fade);
}

// pokebw2's PdcMsg_ConsumeFinished
BOOL func_ov098_021b6b74(PdcMsg *msg) {
    if (msg->finished) {
        msg->finished = FALSE;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's PdcMsg_Print
static void func_ov098_021b6b8c(PdcMsg *msg, int waitFrames) {
    GFL_BitmapFill(BmpWin_GetBitmap(msg->win), 12);
    GFL_TextRndUpdateColorIndexLUT(1, 9, 12);
    GFL_BGSysSetBGEnabled(PDC_MSG_BG, TRUE);
    if (waitFrames != 0) {
        msg->stream = func_0201cce4(msg->win, 0, 0, msg->strbuf, msg->font, msg->printWait, msg->tcbManager, 0,
                                    msg->heapId, 12);
        func_0201ce0c(msg->stream);
        msg->instant = FALSE;
    } else {
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(msg->win), 0, 0, msg->strbuf, msg->font);
        BmpWin_FlushMap(msg->win);
        GFL_BGSysLoadScr(PDC_MSG_BG);
        BmpWin_FlushChar(msg->win);
        msg->instant = TRUE;
    }
    msg->step = PDC_MSG_STEP_FADE_IN_START;
    msg->timer = waitFrames;
    msg->timerReset = waitFrames;
    msg->finished = FALSE;
}

// pokebw2's PdcMsgFade_Init
void func_ov098_021b6c34(PdcMsgFade *fade, BmpWin *win) {
    fade->win = win;
    fade->state = PDC_MSG_FADE_SHOWN;
    fade->evA = 31 << 12;
    fade->evB = 7 << 12;
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1,
                     GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ |
                         GX_BLEND_PLANEMASK_BD,
                     31, 7);
}

// Fades the window out
// pokebw2's PdcMsgFade_StartOut
static void func_ov098_021b6c60(PdcMsgFade *fade) {
    if (fade->state == PDC_MSG_FADE_SHOWN || fade->state == PDC_MSG_FADE_IN) {
        fade->evA = 31 << 12;
        fade->evB = 7 << 12;
        fade->evADelta = -0x52aa;
        fade->evBDelta = 6 << 10;
        fade->frames = 6;
        fade->state = PDC_MSG_FADE_OUT;
    }
}

// Fades the window in, after clearing it when the message is printed a character at a time
// pokebw2's PdcMsgFade_StartIn
static void func_ov098_021b6c90(PdcMsgFade *fade, u8 instant) {
    if (fade->state == PDC_MSG_FADE_OUT || fade->state == PDC_MSG_FADE_HIDDEN) {
        if (!instant) {
            GFL_BitmapFill(BmpWin_GetBitmap(fade->win), 12);
            BmpWin_FlushChar(fade->win);
        }
        fade->evA = 0;
        fade->evB = 1 << 16;
        fade->evADelta = 0x52aa;
        fade->evBDelta = -0x1800;
        fade->frames = 6;
        fade->state = PDC_MSG_FADE_IN_START;
    }
}

// Steps the fade a frame, which returns whether it is at rest
// pokebw2's PdcMsgFade_Update
static BOOL func_ov098_021b6cdc(PdcMsgFade *fade) {
    switch (fade->state) {
    case PDC_MSG_FADE_HIDDEN:
        return TRUE;
    case PDC_MSG_FADE_SHOWN:
        return TRUE;
    case PDC_MSG_FADE_IN_START:
        fade->state = PDC_MSG_FADE_IN;
        return FALSE;
    case PDC_MSG_FADE_IN:
        if (fade->frames != 0) {
            fade->evA += fade->evADelta;
            fade->evB += fade->evBDelta;
            fade->frames--;
        } else {
            fade->evA = 31 << 12;
            fade->evB = 7 << 12;
            fade->state = PDC_MSG_FADE_SHOWN;
        }
        break;
    case PDC_MSG_FADE_OUT:
        if (fade->frames != 0) {
            fade->evA += fade->evADelta;
            fade->evB += fade->evBDelta;
            fade->frames--;
        } else {
            fade->evA = 0;
            fade->evB = 1 << 16;
            fade->state = PDC_MSG_FADE_HIDDEN;
        }
        break;
    }
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1,
                     GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ |
                         GX_BLEND_PLANEMASK_BD,
                     (u8)(fade->evA >> 12), (u8)(fade->evB >> 12));
    return FALSE;
}
