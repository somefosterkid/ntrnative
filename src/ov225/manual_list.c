// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov319/manual_list.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/manual.h"
#include "app/manual_common.h"
#include "app/manual_graphic.h"
#include "app/manual_list.h"
#include "app/manual_touchbar.h"
#include "constants/sound.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nnsys/gfd.h"
#include "system/app_taskmenu.h"
#include "system/printsys.h"

// The manual's list of buttons, named after the ROM's "manual_list.c". Six buttons show at once, with a scroll bar
// at the right when the list has more. The BG is moved 12 pixels down, and the buttons are drawn from one above the
// first to one below the last, so that the neighbours of the six peek in at the edges

// The buttons that fit, and the windows that draw them and the two that peek in
#define VISIBLE_COUNT 6
#define WIN_COUNT 8

#define SCROLL_BAR_X 240
#define SCROLL_BAR_TOP 17
#define SCROLL_BAR_BOTTOM 135
// The part of the screen that the scroll bar is dragged over
#define SCROLL_AREA_TOP 25
#define SCROLL_AREA_BOTTOM 144
#define SCROLL_AREA_HEIGHT (SCROLL_AREA_BOTTOM - SCROLL_AREA_TOP)

#define BUTTON_COLOR 0x39e3
#define EDGE_BUTTON_COLOR 0x3da3

enum {
    STATE_IDLE,
    STATE_END,
    STATE_SCROLL_BAR,
    STATE_TOUCH_BAR,
    STATE_DECIDE,
};

struct ManualList {
    ManualListParam *param;
    ManualCommon *common;
    AppTaskMenuRes *res;
    AppTaskMenuWin *wins[WIN_COUNT];
    BOOL shown[WIN_COUNT];
    u16 color;
    u32 chars;
    u32 palette;
    u32 cellAnims;
    ClActor *scrollBar;
    ClActor *marks[WIN_COUNT];
    TCB *vblankTask;
    u32 state;
    BOOL dragging;
    // Set once dragging the scroll bar has put the cursor on the first button: it then stays there
    BOOL cursorFollows;
    // Counts down to the end once the list is left
    s8 endTimer;
    BOOL created;
};

static void func_ov225_021ba894(TCB *tcb, void *data);
static void func_ov225_021ba898(ManualList *list);
static void func_ov225_021ba9d0(ManualList *list);
static void func_ov225_021baa44(ManualList *list);
static void func_ov225_021baa50(ManualList *list);
static void func_ov225_021baa88(ManualList *list);
static BOOL func_ov225_021baabc(ManualList *list);
static BOOL func_ov225_021bae2c(ManualList *list);
static void func_ov225_021baff8(TouchRect *rects);
static u16 func_ov225_021bb034(ManualList *list);
static u16 func_ov225_021bb044(ManualList *list);
static u16 func_ov225_021bb05c(ManualList *list);
static u16 func_ov225_021bb074(ManualList *list);
static void func_ov225_021bb088(ManualList *list);
static u16 func_ov225_021bb1f4(ManualList *list, u16 count, u16 y);
static u16 func_ov225_021bb258(ManualList *list, u16 count, u16 scroll);

// pokebw2's ManualList_Create
ManualList *func_ov225_021ba65c(ManualListParam *param, ManualCommon *common) {
    u8 i;
    ClActorPos pos;
    u32 mode;
    u16 y;
    ManualList *list = GFL_HeapAllocate(common->heapId, sizeof(ManualList), TRUE, "manual_list.c", 223);

    list->param = param;
    param->result = MANUAL_LIST_RESULT_RETURN;
    list->common = common;
    for (i = 0; i < WIN_COUNT; i++) {
        list->wins[i] = NULL;
        list->shown[i] = FALSE;
    }
    list->created = FALSE;
    func_ov225_021ba898(list);
    func_ov225_021baa44(list);
    GFL_BGSysMoveBGReq(5, BG_MOVE_SET_Y, 12);
    func_0204b3dc(list->scrollBar, list->param->count > VISIBLE_COUNT);
    y = func_ov225_021bb258(list, func_ov225_021bb074(list) + 1, list->param->scroll);
    pos.x = SCROLL_BAR_X;
    pos.y = y;
    func_0204b404(list->scrollBar, &pos, 1);
    list->vblankTask = GFL_VBlankTCBAdd(func_ov225_021ba894, list, 1);
    list->state = STATE_IDLE;
    list->dragging = FALSE;
    list->cursorFollows = FALSE;
    list->endTimer = -1;
    mode = MANUAL_TOUCHBAR_MODE_TITLE;
    if (list->param->type == MANUAL_LIST_CATEGORY) {
        mode = MANUAL_TOUCHBAR_MODE_CATEGORY;
    } else if (list->param->type == MANUAL_LIST_TITLE) {
        mode = MANUAL_TOUCHBAR_MODE_TITLE;
    }
    func_ov225_021ba068(list->common->touchBar, mode);
    return list;
}

// pokebw2's ManualList_Free
void func_ov225_021ba740(ManualList *list) {
    GFL_TCBRemove(list->vblankTask);
    GFL_BGSysMoveBGReq(5, BG_MOVE_SET_Y, 0);
    GFL_HeapFree(list);
}

// pokebw2's ManualList_Main
BOOL func_ov225_021ba760(ManualList *list) {
    BOOL ended = FALSE;
    u8 i;

    if (!func_0201c6b4(list->common->queue)) {
        return ended;
    }
    if (list->endTimer == 0) {
        ended = TRUE;
    } else if (list->endTimer > 0) {
        if (list->endTimer == 1) {
            func_ov225_021baa50(list);
        }
        list->endTimer--;
    }
    if (list->state != STATE_END) {
        for (i = 0; i < WIN_COUNT; i++) {
            if (list->wins[i] != NULL && list->shown[i]) {
                func_02028178(list->wins[i]);
            }
        }
    }
    if (list->state == STATE_IDLE || list->state == STATE_SCROLL_BAR) {
        if (func_ov225_021bae2c(list)) {
            list->state = STATE_SCROLL_BAR;
        } else {
            list->state = STATE_IDLE;
        }
    }
    if (list->state == STATE_IDLE || list->state == STATE_TOUCH_BAR) {
        func_ov225_021b9fbc(list->common->touchBar);
        if (list->state == STATE_IDLE) {
            if (func_ov225_021ba158(list->common->touchBar) != -1) {
                list->state = STATE_TOUCH_BAR;
            }
        } else {
            u32 decided = func_ov225_021ba154(list->common->touchBar);

            if (decided != -1) {
                if (decided == MANUAL_TOUCHBAR_ICON_RETURN) {
                    list->param->result = MANUAL_LIST_RESULT_RETURN;
                }
                list->state = STATE_END;
                list->endTimer = 1;
            }
        }
    }
    if (list->state == STATE_IDLE || list->state == STATE_DECIDE) {
        if (list->state == STATE_IDLE) {
            if (func_ov225_021baabc(list)) {
                list->state = STATE_DECIDE;
            }
        } else {
            if (func_02028230(list->wins[func_ov225_021bb034(list)])) {
                list->param->result = MANUAL_LIST_RESULT_DECIDE;
                list->state = STATE_END;
                list->endTimer = 1;
            }
        }
    }
    return ended;
}

// pokebw2's ManualList_VBlank
static void func_ov225_021ba894(TCB *tcb, void *data) {
}

// pokebw2's ManualList_LoadGraphics
static void func_ov225_021ba898(ManualList *list) {
    u8 i;

    GFL_G2DIOLoadArcNCLRDefault(list->common->arc, MANUAL_ARC_BG_NCLR, PALTYPE_SUB_BG, 0, 2 * 0x20,
                                list->common->heapId);
    GFL_BGSysLoadArcNCGRStatic(list->common->arc, MANUAL_ARC_BG_NCGR, 4, 0, 0, FALSE, list->common->heapId);
    GFL_G2DIOLoadNSCRAsync(list->common->arc, MANUAL_ARC_LIST_NSCR, 4, 0, 0, 0, FALSE, list->common->heapId);
    list->palette =
        func_0204ac18(list->common->arc, MANUAL_ARC_OBJ_NCLR, CLACT_VRAM_SUB, 0, 0, 1, list->common->heapId);
    list->chars = func_0204a6c8(list->common->arc, MANUAL_ARC_OBJ_NCGR, FALSE, CLACT_VRAM_SUB, list->common->heapId);
    list->cellAnims = func_0204af28(list->common->arc, MANUAL_ARC_OBJ_NCER, MANUAL_ARC_OBJ_NANR, list->common->heapId);
    {
        ClActorSetup setup = { SCROLL_BAR_X, SCROLL_BAR_TOP, 0, 0, 3 };

        list->scrollBar = func_0204b294(func_ov225_021b9a08(list->common->graphic), list->chars, list->palette,
                                        list->cellAnims, &setup, 1, list->common->heapId);
        func_0204b3dc(list->scrollBar, FALSE);
        func_0204b92c(list->scrollBar, TRUE);
    }
    {
        ClActorSetup setup = { 0, -8, 1, 0, 2 };

        for (i = 0; i < WIN_COUNT; i++) {
            setup.y = i * 24 - 8;
            list->marks[i] = func_0204b294(func_ov225_021b9a08(list->common->graphic), list->chars,
                                           list->palette, list->cellAnims, &setup, 1, list->common->heapId);
            func_0204b3dc(list->marks[i], FALSE);
            func_0204b92c(list->marks[i], TRUE);
        }
    }
    func_ov225_021ba9d0(list);
}

// pokebw2's ManualList_CreateMenu
static void func_ov225_021ba9d0(ManualList *list) {
    GFL_BGSysClearBG(5);
    list->res = func_02027f60(5, 4, list->common->font, list->common->queue, list->common->heapId);
    list->color = 0x1ce7;
    NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_SUB, (4 * 16 + 13) * 2, &list->color, 2);
    NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_SUB, (5 * 16 + 13) * 2, &list->color, 2);
    func_ov225_021bb088(list);
    if (!func_020355c4()) {
        func_02028218(list->wins[func_ov225_021bb034(list)], TRUE);
        list->cursorFollows = FALSE;
    }
}

// pokebw2's ManualList_QueueScreen
static void func_ov225_021baa44(ManualList *list) {
    GFL_BGSysQueueScrLoad(4);
}

// pokebw2's ManualList_FreeGraphics
static void func_ov225_021baa50(ManualList *list) {
    u8 i;

    func_ov225_021baa88(list);
    for (i = 0; i < WIN_COUNT; i++) {
        func_0204b3b4(list->marks[i]);
    }
    func_0204b3b4(list->scrollBar);
    func_0204afd8(list->cellAnims);
    func_0204a8d4(list->chars);
    func_0204ada4(list->palette);
}

// pokebw2's ManualList_FreeMenu
static void func_ov225_021baa88(ManualList *list) {
    u8 i;

    func_0201c6ec(list->common->queue);
    for (i = 0; i < WIN_COUNT; i++) {
        if (list->wins[i] != NULL) {
            func_02028148(list->wins[i]);
        }
    }
    func_02027fd8(list->res);
    GFL_BGSysQueueScrLoad(5);
}

// Moves the cursor with the keys, or picks a button by touch. Returns whether a button was picked
// pokebw2's ManualList_UpdateCursor
static BOOL func_ov225_021baabc(ManualList *list) {
    TouchRect rects[VISIBLE_COUNT + 1];
    s32 touched;
    u16 visible;
    u16 last;

    func_ov225_021baff8(rects);
    touched = func_02035c08(rects);
    visible = func_ov225_021bb05c(list);
    last = func_ov225_021bb044(list);
    if (!func_020355c4()) {
        if (touched >= 0 && touched < visible) {
            func_020355d8(TRUE);
            func_02028218(list->wins[func_ov225_021bb034(list)], FALSE);
        }
    } else if (GCTX_HIDGetPressedKeys() != 0) {
        if (!(GCTX_HIDGetPressedKeys() & PAD_BUTTON_B)) {
            func_020355d8(FALSE);
            func_02028218(list->wins[func_ov225_021bb034(list)], TRUE);
            list->cursorFollows = FALSE;
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return FALSE;
        }
    }
    if (!func_020355c4()) {
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
            func_0202822c(list->wins[func_ov225_021bb034(list)], TRUE);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            return TRUE;
        }
        if (list->param->count > 1) {
            if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
                func_02028218(list->wins[func_ov225_021bb034(list)], FALSE);
                if (list->param->cursor > list->param->scroll) {
                    list->param->cursor--;
                } else if (list->param->count <= VISIBLE_COUNT) {
                    list->param->cursor = list->param->count - 1;
                } else {
                    ClActorPos pos;
                    u16 y;

                    if (list->param->cursor != 0) {
                        list->param->scroll--;
                        list->param->cursor--;
                    } else {
                        list->param->scroll = func_ov225_021bb074(list);
                        list->param->cursor = list->param->count - 1;
                    }
                    func_ov225_021bb088(list);
                    y = func_ov225_021bb258(list, func_ov225_021bb074(list) + 1, list->param->scroll);
                    pos.x = SCROLL_BAR_X;
                    pos.y = y;
                    func_0204b404(list->scrollBar, &pos, 1);
                }
                func_02028218(list->wins[func_ov225_021bb034(list)], TRUE);
                list->cursorFollows = FALSE;
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
                func_02028218(list->wins[func_ov225_021bb034(list)], FALSE);
                if (list->param->cursor < last) {
                    list->param->cursor++;
                } else if (list->param->count <= VISIBLE_COUNT) {
                    list->param->cursor = 0;
                } else {
                    ClActorPos pos;
                    u16 y;

                    if (list->param->cursor < list->param->count - 1) {
                        list->param->scroll++;
                        list->param->cursor++;
                    } else {
                        list->param->scroll = 0;
                        list->param->cursor = 0;
                    }
                    func_ov225_021bb088(list);
                    y = func_ov225_021bb258(list, func_ov225_021bb074(list) + 1, list->param->scroll);
                    pos.x = SCROLL_BAR_X;
                    pos.y = y;
                    func_0204b404(list->scrollBar, &pos, 1);
                }
                func_02028218(list->wins[func_ov225_021bb034(list)], TRUE);
                list->cursorFollows = FALSE;
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            } else if (GCTX_HIDGetTypedKeys() & (PAD_BUTTON_L | PAD_KEY_LEFT)) {
                if (list->param->cursor != 0) {
                    func_02028218(list->wins[func_ov225_021bb034(list)], FALSE);
                    if (list->param->count <= VISIBLE_COUNT) {
                        list->param->cursor = 0;
                    } else {
                        ClActorPos pos;
                        u16 y;

                        if (list->param->scroll >= VISIBLE_COUNT) {
                            list->param->scroll -= VISIBLE_COUNT;
                            list->param->cursor -= VISIBLE_COUNT;
                        } else {
                            list->param->scroll = 0;
                            if (list->param->cursor >= VISIBLE_COUNT) {
                                list->param->cursor -= VISIBLE_COUNT;
                            } else {
                                list->param->cursor = 0;
                            }
                        }
                        func_ov225_021bb088(list);
                        y = func_ov225_021bb258(list, func_ov225_021bb074(list) + 1, list->param->scroll);
                        pos.x = SCROLL_BAR_X;
                        pos.y = y;
                        func_0204b404(list->scrollBar, &pos, 1);
                    }
                    func_02028218(list->wins[func_ov225_021bb034(list)], TRUE);
                    list->cursorFollows = FALSE;
                    GFL_SndSEPlay(SEQ_SE_SELECT1);
                }
            } else if (GCTX_HIDGetTypedKeys() & (PAD_BUTTON_R | PAD_KEY_RIGHT)) {
                if (list->param->cursor < list->param->count - 1) {
                    func_02028218(list->wins[func_ov225_021bb034(list)], FALSE);
                    if (list->param->count <= VISIBLE_COUNT) {
                        list->param->cursor = list->param->count - 1;
                    } else {
                        ClActorPos pos;
                        u16 y;

                        list->param->scroll += VISIBLE_COUNT;
                        if (list->param->scroll > func_ov225_021bb074(list)) {
                            list->param->scroll = func_ov225_021bb074(list);
                        }
                        list->param->cursor += VISIBLE_COUNT;
                        if (list->param->cursor >= list->param->count) {
                            list->param->cursor = list->param->count - 1;
                        }
                        func_ov225_021bb088(list);
                        y = func_ov225_021bb258(list, func_ov225_021bb074(list) + 1, list->param->scroll);
                        pos.x = SCROLL_BAR_X;
                        pos.y = y;
                        func_0204b404(list->scrollBar, &pos, 1);
                    }
                    func_02028218(list->wins[func_ov225_021bb034(list)], TRUE);
                    list->cursorFollows = FALSE;
                    GFL_SndSEPlay(SEQ_SE_SELECT1);
                }
            }
        }
    } else {
        if (touched >= 0 && touched < visible) {
            list->param->cursor = list->param->scroll + touched;
            func_02028218(list->wins[func_ov225_021bb034(list)], TRUE);
            list->cursorFollows = FALSE;
            func_0202822c(list->wins[func_ov225_021bb034(list)], TRUE);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            return TRUE;
        }
    }
    return FALSE;
}

// Drags the scroll bar. Returns whether it is being dragged
// pokebw2's ManualList_UpdateScrollBar
static BOOL func_ov225_021bae2c(ManualList *list) {
    if (!func_0204b3f4(list->scrollBar)) {
        return FALSE;
    }
    if (list->dragging) {
        u32 x;
        u32 y;

        if (func_02035cb8(&x, &y)) {
            ClActorPos pos;
            u16 scroll;

            func_020355d8(TRUE);
            func_02028218(list->wins[func_ov225_021bb034(list)], FALSE);
            pos.x = SCROLL_BAR_X;
            pos.y = y < 8 ? 0 : y - 8;
            if (pos.y < SCROLL_BAR_TOP) {
                pos.y = SCROLL_BAR_TOP;
            } else if (pos.y > SCROLL_BAR_BOTTOM) {
                pos.y = SCROLL_BAR_BOTTOM;
            }
            func_0204b404(list->scrollBar, &pos, 1);
            scroll = func_ov225_021bb1f4(list, func_ov225_021bb074(list) + 1, y);
            if (scroll != list->param->scroll) {
                u16 last;

                list->param->scroll = scroll;
                last = func_ov225_021bb044(list);
                if (list->cursorFollows || list->param->cursor < list->param->scroll || last < list->param->cursor) {
                    list->param->cursor = list->param->scroll;
                    list->cursorFollows = TRUE;
                }
                func_ov225_021bb088(list);
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
        } else {
            list->dragging = FALSE;
        }
        return TRUE;
    } else {
        u32 x;
        u32 y;

        if (func_02035d20(&x, &y)) {
            if (x >= SCROLL_BAR_X && x <= SCROLL_BAR_X + 7 && y >= SCROLL_BAR_TOP && y <= 150) {
                ClActorPos pos;
                u16 scroll;

                func_020355d8(TRUE);
                func_02028218(list->wins[func_ov225_021bb034(list)], FALSE);
                GFL_SndSEPlay(SEQ_SE_SELECT1);
                list->dragging = TRUE;
                pos.x = SCROLL_BAR_X;
                pos.y = y < 8 ? 0 : y - 8;
                if (pos.y < SCROLL_BAR_TOP) {
                    pos.y = SCROLL_BAR_TOP;
                } else if (pos.y > SCROLL_BAR_BOTTOM) {
                    pos.y = SCROLL_BAR_BOTTOM;
                }
                func_0204b404(list->scrollBar, &pos, 1);
                scroll = func_ov225_021bb1f4(list, func_ov225_021bb074(list) + 1, y);
                if (scroll != list->param->scroll) {
                    u16 last;

                    list->param->scroll = scroll;
                    last = func_ov225_021bb044(list);
                    if (list->cursorFollows || list->param->cursor < list->param->scroll ||
                        last < list->param->cursor) {
                        list->param->cursor = list->param->scroll;
                        list->cursorFollows = TRUE;
                    }
                    func_ov225_021bb088(list);
                }
            }
        }
    }
    if (list->dragging) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ManualList_MakeTouchRects
static void func_ov225_021baff8(TouchRect *rects) {
    u8 i;

    for (i = 0; i < VISIBLE_COUNT; i++) {
        rects[i].top = i * 24 + 12;
        rects[i].bottom = (i + 1) * 24 + 11;
        rects[i].left = 16;
        rects[i].right = 239;
    }
    rects[VISIBLE_COUNT].top = TOUCH_RECT_END;
    rects[VISIBLE_COUNT].bottom = 0;
    rects[VISIBLE_COUNT].left = 0;
    rects[VISIBLE_COUNT].right = 0;
}

// The window of the cursor's button
// pokebw2's ManualList_GetCursorWin
static u16 func_ov225_021bb034(ManualList *list) {
    return list->param->cursor - list->param->scroll + 1;
}

// pokebw2's ManualList_GetLastVisible
static u16 func_ov225_021bb044(ManualList *list) {
    u16 last = list->param->scroll + VISIBLE_COUNT - 1;

    if (last >= list->param->count) {
        last = list->param->count - 1;
    }
    return last;
}

// pokebw2's ManualList_GetVisibleCount
static u16 func_ov225_021bb05c(ManualList *list) {
    return func_ov225_021bb044(list) - list->param->scroll + 1;
}

// pokebw2's ManualList_GetMaxScroll
static u16 func_ov225_021bb074(ManualList *list) {
    if (list->param->count <= VISIBLE_COUNT) {
        return 0;
    }
    return list->param->count - VISIBLE_COUNT;
}

// Draws the buttons from the one above the first to the one below the last, and clears the windows left over
// pokebw2's ManualList_Print
static void func_ov225_021bb088(ManualList *list) {
    u8 i = 0;
    u8 j;
    u8 lastWin;
    u16 item = list->param->scroll - 1;
    u16 last;
    AppTaskMenuItem menuItem;

    if (list->param->scroll == 0) {
        if (list->wins[0] != NULL) {
            func_0202843c(list->wins[0]);
            list->shown[0] = FALSE;
        }
        i = 1;
        item = list->param->scroll;
        func_0204b3dc(list->marks[0], FALSE);
    }
    last = func_ov225_021bb044(list) + 1;
    if (last >= list->param->count) {
        last--;
    }
    lastWin = i + last - item;
    for (j = lastWin + 1; j <= WIN_COUNT - 1; j++) {
        if (list->wins[j] != NULL) {
            func_0202843c(list->wins[j]);
            list->shown[j] = FALSE;
        }
        func_0204b3dc(list->marks[j], FALSE);
    }
    for (; i <= lastWin; i++) {
        if (list->wins[i] != NULL) {
            func_0202815c(list->wins[i]);
        }
        menuItem.str = GFL_MsgDataLoadStrbufNew(list->common->msgManual, list->param->items[item].msg);
        if (i == 0 || i == WIN_COUNT - 1) {
            menuItem.color = EDGE_BUTTON_COLOR;
        } else {
            menuItem.color = BUTTON_COLOR;
        }
        menuItem.type = 0;
        if (list->created) {
            list->wins[i] =
                func_0202804c(list->res, &menuItem, 2, i * 3, 28, 3, 0, FALSE, list->common->heapId);
        } else {
            list->wins[i] = func_02027fec(list->res, &menuItem, 2, i * 3, 28, list->common->heapId);
        }
        func_02028178(list->wins[i]);
        list->shown[i] = TRUE;
        GFL_StrBufFree(menuItem.str);
        if (list->param->items[item].mark == MANUAL_LIST_MARK_NONE) {
            func_0204b3dc(list->marks[i], FALSE);
        } else if (list->param->items[item].mark == MANUAL_LIST_MARK_NEW) {
            func_0204b3dc(list->marks[i], TRUE);
        }
        item++;
    }
    list->created = TRUE;
}

// The scroll position, out of count, of a point of the scroll area
// pokebw2's ManualList_GetScrollAt
static u16 func_ov225_021bb1f4(ManualList *list, u16 count, u16 y) {
    u16 i;

    if (count <= 1) {
        i = 0;
    } else if (y < SCROLL_AREA_TOP) {
        i = 0;
    } else if (y >= SCROLL_AREA_BOTTOM) {
        i = count - 1;
    } else {
        for (i = 0; i < count; i++) {
            u16 bottom = (i + 1) * SCROLL_AREA_HEIGHT / count + SCROLL_AREA_TOP;
            u16 top = i * SCROLL_AREA_HEIGHT / count + SCROLL_AREA_TOP;

            if (top <= y && y < bottom) {
                break;
            }
        }
    }
    if (i >= count) {
        i = 0;
    }
    return i;
}

// Where the scroll bar goes for a scroll position, out of count
// pokebw2's ManualList_GetScrollBarY
static u16 func_ov225_021bb258(ManualList *list, u16 count, u16 scroll) {
    u16 y;

    if (scroll == 0) {
        y = SCROLL_BAR_TOP;
    } else if (scroll == count - 1) {
        y = SCROLL_BAR_BOTTOM;
    } else {
        u16 top = scroll * SCROLL_AREA_HEIGHT / count + SCROLL_AREA_TOP;
        u16 bottom = (scroll + 1) * SCROLL_AREA_HEIGHT / count + SCROLL_AREA_TOP;

        y = (top + bottom) / 2;
        y -= 8;
    }
    if (y < SCROLL_BAR_TOP) {
        y = SCROLL_BAR_TOP;
    } else if (y > SCROLL_BAR_BOTTOM) {
        y = SCROLL_BAR_BOTTOM;
    }
    return y;
}
