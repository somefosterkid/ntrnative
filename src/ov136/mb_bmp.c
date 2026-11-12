// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov215/mb_bmp.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/mailbox_local.h"
#include "constants/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "gfl/tcbl.h"
#include "pml/mail.h"
#include "system/bgwinfrm.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/pms_data.h"
#include "system/pms_draw.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wordset.h"
#include "text/system/btl_server_flow_2.h"
#include "field/beacon_view.h"

// The mailbox screen's windows and what is printed into them. The ROM doesn't name this file; the name is a guess,
// after box2_bmp.c

// Black 2's; twinned code confirms the offsets of bg, x, y, width, height and palette in Black
typedef struct {
    u32 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u16 palette;
    u16 baseChar;
} MailBoxWindowTemplate;

static void func_ov136_021c5f80(BmpWin *window, StrBuf *str, MailBoxPrintState *state, PrintQueue *queue, int x, int y,
                                Font *font, u16 color, u32 align);
static void func_ov136_021c5fdc(MailBoxView *view, MsgData *msgData, u32 index, u32 messageId, int x, int y,
                                   Font *font, u16 color, u32 align);
static void func_ov136_021c61d4(MailBoxWork *work, int unused);
static void func_ov136_021c6230(MailBoxWork *work);

// pokebw2's sWindowTemplates
static const MailBoxWindowTemplate data_ov136_021c7a20[MAILBOX_WINDOW_COUNT] = {
    { 2, 0, 24, 1, 1, 1, 0x3f0 },   { 6, 0, 24, 1, 1, 1, 0x3f0 },   { 2, 4, 1, 8, 2, 1, 0x3f0 },
    { 2, 4, 1, 8, 2, 1, 0x3e0 },    { 2, 4, 1, 8, 2, 1, 0x3d0 },    { 2, 4, 1, 8, 2, 1, 0x3c0 },
    { 2, 4, 1, 8, 2, 1, 0x3b0 },    { 2, 4, 1, 8, 2, 1, 0x3a0 },    { 2, 4, 1, 8, 2, 1, 0x390 },
    { 2, 4, 1, 8, 2, 1, 0x380 },    { 2, 4, 1, 8, 2, 1, 0x370 },    { 2, 4, 1, 8, 2, 1, 0x360 },
    { 2, 4, 1, 8, 2, 1, 0x350 },    { 2, 4, 1, 8, 2, 1, 0x340 },    { 2, 4, 1, 8, 2, 1, 0x330 },
    { 2, 4, 1, 8, 2, 1, 0x320 },    { 2, 4, 1, 8, 2, 1, 0x310 },    { 2, 4, 1, 8, 2, 1, 0x300 },
    { 2, 4, 1, 8, 2, 1, 0x2f0 },    { 2, 4, 1, 8, 2, 1, 0x2e0 },    { 2, 4, 1, 8, 2, 1, 0x2d0 },
    { 2, 4, 1, 8, 2, 1, 0x2c0 },    { 1, 4, 1, 8, 2, 1, 0x3f0 },    { 1, 25, 21, 6, 2, 15, 0x3d8 },
    { 1, 10, 22, 5, 2, 15, 0x3ce }, { 0, 2, 1, 27, 4, 14, 0x1 },    { 6, 1, 1, 13, 2, 15, 0x1 },
    { 4, 3, 3, 26, 4, 1, 0x1 },     { 4, 3, 8, 26, 4, 1, 0x69 },    { 4, 3, 13, 26, 4, 1, 0xd1 },
    { 4, 8, 19, 11, 2, 1, 0x139 },  { 4, 21, 19, 8, 2, 1, 0x139 },
};

// pokebw2's MailBox_CreateWindows
void func_ov136_021c5e58(MailBoxWork *work) {
    u32 i;
    BmpWin *window;

    BmpWin_InitAllocator(HEAPID_MAILBOX_VIEW);
    work->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_MAILBOX_VIEW);
    work->view->tcbManager = GFL_TCBExMgrCreate(HEAPID_MAILBOX_VIEW, HEAPID_MAILBOX_VIEW, 32, 32);
    work->view->printQueue = func_0201c440(HEAPID_MAILBOX_VIEW);
    for (i = 0; i < MAILBOX_WINDOW_COUNT; i++) {
        const MailBoxWindowTemplate *template = &data_ov136_021c7a20[i];

        MailBoxPrintState *printStates;

        work->view->windows[i] = BmpWin_CreateDynamic(template->bg, template->x, template->y, template->width,
                                                       template->height, template->palette, TRUE);
        printStates = work->view->printStates;
        printStates[i].window = work->view->windows[i];
        printStates[i].printing = FALSE;
    }
    for (i = 0; i < MAILBOX_WINDOW_COUNT - 10; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(work->view->windows[i]), 4);
        BmpWin_FlushChar(work->view->windows[i]);
    }
    window = work->view->windows[0];
    BmpWin_TransferNow(window);
    window = work->view->windows[1];
    BmpWin_TransferNow(window);
}

// pokebw2's MailBox_FreeWindows
void func_ov136_021c5f44(MailBoxWork *work) {
    u32 i;

    func_0201c4c0(work->view->printQueue);
    GFL_TCBExMgrFree(work->view->tcbManager);
    for (i = 0; i < MAILBOX_WINDOW_COUNT; i++) {
        BmpWin_Free(work->view->windows[i]);
    }
    GFL_FontFree(work->font);
    BmpWin_FreeAllocator();
}

#define MAILBOX_ALIGN_LEFT 0
#define MAILBOX_ALIGN_RIGHT 1
#define MAILBOX_ALIGN_CENTER 2

// Starts printing a string into the window the state belongs to
static void func_ov136_021c5f80(BmpWin *window, StrBuf *str, MailBoxPrintState *state, PrintQueue *queue, int x, int y,
                                Font *font, u16 color, u32 align) {
    if (align == MAILBOX_ALIGN_RIGHT) {
        x -= GFL_FontGetBlockWidth(str, font, 0);
    } else if (align == MAILBOX_ALIGN_CENTER) {
        x -= GFL_FontGetBlockWidth(str, font, 0) / 2;
    }
    func_0201c724(queue, BmpWin_GetBitmap(state->window), x, y, str, font, color);
    state->printing = TRUE;
}

static void func_ov136_021c5fdc(MailBoxView *view, MsgData *msgData, u32 index, u32 messageId, int x, int y,
                                   Font *font, u16 color, u32 align) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(msgData, messageId);

    func_ov136_021c5f80(view->windows[index], str, &view->printStates[index], view->printQueue, x, y, font, color,
                        align);
    GFL_StrBufFree(str);
}

// pokebw2's MailBox_DrawMailFrame
void func_ov136_021c6028(MailBoxWork *work, u32 frame, u32 windowIndex, u32 mailIndex) {
    MailData *mail;
    GFLBitmap *tileA;
    GFLBitmap *tileB;
    BmpWin *window;
    MailBoxPrintState *printState;
    u32 i;

    tileA = GFL_BitmapWrap(work->view->tileA, 1, 1, 0x20, HEAPID_MAILBOX_VIEW);
    tileB = GFL_BitmapWrap(work->view->tileB, 1, 1, 0x20, HEAPID_MAILBOX_VIEW);
    window = work->view->windows[windowIndex];
    mail = work->view->mails[mailIndex];
    printState = &work->view->printStates[windowIndex];
    GFL_BitmapFill(tileA, 4);
    GFL_BitmapFill(tileB, 4);
    GFL_StrBufLoadString(work->view->strbuf, func_02008f84(mail));
    for (i = 0; i < 8; i++) {
        GFL_BitmapCopyArea(BmpWin_GetBitmap(window), tileA, 0, 0, i * 8, 0, 8, 8, GFL_BITMAP_NO_COLOR_KEY);
        GFL_BitmapCopyArea(BmpWin_GetBitmap(window), tileB, 0, 0, i * 8, 8, 8, 8, GFL_BITMAP_NO_COLOR_KEY);
    }
    GFL_BitmapFill(BmpWin_GetBitmap(window), 4);
    if (func_02008f9c(mail) == 0) {
        func_ov136_021c5f80(window, work->view->strbuf, printState, work->view->printQueue, 0, 0, work->font,
                            PRINT_COLOR(5, 6, 4), MAILBOX_ALIGN_LEFT);
    } else {
        func_ov136_021c5f80(window, work->view->strbuf, printState, work->view->printQueue, 0, 0, work->font,
                            PRINT_COLOR(7, 8, 4), MAILBOX_ALIGN_LEFT);
    }
    func_0202cebc(work->view->frames, frame, window);
    GFL_BitmapFree(tileB);
    GFL_BitmapFree(tileA);
}

// pokebw2's MailBox_PrintTitle
void func_ov136_021c6170(MailBoxWork *work) {
    BmpWin *window;

    GFL_BitmapFill(BmpWin_GetBitmap(work->view->windows[MAILBOX_WIN_TITLE]), 0);
    func_ov136_021c5fdc(work->view, work->view->msgData, MAILBOX_WIN_TITLE, BtlServerFlow2_Text_000, 52, 0,
                           work->font, PRINT_COLOR(1, 2, 0), MAILBOX_ALIGN_CENTER);
    window = work->view->windows[MAILBOX_WIN_TITLE];
    BmpWin_Transfer(window);
}

// pokebw2's MailBox_Nop1
void func_ov136_021c61cc(MailBoxView *view) {
}

// pokebw2's MailBox_Nop2
void func_ov136_021c61d0(MailBoxView *view) {
}

// pokebw2's MailBox_RegisterAuthorName
static void func_ov136_021c61d4(MailBoxWork *work, int unused) {
    MailData *mail = work->view->mails[work->mailIndex];
    StrBuf *name = GFL_StrBufCreate(16, HEAPID_MAILBOX_VIEW);

    // The original fetches the name twice and drops the first result
    func_02008f84(mail);
    GFL_StrBufLoadString(name, func_02008f84(mail));
    func_0201ee0c(work->view->wordSet, 0, name, func_02008f9c(mail), 1, 2);
    GFL_StrBufFree(name);
}

// pokebw2's MailBox_PrintMessage
static void func_ov136_021c6230(MailBoxWork *work) {
    BmpWin *window = work->view->windows[MAILBOX_WIN_MESSAGE];

    GFL_BitmapFill(BmpWin_GetBitmap(window), 15);
    work->view->stream = func_0201cce4(window, 0, 0, work->view->strbuf, work->font, func_02012ffc(),
                                       work->view->tcbManager, 0, HEAPID_MAILBOX_VIEW, 15);
}

// pokebw2's MailBox_PrintWhatWouldYouLike
void func_ov136_021c6284(MailBoxWork *work) {
    StrBuf *str;

    func_ov136_021c61d4(work, 0);
    str = GFL_MsgDataLoadStrbufNew(work->view->msgData, BtlServerFlow2_Text_009);
    GFL_WordSetFormatStrbuf(work->view->wordSet, work->view->strbuf, str);
    GFL_StrBufFree(str);
    func_ov136_021c6230(work);
}

// pokebw2's MailBox_ShowMessageWindow
void func_ov136_021c62bc(MailBoxWork *work) {
    BmpWin *window;

    BmpWin_DrawFrame(work->view->windows[MAILBOX_WIN_MESSAGE], 2, 0x3e2, 13);
    window = work->view->windows[MAILBOX_WIN_MESSAGE];
    BmpWin_Transfer(window);
}

// pokebw2's MailBox_HideMessageWindow
void func_ov136_021c62f4(MailBoxWork *work) {
    func_0201f7a8(work->view->windows[MAILBOX_WIN_MESSAGE], 2);
    GFL_BGSysQueueScrLoad(0);
}

// pokebw2's MailBox_SetMessage
void func_ov136_021c630c(MailBoxWork *work, u32 message) {
    switch (message) {
    case MAILBOX_MESSAGE_ERASE:
        GFL_MsgDataLoadStrbuf(work->view->msgData, BtlServerFlow2_Text_010, work->view->strbuf);
        break;
    case MAILBOX_MESSAGE_ERASED:
        GFL_MsgDataLoadStrbuf(work->view->msgData, BtlServerFlow2_Text_011, work->view->strbuf);
        break;
    case MAILBOX_MESSAGE_ASK_HOLD:
        GFL_MsgDataLoadStrbuf(work->view->msgData, BtlServerFlow2_Text_012, work->view->strbuf);
        break;
    case MAILBOX_MESSAGE_HOLD_DONE:
        GFL_MsgDataLoadStrbuf(work->view->msgData, BtlServerFlow2_Text_013, work->view->strbuf);
        break;
    case MAILBOX_MESSAGE_BAG_FULL:
        GFL_MsgDataLoadStrbuf(work->view->msgData, BtlServerFlow2_Text_014, work->view->strbuf);
        break;
    }
    func_ov136_021c6230(work);
}

// pokebw2's MailBox_PrintMail
void func_ov136_021c6374(MailBoxWork *work) {
    u32 i;
    BmpWin *window;

    for (i = 0; i < 3; i++) {
        PMSData *sentence = func_02008fc8(work->view->mails[work->mailIndex], i);

        if (func_02024450(sentence)) {
            func_0202dc48(work->view->pmsDraw, work->view->windows[MAILBOX_WIN_SENTENCE_1 + i], sentence, i);
            BmpWin_Transfer(work->view->windows[MAILBOX_WIN_SENTENCE_1 + i]);
        }
    }
    work->view->stampSentence.type = 6;
    work->view->stampSentence.id = 0;
    work->view->stampSentence.words[0] = func_02008fc0(work->view->mails[work->mailIndex]);
    if (work->view->stampSentence.words[0] != 0) {
        func_0202dc48(work->view->pmsDraw, work->view->windows[MAILBOX_WIN_STAMP], &work->view->stampSentence, 3);
    }
    GFL_BitmapFill(BmpWin_GetBitmap(work->view->windows[MAILBOX_WIN_AUTHOR]), 0);
    GFL_StrBufLoadString(work->view->strbuf, func_02008f84(work->view->mails[work->mailIndex]));
    func_ov136_021c5f80(work->view->windows[MAILBOX_WIN_AUTHOR], work->view->strbuf,
                        &work->view->printStates[MAILBOX_WIN_AUTHOR], work->view->printQueue, 0, 2, work->font,
                        PRINT_COLOR(1, 2, 0), MAILBOX_ALIGN_LEFT);
    window = work->view->windows[MAILBOX_WIN_AUTHOR];
    BmpWin_Transfer(window);
}
