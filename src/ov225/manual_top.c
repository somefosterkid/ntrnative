// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov319/manual_top.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/manual.h"
#include "app/manual_common.h"
#include "app/manual_top.h"
#include "app/manual_touchbar.h"
#include "constants/sound.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "system/app_taskmenu.h"
#include "system/printsys.h"
#include "text/system/title_7.h"

// The manual's first screen, named after the ROM's "manual_top.c": two buttons, for the categories and for all the
// titles

#define BUTTON_COUNT 2
#define BUTTON_COLOR 0x39e3

enum {
    BUTTON_CATEGORY,
    BUTTON_TITLE,
};

enum {
    STATE_IDLE,
    STATE_END,
    STATE_TOUCH_BAR,
    STATE_DECIDE,
};

struct ManualTop {
    ManualTopParam *param;
    ManualCommon *common;
    AppTaskMenuRes *res;
    AppTaskMenuWin *wins[BUTTON_COUNT];
    TCB *vblankTask;
    u32 state;
    // Counts down to the end once the screen is left
    s8 endTimer;
};

static void func_ov225_021bb3fc(TCB *tcb, void *data);
static void func_ov225_021bb400(ManualTop *top);
static void func_ov225_021bb458(ManualTop *top);
static void func_ov225_021bb4f8(ManualTop *top);
static void func_ov225_021bb504(ManualTop *top);
static void func_ov225_021bb50c(ManualTop *top);
static BOOL func_ov225_021bb53c(ManualTop *top);

// pokebw2's sManualTopTouchRects
static const TouchRect data_ov225_021bbf9c[] = {
    { 24, 47, 8, 247 },
    { 48, 71, 8, 247 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

// pokebw2's ManualTop_Create
ManualTop *func_ov225_021bb2b4(ManualTopParam *param, ManualCommon *common) {
    ManualTop *top = GFL_HeapAllocate(common->heapId, sizeof(ManualTop), TRUE, "manual_top.c", 157);

    top->param = param;
    param->result = MANUAL_TOP_RESULT_RETURN;
    top->common = common;
    func_ov225_021bb400(top);
    func_ov225_021bb4f8(top);
    top->vblankTask = GFL_VBlankTCBAdd(func_ov225_021bb3fc, top, 1);
    top->state = STATE_IDLE;
    top->endTimer = -1;
    func_ov225_021ba068(top->common->touchBar, MANUAL_TOUCHBAR_MODE_TOP);
    return top;
}

// pokebw2's ManualTop_Free
void func_ov225_021bb308(ManualTop *top) {
    GFL_TCBRemove(top->vblankTask);
    GFL_HeapFree(top);
}

// Returns whether the screen has ended
// pokebw2's ManualTop_Main
BOOL func_ov225_021bb31c(ManualTop *top) {
    BOOL ended = FALSE;
    u8 i;

    if (top->endTimer == 0) {
        ended = TRUE;
    } else if (top->endTimer > 0) {
        if (top->endTimer == 1) {
            func_ov225_021bb504(top);
        }
        top->endTimer--;
    }
    if (top->state == STATE_IDLE || top->state == STATE_TOUCH_BAR) {
        func_ov225_021b9fbc(top->common->touchBar);
        if (top->state == STATE_IDLE) {
            if (func_ov225_021ba158(top->common->touchBar) != -1) {
                top->state = STATE_TOUCH_BAR;
            }
        } else {
            u32 decided = func_ov225_021ba154(top->common->touchBar);

            if (decided != -1) {
                switch (decided) {
                case MANUAL_TOUCHBAR_ICON_RETURN:
                    top->param->result = MANUAL_TOP_RESULT_RETURN;
                    break;
                case MANUAL_TOUCHBAR_ICON_CLOSE:
                    top->param->result = MANUAL_TOP_RESULT_CLOSE;
                    break;
                }
                top->state = STATE_END;
                top->endTimer = 1;
            }
        }
    }
    if (top->state == STATE_IDLE || top->state == STATE_DECIDE) {
        for (i = 0; i < BUTTON_COUNT; i++) {
            func_02028178(top->wins[i]);
        }
        if (top->state == STATE_IDLE) {
            if (func_ov225_021bb53c(top)) {
                top->state = STATE_DECIDE;
            }
        } else {
            if (func_02028230(top->wins[top->param->cursor])) {
                if (top->param->cursor == BUTTON_CATEGORY) {
                    top->param->result = MANUAL_TOP_RESULT_CATEGORY;
                } else if (top->param->cursor == BUTTON_TITLE) {
                    top->param->result = MANUAL_TOP_RESULT_TITLE;
                }
                top->state = STATE_END;
                top->endTimer = 1;
            }
        }
    }
    return ended;
}

// pokebw2's ManualTop_VBlank
static void func_ov225_021bb3fc(TCB *tcb, void *data) {
}

// pokebw2's ManualTop_LoadGraphics
static void func_ov225_021bb400(ManualTop *top) {
    GFL_G2DIOLoadArcNCLRDefault(top->common->arc, MANUAL_ARC_BG_NCLR, PALTYPE_SUB_BG, 0, 2 * 0x20, top->common->heapId);
    GFL_BGSysLoadArcNCGRStatic(top->common->arc, MANUAL_ARC_BG_NCGR, 4, 0, 0, FALSE, top->common->heapId);
    GFL_G2DIOLoadNSCRAsync(top->common->arc, MANUAL_ARC_TOP_NSCR, 4, 0, 0, 0, FALSE, top->common->heapId);
    func_ov225_021bb458(top);
}

// pokebw2's ManualTop_CreateMenu
static void func_ov225_021bb458(ManualTop *top) {
    AppTaskMenuItem items[BUTTON_COUNT];
    u8 i;

    top->res = func_02027f60(5, 4, top->common->font, top->common->queue, top->common->heapId);
    items[BUTTON_CATEGORY].str = GFL_MsgDataLoadStrbufNew(top->common->msgSystem, Title7_Text_000);
    items[BUTTON_CATEGORY].color = BUTTON_COLOR;
    items[BUTTON_CATEGORY].type = 0;
    items[BUTTON_TITLE].str = GFL_MsgDataLoadStrbufNew(top->common->msgSystem, Title7_Text_001);
    items[BUTTON_TITLE].color = BUTTON_COLOR;
    items[BUTTON_TITLE].type = 0;
    for (i = 0; i < BUTTON_COUNT; i++) {
        top->wins[i] = func_02027fec(top->res, &items[i], 1, (i + 1) * 3, 30, top->common->heapId);
        GFL_StrBufFree(items[i].str);
    }
    if (!func_020355c4()) {
        func_02028218(top->wins[top->param->cursor], TRUE);
    }
}

// pokebw2's ManualTop_QueueScreen
static void func_ov225_021bb4f8(ManualTop *top) {
    GFL_BGSysQueueScrLoad(4);
}

// pokebw2's ManualTop_FreeGraphics
static void func_ov225_021bb504(ManualTop *top) {
    func_ov225_021bb50c(top);
}

// pokebw2's ManualTop_FreeMenu
static void func_ov225_021bb50c(ManualTop *top) {
    u8 i;

    func_0201c6ec(top->common->queue);
    for (i = 0; i < BUTTON_COUNT; i++) {
        func_02028148(top->wins[i]);
    }
    func_02027fd8(top->res);
    GFL_BGSysQueueScrLoad(5);
}

// Moves the cursor with the keys, or picks a button by touch. Returns whether a button was picked
// pokebw2's ManualTop_UpdateCursor
static BOOL func_ov225_021bb53c(ManualTop *top) {
    s32 touched = func_02035c08(data_ov225_021bbf9c);

    if (!func_020355c4()) {
        if (touched >= 0 && touched <= BUTTON_COUNT - 1) {
            func_020355d8(TRUE);
            func_02028218(top->wins[top->param->cursor], FALSE);
        }
    } else if (GCTX_HIDGetPressedKeys() != 0) {
        if (!(GCTX_HIDGetPressedKeys() & (PAD_BUTTON_B | PAD_BUTTON_X))) {
            func_020355d8(FALSE);
            func_02028218(top->wins[top->param->cursor], TRUE);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return FALSE;
        }
    }
    if (!func_020355c4()) {
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
            func_0202822c(top->wins[top->param->cursor], TRUE);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            return TRUE;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
            func_02028218(top->wins[top->param->cursor], FALSE);
            if (top->param->cursor == BUTTON_CATEGORY) {
                top->param->cursor = BUTTON_TITLE;
            } else {
                top->param->cursor = BUTTON_CATEGORY;
            }
            func_02028218(top->wins[top->param->cursor], TRUE);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
        } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
            func_02028218(top->wins[top->param->cursor], FALSE);
            if (top->param->cursor == BUTTON_TITLE) {
                top->param->cursor = BUTTON_CATEGORY;
            } else {
                top->param->cursor = BUTTON_TITLE;
            }
            func_02028218(top->wins[top->param->cursor], TRUE);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
        } else if (GCTX_HIDGetTypedKeys() & (PAD_BUTTON_L | PAD_KEY_LEFT)) {
            if (top->param->cursor == BUTTON_TITLE) {
                func_02028218(top->wins[top->param->cursor], FALSE);
                top->param->cursor = BUTTON_CATEGORY;
                func_02028218(top->wins[top->param->cursor], TRUE);
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
        } else if (GCTX_HIDGetTypedKeys() & (PAD_BUTTON_R | PAD_KEY_RIGHT)) {
            if (top->param->cursor == BUTTON_CATEGORY) {
                func_02028218(top->wins[top->param->cursor], FALSE);
                top->param->cursor = BUTTON_TITLE;
                func_02028218(top->wins[top->param->cursor], TRUE);
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
        }
    } else {
        if (touched >= 0 && touched <= BUTTON_COUNT - 1) {
            top->param->cursor = touched;
            func_02028218(top->wins[top->param->cursor], TRUE);
            func_0202822c(top->wins[top->param->cursor], TRUE);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            return TRUE;
        }
    }
    return FALSE;
}
