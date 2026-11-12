// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov298/zukan_detail_headbar.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/zukan_detail.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "system/printsys.h"
#include "text/system/btl_server_flow_title.h"

// The detail screen's head bar, on BG 5 at the top of the sub screen: the title of the page, which slides in and out

#define HEADBAR_BG 5
// How far the bar is hidden above the screen
#define HIDDEN_Y 24

enum {
    HEADBAR_HIDDEN,
    HEADBAR_APPEARING,
    HEADBAR_SHOWN,
    HEADBAR_DISAPPEARING,
};

struct ZukanDetailHeadbar {
    HeapID heapId;
    Font *font;
    int state;
    // The title shown, from ZUKAN_DETAIL_HEADBAR_TITLE_COUNT for none
    int title;
    u8 wait;
    BmpWin *window;
    // The bar's characters with the title printed on them, and where they are in the BG's characters
    GFLBitmap *bitmap;
    u32 charPos;
    u32 charSize;
    MsgData *msgData;
    TCB *vblankTcb;
    PrintQueue *printQueue;
    BOOL transferPending;
};

static void func_ov208_021be864(TCB *tcb, void *data);
static void func_ov208_021be868(ZukanDetailHeadbar *headbar);
static void func_ov208_021be8fc(ZukanDetailHeadbar *headbar);
static void func_ov208_021be920(ZukanDetailHeadbar *headbar);
static void func_ov208_021be9c8(ZukanDetailHeadbar *headbar);
static void func_ov208_021be9cc(ZukanDetailHeadbar *headbar);

// pokebw2's ZukanDetailHeadbar_Create
ZukanDetailHeadbar *func_ov208_021be710(HeapID heapId, Font *font) {
    ZukanDetailHeadbar *headbar =
        GFL_HeapAllocate(heapId, sizeof(ZukanDetailHeadbar), TRUE, "zukan_detail_headbar.c", 120);

    headbar->heapId = heapId;
    headbar->font = font;
    headbar->state = HEADBAR_HIDDEN;
    headbar->title = ZUKAN_DETAIL_HEADBAR_TITLE_COUNT;
    GFL_BGSysMoveBG(HEADBAR_BG, BG_MOVE_SET_Y, HIDDEN_Y);
    GFL_BGSysSetBGPriority(HEADBAR_BG, 0);
    func_ov208_021be868(headbar);
    func_ov208_021be920(headbar);
    headbar->vblankTcb = GFL_VBlankTCBAdd(func_ov208_021be864, headbar, 1);
    headbar->printQueue = func_0201c440(headbar->heapId);
    headbar->transferPending = FALSE;
    return headbar;
}

// pokebw2's ZukanDetailHeadbar_Free
void func_ov208_021be778(ZukanDetailHeadbar *headbar) {
    func_0201c6ec(headbar->printQueue);
    func_0201c4c0(headbar->printQueue);
    GFL_TCBRemove(headbar->vblankTcb);
    func_ov208_021be9c8(headbar);
    func_ov208_021be8fc(headbar);
    GFL_HeapFree(headbar);
}

// pokebw2's ZukanDetailHeadbar_Update
void func_ov208_021be7a4(ZukanDetailHeadbar *headbar) {
    switch (headbar->state) {
    case HEADBAR_HIDDEN:
    case HEADBAR_SHOWN:
        break;
    case HEADBAR_APPEARING:
        if (headbar->wait == 0) {
            if (GFL_BGSysGetBGOffsetY(HEADBAR_BG) <= 0) {
                GFL_BGSysMoveBGReq(HEADBAR_BG, BG_MOVE_SET_Y, 0);
                headbar->state = HEADBAR_SHOWN;
            } else {
                GFL_BGSysMoveBGReq(HEADBAR_BG, BG_MOVE_UP, 3);
                headbar->wait = 0;
            }
        } else {
            headbar->wait--;
        }
        break;
    case HEADBAR_DISAPPEARING:
        if (headbar->wait == 0) {
            if (GFL_BGSysGetBGOffsetY(HEADBAR_BG) >= HIDDEN_Y) {
                GFL_BGSysMoveBGReq(HEADBAR_BG, BG_MOVE_SET_Y, HIDDEN_Y);
                headbar->state = HEADBAR_HIDDEN;
            } else {
                GFL_BGSysMoveBGReq(HEADBAR_BG, BG_MOVE_DOWN, 3);
                headbar->wait = 0;
            }
        } else {
            headbar->wait--;
        }
        break;
    }

    func_ov208_021be9cc(headbar);
    func_0201c4e4(headbar->printQueue);
}

// pokebw2's ZukanDetailHeadbar_SetTitle
void func_ov208_021be82c(ZukanDetailHeadbar *headbar, int title) {
    headbar->title = title;
    func_ov208_021be9c8(headbar);
    func_ov208_021be920(headbar);
}

// pokebw2's ZukanDetailHeadbar_GetState
int func_ov208_021be840(ZukanDetailHeadbar *headbar) {
    return headbar->state;
}

// pokebw2's ZukanDetailHeadbar_Appear
void func_ov208_021be844(ZukanDetailHeadbar *headbar) {
    if (headbar->state != HEADBAR_SHOWN) {
        headbar->state = HEADBAR_APPEARING;
        headbar->wait = 0;
    }
}

// pokebw2's ZukanDetailHeadbar_Disappear
void func_ov208_021be854(ZukanDetailHeadbar *headbar) {
    if (headbar->state != HEADBAR_HIDDEN) {
        headbar->state = HEADBAR_DISAPPEARING;
        headbar->wait = 0;
    }
}

// pokebw2's ZukanDetailHeadbar_VBlank
static void func_ov208_021be864(TCB *tcb, void *data) {
}

// pokebw2's ZukanDetailHeadbar_InitBG
static void func_ov208_021be868(ZukanDetailHeadbar *headbar) {
    headbar->window = BmpWin_CreateDynamic(HEADBAR_BG, 0, 0, 16, 1, 13, FALSE);
    GFL_BitmapFill(BmpWin_GetBitmap(headbar->window), 0);
    BmpWin_FlushChar(headbar->window);
    headbar->bitmap = GFL_G2DIOLoadBitmap(ARCID_ZUKAN_GRA, 14, FALSE, headbar->heapId);
    headbar->charSize = GFL_BitmapCalcPixelDataSize(headbar->bitmap);
    headbar->charPos = GFL_BGSysLoadCharDynamic(HEADBAR_BG, GFL_BitmapGetPixelData(headbar->bitmap), headbar->charSize);
    loadBGScrToVramByNarcNoReserve(ARCID_ZUKAN_GRA, 37, HEADBAR_BG, 0, headbar->charPos, 0xc0, FALSE, headbar->heapId);
    GFL_BGSysSetScrPaletteNo(HEADBAR_BG, 0, 0, 32, 24, 13);
    headbar->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_SERVER_FLOW_TITLE, headbar->heapId);
}

// pokebw2's ZukanDetailHeadbar_FreeBG
static void func_ov208_021be8fc(ZukanDetailHeadbar *headbar) {
    GFL_MsgDataFree(headbar->msgData);
    GFL_BGSysFreeCharMemory(HEADBAR_BG, headbar->charPos, headbar->charSize);
    GFL_BitmapFree(headbar->bitmap);
    BmpWin_Free(headbar->window);
}

// pokebw2's ZukanDetailHeadbar_PrintTitle
static void func_ov208_021be920(ZukanDetailHeadbar *headbar) {
    if (headbar->title >= 0 && headbar->title < ZUKAN_DETAIL_HEADBAR_TITLE_COUNT) {
        ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, headbar->heapId);
        // The palette of each title's bar
        u32 palettes[ZUKAN_DETAIL_HEADBAR_TITLE_COUNT] = { 0, 2, 1, 3, 3 };
        GFLBitmap *bitmap;
        StrBuf *strbuf;

        GFL_G2DIOLoadArcNCLR(arc, 4, PALTYPE_SUB_BG, palettes[headbar->title] * 32, 13 * 32, 32, headbar->heapId);
        GFL_ArcToolFree(arc);
        bitmap = GFL_G2DIOLoadBitmap(ARCID_ZUKAN_GRA, 14, FALSE, headbar->heapId);
        GFL_BitmapCopy(bitmap, headbar->bitmap);
        GFL_BitmapFree(bitmap);
        strbuf = GFL_MsgDataLoadStrbufNew(headbar->msgData, BtlServerFlowTitle_Text_178 + headbar->title);
        func_0201c724(headbar->printQueue, headbar->bitmap, 24, 5, strbuf, headbar->font, PRINT_COLOR(15, 14, 0));
        GFL_StrBufFree(strbuf);
        headbar->transferPending = TRUE;
        func_ov208_021be9cc(headbar);
    }
}

// pokebw2's ZukanDetailHeadbar_ClearTitle
static void func_ov208_021be9c8(ZukanDetailHeadbar *headbar) {
}

// Loads the bar's characters once the title has been printed on them
// pokebw2's ZukanDetailHeadbar_TransferTitle
static void func_ov208_021be9cc(ZukanDetailHeadbar *headbar) {
    if (headbar->transferPending && !func_0201c6c4(headbar->printQueue, headbar->bitmap)) {
        GFL_BGSysLoadChar(HEADBAR_BG, GFL_BitmapGetPixelData(headbar->bitmap), headbar->charSize, headbar->charPos);
        GFL_BGSysQueueScrLoad(HEADBAR_BG);
        headbar->transferPending = FALSE;
    }
}
