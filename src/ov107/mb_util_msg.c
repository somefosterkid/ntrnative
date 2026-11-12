// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov181/mb_util_msg.c, Black 2/White 2's version
// of this file in its overlay 181, adapted to Black's: no sub window, and the talk window's overlay loaded always. The
// names are Black's dsd names; pokebw2's guesses are in the header and the comments.
#include "app/mb_parent/mb_util_msg.h"
#include "types.h"
#include "constants/arc.h"
#include "field/talkmsgwin.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/overlay.h"
#include "gfl/str.h"
#include "gfl/tcbl.h"
#include "system/app_keycursor.h"
#include "system/app_printsys_common.h"
#include "system/app_taskmenu.h"
#include "system/bmp_menu.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/time_icon.h"
#include "system/wordset.h"

// The messages, windows and menus of the DS Download Play parent (mb_util_msg.c, named by the ROM's embedded string):
// one message window that prints through a stream or a print queue, the yes/no menus, and the frame of the field's
// talk window from overlay 21

// The message window's shapes, by place and height
enum {
    MB_UTIL_MSG_WINDOW_NONE,
    MB_UTIL_MSG_WINDOW_MIDDLE,
    MB_UTIL_MSG_WINDOW_TALL,
    MB_UTIL_MSG_WINDOW_TALLER,
    MB_UTIL_MSG_WINDOW_BOTTOM_2,
    MB_UTIL_MSG_WINDOW_BOTTOM_4,
    MB_UTIL_MSG_WINDOW_BOTTOM_6,
    MB_UTIL_MSG_WINDOW_TOP_4,
    // The bottom window with the talk window's frame
    MB_UTIL_MSG_WINDOW_TALK,
};

struct MBUtilMsg {
    HeapID heapId;
    u8 msgBg;
    u8 menuBg;
    u32 windowType;
    // Whether the messages wait for the keys too
    BOOL useKeys;
    TCBExManager *tcbManager;
    BmpWin *window;
    Font *font;
    PrintStream *stream;
    MsgData *msgData;
    StrBuf *strbuf;
    WordSet *wordSet;
    AppPrintsysCommon printCommon;
    PrintQueue *queue;
    BOOL queued;
    BOOL mapShown;
    AppTaskMenu *menu;
    AppTaskMenuRes *menuRes;
    KeyCursor *cursor;
    ConfirmDialogSetup confirmSetup;
    BmpMenu *confirm;
    TalkMsgWinSys *talkWin;
    // Whether to show the wait icon once the queued message is printed
    BOOL showWaitIcon;
    WaitIcon *waitIcon;
};

MBUtilMsg *func_ov107_021f1404(HeapID heapId, u8 msgBg, u8 menuBg, u32 fileId, BOOL useTalkWin, u32 useKeys) {
    MBUtilMsg *msg = GFL_HeapAllocate(heapId, sizeof(MBUtilMsg), TRUE, "mb_util_msg.c", 102);

    msg->heapId = heapId;
    msg->msgBg = msgBg;
    msg->menuBg = menuBg;
    msg->useKeys = useKeys;
    msg->window = NULL;
    msg->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, msg->heapId);
    msg->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, fileId, msg->heapId);
    LoadSysMsgBox(msgBg, 1, 13, 0, msg->heapId);
    if (msgBg < 4) {
        GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x1c0, 0x20, msg->heapId);
    } else {
        GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 0x1c0, 0x20, msg->heapId);
    }
    func_0201dd68();
    msg->tcbManager = GFL_TCBExMgrCreate(msg->heapId, msg->heapId, 3, 0x100);
    msg->stream = NULL;
    msg->strbuf = NULL;
    msg->wordSet = NULL;
    msg->queue = func_0201c440(msg->heapId);
    msg->menuRes = func_02027f60(msg->menuBg, 10, msg->font, msg->queue, msg->heapId);
    msg->menu = NULL;
    msg->confirm = NULL;
    msg->cursor = func_02028590(15, useKeys, TRUE, msg->heapId);
    GFL_OvlLoad(OVERLAY_ID(21));
    if (useTalkWin == TRUE) {
        TalkMsgWinSetup setup;

        setup.heapId = heapId;
        setup.unk4 = 0;
        setup.font = msg->font;
        setup.unkC = 10;
        setup.bg = msgBg;
        setup.unk14 = 12;
        setup.unk15 = 14;
        msg->talkWin = func_ov021_02194670(&setup);
    } else {
        msg->talkWin = NULL;
    }
    msg->queued = FALSE;
    msg->mapShown = FALSE;
    msg->showWaitIcon = FALSE;
    msg->waitIcon = NULL;
    return msg;
}

void func_ov107_021f1520(MBUtilMsg *msg) {
    if (msg->talkWin != NULL) {
        func_ov021_02194798(msg->talkWin);
    }
    if (msg->menu != NULL) {
        func_02027858(msg->menu);
    }
    if (msg->confirm != NULL) {
        func_0201fef4(msg->confirm);
    }
    if (msg->waitIcon != NULL) {
        func_0202f038(msg->waitIcon);
        msg->waitIcon = NULL;
    }
    func_02028604(msg->cursor);
    func_02027fd8(msg->menuRes);
    func_0201c4c0(msg->queue);
    if (msg->stream != NULL) {
        func_0201ce48(msg->stream);
    }
    if (msg->strbuf != NULL) {
        GFL_StrBufFree(msg->strbuf);
    }
    if (msg->wordSet != NULL) {
        func_ov107_021f18ec(msg);
    }
    GFL_MsgDataFree(msg->msgData);
    if (msg->window != NULL) {
        BmpWin_Free(msg->window);
    }
    GFL_FontFree(msg->font);
    GFL_TCBExMgrFree(msg->tcbManager);
    GFL_HeapFree(msg);
    GFL_OvlUnload(OVERLAY_ID(21));
}

void func_ov107_021f15b0(MBUtilMsg *msg) {
    GFL_TCBExMgrUpdate(msg->tcbManager);
    if (msg->stream != NULL) {
        BOOL done = func_02028484(&msg->printCommon, msg->stream);

        func_020286c4(msg->cursor, msg->stream, msg->window);
        if (done == TRUE) {
            func_0201ce48(msg->stream);
            msg->stream = NULL;
        }
    }
    func_0201c4e4(msg->queue);
    if (msg->queued == TRUE && func_0201c6b4(msg->queue) == TRUE) {
        BmpWin_FlushChar(msg->window);
        msg->queued = FALSE;
        if (msg->showWaitIcon == TRUE) {
            msg->showWaitIcon = FALSE;
            msg->waitIcon = func_0202ee8c(msg->tcbManager, msg->window, 15, 16, msg->heapId);
        }
    }
    if (msg->menu != NULL) {
        func_02027974(msg->menu);
    }
}

void func_ov107_021f1628(MBUtilMsg *msg, u32 type) {
    if (msg->windowType != type) {
        if (msg->window != NULL) {
            BmpWin_Free(msg->window);
            msg->window = NULL;
        }
        switch (type) {
        case MB_UTIL_MSG_WINDOW_NONE:
            break;
        case MB_UTIL_MSG_WINDOW_MIDDLE:
            msg->window = BmpWin_CreateDynamic(msg->msgBg, 3, 8, 26, 8, 14, TRUE);
            break;
        case MB_UTIL_MSG_WINDOW_TALL:
            msg->window = BmpWin_CreateDynamic(msg->msgBg, 3, 2, 26, 18, 14, TRUE);
            break;
        case MB_UTIL_MSG_WINDOW_TALLER:
            msg->window = BmpWin_CreateDynamic(msg->msgBg, 3, 2, 26, 20, 14, TRUE);
            break;
        case MB_UTIL_MSG_WINDOW_BOTTOM_2:
            msg->window = BmpWin_CreateDynamic(msg->msgBg, 1, 21, 30, 2, 14, TRUE);
            break;
        case MB_UTIL_MSG_WINDOW_BOTTOM_4:
        case MB_UTIL_MSG_WINDOW_TALK:
            msg->window = BmpWin_CreateDynamic(msg->msgBg, 1, 19, 30, 4, 14, TRUE);
            break;
        case MB_UTIL_MSG_WINDOW_BOTTOM_6:
            msg->window = BmpWin_CreateDynamic(msg->msgBg, 1, 17, 30, 6, 14, TRUE);
            break;
        case MB_UTIL_MSG_WINDOW_TOP_4:
            msg->window = BmpWin_CreateDynamic(msg->msgBg, 1, 1, 30, 4, 14, TRUE);
            break;
        }
        msg->windowType = type;
    }
    GFL_BitmapFill(BmpWin_GetBitmap(msg->window), 0);
    BmpWin_FlushChar(msg->window);
    BmpWin_FlushMap(msg->window);
    GFL_BGSysLoadScr(msg->msgBg);
}

void func_ov107_021f1704(MBUtilMsg *msg, u32 msgId, s32 wait) {
    if (msg->stream != NULL) {
        func_0201ce48(msg->stream);
        msg->stream = NULL;
    }
    if (msg->waitIcon != NULL) {
        func_0202f038(msg->waitIcon);
        msg->waitIcon = NULL;
    }
    msg->mapShown = FALSE;
    msg->showWaitIcon = FALSE;
    if (msg->strbuf != NULL) {
        GFL_StrBufFree(msg->strbuf);
        msg->strbuf = NULL;
    }
    GFL_BitmapFill(BmpWin_GetBitmap(msg->window), 15);
    msg->strbuf = GFL_MsgDataLoadStrbufNew(msg->msgData, msgId);
    if (msg->wordSet != NULL) {
        StrBuf *strbuf = GFL_StrBufCreate(379, msg->heapId);

        GFL_WordSetFormatStrbuf(msg->wordSet, strbuf, msg->strbuf);
        GFL_StrBufFree(msg->strbuf);
        msg->strbuf = strbuf;
    }
    msg->stream = func_0201cce4(msg->window, 0, 0, msg->strbuf, msg->font, wait, msg->tcbManager, 2, msg->heapId, 15);
    if (msg->windowType == MB_UTIL_MSG_WINDOW_TALK) {
        func_ov021_02194a24(msg->talkWin, msg->window, 0);
    } else {
        BmpWin_DrawFrame(msg->window, TRUE, 1, 13);
    }
    if (msg->useKeys == TRUE) {
        func_02028470(&msg->printCommon, APP_PRINTSYS_COMMON_KEYS | APP_PRINTSYS_COMMON_TOUCH);
    } else {
        func_02028470(&msg->printCommon, APP_PRINTSYS_COMMON_TOUCH);
    }
}

void func_ov107_021f17e0(MBUtilMsg *msg, u32 msgId) {
    if (msg->stream != NULL) {
        func_0201ce48(msg->stream);
        msg->stream = NULL;
    }
    if (msg->waitIcon != NULL) {
        func_0202f038(msg->waitIcon);
        msg->waitIcon = NULL;
    }
    if (msg->strbuf != NULL) {
        GFL_StrBufFree(msg->strbuf);
        msg->strbuf = NULL;
    }
    GFL_BitmapFill(BmpWin_GetBitmap(msg->window), 15);
    msg->strbuf = GFL_MsgDataLoadStrbufNew(msg->msgData, msgId);
    if (msg->wordSet != NULL) {
        StrBuf *strbuf = GFL_StrBufCreate(379, msg->heapId);

        GFL_WordSetFormatStrbuf(msg->wordSet, strbuf, msg->strbuf);
        GFL_StrBufFree(msg->strbuf);
        msg->strbuf = strbuf;
    }
    func_0201c6fc(msg->queue, BmpWin_GetBitmap(msg->window), 0, 0, msg->strbuf, msg->font);
    msg->queued = TRUE;
    if (msg->windowType == MB_UTIL_MSG_WINDOW_TALK) {
        func_ov021_02194a24(msg->talkWin, msg->window, 0);
        GFL_BGSysLoadScr(msg->msgBg);
    } else {
        BmpWin_DrawFrame(msg->window, TRUE, 1, 13);
    }
}

void func_ov107_021f1898(MBUtilMsg *msg) {
    GFL_BitmapFill(BmpWin_GetBitmap(msg->window), 0);
    if (msg->windowType == MB_UTIL_MSG_WINDOW_TALK) {
        func_ov021_02194a34(msg->talkWin, msg->window);
        GFL_BGSysLoadScr(msg->msgBg);
    } else {
        func_0201f7a8(msg->window, TRUE);
    }
    if (msg->waitIcon != NULL) {
        func_0202f038(msg->waitIcon);
        msg->waitIcon = NULL;
    }
}

void func_ov107_021f18d8(MBUtilMsg *msg) {
    if (msg->wordSet == NULL) {
        msg->wordSet = GFL_WordSetSystemCreateDefault(msg->heapId);
    }
}

void func_ov107_021f18ec(MBUtilMsg *msg) {
    if (msg->wordSet != NULL) {
        GFL_WordSetSystemFree(msg->wordSet);
        msg->wordSet = NULL;
    }
}

void func_ov107_021f1900(MBUtilMsg *msg, u32 index, s32 number, u32 digits) {
    WordSetNumber(msg->wordSet, index, number, digits, 0, TRUE);
}

void func_ov107_021f1918(MBUtilMsg *msg, u32 index, s32 number, u32 digits) {
    WordSetNumber(msg->wordSet, index, number, digits, 2, TRUE);
}

void func_ov107_021f1930(MBUtilMsg *msg, u32 pos) {
    AppTaskMenuItem items[2];
    AppTaskMenuInit init;

    items[0].str = GFL_MsgDataLoadStrbufNew(msg->msgData, 0x12);
    items[1].str = GFL_MsgDataLoadStrbufNew(msg->msgData, 0x13);
    items[0].color = 0x39e3;
    items[1].color = 0x39e3;
    items[0].type = 0;
    items[1].type = 0;
    init.heapId = msg->heapId;
    init.itemCount = 2;
    init.items = items;
    init.posType = APP_TASKMENU_POS_TOP_LEFT;
    switch (pos) {
    case 0:
        init.x = 24;
        init.y = 6;
        break;
    case 1:
        init.x = 24;
        init.y = 12;
        break;
    case 2:
        init.x = 24;
        init.y = 18;
        break;
    }
    init.width = 8;
    init.height = 3;
    msg->menu = func_02027778(&init, msg->menuRes);
    GFL_StrBufFree(items[0].str);
    GFL_StrBufFree(items[1].str);
}

void func_ov107_021f19b8(MBUtilMsg *msg) {
    func_02027858(msg->menu);
    msg->menu = NULL;
}

// The item picked, from 1, or 0 while the menu waits
int func_ov107_021f19c8(MBUtilMsg *msg) {
    if (func_020279e8(msg->menu) == TRUE) {
        return func_02027a04(msg->menu) + 1;
    }
    return 0;
}

void func_ov107_021f19e4(MBUtilMsg *msg, u32 unused) {
    msg->confirmSetup.bg = msg->menuBg;
    msg->confirmSetup.x = 24;
    msg->confirmSetup.y = 13;
    msg->confirmSetup.palette = 14;
    msg->confirmSetup.unk4 = 0;
    msg->confirm = ShopUI_CreateConfirmDialog(&msg->confirmSetup, 1, 13, 0, msg->heapId);
}

void func_ov107_021f1a24(MBUtilMsg *msg) {
    msg->confirm = NULL;
}

// 1 for yes, 2 for no, 0 while the dialog waits
int func_ov107_021f1a2c(MBUtilMsg *msg) {
    u32 result = func_0201febc(msg->confirm);

    if (result == 0) {
        msg->confirm = NULL;
        return 1;
    }
    if (result == BMPMENU_CANCEL) {
        msg->confirm = NULL;
        return 2;
    }
    return 0;
}

void func_ov107_021f1a58(MBUtilMsg *msg, s32 wait) {
    MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SCRIPT_MESSAGE, 0x9e, msg->heapId);

    if (msg->stream != NULL) {
        func_0201ce48(msg->stream);
        msg->stream = NULL;
    }
    msg->mapShown = FALSE;
    msg->showWaitIcon = FALSE;
    if (msg->strbuf != NULL) {
        GFL_StrBufFree(msg->strbuf);
        msg->strbuf = NULL;
    }
    GFL_BitmapFill(BmpWin_GetBitmap(msg->window), 15);
    msg->strbuf = GFL_MsgDataLoadStrbufNew(msgData, 0x16);
    msg->stream = func_0201cce4(msg->window, 0, 0, msg->strbuf, msg->font, wait, msg->tcbManager, 2, msg->heapId, 15);
    BmpWin_DrawFrame(msg->window, TRUE, 1, 13);
    GFL_MsgDataFree(msgData);
    if (msg->useKeys == TRUE) {
        func_02028470(&msg->printCommon, APP_PRINTSYS_COMMON_KEYS | APP_PRINTSYS_COMMON_TOUCH);
    } else {
        func_02028470(&msg->printCommon, APP_PRINTSYS_COMMON_TOUCH);
    }
}

MsgData *func_ov107_021f1afc(MBUtilMsg *msg) {
    return msg->msgData;
}

WordSet *func_ov107_021f1b00(MBUtilMsg *msg) {
    return msg->wordSet;
}

Font *func_ov107_021f1b04(MBUtilMsg *msg) {
    return msg->font;
}

BOOL func_ov107_021f1b08(MBUtilMsg *msg) {
    return func_0201c6b4(msg->queue);
}

BOOL func_ov107_021f1b14(MBUtilMsg *msg) {
    if (msg->stream == NULL) {
        return TRUE;
    }
    return FALSE;
}

void func_ov107_021f1b24(MBUtilMsg *msg, BOOL shown) {
    msg->mapShown = shown;
    BmpWin_FlushMap(msg->window);
    GFL_BGSysLoadScr(msg->msgBg);
}

void func_ov107_021f1b38(MBUtilMsg *msg, BOOL show) {
    msg->showWaitIcon = show;
}
