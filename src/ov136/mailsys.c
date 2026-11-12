// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov215/mailsys.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/pms_input.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "field/mailsys.h"
#include "field/mailtool.h"
#include "field/mailview.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/bmp.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/os.h"
#include "save/records.h"
#include "save/save_control.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/pms_draw.h"
#include "system/pmsi_param.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wipe.h"
#include "text/system/btl_server_flow_3.h"
#include "field/beacon_view.h"
#include "pml/mail.h"

// The mail screen's procs (overlay 215's mailsys.c): one runs the screen of the mail, and the sentence input when the
// player picks a sentence to write; the other is the screen itself, whose phases are the keys and touch input

// The record of mails written
#define RECORD_MAILS_WRITTEN 0x1d

// The phases of the screen, which the table runs
enum {
    MAIL_PHASE_NOTICE = 2,
    MAIL_PHASE_CANCEL = 3,
};

// The steps of func_ov136_021c42c8
enum {
    MAIL_SEQ_INIT,
    MAIL_SEQ_LOAD,
    MAIL_SEQ_FADE_IN,
    MAIL_SEQ_PHASE,
    MAIL_SEQ_FADE_OUT,
    MAIL_SEQ_UNLOAD,
};

// The work of the screen's proc
// Black 2's; twinned code confirms the offsets of heapId, pmsiParam, edit, sentence and procMgr in Black
typedef struct {
    u32 heapId;
    u32 unk04;
    PMSIParam *pmsiParam;
    u32 unk0C;
    MailEdit *edit;
    PMSData sentence;
    GameProcManager *procMgr;
} MailProcWork;

typedef BOOL (*MailSysPhaseFunc)(MailSysWork *work);

static void func_ov136_021c3fc8(MailProcWork *work, MailParam *param);
static MailEdit *func_ov136_021c409c(MailData *mail, u32 heapId);
static void func_ov136_021c4134(MailEdit *edit);
static void func_ov136_021c414c(MailData *mail, MailEdit *edit);
static void func_ov136_021c4184(MailSysWork *work, MailEdit *edit);
static BOOL func_ov136_021c41e0(GameProc *proc, u32 *state, void *param, void *unused);
static BOOL func_ov136_021c427c(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov136_021c4290(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov136_021c42c8(MailSysWork *work);
static BOOL func_ov136_021c446c(MailSysWork *work);
static void func_ov136_021c44fc(MailSysWork *work);
static BOOL func_ov136_021c4538(MailSysWork *work);
static BOOL func_ov136_021c4590(MailSysWork *work);
static BOOL func_ov136_021c45c0(MailSysWork *work);
static BOOL func_ov136_021c4700(MailSysWork *work);
static BOOL func_ov136_021c479c(MailSysWork *work);
void func_ov136_021c47c4(MailSysWork *work, u32 messageId);
static BOOL func_ov136_021c483c(MailSysWork *work);
void func_ov136_021c48fc(MailSysWork *work);
s32 func_ov136_021c4998(MailSysWork *work);
static BOOL func_ov136_021c49d0(MailSysWork *work);
static void func_ov136_021c4af8(void);
static void func_ov136_021c4b5c(TCB *tcb, void *data);

static const GameProcFunctions data_ov136_021c7750 = {
    func_ov136_021c41e0,
    func_ov136_021c427c,
    func_ov136_021c4290,
};

static const GameProcFunctions data_ov136_021c775c = {
    func_ov110_021bd484,
    func_ov110_021bd534,
    func_ov110_021bd5a0,
};

const GameProcFunctions MAILSYS_PROC_FUNCTIONS = {
    func_ov136_021c3e3c,
    func_ov136_021c3e9c,
    func_ov136_021c3f98,
};

static const MailSysPhaseFunc data_ov136_021c7774[] = {
    func_ov136_021c4590,
    func_ov136_021c479c,
    func_ov136_021c483c,
    func_ov136_021c49d0,
};

// The sentence, of sentence type 5, that a design gives each of its three sentences to start from, or 0xff
static const u8 data_ov136_021c77a0[MAIL_DESIGN_COUNT][MAIL_WORD_COUNT] = {
    { 0, 1, 2 },          { 3, 4, 5 },          { 6, 7, 8 },          { 9, 10, 20 },
    { 11, 12, 13 },       { 14, 15, 16 },       { 17, 18, 19 },       { 0xff, 0xff, 0xff },
    { 0xff, 0xff, 0xff }, { 0xff, 0xff, 0xff }, { 0xff, 0xff, 0xff }, { 0xff, 0xff, 0xff },
};

// The three sentences, the word, and the two buttons, the cancel one last
static const TouchRect data_ov136_021c7784[] = {
    { 0x18, 0x38, 0x18, 0xe8 }, { 0x40, 0x60, 0x18, 0xe8 }, { 0x68, 0x88, 0x18, 0xe8 },
    { 0x98, 0xa8, 0x40, 0x98 }, { 0xa8, 0xc0, 0xa8, 0xf8 }, { 0x90, 0xa8, 0xa8, 0xf8 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

// pokebw2's MailSys_ProcInit
BOOL func_ov136_021c3e3c(GameProc *proc, u32 *state, void *param, void *unused) {
    MailParam *mailParam = param;
    MailProcWork *work;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_MAIL, 0x1000);
    work = GFL_ProcInitSubsystem(proc, sizeof(MailProcWork), HEAPID_MAIL);
    MIi_CpuClearFast(0, work, sizeof(MailProcWork));
    work->heapId = HEAPID_MAIL;
    work->edit = func_ov136_021c409c(mailParam->mail, HEAPID_MAIL);
    if (mailParam->mode == MAIL_PARAM_MODE_WRITE) {
        work->edit->design = mailParam->design;
    }
    if (work->edit->design >= MAIL_DESIGN_COUNT) {
        work->edit->design = 0;
    }
    work->edit->mode = mailParam->mode;
    work->procMgr = CreateGameProcManager(HEAPID_MAIL);
    return TRUE;
}

// pokebw2's func_ov136_021c3e9c
BOOL func_ov136_021c3e9c(GameProc *proc, u32 *state, void *param, void *workPtr) {
    MailParam *mailParam = param;
    MailProcWork *work = workPtr;
    BOOL running = GFL_ProcMgrUpdate(work->procMgr);

    switch (*state) {
    case 0:
        work->edit->mode = mailParam->mode;
        QueueGameProc(work->procMgr, OVERLAY_NONE, &data_ov136_021c7750, work->edit);
        *state = 1;
        break;
    case 1:
        if (running == TRUE) {
            break;
        }
        switch (work->edit->mode) {
        case MAIL_EDIT_CANCEL:
            *state = 2;
            break;
        case MAIL_EDIT_DONE:
            *state = 2;
            break;
        default:
            *state = 3;
            break;
        }
        break;
    case 2:
        if (mailParam->mode == MAIL_PARAM_MODE_WRITE) {
            if (work->edit->mode == MAIL_EDIT_DONE) {
                func_ov136_021c414c(mailParam->mail, work->edit);
                RecordAddOne(getTrainerCardInfoBlkAddress(mailParam->save), RECORD_MAILS_WRITTEN);
                mailParam->written = TRUE;
            } else {
                mailParam->written = FALSE;
            }
        }
        return TRUE;
    case 3:
        GFL_OvlLoad(OVERLAY_ID(110));
        func_ov136_021c3fc8(work, mailParam);
        QueueGameProc(work->procMgr, OVERLAY_NONE, &data_ov136_021c775c, work->pmsiParam);
        *state = 4;
        break;
    case 4:
        if (running == TRUE) {
            break;
        }
        if (!func_0202411c(work->pmsiParam)) {
            if (work->edit->selected < MAIL_WORD_COUNT) {
                func_02024134(work->pmsiParam, &work->edit->words[work->edit->selected]);
            } else {
                work->edit->word = func_02024124(work->pmsiParam);
            }
        }
        func_020240fc(work->pmsiParam);
        GFL_OvlUnload(OVERLAY_ID(110));
        *state = 0;
        break;
    }
    return FALSE;
}

// pokebw2's MailSys_ProcExit
BOOL func_ov136_021c3f98(GameProc *proc, u32 *state, void *param, void *workPtr) {
    MailProcWork *work = workPtr;
    HeapID heapId;

    FreeGameProcManager(work->procMgr);
    func_ov136_021c4134(work->edit);
    heapId = work->heapId;
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapStatusValidate(heapId);
    GFL_HeapDelete(heapId);
    return TRUE;
}

// pokebw2's MailSys_StartPMSInput
static void func_ov136_021c3fc8(MailProcWork *work, MailParam *param) {
    if (work->edit->selected == MAIL_WORD_COUNT) {
        func_0202428c(&work->sentence);
        work->sentence.type = 6;
        work->sentence.id = 0;
        work->sentence.words[0] = work->edit->word;
        work->pmsiParam = func_02024044(PMSI_MODE_WORD, 0, NULL, TRUE, param->save, work->heapId);
        func_02024104(work->pmsiParam, work->edit->word);
    } else if (work->edit->hasPreset[work->edit->selected] == TRUE) {
        func_02024570(&work->sentence, &work->edit->words[work->edit->selected]);
        work->pmsiParam = func_02024044(PMSI_MODE_SENTENCE, 0, &work->sentence, TRUE, param->save, work->heapId);
    } else {
        work->pmsiParam = func_02024044(PMSI_MODE_SENTENCE, 0, NULL, TRUE, param->save, work->heapId);
        if (func_02024450(&work->edit->words[work->edit->selected])) {
            func_02024570(&work->sentence, &work->edit->words[work->edit->selected]);
        } else {
            func_020242a4(&work->sentence, 0);
        }
        func_02024108(work->pmsiParam, &work->sentence);
    }
}

// pokebw2's MailSys_CreateEdit
static MailEdit *func_ov136_021c409c(MailData *mail, u32 heapId) {
    MailEdit *edit = GFL_HeapAllocate(heapId, sizeof(MailEdit), FALSE, "mailsys.c", 0x147);
    u16 i;

    MIi_CpuClearFast(0, edit, sizeof(MailEdit));
    edit->mode = 0;
    edit->trainerId = func_02008f80(mail);
    edit->name = GFL_StrBufCreate(16, heapId);
    GFL_StrBufLoadString(edit->name, func_02008f84(mail));
    edit->design = func_02008fa4(mail);
    edit->unk09 = func_02008fb0(mail);
    edit->unk0A = func_02008fb8(mail);
    edit->word = func_02008fc0(mail);
    for (i = 0; i < MAIL_WORD_COUNT; i++) {
        func_02024570(&edit->words[i], func_02008fc8(mail, i));
    }
    return edit;
}

// pokebw2's MailSys_FreeEdit
static void func_ov136_021c4134(MailEdit *edit) {
    if (edit->name != NULL) {
        GFL_StrBufFree(edit->name);
    }
    GFL_HeapFree(edit);
}

// pokebw2's MailSys_ApplyEdit
static void func_ov136_021c414c(MailData *mail, MailEdit *edit) {
    u16 i;

    for (i = 0; i < MAIL_WORD_COUNT; i++) {
        func_02008fd8(mail, &edit->words[i], i);
    }
    func_02008fc4(mail, edit->word);
    func_02008fa8(mail, edit->design);
}

// pokebw2's MailSys_InitPresets
static void func_ov136_021c4184(MailSysWork *work, MailEdit *edit) {
    int i;

    for (i = 0; i < MAIL_WORD_COUNT; i++) {
        if (!func_02024450(&edit->words[i]) && data_ov136_021c77a0[edit->design][i] != 0xff) {
            func_020242c0(&edit->words[i], 5, data_ov136_021c77a0[edit->design][i]);
        }
        if (data_ov136_021c77a0[edit->design][i] == 0xff) {
            edit->hasPreset[i] = FALSE;
        } else {
            edit->hasPreset[i] = TRUE;
        }
    }
}

// pokebw2's MailSys_EditInit
static BOOL func_ov136_021c41e0(GameProc *proc, u32 *state, void *param, void *unused) {
    MailEdit *edit = param;
    MailSysWork *work;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_MAIL_SCREEN, 0x20000);
    work = GFL_ProcInitSubsystem(proc, sizeof(MailSysWork), HEAPID_MAIL_SCREEN);
    MIi_CpuClearFast(0, work, sizeof(MailSysWork));
    work->heapId = HEAPID_MAIL_SCREEN;
    work->edit = edit;
    work->phase = edit->mode;
    work->initialPhase = work->phase;
    work->cursor = edit->selected;
    work->savedCursor = edit->cursor;
    work->blinkCurrent = work->blinkTarget = work->cursor;
    edit->mode = MAIL_EDIT_CANCEL;
    work->textSpeed = func_02012ffc();
    func_ov136_021c4184(work, work->edit);
    work->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, work->heapId);
    work->tcbEx = GFL_TCBExMgrCreate(work->heapId, work->heapId, 32, 32);
    work->keyMode = TRUE;
    GFL_BGSysCreate(work->heapId);
    return TRUE;
}

// pokebw2's MailSys_EditMain
static BOOL func_ov136_021c427c(GameProc *proc, u32 *state, void *param, void *work) {
    if (func_ov136_021c42c8(work)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's MailSys_EditExit
static BOOL func_ov136_021c4290(GameProc *proc, u32 *state, void *param, void *workPtr) {
    MailSysWork *work = workPtr;
    u32 heapId;

    GFL_BGSysFree();
    GFL_TCBExMgrFree(work->tcbEx);
    GFL_FontFree(work->font);
    heapId = work->heapId;
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapStatusValidate(heapId);
    GFL_HeapDelete(heapId);
    return TRUE;
}

// pokebw2's MailSys_Update
static BOOL func_ov136_021c42c8(MailSysWork *work) {
    BOOL done = FALSE;

    switch (work->seq) {
    case MAIL_SEQ_INIT:
        func_ov136_021c4af8();
        func_0202223c(0, 0);
        func_0202223c(1, 0);
        func_02022224(0);
        func_02022224(1);
        work->seq++;
        break;
    case MAIL_SEQ_LOAD:
        if (func_ov136_021c4bfc(work)) {
            work->vblankTask = GFL_VBlankTCBAdd(func_ov136_021c4b5c, work, 16);
            GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
            killBrightnessEitherEngine(0);
            G2_SetBlendAlpha(GX_PLANEMASK_BG2, GX_PLANEMASK_BG3, 28, 4);
            func_0202173c(work->palFade, 5, 0xffff, -1, 16, 0, 0, GFL_VBlankGetTCBMgr());
            work->seq++;
        }
        break;
    case MAIL_SEQ_FADE_IN:
        if (func_02021ed8(work->palFade) == 0) {
            work->seq++;
        }
        func_0202dbc4(work->pmsDraw);
        break;
    case MAIL_SEQ_PHASE:
        if (data_ov136_021c7774[work->phase](work) == TRUE) {
            work->seq++;
            func_0202173c(work->palFade, 5, 0xffff, -1, 0, 16, 0, GFL_VBlankGetTCBMgr());
        }
        func_0202dbc4(work->pmsDraw);
        if (work->confirmButton != NULL) {
            func_02028178(work->confirmButton);
            func_02028178(work->cancelButton);
        }
        break;
    case MAIL_SEQ_FADE_OUT:
        if (func_02021ed8(work->palFade) == 0) {
            func_0202223c(0, 0);
            func_0202223c(1, 0);
            GFL_TCBRemove(work->vblankTask);
            func_ov136_021c4af8();
            work->seq++;
        }
        func_0202dbc4(work->pmsDraw);
        break;
    case MAIL_SEQ_UNLOAD:
        if (func_ov136_021c4d14(work)) {
            done = TRUE;
        }
        break;
    }
    if (work->clUnit != NULL) {
        func_0204a600();
    }
    if (work->tcbMgr != NULL) {
        GFL_TCBMgrUpdate(work->tcbMgr);
    }
    if (work->tcbEx != NULL) {
        GFL_TCBExMgrUpdate(work->tcbEx);
    }
    if (work->printQueue != NULL) {
        func_0201c4e4(work->printQueue);
    }
    if (work->pmsQueue != NULL) {
        func_0201c4e4(work->pmsQueue);
    }
    return done;
}

// Whether the mail can't be kept yet: the sentences that a design presets need all their words, and the others need
// one of them
// pokebw2's MailSys_IsIncomplete
static BOOL func_ov136_021c446c(MailSysWork *work) {
    int required = 1;
    int presetCount = 0;
    int freeFilled = 0;
    int presetComplete = 0;
    int i;

    for (i = 0; i < MAIL_WORD_COUNT; i++) {
        if (data_ov136_021c77a0[work->edit->design][i] != 0xff) {
            presetCount++;
        }
    }
    if (presetCount == MAIL_WORD_COUNT) {
        required = 0;
    }
    for (i = 0; i < MAIL_WORD_COUNT; i++) {
        if (data_ov136_021c77a0[work->edit->design][i] != 0xff) {
            if (func_02024464(&work->edit->words[i], 100)) {
                presetComplete++;
            }
        } else if (func_02024450(&work->edit->words[i])) {
            freeFilled++;
        }
    }
    if (freeFilled < required || presetComplete < presetCount) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's MailSys_ResetBlink
static void func_ov136_021c44fc(MailSysWork *work) {
    func_02021fd8(work->palFade, 0, work->blinkCurrent + 0x21, 1, 0, 0x7fff);
    work->blinkFalling = 0;
    work->blinkLevel = 0;
}

// pokebw2's MailSys_OnTouchMode
BOOL func_ov136_021c4528(MailSysWork *work) {
    func_ov136_021c44fc(work);
    return FALSE;
}

// pokebw2's MailSys_OnKeyMode
BOOL func_ov136_021c4534(MailSysWork *work) {
    return FALSE;
}

// Switches between the keys and the touch screen when the other one is used, and returns whether the keys took over
// pokebw2's MailSys_CheckInputModeSwitch
static BOOL func_ov136_021c4538(MailSysWork *work) {
    if (work->keyMode == FALSE) {
        if (func_02035c38()) {
            return FALSE;
        }
        if (GCTX_HIDGetHeldKeys() != 0) {
            if (work->onKeyMode != NULL) {
                work->onKeyMode(work);
            }
            work->keyMode = TRUE;
            return TRUE;
        }
    } else {
        if (GCTX_HIDGetHeldKeys() != 0) {
            return FALSE;
        }
        if (func_02035c38()) {
            if (work->onTouchMode != NULL) {
                work->onTouchMode(work);
            }
            work->keyMode = FALSE;
            return FALSE;
        }
    }
    return FALSE;
}

// pokebw2's MailSys_PhaseView
static BOOL func_ov136_021c4590(MailSysWork *work) {
    if (GCTX_HIDGetPressedKeys() != 0) {
        work->keyMode = TRUE;
    } else if (func_02035c60()) {
        work->keyMode = FALSE;
    } else {
        return FALSE;
    }
    GFL_SndSEPlay(SEQ_SE_DECIDE3);
    return TRUE;
}

// pokebw2's MailSys_PhaseKeys
// pokebw2's MailSys_PhaseKeys
static BOOL func_ov136_021c45c0(MailSysWork *work) {
    BOOL moved;

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        if (work->cursor == 4) {
            if (func_ov136_021c446c(work)) {
                GFL_SndSEPlay(SEQ_SE_CANCEL1);
                work->phase = MAIL_PHASE_NOTICE;
                return FALSE;
            }
            work->edit->mode = MAIL_EDIT_DONE;
            GFL_SndSEPlay(SEQ_SE_DECIDE3);
            work->edit->selected = 0;
            work->edit->cursor = 0;
            func_0202822c(work->confirmButton, TRUE);
            return TRUE;
        } else if (work->cursor == 5) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            work->phase = MAIL_PHASE_CANCEL;
            return FALSE;
        } else {
            work->edit->selected = work->cursor;
            work->edit->mode = work->edit->selected;
            work->edit->cursor = work->savedCursor;
            GFL_SndSEPlay(SEQ_SE_DECIDE2);
            return TRUE;
        }
    } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        work->phase = MAIL_PHASE_CANCEL;
        return FALSE;
    } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_START) {
        work->cursor = 4;
        work->savedCursor = 0;
        moved = TRUE;
    } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_DOWN) {
        work->cursor = (work->cursor + 1) % 6;
        moved = TRUE;
    } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_UP) {
        work->cursor = (work->cursor + 5) % 6;
        moved = TRUE;
    } else {
        return FALSE;
    }
    if (!moved) {
        return FALSE;
    }
    GFL_SndSEPlay(SEQ_SE_DECIDE1);
    if (work->cursor < 4) {
        func_02028218(work->confirmButton, FALSE);
        func_02028218(work->cancelButton, FALSE);
    } else if (work->cursor == 4) {
        func_02028218(work->confirmButton, TRUE);
        func_02028218(work->cancelButton, FALSE);
    } else if (work->cursor == 5) {
        func_02028218(work->confirmButton, FALSE);
        func_02028218(work->cancelButton, TRUE);
    }
    work->blinkTarget = work->cursor;
    return FALSE;
}

// pokebw2's MailSys_PhaseTouch
static BOOL func_ov136_021c4700(MailSysWork *work) {
    s32 touched = func_02035c08(data_ov136_021c7784);

    if (touched == TOUCH_RECT_NONE) {
        return FALSE;
    }
    if (touched == 4) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        work->phase = MAIL_PHASE_CANCEL;
        return FALSE;
    } else if (touched < 4) {
        work->edit->selected = touched;
        work->edit->mode = work->edit->selected;
        work->edit->cursor = work->savedCursor;
        GFL_SndSEPlay(SEQ_SE_DECIDE2);
        return TRUE;
    }
    if (func_ov136_021c446c(work)) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        work->phase = MAIL_PHASE_NOTICE;
        return FALSE;
    }
    work->edit->mode = MAIL_EDIT_DONE;
    GFL_SndSEPlay(SEQ_SE_DECIDE3);
    work->edit->selected = 0;
    work->edit->cursor = 0;
    func_0202822c(work->confirmButton, TRUE);
    func_02028218(work->cancelButton, FALSE);
    return TRUE;
}

// pokebw2's MailSys_PhaseWrite
static BOOL func_ov136_021c479c(MailSysWork *work) {
    if (func_ov136_021c4538(work)) {
        return FALSE;
    }
    if (work->keyMode == TRUE) {
        return func_ov136_021c45c0(work);
    }
    return func_ov136_021c4700(work);
}

// pokebw2's MailSys_ShowMessage
void func_ov136_021c47c4(MailSysWork *work, u32 messageId) {
    BmpWin_DrawFrame(work->windows[6], WINFRAME_TRANSFER_NONE, 0xd, 4);
    GFL_BitmapFill(BmpWin_GetBitmap(work->windows[6]), 0xff);
    GFL_MsgDataLoadStrbuf(work->msgData, messageId, work->msgBuf);
    work->printStream = func_0201cce4(work->windows[6], 0, 0, work->msgBuf, work->font, work->textSpeed,
                                      work->tcbEx, 4, work->heapId, 15);
    BmpWin_TransferNow(work->windows[6]);
}

// pokebw2's MailSys_PhaseNotice
// pokebw2's MailSys_PhaseNotice
static BOOL func_ov136_021c483c(MailSysWork *work) {
    switch (work->subSeq) {
    case 0:
        func_0202822c(work->confirmButton, TRUE);
        func_02028218(work->cancelButton, FALSE);
        work->subSeq = 1;
        break;
    case 1:
        if (func_02028230(work->confirmButton)) {
            func_020282a4(work->confirmButton);
            func_02028218(work->confirmButton, TRUE);
            func_ov136_021c47c4(work, BtlServerFlow3_Text_002);
            work->subSeq = 2;
        }
        break;
    case 2:
        if (func_0201ce30(work->printStream) == PRINT_STREAM_DONE) {
            if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) || func_02035c60()) {
                func_0201ce48(work->printStream);
                work->subSeq = 3;
            }
        }
        break;
    case 3: {
        BmpWin *window;

        func_0201f7a8(work->windows[6], WINFRAME_TRANSFER_NONE);
        window = work->windows[6];
        func_020456f8(window);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
        func_ov136_021c44fc(work);
        work->subSeq = 0;
        work->phase = work->initialPhase;
        break;
    }
    }
    return FALSE;
}


// pokebw2's MailSys_OpenConfirmMenu
void func_ov136_021c48fc(MailSysWork *work) {
    AppTaskMenuInit init;

    work->menuItems[0].str = GFL_MsgDataLoadStrbufNew(work->msgData, BtlServerFlow3_Text_004);
    work->menuItems[0].color = PRINT_COLOR(14, 15, 3);
    work->menuItems[0].type = 0;
    work->menuItems[1].str = GFL_MsgDataLoadStrbufNew(work->msgData, BtlServerFlow3_Text_005);
    work->menuItems[1].color = PRINT_COLOR(14, 15, 3);
    work->menuItems[1].type = 0;
    init.heapId = work->heapId;
    init.itemCount = 2;
    init.items = work->menuItems;
    init.posType = APP_TASKMENU_POS_TOP_LEFT;
    init.x = 23;
    init.y = 6;
    init.width = 8;
    init.height = 3;
    work->menu = func_02027778(&init, work->menuRes);
    func_02021fd8(work->palFade, 0, 0, 0x30, 10, 0);
    func_0202843c(work->confirmButton);
    func_0202843c(work->cancelButton);
}

// The item picked in the confirm menu once it has flashed, or -1
// pokebw2's MailSys_GetMenuResult
s32 func_ov136_021c4998(MailSysWork *work) {
    s32 result = -1;

    if (func_020279e8(work->menu)) {
        result = func_02027a04(work->menu);
        func_02027858(work->menu);
        GFL_StrBufFree(work->menuItems[0].str);
        GFL_StrBufFree(work->menuItems[1].str);
    }
    return result;
}

// pokebw2's MailSys_PhaseCancel
// pokebw2's MailSys_PhaseCancel
static BOOL func_ov136_021c49d0(MailSysWork *work) {
    switch (work->subSeq) {
    case 0:
        func_02028218(work->confirmButton, FALSE);
        func_0202822c(work->cancelButton, TRUE);
        break;
    case 1:
        if (func_02028230(work->cancelButton)) {
            func_020282a4(work->cancelButton);
            func_02028218(work->cancelButton, TRUE);
            work->confirming = TRUE;
            func_ov136_021c47c4(work, BtlServerFlow3_Text_003);
            func_ov136_021c44fc(work);
            work->blinkCurrent = work->blinkTarget;
            break;
        }
        return FALSE;
    case 2:
        if (func_0201ce30(work->printStream) == PRINT_STREAM_DONE) {
            func_0201ce48(work->printStream);
            func_ov136_021c48fc(work);
            work->subSeq = 3;
        }
        return FALSE;
    case 3: {
        s32 result;
        BmpWin *window;

        func_02027974(work->menu);
        result = func_ov136_021c4998(work);
        if (result < 0) {
            return FALSE;
        }
        func_0201f7a8(work->windows[6], WINFRAME_TRANSFER_NONE);
        window = work->windows[6];
        func_020456f8(window);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
        work->subSeq = 0;
        if (result == 0) {
            work->edit->mode = MAIL_EDIT_CANCEL;
            work->phase = work->initialPhase;
            return TRUE;
        }
        work->phase = work->initialPhase;
        work->confirming = FALSE;
        func_02021fd8(work->palFade, 0, 0, 0x30, 0, 0);
        func_02028454(work->confirmButton);
        func_02028454(work->cancelButton);
        return FALSE;
    }
    }
    work->subSeq++;
    return FALSE;
}


// pokebw2's MailSys_DisableAllBGs
static void func_ov136_021c4af8(void) {
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, FALSE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG1, FALSE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG2, FALSE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG3, FALSE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, FALSE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG1, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG2, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG3, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, FALSE);
}

// pokebw2's MailSys_VBlank
static void func_ov136_021c4b5c(TCB *tcb, void *data) {
    MailSysWork *work = data;

    if (work->palFade != NULL) {
        func_02021d50(work->palFade);
    }
    func_0204a648();
    GFL_BGSysUpdate();
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

// pokebw2's MailSys_UpdateBlink
void func_ov136_021c4b88(TCB *tcb, void *data) {
    MailSysWork *work = data;

    if (work->confirming == 0 && work->keyMode != 0) {
        if (work->blinkCurrent != work->blinkTarget) {
            func_ov136_021c44fc(work);
            work->blinkCurrent = work->blinkTarget;
        }
        func_02021fd8(work->palFade, 0, work->blinkTarget + 0x21, 1, work->blinkLevel >> 1, 0x7fff);
        if (work->blinkFalling) {
            if (work->blinkLevel-- == 1) {
                work->blinkFalling ^= 1;
            }
        } else {
            if (work->blinkLevel++ == 24) {
                work->blinkFalling ^= 1;
            }
        }
    }
}
