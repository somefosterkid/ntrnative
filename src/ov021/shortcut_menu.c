// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/shortcut_menu.c, ported by tools/port_file.py.
// The Y Button's shortcut menu: the list of the shortcuts registered in the save, which the player scrolls, confirms
// with Y, and reorders with SELECT
#include <stddef.h>
#include "types.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "field/field.h"
#include "field/field_event.h"
#include "field/shortcut_menu.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "gfl/ui.h"
#include "save/shortcut.h"
#include "system/bmp_menulist.h"
#include "system/bmp_menuwork.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "app/name_entry.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
BmpMenuList *BmpMenuList_Create(const BmpMenuListHeader *header, u32 listTop, u16 cursorRow, HeapID heapId);
u8 ShortcutSave_GetRegistItem(ShortcutSave *shortcutSave, u32 index);

#define BG 1
#define ACTORS 3

// What the menu does next
enum {
    STATE_CLOSED,
    STATE_OPENING,
    STATE_SCROLL_IN,
    STATE_INPUT,
    STATE_CLOSING,
    STATE_SCROLL_OUT,
};

// Black 2's; twinned code confirms the offsets of options, list, window, queue, printWindow, mode, actors, heapId,
// scrollDir, cursorMoved, showActors, unit, palette, chars, cellAnims, clactActors, menuMsgData, menuFont, menuQueue,
// state, scroll, code, item, listPos, scrolling and tcb in Black
struct ShortcutMenuApp {
    ListMenuOption *options;
    BmpMenuList *list;
    BmpWin *window;
    PrintQueue *queue;
    PrintWindow printWindow;
    u32 input;
    u16 listTop;
    u16 cursorRow;
    ShortcutSave *shortcut;
    u32 mode;
    ClActor *actors[ACTORS];
    HeapID heapId;
    MsgData *msgData;
    Font *font;
    u32 scrollDir;
    u32 cursorMoved;
    u32 showActors;
    ClActUnit *unit;
    u16 palette;
    u16 chars;
    u16 cellAnims;
    ClActor *clactActors[ACTORS];
    MsgData *menuMsgData;
    Font *menuFont;
    PrintQueue *menuQueue;
    u16 state;
    u16 scroll;
    u32 code;
    u32 item;
    u16 *listPos;
    u32 scrolling;
    TCB *tcb;
};

// What the event that puts the field back after the menu runs on
typedef struct {
    GameSystem *gsys;
    Field *field;
    void *param;
    BOOL transition;
} ShortcutMenuReturn;

// pokebw2's sBgSetup
static const BGSetup data_ov021_021daa60 = { 0, 0, 0x1000, 0, 2, 0, 0, 4, 0x8000, 0, 0, 0, 0 };

// pokebw2's sActorX
static const u32 data_ov021_021daa54[ACTORS] = { 0x80, 0xa, 0xf6 };

// pokebw2's sListHeader
static const BmpMenuListHeader data_ov021_021daa80 = {
    NULL, NULL, NULL, NULL, 0, 5, 0, 13, 0, 8, 1, 15, 2, 0, 0, BMPMENULIST_SKIP_NONE, 0, 1, NULL, 12, 16, NULL,
    NULL, NULL, NULL,
};

// The subprocess of overlay 0x118 that the menu returns through, whose work it leaves empty
static GameProcFunctions sReturnFunctions = { NULL, NULL, NULL };

static void FreeList(ShortcutMenuApp *work, u16 *listTop, u16 *cursorRow);
static void ClampListPos(u16 *listTop, u16 *cursorRow, u16 count);
static void CreateList(ShortcutMenuApp *work, u16 listTop, u16 cursorRow, HeapID heapId);
static void SetActorsY(ShortcutMenuApp *work, u32 y);
static void ScrollBgIn(TCB *tcb, void *data);
static BOOL PrintDone(ShortcutMenuApp *work);
s32 GetInput(ShortcutMenuApp *work);
static void OnListCursor(BmpMenuList *list, intptr_t value, u8 init);
static void RedrawHighlight(BmpMenuList *list, u8 a1, u8 a2);
static void OnCursorUp(ShortcutMenuApp *work, u16 value, u32 count, u32 *handled);
static void OnCursorDown(ShortcutMenuApp *work, u16 value, u32 count, u32 *handled);
static void ReadInput(ShortcutMenuApp *work);
static void SetupMenu(ShortcutMenuApp *work, u16 *listPos, MsgData *msgData, Font *font, PrintQueue *queue,
                      ShortcutSave *shortcut, ClActor **actors, HeapID heapId);
static void FreeMenu(ShortcutMenuApp *work, u16 *listPos);
void func_ov021_021c59a8(ShortcutMenuApp *work, u16 *listTop, u16 *cursorRow);
static void func_ov021_021c59f8(ShortcutMenuApp *work, s32 y);
static void func_ov021_021c577c(BmpMenuList *list, u8 a1, u8 a2);
static BOOL func_ov021_021c56f8(ShortcutMenuApp *work);
static void func_ov021_021c58cc(ShortcutMenuApp *work, u16 listTop, u16 cursorRow, HeapID heapId);
void func_ov021_021c5424(ShortcutMenuApp *work, u16 *listPos);
void func_ov021_021c59bc(TCB *tcb, void *data);
static void func_ov021_021c553c(ShortcutMenuApp *work);
u32 func_ov021_021c5768(ShortcutMenuApp *work);
ShortcutSave *func_0200c40c(SaveControl *save);
void func_ov021_021c53a0(ShortcutMenuApp *work, const u16 *listPos, MsgData *msgData, Font *font, PrintQueue *queue, ShortcutSave *shortcut, ClActor **actors, HeapID heapId);


void *func_ov021_021c4f7c(GameData *gameData, u32 mode, u16 *listPos, HeapID heapId, HeapID allocHeapId) {
    ClActorSetup setup;
    ShortcutMenuApp *work;
    ShortcutSave *shortcut;
    ArcTool *arc;
    int i;

    work = GFL_HeapAllocate(allocHeapId, sizeof(ShortcutMenuApp), FALSE, "shortcut_menu.c", 252);
    MI_CpuFill8(work, 0, sizeof(ShortcutMenuApp));
    work->code = 0;
    work->item = 0x19;
    work->listPos = listPos;
    GFL_BGSysCreateBG(BG, &data_ov021_021daa60, 0);
    GFL_BGSysClearBG(BG);
    GFL_BGSysFillChar(BG, 0, 1, 0);
    work->unit = func_0204b100(8, 0, allocHeapId);

    arc = GFL_ArcSysCreateFileHandle(0x80, allocHeapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, 0, 0x160, 0x40, allocHeapId);
    work->palette = func_0204abf0(arc, 4, 0, 0x40, allocHeapId);
    work->chars = func_0204a6c8(arc, 3, FALSE, 0, allocHeapId);
    work->cellAnims = func_0204af28(arc, 2, 1, allocHeapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 6, 0, 0x140, 0x20, allocHeapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 5, BG, 0, 0, FALSE, allocHeapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 7, BG, 0, 0, FALSE, allocHeapId);
    GFL_BGSysSetScrPaletteNo(BG, 0, 0xb, 0x20, 1, 0xa);
    GFL_ArcToolFree(arc);

    MI_CpuFill8(&setup, 0, sizeof(setup));
    setup.y = 0x60;
    for (i = 0; i < ACTORS; i++) {
        setup.sequence = i;
        setup.x = data_ov021_021daa54[i];
        work->clactActors[i] = func_0204b294(work->unit, work->chars, work->palette, work->cellAnims, &setup, 0, allocHeapId);
        func_0204b3dc(work->clactActors[i], FALSE);
        func_0204b92c(work->clactActors[i], TRUE);
    }

    work->menuMsgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_ABILITY_HANDLERS_4, allocHeapId);
    work->menuFont = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, heapId);
    work->menuQueue = func_0201c450(0x1000, allocHeapId);
    shortcut = func_0200c40c(GameData_GetSaveControl(gameData));
    func_ov021_021c53a0(work, work->listPos, work->menuMsgData, work->menuFont, work->menuQueue, shortcut, work->clactActors, heapId);
    GFL_BGSysQueueScrLoad(BG);
    G2_SetBlendAlpha(2, 1, 0xc, 4);

    switch (mode) {
    case 0:
        GFL_BGSysMoveBG(BG, BG_MOVE_SET_Y, -0xc0);
        GFL_BGSysSetBGEnabled(BG, FALSE);
        work->state = STATE_CLOSED;
        break;
    case 1:
        GFL_BGSysMoveBG(BG, BG_MOVE_SET_Y, 0);
        GFL_BGSysSetBGEnabled(BG, TRUE);
        work->state = STATE_INPUT;
        break;
    }
    return work;
}

void func_ov021_021c51a0(void *app) {
    ShortcutMenuApp *work = app;
    int i;

    reg_G2_BLDCNT = 0;
    GFL_BGSysSetBGEnabled(BG, FALSE);
    func_ov021_021c5424(work, work->listPos);
    for (i = 0; i < ACTORS; i++) {
        func_0204b3b4(work->clactActors[i]);
    }
    func_0201c6ec(work->menuQueue);
    func_0201c4c0(work->menuQueue);
    GFL_FontFree(work->menuFont);
    GFL_MsgDataFree(work->menuMsgData);
    func_0204ada4(work->palette);
    func_0204a8d4(work->chars);
    func_0204afd8(work->cellAnims);
    func_0204b1cc(work->unit);
    GFL_BGSysFreeFilledChar(BG, 1, 0);
    GFL_BGSysReleaseBG(BG);
    GFL_HeapFree(work);
}

void func_ov021_021c5224(void *app) {
    ShortcutMenuApp *work = app;
    int i;

    if (!func_ov021_021c5334(work)) {
        return;
    }
    switch (work->state) {
    case STATE_OPENING:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        work->scroll = 0;
        work->scrolling = FALSE;
        work->tcb = GFL_VBlankTCBAdd(func_ov021_021c59bc, work, 0);
        work->state = STATE_SCROLL_IN;
        break;
    case STATE_SCROLL_IN:
        if (work->scrolling) {
            GFL_TCBRemove(work->tcb);
            work->state = STATE_INPUT;
        }
        break;
    case STATE_INPUT: {
        s32 input;

        func_ov021_021c553c(work);
        input = func_ov021_021c5768(work);
        switch (input) {
        case BMPMENULIST_NULL:
            work->code = 0;
            break;
        case BMPMENULIST_CANCEL:
            work->code = 2;
            break;
        default:
            work->code = 1;
            work->item = input;
            break;
        }
        break;
    }
    case STATE_CLOSING:
        for (i = 0; i < ACTORS; i++) {
            func_0204b3dc(work->clactActors[i], FALSE);
        }
        work->scroll = 4;
        work->state = STATE_SCROLL_OUT;
        break;
    case STATE_SCROLL_OUT:
        GFL_BGSysMoveBG(BG, BG_MOVE_SET_Y, work->scroll * 0xc0 / 4 - 0xc0);
        if (work->scroll-- == 0) {
            work->state = STATE_CLOSED;
        }
        break;
    }
}

BOOL func_ov021_021c5334(void *app) {
    ShortcutMenuApp *work = app;

    func_0201c4e4(work->menuQueue);
    return func_ov021_021c56f8(work);
}


void func_ov021_021c5348(void *app) {
    ShortcutMenuApp *work = app;

    if (work->state == STATE_CLOSED) {
        work->code = 0;
        work->item = 0x19;
        GFL_BGSysSetBGEnabled(BG, TRUE);
        work->state = STATE_OPENING;
    }
}

void func_ov021_021c536c(void *app) {
    ShortcutMenuApp *work = app;

    if (work->state == STATE_INPUT) {
        work->state = STATE_CLOSING;
    }
}

BOOL func_ov021_021c5380(void *app) {
    ShortcutMenuApp *work = app;
    BOOL busy = TRUE;

    if (work->state != STATE_SCROLL_IN && work->state != STATE_SCROLL_OUT) {
        busy = FALSE;
    }
    return busy;
}

u32 func_ov021_021c5394(void *app, u32 *item) {
    ShortcutMenuApp *work = app;

    if (item != NULL) {
        *item = work->item;
    }
    return work->code;
}

void func_ov021_021c53a0(ShortcutMenuApp *work, const u16 *listPos, MsgData *msgData, Font *font, PrintQueue *queue,
                                ShortcutSave *shortcut, ClActor **actors, HeapID heapId) {
    BmpWin *window;
    int i;

    MI_CpuFill8(work, 0, offsetof(ShortcutMenuApp, unit));
    work->queue = queue;
    work->shortcut = shortcut;
    work->heapId = heapId;
    work->msgData = msgData;
    work->font = font;
    for (i = 0; i < ACTORS; i++) {
        work->actors[i] = actors[i];
    }
    window = BmpWin_CreateDynamic(BG, 1, 0xc, 0x1e, 0xc, 0xb, TRUE);
    work->window = window;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
    work->printWindow.window = work->window;
    work->printWindow.flushPending = FALSE;
    func_ov021_021c58cc(work, listPos[0], listPos[1], heapId);
}


void func_ov021_021c5424(ShortcutMenuApp *work, u16 *listPos) {
    func_ov021_021c59a8(work, listPos, listPos + 1);
    BmpWin_Free(work->window);
    MI_CpuFill8(work, 0, offsetof(ShortcutMenuApp, unit));
}

void func_ov021_021c5440(ShortcutMenuApp *work, u16 value, u32 count, u32 *handled) {
    if (value == 0) {
        s32 maxShown = (s32)func_02020350(work->list, BMPMENULIST_PARAM_MAX_SHOWN);

        func_ov021_021c59a8(work, NULL, NULL);
        func_ov021_021c58cc(work, (count - 1) - (maxShown - 1), maxShown - 1, work->heapId);
        if (work->mode == 1) {
            work->scrollDir = 1;
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        *handled = TRUE;
    } else if (work->scrollDir == 1 && work->mode == 1) {
        u16 cursorRow;
        u16 y;

        func_020202fc(work->list, NULL, &cursorRow);
        y = func_02020324(work->list, cursorRow) >> 3;
        func_ov021_021c59f8(work, (y + 0xc) * 8);
        work->scrollDir = 0;
        *handled = TRUE;
    }
}

void func_ov021_021c54c8(ShortcutMenuApp *work, u16 value, u32 count, u32 *handled) {
    if (value == count - 1) {
        if (work->mode == 1) {
            u16 cursorRow;
            u16 y;

            func_020202fc(work->list, NULL, &cursorRow);
            y = func_02020324(work->list, cursorRow + 1) >> 3;
            func_ov021_021c59f8(work, (y + 0xc) * 8);
            work->scrollDir ^= 1;
        }
        if (work->scrollDir == 0) {
            func_ov021_021c59a8(work, NULL, NULL);
            func_ov021_021c58cc(work, 0, 0, work->heapId);
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        *handled = TRUE;
    } else {
        work->scrollDir = 0;
    }
}

static void func_ov021_021c553c(ShortcutMenuApp *work) {
    u32 cursorHandled;
    u16 cursorIndex;
    u16 buttonIndex;
    u16 selectIndex;
    u16 index;
    u16 cursorRow;
    u16 listTop;
    s32 count;
    s32 result = BMPMENULIST_NULL;
    BOOL handled = FALSE;

    cursorHandled = FALSE;
    func_020355d8(FALSE);
    func_020202ec(work->list, &cursorIndex);
    count = (s32)func_02020350(work->list, BMPMENULIST_PARAM_COUNT);

    if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
        func_ov021_021c5440(work, cursorIndex, count, &cursorHandled);
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
        func_ov021_021c54c8(work, cursorIndex, count, &cursorHandled);
    }
    if (cursorHandled == 0) {
        result = (s32)BmpMenuList_Update(work->list);
    }
    if (result == BMPMENULIST_NULL) {
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
            result = BMPMENULIST_CANCEL;
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
        } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_Y) {
            func_020202ec(work->list, &buttonIndex);
            result = (s32)func_02020344(work->list, buttonIndex);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        } else if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) && work->mode == 1) {
            func_020202ec(work->list, &selectIndex);
            result = (s32)func_02020344(work->list, selectIndex);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        }
    }
    if (work->mode == 0) {
        work->input = result;
    } else if (work->mode == 1) {
        if (result == BMPMENULIST_CANCEL) {
            handled = TRUE;
        } else if (result != BMPMENULIST_NULL) {
            result = (s32)func_02020344(work->list, work->listTop + work->cursorRow);
            func_020202ec(work->list, &index);
            if ((work->scrollDir != 0)) {
                result = ShortcutSave_MoveItemToIndex(work->shortcut, result, 0xff);
            } else {
                result = ShortcutSave_MoveItemToIndex(work->shortcut, result, index);
            }
            func_ov021_021c59a8(work, &listTop, &cursorRow);
            if (index - 1 == result) {
                cursorRow--;
            }
            func_ov021_021c58cc(work, listTop, cursorRow, work->heapId);
            handled = TRUE;
        }
    }
    if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) && work->mode == 0
        && (u32)func_02020350(work->list, BMPMENULIST_PARAM_COUNT) > 1) {
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        handled = TRUE;
    }
    if (handled) {
        work->mode ^= 1;
        if (work->mode == 0) {
            int i;

            work->scrollDir = 0;
            for (i = 0; i < ACTORS; i++) {
                func_0204b3dc(work->actors[i], FALSE);
            }
            func_ov021_021c577c(work->list, FALSE, FALSE);
        } else if (work->mode == 1) {
            func_020202fc(work->list, &work->listTop, &work->cursorRow);
            work->showActors = TRUE;
        }
    }
}




static BOOL func_ov021_021c56f8(ShortcutMenuApp *work) {
    PrintQueue *queue = work->queue;
    BOOL done;

    if (work->printWindow.flushPending && !func_0201c6c4(queue, BmpWin_GetBitmap(work->printWindow.window))) {
        BmpWin_FlushChar(work->printWindow.window);
        work->printWindow.flushPending = FALSE;
    }
    done = !work->printWindow.flushPending;
    if (done) {
        int i;

        if (work->cursorMoved) {
            func_ov021_021c577c(work->list, 0, 0);
            work->cursorMoved = 0;
        }
        if (work->showActors) {
            for (i = 0; i < ACTORS; i++) {
                func_0204b3dc(work->actors[i], TRUE);
            }
            work->showActors = 0;
        }
    }
    return done;
}



u32 func_ov021_021c5768(ShortcutMenuApp *work) {
    return work->input;
}

static void func_ov021_021c576c(BmpMenuList *list, intptr_t value, u8 init) {
    ShortcutMenuApp *work = (ShortcutMenuApp *)func_02020350(list, BMPMENULIST_PARAM_WORK);

    work->cursorMoved = 1;
}

static void func_ov021_021c577c(BmpMenuList *list, u8 a1, u8 a2) {
    ShortcutMenuApp *work = (ShortcutMenuApp *)func_02020350(list, BMPMENULIST_PARAM_WORK);
    u16 cursorY = func_02020314(list) >> 3;
    u32 rowHeight = (u32)func_02020350(list, BMPMENULIST_PARAM_ROW_HEIGHT) >> 3;

    if (work->mode == 0) {
        GFL_BGSysSetScrPaletteNo(BG, 1, 0xc, 0x1e, 0xc, 0xb);
        GFL_BGSysSetScrPaletteNo(BG, 1, cursorY + 0xc, 0x1e, rowHeight, 0xc);
        GFL_BGSysSetScrPaletteNo(BG, 0, 0xb, 0x20, 1, 0xa);
        GFL_BGSysQueueScrLoad(BG);
    } else if (work->mode == 1) {
        u16 listTop;
        u16 cursorRow;
        u32 listY;
        u32 y;

        GFL_BGSysSetScrPaletteNo(BG, 1, 0xc, 0x1e, 0xc, 0xb);
        func_020202fc(list, &listTop, &cursorRow);
        listY = (s32)func_02020350(list, BMPMENULIST_PARAM_Y);
        y = listY + (s32)func_02020350(list, BMPMENULIST_PARAM_ROW_HEIGHT) * (work->cursorRow + work->listTop - listTop);
        GFL_BGSysSetScrPaletteNo(BG, 1, y / 8 + 0xc, 0x1e, rowHeight, 0xc);
        GFL_BGSysSetScrPaletteNo(BG, 0, 0xb, 0x20, 1, 0xa);
        GFL_BGSysQueueScrLoad(BG);
    }
    if (work->scrollDir == 1) {
        u16 maxShown = func_02020350(work->list, BMPMENULIST_PARAM_MAX_SHOWN);
        u16 rowY = func_02020324(work->list, maxShown) >> 3;

        func_ov021_021c59f8(work, (rowY + 0xc) * 8);
    } else {
        func_ov021_021c59f8(work, (cursorY + 0xc) * 8);
    }
}


void func_ov021_021c5880(u16 *listTop, u16 *cursorRow, u16 count) {
    s32 total = *listTop + *cursorRow;

    if (count <= 5) {
        *cursorRow = total;
        *listTop = 0;
    } else if (total <= 3) {
        *cursorRow = total;
        *listTop = 0;
    } else if (total <= (s32)count - 2 && total > 3) {
        *listTop = total - 3;
        *cursorRow = 3;
    } else if (total == count - 1) {
        *listTop = total - 4;
        *cursorRow = 4;
    }
}

static void func_ov021_021c58cc(ShortcutMenuApp *work, u16 listTop, u16 cursorRow, HeapID heapId) {
    BmpMenuListHeader header;
    u32 count = ShortcutSave_GetShortcutCount(work->shortcut);
    u32 i;

    work->options = ListMenuCore_CreateOptionList(count, heapId);
    for (i = 0; i < count; i++) {
        u32 value = func_0200c464(work->shortcut, i);

        ListMenuCore_AppendMsgOption(work->options, work->msgData, value, value, heapId);
    }
    while (cursorRow + listTop >= count) {
        if (cursorRow != 0) {
            cursorRow--;
        } else if (listTop != 0) {
            listTop--;
        }
    }
    func_ov021_021c5880(&listTop, &cursorRow, count);

    header = data_ov021_021daa80;
    header.options = work->options;
    header.count = count;
    header.cursorCallback = func_ov021_021c576c;
    header.printCallback = NULL;
    header.window = work->window;
    header.work = work;
    header.window = work->window;
    header.msgData = work->msgData;
    header.printWindow = &work->printWindow;
    header.queue = work->queue;
    header.font = work->font;
    work->list = BmpMenuList_Create(&header, listTop, cursorRow, heapId);
}



void func_ov021_021c59a8(ShortcutMenuApp *work, u16 *listTop, u16 *cursorRow) {
    BmpMenuList_Free(work->list, listTop, cursorRow);
    ListMenuCore_FreeOptionList(work->options);
}

void func_ov021_021c59bc(TCB *tcb, void *data) {
    ShortcutMenuApp *work = data;
    s32 y = work->scroll * 0xc0 / 4 - 0xc0;

    if (work->scroll++ >= 4) {
        y = 0;
        work->scrolling = TRUE;
    }
    GFL_BGSysMoveBG(BG, BG_MOVE_SET_Y, y);
}

static void func_ov021_021c59f8(ShortcutMenuApp *work, s32 y) {
    ClActorPos pos;
    int i;

    for (i = 0; i < ACTORS; i++) {
        func_0204b454(work->actors[i], &pos, 0);
        pos.y = y;
        func_0204b404(work->actors[i], &pos, 0);
    }
}






static inline void FreeMenu(ShortcutMenuApp *work, u16 *listPos) {
    FreeList(work, listPos, listPos + 1);
    BmpWin_Free(work->window);
    MI_CpuFill8(work, 0, offsetof(ShortcutMenuApp, unit));
}

static inline void OnCursorUp(ShortcutMenuApp *work, u16 value, u32 count, u32 *handled) {
    if (value == 0) {
        s32 maxShown = (s32)func_02020350(work->list, BMPMENULIST_PARAM_MAX_SHOWN);

        FreeList(work, NULL, NULL);
        CreateList(work, (count - 1) - (maxShown - 1), maxShown - 1, work->heapId);
        if (work->mode == 1) {
            work->scrollDir = 1;
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        *handled = TRUE;
    } else if (work->scrollDir == 1 && work->mode == 1) {
        u16 cursorRow;
        u16 y;

        func_020202fc(work->list, NULL, &cursorRow);
        y = func_02020324(work->list, cursorRow) >> 3;
        SetActorsY(work, (y + 12) * 8);
        work->scrollDir = 0;
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        *handled = TRUE;
    }
}

static inline void OnCursorDown(ShortcutMenuApp *work, u16 value, u32 count, u32 *handled) {
    if (value == count - 1) {
        if (work->mode == 1) {
            u16 cursorRow;
            u16 y;

            func_020202fc(work->list, NULL, &cursorRow);
            y = func_02020324(work->list, cursorRow + 1) >> 3;
            SetActorsY(work, (y + 12) * 8);
            work->scrollDir ^= 1;
        }
        if (work->scrollDir == 0) {
            FreeList(work, NULL, NULL);
            CreateList(work, 0, 0, work->heapId);
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        *handled = TRUE;
    } else {
        work->scrollDir = 0;
    }
}

static inline void ReadInput(ShortcutMenuApp *work) {
    s32 count;
    u16 listTop;
    u16 cursorRow;
    u16 index;
    u16 selectIndex;
    u16 buttonIndex;
    u16 cursorIndex;
    u32 handled = FALSE;
    s32 result = BMPMENULIST_NULL;

    func_020355d8(FALSE);
    func_020202ec(work->list, &cursorIndex);
    count = (s32)func_02020350(work->list, BMPMENULIST_PARAM_COUNT);

    if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
        OnCursorUp(work, cursorIndex, count, &handled);
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
        OnCursorDown(work, cursorIndex, count, &handled);
    }
    if (!handled) {
        result = (s32)BmpMenuList_Update(work->list);
    }
    if (result == BMPMENULIST_NULL) {
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
            result = BMPMENULIST_CANCEL;
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
        } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_Y) {
            func_020202ec(work->list, &buttonIndex);
            result = (s32)func_02020344(work->list, buttonIndex);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        } else if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) && work->mode == 1) {
            func_020202ec(work->list, &selectIndex);
            result = (s32)func_02020344(work->list, selectIndex);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        }
    }
    if (work->mode == 0) {
        work->input = result;
    } else if (work->mode == 1 && result != BMPMENULIST_NULL) {
        if (result != 0) {
            u8 newIndex;

            result = (s32)func_02020344(work->list, work->listTop + work->cursorRow);
            func_020202ec(work->list, &index);
            if (work->scrollDir != 0) {
                newIndex = 0xff;
            } else {
                newIndex = (u8)index;
            }
            result = ShortcutSave_MoveItemToIndex(work->shortcut, result, newIndex);
            FreeList(work, &listTop, &cursorRow);
            if (index - 1 == result) {
                cursorRow--;
            }
            CreateList(work, listTop, cursorRow, work->heapId);
        }
        handled = TRUE;
    }
    if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) && work->mode == 0
        && func_02020350(work->list, BMPMENULIST_PARAM_COUNT) > 1) {
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        handled = TRUE;
    }
    if (handled) {
        work->mode ^= 1;
        if (work->mode == 0) {
            int i;

            work->scrollDir = 0;
            for (i = 0; i < ACTORS; i++) {
                func_0204b3dc(work->actors[i], FALSE);
            }
            RedrawHighlight(work->list, FALSE, FALSE);
        } else {
            func_020202fc(work->list, &work->listTop, &work->cursorRow);
            work->showActors = TRUE;
        }
    }
}

static inline BOOL PrintDone(ShortcutMenuApp *work) {
    PrintQueue *queue = work->queue;
    BOOL done = TRUE;

    if (work->printWindow.flushPending && !func_0201c6c4(queue, BmpWin_GetBitmap(work->printWindow.window))) {
        BmpWin_FlushChar(work->printWindow.window);
        work->printWindow.flushPending = FALSE;
    }
    done = (work->printWindow.flushPending == 0);
    if (done) {
        int i;

        if (work->cursorMoved) {
            RedrawHighlight(work->list, 0, 0);
            work->cursorMoved = 0;
        }
        if (work->showActors) {
            for (i = 0; i < ACTORS; i++) {
                func_0204b3dc(work->actors[i], TRUE);
            }
            work->showActors = 0;
        }
    }
    return done;
}


static inline void OnListCursor(BmpMenuList *list, intptr_t value, u8 init) {
    ShortcutMenuApp *work = (ShortcutMenuApp *)func_02020350(list, BMPMENULIST_PARAM_WORK);

    work->cursorMoved = 1;
}

static inline void RedrawHighlight(BmpMenuList *list, u8 a1, u8 a2) {
    ShortcutMenuApp *work = (ShortcutMenuApp *)func_02020350(list, BMPMENULIST_PARAM_WORK);
    u16 cursorY = func_02020314(list) >> 3;
    u32 rowHeight = (u32)func_02020350(list, BMPMENULIST_PARAM_ROW_HEIGHT) >> 3;

    if (work->mode == 0) {
        GFL_BGSysSetScrPaletteNo(BG, 1, 0xc, 0x1e, 0xc, 0xb);
        GFL_BGSysSetScrPaletteNo(BG, 1, cursorY + 0xc, 0x1e, rowHeight, 0xc);
        GFL_BGSysSetScrPaletteNo(BG, 0, 0xb, 0x20, 1, 0xa);
        GFL_BGSysQueueScrLoad(BG);
    } else if (work->mode == 1) {
        u16 listTop;
        u16 cursorRow;
        u32 listY;
        u32 rowH;
        u32 y;

        GFL_BGSysSetScrPaletteNo(BG, 1, 0xc, 0x1e, 0xc, 0xb);
        func_020202fc(list, &listTop, &cursorRow);
        listY = (s32)func_02020350(list, BMPMENULIST_PARAM_Y);
        rowH = (s32)func_02020350(list, BMPMENULIST_PARAM_ROW_HEIGHT);
        y = listY + rowH * (work->cursorRow + work->listTop - listTop);
        GFL_BGSysSetScrPaletteNo(BG, 1, y / 8 + 0xc, 0x1e, rowHeight, 0xc);
        GFL_BGSysSetScrPaletteNo(BG, 0, 0xb, 0x20, 1, 0xa);
        GFL_BGSysQueueScrLoad(BG);
    }
    if (work->scrollDir == 1) {
        u16 rowY = func_02020324(list, func_02020350(list, BMPMENULIST_PARAM_MAX_SHOWN)) >> 3;

        SetActorsY(work, (rowY + 0xc) * 8);
    } else {
        SetActorsY(work, (cursorY + 0xc) * 8);
    }
}

static inline void ClampListPos(u16 *listTop, u16 *cursorRow, u16 count) {
    s32 total = *listTop + *cursorRow;

    if (count <= 5) {
        *cursorRow = total;
        *listTop = 0;
    } else if (total <= 3) {
        *cursorRow = total;
        *listTop = 0;
    } else if (total <= (s32)count - 2 && total > 3) {
        *listTop = total - 3;
        *cursorRow = 3;
    } else if (total == count - 1) {
        *listTop = total - 4;
        *cursorRow = 4;
    }
}

static inline void CreateList(ShortcutMenuApp *work, u16 listTop, u16 cursorRow, HeapID heapId) {
    BmpMenuListHeader header;
    u32 count = ShortcutSave_GetShortcutCount(work->shortcut);
    u32 i;

    work->options = ListMenuCore_CreateOptionList(count, heapId);
    for (i = 0; i < count; i++) {
        u32 value = ShortcutSave_GetRegistItem(work->shortcut, i);

        ListMenuCore_AppendMsgOption(work->options, work->msgData, value, value, heapId);
    }
    {
        u16 row = cursorRow;
        u16 top = listTop;

        while (row + top >= count) {
            if (row != 0) {
                row--;
            } else if (top != 0) {
                top--;
            }
        }
        listTop = top;
        cursorRow = row;
    }
    ClampListPos(&listTop, &cursorRow, count);

    header = data_ov021_021daa80;
    header.options = work->options;
    header.count = count;
    header.work = work;
    header.cursorCallback = OnListCursor;
    header.printCallback = NULL;
    header.msgData = work->msgData;
    header.printWindow = &work->printWindow;
    header.queue = work->queue;
    header.font = work->font;
    work->list = BmpMenuList_Create(&header, listTop, cursorRow, heapId);
}

static inline void FreeList(ShortcutMenuApp *work, u16 *listTop, u16 *cursorRow) {
    BmpMenuList_Free(work->list, listTop, cursorRow);
    ListMenuCore_FreeOptionList(work->options);
}

static inline void ScrollBgIn(TCB *tcb, void *data) {
    ShortcutMenuApp *work = data;
    s32 y = work->scroll * 0xc0 / 4 - 0xc0;

    if (work->scroll++ >= 4) {
        y = 0;
        work->scrolling = TRUE;
    }
    GFL_BGSysMoveBG(BG, BG_MOVE_SET_Y, y);
}

static inline void SetActorsY(ShortcutMenuApp *work, u32 y) {
    ClActorPos pos;
    int i;

    for (i = 0; i < ACTORS; i++) {
        func_0204b454(work->actors[i], &pos, 0);
        pos.y = y;
        func_0204b404(work->actors[i], &pos, 0);
    }
}

GameEventReturnCode func_ov021_021c5a54(GameEvent *event, u32 *state, void *data);

GameEvent *func_ov021_021c5a28(GameSystem *gsys, Field *field, void *param, BOOL transition) {
    ShortcutMenuReturn *ret;
    GameEvent *event;

    event = GameEvent_Create(gsys, NULL, func_ov021_021c5a54, sizeof(*ret));
    ret = GameEvent_GetData(event);
    ret->gsys = gsys;
    ret->field = field;
    ret->param = param;
    ret->transition = transition;
    return event;
}

GameEventReturnCode func_ov021_021c5a54(GameEvent *event, u32 *state, void *data) {
    ShortcutMenuReturn *ret = data;

    switch (*state) {
    case 0:
        if (ret->transition) {
            GameEvent_ChainNext(event, EventFieldSubprocessTransition_Create(ret->gsys, ret->field, OVERLAY_ID(194),
                                                                             &data_ov194_021f5580, ret->param));
        } else {
            GameEvent_ChainNext(event, EventFieldSubprocessCall_Create(ret->gsys, ret->field, OVERLAY_ID(194),
                                                                       &data_ov194_021f5580, ret->param));
        }
        (*state)++;
        break;
    case 1:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


