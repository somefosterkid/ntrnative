// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov298/zukan_detail.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/zukan_detail.h"
#include "constants/arc.h"
#include "gfl/fade.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "save/pokedex.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/printsys.h"

// The Pokédex's detail screen: sets up the bars and the systems that its pages share, and runs the page that the bars
// switch to

// Black 2's; twinned code confirms the offsets of graphic, font, printQueue, vblankTcb, touchbar, headbar, common,
// procSys and pageParam in Black
typedef struct {
    HeapID heapId;
    ZukanDetailGraphic *graphic;
    Font *font;
    PrintQueue *printQueue;
    TCB *vblankTcb;
    ZukanDetailTouchbar *touchbar;
    ZukanDetailHeadbar *headbar;
    ZukanDetailCommon *common;
    ZukanDetailProcSys *procSys;
    void *pageParam;
    // The page to switch to once the current one has exited, or ZUKAN_DETAIL_PAGE_NONE
    int nextPage;
} ZukanDetailWork;

BOOL func_ov208_021bca68(GameProc *proc, u32 *state, void *param_, void *work);
BOOL func_ov208_021bcb4c(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov208_021bcbb4(GameProc *proc, u32 *state, void *param_, void *work);
void func_ov208_021bcc80(TCB *tcb, void *data);
void func_ov208_021bcc84(ZukanDetailParam *param, ZukanDetailWork *wk);

// Black's heap for the Pokédex's detail screen
#define HEAPID_ZUKAN_DETAIL 0x95

// pokebw2's ZUKAN_DETAIL_PROC_FUNCTIONS
// pokebw2's ZukanDetail_Exit
// pokebw2's ZukanDetail_Init
// pokebw2's ZukanDetail_Main
const GameProcFunctions data_ov208_021c8108 = { func_ov208_021bca68, func_ov208_021bcbb4, func_ov208_021bcb4c };

// pokebw2's ZukanDetail_VBlank


BOOL func_ov208_021bca68(GameProc *proc, u32 *state, void *param_, void *work) {
    ZukanDetailParam *param = param_;
    ZukanDetailWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_ZUKAN_DETAIL, 0x90000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(ZukanDetailWork), HEAPID_ZUKAN_DETAIL);
    MI_CpuFill8(wk, 0, sizeof(ZukanDetailWork));
    wk->heapId = HEAPID_ZUKAN_DETAIL;
    param->result = ZUKAN_DETAIL_RESULT_CLOSE;

    wk->graphic = func_ov208_021bce10(0, wk->heapId, TRUE);
    func_ov208_021bcec0(wk->graphic);
    wk->graphic = func_ov208_021bce10(0, wk->heapId, FALSE);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, wk->heapId);
    wk->printQueue = func_0201c440(wk->heapId);
    wk->vblankTcb = GFL_VBlankTCBAdd(func_ov208_021bcc80, wk, 1);
    wk->touchbar = func_ov208_021bd8c0(wk->heapId, func_0200ca54(GameData_GetPokedex(param->gameData)));
    wk->headbar = func_ov208_021be710(wk->heapId, wk->font);
    wk->common = func_ov208_021bc35c(wk->heapId, param->gameData, wk->graphic, wk->font, wk->printQueue,
        wk->touchbar, wk->headbar, param->list, param->count, &param->index);
    wk->procSys = NULL;
    wk->pageParam = NULL;
    func_ov208_021bcc84(param, wk);
    wk->nextPage = ZUKAN_DETAIL_PAGE_NONE;

    GFL_FadeSet(3, 0, 0, 0);
    func_0203d34c(TRUE, wk->heapId);
    return TRUE;
}

BOOL func_ov208_021bcb4c(GameProc *proc, u32 *state, void *param, void *work) {
    ZukanDetailWork *wk = work;

    GFL_FadeSet(3, 16, 16, 0);
    if (wk->procSys != NULL) {
        func_ov208_021bc26c(wk->procSys);
    }
    if (wk->pageParam != NULL) {
        GFL_HeapFree(wk->pageParam);
    }
    func_ov208_021bc39c(wk->common);
    func_ov208_021be778(wk->headbar);
    func_ov208_021bd954(wk->touchbar);
    GFL_TCBRemove(wk->vblankTcb);
    func_0201c6ec(wk->printQueue);
    func_0201c4c0(wk->printQueue);
    GFL_FontFree(wk->font);
    func_ov208_021bcec0(wk->graphic);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_ZUKAN_DETAIL);
    return TRUE;
}

static BOOL func_ov208_021bcbb4(GameProc *proc, u32 *state, void *param_, void *work) {
    ZukanDetailParam *param = param_;
    ZukanDetailWork *wk = work;
    BOOL pageChanged = FALSE;
    int command;

    if ((int)*state <= 8) {
        (*state)++;
    }

    if (func_ov208_021bc274(wk->procSys, wk->common)) {
        if (wk->nextPage != ZUKAN_DETAIL_PAGE_NONE) {
            param->page = wk->nextPage;
            func_ov208_021bcc84(param, wk);
            wk->nextPage = ZUKAN_DETAIL_PAGE_NONE;
            pageChanged = TRUE;
        } else {
            return TRUE;
        }
    }

    if (!pageChanged) {
        command = func_ov208_021bdb08(wk->touchbar);
        if (command == ZUKAN_DETAIL_CMD_NONE) {
            command = func_ov208_021bdb54(wk->touchbar);
        }
        func_ov208_021bc2f0(wk->procSys, wk->common, command);
        switch (command) {
        case ZUKAN_DETAIL_CMD_CLOSE:
            param->result = ZUKAN_DETAIL_RESULT_CLOSE;
            break;
        case ZUKAN_DETAIL_CMD_RETURN:
            param->result = ZUKAN_DETAIL_RESULT_RETURN;
            break;
        case ZUKAN_DETAIL_CMD_INFO:
            wk->nextPage = ZUKAN_DETAIL_PAGE_INFO;
            break;
        case ZUKAN_DETAIL_CMD_MAP:
            wk->nextPage = ZUKAN_DETAIL_PAGE_MAP;
            break;
        case ZUKAN_DETAIL_CMD_VOICE:
            wk->nextPage = ZUKAN_DETAIL_PAGE_VOICE;
            break;
        case ZUKAN_DETAIL_CMD_FORM:
            wk->nextPage = ZUKAN_DETAIL_PAGE_FORM;
            break;
        case ZUKAN_DETAIL_CMD_NONE:
        case ZUKAN_DETAIL_CMD_CUR_D:
        case ZUKAN_DETAIL_CMD_CUR_U:
        case ZUKAN_DETAIL_CMD_CHECK:
            break;
        }
    }

    func_ov208_021bd974(wk->touchbar);
    func_ov208_021be7a4(wk->headbar);
    func_0201c4e4(wk->printQueue);
    func_ov208_021bcf18(wk->graphic);
    func_ov208_021bcf2c(wk->graphic);
    if (!pageChanged) {
        func_ov208_021bc310(wk->procSys, wk->common);
    }
    func_ov208_021bcf3c(wk->graphic);
    return FALSE;
}


void func_ov208_021bcc80(TCB *tcb, void *data) {
}

void func_ov208_021bcc84(ZukanDetailParam *param, ZukanDetailWork *wk) {
    const ZukanDetailProcFuncs *funcs;

    if (wk->procSys != NULL) {
        func_ov208_021bc26c(wk->procSys);
    }
    if (wk->pageParam != NULL) {
        GFL_HeapFree(wk->pageParam);
    }

    switch (param->page) {
    case ZUKAN_DETAIL_PAGE_NONE:
        break;
    case ZUKAN_DETAIL_PAGE_INFO: {
        ZukanDetailInfoParam *pageParam =
            GFL_HeapAllocate(wk->heapId, sizeof(ZukanDetailInfoParam), TRUE, "zukan_detail.c", 451);
        func_ov208_021bea00(pageParam, wk->heapId);
        wk->pageParam = pageParam;
        funcs = &data_ov208_021c8508;
        break;
    }
    case ZUKAN_DETAIL_PAGE_MAP: {
        ZukanDetailMapParam *pageParam =
            GFL_HeapAllocate(wk->heapId, sizeof(ZukanDetailMapParam), TRUE, "zukan_detail.c", 459);
        func_ov208_021bf784(pageParam, wk->heapId);
        wk->pageParam = pageParam;
        funcs = &data_ov208_021c8590;
        break;
    }
    case ZUKAN_DETAIL_PAGE_VOICE: {
        ZukanDetailVoiceParam *pageParam =
            GFL_HeapAllocate(wk->heapId, sizeof(ZukanDetailVoiceParam), TRUE, "zukan_detail.c", 467);
        func_ov208_021c255c(pageParam, wk->heapId);
        wk->pageParam = pageParam;
        funcs = &data_ov208_021c86f0;
        break;
    }
    case ZUKAN_DETAIL_PAGE_FORM: {
        ZukanDetailFormParam *pageParam =
            GFL_HeapAllocate(wk->heapId, sizeof(ZukanDetailFormParam), TRUE, "zukan_detail.c", 475);
        func_ov208_021c3b68(pageParam, wk->heapId);
        wk->pageParam = pageParam;
        funcs = &data_ov208_021c881c;
        break;
    }
    }

    wk->procSys = func_ov208_021bc240(funcs, wk->pageParam, wk->heapId);
}


