// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov190/wifilogin_message.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Wi-Fi login and logout's messages and menus: a message window printed a character at a time, a title window and
// a yes/no menu. The name is the ROM's string, from GFL_HeapAllocate's call. Function names are ours.

#include "app/wifilogin_message.h"
#include "types.h"
#include "constants/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "nitro/ioreg.h"
#include "system/app_printsys_common.h"
#include "system/app_taskmenu.h"
#include "system/bmp_menu.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/time_icon.h"
#include "system/wordset.h"

#define WIFILOGIN_MESSAGE_TITLE_BG 1

static void func_ov115_021d5c30(WifiLoginMessage *msg, MsgData *msgData, u32 msgNo);

// pokebw2's WifiLoginMessage_Create
WifiLoginMessage *func_ov115_021d5984(HeapID heapId, u32 bank, u32 style) {
    WifiLoginMessage *msg = GFL_HeapAllocate(heapId, sizeof(WifiLoginMessage), TRUE, "wifilogin_message.c", 135);

    msg->heapId = heapId;
    msg->style = style;
    BmpWin_InitAllocator(msg->heapId);
    func_0201dd60();
    msg->tcbManager = GFL_TCBExMgrCreate(msg->heapId, msg->heapId, 1, 0);
    msg->printQueue = func_0201c440(msg->heapId);
    msg->strbuf = GFL_StrBufCreate(400, msg->heapId);
    msg->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, msg->heapId);
    msg->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, bank, msg->heapId);
    msg->wordSet = GFL_WordSetSystemCreateDefault(msg->heapId);
    msg->menu.res = func_02027f60(5, 9, msg->font, msg->printQueue, msg->heapId);
    msg->frameChars = LoadCursorImageEndOfHeap(func_ov115_021d5fa0(msg->style), 12, 0, msg->heapId);
    msg->frameCharsOther = LoadCursorImageEndOfHeap(func_ov115_021d5fc4(msg->style), 12, 0, msg->heapId);
    u32 palType = PALTYPE_MAIN_BG;

    if (msg->style == WIFILOGIN_MESSAGE_STYLE_PDWACC) {
        palType = PALTYPE_SUB_BG;
    }
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, palType, 0x160, 0x20, msg->heapId);
    return msg;
}

// pokebw2's WifiLoginMessage_Update
void func_ov115_021d5a88(WifiLoginMessage *msg) {
    GFL_TCBExMgrUpdate(msg->tcbManager);
    func_0201c4e4(msg->printQueue);
}

// pokebw2's WifiLoginMessage_Free
void func_ov115_021d5a9c(WifiLoginMessage *msg) {
    if (msg->waitIcon != NULL) {
        func_0202f038(msg->waitIcon);
        msg->waitIcon = NULL;
    }
    if (msg->stream != NULL) {
        func_0201ce48(msg->stream);
        msg->stream = NULL;
    }
    func_ov115_021d5d3c(msg);
    GFL_BGSysFreeCharMemory(func_ov115_021d5fa0(msg->style), CHAR_POS(msg->frameChars), CHAR_SIZE(msg->frameChars));
    GFL_BGSysFreeCharMemory(func_ov115_021d5fc4(msg->style), CHAR_POS(msg->frameCharsOther),
                            CHAR_SIZE(msg->frameCharsOther));
    func_0201dd68();
    GFL_WordSetSystemFree(msg->wordSet);
    GFL_MsgDataFree(msg->msgData);
    GFL_FontFree(msg->font);
    GFL_StrBufFree(msg->strbuf);
    func_02027fd8(msg->menu.res);
    GFL_TCBExMgrFree(msg->tcbManager);
    func_0201c6ec(msg->printQueue);
    func_0201c4c0(msg->printQueue);
    if (msg->messageWin != NULL) {
        BmpWin_Free(msg->messageWin);
        msg->messageWin = NULL;
    }
    if (msg->unk30 != NULL) {
        BmpWin_Free(msg->unk30);
        msg->unk30 = NULL;
    }
    BmpWin_FreeAllocator();
    GFL_HeapFree(msg);
}

// pokebw2's WifiLoginMessage_Print
void func_ov115_021d5b50(WifiLoginMessage *msg, u32 msgNo) {
    func_ov115_021d5b5c(msg, msg->msgData, msgNo);
}

// Whether the message window is the PDWACC style's
static inline BOOL WifiLoginMessage_IsPdwacc(const WifiLoginMessage *msg) {
    return msg->style == WIFILOGIN_MESSAGE_STYLE_PDWACC;
}

// pokebw2's WifiLoginMessage_PrintFrom
void func_ov115_021d5b5c(WifiLoginMessage *msg, MsgData *msgData, u32 msgNo) {
    BmpWin *window;
    u8 y;
    u32 flags;

    GFL_MsgDataLoadStrbuf(msgData, msgNo, msg->strbuf);
    y = 1;
    if (!WifiLoginMessage_IsPdwacc(msg)) {
        y = 19;
    }
    if (msg->messageWin == NULL) {
        msg->messageWin = BmpWin_CreateDynamic(func_ov115_021d5fa0(msg->style), 1, y, 30, 4, 11, TRUE);
    }
    window = msg->messageWin;
    GFL_BitmapFill(BmpWin_GetBitmap(window), 15);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    flags = APP_PRINTSYS_COMMON_KEYS;
    if (WifiLoginMessage_IsPdwacc(msg)) {
        flags = APP_PRINTSYS_COMMON_KEYS | APP_PRINTSYS_COMMON_TOUCH;
    }
    func_02028470(&msg->printsys, flags);
    if (msg->stream != NULL) {
        func_0201ce48(msg->stream);
    }
    msg->stream =
        func_0201cce4(window, 0, 0, msg->strbuf, msg->font, func_02012ffc(), msg->tcbManager, 2, msg->heapId, 15);
    BmpWin_DrawFrame(window, WINFRAME_TRANSFER_VBLANK, msg->frameChars, 12);
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(func_ov115_021d5fa0(msg->style));
}

// pokebw2's WifiLoginMessage_PrintWaiting
void func_ov115_021d5c24(WifiLoginMessage *msg, u32 msgNo) {
    func_ov115_021d5c30(msg, msg->msgData, msgNo);
}

// Prints a message and shows the wait icon on its window
// pokebw2's WifiLoginMessage_PrintWaitingFrom
static void func_ov115_021d5c30(WifiLoginMessage *msg, MsgData *msgData, u32 msgNo) {
    func_ov115_021d5b5c(msg, msgData, msgNo);
    if (msg->waitIcon != NULL) {
        func_0202f038(msg->waitIcon);
        msg->waitIcon = NULL;
    }
    msg->waitIcon = func_0202ee30(GFL_VBlankGetTCBMgr(), msg->messageWin, 15, 16, msg->heapId);
}

// Whether the message is printed and was continued
// pokebw2's WifiLoginMessage_IsDone
BOOL func_ov115_021d5c70(WifiLoginMessage *msg) {
    BOOL done = TRUE;

    if (msg->stream != NULL) {
        done = func_02028484(&msg->printsys, msg->stream);
        if (done) {
            func_0201ce48(msg->stream);
            msg->stream = NULL;
        }
    }
    return done;
}

// pokebw2's WifiLoginMessage_ClearWindow
void func_ov115_021d5c94(WifiLoginMessage *msg) {
    if (msg->waitIcon != NULL) {
        func_0202f038(msg->waitIcon);
        msg->waitIcon = NULL;
    }
    if (msg->messageWin != NULL) {
        func_0201f7a8(msg->messageWin, WINFRAME_TRANSFER_NONE);
        func_020456f8(msg->messageWin);
        GFL_BGSysQueueScrLoad(func_ov115_021d5fa0(msg->style));
    }
}

// Prints a title in the title window
// pokebw2's WifiLoginMessage_PrintTitle
void func_ov115_021d5ccc(WifiLoginMessage *msg, u32 msgNo) {
    BmpWin *window;

    if (msg->titleWin == NULL) {
        msg->titleWin = BmpWin_CreateDynamic(WIFILOGIN_MESSAGE_TITLE_BG, 4, 7, 24, 2, 11, TRUE);
    }
    window = msg->titleWin;
    GFL_BitmapFill(BmpWin_GetBitmap(window), 0);
    GFL_MsgDataLoadStrbuf(msg->msgData, msgNo, msg->strbuf);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(window), 0, 0, msg->strbuf, msg->font, PRINT_COLOR(15, 6, 0));
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(WIFILOGIN_MESSAGE_TITLE_BG);
}

// pokebw2's WifiLoginMessage_ClearTitle
void func_ov115_021d5d3c(WifiLoginMessage *msg) {
    if (msg->titleWin != NULL) {
        func_020456f8(msg->titleWin);
        BmpWin_Free(msg->titleWin);
        msg->titleWin = NULL;
        GFL_BGSysQueueScrLoad(WIFILOGIN_MESSAGE_TITLE_BG);
    }
}

// Creates the yes/no menu: task menu buttons or a confirm dialog, as the style says. layout 0 or 1 puts it higher or
// lower on the screen
// pokebw2's WifiLoginMessage_CreateMenu
WifiLoginMessageMenu *func_ov115_021d5d5c(WifiLoginMessage *msg, u32 layout, BOOL dimTopScreen, u32 cursorPos) {
    WifiLoginMessageMenu *menu = &msg->menu;

    menu->style = msg->style;
    switch (menu->style) {
    case WIFILOGIN_MESSAGE_STYLE_PDWACC: {
        AppTaskMenuInit init;

        init.heapId = msg->heapId;
        init.itemCount = 2;
        init.items = menu->items;
        switch (layout) {
        case 0:
            init.x = 32;
            init.y = 13;
            init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
            break;
        case 1:
            init.x = 32;
            init.y = 24;
            init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
            break;
        }
        init.width = 13;
        init.height = 3;
        menu->items[0].str = GFL_StrBufCreate(100, msg->heapId);
        GFL_MsgDataLoadStrbuf(msg->msgData, 28, menu->items[0].str);
        menu->items[0].color = PRINT_COLOR(14, 15, 0);
        menu->items[1].str = GFL_StrBufCreate(100, msg->heapId);
        GFL_MsgDataLoadStrbuf(msg->msgData, 29, menu->items[1].str);
        menu->items[1].color = PRINT_COLOR(14, 15, 0);
        menu->taskMenu = func_02027778(&init, menu->res);
        GFL_StrBufFree(menu->items[0].str);
        GFL_StrBufFree(menu->items[1].str);
        if (dimTopScreen) {
            G2S_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_OBJ, -8);
        }
        func_02027cfc(menu->taskMenu, cursorPos);
        break;
    }
    case WIFILOGIN_MESSAGE_STYLE_GSYNC:
        menu->dialog = func_ov115_021d5fe8(msg, layout, cursorPos);
        menu->result = BMPMENU_NULL;
        break;
    }
    return menu;
}

// pokebw2's WifiLoginMessage_FreeMenu
void func_ov115_021d5e34(WifiLoginMessageMenu *menu) {
    switch (menu->style) {
    case WIFILOGIN_MESSAGE_STYLE_PDWACC:
        func_02027858(menu->taskMenu);
        break;
    case WIFILOGIN_MESSAGE_STYLE_GSYNC:
        break;
    }
}

void func_ov115_021d5e48(WifiLoginMessageMenu *menu) {
    switch (menu->style) {
    case WIFILOGIN_MESSAGE_STYLE_PDWACC:
        func_02027974(menu->taskMenu);
        break;
    case WIFILOGIN_MESSAGE_STYLE_GSYNC:
        menu->result = func_0201febc(menu->dialog);
        break;
    }
}


// Whether the player has answered the yes/no menu
// pokebw2's WifiLoginMessage_IsMenuAnswered
BOOL func_ov115_021d5e6c(WifiLoginMessageMenu *menu) {
    switch (menu->style) {
    case WIFILOGIN_MESSAGE_STYLE_PDWACC:
        return func_020279e8(menu->taskMenu);
    case WIFILOGIN_MESSAGE_STYLE_GSYNC:
        if (menu->result != BMPMENU_NULL) {
            return TRUE;
        }
        return FALSE;
    default:
        GFL_ASSERT(0);
        return FALSE;
    }
}

u8 func_ov115_021d5ea8(WifiLoginMessageMenu *menu) {
    switch (menu->style) {
    case WIFILOGIN_MESSAGE_STYLE_PDWACC:
        return func_02027a04(menu->taskMenu);
    case WIFILOGIN_MESSAGE_STYLE_GSYNC:
        return menu->result;
    default:
        GFL_ASSERT(0);
        return 0;
    }
}


// Prints a message in the window of the notices, which it creates the first time
// pokebw2's WifiLoginMessage_PrintNotice
void func_ov115_021d5ed8(WifiLoginMessage *msg, u32 msgNo) {
    StrBuf *formatted;
    BmpWin *window;

    GFL_MsgDataLoadStrbuf(msg->msgData, msgNo, msg->strbuf);
    formatted = GFL_StrBufClone(msg->strbuf, msg->heapId);
    GFL_WordSetFormatStrbuf(msg->wordSet, msg->strbuf, formatted);
    if (msg->unk30 == NULL) {
        msg->unk30 = BmpWin_CreateDynamic(func_ov115_021d5fc4(msg->style), 1, 1, 30, 16, 11, TRUE);
    }
    window = msg->unk30;
    GFL_BitmapFill(BmpWin_GetBitmap(window), 15);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 0, 0, msg->strbuf, msg->font);
    BmpWin_DrawFrame(window, WINFRAME_TRANSFER_VBLANK, msg->frameCharsOther, 12);
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(func_ov115_021d5fc4(msg->style));
    GFL_StrBufFree(formatted);
}

// Clears the window of the notices
// pokebw2's WifiLoginMessage_ClearNotice
void func_ov115_021d5f7c(WifiLoginMessage *msg) {
    if (msg->unk30 != NULL) {
        func_0201f7a8(msg->unk30, WINFRAME_TRANSFER_NONE);
        func_020456f8(msg->unk30);
        GFL_BGSysQueueScrLoad(func_ov115_021d5fc4(msg->style));
    }
}

// The BG of the message window
// pokebw2's WifiLoginMessage_GetMessageBG
u8 func_ov115_021d5fa0(u32 style) {
    switch (style) {
    case WIFILOGIN_MESSAGE_STYLE_GSYNC:
        return 1;
    case WIFILOGIN_MESSAGE_STYLE_PDWACC:
        return 5;
    default:
        GFL_ASSERT(0);
        return 0;
    }
}

// The BG of the window of the notices
// pokebw2's WifiLoginMessage_GetNoticeBG
u8 func_ov115_021d5fc4(u32 style) {
    switch (style) {
    case WIFILOGIN_MESSAGE_STYLE_GSYNC:
        return 2;
    case WIFILOGIN_MESSAGE_STYLE_PDWACC:
        return 5;
    default:
        GFL_ASSERT(0);
        return 0;
    }
}

// The confirm dialog of the GSYNC style, with its place on the screen by layout
// pokebw2's WifiLoginMessage_CreateConfirmDialog
BmpMenu *func_ov115_021d5fe8(WifiLoginMessage *msg, u32 layout, u32 cursorPos) {
    ConfirmDialogSetup setup = {1, 0, 0, 11, 0};

    setup.unk4 = msg->frameChars;
    switch (layout) {
    case 0:
        setup.x = 24;
        setup.y = 13;
        break;
    case 1:
        setup.x = 24;
        setup.y = 19;
        break;
    }
    return ShopUI_CreateConfirmDialog(&setup, setup.unk4, 12, cursorPos, msg->heapId);
}
