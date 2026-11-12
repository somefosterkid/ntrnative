// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov319/manual.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/manual.h"
#include "app/manual_category.h"
#include "app/manual_common.h"
#include "app/manual_data.h"
#include "app/manual_explain.h"
#include "app/manual_list.h"
#include "app/manual_title.h"
#include "app/manual_top.h"
#include "field/event_game_manual.h"
#include "gfl/bg_sys.h"
#include "gfl/fade.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "gfl/tcb.h"

// The in-game manual's process: it goes between the top screen, the categories, the titles and a title's pages. The
// ROM has no name for this file; "manual.c" is a guess from its nine others

enum {
    SCENE_TOP,
    SCENE_CATEGORY,
    SCENE_TITLE,
    SCENE_EXPLAIN,
};

enum {
    STATE_START,
    STATE_FADE_IN,
    STATE_MAIN,
    STATE_FADE_OUT,
    STATE_END,
};

typedef struct {
    HeapID heapId;
    GameManualSubwork *param;
    ManualCommon *common;
    TCB *vblankTask;
    u32 scene;
    ManualTopParam topParam;
    ManualTop *top;
    ManualCategoryParam categoryParam;
    ManualCategory *category;
    ManualTitleParam titleParam;
    ManualTitle *title;
    ManualExplainParam explainParam;
    ManualExplain *explain;
} Manual;

static BOOL func_ov225_021b95a0(GameProc *proc, u32 *state, void *param, void *data);
static BOOL func_ov225_021b9660(GameProc *proc, u32 *state, void *param, void *data);
static BOOL func_ov225_021b96bc(GameProc *proc, u32 *state, void *param, void *data);
static void func_ov225_021b98fc(TCB *tcb, void *data);

// pokebw2's Manual_Exit
// pokebw2's Manual_Init
// pokebw2's Manual_Main
// pokebw2's gManualProcFunctions
const GameProcFunctions data_ov225_021bbde4 = { func_ov225_021b95a0, func_ov225_021b96bc, func_ov225_021b9660 };

static BOOL func_ov225_021b95a0(GameProc *proc, u32 *state, void *param, void *data) {
    Manual *work;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_MANUAL, 0x50000);
    work = GFL_ProcInitSubsystem(proc, sizeof(Manual), HEAPID_MANUAL);
    MI_CpuFill8(work, 0, sizeof(Manual));
    work->heapId = HEAPID_MANUAL;
    work->param = param;
    work->param->result = MANUAL_RESULT_RETURN;
    work->common = func_ov225_021b9d18(work->param->gameData, work->heapId);
    GFL_BGSysSetBGPriority(4, 3);
    GFL_BGSysSetBGPriority(5, 2);
    GFL_BGSysSetBGPriority(6, 1);
    GFL_BGSysSetBGPriority(7, 0);
    work->vblankTask = GFL_VBlankTCBAdd(func_ov225_021b98fc, work, 1);
    GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 0);
    func_0203d34c(FALSE, work->heapId);
    work->topParam.cursor = 0;
    work->categoryParam.scroll = 0;
    work->categoryParam.cursor = 0;
    work->titleParam.mode = MANUAL_TITLE_MODE_CATEGORY;
    work->titleParam.category = 0;
    work->titleParam.scroll = 0;
    work->titleParam.cursor = 0;
    work->explainParam.pageCount = 1;
    work->explainParam.titleMsg = 0;
    work->explainParam.pages[0].image = MANUAL_IMAGE_NONE;
    work->explainParam.pages[0].msg = 0;
    work->scene = SCENE_TOP;
    work->top = func_ov225_021bb2b4(&work->topParam, work->common);
    return TRUE;
}

static BOOL func_ov225_021b9660(GameProc *proc, u32 *state, void *param, void *data) {
    Manual *work = data;

    switch (work->scene) {
    case SCENE_TOP:
        func_ov225_021bb308(work->top);
        break;
    case SCENE_CATEGORY:
        func_ov225_021bb7f4(work->category);
        break;
    case SCENE_TITLE:
        func_ov225_021bba14(work->title);
        break;
    case SCENE_EXPLAIN:
        func_ov225_021bbae8(work->explain);
        break;
    }
    GFL_TCBRemove(work->vblankTask);
    func_ov225_021b9dec(work->common);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_MANUAL);
    return TRUE;
}

static BOOL func_ov225_021b96bc(GameProc *proc, u32 *state, void *param, void *data) {
    Manual *work = data;

    switch (*state) {
    case STATE_START:
        *state = STATE_FADE_IN;
        break;
    case STATE_FADE_IN:
        if (!GFL_FadeIsRunning()) {
            *state = STATE_MAIN;
        }
        break;
    case STATE_MAIN: {
        BOOL end = FALSE;

        switch (work->scene) {
        case SCENE_TOP:
            if (func_ov225_021bb31c(work->top)) {
                if (work->topParam.result <= MANUAL_TOP_RESULT_RETURN) {
                    if (work->topParam.result == MANUAL_TOP_RESULT_CLOSE) {
                        work->param->result = MANUAL_RESULT_CLOSE;
                    } else if (work->topParam.result == MANUAL_TOP_RESULT_RETURN) {
                        work->param->result = MANUAL_RESULT_RETURN;
                    }
                    end = TRUE;
                } else {
                    func_ov225_021bb308(work->top);
                    if (work->topParam.result == MANUAL_TOP_RESULT_CATEGORY) {
                        work->scene = SCENE_CATEGORY;
                        work->categoryParam.scroll = 0;
                        work->categoryParam.cursor = 0;
                        work->category = func_ov225_021bb6e0(&work->categoryParam, work->common);
                    } else if (work->topParam.result == MANUAL_TOP_RESULT_TITLE) {
                        work->scene = SCENE_TITLE;
                        work->titleParam.mode = MANUAL_TITLE_MODE_ALL;
                        work->titleParam.category = 0;
                        work->titleParam.scroll = 0;
                        work->titleParam.cursor = 0;
                        work->title = func_ov225_021bb854(&work->titleParam, work->common);
                    }
                }
            }
            break;
        case SCENE_CATEGORY:
            if (func_ov225_021bb844(work->category)) {
                func_ov225_021bb7f4(work->category);
                if (work->categoryParam.result == MANUAL_LIST_RESULT_RETURN) {
                    work->scene = SCENE_TOP;
                    work->top = func_ov225_021bb2b4(&work->topParam, work->common);
                } else {
                    work->scene = SCENE_TITLE;
                    work->titleParam.mode = MANUAL_TITLE_MODE_CATEGORY;
                    work->titleParam.category = work->categoryParam.selected;
                    work->titleParam.scroll = 0;
                    work->titleParam.cursor = 0;
                    work->title = func_ov225_021bb854(&work->titleParam, work->common);
                }
            }
            break;
        case SCENE_TITLE:
            if (func_ov225_021bba64(work->title)) {
                func_ov225_021bba14(work->title);
                if (work->titleParam.result == MANUAL_LIST_RESULT_RETURN) {
                    if (work->titleParam.mode == MANUAL_TITLE_MODE_CATEGORY) {
                        work->scene = SCENE_CATEGORY;
                        work->category = func_ov225_021bb6e0(&work->categoryParam, work->common);
                    } else if (work->titleParam.mode == MANUAL_TITLE_MODE_ALL) {
                        work->scene = SCENE_TOP;
                        work->top = func_ov225_021bb2b4(&work->topParam, work->common);
                    }
                } else {
                    u16 title;
                    u16 pageCount;
                    u16 flag;
                    u16 i;

                    work->scene = SCENE_EXPLAIN;
                    title = work->titleParam.selected;
                    pageCount = func_ov225_021b9bec(work->common->data, title);
                    flag = func_ov225_021b9be0(work->common->data, title);
                    if (!func_ov225_021b9c98(work->common->data, flag, work->common->gameData)) {
                        func_ov225_021b9cb0(work->common->data, flag, work->common->gameData);
                    }
                    work->explainParam.titleMsg = func_ov225_021b9bc8(work->common->data, title);
                    work->explainParam.pageCount = func_ov225_021b9bec(work->common->data, title);
                    for (i = 0; i < pageCount; i++) {
                        u16 image = func_ov225_021b9c0c(work->common->data, title, i);

                        if (!func_ov225_021b9cc8(work->common->data, image)) {
                            image = MANUAL_IMAGE_NONE;
                        }
                        work->explainParam.pages[i].image = image;
                        work->explainParam.pages[i].msg = func_ov225_021b9bf8(work->common->data, title, i);
                    }
                    work->explain = func_ov225_021bba74(&work->explainParam, work->common);
                }
            }
            break;
        case SCENE_EXPLAIN:
            if (func_ov225_021bbb0c(work->explain)) {
                func_ov225_021bbae8(work->explain);
                if (work->explainParam.result == 0) {
                    work->scene = SCENE_TITLE;
                    work->title = func_ov225_021bb854(&work->titleParam, work->common);
                }
            }
            break;
        }
        if (end) {
            *state = STATE_FADE_OUT;
            GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 0);
        }
        break;
    }
    case STATE_FADE_OUT:
        if (!GFL_FadeIsRunning()) {
            *state = STATE_END;
        }
        break;
    case STATE_END:
        return TRUE;
    }
    func_ov225_021b9e60(work->common);
    return FALSE;
}

// pokebw2's Manual_VBlank
static void func_ov225_021b98fc(TCB *tcb, void *data) {
}
