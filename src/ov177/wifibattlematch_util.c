// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov262/wifibattlematch_util.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/wifibattlematch_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "save/records.h"
#include "save/save_control.h"
#include "system/app_keycursor.h"
#include "system/app_printsys_common.h"
#include "system/bmp_menulist.h"
#include "system/bmp_menuwork.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/time_icon.h"

// The task runner, message p_wk and list menu that the Wi-Fi and live competition screens share, and the match counts
// in the records (the file name is the ROM's own string)

#define MSGBOX_BG_COLOR 15

struct WifiBattleMatchTask {
    WifiBattleMatchTaskFunc func;
    u32 seq;
    void *work;
    u32 returnSeq;
};

struct WifiBattleMatchMsgBox {
    u32 unk0;
    Font *font;
    PrintStream *stream;
    TCBExManager *tcbManager;
    WaitIcon *waitIcon;
    BmpWin *window;
    StrBuf *strBuf;
    u16 bgColor;
    u16 heapId;
    PrintWindow printWindow;
    PrintQueue *queue;
    u32 mode;
    BOOL done;
    KeyCursor *p_keycursor;
    AppPrintsysCommon printsys;
};

struct WifiBattleMatchMenu {
    BmpWin *window;
    PrintQueue *queue;
    PrintWindow printWindow;
    BmpMenuList *list;
    ListMenuOption *options;
    s32 cancelValue;
};

static void func_ov177_021e8754(WifiBattleMatchMsgBox *p_wk, u32 mode);
void func_ov177_021e8848(WifiBattleMatchMsgBox *p_wk);

// pokebw2's sMenuListHeader
static const BmpMenuListHeader data_ov177_021e8d68 = {
    NULL, NULL, NULL, NULL, 0, 0, 0, 13, 0, 3, 1, 15, 2, 0, 1, 0, 0, 0, NULL, 12, 13, NULL, NULL, NULL, NULL,
};

WifiBattleMatchMsgBox *func_ov177_021e8568(u32 bg, u32 palette, u16 framePalette, u16 frameChar,
                                           PrintQueue *printQueue, Font *font, HeapID heapId) {
    WifiBattleMatchMsgBox *p_wk = GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchMsgBox), FALSE,
                                                  "wifibattlematch_util.c", 94);
    BmpWin *window;

    MI_CpuFill8(p_wk, 0, sizeof(WifiBattleMatchMsgBox));
    p_wk->bgColor = MSGBOX_BG_COLOR;
    p_wk->font = font;
    p_wk->queue = func_0201c440(heapId);
    p_wk->mode = 3;
    p_wk->heapId = heapId;
    p_wk->strBuf = GFL_StrBufCreate(0x300, heapId);
    p_wk->window = BmpWin_CreateDynamic(bg, 1, 19, 30, 4, palette, TRUE);
    PrintWindow_Init(&p_wk->printWindow, p_wk->window);
    GFL_BitmapFill(BmpWin_GetBitmap(p_wk->window), p_wk->bgColor);
    window = p_wk->window;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
    p_wk->tcbManager = GFL_TCBExMgrCreate(heapId, heapId, 1, 32);
    BmpWin_DrawFrame(p_wk->window, WINFRAME_TRANSFER_NOW, frameChar, framePalette);
    return p_wk;
}

// pokebw2's WifiBattleMatchMsgBox_Free
void func_ov177_021e8628(WifiBattleMatchMsgBox *p_wk) {
    if (p_wk->stream != NULL) {
        func_0201ce48(p_wk->stream);
        p_wk->stream = NULL;
    }
    if (p_wk->waitIcon != NULL) {
        func_0202f038(p_wk->waitIcon);
        p_wk->waitIcon = NULL;
    }
    if (p_wk->p_keycursor != NULL) {
        func_02028604(p_wk->p_keycursor);
    }
    GFL_TCBExMgrFree(p_wk->tcbManager);
    func_0201f7a8(p_wk->window, WINFRAME_TRANSFER_NOW);
    BmpWin_Free(p_wk->window);
    GFL_StrBufFree(p_wk->strBuf);
    func_0201c4c0(p_wk->queue);
    GFL_HeapFree(p_wk);
}

void func_ov177_021e867c(WifiBattleMatchMsgBox *p_wk) {
    func_0201c4e4(p_wk->queue);
    switch (p_wk->mode) {
    case 2:
        PrintWindow_Flush(&p_wk->printWindow, p_wk->queue);
        break;
    case 0:
        PrintWindow_Flush(&p_wk->printWindow, p_wk->queue);
        p_wk->done = PrintWindow_IsPrinted(&p_wk->printWindow);
        break;
    case 1:
        if (p_wk->stream != NULL) {
            if (p_wk->p_keycursor != NULL) {
                func_020286c4(p_wk->p_keycursor, p_wk->stream, p_wk->window);
            }
            if (func_02028484(&p_wk->printsys, p_wk->stream)) {
                p_wk->done = TRUE;
            }
        }
        break;
    case 3:
        break;
    }
    GFL_TCBExMgrUpdate(p_wk->tcbManager);
}


// pokebw2's WifiBattleMatchMsgBox_Print
void func_ov177_021e8738(WifiBattleMatchMsgBox *p_wk, MsgData *msgData, u32 msgId, u32 mode) {
    GFL_MsgDataLoadStrbuf(msgData, msgId, p_wk->strBuf);
    func_ov177_021e8754(p_wk, mode);
}

// pokebw2's WifiBattleMatchUtil_MsgBoxStart
void func_ov177_021e8754(WifiBattleMatchMsgBox *p_wk, u32 mode) {
    func_0201c6ec(p_wk->queue);
    GFL_BitmapFill(BmpWin_GetBitmap(p_wk->window), p_wk->bgColor);
    if (p_wk->stream != NULL) {
        func_0201ce48(p_wk->stream);
        p_wk->stream = NULL;
    }
    if (p_wk->p_keycursor != NULL) {
        func_02028604(p_wk->p_keycursor);
        p_wk->p_keycursor = NULL;
    }
    func_ov177_021e8848(p_wk);
    switch (mode) {
    case 2:
        p_wk->waitIcon = func_0202ee30(GFL_VBlankGetTCBMgr(), p_wk->window, p_wk->bgColor, 16, p_wk->heapId);
        PrintWindow_PrintNoColor(&p_wk->printWindow, p_wk->queue, 0, 0, p_wk->strBuf, p_wk->font);
        p_wk->mode = 0;
        break;
    case 0:
        PrintWindow_PrintNoColor(&p_wk->printWindow, p_wk->queue, 0, 0, p_wk->strBuf, p_wk->font);
        p_wk->mode = 0;
        break;
    case 1:
        GFL_ASSERT(p_wk->p_keycursor == NULL);
        func_02028470(&p_wk->printsys, 2);
        p_wk->p_keycursor = func_02028590(p_wk->bgColor, TRUE, FALSE, p_wk->heapId);
        p_wk->stream = func_0201cce4(p_wk->window, 0, 0, p_wk->strBuf, p_wk->font, func_02012ffc(),
                                     p_wk->tcbManager, FALSE, p_wk->heapId, p_wk->bgColor);
        p_wk->mode = 1;
        break;
    }
    p_wk->done = FALSE;
}

BOOL func_ov177_021e8844(WifiBattleMatchMsgBox *p_wk) {
    return p_wk->done;
}



// Ends the wait of a message: it counts as done, and its wait icon goes
// pokebw2's WifiBattleMatchUtil_MsgBoxStopWait
void func_ov177_021e8848(WifiBattleMatchMsgBox *p_wk) {
    p_wk->done = TRUE;
    if (p_wk->waitIcon != NULL) {
        func_0202f038(p_wk->waitIcon);
        p_wk->waitIcon = NULL;
        BmpWin_FlushMap(p_wk->window);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(p_wk->window));
    }
}

// pokebw2's WifiBattleMatchMsgBox_Clear
void func_ov177_021e8870(WifiBattleMatchMsgBox *p_wk) {
    GFL_BitmapFill(BmpWin_GetBitmap(p_wk->window), p_wk->bgColor);
    BmpWin_FlushChar(p_wk->window);
}

// pokebw2's WifiBattleMatchMenu_CreateBottomRight
WifiBattleMatchMenu *func_ov177_021e888c(const WifiBattleMatchMenuParam *param, HeapID heapId) {
    u8 height = param->count * 2;
    u8 y = 17 - height;

    return func_ov177_021e88b0(param, 19, y, 12, height, heapId);
}

WifiBattleMatchMenu *func_ov177_021e88b0(const WifiBattleMatchMenuParam *param, u8 x, u8 y, u8 width, u8 height,
                                         HeapID heapId) {
    WifiBattleMatchMenu *menu = GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchMenu), FALSE,
                                                 "wifibattlematch_util.c", 418);
    BmpMenuListHeader header;
    BmpWin *window;
    u32 i;

    MI_CpuFill8(menu, 0, sizeof(WifiBattleMatchMenu));
    menu->queue = param->printQueue;
    menu->cancelValue = param->cancelValue;
    menu->window = BmpWin_CreateDynamic(param->bg, x, y, width, height, param->palette, TRUE);
    BmpWin_DrawFrame(menu->window, WINFRAME_TRANSFER_NONE, param->frameChar, param->framePalette);
    window = menu->window;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
    PrintWindow_Init(&menu->printWindow, menu->window);
    menu->options = ListMenuCore_CreateOptionList(param->count, heapId);
    for (i = 0; i < param->count; i++) {
        ListMenuCore_AppendMsgOption(menu->options, param->msgData, param->msgIds[i], i, heapId);
    }
    header = data_ov177_021e8d68;
    header.options = menu->options;
    header.count = param->count;
    header.maxShown = param->count;
    header.window = menu->window;
    header.msgData = param->msgData;
    header.printWindow = &menu->printWindow;
    header.queue = param->printQueue;
    header.font = param->font;
    menu->list = BmpMenuList_Create(&header, 0, param->cursorRow, heapId);
    func_02020cf8(menu->list, heapId);
    if (param->canCancel) {
        func_02020d10(menu->list, FALSE);
    } else {
        func_02020d10(menu->list, TRUE);
    }
    return menu;
}


// pokebw2's WifiBattleMatchMenu_Free
void func_ov177_021e89d0(WifiBattleMatchMenu *menu) {
    BmpMenuList_Free(menu->list, NULL, NULL);
    ListMenuCore_FreeOptionList(menu->options);
    func_0201f7a8(menu->window, WINFRAME_TRANSFER_NOW);
    func_020456f8(menu->window);
    BmpWin_Free(menu->window);
    GFL_HeapFree(menu);
}

// pokebw2's WifiBattleMatchMenu_Update
s32 func_ov177_021e8a00(WifiBattleMatchMenu *menu) {
    s32 result;

    PrintWindow_Flush(&menu->printWindow, menu->queue);
    result = (s32)BmpMenuList_Update(menu->list);
    if (result == BMPMENULIST_CANCEL) {
        result = menu->cancelValue;
    }
    return result;
}

// pokebw2's WifiBattleMatchTask_Create
WifiBattleMatchTask *func_ov177_021e8a3c(void *work, WifiBattleMatchTaskFunc func, HeapID heapId) {
    WifiBattleMatchTask *task = GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchTask), FALSE,
                                                 "wifibattlematch_util.c", 582);

    MI_CpuFill8(task, 0, sizeof(WifiBattleMatchTask));
    task->work = work;
    func_ov177_021e8aa0(task, func);
    return task;
}

// pokebw2's WifiBattleMatchTask_Free
void func_ov177_021e8a78(WifiBattleMatchTask *task) {
    GFL_HeapFree(task);
}

// pokebw2's WifiBattleMatchTask_Run
void func_ov177_021e8a80(WifiBattleMatchTask *task) {
    if (task->func != NULL) {
        task->func(task, &task->seq, task->work);
    }
}

// pokebw2's WifiBattleMatchTask_IsEnded
BOOL func_ov177_021e8a90(WifiBattleMatchTask *task) {
    return task->func == NULL;
}

// pokebw2's WifiBattleMatchTask_SetFunc
void func_ov177_021e8aa0(WifiBattleMatchTask *task, WifiBattleMatchTaskFunc func) {
    task->func = func;
    task->seq = 0;
}

// pokebw2's WifiBattleMatchTask_End
void func_ov177_021e8aa8(WifiBattleMatchTask *task) {
    func_ov177_021e8aa0(task, NULL);
}

// pokebw2's WifiBattleMatchTask_SetReturnSeq
void func_ov177_021e8ab4(WifiBattleMatchTask *task, u32 seq) {
    task->returnSeq = seq;
}

// pokebw2's WifiBattleMatchTask_ReturnToSeq
void func_ov177_021e8ab8(WifiBattleMatchTask *task) {
    task->seq = task->returnSeq;
}

// Counts a match in the records, by kind
void func_ov177_021e8ac0(GameData *gameData, u32 mode) {
    GameRecords *records = getTrainerCardInfoBlkAddress(GameData_GetSaveControl(gameData));

    switch (mode) {
    case 1:
    case 4:
        RecordAddOne(records, 0x12);
        break;
    case 0:
    case 3:
        RecordAddOne(records, 0x13);
        break;
    case 2:
        RecordAddOne(records, 0x14);
        break;
    }
    RecordAddOne(records, 0x11);
}

