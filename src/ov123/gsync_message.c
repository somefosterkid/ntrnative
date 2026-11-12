// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov199/gsync_message.c, which is the same code in
// Black 2/White 2's overlay 199, less GSyncMessage_Print and with the Game Sync ID's text made in pdwacc_message.c.
// The names are Black's dsd names; pokebw2's guesses are in the comments.
#include "app/gsync/gsync_message.h"
#include "app/gsync/pdwacc_message.h"
#include "constants/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/button_man.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "gfl/tcbl.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "system/app_keycursor.h"
#include "system/app_printsys_common.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/time_icon.h"
#include "system/wordset.h"

// Game Sync's messages: the message window, the info window below it and the yes/no menu. The name is the ROM's
// string, from GFL_HeapAllocate's call

#define GSYNC_MESSAGE_WIN_MAX 12

struct GSyncMessage {
    u32 unk0;
    // The frames of the info window and the message window, as LoadCursorImageEndOfHeap gives them
    u32 infoFrameChars;
    u32 frameChars;
    u8 unkC[0x10];
    ButtonMan *buttonMan;
    MsgData *msgData;
    WordSet *wordSet;
    Font *font;
    StrBuf *strBuf;
    StrBuf *tmpBuf;
    u8 unk34[0xc];
    KeyCursor *keyCursor;
    BmpWin *msgWin;
    BmpWin *infoWin;
    BmpWin *wins[GSYNC_MESSAGE_WIN_MAX];
    WaitIcon *waitIcon;
    PrintStream *printStream;
    TCBExManager *tcbMgr;
    PrintQueue *printQueue;
    AppTaskMenuItem yesNoItems[2];
    AppTaskMenuRes *menuRes;
    AppPrintsysCommon printCommon;
    HeapID heapId;
};

// pokebw2's GSyncMessage_DeleteButtons
static void func_ov123_021d54e0(GSyncMessage *msg) {
    if (msg->buttonMan != NULL) {
        func_02051c6c(msg->buttonMan);
        msg->buttonMan = NULL;
    }
}

GSyncMessage *func_ov123_021d54f4(HeapID heapId, u32 msgFile) {
    GSyncMessage *msg = GFL_HeapAllocate(heapId, sizeof(GSyncMessage), TRUE, "gsync_message.c", 150);

    msg->heapId = heapId;
    msg->wordSet = GFL_WordSetSystemCreateDefault(msg->heapId);
    BmpWin_InitAllocator(msg->heapId);
    func_0201dd60();
    msg->tcbMgr = GFL_TCBExMgrCreate(msg->heapId, msg->heapId, 2, 0);
    msg->printQueue = func_0201c440(msg->heapId);
    msg->strBuf = GFL_StrBufCreate(400, msg->heapId);
    msg->tmpBuf = GFL_StrBufCreate(400, msg->heapId);
    msg->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, msg->heapId);
    msg->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, msgFile, msg->heapId);
    msg->keyCursor = func_02028590(15, TRUE, TRUE, msg->heapId);
    msg->menuRes = func_02027f60(5, 9, msg->font, msg->printQueue, msg->heapId);
    msg->frameChars = LoadCursorImageEndOfHeap(5, 12, 0, msg->heapId);
    msg->infoFrameChars = LoadCursorImageEndOfHeap(6, 12, 0, msg->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_SUB_BG, 11 * 0x20, 0x20, msg->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_MAIN_BG, 11 * 0x20, 0x20, msg->heapId);
    return msg;
}

void func_ov123_021d562c(GSyncMessage *msg) {
    GFL_TCBExMgrUpdate(msg->tcbMgr);
    func_0201c4e4(msg->printQueue);
}

void func_ov123_021d5644(GSyncMessage *msg) {
    int i;

    func_ov123_021d584c(msg);
    if (msg->msgWin != NULL) {
        BmpWin_Free(msg->msgWin);
    }
    if (msg->infoWin != NULL) {
        BmpWin_Free(msg->infoWin);
    }
    for (i = 0; i < GSYNC_MESSAGE_WIN_MAX; i++) {
        if (msg->wins[i] != NULL) {
            BmpWin_Free(msg->wins[i]);
        }
    }
    GFL_BGSysFreeCharMemory(5, CHAR_POS(msg->frameChars), CHAR_SIZE(msg->frameChars));
    GFL_BGSysFreeCharMemory(6, CHAR_POS(msg->infoFrameChars), CHAR_SIZE(msg->infoFrameChars));
    func_ov123_021d54e0(msg);
    func_02028604(msg->keyCursor);
    GFL_WordSetSystemFree(msg->wordSet);
    func_0201dd68();
    GFL_MsgDataFree(msg->msgData);
    GFL_FontFree(msg->font);
    GFL_StrBufFree(msg->strBuf);
    GFL_StrBufFree(msg->tmpBuf);
    func_02027fd8(msg->menuRes);
    func_0201c6ec(msg->printQueue);
    func_0201c4c0(msg->printQueue);
    GFL_TCBExMgrFree(msg->tcbMgr);
    BmpWin_FreeAllocator();
    GFL_HeapFree(msg);
}

void func_ov123_021d56fc(GSyncMessage *msg, BOOL now) {
    BmpWin *win;

    if (msg->msgWin == NULL) {
        msg->msgWin = BmpWin_CreateDynamic(5, 1, 1, 30, 4, 11, TRUE);
    }
    win = msg->msgWin;
    GFL_BitmapFill(BmpWin_GetBitmap(win), 15);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    if (now) {
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(win), 0, 0, msg->strBuf, msg->font);
    } else {
        msg->printStream =
            func_0201cce4(win, 0, 0, msg->strBuf, msg->font, func_02012ffc(), msg->tcbMgr, 2, msg->heapId, 15);
        func_02028470(&msg->printCommon, APP_PRINTSYS_COMMON_KEYS | APP_PRINTSYS_COMMON_TOUCH);
    }
    BmpWin_DrawFrame(win, TRUE, msg->frameChars, 12);
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    GFL_BGSysQueueScrLoad(5);
}

void func_ov123_021d57b8(GSyncMessage *msg, u32 msgId) {
    GFL_MsgDataLoadStrbuf(msg->msgData, msgId, msg->strBuf);
    func_ov123_021d56fc(msg, FALSE);
}

void func_ov123_021d57d0(GSyncMessage *msg, u32 msgId, s32 number, PartyPkm *pkm) {
    GFL_MsgDataLoadStrbuf(msg->msgData, msgId, msg->tmpBuf);
    loadPokemonNicknameToStrbuf(msg->wordSet, 0, pkm);
    WordSetNumber(msg->wordSet, 1, number, 3, 1, TRUE);
    GFL_WordSetFormatStrbuf(msg->wordSet, msg->strBuf, msg->tmpBuf);
}

BOOL func_ov123_021d580c(GSyncMessage *msg) {
    if (msg->printStream != NULL) {
        func_020286c4(msg->keyCursor, msg->printStream, msg->msgWin);
        if (!func_02028484(&msg->printCommon, msg->printStream)) {
            return FALSE;
        }
        func_0201ce48(msg->printStream);
        msg->printStream = NULL;
    }
    return TRUE;
}

void func_ov123_021d584c(GSyncMessage *msg) {
    if (msg->waitIcon != NULL) {
        func_0202f038(msg->waitIcon);
        msg->waitIcon = NULL;
    }
    if (msg->printStream != NULL) {
        func_0201ce48(msg->printStream);
        msg->printStream = NULL;
    }
    if (msg->msgWin != NULL) {
        func_0201f7a8(msg->msgWin, 2);
        func_020456f8(msg->msgWin);
        GFL_BGSysQueueScrLoad(5);
    }
}

AppTaskMenu *func_ov123_021d5890(GSyncMessage *msg, int pos) {
    AppTaskMenuInit init;
    AppTaskMenu *menu;

    init.heapId = msg->heapId;
    init.itemCount = 2;
    init.items = msg->yesNoItems;
    switch (pos) {
    case GSYNC_YESNO_POS_UPPER:
        init.x = 32;
        init.y = 12;
        init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
        break;
    case GSYNC_YESNO_POS_LOWER:
        init.x = 32;
        init.y = 24;
        init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
        break;
    }
    init.width = 13;
    init.height = 3;
    msg->yesNoItems[0].str = GFL_StrBufCreate(100, msg->heapId);
    GFL_MsgDataLoadStrbuf(msg->msgData, 4, msg->yesNoItems[0].str);
    msg->yesNoItems[0].color = PRINT_COLOR(14, 15, 0);
    msg->yesNoItems[1].str = GFL_StrBufCreate(100, msg->heapId);
    GFL_MsgDataLoadStrbuf(msg->msgData, 5, msg->yesNoItems[1].str);
    msg->yesNoItems[1].color = PRINT_COLOR(14, 15, 0);
    menu = func_02027778(&init, msg->menuRes);
    GFL_StrBufFree(msg->yesNoItems[0].str);
    GFL_StrBufFree(msg->yesNoItems[1].str);
    G2S_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG3, -8);
    return menu;
}

void func_ov123_021d5950(GSyncMessage *msg, u32 msgId) {
    GFL_MsgDataLoadStrbuf(msg->msgData, msgId, msg->strBuf);
}

void func_ov123_021d5960(GSyncMessage *msg, int y, int height) {
    BmpWin *win;

    if (msg->infoWin != NULL) {
        BmpWin_Free(msg->infoWin);
    }
    win = BmpWin_CreateDynamic(6, 1, y, 30, height, 11, TRUE);
    msg->infoWin = win;
    GFL_BitmapFill(BmpWin_GetBitmap(win), 15);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(win), 0, 0, msg->strBuf, msg->font);
    BmpWin_DrawFrame(win, TRUE, msg->infoFrameChars, 12);
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    GFL_BGSysQueueScrLoad(6);
}

void func_ov123_021d59e4(GSyncMessage *msg) {
    BmpWin *win;

    if (msg->infoWin != NULL) {
        BmpWin_Free(msg->infoWin);
    }
    win = BmpWin_CreateDynamic(6, 1, 9, 30, 6, 11, TRUE);
    msg->infoWin = win;
    GFL_BitmapFill(BmpWin_GetBitmap(win), 15);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(win), 0, 0, msg->strBuf, msg->font);
    BmpWin_DrawFrame(win, TRUE, msg->infoFrameChars, 12);
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    GFL_BGSysQueueScrLoad(6);
}

void func_ov123_021d5a60(GSyncMessage *msg) {
    func_0201f7a8(msg->infoWin, 2);
    func_020456f8(msg->infoWin);
    GFL_BGSysQueueScrLoad(6);
}

void func_ov123_021d5a7c(GSyncMessage *msg) {
    if (msg->waitIcon != NULL) {
        func_0202f038(msg->waitIcon);
        msg->waitIcon = NULL;
    }
    msg->waitIcon = func_0202ee8c(msg->tcbMgr, msg->msgWin, 15, 16, msg->heapId);
}

void func_ov123_021d5ab0(GSyncMessage *msg, u32 id) {
    func_ov123_021d97cc(id, msg->strBuf);
}
