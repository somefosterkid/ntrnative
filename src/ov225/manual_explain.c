// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov319/manual_explain.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/manual.h"
#include "app/manual_common.h"
#include "app/manual_data.h"
#include "app/manual_explain.h"
#include "app/manual_touchbar.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "system/printsys.h"

// The pages of a title of the manual, named after the ROM's "manual_explain.c": the title's name and a page's text
// on the lower screen, and the page's picture on the upper one. The touch bar's arrows turn the pages

#define TITLE_COLOR PRINT_COLOR(15, 2, 0)
#define TEXT_COLOR PRINT_COLOR(1, 2, 0)

enum {
    WINDOW_UNUSED,
    WINDOW_TITLE,
    WINDOW_TEXT,
    WINDOW_COUNT,
};

enum {
    STATE_IDLE,
    STATE_END,
    STATE_TOUCH_BAR,
};

struct ManualExplain {
    ManualExplainParam *param;
    ManualCommon *common;
    u8 page;
    BOOL pageChanged;
    BmpWin *windows[WINDOW_COUNT];
    PrintQueue *queues[WINDOW_COUNT];
    BOOL printing[WINDOW_COUNT];
    TCB *vblankTask;
    u32 state;
    // Counts down to the end once the screen is left
    s8 endTimer;
};

static void func_ov225_021bbbf0(TCB *tcb, void *data);
static void func_ov225_021bbbf4(ManualExplain *explain);
static void func_ov225_021bbc44(ManualExplain *explain);
static void func_ov225_021bbc50(ManualExplain *explain);
static void func_ov225_021bbc54(ManualExplain *explain);
static void func_ov225_021bbcf4(ManualExplain *explain);
static void func_ov225_021bbd28(ManualExplain *explain);
static void func_ov225_021bbd74(ManualExplain *explain);

// pokebw2's sManualExplainWindows
static const ManualWindowSetup data_ov225_021bbfa8[WINDOW_COUNT] = {
    { 5, 0, 0, 1, 1, 2, FALSE, 0, 0 },
    { 5, 1, 0, 30, 2, 2, FALSE, 0, 1 },
    { 5, 0, 2, 32, 19, 2, FALSE, 4, 6 },
};

// pokebw2's ManualExplain_Create
ManualExplain *func_ov225_021bba74(ManualExplainParam *param, ManualCommon *common) {
    ManualExplain *explain = GFL_HeapAllocate(common->heapId, sizeof(ManualExplain), TRUE, "manual_explain.c", 156);

    explain->param = param;
    param->result = 0;
    explain->common = common;
    explain->page = 0;
    explain->pageChanged = FALSE;
    func_ov225_021bbbf4(explain);
    func_ov225_021bbc44(explain);
    func_ov225_021bbc54(explain);
    explain->vblankTask = GFL_VBlankTCBAdd(func_ov225_021bbbf0, explain, 1);
    explain->state = STATE_IDLE;
    explain->endTimer = -1;
    func_ov225_021ba068(explain->common->touchBar, MANUAL_TOUCHBAR_MODE_EXPLAIN);
    func_ov225_021ba0f0(explain->common->touchBar, explain->param->pageCount, explain->page);
    func_ov225_021bbd74(explain);
    return explain;
}

// pokebw2's ManualExplain_Free
void func_ov225_021bbae8(ManualExplain *explain) {
    func_ov225_021b9e80(explain->common, func_ov225_021b9cdc(explain->common->data));
    GFL_TCBRemove(explain->vblankTask);
    GFL_HeapFree(explain);
}

// Returns whether the screen has ended
// pokebw2's ManualExplain_Main
BOOL func_ov225_021bbb0c(ManualExplain *explain) {
    BOOL ended = FALSE;

    if (explain->endTimer == 0) {
        ended = TRUE;
    } else if (explain->endTimer > 0) {
        if (explain->endTimer == 1) {
            func_ov225_021bbcf4(explain);
            func_ov225_021bbc50(explain);
        }
        explain->endTimer--;
    }
    if (explain->state != STATE_END) {
        func_ov225_021bbd28(explain);
    }
    if (explain->state == STATE_IDLE || explain->state == STATE_TOUCH_BAR) {
        func_ov225_021b9fbc(explain->common->touchBar);
        if (explain->state == STATE_IDLE) {
            u32 touched = func_ov225_021ba158(explain->common->touchBar);

            if (touched != -1) {
                explain->state = STATE_TOUCH_BAR;
                if (touched == MANUAL_TOUCHBAR_ICON_PREV) {
                    if (explain->page != 0) {
                        explain->page--;
                        explain->pageChanged = TRUE;
                    }
                } else if (touched == MANUAL_TOUCHBAR_ICON_NEXT) {
                    if (explain->page < explain->param->pageCount - 1) {
                        explain->page++;
                        explain->pageChanged = TRUE;
                    }
                }
                if (explain->pageChanged) {
                    func_ov225_021bbd74(explain);
                }
            }
        } else {
            u32 decided = func_ov225_021ba154(explain->common->touchBar);

            if (decided != -1) {
                switch (decided) {
                case MANUAL_TOUCHBAR_ICON_RETURN:
                    explain->param->result = 0;
                    explain->state = STATE_END;
                    explain->endTimer = 1;
                    break;
                case MANUAL_TOUCHBAR_ICON_PREV:
                case MANUAL_TOUCHBAR_ICON_NEXT:
                    explain->state = STATE_IDLE;
                    if (explain->pageChanged) {
                        func_ov225_021ba0f0(explain->common->touchBar, explain->param->pageCount, explain->page);
                        explain->pageChanged = FALSE;
                    }
                    break;
                }
            }
        }
    }
    return ended;
}

// pokebw2's ManualExplain_VBlank
static void func_ov225_021bbbf0(TCB *tcb, void *data) {
}

// pokebw2's ManualExplain_LoadGraphics
static void func_ov225_021bbbf4(ManualExplain *explain) {
    GFL_G2DIOLoadArcNCLRDefault(explain->common->arc, MANUAL_ARC_BG_NCLR, PALTYPE_SUB_BG, 0, 2 * 0x20,
                                explain->common->heapId);
    GFL_BGSysLoadArcNCGRStatic(explain->common->arc, MANUAL_ARC_BG_NCGR, 4, 0, 0, FALSE, explain->common->heapId);
    GFL_G2DIOLoadNSCRAsync(explain->common->arc, MANUAL_ARC_EXPLAIN_NSCR, 4, 0, 0, 0, FALSE, explain->common->heapId);
}

// pokebw2's ManualExplain_QueueScreen
static void func_ov225_021bbc44(ManualExplain *explain) {
    GFL_BGSysQueueScrLoad(4);
}

// pokebw2's ManualExplain_FreeGraphics
static void func_ov225_021bbc50(ManualExplain *explain) {
}

// pokebw2's ManualExplain_CreateWindows
static void func_ov225_021bbc54(ManualExplain *explain) {
    u8 i;
    StrBuf *str;

    for (i = 0; i < WINDOW_COUNT; i++) {
        const ManualWindowSetup *setup = &data_ov225_021bbfa8[i];

        explain->windows[i] = BmpWin_CreateDynamic(setup->bg, setup->x, setup->y, setup->width, setup->height,
                                                   setup->palette, setup->fromEnd);
        GFL_BitmapFill(BmpWin_GetBitmap(explain->windows[i]), 0);
        BmpWin_FlushChar(explain->windows[i]);
        explain->printing[i] = FALSE;
    }
    explain->queues[WINDOW_UNUSED] = explain->common->titleQueue;
    explain->queues[WINDOW_TITLE] = explain->common->titleQueue;
    explain->queues[WINDOW_TEXT] = explain->common->queue;
    str = GFL_MsgDataLoadStrbufNew(explain->common->msgManual, explain->param->titleMsg);
    func_0201c724(explain->queues[WINDOW_TITLE], BmpWin_GetBitmap(explain->windows[WINDOW_TITLE]), 0, 1, str,
                  explain->common->font, TITLE_COLOR);
    GFL_StrBufFree(str);
    explain->printing[WINDOW_TITLE] = TRUE;
    func_ov225_021bbd28(explain);
}

// pokebw2's ManualExplain_FreeWindows
static void func_ov225_021bbcf4(ManualExplain *explain) {
    u8 i;

    for (i = 0; i < WINDOW_COUNT; i++) {
        explain->printing[i] = FALSE;
        func_0201c6ec(explain->queues[i]);
        func_020456f8(explain->windows[i]);
        BmpWin_Free(explain->windows[i]);
    }
    GFL_BGSysQueueScrLoad(5);
}

// Shows the windows whose text their queue has printed
// pokebw2's ManualExplain_UpdateWindows
static void func_ov225_021bbd28(ManualExplain *explain) {
    u8 i;

    for (i = 0; i < WINDOW_COUNT; i++) {
        if (explain->printing[i]) {
            if (!func_0201c6c4(explain->queues[i], BmpWin_GetBitmap(explain->windows[i]))) {
                BmpWin *window = explain->windows[i];

                BmpWin_FlushChar(window);
                BmpWin_FlushMap(window);
                GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
                explain->printing[i] = FALSE;
            }
        }
    }
}

// pokebw2's ManualExplain_PrintPage
static void func_ov225_021bbd74(ManualExplain *explain) {
    StrBuf *str;

    func_0201c6ec(explain->queues[WINDOW_TEXT]);
    GFL_BitmapFill(BmpWin_GetBitmap(explain->windows[WINDOW_TEXT]), 0);
    str = GFL_MsgDataLoadStrbufNew(explain->common->msgManual, explain->param->pages[explain->page].msg);
    func_0201c724(explain->queues[WINDOW_TEXT], BmpWin_GetBitmap(explain->windows[WINDOW_TEXT]), 4, 6, str,
                  explain->common->font, TEXT_COLOR);
    GFL_StrBufFree(str);
    explain->printing[WINDOW_TEXT] = TRUE;
    func_ov225_021bbd28(explain);
    func_ov225_021b9e80(explain->common, explain->param->pages[explain->page].image);
}
