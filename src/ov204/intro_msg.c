// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov294/intro_msg.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "demo/intro.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/graphics.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "system/app_keycursor.h"
#include "system/bmp_menulist.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/time_icon.h"
#include "system/wordset.h"
#include "system/bmp_menuwork.h"

// The intro's message window and its yes/no menu

typedef struct {
    BmpMenuList *list;
    ListMenuOption *options;
    s32 result;
} IntroMenu;

struct IntroMsg {
    HeapID heapId;
    PrintQueue *printQueue;
    WordSet *wordSet;
    MsgData *msgData;
    Font *font;
    PrintStream *printStream;
    TCBExManager *tcbManager;
    BmpWin *window;
    KeyCursor *keyCursor;
    IntroMenu menu;
    PrintWindow menuPrint;
    BmpWin *menuWindow;
    StrBuf *strbuf;
    StrBuf *expanded;
    WaitIcon *waitIcon;
};

// The two window frames that func_ov204_021bf23c draws
#define FRAME_0_CHAR 7
#define FRAME_0_PALETTE 9
#define FRAME_1_CHAR 16
#define FRAME_1_PALETTE 8

#define WINDOW_PALETTE 10
#define STRBUF_SIZE 1600

// pokebw2's IntroMsg_Create
IntroMsg *func_ov204_021bf0a0(HeapID heapId) {
    IntroMsg *msg = GFL_HeapAllocate(heapId, sizeof(IntroMsg), TRUE, "intro_msg.c", 129);
    BmpWin *window;

    msg->heapId = heapId;
    msg->wordSet = GFL_WordSetSystemCreateDefault(heapId);
    msg->printQueue = func_0201c440(heapId);
    msg->tcbManager = GFL_TCBExMgrCreate(msg->heapId, msg->heapId, 2, 0);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, WINDOW_PALETTE * 0x20, 0x20, heapId);
    LoadSysMsgBox(1, FRAME_0_CHAR, FRAME_0_PALETTE, 0, heapId);
    LoadSysMsgBox(1, FRAME_1_CHAR, FRAME_1_PALETTE, 1, heapId);
    msg->font = GFL_FontCreate(ARCID_FONT, 0, 1, 0, msg->heapId);
    msg->strbuf = GFL_StrBufCreate(STRBUF_SIZE, msg->heapId);
    msg->expanded = GFL_StrBufCreate(STRBUF_SIZE, msg->heapId);
    msg->menuWindow = BmpWin_CreateDynamic(1, 22, 13, 9, 4, WINDOW_PALETTE, 1);
    GFL_BitmapFill(BmpWin_GetBitmap(msg->menuWindow), 0);
    window = msg->menuWindow;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
    msg->window = BmpWin_CreateDynamic(1, 1, 19, 30, 4, WINDOW_PALETTE, 1);
    msg->keyCursor = func_02028590(15, TRUE, TRUE, msg->heapId);
    msg->waitIcon = func_0202ef60(msg->heapId);
    return msg;
}

// pokebw2's IntroMsg_Free
void func_ov204_021bf1a4(IntroMsg *msg) {
    func_ov204_021bf580(msg);
    func_02028604(msg->keyCursor);
    GFL_StrBufFree(msg->strbuf);
    GFL_StrBufFree(msg->expanded);
    GFL_FontFree(msg->font);
    if (msg->printStream != NULL) {
        func_0201ce48(msg->printStream);
    }
    BmpWin_Free(msg->window);
    if (msg->menuWindow != NULL) {
        BmpWin_Free(msg->menuWindow);
    }
    GFL_TCBExMgrFree(msg->tcbManager);
    func_0201c4c0(msg->printQueue);
    if (msg->msgData != NULL) {
        GFL_MsgDataFree(msg->msgData);
    }
    GFL_WordSetSystemFree(msg->wordSet);
    GFL_HeapFree(msg);
}

// pokebw2's IntroMsg_LoadMessages
void func_ov204_021bf204(IntroMsg *msg, BOOL preload, u16 fileId) {
    if (msg->msgData != NULL) {
        GFL_MsgDataFree(msg->msgData);
    }
    msg->msgData = GFL_MsgSysLoadData(preload, ARCID_SYSTEM_MESSAGE, fileId, msg->heapId);
}

// pokebw2's IntroMsg_Update
void func_ov204_021bf228(IntroMsg *msg) {
    GFL_TCBExMgrUpdate(msg->tcbManager);
    func_0201c4e4(msg->printQueue);
}

// pokebw2's IntroMsg_Print
void func_ov204_021bf23c(IntroMsg *msg, u32 messageId, BOOL frame) {
    s32 wait = func_02012ffc();
    BmpWin *window = msg->window;

    GFL_BitmapFill(BmpWin_GetBitmap(window), 15);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    GFL_MsgDataLoadStrbuf(msg->msgData, messageId, msg->strbuf);
    GFL_WordSetFormatStrbuf(msg->wordSet, msg->expanded, msg->strbuf);
    msg->printStream = func_0201cce4(window, 4, 0, msg->expanded, msg->font, wait, msg->tcbManager, 0xffff,
                                     msg->heapId, 15);
    if (!frame) {
        BmpWin_DrawFrame(window, WINFRAME_TRANSFER_VBLANK, FRAME_0_CHAR, FRAME_0_PALETTE);
    } else {
        BmpWin_DrawFrame(window, WINFRAME_TRANSFER_VBLANK, FRAME_1_CHAR, FRAME_1_PALETTE);
    }
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
}

// pokebw2's IntroMsg_Clear
void func_ov204_021bf2d4(IntroMsg *msg) {
    func_0201f7a8(msg->window, WINFRAME_TRANSFER_NOW);
}

// pokebw2's IntroMsg_GetPrintState
u32 func_ov204_021bf2e0(IntroMsg *msg) {
    if (msg->printStream == NULL) {
        return PRINT_STREAM_DONE;
    }
    return func_0201ce30(msg->printStream);
}

// Prints the message on, with A, B or a touch hurrying it and going past its pauses. Returns TRUE once it has ended
// pokebw2's IntroMsg_UpdatePrint
BOOL func_ov204_021bf2f4(IntroMsg *msg) {
    if (msg->printStream != NULL) {
    func_020286c4(msg->keyCursor, msg->printStream, msg->window);
    switch (func_0201ce30(msg->printStream)) {
    case PRINT_STREAM_DONE:
        func_0201ce48(msg->printStream);
        msg->printStream = NULL;
        return TRUE;
    case PRINT_STREAM_PAUSED:
        if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_A || GCTX_HIDGetPressedKeys() == PAD_BUTTON_B || func_02035c60()) {
            func_0201ce38(msg->printStream);
            GFL_SndSEPlay(SEQ_SE_MESSAGE);
        }
        break;
    case PRINT_STREAM_RUNNING:
        if ((GCTX_HIDGetHeldKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c38()) {
            func_0201ce5c(msg->printStream, 0);
        }
        break;
    }
    return FALSE;
    }
    return FALSE;
}

// pokebw2's IntroMsg_InitMenuPrint
PrintWindow *func_ov204_021bf374(IntroMsg *msg, BmpWin *window) {
    msg->menuPrint.window = window;
    msg->menuPrint.flushPending = FALSE;
    return &msg->menuPrint;
}

void func_ov204_021bf384(IntroMsg *msg, const IntroMenuItem *items, u32 count, BOOL a3) {
    IntroMenu *menu;
    BmpWin *window;
    BmpMenuListHeader header;
    HeapID heapId;
    u32 i;

    menu = &msg->menu;
    heapId = msg->heapId;
    window = msg->menuWindow;
    MI_CpuFill8(menu, 0, sizeof(IntroMenu));
    menu->options = ListMenuCore_CreateOptionList(count, heapId);
    for (i = 0; i < count; i++) {
        GFL_MsgDataLoadStrbuf(msg->msgData, items[i].message, msg->strbuf);
        ListMenuCore_AppendStrBufOption(&menu->options[i], msg->strbuf, items[i].value, heapId);
    }
    MI_CpuFill8(&header, 0, sizeof(BmpMenuListHeader));
    header.options = menu->options;
    header.window = window;
    header.count = count;
    header.maxShown = 5;
    header.labelX = 0;
    header.itemX = 16;
    header.cursorX = 0;
    header.y = 2;
    header.fgColor = 1;
    header.bgColor = 15;
    header.shadowColor = 2;
    header.letterSpacing = 0;
    header.lineSpacing = 0;
    header.pageSkip = BMPMENULIST_SKIP_LR_KEY;
    header.fontId = 0;
    header.cursorDisplay = BMPMENULIST_CURSOR_SHOW;
    header.work = NULL;
    header.fontSizeX = 16;
    header.fontSizeY = 16;
    header.msgData = NULL;
    header.printWindow = func_ov204_021bf374(msg, window);
    header.queue = msg->printQueue;
    header.font = msg->font;
    menu->list = BmpMenuList_Create(&header, 0, 0, heapId);
    func_02020cf8(menu->list, heapId);
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
    BmpWin_DrawFrame(window, WINFRAME_TRANSFER_VBLANK, FRAME_0_CHAR, FRAME_0_PALETTE);
    if (a3) {
        func_02020d10(menu->list, 0);
    } else {
        func_02020d10(menu->list, 1);
    }
}


// pokebw2's IntroMsg_CloseMenu
void func_ov204_021bf4e8(IntroMsg *msg) {
    IntroMenu *menu = &msg->menu;

    func_0201f7a8(msg->menuWindow, WINFRAME_TRANSFER_NOW);
    GFL_BitmapFill(BmpWin_GetBitmap(msg->menuWindow), 0);
    BmpWin_FlushChar(msg->menuWindow);
    BmpMenuList_Free(msg->menu.list, NULL, NULL);
    ListMenuCore_FreeOptionList(menu->options);
    MI_CpuFill8(menu, 0, sizeof(IntroMenu));
}

// pokebw2's IntroMsg_UpdateMenu
void func_ov204_021bf528(IntroMsg *msg) {
    IntroMenu *menu = &msg->menu;

    menu->result = (s32)BmpMenuList_Update(menu->list);
}

// pokebw2's IntroMsg_GetMenuResult
u32 func_ov204_021bf538(IntroMsg *msg, s32 *value) {
    s32 result = msg->menu.result;

    switch (result) {
    case BMPMENULIST_NULL:
        return INTRO_MENU_NONE;
    case BMPMENULIST_CANCEL:
        return INTRO_MENU_CANCELLED;
    }
    if (value != NULL) {
        *value = result;
    }
    return INTRO_MENU_CHOSEN;
}

// pokebw2's IntroMsg_GetWordSet
WordSet *func_ov204_021bf55c(IntroMsg *msg) {
    return msg->wordSet;
}

// pokebw2's IntroMsg_ShowWaitIcon
void func_ov204_021bf560(IntroMsg *msg) {
    func_0202ef98(msg->waitIcon, GFL_VBlankGetTCBMgr(), msg->window, 15, 16);
}

// pokebw2's IntroMsg_HideWaitIcon
void func_ov204_021bf580(IntroMsg *msg) {
    if (msg->waitIcon != NULL) {
        func_0202f038(msg->waitIcon);
        msg->waitIcon = NULL;
    }
}
