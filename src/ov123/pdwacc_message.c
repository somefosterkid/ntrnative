// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov199/pdwacc_message.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Dream World account screens' messages: the message window, an info window, the yes/no menu and the Game Sync
// ID. The name is the ROM's string, from GFL_HeapAllocate's call. Function names are ours.

#include "types.h"
#include "app/gsync/pdwacc_message.h"
#include "constants/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/button_man.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcbl.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/time_icon.h"
#include "system/wordset.h"

#define PDWACC_MESSAGE_WIN_MAX 12

struct PdwAccMessage {
    // The frames of the Game Sync ID's window, the info window and the message window, as LoadCursorImageEndOfHeap
    // gives them
    u32 idFrameChars;
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
    BmpWin *idWin;
    BmpWin *idLabelWin;
    BmpWin *msgWin;
    BmpWin *infoWin;
    BmpWin *wins[PDWACC_MESSAGE_WIN_MAX];
    PrintStream *printStream;
    u32 unk84;
    TCBExManager *tcbMgr;
    PrintQueue *printQueue;
    WaitIcon *waitIcon;
    AppTaskMenuItem yesNoItems[2];
    AppTaskMenuRes *menuRes;
    HeapID heapId;
};

static void func_ov123_021d92b8(PdwAccMessage *msg);

// pokebw2's PdwAccMessage_DeleteButtons
static void func_ov123_021d92b8(PdwAccMessage *msg) {
    if (msg->buttonMan != NULL) {
        func_02051c6c(msg->buttonMan);
        msg->buttonMan = NULL;
    }
}

// pokebw2's PdwAccMessage_Create
PdwAccMessage *func_ov123_021d92cc(HeapID heapId, u32 msgFile) {
    PdwAccMessage *msg = GFL_HeapAllocate(heapId, sizeof(PdwAccMessage), TRUE, "pdwacc_message.c", 155);

    msg->heapId = heapId;
    BmpWin_InitAllocator(msg->heapId);
    func_0201dd60();
    msg->tcbMgr = GFL_TCBExMgrCreate(msg->heapId, msg->heapId, 2, 0);
    msg->printQueue = func_0201c440(msg->heapId);
    msg->strBuf = GFL_StrBufCreate(400, msg->heapId);
    msg->tmpBuf = GFL_StrBufCreate(400, msg->heapId);
    msg->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, msg->heapId);
    msg->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, msgFile, msg->heapId);
    msg->wordSet = GFL_WordSetSystemCreateDefault(msg->heapId);
    msg->menuRes = func_02027f60(5, 9, msg->font, msg->printQueue, msg->heapId);
    msg->idFrameChars = LoadCursorImageEndOfHeap(1, 12, 0, msg->heapId);
    msg->frameChars = LoadCursorImageEndOfHeap(5, 12, 0, msg->heapId);
    msg->infoFrameChars = LoadCursorImageEndOfHeap(6, 12, 0, msg->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_SUB_BG, 11 * 0x20, 0x20, msg->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_MAIN_BG, 11 * 0x20, 0x20, msg->heapId);
    return msg;
}

// pokebw2's PdwAccMessage_Main
void func_ov123_021d9404(PdwAccMessage *msg) {
    GFL_TCBExMgrUpdate(msg->tcbMgr);
    func_0201c4e4(msg->printQueue);
}

// pokebw2's PdwAccMessage_Free
void func_ov123_021d941c(PdwAccMessage *msg) {
    int i;

    GFL_BGSysFreeCharMemory(1, CHAR_POS(msg->idFrameChars), CHAR_SIZE(msg->idFrameChars));
    GFL_BGSysFreeCharMemory(5, CHAR_POS(msg->frameChars), CHAR_SIZE(msg->frameChars));
    GFL_BGSysFreeCharMemory(6, CHAR_POS(msg->infoFrameChars), CHAR_SIZE(msg->infoFrameChars));
    func_ov123_021d92b8(msg);
    func_0201dd68();
    GFL_MsgDataFree(msg->msgData);
    GFL_FontFree(msg->font);
    GFL_StrBufFree(msg->strBuf);
    GFL_StrBufFree(msg->tmpBuf);
    GFL_WordSetSystemFree(msg->wordSet);
    func_02027fd8(msg->menuRes);
    func_0201c6ec(msg->printQueue);
    func_0201c4c0(msg->printQueue);
    if (msg->printStream != NULL) {
        func_0201ce48(msg->printStream);
    }
    if (msg->waitIcon != NULL) {
        func_0202f038(msg->waitIcon);
        msg->waitIcon = NULL;
    }
    if (msg->idWin != NULL) {
        BmpWin_Free(msg->idWin);
    }
    if (msg->idLabelWin != NULL) {
        BmpWin_Free(msg->idLabelWin);
    }
    if (msg->msgWin != NULL) {
        BmpWin_Free(msg->msgWin);
    }
    if (msg->infoWin != NULL) {
        BmpWin_Free(msg->infoWin);
    }
    for (i = 0; i < PDWACC_MESSAGE_WIN_MAX; i++) {
        if (msg->wins[i] != NULL) {
            BmpWin_Free(msg->wins[i]);
        }
    }
    GFL_TCBExMgrFree(msg->tcbMgr);
    BmpWin_FreeAllocator();
    GFL_HeapFree(msg);
}

// pokebw2's PdwAccMessage_PrintStream
void func_ov123_021d9514(PdwAccMessage *msg, u32 msgId) {
    BmpWin *win;

    GFL_MsgDataLoadStrbuf(msg->msgData, msgId, msg->strBuf);
    if (msg->msgWin == NULL) {
        msg->msgWin = BmpWin_CreateDynamic(5, 1, 3, 30, 4, 11, TRUE);
    }
    win = msg->msgWin;
    GFL_BitmapFill(BmpWin_GetBitmap(win), 15);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    msg->printStream =
        func_0201cce4(win, 0, 0, msg->strBuf, msg->font, func_02012ffc(), msg->tcbMgr, 2, msg->heapId, 15);
    BmpWin_DrawFrame(win, TRUE, msg->frameChars, 12);
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    GFL_BGSysQueueScrLoad(5);
}

// pokebw2's PdwAccMessage_StartWaitIcon
void func_ov123_021d95b4(PdwAccMessage *msg) {
    if (msg->waitIcon != NULL) {
        func_0202f038(msg->waitIcon);
        msg->waitIcon = NULL;
    }
    msg->waitIcon = func_0202ee8c(msg->tcbMgr, msg->msgWin, 15, 16, msg->heapId);
}

// The message goes on only with A
// pokebw2's PdwAccMessage_IsPrintFinished
BOOL func_ov123_021d95f0(PdwAccMessage *msg) {
    if (msg->printStream != NULL) {
        switch (func_0201ce30(msg->printStream)) {
        case PRINT_STREAM_DONE:
            func_0201ce48(msg->printStream);
            msg->printStream = NULL;
            break;
        case PRINT_STREAM_PAUSED:
            if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_A) {
                func_0201ce38(msg->printStream);
            }
            break;
        }
        return FALSE;
    }
    return TRUE;
}

// pokebw2's PdwAccMessage_ClearMessage
void func_ov123_021d9634(PdwAccMessage *msg) {
    if (msg->waitIcon != NULL) {
        func_0202f038(msg->waitIcon);
        msg->waitIcon = NULL;
    }
    if (msg->msgWin != NULL) {
        func_0201f7a8(msg->msgWin, 2);
        func_020456f8(msg->msgWin);
        GFL_BGSysQueueScrLoad(5);
        BmpWin_Free(msg->msgWin);
        msg->msgWin = NULL;
    }
}

// pokebw2's PdwAccMessage_CreateYesNo
AppTaskMenu *func_ov123_021d9670(PdwAccMessage *msg, int pos) {
    AppTaskMenuInit init;
    AppTaskMenu *menu;

    init.heapId = msg->heapId;
    init.itemCount = 2;
    init.items = msg->yesNoItems;
    switch (pos) {
    case PDWACC_YESNO_POS_UPPER:
        init.x = 32;
        init.y = 14;
        init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
        break;
    case PDWACC_YESNO_POS_LOWER:
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
    G2S_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_OBJ, -8);
    return menu;
}

// pokebw2's PdwAccMessage_PrintInfo
void func_ov123_021d9730(PdwAccMessage *msg, u32 msgId) {
    BmpWin *win;

    GFL_MsgDataLoadStrbuf(msg->msgData, msgId, msg->strBuf);
    if (msg->infoWin == NULL) {
        msg->infoWin = BmpWin_CreateDynamic(6, 1, 3, 30, 16, 11, TRUE);
    }
    win = msg->infoWin;
    GFL_BitmapFill(BmpWin_GetBitmap(win), 15);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(win), 0, 0, msg->strBuf, msg->font);
    BmpWin_DrawFrame(win, TRUE, msg->infoFrameChars, 12);
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    GFL_BGSysQueueScrLoad(6);
}

// pokebw2's PdwAccMessage_ClearInfo
void func_ov123_021d97b0(PdwAccMessage *msg) {
    func_0201f7a8(msg->infoWin, 2);
    func_020456f8(msg->infoWin);
    GFL_BGSysQueueScrLoad(6);
}

void func_ov123_021d97cc(u32 id, StrBuf *strBuf) {
    u16 chars[32] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R',
        'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '2', '3', '4', '5', '6', '7', '8', '9',
    };
    s32 seed = id;
    u16 crc = getCRC16(&seed, 4);
    u16 params[1] = { 1 };
    u64 val = (s64)seed + ((u64)crc << 32);
    s32 i;

    GFL_StrBufClear(strBuf);
    for (i = 0; i < 10; i++) {
        u16 idx = val & 0x1f;
        val >>= 5;
        GFL_StrBufAppend(strBuf, chars[idx]);
        GFL_StrCmdBuild(strBuf, 0xbd, 4, 1, params);
    }
}


void func_ov123_021d9844(PdwAccMessage *msg, u32 id, u32 unused) {
    BmpWin *win;
    BmpWin *labelWin;

    func_ov123_021d97cc(id, msg->strBuf);
    if (msg->idWin == NULL) {
        msg->idWin = BmpWin_CreateDynamic(1, 1, 8, 30, 4, 11, 1);
    }
    win = msg->idWin;
    GFL_BitmapFill(BmpWin_GetBitmap(win), 15);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(win), 0, 0, msg->strBuf, msg->font);
    BmpWin_DrawFrame(win, TRUE, msg->idFrameChars, 12);
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    if (msg->idLabelWin == NULL) {
        msg->idLabelWin = BmpWin_CreateDynamic(1, 3, 3, 22, 2, 11, 1);
    }
    labelWin = msg->idLabelWin;
    GFL_TextRndUpdateColorIndexLUT(1, 2, 0);
    GFL_MsgDataLoadStrbuf(msg->msgData, 10, msg->strBuf);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(labelWin), 0, 0, msg->strBuf, msg->font);
    BmpWin_FlushChar(labelWin);
    BmpWin_FlushMap(labelWin);
    GFL_BGSysQueueScrLoad(5);
}


// pokebw2's PdwAccMessage_ClearGSyncId
void func_ov123_021d9918(PdwAccMessage *msg) {
    if (msg->idWin != NULL) {
        func_0201f7a8(msg->idWin, 2);
        func_020456f8(msg->idWin);
        func_0201f7a8(msg->idLabelWin, 2);
        func_020456f8(msg->idLabelWin);
        BmpWin_Free(msg->idWin);
        BmpWin_Free(msg->idLabelWin);
        GFL_BGSysQueueScrLoad(5);
        msg->idWin = NULL;
        msg->idLabelWin = NULL;
    }
}
