// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov311/un_select.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/ui/frame_list.h"
#include "app/ui/print_msg.h"
#include "app/ui/touchbar.h"
#include "app/ui/ui_scene.h"
#include "app/un_select.h"
#include "app/un_select_graphic.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
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
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/brightness.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wordset.h"
#include "text/system/un_select.h"

// The Unity Tower's floor select, named after the ROM's "un_select.c": a list of the tower's floors, one for each
// country, that scrolls beside a picture of the tower. Only the floors of countries whose visitors the player has met
// can be chosen. The names are ours

// The floors of the list, from the top floor down to floor 2, one for each country of data_ov218_021bac80
#define UN_SELECT_FLOOR_COUNT 130
#define UN_SELECT_TOP_FLOOR (UN_SELECT_FLOOR_COUNT + 1)
// The floor of an item of the list
#define UN_SELECT_FLOOR(index) (UN_SELECT_TOP_FLOOR - (index))

// The rows of the list, and the marks of a visitor's floor on each, one for each of the two BGs the rows scroll on
#define UN_SELECT_LIST_ROWS 4
#define UN_SELECT_ROW_MARK_COUNT 7

// The cell actors
enum {
    // The list's place on the picture of the tower
    UN_SELECT_ACTOR_CURSOR = 0,
    // The floors of the latest visitors on the picture of the tower
    UN_SELECT_ACTOR_VISITOR = UN_SELECT_ACTOR_CURSOR + 1,
    // The marks of the visitors' floors in the list's rows, on its first BG and on its second
    UN_SELECT_ACTOR_ROW_MARK = UN_SELECT_ACTOR_VISITOR + UNITY_TOWER_VISITOR_MAX,
    UN_SELECT_ACTOR_ROW_MARK_2 = UN_SELECT_ACTOR_ROW_MARK + UN_SELECT_ROW_MARK_COUNT,
    // The scroll bar
    UN_SELECT_ACTOR_BAR = UN_SELECT_ACTOR_ROW_MARK_2 + UN_SELECT_ROW_MARK_COUNT,
    UN_SELECT_ACTOR_COUNT,
};

// The OBJ resources
enum {
    UN_SELECT_RES_CHARS,
    UN_SELECT_RES_PLTT,
    UN_SELECT_RES_CELLS,
    UN_SELECT_RES_COUNT,
};

#define UN_SELECT_RES_NONE 0xffffffff
// A visitor whose country has no floor
#define UN_SELECT_NO_FLOOR 0xffff

// The steps of the scene: the list, the question whether to go to the floor chosen, and the message that the floor
// chosen is the current one
enum {
    UN_SELECT_STEP_LIST,
    UN_SELECT_STEP_CONFIRM,
    UN_SELECT_STEP_SAME_FLOOR,
    UN_SELECT_STEP_COUNT,
};

// What func_ov218_021b9f98 sets
enum {
    UN_SELECT_RESULT_SELECTED,
    UN_SELECT_RESULT_CANCEL,
};

// The messages, the windows they are printed in, and the names of the countries of the list
typedef struct {
    HeapID heapId;
    TCBExManager *tcbManager;
    MsgData *msgData;
    Font *font;
    PrintQueue *printQueue;
    WordSet *wordSet;
    StrBuf *strbuf;
    StrBuf *expanded;
    PrintStream *stream;
    BmpWin *msgWindow;
    BmpWin *titleWindow;
    StrBuf *names[UN_SELECT_FLOOR_COUNT];
} UnSelectMsg;

typedef struct {
    HeapID heapId;
    UnSelectGraphic *graphic;
    TouchBar *touchBar;
    u32 unk0C;
    u32 unk10;
    u32 unk14;
    AppTaskMenuRes *menuRes;
    AppTaskMenu *menu;
    UnSelectParam *param;
    UIScene *scene;
    UnSelectMsg *msg;
    FrameList *list;
    // The item chosen
    int selected;
    // For each item, whether a visitor from its country has come
    BOOL visited[UN_SELECT_FLOOR_COUNT];
    TCB *hblankTask;
    ClActor *actors[UN_SELECT_ACTOR_COUNT];
    u32 res[UN_SELECT_RES_COUNT];
    // The item of each visitor's country, or UN_SELECT_NO_FLOOR
    u32 visitorFloors[UNITY_TOWER_VISITOR_MAX];
    // L or R was pressed, which scrolls by a page, so the marks of the rows are hidden before they are printed again
    BOOL pageScrolled;
} UnSelectWork;

// What an actor is created from: where it starts, its resources and its surface
typedef struct {
    ClActorSetup setup;
    u32 chars;
    u32 palette;
    u32 cells;
    u16 unk14;
    u16 surface;
} UnSelectActorTemplate;

static BOOL func_ov218_021b95a0(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov218_021b95d0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov218_021b9674(GameProc *proc, u32 *state, void *param, void *work);
void func_ov218_021b96cc(UnSelectWork *wk, HeapID heapId);
TouchBar *func_ov218_021b9820(ClActUnit *unit, HeapID heapId);
void func_ov218_021b9888(TouchBar *touchBar);
void func_ov218_021b9890(TouchBar *touchBar);
AppTaskMenu *func_ov218_021b9898(AppTaskMenuRes *res, MsgData *msgData, HeapID heapId);
void func_ov218_021b9914(AppTaskMenu *menu);
void func_ov218_021b991c(AppTaskMenu *menu);
void func_ov218_021b9924(void);
void func_ov218_021b9944(void);
UnSelectMsg *func_ov218_021b9964(HeapID heapId);
void func_ov218_021b9a80(UnSelectMsg *msg);
static void func_ov218_021b9ad0(UnSelectMsg *msg);
void func_ov218_021b9ae4(UnSelectMsg *msg, PrintWindow *window, PrintQueue *queue, Font *font, u32 index);
void func_ov218_021b9b78(UnSelectMsg *msg, u32 msgId);
BOOL func_ov218_021b9c00(UnSelectMsg *msg);
void func_ov218_021b9c74(UnSelectMsg *msg);
Font *func_ov218_021b9c80(UnSelectMsg *msg);
PrintQueue *func_ov218_021b9c84(UnSelectMsg *msg);
MsgData *func_ov218_021b9c88(UnSelectMsg *msg);
void func_ov218_021b9c8c(void *work, u32 index, PrintWindow *window, s16 y, BOOL firstBG);
void func_ov218_021b9cdc(void *work, u32 index, BOOL moved);
void func_ov218_021b9d10(void *work, s16 delta);
void func_ov218_021b9d48(UnSelectWork *wk);
void func_ov218_021b9e28(UnSelectWork *wk);
UnSelectWork *func_ov218_021b9e48(GameProc *proc, UnSelectParam *param);
void func_ov218_021b9f98(UnSelectWork *wk, u32 result);
BOOL func_ov218_021b9fd4(UIScene *scene, void *param);
BOOL func_ov218_021b9fd8(UIScene *scene, void *param);
BOOL func_ov218_021ba0ac(UIScene *scene, void *param);
BOOL func_ov218_021ba0b0(UIScene *scene, void *param);
BOOL func_ov218_021ba138(UIScene *scene, void *param);
BOOL func_ov218_021ba1a0(UIScene *scene, void *param);
BOOL func_ov218_021ba1c0(UIScene *scene, void *param);
BOOL func_ov218_021ba208(UIScene *scene, void *param);
BOOL func_ov218_021ba21c(UIScene *scene, void *param);
void func_ov218_021ba238(TCB *tcb, void *data);
void func_ov218_021ba290(UnSelectWork *wk);
void func_ov218_021ba2ec(UnSelectWork *wk);
void func_ov218_021ba328(UnSelectWork *wk);
void func_ov218_021ba4b4(UnSelectWork *wk);
ClActor *func_ov218_021ba4d4(ClActUnit *unit, UnSelectActorTemplate *template);
void func_ov218_021ba4fc(UnSelectWork *wk);
void func_ov218_021ba554(UnSelectWork *wk, int delta);
void func_ov218_021ba61c(UnSelectWork *wk);
void func_ov218_021ba680(UnSelectWork *wk, int pos);
void func_ov218_021ba6bc(UnSelectWork *wk, int y);
void func_ov218_021ba6e8(UnSelectWork *wk, BOOL firstBG, int y);
BOOL func_ov218_021ba720(ClActor **actors, int index, u16 surface, int y);
BOOL func_ov218_021ba75c(UnSelectWork *wk, u32 index);
u32 func_ov218_021ba780(u32 country);

// The rows of the list, the scroll bar and the buttons that scroll by a page
// pokebw2's sUnSelectListTouch
static const FrameListTouch data_ov218_021babe0[] = {
    { { 0, 39, 84, 231 }, FRAMELIST_TOUCH_ITEM_DECIDE },   { { 40, 79, 84, 231 }, FRAMELIST_TOUCH_ITEM_DECIDE },
    { { 80, 119, 84, 231 }, FRAMELIST_TOUCH_ITEM_DECIDE }, { { 120, 159, 84, 231 }, FRAMELIST_TOUCH_ITEM_DECIDE },
    { { 8, 160, 240, 255 }, FRAMELIST_TOUCH_BAR },         { { 168, 191, 16, 39 }, FRAMELIST_TOUCH_PAGE_UP },
    { { 168, 191, 80, 103 }, FRAMELIST_TOUCH_PAGE_DOWN },  { { TOUCH_RECT_END } },
};

// pokebw2's sUnSelectListCallbacks
static const FrameListCallbacks data_ov218_021bab14 = {
    func_ov218_021b9c8c,
    func_ov218_021b9cdc,
    func_ov218_021b9d10,
};

static const UISceneStep data_ov218_021bab20[UN_SELECT_STEP_COUNT] = {
    { func_ov218_021b9fd4, NULL, func_ov218_021b9fd8, NULL, func_ov218_021ba0ac },
    { func_ov218_021ba0b0, NULL, func_ov218_021ba138, NULL, func_ov218_021ba1a0 },
    { func_ov218_021ba1c0, NULL, func_ov218_021ba208, NULL, func_ov218_021ba21c },
};

const GameProcFunctions UN_SELECT_PROC_FUNCTIONS = {
    func_ov218_021b95a0,
    func_ov218_021b9674,
    func_ov218_021b95d0,
};

// The country of each item of the list, indexed by the item
static const u32 data_ov218_021bac80[UN_SELECT_FLOOR_COUNT] = {
#include "asset/un_select_data_ov218_021bac80.inc"
};

// pokebw2's sUnSelectListSetup
static FrameListSetup data_ov218_021bb060 = {
    { 2, 6, 8, 0, 22, 5, 1, 0, 20, 5, 1, 40, 20, 10, 8, 5, 4, 3, 16, 0 },
    UN_SELECT_FLOOR_COUNT,
    2,
    0,
    UN_SELECT_LIST_ROWS,
    0,
    data_ov218_021babe0,
    &data_ov218_021bab14,
    NULL,
};

// pokebw2's UnSelect_Init
static BOOL func_ov218_021b95a0(GameProc *proc, u32 *state, void *param, void *work) {
    UnSelectWork *wk;

    switch (*state) {
    case 0:
        func_ov218_021b9e48(proc, param);
        (*state)++;
        break;
    case 1:
        wk = work;
        if (!func_ov067_021b7a64(wk->list)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov218_021b95d0(GameProc *proc, u32 *state, void *param, void *work) {
    UnSelectWork *wk = work;
    HeapID heapId;

    switch (*state) {
    case 0:
        GFL_FadeSet(3, 0, 16, 1);
        (*state)++;
        break;
    case 1:
        if (GFL_FadeIsRunning() != TRUE) {
            (*state)++;
        }
        break;
    case 2:
        GFL_TCBRemove(wk->hblankTask);
        G2_BlendNone();
        G2S_BlendNone();
        func_ov218_021b9e28(wk);
        func_ov067_021b613c(wk->scene);
        func_ov218_021b9a80(wk->msg);
        func_ov218_021ba4b4(wk);
        func_ov218_021ba2ec(wk);
        func_02027fd8(wk->menuRes);
        func_ov218_021b9888(wk->touchBar);
        func_ov218_021ba8f0(wk->graphic);
        heapId = wk->heapId;
        GFL_ProcReleaseSubsystem(proc);
        GFL_HeapDelete(heapId);
        GFL_OvlUnload(OVERLAY_ID(67));
        return TRUE;
    }
    return FALSE;
}


// pokebw2's UnSelect_Main
static BOOL func_ov218_021b9674(GameProc *proc, u32 *state, void *param, void *work) {
    UnSelectWork *wk = work;
    BOOL end = FALSE;

    switch (*state) {
    case 0:
        GFL_FadeSet(3, 16, 0, 1);
        (*state)++;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            (*state)++;
        }
        break;
    case 2:
        if (func_ov067_021b6144(wk->scene)) {
            end = TRUE;
        }
        break;
    }
    func_ov218_021b9ad0(wk->msg);
    func_ov218_021ba94c(wk->graphic);
    return end ? TRUE : FALSE;
}

// Loads the BGs, and blanks the floors on the picture of the tower whose countries no visitor has come from
// pokebw2's UnSelect_LoadBG
void func_ov218_021b96cc(UnSelectWork *wk, HeapID heapId) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_UN_SELECT, heapId);
    NNSG2dCharacterData *lowerChars;
    NNSG2dCharacterData *upperChars;
    void *lowerData;
    void *upperData;
    int i;

    GFL_G2DIOLoadArcNCLRDefault(arc, 1, 0, 0, 7 * 0x20, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, 4, 0, 7 * 0x20, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 12, 4, 0, 0x200, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 6, 7, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 13, 4, 0, 0, FALSE, heapId);
    upperData = GFL_G2DIOReadBGNCGRArc(arc, 0, FALSE, &upperChars, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 5, 3, 0, 0, FALSE, heapId);
    lowerData = GFL_G2DIOReadBGNCGRArc(arc, 0, FALSE, &lowerChars, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 2, 2, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 2, 6, 0, 0, FALSE, heapId);
    for (i = 0; i < UN_SELECT_FLOOR_COUNT; i++) {
        // Each floor is two rows of pixels, four floors to a tile
        int floor = UN_SELECT_FLOOR_COUNT - 1 - i;
        u8 *chars = i < 50 ? upperChars->pRawData : lowerChars->pRawData;

        if (!wk->visited[i]) {
            u8 tile = floor / 4;
            u8 row = (3 - floor % 4) * 2;
            u8 offset = row * 4;
            int pos = tile * 0x20 + 0x20 + offset;

            chars[pos] = 0xff;
            chars[pos + 1] = 0xff;
            chars[pos + 2] = 0xff;
            chars[pos + 3] = 0xff;
            chars[pos + 4] = 0xff;
            chars[pos + 5] = 0xff;
            chars[pos + 6] = 0xff;
            chars[pos + 7] = 0xff;
        }
    }
    GFL_BGSysLoadChar(7, upperChars->pRawData, upperChars->szByte, 0);
    GFL_HeapFree(upperData);
    GFL_BGSysLoadChar(3, lowerChars->pRawData, lowerChars->szByte, 0);
    GFL_HeapFree(lowerData);
    GFL_ArcToolFree(arc);
}

TouchBar *func_ov218_021b9820(ClActUnit *unit, HeapID heapId) {
    TouchBarItem items[] = {
        { 1, { 232, 168 } },
        { 4, { 16, 168 } },
        { 5, { 80, 168 } },
    };
    TouchBarSetup setup;
    TouchBar *touchBar;

    MI_CpuFill8(&setup, 0, sizeof(TouchBarSetup));
    setup.items = items;
    setup.count = NELEMS(items);
    setup.unit = unit;
    setup.bg = 1;
    setup.bgPalette = 13;
    setup.objPalette = 0;
    setup.vramType = CLACT_VRAM_MAIN;
    touchBar = func_ov067_021b62e0(&setup, heapId);
    func_ov067_021b64f8(touchBar, 1);
    func_ov067_021b6568(touchBar, 4, 0);
    func_ov067_021b6568(touchBar, 5, 0);
    return touchBar;
}


// pokebw2's UnSelect_FreeTouchBar
void func_ov218_021b9888(TouchBar *touchBar) {
    func_ov067_021b639c(touchBar);
}

// pokebw2's UnSelect_UpdateTouchBar
void func_ov218_021b9890(TouchBar *touchBar) {
    func_ov067_021b63d0(touchBar);
}

// pokebw2's UnSelect_CreateYesNo
AppTaskMenu *func_ov218_021b9898(AppTaskMenuRes *res, MsgData *msgData, HeapID heapId) {
    AppTaskMenuInit init;
    AppTaskMenuItem items[2];
    AppTaskMenu *menu;
    u32 i;

    for (i = 0; i < 2; i++) {
        items[i].str = GFL_MsgDataLoadStrbufNew(msgData, UnSelect_Text_235 + i);
        items[i].color = PRINT_COLOR(14, 15, 3);
        items[i].type = 0;
    }
    init.heapId = heapId;
    init.itemCount = 2;
    init.items = items;
    init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
    init.x = 32;
    init.y = 18;
    init.width = 13;
    init.height = 3;
    menu = func_02027778(&init, res);
    for (i = 0; i < 2; i++) {
        GFL_StrBufFree(items[i].str);
    }
    return menu;
}

// pokebw2's UnSelect_FreeMenu
void func_ov218_021b9914(AppTaskMenu *menu) {
    func_02027858(menu);
}

// pokebw2's UnSelect_UpdateMenu
void func_ov218_021b991c(AppTaskMenu *menu) {
    func_02027974(menu);
}

// Darkens the lower screen's BGs behind the message, and brightens them again
// pokebw2's UnSelect_StartDim
void func_ov218_021b9924(void) {
    func_02023f98(1);
    func_02023ef4(
        8, -8, 0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 1);
}

// pokebw2's UnSelect_StartUndim
void func_ov218_021b9944(void) {
    func_02023f98(1);
    func_02023ef4(
        8, 0, -8, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 1);
}

// pokebw2's UnSelectMsg_Create
UnSelectMsg *func_ov218_021b9964(HeapID heapId) {
    // the fill color of the windows: these two read folded, emitted first in the file's .rodata (pokebw2's)
    static const u8 backColor = 15;
    UnSelectMsg *msg = GFL_HeapAllocate(heapId, sizeof(UnSelectMsg), TRUE, "un_select.c", 972);
    BmpWin *window;

    msg->heapId = heapId;
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 14 * 0x20, 0x20, heapId);
    LoadSysMsgBox(0, 1, 15, 0, heapId);
    msg->wordSet = GFL_WordSetSystemCreateDefault(heapId);
    msg->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_UN_SELECT, heapId);
    msg->printQueue = func_0201c440(heapId);
    msg->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, heapId);
    msg->strbuf = GFL_StrBufCreate(1600, heapId);
    msg->expanded = GFL_StrBufCreate(1600, heapId);
    msg->tcbManager = GFL_TCBExMgrCreate(heapId, heapId, 1, 0);
    msg->msgWindow = BmpWin_CreateDynamic(0, 1, 19, 30, 4, 14, TRUE);
    window = msg->titleWindow = BmpWin_CreateDynamic(4, 1, 1, 30, 2, 6, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(window), backColor);
    GFL_TextRndUpdateColorIndexLUT(1, 2, backColor);
    GFL_MsgDataLoadStrbuf(msg->msgData, UnSelect_Text_233, msg->strbuf);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 4, 0, msg->strbuf, msg->font);
    BmpWin_Transfer(window);
    return msg;
}

// pokebw2's UnSelectMsg_Free
void func_ov218_021b9a80(UnSelectMsg *msg) {
    if (msg->stream != NULL) {
        func_0201ce48(msg->stream);
        msg->stream = NULL;
    }
    BmpWin_Free(msg->titleWindow);
    BmpWin_Free(msg->msgWindow);
    GFL_StrBufFree(msg->strbuf);
    GFL_StrBufFree(msg->expanded);
    GFL_MsgDataFree(msg->msgData);
    func_0201c4c0(msg->printQueue);
    GFL_FontFree(msg->font);
    GFL_TCBExMgrFree(msg->tcbManager);
    GFL_WordSetSystemFree(msg->wordSet);
    GFL_HeapFree(msg);
}

// pokebw2's UnSelectMsg_Update
static void func_ov218_021b9ad0(UnSelectMsg *msg) {
    func_0201c4e4(msg->printQueue);
    GFL_TCBExMgrUpdate(msg->tcbManager);
}

// Prints a row of the list: its floor, and the name of its country, in one line or two
// pokebw2's UnSelectMsg_PrintItem
void func_ov218_021b9ae4(UnSelectMsg *msg, PrintWindow *window, PrintQueue *queue, Font *font, u32 index) {
    int pos = index;
    u32 y = 12;
    u32 floor = UN_SELECT_FLOOR(pos);

    GFL_MsgDataLoadStrbuf(msg->msgData, floor == 2 ? UnSelect_Text_240 : UnSelect_Text_237, msg->strbuf);
    WordSetNumber(msg->wordSet, 2, floor, 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(msg->wordSet, msg->expanded, msg->strbuf);
    func_ov067_021b6ae4(window, queue, 0, 12, msg->expanded, font, PRINT_COLOR(14, 15, 0), PRINT_ALIGN_LEFT);
    if (func_0201d2c8(msg->names[pos]) > 1) {
        y = 4;
    }
    func_ov067_021b6ae4(window, queue, 30, y, msg->names[pos], font, PRINT_COLOR(14, 15, 0), PRINT_ALIGN_LEFT);
}

// pokebw2's UnSelectMsg_Print
void func_ov218_021b9b78(UnSelectMsg *msg, u32 msgId) {
    static const u8 backColor = 15;
    s32 wait = func_02012ffc();
    BmpWin *window = msg->msgWindow;

    GFL_BitmapFill(BmpWin_GetBitmap(window), backColor);
    GFL_TextRndUpdateColorIndexLUT(1, 2, backColor);
    GFL_MsgDataLoadStrbuf(msg->msgData, msgId, msg->strbuf);
    GFL_WordSetFormatStrbuf(msg->wordSet, msg->expanded, msg->strbuf);
    msg->stream = func_0201cce4(window, 4, 0, msg->expanded, msg->font, wait, msg->tcbManager, 0xffff, msg->heapId, 15);
    BmpWin_DrawFrame(msg->msgWindow, 2, 1, 15);
    BmpWin_Transfer(window);
}

// Whether the message has been printed and the player has gone on
// pokebw2's UnSelectMsg_Wait
BOOL func_ov218_021b9c00(UnSelectMsg *msg) {
    if (msg->stream != NULL) {
        switch (func_0201ce30(msg->stream)) {
        case 2:
            func_0201ce48(msg->stream);
            msg->stream = NULL;
            return TRUE;
        case 1:
            if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_A || GCTX_HIDGetPressedKeys() == PAD_BUTTON_B ||
                func_02035c60()) {
                func_0201ce38(msg->stream);
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
            }
            break;
        case 0:
            if ((GCTX_HIDGetHeldKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c38()) {
                func_0201ce5c(msg->stream, 0);
            }
            break;
        }
        return FALSE;
    }
    return FALSE;
}

// pokebw2's UnSelectMsg_ClearFrame
void func_ov218_021b9c74(UnSelectMsg *msg) {
    func_0201f7a8(msg->msgWindow, TRUE);
}

// pokebw2's UnSelectMsg_GetFont
Font *func_ov218_021b9c80(UnSelectMsg *msg) {
    return msg->font;
}

// pokebw2's UnSelectMsg_GetPrintQueue
PrintQueue *func_ov218_021b9c84(UnSelectMsg *msg) {
    return msg->printQueue;
}

// pokebw2's UnSelectMsg_GetMsgData
MsgData *func_ov218_021b9c88(UnSelectMsg *msg) {
    return msg->msgData;
}

// pokebw2's UnSelect_PrintItem
void func_ov218_021b9c8c(void *work, u32 index, PrintWindow *window, s16 y, BOOL firstBG) {
    UnSelectWork *wk = work;
    PrintQueue *queue;

    func_ov218_021ba4fc(wk);
    func_ov067_021b9398(wk->list, index);
    queue = func_ov067_021b9394(wk->list);
    func_ov218_021b9ae4(wk->msg, window, queue, func_ov218_021b9c80(wk->msg), index);
    if (func_ov218_021ba75c(wk, index)) {
        func_ov218_021ba6e8(wk, firstBG, y + 20);
    }
}

// pokebw2's UnSelect_SelectItem
void func_ov218_021b9cdc(void *work, u32 index, BOOL moved) {
    UnSelectWork *wk = work;
    s16 pos = func_ov067_021b8ab8(wk->list);

    func_ov218_021ba680(wk, pos);
    func_ov218_021ba6bc(wk, (func_ov067_021b93a4(wk->list) - func_ov067_021b93b0(wk->list)) * 2 + 92);
}

// pokebw2's UnSelect_ScrollList
void func_ov218_021b9d10(void *work, s16 delta) {
    UnSelectWork *wk = work;
    s16 pos;

    func_ov218_021ba554(wk, -delta);
    pos = func_ov067_021b8ab8(wk->list);
    func_ov218_021ba680(wk, pos);
    func_ov218_021ba6bc(wk, (func_ov067_021b93a4(wk->list) - func_ov067_021b93b0(wk->list)) * 2 + 92);
}

void func_ov218_021b9d48(UnSelectWork *wk) {
    int i;
    int floor;
    ArcTool *arc;
    MsgData *msgData;

    data_ov218_021bb060.work = wk;
    floor = wk->param->floor;
    if (floor <= 1) {
        floor = 2;
    }
    for (i = 0; i < UN_SELECT_FLOOR_COUNT; i++) {
        if (UN_SELECT_FLOOR(i) == floor) {
            if (i > UN_SELECT_FLOOR_COUNT - UN_SELECT_LIST_ROWS) {
                data_ov218_021bb060.cursorPos = i - (UN_SELECT_FLOOR_COUNT - UN_SELECT_LIST_ROWS);
                data_ov218_021bb060.scroll = i - data_ov218_021bb060.cursorPos;
            } else {
                data_ov218_021bb060.scroll = i;
            }
        }
    }
    wk->list = func_ov067_021b7754(&data_ov218_021bb060, wk->heapId);
    arc = GFL_ArcSysCreateFileHandle(ARCID_UN_SELECT, wk->heapId);
    func_ov067_021b7a10(wk->list, arc, 3, FALSE, 0);
    func_ov067_021b7a10(wk->list, arc, 4, FALSE, 1);
    func_ov067_021b7a4c(wk->list, arc, 1, 2, 3);
    GFL_ArcToolFree(arc);
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_UN_SELECT, wk->heapId);
    for (i = 0; i < UN_SELECT_FLOOR_COUNT; i++) {
        u32 type = 1;

        if (wk->visited[i] == TRUE) {
            type = 0;
        }
        func_ov067_021b79e4(wk->list, type, i);
        if (type == 0) {
            wk->msg->names[i] = GFL_MsgDataLoadStrbufNew(msgData, data_ov218_021bac80[i]);
        } else {
            wk->msg->names[i] = GFL_MsgDataLoadStrbufNew(msgData, UnSelect_Text_238);
        }
    }
    GFL_MsgDataFree(msgData);
}


void func_ov218_021b9e28(UnSelectWork *wk) {
    int i;

    for (i = 0; i < UN_SELECT_FLOOR_COUNT; i++) {
        GFL_StrBufFree(wk->msg->names[i]);
    }
    func_ov067_021b796c(wk->list);
}

UnSelectWork *func_ov218_021b9e48(GameProc *proc, UnSelectParam *param) {
    UnSelectWork *wk;
    int i;
    Font *font;
    PrintQueue *queue;

    GFL_OvlLoad(OVERLAY_ID(67));
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_UN_SELECT, 0x30000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(UnSelectWork), HEAPID_UN_SELECT);
    MI_CpuFill8(wk, 0, sizeof(UnSelectWork));
    wk->heapId = HEAPID_UN_SELECT;
    wk->param = param;
    for (i = 0; i < UN_SELECT_FLOOR_COUNT; i++) {
        if (param->countryVisited[data_ov218_021bac80[i] - 1]) {
            wk->visited[i] = TRUE;
        } else {
            wk->visited[i] = FALSE;
        }
    }
    for (i = 0; i < UNITY_TOWER_VISITOR_MAX; i++) {
        wk->visitorFloors[i] =
            param->visitorCountries[i] != 0 ? func_ov218_021ba780(param->visitorCountries[i]) : UN_SELECT_NO_FLOOR;
    }
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    wk->graphic = func_ov218_021ba848(0, wk->heapId);
    wk->scene = func_ov067_021b6100(wk->heapId, data_ov218_021bab20, UN_SELECT_STEP_COUNT, UN_SELECT_STEP_LIST, wk);
    func_ov218_021b96cc(wk, wk->heapId);
    wk->msg = func_ov218_021b9964(wk->heapId);
    wk->touchBar = func_ov218_021b9820(func_ov218_021ba960(wk->graphic), wk->heapId);
    font = func_ov218_021b9c80(wk->msg);
    queue = func_ov218_021b9c84(wk->msg);
    wk->menuRes = func_02027f60(1, 11, font, queue, wk->heapId);
    func_ov218_021ba290(wk);
    func_ov218_021b9d48(wk);
    func_ov218_021ba328(wk);
    func_ov218_021ba61c(wk);
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG3, 6, 10);
    wk->hblankTask = GFL_HBlankTCBAdd(func_ov218_021ba238, wk, 0);
    func_0203d34c(TRUE, wk->heapId);
    return wk;
}



// pokebw2's UnSelect_SetResult
void func_ov218_021b9f98(UnSelectWork *wk, u32 result) {
    switch (result) {
    case UN_SELECT_RESULT_SELECTED: {
        int selected = wk->selected;

        wk->param->selectedFloor = UN_SELECT_FLOOR(selected);
        wk->param->selectedCountry = data_ov218_021bac80[selected];
        wk->param->selected = TRUE;
        break;
    }
    case UN_SELECT_RESULT_CANCEL:
        wk->param->selectedFloor = 0;
        wk->param->selectedCountry = 0;
        wk->param->selected = FALSE;
        break;
    }
}

// pokebw2's UnSelect_ListBegin
BOOL func_ov218_021b9fd4(UIScene *scene, void *param) {
    return TRUE;
}

BOOL func_ov218_021b9fd8(UIScene *scene, void *param) {
    UnSelectWork *wk = param;
    BOOL pageScrolled = FALSE;
    int selected;
    u16 next;

    wk->pageScrolled = FALSE;
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_L) {
        pageScrolled = TRUE;
    } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_R) {
        pageScrolled = TRUE;
    }
    if (pageScrolled) {
        wk->pageScrolled = TRUE;
    }
    if (func_ov067_021b7ab0(wk->list) < UN_SELECT_LIST_ROWS) {
        selected = func_ov067_021b93a4(wk->list);
        if (wk->visited[selected] == TRUE) {
            wk->selected = selected;
            func_ov067_021b9280(wk->list, (u16)func_ov067_021b93b0(wk->list), 3);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            if (wk->param->floor == UN_SELECT_FLOOR(selected)) {
                next = 2;
            } else {
                next = 1;
            }
            func_ov067_021b61f8(scene, next);
            return TRUE;
        }
    }
    func_ov218_021b9890(wk->touchBar);
    switch (func_ov067_021b6448(wk->touchBar)) {
    case -1:
    case 0:
        break;
    case 1:
        func_ov218_021b9f98(wk, 1);
        func_ov067_021b61f8(scene, UI_SCENE_END);
        return TRUE;
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    }
    return FALSE;
}


// pokebw2's UnSelect_ListEnd
BOOL func_ov218_021ba0ac(UIScene *scene, void *param) {
    return TRUE;
}

BOOL func_ov218_021ba0b0(UIScene *scene, void *param) {
    UnSelectWork *wk = param;

    switch (func_ov067_021b61fc(scene)) {
    case 0:
        func_ov218_021b9924();
        func_ov067_021b6200(scene);
        break;
    case 1:
        if (GFL_FadeIsRunning() != TRUE && func_02024004(1)) {
            loadCountryToStrbuf(wk->msg->wordSet, 0, data_ov218_021bac80[wk->selected]);
            func_ov218_021b9b78(wk->msg, UnSelect_Text_234);
            func_ov067_021b6200(scene);
        }
        break;
    case 2:
        if (func_ov218_021b9c00(wk->msg)) {
            func_ov067_021b6200(scene);
        }
        break;
    case 3:
        wk->menu = func_ov218_021b9898(wk->menuRes, func_ov218_021b9c88(wk->msg), wk->heapId);
        return TRUE;
    }
    return FALSE;
}


BOOL func_ov218_021ba138(UIScene *scene, void *param) {
    UnSelectWork *wk = param;

    func_ov218_021b991c(wk->menu);
    if (func_020279e8(wk->menu)) {
        switch (func_02027a04(wk->menu)) {
        case 0:
            func_ov218_021b9f98(wk, 0);
            func_ov218_021b9944();
            func_ov218_021b9c74(wk->msg);
            func_ov218_021b9914(wk->menu);
            func_ov067_021b61f8(scene, UI_SCENE_END);
            break;
        case 1:
            func_ov218_021b9944();
            func_ov218_021b9c74(wk->msg);
            func_ov218_021b9914(wk->menu);
            func_ov067_021b61f8(scene, 0);
            break;
        }
        return TRUE;
    }
    return FALSE;
}


// pokebw2's UnSelect_ConfirmEnd
BOOL func_ov218_021ba1a0(UIScene *scene, void *param) {
    if (GFL_FadeIsRunning() == TRUE || !func_02024004(1)) {
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov218_021ba1c0(UIScene *scene, void *param) {
    UnSelectWork *wk = param;

    switch (func_ov067_021b61fc(scene)) {
    case 0:
        func_ov218_021b9924();
        func_ov067_021b6200(scene);
        break;
    case 1:
        if (GFL_FadeIsRunning() != TRUE && func_02024004(1)) {
            func_ov218_021b9b78(wk->msg, 0xef);
            func_ov067_021b6200(scene);
        }
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}


// pokebw2's UnSelect_SameFloorRun
BOOL func_ov218_021ba208(UIScene *scene, void *param) {
    UnSelectWork *wk = param;

    if (func_ov218_021b9c00(wk->msg)) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov218_021ba21c(UIScene *scene, void *param) {
    UnSelectWork *wk = param;

    func_ov218_021b9944();
    func_ov218_021b9c74(wk->msg);
    func_ov067_021b61f8(scene, 0);
    return TRUE;
}


// Blends the lower screen's BGs less toward its bottom
// pokebw2's UnSelect_HBlank
void func_ov218_021ba238(TCB *tcb, void *data) {
    s32 line = GX_GetVCount();

    if (line >= 152) {
        G2S_ChangeBlendAlpha(11, 5);
    } else if (line >= 112) {
        G2S_ChangeBlendAlpha(10, 6);
    } else if (line >= 72) {
        G2S_ChangeBlendAlpha(9, 7);
    } else if (line >= 32) {
        G2S_ChangeBlendAlpha(8, 8);
    } else {
        G2S_ChangeBlendAlpha(6, 10);
    }
}

// pokebw2's UnSelect_LoadObjRes
void func_ov218_021ba290(UnSelectWork *wk) {
    HeapID heapId = wk->heapId;
    ArcTool *arc;

    wk->res[UN_SELECT_RES_CHARS] = UN_SELECT_RES_NONE;
    wk->res[UN_SELECT_RES_PLTT] = UN_SELECT_RES_NONE;
    wk->res[UN_SELECT_RES_CELLS] = UN_SELECT_RES_NONE;
    arc = GFL_ArcSysCreateFileHandle(ARCID_UN_SELECT, heapId);
    wk->res[UN_SELECT_RES_CHARS] = func_0204a6c8(arc, 10, FALSE, CLACT_VRAM_BOTH, heapId);
    wk->res[UN_SELECT_RES_PLTT] = func_0204abf0(arc, 11, CLACT_VRAM_BOTH, 3 * 0x20, heapId);
    wk->res[UN_SELECT_RES_CELLS] = func_0204af28(arc, 9, 8, heapId);
    GFL_ArcToolFree(arc);
}

// pokebw2's UnSelect_FreeObjRes
void func_ov218_021ba2ec(UnSelectWork *wk) {
    if (wk->res[UN_SELECT_RES_CHARS] != UN_SELECT_RES_NONE) {
        func_0204a8d4(wk->res[UN_SELECT_RES_CHARS]);
    }
    if (wk->res[UN_SELECT_RES_PLTT] != UN_SELECT_RES_NONE) {
        func_0204ada4(wk->res[UN_SELECT_RES_PLTT]);
    }
    if (wk->res[UN_SELECT_RES_CELLS] != UN_SELECT_RES_NONE) {
        func_0204afd8(wk->res[UN_SELECT_RES_CELLS]);
    }
}

// pokebw2's UnSelect_CreateActors
void func_ov218_021ba328(UnSelectWork *wk) {
    ClActUnit *unit = func_ov218_021ba960(wk->graphic);
    int i;

    for (i = 0; i < UN_SELECT_ACTOR_COUNT; i++) {
        wk->actors[i] = NULL;
    }
    {
        UnSelectActorTemplate template = {
            { 8, 104, 2, 0, 2 },
            wk->res[UN_SELECT_RES_CHARS],
            wk->res[UN_SELECT_RES_PLTT],
            wk->res[UN_SELECT_RES_CELLS],
            3,
            0xffff,
        };

        wk->actors[UN_SELECT_ACTOR_CURSOR] = func_ov218_021ba4d4(unit, &template);
    }
    {
        UnSelectActorTemplate template = {
            { 40, 0, 0, 0, 2 },
            wk->res[UN_SELECT_RES_CHARS],
            wk->res[UN_SELECT_RES_PLTT],
            wk->res[UN_SELECT_RES_CELLS],
            3,
            0xffff,
        };

        for (i = 0; i < UNITY_TOWER_VISITOR_MAX; i++) {
            wk->actors[UN_SELECT_ACTOR_VISITOR + i] = func_ov218_021ba4d4(unit, &template);
        }
    }
    {
        UnSelectActorTemplate template = {
            { 242, 0, 1, 0, 2 },
            wk->res[UN_SELECT_RES_CHARS],
            wk->res[UN_SELECT_RES_PLTT],
            wk->res[UN_SELECT_RES_CELLS],
            3,
            0,
        };

        for (i = 0; i < UN_SELECT_ROW_MARK_COUNT; i++) {
            ClActor *actor = func_ov218_021ba4d4(unit, &template);

            func_0204b3dc(actor, FALSE);
            wk->actors[UN_SELECT_ACTOR_ROW_MARK + i] = actor;
        }
    }
    {
        UnSelectActorTemplate template = {
            { 242, 0, 1, 0, 2 },
            wk->res[UN_SELECT_RES_CHARS],
            wk->res[UN_SELECT_RES_PLTT],
            wk->res[UN_SELECT_RES_CELLS],
            3,
            1,
        };

        for (i = 0; i < UN_SELECT_ROW_MARK_COUNT; i++) {
            ClActor *actor = func_ov218_021ba4d4(unit, &template);

            func_0204b688(actor, 1);
            func_0204b3dc(actor, FALSE);
            wk->actors[UN_SELECT_ACTOR_ROW_MARK_2 + i] = actor;
        }
    }
    {
        UnSelectActorTemplate template = {
            { 246, 152, 3, 0, 2 },
            wk->res[UN_SELECT_RES_CHARS],
            wk->res[UN_SELECT_RES_PLTT],
            wk->res[UN_SELECT_RES_CELLS],
            3,
            0,
        };

        wk->actors[UN_SELECT_ACTOR_BAR] = func_ov218_021ba4d4(unit, &template);
    }
}

// pokebw2's UnSelect_FreeActors
void func_ov218_021ba4b4(UnSelectWork *wk) {
    int i;

    for (i = 0; i < UN_SELECT_ACTOR_COUNT; i++) {
        if (wk->actors[i] != NULL) {
            func_0204b3b4(wk->actors[i]);
        }
    }
}

ClActor *func_ov218_021ba4d4(ClActUnit *unit, UnSelectActorTemplate *template) {
    ClActor *actor = func_0204b294(unit, template->chars, template->palette, template->cells, &template->setup,
        template->surface, HEAPID_UN_SELECT);

    func_0204b92c(actor, TRUE);
    return actor;
}


// pokebw2's UnSelect_HideRowMarks
void func_ov218_021ba4fc(UnSelectWork *wk) {
    int i;
    ClActor *actor;

    if (wk->pageScrolled) {
        wk->pageScrolled = FALSE;
        for (i = 0; i < UN_SELECT_ROW_MARK_COUNT; i++) {
            actor = wk->actors[UN_SELECT_ACTOR_ROW_MARK + i];
            if (func_0204b3f4(actor)) {
                func_0204b3dc(actor, FALSE);
            }
        }
        for (i = 0; i < UN_SELECT_ROW_MARK_COUNT; i++) {
            actor = wk->actors[UN_SELECT_ACTOR_ROW_MARK_2 + i];
            if (func_0204b3f4(actor)) {
                func_0204b3dc(actor, FALSE);
            }
        }
    }
}

// Moves the marks with the rows, and hides those that have scrolled off
// pokebw2's UnSelect_ScrollRowMarks
void func_ov218_021ba554(UnSelectWork *wk, int delta) {
    int i;
    s16 y;
    ClActorPos pos;

    for (i = 0; i < UN_SELECT_ROW_MARK_COUNT; i++) {
        ClActor *actor = wk->actors[UN_SELECT_ACTOR_ROW_MARK + i];

        if (func_0204b3f4(actor)) {
            func_0204b454(actor, &pos, 0);
            pos.y += (s16)delta;
            y = pos.y;
            func_0204b404(actor, &pos, 0);
            if (y <= -20 || y >= 220) {
                func_0204b3dc(actor, FALSE);
            }
        }
    }
    for (i = 0; i < UN_SELECT_ROW_MARK_COUNT; i++) {
        ClActor *actor = wk->actors[UN_SELECT_ACTOR_ROW_MARK_2 + i];

        if (func_0204b3f4(actor)) {
            func_0204b454(actor, &pos, 1);
            pos.y += (s16)delta;
            y = pos.y;
            func_0204b404(actor, &pos, 1);
            if (y <= -28 || y >= 212) {
                func_0204b3dc(actor, FALSE);
            }
        }
    }
}

// Marks the floors of the latest visitors on the picture of the tower
// pokebw2's UnSelect_PlaceVisitors
void func_ov218_021ba61c(UnSelectWork *wk) {
    int i;

    for (i = 0; i < UNITY_TOWER_VISITOR_MAX; i++) {
        ClActor *actor = wk->actors[UN_SELECT_ACTOR_VISITOR + i];

        if (wk->visitorFloors[i] == UN_SELECT_NO_FLOOR) {
            func_0204b3dc(actor, FALSE);
        } else {
            ClActorPos pos;

            func_0204b454(actor, &pos, 0xffff);
            pos.y = wk->visitorFloors[i] * 2 + 92;
            func_0204b404(actor, &pos, 0xffff);
        }
    }
}

// pokebw2's UnSelect_SetBarPos
void func_ov218_021ba680(UnSelectWork *wk, int pos) {
    s16 y;
    ClActor *actor = wk->actors[UN_SELECT_ACTOR_BAR];
    ClActorPos actorPos;

    y = pos;
    if (y < 16) {
        y = 16;
    } else if (y > 152) {
        y = 152;
    }
    func_0204b454(actor, &actorPos, 0);
    actorPos.y = y;
    func_0204b404(actor, &actorPos, 0);
}

// pokebw2's UnSelect_SetCursorPos
void func_ov218_021ba6bc(UnSelectWork *wk, int y) {
    ClActor *actor = wk->actors[UN_SELECT_ACTOR_CURSOR];
    ClActorPos pos;

    func_0204b454(actor, &pos, 0xffff);
    pos.y = y;
    func_0204b404(actor, &pos, 0xffff);
}

// Shows a mark that is hidden at y
// pokebw2's UnSelect_ShowRowMark
void func_ov218_021ba6e8(UnSelectWork *wk, BOOL firstBG, int y) {
    int i;
    int count;
    ClActor **actors;
    u16 surface;

    if (firstBG) {
        surface = 0;
        actors = &wk->actors[UN_SELECT_ACTOR_ROW_MARK];
        count = UN_SELECT_ROW_MARK_COUNT;
    } else {
        surface = 1;
        actors = &wk->actors[UN_SELECT_ACTOR_ROW_MARK_2];
        count = UN_SELECT_ROW_MARK_COUNT;
    }
    for (i = 0; i < count; i++) {
        if (func_ov218_021ba720(actors, i, surface, y)) {
            break;
        }
    }
}

// Shows an actor at y unless it is shown already, and returns whether it did
// pokebw2's UnSelect_ShowActor
BOOL func_ov218_021ba720(ClActor **actors, int index, u16 surface, int y) {
    ClActorPos pos;
    BOOL shown;
    ClActor *actor = actors[index];

    shown = FALSE;
    if (!func_0204b3f4(actor)) {
        func_0204b3dc(actor, TRUE);
        shown = TRUE;
        func_0204b454(actor, &pos, surface);
        pos.y = y;
        func_0204b404(actor, &pos, surface);
    }
    return shown;
}

// pokebw2's UnSelect_IsVisitorFloor
BOOL func_ov218_021ba75c(UnSelectWork *wk, u32 index) {
    int i;

    for (i = 0; i < UNITY_TOWER_VISITOR_MAX; i++) {
        if (index == wk->visitorFloors[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

// The item of a country, or UN_SELECT_NO_FLOOR
// pokebw2's UnSelect_FindCountry
u32 func_ov218_021ba780(u32 country) {
    int i;

    for (i = 0; i < UN_SELECT_FLOOR_COUNT; i++) {
        if (country == data_ov218_021bac80[i]) {
            break;
        }
    }
    if (i >= UN_SELECT_FLOOR_COUNT) {
        return UN_SELECT_NO_FLOOR;
    }
    return i;
}
