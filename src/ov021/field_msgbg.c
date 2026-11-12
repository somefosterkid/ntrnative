// Black's code at ov021 0x021901ec-0x021929e8. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field.h"
#include "field/field_msgbg.h"
#include "field/ov021_021929e8.h"
#include "field/ov021_021942c8.h"
#include "field/talkmsgwin.h"
#include "field/union_msg.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "nnsys/g2d.h"
#include "struct_decls.h"
#include "system/bmp_menulist.h"
#include "system/bmp_menuwork.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "gfl/sound.h"
#include "field/beacon_view.h"
#include "gfl/graphics.h"
#include "nitro/ioreg.h"
#include "system/text_speed.h"


u32 func_ov021_021919c8(MsgBGNameWin *param_1);
void func_ov021_02191dc8(u8 param_1, u8 param_2, u8 param_3, u8 param_4, u8 param_5, u16 param_6, u8 param_7);
u8 func_ov021_021927f8(MsgBGCursor *param_1);
BOOL func_ov021_02190d08(u32 event);
ListMenuUI *ListMenuUI_CreateCore(FieldMsgBG *param_1, const ListMenuRequest *param_2, ListMenuOption *param_3, u16 param_4, u16 param_5, u16 param_6, BmpMenuListCursorCallback param_7, BmpMenuListPrintCallback param_8, void *param_9);
void func_ov021_02190b2c(BmpMenuListHeader *header, const ListMenuRequest *request);

void func_ov021_021931f4(MsgBGInput *param_1);
void func_ov021_02192bd0(FieldMsgBG *param_1, s32 param_2);
void func_ov021_02192cf0(FieldMsgBG *param_1, s32 param_2);
void func_ov021_02192eb8(TCB *param_1, void *param_2);
void func_ov021_02194764(TalkMsgWinSys *param_1);
void func_ov021_021905c8(FieldMsgBGWin *param_1, int param_2, int param_3, StrBuf *param_4, u16 param_5, Font *param_6);
void func_ov021_02193130(BmpWin *param_1);
void func_ov021_02190550(FieldMsgBGWin *param_1, s32 param_2, s32 param_3, u32 param_4);
void func_ov021_02192bc0(BmpWin *param_1);
ListMenuUI *func_ov021_02190960(FieldMsgBG *param_1, const ListMenuRequest *param_2, ListMenuOption *param_3, u16 param_4, u16 param_5);
void func_ov021_021927e8(MsgBGCursor *param_1);
MsgBGTalkWin *func_ov021_02190dcc(FieldMsgBG *param_1, MsgData *param_2, u8 param_3, u8 param_4, u16 param_5, u16 param_6);
u8 func_ov021_0219320c(MsgBGInput *param_1);
void func_ov021_021928c0(MsgBGCursor *param_1, GFLBitmap *param_2, u8 param_3);
void func_ov021_02194950(TalkMsgWinSys *param_1, s32 param_2);
BOOL func_ov021_02194960(TalkMsgWinSys *param_1, s32 param_2);
void func_ov021_0219492c(TalkMsgWinSys *param_1, s32 param_2);
BmpWin *func_ov021_02194990(TalkMsgWinSys *param_1, s32 param_2);
void func_ov021_02194a48(TalkMsgWinSys *param_1, BmpWin *param_2);
s32 func_ov021_0219319c(FieldMsgBG *param_1, MsgBGNameWin *param_2);
void func_ov021_021917c4(FieldMsgBG *param_1, MsgBGNameWin *param_2, StrBuf *param_3, u32 param_4, u8 param_5, u8 param_6, u8 param_7, u8 param_8, u32 param_9);
MsgBGNameWin *func_ov021_021931c4(FieldMsgBG *param_1, s32 param_2);
BOOL func_ov021_02194974(TalkMsgWinSys *param_1, s32 param_2);
PrintStream *func_ov021_02194984(TalkMsgWinSys *param_1, s32 param_2);

MsgBGScrollWin *func_ov021_02191a10(FieldMsgBG *param_1, u32 param_2, Font *param_3);
void func_ov021_02191fa0(u8 param_1, u8 param_2, u8 param_3, u8 param_4, u8 param_5, u16 param_6, u8 param_7);
void func_ov021_021924f0(u16 param_1, u16 param_2, GFLBitmap *param_3, Font *param_4, StrBuf *param_5, HeapID param_6);
void func_ov021_021928f8(MsgBGCursor *param_1, GFLBitmap *param_2, GFLBitmap *param_3);
void func_ov021_021927c0(MsgBGCursor *param_1, FieldMsgBG *param_2);
void func_ov021_02190538(FieldMsgBGWin *param_1);
void func_ov021_02190d48(MsgBGStream *param_1);
u32 func_ov021_02190d5c(MsgBGStream *param_1);
MsgBGStream *func_ov021_02190d2c(FieldMsgBG *param_1, StrBuf *param_2, BmpWin *param_3, u16 param_4, u16 param_5, s32 param_6);
void func_ov021_021927fc(MsgBGCursor *param_1, GFLBitmap *param_2, int param_3);
void func_ov021_02192860(MsgBGCursor *param_1, GFLBitmap *param_2, u8 param_3);
void func_ov021_02191d88(u16 param_1, s32 param_2, u32 param_3, HeapID param_4);
u32 func_ov021_0219214c(GFLBitmap *param_1, Font *param_2, StrBuf *param_3, StrBuf *param_4, u32 *param_5, u8 param_6);
u32 func_ov021_02192138(StrBuf *param_1, StrBuf *param_2, u32 param_3);
BOOL func_ov021_021921c8(GFLBitmap *param_1, GFLBitmap *param_2, GFLBitmap *param_3, s32 param_4, u16 param_5);

// The yes/no menu's request, which func_ov021_02190be4 copies. It is defined after the BG setup functions, where the
// size sort lays it out after their setups, as the ROM has it
// pokebw2's sYesNoMenuRequest
static const ListMenuRequest data_ov021_021d3f38;

FieldMsgBG *func_ov021_021901ec(HeapID heapId, G3DCamera *camera) {
    FieldMsgBG *msgBG;

    msgBG = GFL_HeapAllocate(heapId, sizeof(FieldMsgBG), 1, "field_msgbg.c", 0x1a4);
    msgBG->heapId = heapId;
    msgBG->bgId = 0xff;
    msgBG->unk_4 = 0xff;
    msgBG->camera = camera;
    msgBG->font = GFL_FontCreate(0x17, 0, 0, 0, msgBG->heapId);
    msgBG->printQueue = func_0201c440(msgBG->heapId);
    msgBG->tcbEx = GFL_TCBExMgrCreate(msgBG->heapId, msgBG->heapId, 1, 4);
    func_ov021_021931f4(&msgBG->input);
    msgBG->cursorCharFile = GFL_ArcSysReadHeapNewLZ(5, 0xc, 0, heapId);
    NNS_G2dGetUnpackedBGCharacterData(msgBG->cursorCharFile, &msgBG->cursorChar);
    return msgBG;
}

void func_ov021_02190270(FieldMsgBG *msgBGSys) {
    func_ov021_02192bd0(msgBGSys, 1);
}

void func_ov021_0219027c(FieldMsgBG *msgBGSys) {
    func_ov021_02192cf0(msgBGSys, 0);
    if (msgBGSys->vblankTcb == NULL) {
        msgBGSys->vblankTcb = GFL_VBlankTCBAdd(func_ov021_02192eb8, msgBGSys, 0);
    }
}

void func_ov021_021902a4(u8 bgId, u16 heapId) {
    func_ov021_02192fa4(bgId, heapId);
}

void func_ov021_021902ac(FieldMsgBG *param_1, s32 param_2) {
    func_ov021_02192fec(param_1->bgId, param_1->heapId, param_2);
}



void func_ov021_021902bc(FieldMsgBG *param_1) {
    s32 i;
    FieldMsgBGWin *win;

    i = 0;
    win = param_1->wins;
    if (param_1->cursorCharFile != NULL) {
        GFL_HeapFree(param_1->cursorCharFile);
    }
    if (param_1->talkMsgWin != NULL) {
        func_ov021_02194798(param_1->talkMsgWin);
    }
    if (param_1->tcbEx != NULL) {
        GFL_TCBExMgrFree(param_1->tcbEx);
    }
    if (param_1->bgId != 0xff) {
        GFL_BGSysFreeFilledChar(param_1->bgId, 1, 0);
        GFL_BGSysReleaseBG(param_1->bgId);
    }
    if (param_1->unk_4 != 0xff) {
        GFL_BGSysReleaseBG(param_1->unk_4);
    }
    do {
        if (win->strBuf != NULL) {
            GFL_StrBufFree(win->strBuf);
        }
        i = i + 1;
        win++;
    } while (i < 4);
    if (param_1->printQueue != NULL) {
        func_0201c4c0(param_1->printQueue);
    }
    if (param_1->font != NULL) {
        GFL_FontFree(param_1->font);
    }
    GFL_HeapFree(param_1);
}

void func_ov021_02190344(FieldMsgBG *param_1) {
    if (param_1->talkMsgWin != NULL) {
        func_ov021_02194798(param_1->talkMsgWin);
        param_1->talkMsgWin = NULL;
    }
    if (param_1->bgId != 0xff) {
        GFL_BGSysFreeFilledChar(param_1->bgId, 1, 0);
        GFL_BGSysReleaseBG(param_1->bgId);
        param_1->bgId = 0xff;
    }
}


BOOL func_ov021_02190378(FieldMsgBG *msgBGSys) {
    if (msgBGSys->unk_4 != 0xff) {
        GFL_BGSysReleaseBG(msgBGSys->unk_4);
        msgBGSys->unk_4 = 0xff;
        return 1;
    }
    return 0;
}

void func_ov021_02190398(FieldMsgBG *param_1) {
    if (param_1->talkMsgWin != NULL) {
        func_ov021_02194798(param_1->talkMsgWin);
    }
    func_ov021_02192e0c(param_1);
    if (param_1->vblankTcb == NULL) {
        param_1->vblankTcb = GFL_VBlankTCBAdd(func_ov021_02192eb8, param_1, 0);
    }
}


u32 func_ov021_021903cc(FieldMsgBG *param_1) {
    if (param_1->vblankTcb == NULL) {
        return 1;
    }
    return 0;
}

void func_ov021_021903e0(FieldMsgBG *param_1) {
    s32 i;
    MsgBGNameWin **p;
    FieldMsgBGWin *win;
    PrintQueue *printQueue;

    func_0201c4e4(param_1->printQueue);
    for (i = 0, p = param_1->nameWins; i < 8; i++, p++) {
        if (*p != NULL) {
            func_ov021_021919c8(*p);
        }
    }
    for (i = 0, win = param_1->wins; i < 4; i++, win++) {
        printQueue = win->printQueue;
        if (printQueue != NULL) {
            if (win->printWin.flushPending != 0) {
                if (func_0201c6c4(printQueue, BmpWin_GetBitmap(win->printWin.window)) == 0) {
                    BmpWin_FlushChar(win->printWin.window);
                    win->printWin.flushPending = 0;
                }
            }
            win->unk_4 = (win->printWin.flushPending == 0);
        }
    }
    GFL_TCBExMgrUpdate(param_1->tcbEx);
    if (param_1->talkMsgWin != NULL) {
        func_ov021_0219470c(param_1->talkMsgWin);
        func_ov021_02194760(param_1->talkMsgWin);
    }
}


void func_ov021_0219047c(FieldMsgBG *param_1) {
    if (param_1->talkMsgWin != NULL) {
        func_ov021_02194764(param_1->talkMsgWin);
    }
}

void func_ov021_02190490(FieldMsgBG *param_1) {
    func_0201c6ec(param_1->printQueue);
}

BOOL func_ov021_0219049c(FieldMsgBG *msgBGSys) {
    BOOL BVar1;

    BVar1 = func_0201c6b4(msgBGSys->printQueue);
    return BVar1;
}

PrintQueue *func_ov021_021904a8(FieldMsgBG *msgBGSys) {
    return msgBGSys->printQueue;
}

Font *func_ov021_021904ac(FieldMsgBG *msgBGSys) {
    return msgBGSys->font;
}

MsgData *func_ov021_021904b0(FieldMsgBG *msgBGSys, u32 fileId) {
    MsgData *pMVar1;

    pMVar1 = GFL_MsgSysLoadData(0, 2, (u16)fileId, msgBGSys->heapId);
    return pMVar1;
}

void func_ov021_021904c8(MsgData *msgData) {
    GFL_MsgDataFree(msgData);
}


void func_ov021_021904d0(FieldMsgBG *msgBGSys) {
    msgBGSys->resetBG2 = 1;
}

void func_ov021_021904dc(FieldMsgBG *param_1, u8 param_2) {
    func_ov021_02193208(&param_1->input, param_2);
}

FieldMsgBGWin *func_ov021_021904ec(FieldMsgBG *param_1, MsgData *param_2, BmpWin *param_3) {
    s32 i;
    FieldMsgBGWin *win;

    for (i = 0, win = param_1->wins; i < 4; i++, win++) {
        if (win->printQueue == NULL) {
            win->owner = param_1;
            win->unk_14 = param_3;
            win->unk_4 = 1;
            win->font = param_1->font;
            win->printQueue = param_1->printQueue;
            win->strBuf = GFL_StrBufCreate(0xa0, param_1->heapId);
            win->msgData = param_2;
            win->printWin.window = param_3;
            win->printWin.flushPending = 0;
            return win;
        }
    }
    return NULL;
}

void func_ov021_02190538(FieldMsgBGWin *param_1) {
    StrBuf *strbuf = param_1->strBuf;

    param_1->font = NULL;
    param_1->printQueue = NULL;
    GFL_StrBufFree(strbuf);
    param_1->strBuf = NULL;
    param_1->msgData = NULL;
}

void func_ov021_02190550(FieldMsgBGWin *param_1, s32 param_2, s32 param_3, u32 param_4) {
    PrintQueue *queue;
    GFLBitmap *bitmap;

    GFL_MsgDataLoadStrbuf(param_1->msgData, param_4, param_1->strBuf);
    queue = param_1->printQueue;
    bitmap = BmpWin_GetBitmap(param_1->printWin.window);
    func_0201c6fc(queue, bitmap, param_2, param_3, param_1->strBuf, param_1->font);
    param_1->printWin.flushPending = 1;
}

void func_ov021_02190590(FieldMsgBGWin *param_1, u16 param_2, u16 param_3, StrBuf *param_4) {
    PrintQueue *queue = param_1->printQueue;
    GFLBitmap *bitmap = BmpWin_GetBitmap(param_1->printWin.window);
    func_0201c6fc(queue, bitmap, param_2, param_3, param_4, param_1->font);
    param_1->printWin.flushPending = 1;
}

void func_ov021_021905c8(FieldMsgBGWin *param_1, int param_2, int param_3, StrBuf *param_4, u16 param_5, Font *param_6) {
    PrintQueue *queue = param_1->printQueue;
    GFLBitmap *bitmap = BmpWin_GetBitmap(param_1->printWin.window);
    func_0201c724(queue, bitmap, param_2, param_3, param_4, param_6, param_5);
    param_1->printWin.flushPending = 1;
}







void func_ov021_02190604(FieldMsgBGWin *param_1, u16 param_2, u16 param_3, StrBuf *param_4, u16 param_5) {
    func_ov021_021905c8(param_1, param_2, param_3, param_4, param_5, param_1->font);
}

u32 func_ov021_0219061c(FieldMsgBGWin *param_1) {
    return param_1->unk_4;
}

void func_ov021_02190620(FieldMsgBGWin *param_1) {
    u8 bg;
    GFLBitmap *bitmap;

    bitmap = BmpWin_GetBitmap(param_1->unk_14);
    GFL_BitmapFill(bitmap, 0xff);
    bg = BmpWin_GetBGIndex(param_1->unk_14);
    GFL_BGSysLoadScr(bg);
}

void func_ov021_0219063c(FieldMsgBGWin *param_1, u16 param_2, u16 param_3, s8 param_4, int param_5) {
    GFLBitmap *bitmap = BmpWin_GetBitmap(param_1->unk_14);
    GFL_BitmapFillArea(bitmap, param_2, param_3, param_4, param_5, 0xff);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(param_1->unk_14));
}


PrintQueue *func_ov021_02190678(FieldMsgBGWin *param_1) {
    return param_1->printQueue;
}

PrintWindow *func_ov021_0219067c(FieldMsgBGWin *param_1) {
    return &param_1->printWin;
}

MsgBGSysWin *MoneyWin_Create(FieldMsgBG *param_1, MsgData *param_2, u16 param_3, u16 param_4, u16 param_5, u16 param_6, u16 param_7) {
    MsgBGSysWin *window;
    BmpWin *bmpWin;
    FieldMsgBGWin *win;
    u8 bgId;

    bgId = param_1->bgId;
    func_ov021_02192f74(param_1);
    if (param_7 == 0xe) {
        bgId = param_1->unk_4;
    }
    window = GFL_HeapAllocate(param_1->heapId, sizeof(MsgBGSysWin), 1, "field_msgbg.c", 0x483);
    window->msgBG = param_1;
    bmpWin = func_ov021_021930c0(bgId, param_1->heapId, param_3, param_4, param_5, param_6, param_7);
    window->bmpWin = bmpWin;
    win = func_ov021_021904ec(param_1, param_2, bmpWin);
    window->win = win;
    if (param_7 == 0xe) {
        func_ov021_02193144(param_1->heapId);
        setAlphaBlend(1);
    } else {
        loadPltt(param_1->heapId);
    }
    return window;
}

MsgBGSysWin *FieldMsgBG_CreateMoneyWin(FieldMsgBG *param_1, MsgData *param_2, u16 param_3, u16 param_4, u16 param_5, u16 param_6) {
    param_1->unk_6 = 0xe;
    return MoneyWin_Create(param_1, param_2, param_3, param_4, param_5, param_6, param_1->unk_6);
}



void func_ov021_02190728(MsgBGSysWin *window) {
    func_ov021_02193130(window->bmpWin);
    func_ov021_02190538(window->win);
    GFL_HeapFree(window);
}

void func_ov021_02190740(MsgBGSysWin *param_1, u16 param_2, u16 param_3, u32 param_4) {
    u8 bg;

    func_ov021_02190550(param_1->win, param_2, param_3, param_4);
    bg = BmpWin_GetBGIndex(param_1->bmpWin);
    GFL_BGSysLoadScr(bg);
}

void func_ov021_02190758(MsgBGSysWin *window, u16 x, u16 y, StrBuf *strbuf) {
    func_ov021_02190590(window->win, x, y, strbuf);
}


void func_ov021_02190764(MsgBGSysWin *param_1, u16 param_2, u16 param_3, StrBuf *param_4, u16 param_5, Font *param_6) {
    func_ov021_021905c8(param_1->win, param_2, param_3, param_4, param_5, param_6);
}

BOOL func_ov021_0219077c(MsgBGSysWin *window) {
    return func_ov021_0219061c(window->win);
}

void func_ov021_02190788(MsgBGSysWin *window) {
    func_ov021_02190620(window->win);
}

void func_ov021_02190794(MsgBGSysWin *window, u16 x, u16 y, s8 width, s32 height) {
    func_ov021_0219063c(window->win, x, y, width, height);
}


MsgBGSysWin *func_ov021_021907a8(FieldMsgBG *param_1, MsgData *param_2) {
    return FieldMsgBG_CreateMoneyWin(param_1, param_2, 1, 0x13, 0x1e, 4);
}

BmpWin *func_ov021_021907c0(MsgBGSysWin *window) {
    return window->bmpWin;
}

MsgBGSysWin *func_ov021_021907c4(FieldMsgBG *param_1, MsgData *param_2, u16 param_3, u16 param_4, u16 param_5, u16 param_6) {
    MsgBGSysWin *window;
    BmpWin *bmpWin;
    FieldMsgBGWin *win;

    param_1->unk_6 = 0xe;
    func_ov021_02192f74(param_1);
    window = GFL_HeapAllocate(param_1->heapId, sizeof(MsgBGSysWin), 1, "field_msgbg.c", 0x560);
    window->msgBG = param_1;
    bmpWin = func_ov021_02192b20(param_3, param_4, param_5, param_6, param_1->heapId);
    window->bmpWin = bmpWin;
    win = func_ov021_021904ec(param_1, param_2, bmpWin);
    window->win = win;
    setAlphaBlend(1);
    return window;
}


void func_ov021_0219082c(MsgBGSysWin *window) {
    func_ov021_02192bc0(window->bmpWin);
    func_ov021_02190538(window->win);
    GFL_HeapFree(window);
}

void func_ov021_02190844(MsgBGSysWin *window, u16 x, u16 y, StrBuf *strbuf) {
    func_ov021_02190590(window->win, x + 8, y, strbuf);
}


void func_ov021_02190854(MsgBGSysWin *window) {
    func_ov021_02190620(window->win);
}

ListMenuUI *ListMenuUI_CreateCore(FieldMsgBG *param_1, const ListMenuRequest *param_2, ListMenuOption *param_3, u16 param_4, u16 param_5, u16 param_6, BmpMenuListCursorCallback param_7, BmpMenuListPrintCallback param_8, void *param_9) {
    u8 bgId = param_1->bgId;
    ListMenuUI *ui;
    BmpMenuListHeader header;

    func_ov021_02192f74(param_1);
    if (param_6 == 0xe) {
        bgId = param_1->unk_4;
    }
    ui = GFL_HeapAllocate(param_1->heapId, sizeof(ListMenuUI), 1, "field_msgbg.c", 0x606);
    func_ov021_02190b2c(&header, param_2);
    ui->msgBG = param_1;
    ui->options = param_3;
    ui->window = func_ov021_021930c0(bgId, param_1->heapId, param_2->x, param_2->y, param_2->width, param_2->height, param_6);
    ui->win = func_ov021_021904ec(param_1, NULL, ui->window);
    header.msgData = NULL;
    header.printWindow = func_ov021_0219067c(ui->win);
    header.queue = func_ov021_02190678(ui->win);
    header.font = param_1->font;
    header.window = ui->window;
    header.options = param_3;
    header.printCallback = param_8;
    header.cursorCallback = param_7;
    header.work = param_9;
    ui->list = BmpMenuList_Create(&header, param_4, param_5, param_1->heapId);
    func_02020cf8(ui->list, param_1->heapId);
    if (param_6 == 0xe) {
        func_ov021_02193144(param_1->heapId);
        setAlphaBlend(1);
    } else {
        loadPltt(param_1->heapId);
    }
    return ui;
}


ListMenuUI *func_ov021_02190938(FieldMsgBG *param_1, const ListMenuRequest *param_2, ListMenuOption *param_3, u16 param_4, u16 param_5, BmpMenuListCursorCallback param_6, BmpMenuListPrintCallback param_7, void *param_8) {
    param_1->unk_6 = 0xe;
    return ListMenuUI_CreateCore(param_1, param_2, param_3, param_4, param_5, param_1->unk_6, param_6, param_7, param_8);
}

ListMenuUI *func_ov021_02190960(FieldMsgBG *param_1, const ListMenuRequest *param_2, ListMenuOption *param_3, u16 param_4, u16 param_5) {
    return func_ov021_02190938(param_1, param_2, param_3, param_4, param_5, NULL, param_2->printCallback, NULL);
}



ListMenuUI *func_ov021_0219097c(FieldMsgBG *param_1, ListMenuRequest *param_2, ListMenuOption *param_3) {
    return func_ov021_02190960(param_1, param_2, param_3, 0, 0);
}

ListMenuUI *ListMenuUI_Create(FieldMsgBG *msgBGSys, const ListMenuRequest *request, ListMenuOption *options, BmpMenuListCursorCallback callback, void *work, u16 a5, u16 cursor, u32 flags) {
    ListMenuUI *ui;

    ui = ListMenuUI_CreateCore(msgBGSys, request, options, a5, cursor, msgBGSys->unk_6, callback, NULL, work);
    if (flags == 0) {
        func_02020d10(ui->list, 1);
    }
    return ui;
}


void func_ov021_021909bc(ListMenuUI *window) {
    BmpMenuList_Free(window->list, NULL, NULL);
    ListMenuCore_FreeOptionList(window->options);
    func_ov021_02193130(window->window);
    func_ov021_02190538(window->win);
    GFL_HeapFree(window);
}

intptr_t ListMenuUI_Update(ListMenuUI *menu) {
    BOOL BVar1;
    intptr_t result;

    BVar1 = func_ov021_0219049c(menu->msgBG);
    if (BVar1 == 0) {
        return -1;
    }
    result = BmpMenuList_Update(menu->list);
    if (result == -1) {
        return -1;
    }
    if (result == -2) {
        result = -2;
    }
    return result;
}

void func_ov021_02190a14(ListMenuUI *param_1) {
    func_02020234(param_1->list);
}

ListMenuOption *InitListMenuOptionHeap(u32 count, HeapID heapId) {
    return ListMenuCore_CreateOptionList(count, heapId);
}

ListMenuOption *func_ov021_02190a28(const ListMenuRow *rows, s32 count, MsgData *msgData, HeapID heapId) {
    ListMenuOption *options;
    s32 i;

    options = InitListMenuOptionHeap(count, heapId);
    for (i = 0; i < count; i++) {
        ListMenuCore_AppendMsgOption(options, msgData, rows[i].messageId, rows[i].value, heapId);
    }
    return options;
}



void AppendListMenuOption(ListMenuOption *options, StrBuf *text, u32 value, HeapID heapId) {
    ListMenuCore_AppendStrBufOption(options, text, value, heapId);
}

void func_ov021_02190a6c(ListMenuOption *options) {
    ListMenuCore_FreeStrBufs(options);
}

u32 func_ov021_02190a74(FieldMsgBG *param_1, ListMenuOption *param_2, s32 *param_3, u32 param_4) {
    const ListMenuOption *options;
    u32 spacing;
    s32 best;
    s32 i;
    u32 width;
    u32 max;

    max = 0;
    best = 0;
    i = 0;
    options = param_2;
    if (options->text != (StrBuf *)-1) {
        spacing = (u16)param_4;
        do {
            if (options->text == NULL) {
                break;
            }
            width = GFL_FontGetBlockWidth(options->text, param_1->font, spacing);
            if (width > max) {
                max = width;
                best = i;
            }
            options++;
            i++;
        } while (options->text != (StrBuf *)-1);
    }
    *param_3 = best;
    return max;
}

u32 func_ov021_02190ac4(ListMenuOption *options) {
    return ListMenuCore_GetFirstFreeIndex(options);
}

u32 func_ov021_02190acc(FieldMsgBG *param_1, ListMenuOption *param_2, s32 param_3, u16 param_4) {
    s32 index;
    u32 n;
    u32 rows;

    n = func_ov021_02190a74(param_1, param_2, &index, param_4);
    n += param_4 + 0x10;
    rows = n >> 3;
    if ((n & 7) != 0) {
        rows++;
    }
    return rows;
}





u32 func_ov021_02190aec(s32 param_1, s32 param_2, s32 param_3, s32 param_4) {
    u32 i;
    s32 val;

    val = (param_2 + param_3) * param_1;
    i = val + 7U >> 3;
    if ((param_4 != 0) && (i * 8 - val < 6)) {
        i = i + 1;
    }
    return i;
}

void ListMenuRequest_Set(ListMenuRequest *request, u16 count, u16 x, u16 y, u16 width, u16 height) {
    request->count = count;
    request->y = y;
    request->x = x;
    request->width = width;
    request->height = height;
}

void func_ov021_02190b1c(ListMenuUI *param_1) {
    BmpWin_DrawFrame(param_1->window, 0, 1, 0xd);
}

void func_ov021_02190b2c(BmpMenuListHeader *header, const ListMenuRequest *request) {
    MI_CpuFill8(header, 0, sizeof(BmpMenuListHeader));
    header->count = request->count;
    header->maxShown = request->unk02;
    header->labelX = request->unk04;
    header->itemX = request->unk05;
    header->cursorX = request->unk06;
    header->y = request->unk07_0;
    header->fgColor = request->unk07_4;
    header->bgColor = request->unk08_0;
    header->shadowColor = request->unk08_4;
    header->letterSpacing = request->unk0A_0;
    header->lineSpacing = request->unk0A_3;
    header->pageSkip = request->unk0C_0;
    header->fontSizeX = request->unk10;
    header->fontSizeY = request->rowHeight;
    header->printCallback = request->printCallback;
}

ListMenuUI *func_ov021_02190be4(FieldMsgBG *param_1, u32 param_2) {
    ListMenuRequest request = data_ov021_021d3f38;
    ListMenuRow rows[2] = { { 0, { 0 } }, { 1, { 1 } } };
    MsgData *msgData;
    ListMenuOption *options;
    ListMenuUI *ui;

    msgData = func_ov021_021904b0(param_1, 0xe9);
    options = func_ov021_02190a28(rows, 2, msgData, param_1->heapId);
    GFL_MsgDataFree(msgData);
    ListMenuRequest_Set(&request, 2, 0x18, 10, 7, 4);
    ui = ListMenuUI_CreateCore(param_1, &request, options, 0, param_2, param_1->unk_6, NULL, NULL, NULL);
    func_02020d10(ui->list, 0);
    return ui;
}



u32 func_ov021_02190c78(ListMenuUI *window) {
    intptr_t result;

    result = ListMenuUI_Update(window);
    if (result == BMPMENULIST_NULL) {
        return 2;
    }
    if (result == BMPMENULIST_CANCEL) {
        result = 1;
    }
    return (u32)result;
}

MsgBGStream *func_ov021_02190c94(FieldMsgBG *param_1, StrBuf *param_2, BmpWin *param_3, u16 param_4, u16 param_5, s32 param_6, u16 param_7) {
    MsgBGStream *block;

    block = GFL_HeapAllocate(param_1->heapId, sizeof(MsgBGStream), 1, "field_msgbg.c", 0x85e);
    block->msgBG = param_1;
    block->speed = param_6;
    block->stream = func_0201cd10(param_3, param_4, param_5, param_2, param_1->font, param_6, param_1->tcbEx, 0, param_1->heapId, param_7, func_ov021_02190d08);
    func_ov021_02193200(&block->msgBG->input);
    return block;
}

BOOL func_ov021_02190d08(u32 event) {
    BOOL BVar1;
    switch (event) {
    case 3:
        GFL_SndSEPlay(0x56b);
        break;
    case 6:
        BVar1 = GFL_SndPlayerIsActiveAny();
        return BVar1;
    }
    return 0;
}

MsgBGStream *func_ov021_02190d2c(FieldMsgBG *param_1, StrBuf *param_2, BmpWin *param_3, u16 param_4, u16 param_5, s32 param_6) {
    return func_ov021_02190c94(param_1, param_2, param_3, param_4, param_5, param_6, 0xf);
}




void func_ov021_02190d48(MsgBGStream *param_1) {
    func_0201ce48(param_1->stream);
    GFL_HeapFree(param_1);
}

u32 func_ov021_02190d5c(MsgBGStream *param_1) {
    u32 result;

    result = func_0201ce30(param_1->stream);
    switch (result) {
    case 0:
        if (func_ov021_02193228(&param_1->msgBG->input)) {
            func_ov021_021932a0(param_1->stream, TRUE);
        } else {
            func_ov021_021932a0(param_1->stream, FALSE);
        }
        break;
    case 1:
        if (func_ov021_02193260(&param_1->msgBG->input, func_0201ce34(param_1->stream))) {
            if (func_ov021_0219320c(&param_1->msgBG->input) == 0) {
                GFL_SndSEPlay(0x547);
            }
            func_0201ce38(param_1->stream);
            result = 0;
            func_ov021_021932a0(param_1->stream, 0);
        }
        break;
    case 2:
        break;
    }
    return result;
}

MsgBGTalkWin *func_ov021_02190dcc(FieldMsgBG *param_1, MsgData *param_2, u8 param_3, u8 param_4, u16 param_5, u16 param_6) {
    MsgBGTalkWin *window;
    BmpWin *bmpWin;
    StrBuf *strBuf;

    param_1->unk_6 = 0xe;
    func_ov021_02192f74(param_1);
    window = GFL_HeapAllocate(param_1->heapId, sizeof(MsgBGTalkWin), 1, "field_msgbg.c", 0x8ff);
    window->msgBG = param_1;
    window->msgData = param_2;
    bmpWin = func_ov021_021930c0(param_1->unk_4, param_1->heapId, param_3, param_4, param_5, param_6, param_1->unk_6);
    window->bmpWin = bmpWin;
    strBuf = GFL_StrBufCreate(0xa0, param_1->heapId);
    window->strBuf = strBuf;
    func_ov021_021927c0(&window->cursor, param_1);
    func_ov021_02193144(param_1->heapId);
    setAlphaBlend(1);
    return window;
}



void func_ov021_02190e40(MsgBGTalkWin *window) {
    func_ov021_02193130(window->bmpWin);
    if (window->stream != NULL) {
        func_ov021_02190d48(window->stream);
    }
    if (window->win != NULL) {
        func_ov021_02190538(window->win);
    }
    GFL_StrBufFree(window->strBuf);
    func_ov021_021927e8(&window->cursor);
    GFL_HeapFree(window);
}

void func_ov021_02190e74(MsgBGTalkWin *window, u8 param_2, u8 param_3, u32 messageId) {
    s32 result;

    if (window->stream != NULL) {
        func_ov021_02190d48(window->stream);
    }
    GFL_MsgDataLoadStrbuf(window->msgData, messageId, window->strBuf);
    result = func_ov021_02193210(&window->msgBG->input);
    window->stream = func_ov021_02190d2c(window->msgBG, window->strBuf, window->bmpWin, param_2, param_3, result);
}

void func_ov021_02190eb8(MsgBGTalkWin *window, u8 param_2, u8 param_3, StrBuf *param_4) {
    s32 result;

    if (window->stream != NULL) {
        func_ov021_02190d48(window->stream);
    }
    result = func_ov021_02193210(&window->msgBG->input);
    window->stream = func_ov021_02190d2c(window->msgBG, param_4, window->bmpWin, param_2, param_3, result);
}

BOOL func_ov021_02190ef0(MsgBGTalkWin *window) {
    u32 result;

    result = func_ov021_02190d5c(window->stream);
    GCTX_HIDGetPressedKeys();
    GCTX_HIDGetHeldKeys();
    switch (result) {
    case 0:
        if (func_ov021_021927f8(&window->cursor) == 1) {
            func_ov021_021927fc(&window->cursor, BmpWin_GetBitmap(window->bmpWin), 0xf);
            BmpWin_FlushChar(window->bmpWin);
        }
        break;
    case 1:
        if (func_ov021_0219320c(&window->msgBG->input) == 0) {
            func_ov021_021928c0(&window->cursor, BmpWin_GetBitmap(window->bmpWin), 0xf);
            BmpWin_FlushChar(window->bmpWin);
        }
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}




MsgBGTalkWin *func_ov021_02190f64(FieldMsgBG *msgBGSys, MsgData *msgData) {
    return func_ov021_02190dcc(msgBGSys, msgData, 1, 0x13, 0x1e, 4);
}

void func_ov021_02190f7c(MsgBGTalkWin *window) {
    GFLBitmap *bitmap;

    bitmap = BmpWin_GetBitmap(window->bmpWin);
    GFL_BitmapFill(bitmap, 0xff);
    GFL_BGSysLoadScr(window->msgBG->bgId);
}

BmpWin *func_ov021_02190f9c(MsgBGTalkWin *window) {
    return window->bmpWin;
}

MsgBGTalkWin *func_ov021_02190fa0(FieldMsgBG *param_1, MsgData *param_2, u32 param_3) {
    MsgBGTalkWin *window;
    BmpWin *bmpWin;
    StrBuf *strBuf;

    param_1->unk_6 = 0xe;
    func_ov021_02192f74(param_1);
    window = GFL_HeapAllocate(param_1->heapId, sizeof(MsgBGTalkWin), 1, "field_msgbg.c", 0x9f4);
    window->msgBG = param_1;
    window->msgData = param_2;
    bmpWin = func_ov021_02192b20(1, (u8)param_3, 0x1e, 4, param_1->heapId);
    window->bmpWin = bmpWin;
    strBuf = GFL_StrBufCreate(0xa0, param_1->heapId);
    window->strBuf = strBuf;
    func_ov021_021927c0(&window->cursor, param_1);
    func_ov021_02193144(param_1->heapId);
    setAlphaBlend(1);
    return window;
}


void func_ov021_02191008(MsgBGTalkWin *window) {
    func_ov021_02192bc0(window->bmpWin);
    if (window->stream != NULL) {
        func_ov021_02190d48(window->stream);
    }
    if (window->win != NULL) {
        func_ov021_02190538(window->win);
    }
    GFL_StrBufFree(window->strBuf);
    func_ov021_021927e8(&window->cursor);
    GFL_HeapFree(window);
}

void func_ov021_0219103c(MsgBGTalkWin *window, u16 param_2, u8 param_3, u32 messageId) {
    s32 result;

    if (window->stream != NULL) {
        func_ov021_02190d48(window->stream);
    }
    GFL_MsgDataLoadStrbuf(window->msgData, messageId, window->strBuf);
    result = func_ov021_02193210(&window->msgBG->input);
    window->stream = func_ov021_02190d2c(window->msgBG, window->strBuf, window->bmpWin, param_2 + 8, param_3, result);
}

void func_ov021_02191084(MsgBGTalkWin *window, u16 param_2, u8 param_3, StrBuf *param_4) {
    s32 result;

    if (window->stream != NULL) {
        func_ov021_02190d48(window->stream);
    }
    result = func_ov021_02193210(&window->msgBG->input);
    window->stream = func_ov021_02190d2c(window->msgBG, param_4, window->bmpWin, param_2 + 8, param_3, result);
}

BOOL func_ov021_021910c0(MsgBGTalkWin *window) {
    u32 result;

    result = func_ov021_02190d5c(window->stream);
    GCTX_HIDGetPressedKeys();
    GCTX_HIDGetHeldKeys();
    switch (result) {
    case 0:
        if (func_ov021_021927f8(&window->cursor) == 1) {
            func_ov021_021927fc(&window->cursor, BmpWin_GetBitmap(window->bmpWin), 0xf);
            BmpWin_FlushChar(window->bmpWin);
        }
        break;
    case 1:
        if (func_ov021_0219320c(&window->msgBG->input) == 0) {
            func_ov021_021928c0(&window->cursor, BmpWin_GetBitmap(window->bmpWin), 0xf);
            BmpWin_FlushChar(window->bmpWin);
        }
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}




void func_ov021_02191134(MsgBGTalkWin *window) {
    GFLBitmap *pGVar2;

    if (func_ov021_0219320c(&window->msgBG->input) == 0) {
        pGVar2 = BmpWin_GetBitmap(window->bmpWin);
        func_ov021_021928c0(&window->cursor, pGVar2, 0xf);
        BmpWin_FlushChar(window->bmpWin);
    }
}

void func_ov021_02191164(MsgBGTalkWin *window) {
    GFLBitmap *bitmap;

    bitmap = BmpWin_GetBitmap(window->bmpWin);
    GFL_BitmapFill(bitmap, 0xff);
    GFL_BGSysLoadScr(window->msgBG->bgId);
}

void func_ov021_02191184(MsgBGTalkWin *window, u32 param_2, u8 param_3, StrBuf *param_4) {
    FieldMsgBGWin *win;

    if (window->win != NULL) {
        func_ov021_02190538(window->win);
    }
    win = func_ov021_021904ec(window->msgBG, NULL, window->bmpWin);
    window->win = win;
    func_ov021_02190590(win, param_2 + 8, param_3, param_4);
}


BmpWin *func_ov021_021911b4(MsgBGTalkWin *window) {
    return window->bmpWin;
}

void ActorMsgWin_Create(FieldMsgBG *param_1, ActorMsgWin *param_2, u32 param_3, const VecFx32 *param_4, StrBuf *param_5, s32 param_6, u32 param_7) {
    s32 result;

    param_2->pos = param_4;
    param_2->posCopy = *param_4;
    param_1->unk_6 = 0xb;
    param_2->msgBG = param_1;
    param_2->talkMsgWin = param_1->talkMsgWin;
    param_2->mode = param_3;
    loadPltt(param_1->heapId);
    setAlphaBlend(0);
    switch (param_3) {
    case 0:
        result = func_ov021_02193210(&param_1->input);
        ActorMsgWin_CreateAtTop(param_2->talkMsgWin, 0, param_4, param_5, 0xf, param_6, param_7, result);
        break;
    case 1:
        result = func_ov021_02193210(&param_1->input);
        ActorMsgWin_CreateAtBottom(param_2->talkMsgWin, 1, param_4, param_5, 0xf, param_6, param_7, result);
        break;
    default:
        result = func_ov021_02193210(&param_1->input);
        func_ov021_021948cc(param_2->talkMsgWin, 2, param_4, param_5, 0xf, param_6, result);
        break;
    }
    func_ov021_02193200(&param_1->input);
    func_ov021_02194940(param_2->talkMsgWin, param_2->mode);
    func_ov021_021927c0(&param_2->cursor, param_1);
    if (param_6 == 1) {
        param_2->scroll = 8;
        GFL_SndSEPlay(0x72c);
    }
}


ActorMsgWin *ActorMsgWin_CheckAndCreate(FieldMsgBG *param_1, int param_2, const VecFx32 *param_3, StrBuf *param_4, u32 param_5, u32 param_6) {
    ActorMsgWin *win;

    if (param_2 < 3) {
        win = &param_1->actorWins[param_2];
        if (win->talkMsgWin == NULL) {
            ActorMsgWin_Create(param_1, win, param_2, param_3, param_4, param_5, param_6);
            return win;
        }
    }
    return NULL;
}


void func_ov021_021912d8(ActorMsgWin *msgWin) {
    if (msgWin->strBuf != NULL) {
        GFL_StrBufFree(msgWin->strBuf);
    }
    func_ov021_021927e8(&msgWin->cursor);
    func_ov021_02194950(msgWin->talkMsgWin, msgWin->mode);
}

BOOL func_ov021_021912f8(ActorMsgWin *msgWin) {
    BOOL BVar1;

    BVar1 = func_ov021_02194960(msgWin->talkMsgWin, msgWin->mode);
    if (BVar1 != 0) {
        MI_CpuFill8(msgWin, 0, sizeof(ActorMsgWin));
        return 1;
    }
    return 0;
}

void func_ov021_0219131c(ActorMsgWin *msgWin) {
    if (msgWin->strBuf != NULL) {
        GFL_StrBufFree(msgWin->strBuf);
    }
    func_ov021_0219492c(msgWin->talkMsgWin, msgWin->mode);
    func_ov021_021927e8(&msgWin->cursor);
    MI_CpuFill8(msgWin, 0, sizeof(ActorMsgWin));
}

void func_ov021_02191348(ActorMsgWin *param_1, StrBuf *param_2) {
    BmpWin *window;
    GFLBitmap *bitmap;
    u8 bg;
    s32 result;

    window = func_ov021_02194990(param_1->talkMsgWin, param_1->mode);
    bitmap = BmpWin_GetBitmap(window);
    GFL_BitmapFill(bitmap, 0xff);
    result = func_ov021_02193210(&param_1->msgBG->input);
    func_ov021_0219499c(param_1->talkMsgWin, param_1->mode, param_2, result);
    bg = BmpWin_GetBGIndex(window);
    GFL_BGSysLoadScr(bg);
}

BOOL func_ov021_02191388(ActorMsgWin *param_1) {
    PrintStream *stream;
    BmpWin *bmpWin;
    GFLBitmap *bitmap;
    u32 result;

    if (!func_ov021_02194974(param_1->talkMsgWin, param_1->mode)) {
        return FALSE;
    }
    stream = func_ov021_02194984(param_1->talkMsgWin, param_1->mode);
    result = func_0201ce30(stream);
    GFL_BGSysMoveBG(1, 3, param_1->scroll);
    if (param_1->scroll < 0) {
        param_1->scroll += 2;
        if (param_1->scroll > 0) {
            param_1->scroll = 0;
        }
    }
    param_1->scroll = -param_1->scroll;
    switch (result) {
    case 0:
        if (func_ov021_021927f8(&param_1->cursor) == 1) {
            bmpWin = func_ov021_02194990(param_1->talkMsgWin, param_1->mode);
            bitmap = BmpWin_GetBitmap(bmpWin);
            func_ov021_021927fc(&param_1->cursor, bitmap, 0xf);
            BmpWin_FlushChar(bmpWin);
        }
        if (func_ov021_02193228(&param_1->msgBG->input)) {
            func_ov021_021932a0(stream, TRUE);
        } else {
            func_ov021_021932a0(stream, FALSE);
        }
        break;
    case 1:
        bmpWin = func_ov021_02194990(param_1->talkMsgWin, param_1->mode);
        bitmap = BmpWin_GetBitmap(bmpWin);
        if (func_ov021_0219320c(&param_1->msgBG->input) == 0) {
            func_ov021_021928c0(&param_1->cursor, bitmap, 0xf);
        }
        if (func_ov021_02193260(&param_1->msgBG->input, func_0201ce34(stream))) {
            if (func_ov021_0219320c(&param_1->msgBG->input) == 0) {
                GFL_SndSEPlay(0x547);
            }
            func_0201ce38(stream);
            func_ov021_021927fc(&param_1->cursor, bitmap, 0xf);
            func_ov021_021932a0(stream, 0);
        }
        BmpWin_FlushChar(bmpWin);
        break;
    case 2:
        if (param_1->scroll != 0) {
            param_1->scroll = 0;
            GFL_BGSysMoveBG(1, 3, param_1->scroll);
        }
        return TRUE;
    }
    return FALSE;
}



void func_ov021_021914cc(ActorMsgWin *msgWin) {
    BmpWin *window;
    GFLBitmap *pGVar2;

    if (func_ov021_0219320c(&msgWin->msgBG->input) == 0) {
        window = func_ov021_02194990(msgWin->talkMsgWin, msgWin->mode);
        pGVar2 = BmpWin_GetBitmap(window);
        func_ov021_021928c0(&msgWin->cursor, pGVar2, 0xf);
        BmpWin_FlushChar(window);
    }
}

void func_ov021_02191504(FieldMsgBG *param_1, MsgBGBalloonWin *param_2, u16 param_3, u16 param_4, u16 param_5, u16 param_6, u32 param_7) {
    param_1->unk_6 = 0xb;
    param_2->talkMsgWin = param_1->talkMsgWin;
    loadPltt(param_1->heapId);
    setAlphaBlend(0);
    param_2->bmpWin = func_ov021_02194a00(param_2->talkMsgWin, param_3, param_4, param_5, param_6, param_7);
    func_ov021_021927c0(&param_2->cursor, param_1);
}

MsgBGBalloonWin *func_ov021_02191558(FieldMsgBG *param_1, MsgData *param_2, u32 param_3, u16 param_4, u16 param_5, u16 param_6, u16 param_7) {
    MsgBGBalloonWin *win;

    win = GFL_HeapAllocate(param_1->heapId, sizeof(MsgBGBalloonWin), 1, "field_msgbg.c", 0xca7);
    win->msgBG = param_1;
    win->msgData = param_2;
    win->strBuf = GFL_StrBufCreate(0xa0, param_1->heapId);
    func_ov021_02191504(param_1, win, param_4, param_5, param_6, param_7, param_3);
    if (param_3 == 1) {
        win->scroll = 8;
    }
    return win;
}



void func_ov021_021915b4(MsgBGBalloonWin *window) {
    if (window->stream != NULL) {
        func_ov021_02190d48(window->stream);
    }
    if (window->win != NULL) {
        func_ov021_02190538(window->win);
    }
    if (window->strBuf != NULL) {
        GFL_StrBufFree(window->strBuf);
    }
    func_ov021_02194a48(window->talkMsgWin, window->bmpWin);
    func_ov021_021927e8(&window->cursor);
    GFL_HeapFree(window);
}

void func_ov021_021915ec(MsgBGBalloonWin *window) {
    GFLBitmap *bitmap;

    bitmap = BmpWin_GetBitmap(window->bmpWin);
    GFL_BitmapFill(bitmap, 0xff);
    GFL_BGSysLoadScr(window->msgBG->bgId);
}

void func_ov021_0219160c(MsgBGBalloonWin *window) {
    func_ov021_02194a34(window->talkMsgWin, window->bmpWin);
}

void func_ov021_0219161c(MsgBGBalloonWin *window, u32 param_2, u32 param_3, u32 messageId) {
    FieldMsgBGWin *win;

    if (window->win != NULL) {
        func_ov021_02190538(window->win);
    }
    win = func_ov021_021904ec(window->msgBG, window->msgData, window->bmpWin);
    window->win = win;
    func_ov021_02190550(win, param_2, param_3, messageId);
}

void func_ov021_02191648(MsgBGBalloonWin *window, u8 param_2, u8 param_3, StrBuf *param_4) {
    FieldMsgBGWin *win;

    if (window->win != NULL) {
        func_ov021_02190538(window->win);
    }
    win = func_ov021_021904ec(window->msgBG, NULL, window->bmpWin);
    window->win = win;
    func_ov021_02190590(win, param_2, param_3, param_4);
}

void func_ov021_02191674(MsgBGBalloonWin *window, u16 x, u16 y, StrBuf *strbuf, u16 color) {
    if (window->win != NULL) {
        func_ov021_02190538(window->win);
    }
    window->win = func_ov021_021904ec(window->msgBG, NULL, window->bmpWin);
    func_ov021_02190604(window->win, x, y, strbuf, color);
}

void func_ov021_021916a8(MsgBGBalloonWin *window, u8 param_2, u8 param_3, StrBuf *param_4) {
    s32 result;

    if (window->stream != NULL) {
        func_ov021_02190d48(window->stream);
    }
    result = func_ov021_02193210(&window->msgBG->input);
    window->stream = func_ov021_02190d2c(window->msgBG, param_4, window->bmpWin, param_2, param_3, result);
}

BOOL func_ov021_021916e0(MsgBGBalloonWin *window) {
    u32 result;

    GCTX_HIDGetPressedKeys();
    GCTX_HIDGetHeldKeys();
    result = func_ov021_02190d5c(window->stream);
    window->state = func_0201ce30(window->stream->stream);
    GFL_BGSysMoveBG(1, 3, window->scroll);
    if (window->scroll < 0) {
        window->scroll += 2;
        if (window->scroll > 0) {
            window->scroll = 0;
        }
    }
    window->scroll = -window->scroll;
    switch (result) {
    case 0:
        if (func_ov021_021927f8(&window->cursor) == 1) {
            func_ov021_021927fc(&window->cursor, BmpWin_GetBitmap(window->bmpWin), 0xf);
            BmpWin_FlushChar(window->bmpWin);
        }
        break;
    case 1:
        if (func_ov021_0219320c(&window->msgBG->input) == 0) {
            func_ov021_021928c0(&window->cursor, BmpWin_GetBitmap(window->bmpWin), 0xf);
            BmpWin_FlushChar(window->bmpWin);
        }
        break;
    case 2:
        if (window->scroll != 0) {
            window->scroll = 0;
            GFL_BGSysMoveBG(1, 3, window->scroll);
        }
        return TRUE;
    }
    return FALSE;
}






void func_ov021_02191794(MsgBGBalloonWin *window) {
    GFLBitmap *pGVar2;

    if (func_ov021_0219320c(&window->msgBG->input) == 0) {
        pGVar2 = BmpWin_GetBitmap(window->bmpWin);
        func_ov021_021928c0(&window->cursor, pGVar2, 0xf);
        BmpWin_FlushChar(window->bmpWin);
    }
}

u32 func_ov021_021917c0(MsgBGBalloonWin *window) {
    return window->state;
}

void func_ov021_021917c4(FieldMsgBG *param_1, MsgBGNameWin *param_2, StrBuf *param_3, u32 param_4, u8 param_5, u8 param_6, u8 param_7, u8 param_8, u32 param_9) {
    s32 result;

    param_1->unk_6 = 0xb;
    param_2->talkMsgWin = param_1->talkMsgWin;
    param_2->slot = param_4;
    param_2->index = param_9;
    loadPltt(param_1->heapId);
    setAlphaBlend(0);
    result = func_ov021_02193210(&param_1->input);
    func_ov021_021947e8(param_2->talkMsgWin, param_2->slot, param_3, param_5, param_6, param_7, param_8, 0xff, 0, result, 2, 0);
    func_ov021_02194940(param_2->talkMsgWin, param_2->slot);
}

static inline u32 PixelsToTiles(u32 pixels) {
    int rest = pixels;
    pixels >>= 3;
    if (rest & 7) {
        pixels++;
    }
    return pixels;
}

// The numbered windows' text origin in pixels, also included in their measured bounds (pokebw2's field_msgbg.c)
typedef struct {
    u8 x;
    u8 y;
} NumberedMsgWinTextOrigin;

static inline void NumberedMsgWin_InitTextOrigin(NumberedMsgWinTextOrigin *origin) {
    origin->x = 2;
    origin->y = 0;
}

void func_ov021_02191830(FieldMsgBG *sys, StrBuf *str, u8 *width, u8 *height) {
    NumberedMsgWinTextOrigin origin;
    u32 w;
    u32 h;
    u32 pixels;

    NumberedMsgWin_InitTextOrigin(&origin);
    w = GFL_FontGetBlockWidth(str, sys->font, 0) + origin.x;
    h = GFL_FontGetBlockHeight(str, sys->font) + origin.y;

    // Each size becomes tiles in place, rounded up when its pixels don't fill the last tile
    pixels = w;
    w = pixels / 8;
    if (pixels % 8) {
        w++;
    }
    *width = w;
    pixels = h;
    h = pixels / 8;
    if (pixels % 8) {
        h++;
    }
    *height = h;
}



void func_ov021_02191870(FieldMsgBG *msgBGSys, MsgData *msgData, u32 msgId, u16 index, u8 x, u8 y, u8 width, u8 a7) {
    MsgBGNameWin *win;
    s32 result;
    StrBuf *strbuf;

    win = GFL_HeapAllocate(msgBGSys->heapId, sizeof(MsgBGNameWin), 1, "field_msgbg.c", 0xe2a);
    result = func_ov021_0219319c(msgBGSys, win);
    strbuf = GFL_StrBufCreate(0x40, msgBGSys->heapId);
    win->strBuf = strbuf;
    GFL_MsgDataLoadStrbuf(msgData, msgId, strbuf);
    func_ov021_021917c4(msgBGSys, win, win->strBuf, result + 3U & 0xffff, x, y, width, a7, (u32)index);
}

void func_ov021_021918dc(FieldMsgBG *param_1, StrBuf *param_2, u32 param_3, u8 param_4, u8 param_5, u8 param_6, u8 param_7) {
    MsgBGNameWin *win;
    s32 result;
    StrBuf *dest;

    win = GFL_HeapAllocate(param_1->heapId, sizeof(MsgBGNameWin), 1, "field_msgbg.c", 0xe48);
    result = func_ov021_0219319c(param_1, win);
    dest = GFL_StrBufCreate(GFL_StrBufGetCharCount(param_2) + 1, param_1->heapId);
    win->strBuf = dest;
    GFL_StrBufCopy(dest, param_2);
    func_ov021_021917c4(param_1, win, win->strBuf, result + 3U & 0xffff, param_4, param_5, param_6, param_7, param_3);
}

void func_ov021_0219194c(MsgBGNameWin *param_1) {
    if (param_1->strBuf != NULL) {
        GFL_StrBufFree(param_1->strBuf);
    }
    func_ov021_0219492c(param_1->talkMsgWin, param_1->slot);
    GFL_HeapFree(param_1);
}

void func_ov021_0219196c(FieldMsgBG *msgBGSys, u16 index) {
    func_ov021_0219194c(func_ov021_021931c4(msgBGSys, index));
}

void func_ov021_02191978(FieldMsgBG *msgBGSys) {
    MsgBGNameWin *ptr;
    s32 i;

    i = 0;
    do {
        if (msgBGSys->nameWins[i] != NULL) {
            msgBGSys->nameWins[i] = NULL;
            ptr = msgBGSys->nameWins[i];
            msgBGSys->nameWins[i] = NULL;
            func_ov021_0219194c(ptr);
        }
        i = i + 1;
    } while (i < 8);
}


BOOL func_ov021_021919ac(FieldMsgBG *msgBGSys) {
    s32 i;

    i = 0;
    do {
        if (msgBGSys->nameWins[i] != NULL) {
            return 1;
        }
        i = i + 1;
    } while (i < 8);
    return 0;
}

u32 func_ov021_021919c8(MsgBGNameWin *param_1) {
    BOOL BVar1;
    PrintStream *stream;
    u32 result;

    BVar1 = func_ov021_02194974(param_1->talkMsgWin, param_1->slot);
    if (BVar1 == 0) {
        return 0;
    }
    GCTX_HIDGetPressedKeys();
    GCTX_HIDGetHeldKeys();
    stream = func_ov021_02194984(param_1->talkMsgWin, param_1->slot);
    result = func_0201ce30(stream);
    if (((result != 0) && (result != 1)) && (result == 2)) {
        return 1;
    }
    return 0;
}

MsgBGScrollWin *func_ov021_02191a04(FieldMsgBG *param_1, u32 param_2) {
    return func_ov021_02191a10(param_1, param_2, param_1->font);
}

MsgBGScrollWin *func_ov021_02191a10(FieldMsgBG *param_1, u32 param_2, Font *param_3) {
    MsgBGScrollWin *block;
    u8 x;
    u8 y;
    u8 w;

    block = GFL_HeapAllocate(param_1->heapId, sizeof(MsgBGScrollWin), 1, "field_msgbg.c", 0xee0);
    block->msgBG = param_1;
    block->font = param_3;
    block->scrollY = -0x30;
    GFL_BGSysMoveBG(param_1->bgId, 0, 0);
    GFL_BGSysMoveBG(block->msgBG->bgId, 3, block->scrollY);
    block->bmpWin = BmpWin_CreateDynamic(param_1->bgId, 2, 0x13, 0x1c, 4, 9, 1);
    block->bitmap1 = GFL_BitmapCreate(0x1c, 4, 0x20, param_1->heapId);
    block->bitmap2 = GFL_BitmapCreate(0x1c, 4, 0x20, param_1->heapId);
    GFL_BitmapFill(BmpWin_GetBitmap(block->bmpWin), 0xf);
    BmpWin_FlushMap(block->bmpWin);
    BmpWin_FlushChar(block->bmpWin);
    func_ov021_02191d88(param_1->bgId, 9, param_2, param_1->heapId);
    x = BmpWin_GetPosX(block->bmpWin);
    y = BmpWin_GetPosY(block->bmpWin);
    w = func_02045738(block->bmpWin);
    func_ov021_02191dc8(param_1->bgId, x, y, w, func_02045740(block->bmpWin), 0x33, 9);
    GFL_BGSysLoadScr(param_1->bgId);
    func_ov021_021927c0(&block->cursor, block->msgBG);
    param_1->unk_6 = 0xb;
    setAlphaBlend(0);
    return block;
}


void func_ov021_02191b14(MsgBGScrollWin *window) {
    u8 posX;
    u8 posY;
    u8 width;
    u8 height;

    BmpWin_GetBitmap(window->bmpWin);
    posX = BmpWin_GetPosX(window->bmpWin);
    posY = BmpWin_GetPosY(window->bmpWin);
    width = func_02045738(window->bmpWin);
    height = func_02045740(window->bmpWin);
    func_ov021_02191fa0(window->msgBG->bgId, posX, posY, width, height, 0, 0xf);
    func_020456f8(window->bmpWin);
    posX = BmpWin_GetPosX(window->bmpWin);
    posY = BmpWin_GetPosY(window->bmpWin);
    width = func_02045738(window->bmpWin);
    height = func_02045740(window->bmpWin);
    GFL_BGSysFillScrArea(window->msgBG->bgId, 0, posX, posY, width, height, 0x11);
    BmpWin_FlushChar(window->bmpWin);
    BmpWin_Free(window->bmpWin);
    GFL_BGSysLoadScr(window->msgBG->bgId);
    GFL_BGSysMoveBG(window->msgBG->bgId, 3, 0);
    GFL_BitmapFree(window->bitmap2);
    GFL_BitmapFree(window->bitmap1);
    if (window->lineBuf != NULL) {
        GFL_StrBufFree(window->lineBuf);
    }
    func_ov021_021927e8(&window->cursor);
    GFL_HeapFree(window);
}

BOOL func_ov021_02191be8(MsgBGScrollWin *window, StrBuf *strbuf) {
    u32 keys;

    switch (window->state) {
    case 0:
        window->strBuf = strbuf;
        window->lineBuf = GFL_StrBufCreate(GFL_StrBufGetCharCount(strbuf) + 2, window->msgBG->heapId);
        func_ov021_0219214c(BmpWin_GetBitmap(window->bmpWin), window->font, window->strBuf, window->lineBuf, &window->line, 0xf);
        BmpWin_FlushChar(window->bmpWin);
        GFL_BGSysLoadScr(window->msgBG->bgId);
        window->state++;
        break;
    case 1:
        window->scrollY += 0xc;
        if (window->scrollY >= 0) {
            window->scrollY = 0;
            window->state++;
        }
        GFL_BGSysMoveBG(window->msgBG->bgId, 3, window->scrollY);
        break;
    case 2:
        if (func_ov021_02192138(window->strBuf, window->lineBuf, window->line) == 1) {
            window->state = 5;
            break;
        }
        window->state++;
        /* fall through */
    case 3:
        keys = GCTX_HIDGetPressedKeys();
        if ((keys & 3) == 0) {
            func_ov021_021928c0(&window->cursor, BmpWin_GetBitmap(window->bmpWin), 0xf);
            BmpWin_FlushChar(window->bmpWin);
            break;
        }
        func_ov021_021927fc(&window->cursor, BmpWin_GetBitmap(window->bmpWin), 0xf);
        GFL_BitmapCopy(BmpWin_GetBitmap(window->bmpWin), window->bitmap2);
        func_ov021_0219214c(window->bitmap1, window->font, window->strBuf, window->lineBuf, &window->line, 0xf);
        window->scrollLine = 0;
        GFL_SndSEPlay(0x547);
        window->state++;
        /* fall through */
    case 4:
        if (func_ov021_021921c8(BmpWin_GetBitmap(window->bmpWin), window->bitmap2, window->bitmap1, window->scrollLine, 0xf) == TRUE) {
            window->state = 2;
        } else {
            window->scrollLine += 4;
        }
        BmpWin_FlushChar(window->bmpWin);
        GFL_BGSysLoadScr(window->msgBG->bgId);
        break;
    case 5:
        keys = GCTX_HIDGetPressedKeys();
        if ((keys & 0xf3) == 0) {
            break;
        }
        window->state++;
        /* fall through */
    case 6:
        window->scrollY -= 0xc;
        if (window->scrollY <= -0x30) {
            window->scrollY = -0x30;
        }
        GFL_BGSysMoveBG(window->msgBG->bgId, 3, window->scrollY);
        if (window->scrollY != -0x30) {
            break;
        }
        window->state++;
        return TRUE;
    case 7:
        return TRUE;
    }
    return FALSE;
}


void func_ov021_02191d88(u16 param_1, s32 param_2, u32 param_3, HeapID param_4) {
    if (param_3 >= 4) {
        param_3 = 0;
    }
    GFL_BGSysLoadNCGRStatic(5, 6, param_1, 0x33, 0, 0, param_4);
    GFL_G2DIOLoadNCLR(5, 1, 0, param_3 << 5, param_2 << 5, 0x20, param_4);
}

void func_ov021_02191dc8(u8 param_1, u8 param_2, u8 param_3, u8 param_4, u8 param_5, u16 param_6, u8 param_7) {
    GFL_BGSysFillScrArea(param_1, param_6, param_2 - 2, param_3 - 1, 1, 1, param_7);
    GFL_BGSysFillScrArea(param_1, param_6 + 1, param_2 - 1, param_3 - 1, 1, 1, param_7);
    GFL_BGSysFillScrArea(param_1, param_6 + 2, param_2, param_3 - 1, param_4, 1, param_7);
    GFL_BGSysFillScrArea(param_1, param_6 + 4, param_2 + param_4, param_3 - 1, 1, 1, param_7);
    GFL_BGSysFillScrArea(param_1, param_6 + 5, param_2 + param_4 + 1, param_3 - 1, 1, 1, param_7);
    GFL_BGSysFillScrArea(param_1, param_6 + 6, param_2 - 2, param_3, 1, param_5, param_7);
    GFL_BGSysFillScrArea(param_1, param_6 + 7, param_2 - 1, param_3, 1, param_5, param_7);
    GFL_BGSysFillScrArea(param_1, param_6 + 10, param_2 + param_4, param_3, 1, param_5, param_7);
    GFL_BGSysFillScrArea(param_1, param_6 + 11, param_2 + param_4 + 1, param_3, 1, param_5, param_7);
    GFL_BGSysFillScrArea(param_1, param_6 + 12, param_2 - 2, param_3 + param_5, 1, 1, param_7);
    GFL_BGSysFillScrArea(param_1, param_6 + 13, param_2 - 1, param_3 + param_5, 1, 1, param_7);
    GFL_BGSysFillScrArea(param_1, param_6 + 14, param_2, param_3 + param_5, param_4, 1, param_7);
    GFL_BGSysFillScrArea(param_1, param_6 + 16, param_2 + param_4, param_3 + param_5, 1, 1, param_7);
    GFL_BGSysFillScrArea(param_1, param_6 + 17, param_2 + param_4 + 1, param_3 + param_5, 1, 1, param_7);
}

void func_ov021_02191fa0(u8 param_1, u8 param_2, u8 param_3, u8 param_4, u8 param_5, u16 param_6, u8 param_7) {
    GFL_BGSysFillScrArea(param_1, param_6, param_2 - 2, param_3 - 1, 1, 1, param_7);
    GFL_BGSysFillScrArea(param_1, param_6, param_2 - 1, param_3 - 1, 1, 1, param_7);
    GFL_BGSysFillScrArea(param_1, param_6, param_2, param_3 - 1, param_4, 1, param_7);
    GFL_BGSysFillScrArea(param_1, param_6, param_2 + param_4, param_3 - 1, 1, 1, param_7);
    GFL_BGSysFillScrArea(param_1, param_6, param_2 + param_4 + 1, param_3 - 1, 1, 1, param_7);
    GFL_BGSysFillScrArea(param_1, param_6, param_2 - 2, param_3, 1, param_5, param_7);
    GFL_BGSysFillScrArea(param_1, param_6, param_2 - 1, param_3, 1, param_5, param_7);
    GFL_BGSysFillScrArea(param_1, param_6, param_2 + param_4, param_3, 1, param_5, param_7);
    GFL_BGSysFillScrArea(param_1, param_6, param_2 + param_4 + 1, param_3, 1, param_5, param_7);
    GFL_BGSysFillScrArea(param_1, param_6, param_2 - 2, param_3 + param_5, 1, 1, param_7);
    GFL_BGSysFillScrArea(param_1, param_6, param_2 - 1, param_3 + param_5, 1, 1, param_7);
    GFL_BGSysFillScrArea(param_1, param_6, param_2, param_3 + param_5, param_4, 1, param_7);
    GFL_BGSysFillScrArea(param_1, param_6, param_2 + param_4, param_3 + param_5, 1, 1, param_7);
    GFL_BGSysFillScrArea(param_1, param_6, param_2 + param_4 + 1, param_3 + param_5, 1, 1, param_7);
}




u32 func_ov021_02192138(StrBuf *param_1, StrBuf *param_2, u32 param_3) {
    BOOL BVar1;

    BVar1 = func_0201d37c(param_1, param_2, param_3);
    if (BVar1 == 0) {
        return 1;
    }
    return 0;
}

u32 func_ov021_0219214c(GFLBitmap *bitmap, Font *font, StrBuf *src, StrBuf *dest, u32 *line, u8 color) {
    s32 y;

    GFL_BitmapFill(bitmap, color);
    y = 1;
    if (func_0201d37c(src, dest, *line) == 0) {
        return 1;
    }
    GFL_TextRendererDrawToBitmap(bitmap, 1, y, dest, font);
    y += GFL_FontGetBlockHeight(dest, font) + 1;
    *line = *line + 1;
    if (func_0201d37c(src, dest, *line) == 0) {
        return 1;
    }
    GFL_TextRendererDrawToBitmap(bitmap, 1, y, dest, font);
    *line = *line + 1;
    return 0;
}

BOOL func_ov021_021921c8(GFLBitmap *param_1, GFLBitmap *param_2, GFLBitmap *param_3, s32 param_4, u16 param_5) {
    s32 width = GFL_BitmapGetWidth(param_1);
    s32 height = GFL_BitmapGetHeight(param_1);
    s32 rest;
    GFL_BitmapFill(param_1, param_5);
    if (param_4 > height) {
        param_4 = height;
    }
    rest = height - param_4;
    GFL_BitmapCopyArea(param_2, param_1, 0, param_4, 0, 0, width, rest, param_5);
    GFL_BitmapCopyArea(param_3, param_1, 0, 0, 0, rest, width, param_4, param_5);
    if (param_4 >= height) {
        return TRUE;
    }
    return FALSE;
}



void func_ov021_02192244(u32 param_1, s32 param_2, HeapID param_3) {
    GFL_G2DIOLoadNCLR(5, 2, 0, 0, param_2 << 5, 0x20, param_3);
}

void func_ov021_021922a4(GFLBitmap *param_1, u32 param_2, HeapID param_3);
static inline void LoadCharTile(GFLBitmap *bitmap, HeapID heapId, u32 x, u32 y) {
    u8 *pixels;
    void *file;
    NNSG2dCharacterData *chr;

    pixels = GFL_BitmapGetPixelData(bitmap);
    file = GFL_ArcSysReadHeapNewLZ(5, 8, 0, heapId);
    NNS_G2dGetUnpackedBGCharacterData(file, &chr);
    sys_memcpy_fast((u8 *)chr->pRawData + y * 0x20 + x * 0x20, pixels, 0x20);
    GFL_HeapFree(file);
}
void func_ov021_02192264(GFLBitmap *param_1, u32 param_2, HeapID param_3, u32 param_4) {
    u8 *pixels;
    void *file;
    NNSG2dCharacterData *chr;
    GXBGColorMode y = GX_BG_COLORMODE_16;

    pixels = GFL_BitmapGetPixelData(param_1);
    file = GFL_ArcSysReadHeapNewLZ(5, 8, 0, param_3);
    NNS_G2dGetUnpackedBGCharacterData(file, &chr);
    sys_memcpy_fast((u8 *)chr->pRawData + y * 0x20 + param_4 * 0x20, pixels, 0x20);
    GFL_HeapFree(file);
}

void func_ov021_021922a4(GFLBitmap *bitmap, u32 style, HeapID heapId) {
    u16 x;
    u16 y;
    u16 tilesW = GFL_BitmapGetWidth(bitmap) >> 3;
    u16 tilesH = GFL_BitmapGetHeight(bitmap) >> 3;
    GFLBitmap *tile = GFL_BitmapCreate(1, 1, BGSYS_TILE_SIZE_16, heapId);

    func_ov021_02192264(tile, style, heapId, 9);
    for (y = 0; y < tilesH; y++) {
        for (x = 0; x < tilesW; x++) {
            GFL_BitmapCopyArea(tile, bitmap, 0, 0, x * 8, y * 8, 8, 8, GFL_BITMAP_NO_COLOR_KEY);
        }
    }
    func_ov021_02192264(tile, style, heapId, 1);
    for (x = 0; x < tilesW; x++) {
        GFL_BitmapCopyArea(tile, bitmap, 0, 0, x * 8, 0, 8, 8, GFL_BITMAP_NO_COLOR_KEY);
    }
    func_ov021_02192264(tile, style, heapId, 0x11);
    y = tilesH - 1;
    for (x = 0; x < tilesW; x++) {
        GFL_BitmapCopyArea(tile, bitmap, 0, 0, x * 8, y * 8, 8, 8, GFL_BITMAP_NO_COLOR_KEY);
    }
    func_ov021_02192264(tile, style, heapId, 8);
    for (y = 0; y < tilesH; y++) {
        GFL_BitmapCopyArea(tile, bitmap, 0, 0, 0, y * 8, 8, 8, GFL_BITMAP_NO_COLOR_KEY);
    }
    func_ov021_02192264(tile, style, heapId, 0xa);
    x = tilesW - 1;
    for (y = 0; y < tilesH; y++) {
        GFL_BitmapCopyArea(tile, bitmap, 0, 0, x * 8, y * 8, 8, 8, GFL_BITMAP_NO_COLOR_KEY);
    }
    x = 0;
    y = 0;
    func_ov021_02192264(tile, style, heapId, 0);
    GFL_BitmapCopyArea(tile, bitmap, 0, 0, x * 8, y * 8, 8, 8, GFL_BITMAP_NO_COLOR_KEY);
    x = tilesW - 1;
    y = 0;
    func_ov021_02192264(tile, style, heapId, 2);
    GFL_BitmapCopyArea(tile, bitmap, 0, 0, x * 8, y * 8, 8, 8, GFL_BITMAP_NO_COLOR_KEY);
    x = 0;
    y = tilesH - 1;
    func_ov021_02192264(tile, style, heapId, 0x10);
    GFL_BitmapCopyArea(tile, bitmap, 0, 0, x * 8, y * 8, 8, 8, GFL_BITMAP_NO_COLOR_KEY);
    x = tilesW - 1;
    y = tilesH - 1;
    func_ov021_02192264(tile, style, heapId, 0x12);
    GFL_BitmapCopyArea(tile, bitmap, 0, 0, x * 8, y * 8, 8, 8, GFL_BITMAP_NO_COLOR_KEY);
    GFL_BitmapFree(tile);
}


void func_ov021_021924f0(u16 param_1, u16 param_2, GFLBitmap *param_3, Font *param_4, StrBuf *param_5, HeapID param_6) {
    u32 h;
    u32 i;
    StrBuf *dest;

    h = GFL_FontGetCharHeight(param_4) + 2;
    dest = GFL_StrBufCreate(GFL_StrBufGetCharCount(param_5) + 1, param_6);
    i = 0;
    if (func_0201d37c(param_5, dest, 0) == 1) {
        do {
            GFL_TextRendererDrawToBitmap(param_3, param_1, param_2, dest, param_4);
            param_2 += h;
            i++;
        } while (func_0201d37c(param_5, dest, i) == 1);
    }
    GFL_StrBufFree(dest);
}

MsgBGSignWin *func_ov021_02192570(FieldMsgBG *param_1, u32 param_2, s16 param_3, u8 param_4, u16 param_5, u16 param_6) {
    MsgBGSignWin *block;
    BmpWin *bmpWin;
    GFLBitmap *bitmap;

    block = GFL_HeapAllocate(param_1->heapId, sizeof(MsgBGSignWin), 1, "field_msgbg.c", 0x115d);
    block->msgBG = param_1;
    bmpWin = BmpWin_CreateDynamic(param_1->bgId, param_3, (u32)param_4, param_5, param_6, 9, 1);
    block->bmpWin = bmpWin;
    bitmap = GFL_BitmapCreate(1, 1, 0x20, param_1->heapId);
    block->bitmap = bitmap;
    func_ov021_02192264(bitmap, param_2, param_1->heapId, 9);
    bitmap = BmpWin_GetBitmap(block->bmpWin);
    func_ov021_021922a4(bitmap, param_2, param_1->heapId);
    func_ov021_02192244(param_1->bgId, 9, param_1->heapId);
    BmpWin_FlushMap(block->bmpWin);
    BmpWin_FlushChar(block->bmpWin);
    GFL_BGSysLoadScr(param_1->bgId);
    func_ov021_021927c0(&block->cursor, param_1);
    param_1->unk_6 = 0xb;
    setAlphaBlend(0);
    return block;
}




void func_ov021_02192628(MsgBGSignWin *window) {
    u8 x;
    u8 y;
    u8 width;
    u8 height;

    func_020456f8(window->bmpWin);
    x = BmpWin_GetPosX(window->bmpWin);
    y = BmpWin_GetPosY(window->bmpWin);
    width = func_02045738(window->bmpWin);
    height = func_02045740(window->bmpWin);
    GFL_BGSysFillScrArea(window->msgBG->bgId, 0, x, y, width, height, 0x11);
    BmpWin_FlushChar(window->bmpWin);
    GFL_BGSysLoadScr(window->msgBG->bgId);
    func_ov021_021927e8(&window->cursor);
    GFL_BitmapFree(window->bitmap);
    BmpWin_Free(window->bmpWin);
    GFL_HeapFree(window);
}

void func_ov021_0219269c(MsgBGSignWin *param_1, u16 param_2, u16 param_3, StrBuf *param_4) {
    GFLBitmap *pGVar1;
    FieldMsgBG *msgBG;

    msgBG = param_1->msgBG;
    pGVar1 = BmpWin_GetBitmap(param_1->bmpWin);
    func_ov021_021924f0(param_2, param_3, pGVar1, msgBG->font, param_4, msgBG->heapId);
    BmpWin_FlushChar(param_1->bmpWin);
    GFL_BGSysLoadScr(param_1->msgBG->bgId);
}

BOOL func_ov021_021926d8(MsgBGSignWin *window) {
    u32 result;
    GFLBitmap *pGVar2;

    result = GCTX_HIDGetPressedKeys();
    GCTX_HIDGetHeldKeys();
    if ((result & 3) != 0) {
        return 1;
    }
    pGVar2 = BmpWin_GetBitmap(window->bmpWin);
    func_ov021_021928f8(&window->cursor, pGVar2, window->bitmap);
    BmpWin_FlushChar(window->bmpWin);
    return 0;
}

u32 func_ov021_0219270c(FieldMsgBGWin *param_1, StrBuf *param_2, s32 param_3) {
    u32 result;
    u32 i;

    result = (param_3 * 2) + GFL_FontGetBlockWidth(param_2, param_1->font, 0U);
    i = result >> 3;
    if (7 & result) {
        i += 1;
    }
    return i;
}

u32 func_ov021_0219272c(FieldMsgBG *param_1, StrBuf *param_2, s32 param_3) {
    u32 total;
    StrBuf *dest;
    u32 n;

    total = param_3 * 2 + GFL_FontGetBlockHeight(param_2, param_1->font);
    dest = GFL_StrBufClone(param_2, param_1->heapId);
    n = 0;
    if (func_0201d37c(param_2, dest, 0) == 1) {
        do {
            n++;
            if (n >= 0x18) {
                break;
            }
        } while (func_0201d37c(param_2, dest, n) == 1);
    }
    GFL_StrBufFree(dest);
    total += (n - 1) * 2;
    if ((total & 7) != 0) {
        total += 8;
    }
    return total >> 3;
}



MsgBGCursor *func_ov021_02192788(FieldMsgBG *param_1) {
    MsgBGCursor *ptr;

    ptr = GFL_HeapAllocate(param_1->heapId, sizeof(MsgBGCursor), 1, "field_msgbg.c", 0x1202);
    func_ov021_021927c0(ptr, param_1);
    return ptr;
}

void func_ov021_021927b0(MsgBGCursor *param_1) {
    func_ov021_021927e8(param_1);
    GFL_HeapFree(param_1);
}

void func_ov021_021927c0(MsgBGCursor *param_1, FieldMsgBG *param_2) {
    GFLBitmap *pGVar1;

    MI_CpuFill8(param_1, 0, sizeof(MsgBGCursor));
    pGVar1 = GFL_BitmapWrap(param_2->cursorChar->pRawData, 1, 1, 0x20, param_2->heapId);
    param_1->bitmap = pGVar1;
}

void func_ov021_021927e8(MsgBGCursor *param_1) {
    param_1->shown = 0;
    GFL_BitmapFree(param_1->bitmap);
}

u8 func_ov021_021927f8(MsgBGCursor *param_1) {
    return param_1->shown;
}

void func_ov021_021927fc(MsgBGCursor *param_1, GFLBitmap *param_2, int param_3) {
    u16 offsets[3] = {0, 1, 2};
    u16 x;
    u16 y;

    x = GFL_BitmapGetWidth(param_2) - 8;
    y = GFL_BitmapGetHeight(param_2) - 8 - offsets[param_1->frame];
    GFL_BitmapFillArea(param_2, x, y, 8, 8, param_3);
    param_1->shown = 0;
}

void func_ov021_02192860(MsgBGCursor *param_1, GFLBitmap *param_2, u8 param_3) {
    u16 offsets[3] = {0, 1, 2};
    u16 x;
    u16 y;

    x = GFL_BitmapGetWidth(param_2) - 8;
    y = GFL_BitmapGetHeight(param_2) - 8 - offsets[param_1->frame];
    GFL_BitmapCopyArea(param_1->bitmap, param_2, 0, 0, x, y, 8, 8, 0);
    param_1->shown = 1;
}


void func_ov021_021928c0(MsgBGCursor *param_1, GFLBitmap *param_2, u8 param_3) {
    func_ov021_021927fc(param_1, param_2, param_3);
    param_1->counter++;
    if (param_1->counter >= 4) {
        param_1->counter = 0;
        param_1->frame++;
        param_1->frame = (s32)param_1->frame % 3;
    }
    func_ov021_02192860(param_1, param_2, param_3);
}

void func_ov021_021928f8(MsgBGCursor *param_1, GFLBitmap *param_2, GFLBitmap *param_3) {
    u16 offsets[3] = {0, 1, 2};
    s16 x;
    s16 y;
    s16 rem;

    x = GFL_BitmapGetWidth(param_2) - 8;
    y = GFL_BitmapGetHeight(param_2) - 8 - offsets[param_1->frame];
    rem = y & 7;
    if (rem != 0) {
        GFL_BitmapCopyArea(param_3, param_2, 0, 8 - rem, x, y, 8, rem, 0xffff);
    }
    GFL_BitmapCopyArea(param_3, param_2, 0, 0, x, y + rem, 8, 8 - rem, 0xffff);
    param_1->counter++;
    if (param_1->counter >= 4) {
        param_1->counter = 0;
        param_1->frame++;
        param_1->frame = (s32)param_1->frame % 3;
    }
    x = GFL_BitmapGetWidth(param_2) - 8;
    y = GFL_BitmapGetHeight(param_2) - 8 - offsets[param_1->frame];
    GFL_BitmapCopyArea(param_1->bitmap, param_2, 0, 0, x, y, 8, 8, 0);
    param_1->shown = 1;
}

// Black's code at 0x021929e8-0x021932c0, once the port's src/ov021/ov021_021929e8.c: the same file (its .text follows ours
// without a gap, and its code reads this file's data)

void func_ov021_02192a04(BmpWin *param_1);
void func_ov021_02192b64(BmpWin *param_1);

void func_ov021_021929e8(HeapID param_1) {
    GFL_BGSysLoadNCGRStatic(5, 10, 1, 0x1f, 0, 0, param_1);
}

void func_ov021_02192a04(BmpWin *param_1) {
    u8 posX;
    u8 posY;
    u8 width;
    u8 height;
    u8 bg;
    s32 x;
    s32 y;
    s32 xEnd;
    s32 yEnd;

    posX = BmpWin_GetPosX(param_1);
    posY = BmpWin_GetPosY(param_1);
    width = func_02045738(param_1);
    height = func_02045740(param_1);
    bg = BmpWin_GetBGIndex(param_1);
    x = posX - 1;
    y = posY - 1;
    GFL_BGSysFillScrArea(bg, 0x1f, x, y, 1, 1, 0xd);
    GFL_BGSysFillScrArea(bg, 0x20, posX, y, width, 1, 0xd);
    xEnd = posX + width;
    GFL_BGSysFillScrArea(bg, 0x24, xEnd, y, 1, 1, 0xd);
    GFL_BGSysFillScrArea(bg, 0x25, x, posY, 1, height, 0xd);
    GFL_BGSysFillScrArea(bg, 0x2a, xEnd, posY, 1, height, 0xd);
    yEnd = posY + height;
    GFL_BGSysFillScrArea(bg, 0x2b, x, yEnd, 1, 1, 0xd);
    GFL_BGSysFillScrArea(bg, 0x2d, posX, yEnd, width, 1, 0xd);
    GFL_BGSysFillScrArea(bg, 0x30, xEnd, yEnd, 1, 1, 0xd);
    GFL_BGSysLoadScr(bg);
}


BmpWin *func_ov021_02192b20(u8 param_1, u8 param_2, u8 param_3, u8 param_4, HeapID param_5) {
    BmpWin *window;
    GFLBitmap *bitmap;

    window = BmpWin_CreateDynamic(2, param_1, param_2, param_3, param_4, 0xe, 1);
    bitmap = BmpWin_GetBitmap(window);
    GFL_BitmapFill(bitmap, 0xff);
    BmpWin_FlushMap(window);
    BmpWin_FlushChar(window);
    func_ov021_02192a04(window);
    return window;
}

void func_ov021_02192b64(BmpWin *param_1) {
    u8 bg;
    u8 posX;
    u8 posY;
    u8 width;
    u8 height;

    bg = BmpWin_GetBGIndex(param_1);
    posX = BmpWin_GetPosX(param_1);
    posY = BmpWin_GetPosY(param_1);
    width = func_02045738(param_1);
    height = func_02045740(param_1);
    GFL_BGSysFillScrArea(bg, 0, posX - 1, posY - 1, width + 2, height + 2, 0);
    GFL_BGSysLoadScr(bg);
}


void func_ov021_02192bc0(BmpWin *param_1) {
    func_ov021_02192b64(param_1);
    BmpWin_Free(param_1);
}



void func_ov021_02192bd0(FieldMsgBG *param_1, s32 param_2) {
    G3DCamera *camera;

    param_1->bgId = 1;
    {
        BGSetup setup = { 0, 0, 2048, 0, 1, 0, 0, 4, 0x8000, 0, 0, 0, 0 };
        GFL_BGSysCreateBG(param_1->bgId, &setup, 0);
    }
    param_1->unk_4 = 2;
    {
        BGSetup setup = { 0, 0, 2048, 0, 1, 0, 31, 4, 0x8000, 0, 2, 0, 0 };
        GFL_BGSysCreateBG(param_1->unk_4, &setup, 0);
    }
    GFL_BGSysFillChar(param_1->bgId, 0, 1, 0);
    GFL_BGSysFillScrArea(param_1->bgId, 0, 0, 0, 0x20, 0x20, 0x11);
    GFL_BGSysFillScrArea(param_1->unk_4, 0, 0, 0, 0x20, 0x20, 0x11);
    GFL_BGSysLoadScr(param_1->bgId);
    GFL_BGSysLoadScr(param_1->unk_4);
    if (param_2 != 0) {
        func_ov021_02192fa4(param_1->bgId, param_1->heapId);
    }
    camera = param_1->camera;
    if (camera != NULL) {
        TalkMsgWinSetup talkSetup;

        talkSetup.heapId = param_1->heapId;
        talkSetup.unk4 = camera;
        talkSetup.font = param_1->font;
        talkSetup.unkC = 0xb;
        talkSetup.bg = 1;
        talkSetup.unk14 = 0xc;
        talkSetup.unk15 = 0xb;
        param_1->talkMsgWin = func_ov021_02194670(&talkSetup);
        loadTexture_wrapper(param_1->talkMsgWin);
    }
    G2_SetBlendAlpha(4, 0x1b, 0x10, 4);
    GFL_BGSysSetBGEnabled(param_1->bgId, 1);
    GFL_BGSysSetBGEnabled(param_1->unk_4, 1);
    param_1->unk_6 = 0xe;
}

void func_ov021_02192cf0(FieldMsgBG *param_1, s32 param_2) {
    G3DCamera *camera;

    param_1->bgId = 1;
    {
        BGSetup setup = { 0, 0, 2048, 0, 1, 0, 0, 4, 0x8000, 0, 0, 0, 0 };
        GFL_BGSysCreateBG(param_1->bgId, &setup, 0);
    }
    param_1->unk_4 = 2;
    {
        BGSetup setup = { 0, 0, 2048, 0, 1, 0, 31, 4, 0x8000, 0, 2, 0, 0 };
        GFL_BGSysCreateBG(param_1->unk_4, &setup, 0);
    }
    GFL_BGSysFillChar(param_1->bgId, 0, 1, 0);
    GFL_BGSysFillScrArea(param_1->bgId, 0, 0, 0, 0x20, 0x20, 0x11);
    GFL_BGSysFillScrArea(param_1->unk_4, 0, 0, 0, 0x20, 0x20, 0x11);
    GFL_BGSysLoadScr(param_1->bgId);
    GFL_BGSysLoadScr(param_1->unk_4);
    if (param_2 != 0) {
        func_ov021_02192fa4(param_1->bgId, param_1->heapId);
    }
    camera = param_1->camera;
    if (camera != NULL) {
        TalkMsgWinSetup talkSetup;

        talkSetup.heapId = param_1->heapId;
        talkSetup.unk4 = camera;
        talkSetup.font = param_1->font;
        talkSetup.unkC = 0xb;
        talkSetup.bg = 1;
        talkSetup.unk14 = 0xc;
        talkSetup.unk15 = 0xb;
        param_1->talkMsgWin = func_ov021_02194670(&talkSetup);
    }
    G2_SetBlendAlpha(4, 0x1b, 0x10, 4);
    GFL_BGSysSetBGEnabled(param_1->bgId, 1);
    GFL_BGSysSetBGEnabled(param_1->unk_4, 1);
    param_1->unk_6 = 0xe;
}

void func_ov021_02192e0c(FieldMsgBG *param_1) {
    G3DCamera *camera;

    param_1->bgId = 1;
    {
        BGSetup setup = { 0, 0, 2048, 0, 1, 0, 0, 4, 0x8000, 0, 0, 0, 0 };
        GFL_BGSysCreateBG(param_1->bgId, &setup, 0);
    }
    GFL_BGSysFillChar(param_1->bgId, 0, 1, 0);
    GFL_BGSysFillScrArea(param_1->bgId, 0, 0, 0, 0x20, 0x20, 0x11);
    GFL_BGSysLoadScr(param_1->bgId);
    func_ov021_02192fa4(param_1->bgId, param_1->heapId);
    camera = param_1->camera;
    if (camera != NULL) {
        TalkMsgWinSetup talkSetup;

        talkSetup.heapId = param_1->heapId;
        talkSetup.unk4 = camera;
        talkSetup.font = param_1->font;
        talkSetup.unkC = 0xb;
        talkSetup.bg = 1;
        talkSetup.unk14 = 0xc;
        talkSetup.unk15 = 0xb;
        param_1->talkMsgWin = func_ov021_02194670(&talkSetup);
    }
    GFL_BGSysSetBGEnabled(param_1->bgId, 1);
    param_1->unk_6 = 0xe;
}




void func_ov021_02192eb8(TCB *param_1, void *param_2) {
    FieldMsgBG *msgBG = param_2;
    BOOL BVar1;

    BVar1 = GFL_IRQCartDataTransferIsEnabled();
    if (BVar1 == 0) {
        loadTexture_wrapper(msgBG->talkMsgWin);
        msgBG->vblankTcb = NULL;
        GFL_TCBRemove(param_1);
    }
}

void func_ov021_02192ee0(s32 param_1) {

    GFL_BGSysSetBGEnabled(2, 0);
    if (param_1 == 1) {
        BGSetup setup = { 0, 0, 2048, 0, 1, 0, 31, 4, 0x8000, 0, 2, 0, 0 };
        GFL_BGSysCreateBG(2, &setup, 0);
    } else {
        reg_G2_BG2CNT = (reg_G2_BG2CNT & 0x43) | 0x1f10;
    }
    GFL_BGSysFillScrArea(2, 0, 0, 0, 0x20, 0x20, 0x11);
    GFL_BGSysLoadScr(2);
    G2_SetBlendAlpha(4, 0x1b, 0x10, 4);
    GFL_BGSysSetBGPriority(2, 2);
    GFL_BGSysSetBGEnabled(2, 1);
}


void func_ov021_02192ee0(s32 param_1);

static const ListMenuRequest data_ov021_021d3f38 = {
    1, 10, 0, 13, 0, 3, 1, 15, 2, 0, 0, 1, 0, 0, 0, 12, 13, 0, 0, 0, 0, 0
};

void func_ov021_02192f74(FieldMsgBG *param_1) {
    if (param_1->resetBG2 != 0) {
        param_1->resetBG2 = 0;
        if (param_1->unk_4 == 0xff) {
            GFL_BGSysReleaseBG(2);
            func_ov021_02192ee0(1);
            param_1->unk_4 = 2;
            return;
        }
        func_ov021_02192ee0(0);
    }
}

void func_ov021_02192fa4(u16 bgId, HeapID heapId) {
    GFL_BGSysLoadNCLRDefault(0x17, 5, 0, 0x160, 0x20, heapId);
    GFL_BGSysLoadNCLRDefault(0x17, 7, 0, 0x1c0, 0x20, heapId);
    LoadSysMsgBox(bgId, 1, 0xd, 0, heapId);
    func_ov021_021929e8(heapId);
}



void func_ov021_02192fec(u16 bgId, HeapID heapId, s32 param_3) {
    if (param_3 == 0) {
        GFL_BGSysLoadNCLRDefault(0x17, 5, 0, 0x160, 0x20, heapId);
        GFL_BGSysLoadNCLRDefault(0x17, 7, 0, 0x1c0, 0x20, heapId);
        return;
    }
    if (param_3 == 1) {
        LoadSysMsgBox((u8)bgId, 1, 0xd, 0, heapId);
        return;
    }
    if (param_3 == 2) {
        func_ov021_021929e8(heapId);
    }
}

void setAlphaBlend(BOOL param_1) {
    if (param_1 == 1) {
        G2_SetBlendAlpha(4, 0x1b, 0x10, 4);
    }
}

void setAlphaBlend_wrapper(BOOL enable) {
    setAlphaBlend(enable);
}

void func_ov021_02193068(FieldMsgBG *param_1) {
    GFL_BGSysFreeFilledChar(param_1->bgId, 1, 0);
    GFL_BGSysFillChar(param_1->bgId, 0, 1, 0);
    GFL_BGSysFillScrArea(param_1->bgId, 0, 0, 0, 0x20, 0x20, 0x11);
    GFL_BGSysLoadScr(param_1->bgId);
    func_ov021_02192fa4(param_1->bgId, param_1->heapId);
    func_ov021_02195588(param_1->talkMsgWin);
}

BmpWin *func_ov021_021930c0(u16 param_1, HeapID param_2, u16 param_3, u16 param_4, u16 param_5, u16 param_6, u16 param_7) {
    BmpWin *window;
    GFLBitmap *bitmap;
    s8 framePalette;

    window = BmpWin_CreateDynamic(param_1, param_3, param_4, param_5, param_6, param_7, 1);
    bitmap = BmpWin_GetBitmap(window);
    GFL_BitmapFill(bitmap, 0xff);
    BmpWin_FlushMap(window);
    BmpWin_FlushChar(window);
    GFL_BGSysLoadScr(param_1);
    framePalette = 0xd;
    if (param_7 != 0xe) {
        framePalette = 0xa;
    }
    BmpWin_DrawFrame(window, 0, 1, framePalette);
    return window;
}




void func_ov021_02193130(BmpWin *param_1) {
    func_0201f7a8(param_1, 0);
    BmpWin_Free(param_1);
}

void func_ov021_02193144(u32 heapId) {
    u32 fileId = GetSysMsgBoxPaletteDatID(0);
    GFL_G2DIOLoadNCLR(5, fileId, 0, 0x20, 0x1a0, 0x20, heapId);
}

void loadPltt(u32 heapId) {
    u32 fileId = GetSysMsgBoxPaletteDatID(0);
    GFL_G2DIOLoadNCLR(5, fileId, 0, 0, 0x140, 0x20, heapId);
}

s32 func_ov021_0219319c(FieldMsgBG *param_1, MsgBGNameWin *param_2) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (param_1->nameWins[i] == NULL) {
            param_1->nameWins[i] = param_2;
            return i;
        }
    }
    return i;
}

MsgBGNameWin *func_ov021_021931c4(FieldMsgBG *param_1, s32 param_2) {
    s32 i;
    MsgBGNameWin *value;

    for (i = 0; i < 8; i++) {
        value = param_1->nameWins[i];
        if (value != NULL) {
            if (param_2 == value->index) {
                value = param_1->nameWins[i];
                param_1->nameWins[i] = NULL;
                return value;
            }
        }
    }
    return 0;
}





void func_ov021_021931f4(MsgBGInput *param_1) {
    MI_CpuFill8(param_1, 0, sizeof(MsgBGInput));
}

void func_ov021_02193200(MsgBGInput *param_1) {
    param_1->timer = 0;
    param_1->held = 0;
}

void func_ov021_02193208(MsgBGInput *param_1, u8 param_2) {
    param_1->autoAdvance = param_2;
}

u8 func_ov021_0219320c(MsgBGInput *param_1) {
    return param_1->autoAdvance;
}

s32 func_ov021_02193210(MsgBGInput *param_1) {
    if (param_1->autoAdvance != 0) {
        return func_02013080(1);
    }
    return func_02012ffc();
}

u32 func_ov021_02193228(MsgBGInput *param_1) {
    u32 result;
    u32 result2;

    result = GCTX_HIDGetPressedKeys();
    result2 = GCTX_HIDGetHeldKeys();
    if (param_1->autoAdvance != 0) {
        return 0;
    }
    if ((result & 3) != 0) {
        param_1->held = 1;
    }
    if ((param_1->held != 0) && ((result2 & 3) != 0)) {
        return 1;
    }
    return 0;
}

BOOL func_ov021_02193260(MsgBGInput *param_1, u32 param_2) {
    u32 keys;
    int max;

    keys = GCTX_HIDGetPressedKeys();
    max = param_2 == 0 ? 8 : 0x32;
    if (param_1->autoAdvance != 0) {
        param_1->timer = param_1->timer + 1;
        if (param_1->timer >= max) {
            param_1->timer = 0;
            return TRUE;
        }
        return FALSE;
    }
    if ((keys & 3) != 0) {
        return TRUE;
    }
    return FALSE;
}


void func_ov021_021932a0(PrintStream *param_1, BOOL param_2) {
    s32 wait;

    if (param_2 == 1) {
        wait = func_02013080(2);
        func_0201ce8c(param_1, wait);
        return;
    }
    func_0201ceb8(param_1);
}
