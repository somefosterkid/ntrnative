// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov185/pms_input.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/pms_input.h"
#include "types.h"
#include "app/pms_input_data.h"
#include "app/pms_input_view.h"
#include "app/pmsi_initial_data.h"
#include "app/pmsi_search.h"
#include "app/ui/ui_scene.h"
#include "constants/sound.h"
#include "gfl/button_man.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "system/pms.h"
#include "system/pms_data.h"
#include "system/pmsi_param.h"
#include "system/pms_word.h"

// The phrase input: the player writes one word, two words or a sentence, choosing each word from a group of words or
// from the words of an initial, in a window of words, with the keys or the touch screen. The names are ours, guessed


// Where the input is driven from
#define KEY_MODE_KEY 0
#define KEY_MODE_TOUCH 1

// What the cursor on the buttons is on
enum {
    BUTTON_POS_DECIDE,
    BUTTON_POS_CANCEL,
};

// The categories' cursor's places besides a category: the search, erase and back buttons, and no place
#define CATEGORY_POS_SEARCH 0xfc
#define CATEGORY_POS_ERASE 0xfd
#define CATEGORY_POS_BACK 0xfe
#define CATEGORY_POS_NONE 0xff

// The word groups, and the sentence types the input offers
#define GROUP_COUNT 12
#define SENTENCE_TYPE_COUNT 5

// What a key or a touch on the word window does
enum {
    WORDWIN_RESULT_NONE,
    WORDWIN_RESULT_CURSOR,
    WORDWIN_RESULT_SCROLL,
    WORDWIN_RESULT_SCROLL_CURSOR,
    WORDWIN_RESULT_IGNORE,
    WORDWIN_RESULT_DECIDE,
};

// How many words the word window shows, two to a row
#define WORDWIN_VISIBLE_WORDS 14
#define WORDWIN_ROWS 6

typedef BOOL (*PMSInputMainProc)(PMSInputWork *wk, u32 *seq);
typedef void (*PMSInputSubProc)(PMSInputWork *wk, u32 *seq);
typedef void (*PMSInputKTChangeFunc)(PMSInputWork *wk, u32 *seq);
typedef BOOL (*PMSInputCursorFunc)(PMSInputWork *wk);

typedef struct {
    s16 type;
    s8 id;
    s8 count;
} PMSInputSentenceWork;

typedef struct {
    u16 top;
    u16 scrollMax;
    u16 wordMax;
    u8 x;
    u8 y;
    int scrollVector;
    u16 onBack;
    u16 selected;
} PMSInputWordWin;

typedef struct {
    u8 pos;
    u8 max;
} PMSInputMenu;

struct PMSInputWork {
    PMSIParam *param;
    u32 inputMode;
    PMSData sentence;
    u16 words[PMS_SENTENCE_WORD_MAX];
    PMSInputView *vwk;
    PMSInputData *dwk;
    PMSISearch *search;
    int mainSeq;
    PMSInputMainProc mainProc;
    PMSInputMainProc nextProc;
    int subSeq;
    PMSInputSubProc subProc;
    u16 keyTrg;
    u16 keyCont;
    u16 keyRepeat;
    // The mode button's state: 0 while it is touched
    u32 modeButton;
    u32 unk40;
    ButtonMan *bmn;
    u16 buttonPos;
    u16 editPos;
    u16 categoryPos;
    u16 categoryPosPrev;
    u16 categoryPosSaved;
    PMSInputWordWin wordWin;
    PMSInputSentenceWork sentenceWk;
    u8 sentenceEditPosMax;
    u8 categoryMode;
    u8 scrollBarHeld;
    PMSInputMenu menu;
    int keyMode;
    PMSInputKTChangeFunc ktChangeFunc;
    void *tcbMem;
    TCBManager *tcbMgr;
};

void func_ov110_021bd578(u32 button, u32 event, void *work);
PMSInputWork *func_ov110_021bd5c4(GameProc *proc, PMSIParam *param);
void func_ov110_021bd690(PMSInputSentenceWork *swk, const PMSData *sentence);
void func_ov110_021bd6b4(PMSInputSentenceWork *swk, PMSData *sentence);
void func_ov110_021bd6f8(PMSInputSentenceWork *swk, PMSData *sentence);
void func_ov110_021bd740(PMSInputSentenceWork *swk, PMSData *sentence, int type);
void func_ov110_021bd764(PMSInputWork *wk, GameProc *proc);
void func_ov110_021bd7a0(PMSInputWork *wk, PMSInputMainProc proc);
BOOL func_ov110_021bd808(PMSInputWork *wk, u32 *seq);
void func_ov110_021bd868(PMSInputWork *wk);
BOOL func_ov110_021bd894(PMSInputWork *wk, u32 *seq);
void func_ov110_021bd8a8(PMSInputWork *wk, u32 *seq);
void func_ov110_021bd8b8(PMSInputWork *wk, u32 *seq);
void func_ov110_021bd8c8(PMSInputWork *wk, u32 *seq);
BOOL func_ov110_021bd8d8(PMSInputWork *wk, u32 *seq);
s32 func_ov110_021bda84(PMSInputWork *wk);
s32 func_ov110_021bda94(PMSInputWork *wk);
s32 func_ov110_021bdaa4(PMSInputWork *wk);
s32 func_ov110_021bdb50(PMSInputWork *wk);
BOOL func_ov110_021bdb98(PMSInputWork *wk, u32 *seq);
BOOL func_ov110_021bdc70(PMSInputWork *wk, u32 *seq);
BOOL func_ov110_021bdcb4(PMSInputWork *wk, u32 *seq);
BOOL func_ov110_021bdeac(PMSInputWork *wk, u32 *seq);
BOOL func_ov110_021bdef0(PMSInputWork *wk, u32 *seq);
BOOL func_ov110_021be1b8(PMSInputWork *wk, u32 *seq);
BOOL func_ov110_021be374(PMSInputWork *wk, u32 *seq);
BOOL func_ov110_021be3b8(PMSInputWork *wk, u32 *seq);
BOOL func_ov110_021be4c0(PMSInputWork *wk, u32 *seq);
void func_ov110_021be580(PMSInputWork *wk, u32 *seq);
int func_ov110_021be7b8(PMSInputWork *wk);
s32 func_ov110_021be7f0(PMSInputWork *wk);
int func_ov110_021be800(PMSInputWork *wk);
s32 func_ov110_021be890(PMSInputWork *wk);
int func_ov110_021be8b4(PMSInputWork *wk);
void func_ov110_021be934(PMSInputWork *wk, u32 *seq);
void func_ov110_021beadc(PMSInputWork *wk, u32 *seq);
BOOL func_ov110_021beb44(PMSInputWork *wk);
BOOL func_ov110_021beb60(PMSInputWork *wk);
BOOL func_ov110_021beb78(PMSInputWork *wk);
BOOL func_ov110_021bec20(PMSInputWork *wk);
void func_ov110_021bed84(PMSInputWordWin *ww, PMSInputWork *wk);
static u32 func_ov110_021bedc4(const PMSInputWordWin *ww);
int func_ov110_021bedd0(const PMSInputWordWin *ww);
static int func_ov110_021bede0(const PMSInputWordWin *ww);
static u16 func_ov110_021bede4(const PMSInputWordWin *ww);
static u16 func_ov110_021bede8(const PMSInputWordWin *ww);
BOOL func_ov110_021bedec(PMSInputWork *wk, u32 *seq);
void func_ov110_021bee60(PMSInputWork *wk, u32 *seq);
int func_ov110_021bef4c(PMSInputWork *wk);
BOOL func_ov110_021bf08c(PMSInputWork *wk);
void func_ov110_021bf0c0(PMSInputWork *wk, u32 *seq);
void func_ov110_021bf1c4(PMSInputWork *wk, u32 *seq);
int func_ov110_021bf208(PMSInputWordWin *ww, u16 key);
int func_ov110_021bf338(PMSInputWordWin *ww);
int func_ov110_021bf360(PMSInputWordWin *ww);
int func_ov110_021bf3a0(PMSInputView *vwk, PMSInputWordWin *ww);
int func_ov110_021bf3d4(PMSInputView *vwk, PMSInputWordWin *ww);
int func_ov110_021bf408(PMSInputView *vwk, PMSInputWordWin *ww);
BOOL func_ov110_021bf444(PMSInputWork *wk);
BOOL func_ov110_021bf4c4(PMSInputWork *wk, u32 *seq);
void func_ov110_021bf4f8(PMSInputWork *wk, PMSInputSubProc proc);
void func_ov110_021bf500(PMSInputWork *wk);
void func_ov110_021bf508(PMSInputWork *wk, u32 *seq);
void func_ov110_021bf53c(PMSInputWork *wk, u32 *seq);
void func_ov110_021bf608(PMSInputWork *wk, u32 *seq);
static BOOL func_ov110_021bf680(PMSInputWork *wk);
void func_ov110_021bf6cc(PMSInputMenu *menu, u8 max, u8 pos);

const GameProcFunctions PMS_INPUT_PROC_FUNCTIONS = {
    func_ov110_021bd484,
    func_ov110_021bd534,
    func_ov110_021bd5a0,
};

// pokebw2's PMSInput_Main

BOOL func_ov110_021bd484(GameProc *proc, u32 *state, void *param, void *work) {
    PMSInputWork *wk;

    switch (*state) {
    case 0:
        GFL_OvlLoad(OVERLAY_ID(67));
        GFL_HeapCreateChild(HEAPID_USER, HEAPID_PMS_INPUT_SYS, 0x18000);
        GFL_HeapCreateChild(HEAPID_USER, HEAPID_PMS_INPUT, 0x32000);
        wk = func_ov110_021bd5c4(proc, param);
        func_ov110_021b9bec(wk->vwk, 0);
        (*state)++;
        break;
    case 1:
        wk = work;
        if (func_ov110_021b9c74(wk->vwk)) {
            if (wk->inputMode == PMSI_MODE_SENTENCE) {
                wk->sentenceEditPosMax = func_ov110_021babe8(wk->vwk);
            } else {
                wk->sentenceEditPosMax = 0;
            }
            func_ov110_021b9bec(wk->vwk, 4);
            (*state)++;
        }
        break;
    case 2:
        wk = work;
        if (func_ov110_021b9c74(wk->vwk)) {
            func_ov110_021bd7a0(wk, func_ov110_021bd894);
            func_0203d34c(TRUE, HEAPID_PMS_INPUT);
            return TRUE;
        }
        break;
    }
    GFL_TCBMgrUpdate(wk->tcbMgr);
    return FALSE;
}

BOOL func_ov110_021bd534(GameProc *proc, u32 *state, void *param, void *work) {
    PMSInputWork *wk = work;
    BOOL ret;

    wk->keyTrg = GCTX_HIDGetPressedKeys();
    wk->keyCont = GCTX_HIDGetHeldKeys();
    wk->keyRepeat = GCTX_HIDGetTypedKeys();
    func_02051c88(wk->bmn);
    if (wk->subProc != NULL) {
        wk->subProc(wk, &wk->subSeq);
        ret = FALSE;
    } else {
        ret = wk->mainProc(wk, &wk->mainSeq);
    }
    GFL_TCBMgrUpdate(wk->tcbMgr);
    return ret;
}

// pokebw2's PMSInput_ButtonCallback
void func_ov110_021bd578(u32 button, u32 event, void *work) {
    PMSInputWork *wk = work;

    switch (event) {
    case BMN_EVENT_TOUCH:
        wk->modeButton = button;
        break;
    case BMN_EVENT_RELEASE:
    case BMN_EVENT_SLIDEOUT:
        wk->modeButton = 1;
        break;
    case BMN_EVENT_HOLD:
        break;
    default:
        wk->modeButton = 1;
        break;
    }
}

BOOL func_ov110_021bd5a0(GameProc *proc, u32 *state, void *param, void *work) {
    func_ov110_021bd764(work, proc);
    GFL_HeapDelete(0x29);
    GFL_HeapDelete(HEAPID_PMS_INPUT);
    GFL_OvlUnload(OVERLAY_ID(67));
    return TRUE;
}

PMSInputWork *func_ov110_021bd5c4(GameProc *proc, PMSIParam *param) {
    static const TouchRect sModeButtonRects[] = {
        { 168, 192, 0, 48 },
        { TOUCH_RECT_END, 0, 0, 0 },
    };

    PMSInputWork *wk = GFL_ProcInitSubsystem(proc, sizeof(PMSInputWork), HEAPID_PMS_INPUT_SYS);

    wk->param = param;
    wk->inputMode = func_02024144(wk->param);
    wk->keyMode = func_0202421c(wk->param);
    func_02024168(wk->param, wk->words, &wk->sentence);
    if (wk->inputMode == PMSI_MODE_SENTENCE) {
        func_ov110_021bd690(&wk->sentenceWk, &wk->sentence);
    }
    wk->tcbMem = GFL_HeapAllocate(HEAPID_PMS_INPUT_SYS, GFL_TCBMgrCalcAllocSize(5), FALSE, "pms_input.c", 579);
    wk->tcbMgr = GFL_TCBMgrCreate(5, wk->tcbMem);
    wk->dwk = func_ov110_021b95a0(HEAPID_PMS_INPUT_SYS, wk->param);
    wk->vwk = func_ov110_021b9a50(wk, wk->dwk);
    wk->bmn = func_02051b84(sModeButtonRects, func_ov110_021bd578, wk, HEAPID_PMS_INPUT_SYS);
    wk->categoryMode = 0;
    wk->subProc = NULL;
    wk->subSeq = 0;
    wk->editPos = 0;
    wk->buttonPos = 0;
    wk->modeButton = 1;
    wk->search = func_ov110_021baedc(wk, wk->dwk, HEAPID_PMS_INPUT_SYS);
    func_ov110_021bf4f8(wk, func_ov110_021bf508);
    return wk;
}



// pokebw2's PMSInput_SetupSentenceWork
void func_ov110_021bd690(PMSInputSentenceWork *swk, const PMSData *sentence) {
    swk->type = func_0202452c(sentence);
    swk->count = func_02024584(swk->type);
    swk->id = func_02024530(sentence);
}

// pokebw2's PMSInput_SentenceIncrement
void func_ov110_021bd6b4(PMSInputSentenceWork *swk, PMSData *sentence) {
    swk->id++;
    if (swk->id >= swk->count) {
        swk->id = 0;
        swk->type++;
        if (swk->type >= SENTENCE_TYPE_COUNT) {
            swk->type = 0;
        }
        swk->count = func_02024584(swk->type);
    }
    func_02024598(sentence, swk->type, swk->id);
}

// pokebw2's PMSInput_SentenceDecrement
void func_ov110_021bd6f8(PMSInputSentenceWork *swk, PMSData *sentence) {
    swk->id--;
    if (swk->id < 0) {
        swk->type--;
        if (swk->type < 0) {
            swk->type = SENTENCE_TYPE_COUNT - 1;
        }
        swk->count = func_02024584(swk->type);
        swk->id = swk->count - 1;
    }
    func_02024598(sentence, swk->type, swk->id);
}

// pokebw2's PMSInput_SentenceSetDirect
void func_ov110_021bd740(PMSInputSentenceWork *swk, PMSData *sentence, int type) {
    swk->id = 0;
    swk->type = type;
    swk->count = func_02024584(swk->type);
    func_02024598(sentence, swk->type, swk->id);
}

// pokebw2's PMSInput_DestructWork
void func_ov110_021bd764(PMSInputWork *wk, GameProc *proc) {
    func_02024224(wk->param, wk->keyMode);
    func_02051c6c(wk->bmn);
    func_ov110_021b9b20(wk->vwk);
    func_02030da4(wk->tcbMgr);
    GFL_HeapFree(wk->tcbMem);
    func_ov110_021b95f4(wk->dwk);
    func_ov110_021baf4c(wk->search);
    GFL_ProcReleaseSubsystem(proc);
}

// pokebw2's PMSInput_ChangeMainProc
void func_ov110_021bd7a0(PMSInputWork *wk, PMSInputMainProc proc) {
    wk->mainProc = proc;
    wk->mainSeq = 0;
    if (proc == func_ov110_021bd894) {
        func_ov110_021bae24(wk->vwk, wk, wk->dwk);
        wk->ktChangeFunc = func_ov110_021bd8a8;
    } else if (proc == func_ov110_021be4c0) {
        func_ov110_021bae94(wk->vwk, wk, wk->dwk);
        wk->ktChangeFunc = func_ov110_021bd8b8;
    } else if (proc == func_ov110_021bedec) {
        func_ov110_021baed0(wk->vwk, wk, wk->dwk);
        wk->ktChangeFunc = func_ov110_021bd8c8;
    } else {
        wk->ktChangeFunc = NULL;
    }
}

// pokebw2's PMSInput_KeyStatusChange
BOOL func_ov110_021bd808(PMSInputWork *wk, u32 *seq) {
    if (wk->keyMode == KEY_MODE_TOUCH) {
        if (func_02035c38()) {
            return FALSE;
        }
        if (GCTX_HIDGetHeldKeys()) {
            wk->keyMode = KEY_MODE_KEY;
            if (wk->ktChangeFunc) {
                wk->ktChangeFunc(wk, seq);
            }
            return TRUE;
        }
    } else {
        if (GCTX_HIDGetHeldKeys()) {
            return FALSE;
        }
        if (func_02035c38()) {
            wk->keyMode = KEY_MODE_TOUCH;
            if (wk->ktChangeFunc) {
                wk->ktChangeFunc(wk, seq);
            }
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's PMSInput_StartWordWin
void func_ov110_021bd868(PMSInputWork *wk) {
    GFL_SndSEPlay(SEQ_SE_DECIDE1);
    func_ov110_021bed84(&wk->wordWin, wk);
    wk->nextProc = func_ov110_021bedec;
    func_ov110_021b9bec(wk->vwk, PMSIV_CMD_CATEGORY_TO_WORDWIN);
}

// pokebw2's PMSInput_MainProcEditArea
BOOL func_ov110_021bd894(PMSInputWork *wk, u32 *seq) {
    static const PMSInputMainProc data_ov110_021c3920[] = {
        func_ov110_021bdc70,
        func_ov110_021bdeac,
        func_ov110_021be374,
    };

    return data_ov110_021c3920[wk->inputMode](wk, seq);
}

// pokebw2's PMSInput_EditAreaKTChange
void func_ov110_021bd8a8(PMSInputWork *wk, u32 *seq) {
    func_ov110_021bae24(wk->vwk, wk, wk->dwk);
}

// pokebw2's PMSInput_CategoryKTChange
void func_ov110_021bd8b8(PMSInputWork *wk, u32 *seq) {
    func_ov110_021bae94(wk->vwk, wk, wk->dwk);
}

// pokebw2's PMSInput_WordWinKTChange
void func_ov110_021bd8c8(PMSInputWork *wk, u32 *seq) {
    func_ov110_021baed0(wk->vwk, wk, wk->dwk);
}

// pokebw2's PMSInput_WordKey
BOOL func_ov110_021bd8d8(PMSInputWork *wk, u32 *seq) {
    switch (*seq) {
    case 0:
        if (!func_ov110_021b9c74(wk->vwk)) {
            break;
        }
        if (wk->keyTrg & PAD_KEY_UP) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            wk->buttonPos = BUTTON_POS_CANCEL;
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_EDITAREA_TO_BUTTON);
            *seq = 1;
        } else if (wk->keyTrg & (PAD_KEY_DOWN | PAD_BUTTON_START)) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            wk->buttonPos = BUTTON_POS_DECIDE;
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_EDITAREA_TO_BUTTON);
            *seq = 1;
        } else if (wk->keyTrg & PAD_BUTTON_B) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov110_021bf4f8(wk, func_ov110_021bf608);
        } else if (wk->keyTrg & PAD_BUTTON_A) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov110_021bf82c(wk);
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_EDITAREA_TO_CATEGORY);
            *seq = 5;
        }
        break;
    case 1:
        if (!func_ov110_021b9c74(wk->vwk)) {
            break;
        }
        if (wk->keyTrg & PAD_KEY_UP) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            if (wk->buttonPos != BUTTON_POS_DECIDE) {
                wk->buttonPos ^= 1;
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_BUTTON_CURSOR);
            } else {
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_BUTTON_TO_EDITAREA_SELECT);
                *seq = 0;
            }
        } else if (wk->keyTrg & PAD_KEY_DOWN) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            if (wk->buttonPos != BUTTON_POS_DECIDE) {
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_BUTTON_TO_EDITAREA_SELECT);
                *seq = 0;
            } else {
                wk->buttonPos ^= 1;
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_BUTTON_CURSOR);
            }
        } else if (wk->keyTrg & PAD_BUTTON_START) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            wk->buttonPos = BUTTON_POS_DECIDE;
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_BUTTON_CURSOR);
        } else if (wk->keyTrg & PAD_BUTTON_A) {
            if (wk->buttonPos == BUTTON_POS_DECIDE) {
                if (func_ov110_021bf680(wk)) {
                    GFL_SndSEPlay(SEQ_SE_DECIDE1);
                } else {
                    GFL_SndSEPlay(SEQ_SE_BEEP);
                }
                *seq = 2;
            } else {
                GFL_SndSEPlay(SEQ_SE_CANCEL1);
                *seq = 3;
            }
        } else if (wk->keyTrg & PAD_BUTTON_B) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            *seq = 3;
        }
        break;
    case 2:
        *seq = 4;
        func_ov110_021bf4f8(wk, func_ov110_021bf53c);
        break;
    case 3:
        *seq = 4;
        func_ov110_021bf4f8(wk, func_ov110_021bf608);
        break;
    case 4:
        *seq = 1;
        break;
    case 6:
        if (func_ov110_021b9c74(wk->vwk)) {
            func_ov110_021bd7a0(wk, func_ov110_021be3b8);
        }
        break;
    case 5:
        if (func_ov110_021b9c74(wk->vwk)) {
            func_ov110_021bd7a0(wk, func_ov110_021be4c0);
        }
        break;
    }
    return FALSE;
}

// pokebw2's PMSInput_WordTouchSingle
s32 func_ov110_021bda84(PMSInputWork *wk) {
    // The edit area's OK and quit buttons, and its words
    static const TouchRect data_ov110_021c3938[] = {
#include "asset/pms_input_data_ov110_021c3938.inc"
};

    return func_02035c08(data_ov110_021c3938);
}

// pokebw2's PMSInput_WordTouchDouble
s32 func_ov110_021bda94(PMSInputWork *wk) {
    static const TouchRect data_ov110_021c3948[] = {
#include "asset/pms_input_data_ov110_021c3948.inc"
};

    return func_02035c08(data_ov110_021c3948);
}

// pokebw2's PMSInput_SentenceTouchArea
s32 func_ov110_021bdaa4(PMSInputWork *wk) {
    s32 hit;
    int count, i;
    u32 x, y;
    TouchRect rects[2] = {
        { TOUCH_RECT_END, 0, 0, 0 },
        { TOUCH_RECT_END, 0, 0, 0 },
    };
    // The OK and quit buttons, the arrows that change the sentence on each side, and the sentence's arrows on the upper
    // screen
    static const TouchRect data_ov110_021c395c[] = {
        { 144, 168, 184, 256 }, { 168, 192, 184, 256 }, { 168, 192, 0, 24 },         { 168, 192, 144, 168 },
        { 8, 39, 0, 19 },       { 8, 39, 236, 255 },    { TOUCH_RECT_END, 0, 0, 0 },
    };

    if (!func_02035c60()) {
        return -1;
    }
    hit = func_02035c08(data_ov110_021c395c);
    if (hit != -1) {
        if (func_02024158(wk->param)) {
            if (hit == 2 || hit == 3 || hit == 4 || hit == 5) {
                hit = -1;
            }
            return hit;
        }
        return hit;
    }
    count = func_ov110_021babe8(wk->vwk);
    func_02035d20(&x, &y);
    for (i = 0; i < count; i++) {
        func_ov110_021bac00(wk->vwk, rects, i);
        if (func_02035d40(rects, x, y) != -1) {
            return i + 6;
        }
    }
    return -1;
}

// pokebw2's PMSInput_SentenceTouchType
s32 func_ov110_021bdb50(PMSInputWork *wk) {
    u32 x, y;
    s32 type;

    if (func_02024158(wk->param)) {
        return -1;
    }
    if (!func_02035d20(&x, &y)) {
        return -1;
    }
    if (y <= 168 || (type = x / 24 - 1) < 0 || type >= SENTENCE_TYPE_COUNT) {
        type = -1;
    }
    return type;
}

// pokebw2's PMSInput_WordTouch
BOOL func_ov110_021bdb98(PMSInputWork *wk, u32 *seq) {
    s32 hit;

    switch (*seq) {
    case 0:
    case 1:
        if (wk->inputMode == PMSI_MODE_WORD) {
            hit = func_ov110_021bda84(wk);
        } else {
            hit = func_ov110_021bda94(wk);
        }
        switch (hit) {
        case 0:
            if (func_ov110_021bf680(wk)) {
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
            } else {
                GFL_SndSEPlay(SEQ_SE_BEEP);
            }
            *seq = 2;
            break;
        case 1:
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            *seq = 3;
            break;
        case 2:
        case 3:
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov110_021bf82c(wk);
            wk->editPos = hit - 2;
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_EDITAREA_TO_CATEGORY);
            *seq = 5;
            break;
        }
        break;
    case 2:
        *seq = 4;
        func_ov110_021bf4f8(wk, func_ov110_021bf53c);
        break;
    case 3:
        *seq = 4;
        func_ov110_021bf4f8(wk, func_ov110_021bf608);
        break;
    case 4:
        *seq = 0;
        break;
    case 5:
        if (func_ov110_021b9c74(wk->vwk)) {
            func_ov110_021bd7a0(wk, func_ov110_021be4c0);
        }
        break;
    }
    return FALSE;
}

// pokebw2's PMSInput_MainProcWord
BOOL func_ov110_021bdc70(PMSInputWork *wk, u32 *seq) {
    if (*seq <= 1 && func_ov110_021bd808(wk, seq) && wk->keyMode == KEY_MODE_KEY) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return FALSE;
    }
    if (wk->keyMode == KEY_MODE_KEY) {
        return func_ov110_021bd8d8(wk, seq);
    }
    return func_ov110_021bdb98(wk, seq);
}

// pokebw2's PMSInput_DoubleWordKey
BOOL func_ov110_021bdcb4(PMSInputWork *wk, u32 *seq) {
    switch (*seq) {
    case 0:
        if (!func_ov110_021b9c74(wk->vwk)) {
            break;
        }
        if (wk->keyTrg & PAD_KEY_UP) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            wk->buttonPos = BUTTON_POS_CANCEL;
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_EDITAREA_TO_BUTTON);
            *seq = 1;
        } else if (wk->keyTrg & (PAD_KEY_DOWN | PAD_BUTTON_START)) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            wk->buttonPos = BUTTON_POS_DECIDE;
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_EDITAREA_TO_BUTTON);
            *seq = 1;
        } else if (wk->keyTrg & PAD_KEY_LEFT) {
            if (wk->editPos != 0) {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
                wk->editPos = 0;
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_EDITAREA_CURSOR);
            }
        } else if (wk->keyTrg & PAD_KEY_RIGHT) {
            if (wk->editPos == 0) {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
                wk->editPos = 1;
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_EDITAREA_CURSOR);
            }
        } else if (wk->keyTrg & PAD_BUTTON_B) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov110_021bf4f8(wk, func_ov110_021bf608);
        } else if (wk->keyTrg & PAD_BUTTON_A) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov110_021bf82c(wk);
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_EDITAREA_TO_CATEGORY);
            *seq = 5;
        }
        break;
    case 1:
        if (!func_ov110_021b9c74(wk->vwk)) {
            break;
        }
        if (wk->keyTrg & PAD_KEY_UP) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            if (wk->buttonPos != BUTTON_POS_DECIDE) {
                wk->buttonPos ^= 1;
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_BUTTON_CURSOR);
            } else {
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_BUTTON_TO_EDITAREA_SELECT);
                *seq = 0;
            }
        } else if (wk->keyTrg & PAD_KEY_DOWN) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            if (wk->buttonPos != BUTTON_POS_DECIDE) {
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_BUTTON_TO_EDITAREA_SELECT);
                *seq = 0;
            } else {
                wk->buttonPos ^= 1;
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_BUTTON_CURSOR);
            }
        } else if (wk->keyTrg & PAD_BUTTON_START) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            wk->buttonPos = BUTTON_POS_DECIDE;
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_BUTTON_CURSOR);
        } else if (wk->keyTrg & PAD_BUTTON_A) {
            if (wk->buttonPos == BUTTON_POS_DECIDE) {
                if (func_ov110_021bf680(wk)) {
                    GFL_SndSEPlay(SEQ_SE_DECIDE1);
                } else {
                    GFL_SndSEPlay(SEQ_SE_BEEP);
                }
                *seq = 2;
            } else {
                GFL_SndSEPlay(SEQ_SE_CANCEL1);
                *seq = 3;
            }
        } else if (wk->keyTrg & PAD_BUTTON_B) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            *seq = 3;
        }
        break;
    case 2:
        *seq = 4;
        func_ov110_021bf4f8(wk, func_ov110_021bf53c);
        break;
    case 3:
        *seq = 4;
        func_ov110_021bf4f8(wk, func_ov110_021bf608);
        break;
    case 4:
        *seq = 1;
        break;
    case 6:
        if (func_ov110_021b9c74(wk->vwk)) {
            func_ov110_021bd7a0(wk, func_ov110_021be3b8);
        }
        break;
    case 5:
        if (func_ov110_021b9c74(wk->vwk)) {
            func_ov110_021bd7a0(wk, func_ov110_021be4c0);
        }
        break;
    }
    return FALSE;
}

// pokebw2's PMSInput_MainProcDoubleWord
BOOL func_ov110_021bdeac(PMSInputWork *wk, u32 *seq) {
    if (*seq <= 1 && func_ov110_021bd808(wk, seq) && wk->keyMode == KEY_MODE_KEY) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return FALSE;
    }
    if (wk->keyMode == KEY_MODE_KEY) {
        return func_ov110_021bdcb4(wk, seq);
    }
    return func_ov110_021bdb98(wk, seq);
}

// pokebw2's PMSInput_SentenceKey
BOOL func_ov110_021bdef0(PMSInputWork *wk, u32 *seq) {
    switch (*seq) {
    case 0:
        wk->sentenceEditPosMax = func_ov110_021babe8(wk->vwk);
        *seq = 1;
        // fallthrough
    case 1:
        if (func_02024158(wk->param)) {
            if (wk->keyRepeat & PAD_KEY_LEFT) {
                break;
            }
            if (wk->keyRepeat & PAD_KEY_RIGHT) {
                break;
            }
        }
        if (wk->keyTrg & PAD_KEY_UP) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            if (wk->editPos == 0) {
                wk->buttonPos = BUTTON_POS_CANCEL;
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_EDITAREA_TO_BUTTON);
                *seq = 2;
            } else if (wk->sentenceEditPosMax > 1 && wk->editPos != 0) {
                wk->editPos--;
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_EDITAREA_CURSOR);
            }
        } else if (wk->keyTrg & PAD_KEY_DOWN) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            if (wk->sentenceEditPosMax > 1 && wk->editPos < wk->sentenceEditPosMax - 1) {
                wk->editPos++;
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_EDITAREA_CURSOR);
            } else {
                wk->buttonPos = BUTTON_POS_DECIDE;
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_EDITAREA_TO_BUTTON);
                *seq = 2;
            }
        } else if ((wk->keyTrg & PAD_BUTTON_START) || ((wk->keyTrg & PAD_BUTTON_A) && wk->sentenceEditPosMax == 0)) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            wk->buttonPos = BUTTON_POS_DECIDE;
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_EDITAREA_TO_BUTTON);
            *seq = 2;
        } else if (wk->keyRepeat & PAD_KEY_LEFT) {
            GFL_SndSEPlay(SEQ_SE_SYS_37);
            wk->editPos = 0;
            func_ov110_021bd6f8(&wk->sentenceWk, &wk->sentence);
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_UPDATE_EDITAREA);
            func_ov110_021bac30(wk->vwk, 0);
            *seq = 3;
        } else if (wk->keyRepeat & PAD_KEY_RIGHT) {
            GFL_SndSEPlay(SEQ_SE_SYS_37);
            wk->editPos = 0;
            func_ov110_021bd6b4(&wk->sentenceWk, &wk->sentence);
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_UPDATE_EDITAREA);
            func_ov110_021bac30(wk->vwk, 1);
            *seq = 3;
        } else if (wk->keyTrg & PAD_BUTTON_B) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov110_021bf4f8(wk, func_ov110_021bf608);
        } else if (wk->keyTrg & PAD_BUTTON_A) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov110_021bf82c(wk);
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_EDITAREA_TO_CATEGORY);
            *seq = 8;
        }
        break;
    case 3:
        if (func_ov110_021b9c90(wk->vwk, PMSIV_CMD_UPDATE_EDITAREA)) {
            *seq = 0;
        }
        break;
    case 2:
        if (!func_ov110_021b9c74(wk->vwk)) {
            break;
        }
        if (wk->keyTrg & PAD_KEY_UP) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            if (wk->buttonPos != BUTTON_POS_DECIDE) {
                wk->buttonPos ^= 1;
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_BUTTON_CURSOR);
            } else {
                wk->editPos = wk->sentenceEditPosMax != 0 ? wk->sentenceEditPosMax - 1 : 0;
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_BUTTON_TO_EDITAREA_SELECT);
                *seq = 0;
            }
        } else if (wk->keyTrg & PAD_KEY_DOWN) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            if (wk->buttonPos != BUTTON_POS_DECIDE) {
                wk->editPos = 0;
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_BUTTON_TO_EDITAREA_SELECT);
                *seq = 0;
            } else {
                wk->buttonPos ^= 1;
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_BUTTON_CURSOR);
            }
        } else if (wk->keyTrg & PAD_BUTTON_START) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            wk->buttonPos = BUTTON_POS_DECIDE;
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_BUTTON_CURSOR);
        } else if (wk->keyTrg & PAD_BUTTON_A) {
            if (wk->buttonPos == BUTTON_POS_DECIDE) {
                if (func_ov110_021bf680(wk)) {
                    GFL_SndSEPlay(SEQ_SE_DECIDE1);
                } else {
                    GFL_SndSEPlay(SEQ_SE_BEEP);
                }
                *seq = 4;
            } else {
                GFL_SndSEPlay(SEQ_SE_CANCEL1);
                *seq = 5;
            }
        } else if (wk->keyTrg & PAD_BUTTON_B) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            *seq = 5;
        }
        break;
    case 4:
        *seq = 6;
        func_ov110_021bf4f8(wk, func_ov110_021bf53c);
        break;
    case 5:
        *seq = 6;
        func_ov110_021bf4f8(wk, func_ov110_021bf608);
        break;
    case 6:
        *seq = 2;
        break;
    case 7:
        if (func_ov110_021b9c74(wk->vwk)) {
            func_ov110_021bd7a0(wk, func_ov110_021be3b8);
        }
        break;
    case 8:
        if (func_ov110_021b9c74(wk->vwk)) {
            func_ov110_021bd7a0(wk, func_ov110_021be4c0);
        }
        break;
    }
    return FALSE;
}

// pokebw2's PMSInput_SentenceTouch
BOOL func_ov110_021be1b8(PMSInputWork *wk, u32 *seq) {
    s32 hit;

    switch (*seq) {
    case 0:
        wk->sentenceEditPosMax = func_ov110_021babe8(wk->vwk);
        *seq = 1;
        // fallthrough
    case 1:
    case 2:
        hit = func_ov110_021bdb50(wk);
        if (hit > -1) {
            wk->editPos = 0;
            func_ov110_021bd740(&wk->sentenceWk, &wk->sentence, hit);
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_UPDATE_EDITAREA);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            *seq = 3;
            break;
        }
        hit = func_ov110_021bdaa4(wk);
        switch (hit) {
        case 0:
            if (func_ov110_021bf680(wk)) {
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
            } else {
                GFL_SndSEPlay(SEQ_SE_BEEP);
            }
            wk->buttonPos = BUTTON_POS_DECIDE;
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_BUTTON_TO_EDITAREA);
            *seq = hit + 4;
            break;
        case 1:
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            wk->buttonPos = BUTTON_POS_CANCEL;
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_BUTTON_TO_EDITAREA);
            *seq = hit + 4;
            break;
        case 2:
        case 4:
            GFL_SndSEPlay(SEQ_SE_SYS_37);
            wk->editPos = 0;
            func_ov110_021bd6f8(&wk->sentenceWk, &wk->sentence);
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_UPDATE_EDITAREA);
            func_ov110_021bac30(wk->vwk, 0);
            *seq = 3;
            break;
        case 3:
        case 5:
            GFL_SndSEPlay(SEQ_SE_SYS_37);
            wk->editPos = 0;
            func_ov110_021bd6b4(&wk->sentenceWk, &wk->sentence);
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_UPDATE_EDITAREA);
            func_ov110_021bac30(wk->vwk, 1);
            *seq = 3;
            break;
        case 6:
        case 7:
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov110_021bf82c(wk);
            wk->editPos = hit - 6;
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_EDITAREA_TO_CATEGORY);
            *seq = 8;
            break;
        }
        break;
    case 3:
        if (func_ov110_021b9c90(wk->vwk, PMSIV_CMD_UPDATE_EDITAREA)) {
            *seq = 0;
        }
        break;
    case 4:
        *seq = 6;
        func_ov110_021bf4f8(wk, func_ov110_021bf53c);
        break;
    case 5:
        *seq = 6;
        func_ov110_021bf4f8(wk, func_ov110_021bf608);
        break;
    case 6:
        *seq = 2;
        break;
    case 7:
        if (func_ov110_021b9c74(wk->vwk)) {
            func_ov110_021bd7a0(wk, func_ov110_021be3b8);
        }
        break;
    case 8:
        if (func_ov110_021b9c74(wk->vwk)) {
            func_ov110_021bd7a0(wk, func_ov110_021be4c0);
        }
        break;
    }
    return FALSE;
}

// pokebw2's PMSInput_MainProcSentence
BOOL func_ov110_021be374(PMSInputWork *wk, u32 *seq) {
    if (*seq <= 2 && func_ov110_021bd808(wk, seq) && wk->keyMode == KEY_MODE_KEY) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return FALSE;
    }
    if (wk->keyMode == KEY_MODE_KEY) {
        return func_ov110_021bdef0(wk, seq);
    }
    return func_ov110_021be1b8(wk, seq);
}

// pokebw2's PMSInput_MainProcCommandButton
BOOL func_ov110_021be3b8(PMSInputWork *wk, u32 *seq) {
    switch (*seq) {
    case 0:
        if (!func_ov110_021b9c74(wk->vwk)) {
            break;
        }
        if (wk->keyTrg & PAD_KEY_UP) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            if (wk->buttonPos != BUTTON_POS_DECIDE) {
                wk->buttonPos ^= 1;
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_BUTTON_CURSOR);
            } else {
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_BUTTON_TO_EDITAREA_SELECT);
                *seq = 0;
            }
        } else if (wk->keyTrg & PAD_KEY_DOWN) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            if (wk->buttonPos != BUTTON_POS_DECIDE) {
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_BUTTON_TO_EDITAREA_SELECT);
                *seq = 0;
            } else {
                wk->buttonPos ^= 1;
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_BUTTON_CURSOR);
            }
        } else if (wk->keyTrg & PAD_BUTTON_START) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            wk->buttonPos = BUTTON_POS_DECIDE;
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_BUTTON_CURSOR);
        } else if (wk->keyTrg & PAD_BUTTON_A) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            if (wk->buttonPos == BUTTON_POS_DECIDE) {
                *seq = 1;
            } else {
                *seq = 2;
            }
        } else if (wk->keyTrg & PAD_BUTTON_B) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            *seq = 2;
        }
        break;
    case 1:
        *seq = 3;
        func_ov110_021bf4f8(wk, func_ov110_021bf53c);
        break;
    case 2:
        *seq = 4;
        func_ov110_021bf4f8(wk, func_ov110_021bf608);
        break;
    case 3:
    case 4:
        *seq = 0;
        break;
    case 5:
        if (func_ov110_021b9c74(wk->vwk)) {
            func_ov110_021bd7a0(wk, func_ov110_021bd894);
        }
        break;
    }
    return FALSE;
}

// pokebw2's PMSInput_MainProcCategory
BOOL func_ov110_021be4c0(PMSInputWork *wk, u32 *seq) {
    switch (*seq) {
    case 0:
        if (func_ov110_021b9c90(wk->vwk, PMSIV_CMD_MOVE_CATEGORY_CURSOR) &&
            func_ov110_021b9c90(wk->vwk, PMSIV_CMD_MOVE_CATEGORY) &&
            func_ov110_021b9c90(wk->vwk, PMSIV_CMD_SHOW_MENU)) {
            func_ov110_021beadc(wk, seq);
        }
        break;
    case 1:
        if (func_ov110_021b9c90(wk->vwk, PMSIV_CMD_SHOW_MENU)) {
            if (wk->categoryPosSaved != CATEGORY_POS_ERASE) {
                wk->categoryPos = wk->categoryPosSaved;
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_CATEGORY_CURSOR);
            }
            *seq = 0;
        }
        break;
    case 2:
        if (func_ov110_021b9c74(wk->vwk)) {
            if (wk->nextProc == func_ov110_021bd894) {
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_KTCHANGE_EDITAREA);
            }
            *seq = 3;
        }
        break;
    case 3:
        if (func_ov110_021b9c74(wk->vwk)) {
            func_ov110_021bd7a0(wk, wk->nextProc);
        }
        break;
    case 4:
        if (func_ov110_021b9c90(wk->vwk, PMSIV_CMD_CHANGE_CATEGORY_MODE_ENABLE)) {
            *seq = 0;
        }
        break;
    }
    return FALSE;
}

// pokebw2's PMSInput_CategoryKey
void func_ov110_021be580(PMSInputWork *wk, u32 *seq) {
    if ((wk->keyTrg & PAD_BUTTON_START) && wk->categoryMode == 1) {
        wk->categoryPos = CATEGORY_POS_SEARCH;
        if (func_ov110_021bb054(wk->search)) {
            func_ov110_021bd868(wk);
            *seq = 2;
        } else {
            GFL_SndSEPlay(SEQ_SE_BEEP);
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_CATEGORY_CURSOR);
        }
        return;
    }
    if (wk->modeButton == 0 || (wk->keyTrg & PAD_BUTTON_SELECT)) {
        GFL_SndSEPlay(SEQ_SE_SELECT4);
        wk->categoryMode ^= 1;
        func_ov110_021bf82c(wk);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_CHANGE_CATEGORY_MODE_ENABLE);
        *seq = 4;
        return;
    }
    if (wk->keyTrg & PAD_BUTTON_B) {
        if (wk->categoryMode == 1 && func_ov110_021bafac(wk->search)) {
            wk->categoryPosSaved = wk->categoryPos;
            *seq = 1;
            if (wk->categoryPos != CATEGORY_POS_ERASE) {
                wk->categoryPos = CATEGORY_POS_ERASE;
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_CATEGORY_CURSOR);
            }
            GFL_SndSEPlay(SEQ_SE_CANCEL3);
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_SHOW_MENU);
            func_ov110_021bb054(wk->search);
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_CHANGE_CATEGORY_MODE);
            return;
        }
        if (wk->categoryMode == 1 && wk->categoryPos != CATEGORY_POS_BACK) {
            wk->categoryPos = CATEGORY_POS_BACK;
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_CATEGORY_CURSOR);
        }
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_CATEGORY_TO_EDITAREA);
        wk->nextProc = func_ov110_021bd894;
        *seq = 2;
        return;
    }
    if (wk->keyTrg & PAD_BUTTON_A) {
        if (wk->categoryPos == CATEGORY_POS_SEARCH) {
            if (func_ov110_021bb054(wk->search)) {
                func_ov110_021bd868(wk);
                *seq = 2;
            } else {
                GFL_SndSEPlay(SEQ_SE_BEEP);
            }
        } else if (wk->categoryPos == CATEGORY_POS_ERASE) {
            if (wk->categoryMode == 1 && func_ov110_021bafac(wk->search)) {
                wk->categoryPosSaved = wk->categoryPos;
                *seq = 1;
                GFL_SndSEPlay(SEQ_SE_CANCEL3);
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_SHOW_MENU);
                func_ov110_021bb054(wk->search);
                func_ov110_021b9bec(wk->vwk, PMSIV_CMD_CHANGE_CATEGORY_MODE);
            } else {
                GFL_SndSEPlay(SEQ_SE_BEEP);
            }
        } else if (wk->categoryPos == CATEGORY_POS_BACK) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_CATEGORY_TO_EDITAREA);
            wk->nextProc = func_ov110_021bd894;
            *seq = 2;
        } else if (wk->categoryMode == 0) {
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_CATEGORY);
            if (func_ov110_021beb44(wk)) {
                func_ov110_021bd868(wk);
                *seq = 2;
            } else {
                GFL_SndSEPlay(SEQ_SE_BEEP);
            }
        } else {
            func_ov110_021baf7c(wk->search, wk->categoryPos);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov110_021bb054(wk->search);
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_CHANGE_CATEGORY_MODE);
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_CATEGORY);
        }
        return;
    }
    if (func_ov110_021beb60(wk)) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_CATEGORY_CURSOR);
    }
}

// pokebw2's PMSInput_CategoryTouchButtons
int func_ov110_021be7b8(PMSInputWork *wk) {
    // The categories' back and mode buttons
    static const TouchRect data_ov110_021c392c[] = {
        { 168, 192, 232, 0 },
        { 168, 192, 0, 40 },
        { TOUCH_RECT_END, 0, 0, 0 },
    };

    s32 hit = func_02035c08(data_ov110_021c392c);

    if (hit == -1) {
        return 0;
    }
    switch (hit) {
    case 0:
        return 1;
    case 1:
        if (wk->modeButton == 0) {
            return 2;
        }
        break;
    }
    return 0;
}

// pokebw2's PMSInput_TouchReturnButton
s32 func_ov110_021be7f0(PMSInputWork *wk) {
    static const TouchRect data_ov110_021c3918[] = {
        { 168, 191, 232, 255 },
        { TOUCH_RECT_END, 0, 0, 0 },
    };

    return func_02035c08(data_ov110_021c3918);
}

// pokebw2's PMSInput_CategoryTouchGroup
int func_ov110_021be800(PMSInputWork *wk) {
    u32 x, y;
    int row, col;
    TouchRect rects[2] = {
        { TOUCH_RECT_END, 0, 0, 0 },
        { TOUCH_RECT_END, 0, 0, 0 },
    };

    if (!func_02035c60()) {
        return -1;
    }
    func_02035d20(&x, &y);
    for (row = 0; row < 4; row++) {
        rects[0].top = row * 24 + 56;
        rects[0].bottom = rects[0].top + 16;
        for (col = 0; col < 3; col++) {
            rects[0].left = col * 80 + 12;
            rects[0].right = rects[0].left + 72;
            if (func_02035d40(rects, x, y) != -1) {
                return row * 3 + col;
            }
        }
    }
    return -1;
}

// pokebw2's PMSInput_CategoryTouchSearchButtons
s32 func_ov110_021be890(PMSInputWork *wk) {
    TouchRect rects[4] = {
        { 168, 192, 40, 112 },
        { 168, 192, 112, 184 },
        { 168, 192, 184, 0 },
        { TOUCH_RECT_END, 0, 0, 0 },
    };

    return func_02035c08(rects);
}

// pokebw2's PMSInput_CategoryTouchInitial
int func_ov110_021be8b4(PMSInputWork *wk) {
    int count = func_ov110_021bb1f4();
    int i;

    for (i = 0; i < count; i++) {
        TouchRect rects[2] = {
            { 0, 0, 0, 0 },
            { TOUCH_RECT_END, 0, 0, 0 },
        };
        u32 x, y;

        func_ov110_021bb224(i, &x, &y);
        rects[0].top = y + 48;
        rects[0].bottom = y + 64;
        rects[0].left = x + 8;
        rects[0].right = x + 24;
        if (func_02035c08(rects) != -1) {
            return i;
        }
    }
    return -1;
}

// pokebw2's PMSInput_CategoryTouch
void func_ov110_021be934(PMSInputWork *wk, u32 *seq) {
    int pos;

    switch (func_ov110_021be7b8(wk)) {
    case 1:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_CATEGORY_TO_EDITAREA);
        wk->nextProc = func_ov110_021bd894;
        *seq = 2;
        return;
    case 2:
        GFL_SndSEPlay(SEQ_SE_SELECT4);
        wk->categoryMode ^= 1;
        func_ov110_021bf82c(wk);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_CHANGE_CATEGORY_MODE_ENABLE);
        *seq = 4;
        return;
    }
    if (wk->categoryMode == 0) {
        pos = func_ov110_021be800(wk);
        if (pos < 0) {
            return;
        }
        wk->categoryPos = pos;
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_CATEGORY);
        if (pos >= GROUP_COUNT || func_ov110_021b999c(wk->dwk, wk->categoryPos) == 0) {
            GFL_SndSEPlay(SEQ_SE_BEEP);
            return;
        }
        func_ov110_021bd868(wk);
        *seq = 2;
        return;
    }
    pos = func_ov110_021be8b4(wk);
    if (pos > -1) {
        wk->categoryPos = pos;
        func_ov110_021baf7c(wk->search, wk->categoryPos);
        func_ov110_021bb054(wk->search);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_CHANGE_CATEGORY_MODE);
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_CATEGORY);
        return;
    }
    switch (func_ov110_021be890(wk)) {
    case -1:
        break;
    case 0:
        wk->categoryPos = CATEGORY_POS_SEARCH;
        if (func_ov110_021bb054(wk->search)) {
            func_ov110_021bd868(wk);
            *seq = 2;
        } else {
            GFL_SndSEPlay(SEQ_SE_BEEP);
        }
        break;
    case 1:
        wk->categoryPos = CATEGORY_POS_ERASE;
        if (func_ov110_021bafac(wk->search)) {
            wk->categoryPosSaved = wk->categoryPos;
            *seq = 1;
            GFL_SndSEPlay(SEQ_SE_CANCEL3);
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_SHOW_MENU);
            func_ov110_021bb054(wk->search);
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_CHANGE_CATEGORY_MODE);
        } else {
            GFL_SndSEPlay(SEQ_SE_BEEP);
        }
        break;
    case 2:
        wk->categoryPos = CATEGORY_POS_BACK;
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_CATEGORY_TO_EDITAREA);
        wk->nextProc = func_ov110_021bd894;
        *seq = 2;
        break;
    }
}

// pokebw2's PMSInput_CategoryInput
void func_ov110_021beadc(PMSInputWork *wk, u32 *seq) {
    if (func_ov110_021bd808(wk, seq)) {
        if (wk->categoryMode == 0) {
            if (wk->keyMode == KEY_MODE_KEY && !(GCTX_HIDGetPressedKeys() & PAD_BUTTON_B)) {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
                return;
            }
        } else if (wk->keyMode == KEY_MODE_KEY) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return;
        }
    }
    if (func_ov110_021b9c90(wk->vwk, PMSIV_CMD_CHANGE_CATEGORY_MODE)) {
        if (wk->keyMode == KEY_MODE_KEY) {
            func_ov110_021be580(wk, seq);
        } else {
            func_ov110_021be934(wk, seq);
        }
    }
}

// pokebw2's PMSInput_IsCategoryEnabled
BOOL func_ov110_021beb44(PMSInputWork *wk) {
    if (func_ov110_021b999c(wk->dwk, wk->categoryPos)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's PMSInput_CategoryMoveCursor
BOOL func_ov110_021beb60(PMSInputWork *wk) {
    static const PMSInputCursorFunc data_ov110_021c3910[] = {
        func_ov110_021beb78,
        func_ov110_021bec20,
    };

    return data_ov110_021c3910[wk->categoryMode](wk);
}

// pokebw2's PMSInput_CategoryKeyGroup
BOOL func_ov110_021beb78(PMSInputWork *wk) {
    // Where the cursor goes from each group, up, down, left and right
    static const u8 data_ov110_021c3978[GROUP_COUNT][4] = {
#include "asset/pms_input_data_ov110_021c3978.inc"
};

    u16 pos = wk->categoryPos;
    u32 idx = pos;

    if (pos == CATEGORY_POS_BACK) {
        idx = GROUP_COUNT;
    }
    if (pos != CATEGORY_POS_BACK) {
        wk->categoryPosPrev = pos;
        if (wk->keyRepeat & PAD_KEY_UP) {
            wk->categoryPos = data_ov110_021c3978[idx][0];
            return TRUE;
        }
        if (wk->keyRepeat & PAD_KEY_DOWN) {
            wk->categoryPos = data_ov110_021c3978[idx][1];
            return TRUE;
        }
        if (wk->keyRepeat & PAD_KEY_LEFT) {
            wk->categoryPos = data_ov110_021c3978[idx][2];
            return TRUE;
        }
        if (wk->keyRepeat & PAD_KEY_RIGHT) {
            wk->categoryPos = data_ov110_021c3978[idx][3];
            return TRUE;
        }
    } else {
        wk->categoryPosPrev = CATEGORY_POS_BACK;
        if (wk->keyRepeat & PAD_KEY_UP) {
            wk->categoryPos = 11;
            return TRUE;
        }
        if (wk->keyRepeat & PAD_KEY_DOWN) {
            wk->categoryPos = 2;
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's PMSInput_CategoryKeyInitial
BOOL func_ov110_021bec20(PMSInputWork *wk) {
    u16 pos = wk->categoryPos;
    int next;

    if (pos != CATEGORY_POS_BACK && pos != CATEGORY_POS_SEARCH && pos != CATEGORY_POS_ERASE) {
        next = CATEGORY_POS_NONE;
        if (wk->keyRepeat & PAD_KEY_UP) {
            next = func_ov110_021bb240(pos);
        } else if (wk->keyRepeat & PAD_KEY_DOWN) {
            next = func_ov110_021bb250(pos);
        } else if (wk->keyRepeat & PAD_KEY_RIGHT) {
            next = func_ov110_021bb270(pos);
        } else if (wk->keyRepeat & PAD_KEY_LEFT) {
            next = func_ov110_021bb260(pos);
        }
        if (next != CATEGORY_POS_NONE) {
            wk->categoryPosPrev = wk->categoryPos;
            wk->categoryPos = next;
            return TRUE;
        }
    } else if (wk->keyRepeat & PAD_KEY_UP) {
        next = wk->categoryPosPrev <= PMSI_INITIAL_COUNT - 1 ? func_ov110_021bb280(wk->categoryPosPrev)
                                                             : CATEGORY_POS_NONE;
        if (wk->categoryPos == CATEGORY_POS_SEARCH) {
            if (next >= 20 && next <= 23) {
                wk->categoryPos = next;
            } else {
                wk->categoryPos = 22;
            }
        } else if (wk->categoryPos == CATEGORY_POS_ERASE) {
            if (next == 24 || next == 25 || next == 16) {
                wk->categoryPos = next;
            } else {
                wk->categoryPos = 25;
            }
        } else {
            if (next == 17 || next == 18 || next == 26) {
                wk->categoryPos = next;
            } else {
                wk->categoryPos = 26;
            }
        }
        return TRUE;
    } else if (wk->keyRepeat & PAD_KEY_DOWN) {
        next = wk->categoryPosPrev <= PMSI_INITIAL_COUNT - 1 ? func_ov110_021bb280(wk->categoryPosPrev)
                                                             : CATEGORY_POS_NONE;
        if (wk->categoryPos == CATEGORY_POS_SEARCH) {
            if (next >= 0 && next <= 3) {
                wk->categoryPos = next;
            } else {
                wk->categoryPos = 2;
            }
        } else if (wk->categoryPos == CATEGORY_POS_ERASE) {
            if (next >= 4 && next <= 6) {
                wk->categoryPos = next;
            } else {
                wk->categoryPos = 5;
            }
        } else {
            if (next >= 7 && next <= 9) {
                wk->categoryPos = next;
            } else {
                wk->categoryPos = 8;
            }
        }
        return TRUE;
    } else if (wk->keyRepeat & PAD_KEY_LEFT) {
        switch (pos) {
        case CATEGORY_POS_BACK:
            wk->categoryPos = CATEGORY_POS_ERASE;
            break;
        case CATEGORY_POS_ERASE:
            wk->categoryPos = CATEGORY_POS_SEARCH;
            break;
        case CATEGORY_POS_SEARCH:
            wk->categoryPos = CATEGORY_POS_BACK;
            break;
        }
        return TRUE;
    } else if (wk->keyRepeat & PAD_KEY_RIGHT) {
        switch (pos) {
        case CATEGORY_POS_BACK:
            wk->categoryPos = CATEGORY_POS_SEARCH;
            break;
        case CATEGORY_POS_ERASE:
            wk->categoryPos = CATEGORY_POS_BACK;
            break;
        case CATEGORY_POS_SEARCH:
            wk->categoryPos = CATEGORY_POS_ERASE;
            break;
        }
        return TRUE;
    }
    return FALSE;
}

// pokebw2's PMSInput_SetupWordWin
void func_ov110_021bed84(PMSInputWordWin *ww, PMSInputWork *wk) {
    ww->top = 0;
    ww->x = 0;
    ww->y = 0;
    ww->wordMax = func_ov110_021bf734(wk);
    ww->scrollVector = 0;
    ww->onBack = 0;
    ww->selected = PMS_WORD_NULL;
    if (ww->wordMax > WORDWIN_VISIBLE_WORDS) {
        ww->scrollMax = (ww->wordMax - WORDWIN_VISIBLE_WORDS) / 2 + (ww->wordMax & 1);
    } else {
        ww->scrollMax = 0;
    }
}

// pokebw2's PMSInput_WordWinGetCursorPos
static u32 func_ov110_021bedc4(const PMSInputWordWin *ww) {
    return ww->x + ww->y * 2;
}

// pokebw2's PMSInput_WordWinGetPos
int func_ov110_021bedd0(const PMSInputWordWin *ww) {
    return ww->top * 2 + func_ov110_021bedc4(ww);
}

// pokebw2's PMSInput_WordWinGetScrollVector
static int func_ov110_021bede0(const PMSInputWordWin *ww) {
    return ww->scrollVector;
}

// pokebw2's PMSInput_WordWinGetTop
static u16 func_ov110_021bede4(const PMSInputWordWin *ww) {
    return ww->top;
}

// pokebw2's PMSInput_WordWinGetScrollMax
static u16 func_ov110_021bede8(const PMSInputWordWin *ww) {
    return ww->scrollMax;
}

// pokebw2's PMSInput_MainProcWordWin
BOOL func_ov110_021bedec(PMSInputWork *wk, u32 *seq) {
    switch (*seq) {
    case 0:
        func_ov110_021bf1c4(wk, seq);
        break;
    case 1:
        if (func_ov110_021b9c90(wk->vwk, PMSIV_CMD_SCROLL_WORDWIN)) {
            *seq = 0;
        }
        break;
    case 2:
        if (func_ov110_021b9c90(wk->vwk, PMSIV_CMD_SCROLL_WORDWIN)) {
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_WORDWIN_CURSOR);
            *seq = 0;
        }
        break;
    case 3:
        if (func_ov110_021b9c90(wk->vwk, PMSIV_CMD_SET_WORDWIN_ARROWS)) {
            *seq = 0;
        }
        break;
    case 4:
        if (func_ov110_021b9c74(wk->vwk)) {
            func_ov110_021bd7a0(wk, wk->nextProc);
            *seq = 0;
        }
        break;
    }
    return FALSE;
}

// pokebw2's PMSInput_WordWinKey
void func_ov110_021bee60(PMSInputWork *wk, u32 *seq) {
    switch (func_ov110_021bf208(&wk->wordWin, wk->keyRepeat)) {
    case WORDWIN_RESULT_CURSOR:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_WORDWIN_CURSOR);
        return;
    case WORDWIN_RESULT_SCROLL:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_SCROLL_WORDWIN);
        *seq = 1;
        return;
    case WORDWIN_RESULT_SCROLL_CURSOR:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_SCROLL_WORDWIN);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_WORDWIN_CURSOR);
        *seq = 2;
        return;
    }
    if (wk->keyTrg & PAD_BUTTON_B) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_WORDWIN_TO_CATEGORY);
        wk->nextProc = func_ov110_021be4c0;
        *seq = 4;
        return;
    }
    if (wk->keyTrg & PAD_BUTTON_A) {
        if (wk->wordWin.onBack) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_WORDWIN_TO_CATEGORY);
            wk->nextProc = func_ov110_021be4c0;
            *seq = 4;
            return;
        }
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        wk->wordWin.selected = PMS_WORD_NULL;
        func_ov110_021bf444(wk);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_WORDWIN);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_WORDWIN_TO_EDITAREA);
        wk->nextProc = func_ov110_021bd894;
        *seq = 4;
    }
}

// pokebw2's PMSInput_WordWinTouchWord
int func_ov110_021bef4c(PMSInputWork *wk) {
    u32 x, y;
    int row, col;
    u16 pos;
    BOOL done;
    TouchRect rects[2] = {
        { TOUCH_RECT_END, 0, 0, 0 },
        { TOUCH_RECT_END, 0, 0, 0 },
    };
    // The scroll bar of the word window
    static const TouchRect data_ov110_021c3900[] = {
        { 4, 163, 236, 251 },
        { TOUCH_RECT_END, 0, 0, 0 },
    };

    if (func_02035c08(data_ov110_021c3900) == 0) {
        func_02035d20(&x, &y);
        switch (func_ov110_021bacdc(wk->vwk, x, y)) {
        case 0:
            wk->scrollBarHeld = TRUE;
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return WORDWIN_RESULT_NONE;
        case 1:
            wk->scrollBarHeld = TRUE;
            func_ov110_021bad14(wk->vwk, y);
            return func_ov110_021bf3a0(wk->vwk, &wk->wordWin);
        case 2:
            wk->scrollBarHeld = TRUE;
            func_ov110_021bad14(wk->vwk, y);
            return func_ov110_021bf3d4(wk->vwk, &wk->wordWin);
        case 3:
            break;
        }
    }
    func_02035d20(&x, &y);
    pos = wk->wordWin.top * 2;
    done = FALSE;
    for (row = 0; row < WORDWIN_ROWS; row++) {
        rects[0].top = row * 24 + 13;
        rects[0].bottom = rects[0].top + 22;
        for (col = 0; col < 2; col++) {
            rects[0].left = col * 112 + 16;
            rects[0].right = rects[0].left + 102;
            if (func_02035d40(rects, x, y) != -1) {
                wk->wordWin.selected = pos;
                wk->wordWin.x = col;
                wk->wordWin.y = row;
                return WORDWIN_RESULT_DECIDE;
            }
            pos++;
            if (pos >= wk->wordWin.wordMax - 2) {
                done = TRUE;
                break;
            }
        }
        if (done) {
            break;
        }
    }
    return WORDWIN_RESULT_NONE;
}

// pokebw2's PMSInput_WordWinTouchScrollBar
BOOL func_ov110_021bf08c(PMSInputWork *wk) {
    u32 x, y;

    if (func_02035cb8(&x, &y) == TRUE && wk->scrollBarHeld == TRUE) {
        func_ov110_021bad14(wk->vwk, y);
        return TRUE;
    }
    wk->scrollBarHeld = FALSE;
    return FALSE;
}

// pokebw2's PMSInput_WordWinTouch
void func_ov110_021bf0c0(PMSInputWork *wk, u32 *seq) {
    if (func_ov110_021be7f0(wk) == 0) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_WORDWIN_TO_CATEGORY);
        wk->nextProc = func_ov110_021be4c0;
        *seq = 4;
        return;
    }
    if (func_ov110_021bf08c(wk) == TRUE) {
        switch (func_ov110_021bf408(wk->vwk, &wk->wordWin)) {
        case WORDWIN_RESULT_SCROLL:
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_SET_WORDWIN_ARROWS);
            *seq = 3;
            return;
        case WORDWIN_RESULT_SCROLL_CURSOR:
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_SET_WORDWIN_ARROWS);
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_WORDWIN_CURSOR);
            *seq = 3;
            return;
        }
        return;
    }
    switch (func_ov110_021bef4c(wk)) {
    case WORDWIN_RESULT_DECIDE:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov110_021bf444(wk);
        wk->nextProc = func_ov110_021bd894;
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_WORDWIN);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_WORDWIN_TO_EDITAREA);
        *seq = 4;
        break;
    case WORDWIN_RESULT_CURSOR:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_WORDWIN_CURSOR);
        break;
    case WORDWIN_RESULT_SCROLL:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_SET_WORDWIN_ARROWS);
        *seq = 3;
        break;
    case WORDWIN_RESULT_SCROLL_CURSOR:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_SCROLL_WORDWIN);
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_MOVE_WORDWIN_CURSOR);
        *seq = 2;
        break;
    }
}

// pokebw2's PMSInput_WordWinInput
void func_ov110_021bf1c4(PMSInputWork *wk, u32 *seq) {
    if (func_ov110_021bd808(wk, seq) && wk->keyMode == KEY_MODE_KEY &&
        !(GCTX_HIDGetPressedKeys() & PAD_BUTTON_B)) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return;
    }
    if (wk->keyMode == KEY_MODE_KEY) {
        func_ov110_021bee60(wk, seq);
    } else {
        func_ov110_021bf0c0(wk, seq);
    }
}

// pokebw2's PMSInput_WordWinCheckKey
int func_ov110_021bf208(PMSInputWordWin *ww, u16 key) {
    u32 pos;

    if (key & PAD_KEY_DOWN) {
        if (ww->onBack) {
            return WORDWIN_RESULT_IGNORE;
        }
        if (ww->y < WORDWIN_ROWS - 1) {
            ww->y++;
            pos = func_ov110_021bedd0(ww);
            if (pos < ww->wordMax - 2) {
                return WORDWIN_RESULT_CURSOR;
            }
            if (pos == ww->wordMax - 2) {
                if (pos & 1) {
                    ww->x = 0;
                    return WORDWIN_RESULT_CURSOR;
                }
                ww->y--;
                return WORDWIN_RESULT_IGNORE;
            }
            ww->y--;
            return WORDWIN_RESULT_IGNORE;
        }
        if (ww->top < ww->scrollMax) {
            ww->scrollVector = 1;
            ww->top++;
            pos = func_ov110_021bedd0(ww);
            if (pos < ww->wordMax - 2) {
                return WORDWIN_RESULT_SCROLL;
            }
            ww->x = 0;
            return WORDWIN_RESULT_SCROLL_CURSOR;
        }
        return WORDWIN_RESULT_IGNORE;
    }
    if (key & PAD_KEY_UP) {
        if (ww->onBack) {
            ww->onBack = 0;
            return WORDWIN_RESULT_CURSOR;
        }
        if (ww->y != 0) {
            ww->y--;
            return WORDWIN_RESULT_CURSOR;
        }
        if (ww->top != 0) {
            ww->scrollVector = -1;
            ww->top--;
            return WORDWIN_RESULT_SCROLL;
        }
        return WORDWIN_RESULT_IGNORE;
    }
    if (key & PAD_KEY_LEFT) {
        if (ww->x == 1) {
            ww->x ^= 1;
            pos = func_ov110_021bedd0(ww);
            if (pos < ww->wordMax - 2) {
                return WORDWIN_RESULT_CURSOR;
            }
            ww->x ^= 1;
        }
        return WORDWIN_RESULT_IGNORE;
    }
    if (key & PAD_KEY_RIGHT) {
        if (ww->x == 0) {
            ww->x ^= 1;
            pos = func_ov110_021bedd0(ww);
            if (pos < ww->wordMax - 2) {
                return WORDWIN_RESULT_CURSOR;
            }
            ww->x ^= 1;
        }
        return WORDWIN_RESULT_IGNORE;
    }
    if (key & PAD_BUTTON_L) {
        return func_ov110_021bf338(ww);
    }
    if (key & PAD_BUTTON_R) {
        return func_ov110_021bf360(ww);
    }
    return WORDWIN_RESULT_NONE;
}

// pokebw2's PMSInput_WordWinPageUp
int func_ov110_021bf338(PMSInputWordWin *ww) {
    if (ww->top != 0) {
        if (ww->top >= WORDWIN_ROWS) {
            ww->top -= WORDWIN_ROWS;
            ww->scrollVector = -WORDWIN_ROWS;
        } else {
            ww->scrollVector = -ww->top;
            ww->top = 0;
        }
        return WORDWIN_RESULT_SCROLL;
    }
    return WORDWIN_RESULT_IGNORE;
}

// pokebw2's PMSInput_WordWinPageDown
int func_ov110_021bf360(PMSInputWordWin *ww) {
    if (ww->top < ww->scrollMax) {
        int next = ww->top + WORDWIN_ROWS;

        if (next <= ww->scrollMax) {
            ww->scrollVector = WORDWIN_ROWS;
            ww->top = next;
        } else {
            ww->scrollVector = ww->scrollMax - ww->top;
            ww->top = ww->scrollMax;
        }
        if (func_ov110_021bedd0(ww) < ww->wordMax - 2) {
            return WORDWIN_RESULT_SCROLL;
        }
        ww->x = 0;
        return WORDWIN_RESULT_SCROLL_CURSOR;
    }
    return WORDWIN_RESULT_IGNORE;
}

// pokebw2's PMSInput_WordWinScrollBarUp
int func_ov110_021bf3a0(PMSInputView *vwk, PMSInputWordWin *ww) {
    if (ww->top != 0) {
        ww->top = func_ov110_021bad20(vwk, ww->scrollMax);
        ww->scrollVector = 0;
        if (func_ov110_021bedd0(ww) < ww->wordMax - 2) {
            return WORDWIN_RESULT_SCROLL;
        }
        ww->x = 0;
        return WORDWIN_RESULT_SCROLL_CURSOR;
    }
    return WORDWIN_RESULT_IGNORE;
}

// pokebw2's PMSInput_WordWinScrollBarDown
int func_ov110_021bf3d4(PMSInputView *vwk, PMSInputWordWin *ww) {
    if (ww->top < ww->scrollMax) {
        ww->top = func_ov110_021bad20(vwk, ww->scrollMax);
        ww->scrollVector = 0;
        if (func_ov110_021bedd0(ww) < ww->wordMax - 2) {
            return WORDWIN_RESULT_SCROLL;
        }
        ww->x = 0;
        return WORDWIN_RESULT_SCROLL_CURSOR;
    }
    return WORDWIN_RESULT_IGNORE;
}

// pokebw2's PMSInput_WordWinScrollBarMove
int func_ov110_021bf408(PMSInputView *vwk, PMSInputWordWin *ww) {
    u16 top = ww->top;

    ww->top = func_ov110_021bad20(vwk, ww->scrollMax);
    ww->scrollVector = 0;
    if (top != ww->top) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
    }
    if (func_ov110_021bedd0(ww) < ww->wordMax - 2) {
        return WORDWIN_RESULT_SCROLL;
    }
    ww->x = 0;
    return WORDWIN_RESULT_SCROLL_CURSOR;
}

// pokebw2's PMSInput_SetSelectWord
BOOL func_ov110_021bf444(PMSInputWork *wk) {
    u32 pos = wk->wordWin.selected;
    u16 word;

    if (pos == PMS_WORD_NULL) {
        pos = func_ov110_021bedd0(&wk->wordWin);
    }
    if (wk->categoryMode == 0) {
        word = func_ov110_021b99c4(wk->dwk, wk->categoryPos, pos);
    } else {
        word = func_ov110_021bb1e4(wk->search, pos);
    }
    switch (wk->inputMode) {
    case PMSI_MODE_WORD:
        wk->words[0] = word;
        break;
    case PMSI_MODE_DOUBLE_WORD:
        wk->words[wk->editPos] = word;
        break;
    case PMSI_MODE_SENTENCE:
        func_020245a0(&wk->sentence, func_ov110_021babf4(wk->vwk, wk->editPos), word);
        break;
    }
    return func_ov110_021bf680(wk);
}

// pokebw2's PMSInput_MainProcQuit
BOOL func_ov110_021bf4c4(PMSInputWork *wk, u32 *seq) {
    switch (*seq) {
    case 0:
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_QUIT);
        (*seq)++;
        break;
    case 1:
        if (func_ov110_021b9c74(wk->vwk)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's PMSInput_SetSubProc
void func_ov110_021bf4f8(PMSInputWork *wk, PMSInputSubProc proc) {
    wk->subProc = proc;
    wk->subSeq = 0;
}

// pokebw2's PMSInput_QuitSubProc
void func_ov110_021bf500(PMSInputWork *wk) {
    wk->subProc = NULL;
}

// pokebw2's PMSInput_SubProcFadeIn
void func_ov110_021bf508(PMSInputWork *wk, u32 *seq) {
    switch (*seq) {
    case 0:
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_FADEIN);
        (*seq)++;
        break;
    case 1:
        if (func_ov110_021b9c74(wk->vwk)) {
            func_ov110_021bf500(wk);
        }
        break;
    }
}

// pokebw2's PMSInput_SubProcCommandOK
void func_ov110_021bf53c(PMSInputWork *wk, u32 *seq) {
    switch (*seq) {
    case 0:
        if (!func_ov110_021b9c74(wk->vwk)) {
            break;
        }
        if (func_ov110_021bf680(wk)) {
            func_ov110_021bf6cc(&wk->menu, 1, 0);
            wk->buttonPos = BUTTON_POS_DECIDE;
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_PUSH_BUTTON);
            *seq = 1;
        } else {
            *seq = 3;
        }
        break;
    case 1:
        switch (func_ov110_021bac0c(wk->vwk)) {
        case 0:
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_UPDATE_EDITAREA_CURSOR);
            *seq = 3;
            break;
        case 1:
            if (wk->inputMode == PMSI_MODE_SENTENCE) {
                func_020245ac(&wk->sentence, HEAPID_PMS_INPUT);
            }
            func_020241e0(wk->param, wk->words, &wk->sentence);
            func_ov110_021bd7a0(wk, func_ov110_021bf4c4);
            *seq = 3;
            break;
        }
        break;
    case 2:
        if (wk->keyTrg & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_PLUS_KEY_MASK)) {
            *seq = 3;
        }
        break;
    case 3:
        if (func_ov110_021b9c74(wk->vwk)) {
            func_ov110_021bf500(wk);
        }
        break;
    }
}

// pokebw2's PMSInput_SubProcCommandCancel
void func_ov110_021bf608(PMSInputWork *wk, u32 *seq) {
    switch (*seq) {
    case 0:
        func_ov110_021bf6cc(&wk->menu, 1, 1);
        wk->buttonPos = BUTTON_POS_CANCEL;
        func_ov110_021b9bec(wk->vwk, PMSIV_CMD_PUSH_BUTTON);
        *seq = 1;
        break;
    case 1:
        switch (func_ov110_021bac0c(wk->vwk)) {
        case 0:
            func_ov110_021b9bec(wk->vwk, PMSIV_CMD_UPDATE_EDITAREA_CURSOR);
            *seq = 2;
            break;
        case 1:
            func_ov110_021bd7a0(wk, func_ov110_021bf4c4);
            *seq = 2;
            break;
        }
        break;
    case 2:
        if (func_ov110_021b9c74(wk->vwk)) {
            func_ov110_021bf500(wk);
        }
        break;
    }
}

// pokebw2's PMSInput_CheckInputComplete
static BOOL func_ov110_021bf680(PMSInputWork *wk) {
    switch (wk->inputMode) {
    case PMSI_MODE_WORD:
        if (wk->words[0] != PMS_WORD_NULL) {
            return TRUE;
        }
        return FALSE;
    case PMSI_MODE_DOUBLE_WORD:
        if (wk->words[0] != PMS_WORD_NULL && wk->words[1] != PMS_WORD_NULL) {
            return TRUE;
        }
        return FALSE;
    case PMSI_MODE_SENTENCE:
        return func_02024464(&wk->sentence, HEAPID_PMS_INPUT);
    }
    return FALSE;
}

// pokebw2's PMSInput_InitMenuState
void func_ov110_021bf6cc(PMSInputMenu *menu, u8 max, u8 pos) {
    menu->pos = pos;
    menu->max = max;
}

// pokebw2's PMSInput_GetKeyModePtr
int *func_ov110_021bf6d4(const PMSInputWork *wk) {
    return (int *)&wk->keyMode;
}

// pokebw2's PMSInput_GetInputMode
u32 func_ov110_021bf6d8(const PMSInputWork *wk) {
    return wk->inputMode;
}

// pokebw2's PMSInput_GetCategoryMode
u32 func_ov110_021bf6dc(const PMSInputWork *wk) {
    return wk->categoryMode;
}

// pokebw2's PMSInput_GetSentenceType
u16 func_ov110_021bf6e4(const PMSInputWork *wk) {
    return func_0202452c(&wk->sentence);
}

// pokebw2's PMSInput_GetEditWord
u16 func_ov110_021bf6f0(const PMSInputWork *wk, u32 index) {
    if (wk->inputMode == PMSI_MODE_SENTENCE) {
        return func_020244f0(&wk->sentence, index);
    }
    return wk->words[index];
}

// pokebw2's PMSInput_GetEditSourceString
StrBuf *func_ov110_021bf708(const PMSInputWork *wk, HeapID heapId) {
    return func_0202441c(&wk->sentence, heapId);
}

// pokebw2's PMSInput_GetEditAreaCursorPos
u32 func_ov110_021bf714(const PMSInputWork *wk) {
    return wk->editPos;
}

// pokebw2's PMSInput_GetButtonCursorPos
u32 func_ov110_021bf71c(const PMSInputWork *wk) {
    return wk->buttonPos;
}

// pokebw2's PMSInput_GetCategoryCursorPos
u32 func_ov110_021bf724(const PMSInputWork *wk) {
    return wk->categoryPos;
}

// pokebw2's PMSInput_GetCategoryPosSaved
u32 func_ov110_021bf72c(const PMSInputWork *wk) {
    return wk->categoryPosSaved;
}

// pokebw2's PMSInput_GetCategoryWordMax
u32 func_ov110_021bf734(const PMSInputWork *wk) {
    if (wk->categoryMode == 0) {
        return func_ov110_021b999c(wk->dwk, wk->categoryPos) + 2;
    }
    return func_ov110_021bb1c0(wk->search) + 2;
}

// pokebw2's PMSInput_GetCategoryWord
void func_ov110_021bf758(const PMSInputWork *wk, u32 index, StrBuf *buf) {
    if (wk->categoryMode == 0) {
        func_ov110_021b99a4(wk->dwk, wk->categoryPos, index, buf);
    } else {
        func_ov110_021bb1cc(wk->search, index, buf);
    }
}

// pokebw2's PMSInput_GetWordWinCursorPos
u32 func_ov110_021bf784(const PMSInputWork *wk) {
    if (wk->wordWin.onBack) {
        return -1;
    }
    return func_ov110_021bedc4(&wk->wordWin);
}

// pokebw2's PMSInput_GetWordWinScrollVector
int func_ov110_021bf7a0(const PMSInputWork *wk) {
    return func_ov110_021bede0(&wk->wordWin);
}

// pokebw2's PMSInput_GetWordWinUpArrowVisible
BOOL func_ov110_021bf7ac(const PMSInputWork *wk) {
    return func_ov110_021bede4(&wk->wordWin);
}

// pokebw2's PMSInput_GetWordWinDownArrowVisible
BOOL func_ov110_021bf7b8(const PMSInputWork *wk) {
    return func_ov110_021bede8(&wk->wordWin);
}

// pokebw2's PMSInput_HasStartSentence
BOOL func_ov110_021bf7c4(const PMSInputWork *wk) {
    return func_02024158(wk->param);
}

// pokebw2's PMSInput_GetTCBManager
TCBManager *func_ov110_021bf7d0(const PMSInputWork *wk) {
    return wk->tcbMgr;
}

// pokebw2's PMSInput_GetSearchInputStr
void func_ov110_021bf7d4(const PMSInputWork *wk, StrBuf *buf) {
    func_ov110_021bb00c(wk->search, buf);
}

// pokebw2's PMSInput_ResetSearch
void func_ov110_021bf7e0(const PMSInputWork *wk) {
    func_ov110_021bafe4(wk->search);
}

// pokebw2's PMSInput_GetWordWinScroll
void func_ov110_021bf7ec(const PMSInputWork *wk, u16 *top, u16 *scrollMax) {
    *top = wk->wordWin.top;
    *scrollMax = wk->wordWin.scrollMax;
}

// pokebw2's PMSInput_GetSearchResultCount
u32 func_ov110_021bf7fc(const PMSInputWork *wk) {
    return func_ov110_021bb1c0(wk->search);
}

// pokebw2's PMSInput_GetSearchInputLen
u32 func_ov110_021bf808(const PMSInputWork *wk) {
    return func_ov110_021bafdc(wk->search);
}

// pokebw2's PMSInput_GetSearchResultStr
void func_ov110_021bf814(const PMSInputWork *wk, u32 index, StrBuf *buf) {
    func_ov110_021bb1cc(wk->search, index, buf);
}

// pokebw2's PMSInput_IsEditComplete
BOOL func_ov110_021bf824(const PMSInputWork *wk) {
    return func_ov110_021bf680((PMSInputWork *)wk);
}

// pokebw2's PMSInput_ResetCategoryPos
void func_ov110_021bf82c(PMSInputWork *wk) {
    wk->categoryPos = 0;
    wk->categoryPosPrev = CATEGORY_POS_NONE;
    wk->categoryPosSaved = CATEGORY_POS_ERASE;
}
