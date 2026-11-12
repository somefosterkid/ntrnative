// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov185/pmsiv_category.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/pmsiv_category.h"
#include "types.h"
#include "app/pms_input.h"
#include "app/pms_input_data.h"
#include "app/pms_input_view.h"
#include "app/pmsi_initial_data.h"
#include "app/pmsiv_menu.h"
#include "app/pmsiv_tool.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "system/gf_font.h"
#include "system/printsys.h"

// The phrase input's categories: the buttons of the word groups, three to a row, or of the initials, on BG1, the
// letters the player types for the search and the words it finds on BG6, and the cursor over them. The names are
// ours, guessed

// The message file of the groups' names
#define PMSIV_CATEGORY_MSG_FILE TEXT_BANK_PMS_CATEGORIES
// The name shown for a group with no word unlocked
#define PMSIV_CATEGORY_MSG_EMPTY 13

// The cursor's animations
#define CURSOR_ANIM_GROUP 4
#define CURSOR_ANIM_INITIAL 6
#define CURSOR_ANIM_BUTTON 16
#define CURSOR_ANIM_INITIAL_DECIDE 30
#define CURSOR_ANIM_GROUP_DECIDE 31

// How many results the list shows
#define PMSIV_CATEGORY_RESULT_MAX 22

struct PMSIVCategory {
    PMSInputView *vwk;
    const PMSInputWork *mwk;
    const PMSInputData *dwk;
    u32 unkC;
    int seq;
    u32 unk14;
    PMSIVToolBlendWork blend;
    PMSIVToolScrollWork scroll;
    ClActor *cursor;
    // Whether the cursor was shown before its animation of a category chosen
    BOOL cursorVisible;
    int *keyMode;
    BmpWin *groupWin[PMSI_CATEGORY_COUNT];
    BmpWin *initialWin;
    BmpWin *searchWin;
    BmpWin *resultWin;
    PrintWindow resultPrint;
};

static u32 func_ov110_021bb3f4(PMSIVCategory *wk, u32 charPos);
static u32 func_ov110_021bb4f4(PMSIVCategory *wk, u32 charPos);
static u32 func_ov110_021bb5a0(PMSIVCategory *wk, u32 charPos);
static void func_ov110_021bb5e0(PMSIVCategory *wk);
static void func_ov110_021bb61c(PMSIVCategory *wk);
static void func_ov110_021bb9e8(PMSIVCategory *wk);
void func_ov110_021bba10(PMSIVCategory *wk, u32 count);

const u32 PMSIV_CATEGORY_UNK_7198[3] = { 0x1f, 0x3f, 0x3f };

// The message of each group's name
// pokebw2's sPMSIVCategoryGroupMsgs
static const u8 data_ov110_021c38c4[PMSI_CATEGORY_COUNT] = { 0, 1, 2, 3, 11, 5, 12, 6, 7, 8, 10, 9 };

// pokebw2's PMSIVCategory_Create
PMSIVCategory *func_ov110_021bb290(PMSInputView *vwk, const PMSInputWork *mwk, const PMSInputData *dwk) {
    PMSIVCategory *wk = GFL_HeapAllocate(HEAPID_PMS_INPUT, sizeof(PMSIVCategory), FALSE, "pmsiv_category.c", 220);

    wk->vwk = vwk;
    wk->mwk = mwk;
    wk->dwk = dwk;
    wk->unkC = 0;
    wk->unk14 = 0;
    wk->cursor = NULL;
    wk->cursorVisible = FALSE;
    wk->keyMode = func_ov110_021bf6d4(mwk);
    return wk;
}

// pokebw2's PMSIVCategory_Delete
void func_ov110_021bb2cc(PMSIVCategory *wk) {
    u8 i;

    if (wk->cursor) {
        func_0204b3b4(wk->cursor);
    }
    for (i = 0; i < PMSI_CATEGORY_COUNT; i++) {
        if (wk->groupWin[i]) {
            BmpWin_Free(wk->groupWin[i]);
        }
    }
    if (wk->initialWin) {
        BmpWin_Free(wk->initialWin);
    }
    if (wk->searchWin) {
        BmpWin_Free(wk->searchWin);
    }
    if (wk->resultWin) {
        BmpWin_Free(wk->resultWin);
    }
    GFL_HeapFree(wk);
}

// pokebw2's PMSIVCategory_SetupGraphicDatas
void func_ov110_021bb328(PMSIVCategory *wk, ArcTool *arc) {
    u32 charPos;

    loadBGScrToVramByFileNoReserveNegAlign(arc, 27, 1, 0, 0, FALSE, HEAPID_PMS_INPUT);
    charPos = GFL_BGSysLoadArcNCGRStatic(arc, 15, 1, 0, 0, FALSE, HEAPID_PMS_INPUT);
    charPos = func_ov110_021bb3f4(wk, charPos / 0x20);
    charPos = func_ov110_021bb4f4(wk, charPos);
    charPos = func_ov110_021bb5a0(wk, charPos);
    func_ov110_021bb5e0(wk);
    GFL_BGSysMoveBG(1, BG_MOVE_SET_X, -4);
    GFL_BGSysMoveBG(1, BG_MOVE_SET_Y, 16);
    func_ov110_021bb61c(wk);
    G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG1, -10);
    G2_SetWnd0InsidePlane(GX_PLANEMASK_ALL, TRUE);
    G2_SetWndOutsidePlane(GX_PLANEMASK_ALL & ~GX_PLANEMASK_BG1, TRUE);
    G2_SetWnd0Position(0, 0, 255, 168);
    GX_SetVisibleWnd(GX_WNDMASK_W0);
    GFL_BGSysLoadScr(1);
}

// pokebw2's PMSIVCategory_SetupGroupWindow
static u32 func_ov110_021bb3f4(PMSIVCategory *wk, u32 charPos) {
    int i;
    int x, y;
    MsgData *msgData;
    Font *font;

    font = func_ov110_021bac44(wk->vwk);
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, PMSIV_CATEGORY_MSG_FILE, HEAPID_PMS_INPUT);
    x = 1;
    y = 9;

    for (i = 0; i < PMSI_CATEGORY_COUNT; i++) {
        BmpWin *win;
        StrBuf *str;
        u32 width;

        if (i != 0 && i % 3 == 0) {
            x = 1;
            y += 3;
        }
        if (func_ov110_021b999c(wk->dwk, i)) {
            str = GFL_MsgDataLoadStrbufNew(msgData, data_ov110_021c38c4[i]);
            GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
        } else {
            str = GFL_MsgDataLoadStrbufNew(msgData, PMSIV_CATEGORY_MSG_EMPTY);
            GFL_TextRndUpdateColorIndexLUT(3, 4, 15);
        }
        win = BmpWin_CreateDynamic(1, x, y, 9, 2, 12, TRUE);
        GFL_BitmapFill(BmpWin_GetBitmap(win), 15);
        width = 72 - GFL_FontGetBlockWidth(str, font, 0);
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(win), width / 2, 0, str, font);
        BmpWin_FlushMap(win);
        BmpWin_FlushChar(win);
        GFL_StrBufFree(str);
        wk->groupWin[i] = win;
        charPos += 9 * 2;
        x += 10;
    }
    GFL_MsgDataFree(msgData);
    return charPos;
}

// pokebw2's PMSIVCategory_SetupInitialWindow
static u32 func_ov110_021bb4f4(PMSIVCategory *wk, u32 charPos) {
    u32 count;
    Font *font = func_ov110_021bac44(wk->vwk);
    BmpWin *win = BmpWin_CreateDynamic(1, 1, 8, 30, 14, 12, TRUE);
    StrBuf *str = GFL_StrBufCreate(4, HEAPID_PMS_INPUT);
    u32 i;
    u32 x, y;

    GFL_BitmapFill(BmpWin_GetBitmap(win), 15);
    count = func_ov110_021bb1f4();
    for (i = 0; i < count; i++) {
        func_ov110_021bb1f8(i, str);
        func_ov110_021bb224(i, &x, &y);
        GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(win), x + 5, y, str, font);
    }
    BmpWin_FlushChar(win);
    GFL_StrBufFree(str);
    wk->initialWin = win;
    return charPos + 30 * 14;
}

// pokebw2's PMSIVCategory_SetupSearchWindow
static u32 func_ov110_021bb5a0(PMSIVCategory *wk, u32 charPos) {
    BmpWin *win = BmpWin_CreateDynamic(1, 10, 3, 11, 2, 12, TRUE);

    GFL_BitmapFill(BmpWin_GetBitmap(win), 1);
    BmpWin_FlushChar(win);
    wk->searchWin = win;
    return charPos + 11 * 2;
}

// pokebw2's PMSIVCategory_SetupResultWindow
static void func_ov110_021bb5e0(PMSIVCategory *wk) {
    BmpWin *win = BmpWin_CreateDynamic(6, 1, 1, 30, 22, 0, TRUE);

    BmpWin_FlushMap(win);
    GFL_BGSysLoadScr(6);
    GFL_BGSysMoveBG(6, BG_MOVE_SET_Y, -192);
    wk->resultWin = win;
}

// pokebw2's PMSIVCategory_SetupActor
static void func_ov110_021bb61c(PMSIVCategory *wk) {
    PMSIVObjRes res;

    func_ov110_021bac64(wk->vwk, &res, 0, 0);
    wk->cursor = func_ov110_021bac78(wk->vwk, &res, 48, 64, 3, NNS_G2D_VRAM_TYPE_2DMAIN);
    func_0204b878(wk->cursor, CURSOR_ANIM_GROUP);
    func_0204b3dc(wk->cursor, FALSE);
}

// pokebw2's PMSIVCategory_VisibleCursor
void func_ov110_021bb658(PMSIVCategory *wk, BOOL visible) {
    if (visible) {
        if (*wk->keyMode == 0) {
            func_0204b3dc(wk->cursor, TRUE);
            if (func_ov110_021bf6dc(wk->mwk) == 0) {
                func_0204b878(wk->cursor, CURSOR_ANIM_GROUP);
            } else {
                func_0204b878(wk->cursor, CURSOR_ANIM_INITIAL);
            }
        } else {
            func_0204b3dc(wk->cursor, FALSE);
        }
    } else {
        func_0204b3dc(wk->cursor, FALSE);
    }
}

// pokebw2's PMSIVCategory_MoveCursor
void func_ov110_021bb6a0(PMSIVCategory *wk, u32 pos) {
    ClActorPos actPos = data_ov110_021c38d0;
    u32 anim;

    if (func_ov110_021bf6dc(wk->mwk) == 0 && pos != PMSIV_CATEGORY_POS_BACK) {
        actPos.x = (pos % 3) * 80 + 48;
        actPos.y = (pos / 3) * 24 + 64;
        anim = CURSOR_ANIM_GROUP;
    } else {
        PMSIVMenu *menu = func_ov110_021bac3c(wk->vwk);

        anim = CURSOR_ANIM_BUTTON;
        if (pos == PMSIV_CATEGORY_POS_BUTTON_0) {
            if (*wk->keyMode == 0) {
                func_ov110_021bcf7c(menu, 0, TRUE);
            } else {
                func_ov110_021bcf7c(menu, 0, FALSE);
            }
        } else if (pos == PMSIV_CATEGORY_POS_BUTTON_1) {
            if (*wk->keyMode == 0) {
                func_ov110_021bcf7c(menu, 1, TRUE);
            } else {
                func_ov110_021bcf7c(menu, 0, FALSE);
            }
        } else if (pos == PMSIV_CATEGORY_POS_BACK) {
            if (*wk->keyMode == 0) {
                func_ov110_021bcf7c(menu, 2, TRUE);
            } else {
                func_ov110_021bcf7c(menu, 0, FALSE);
            }
        } else {
            u32 x, y;

            func_ov110_021bcf7c(menu, 0, FALSE);
            func_ov110_021bb224(pos, &x, &y);
            actPos.x = x + 18;
            actPos.y = y + 56;
            anim = CURSOR_ANIM_INITIAL;
        }
    }
    func_0204b404(wk->cursor, &actPos, CLACT_SURFACE_MAIN);
    func_0204b878(wk->cursor, anim);
}

// pokebw2's PMSIVCategory_StartEnableBG
void func_ov110_021bb76c(PMSIVCategory *wk) {
    wk->seq = 0;
    func_ov110_021bc8fc(&wk->blend, GX_BLEND_PLANEMASK_BG1, -10, 0, 16);
}

// pokebw2's PMSIVCategory_WaitEnableBG
BOOL func_ov110_021bb784(PMSIVCategory *wk) {
    if (wk->seq == 0 && func_ov110_021bc93c(&wk->blend)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's PMSIVCategory_StartDisableBG
void func_ov110_021bb7a0(PMSIVCategory *wk) {
    wk->seq = 0;
    func_ov110_021bc78c(&wk->scroll, 1, PMSIV_TOOL_SCROLL_Y, 0, 6);
}

// pokebw2's PMSIVCategory_WaitDisableBG
BOOL func_ov110_021bb7b8(PMSIVCategory *wk) {
    switch (wk->seq) {
    case 0:
        if (func_ov110_021bc7dc(&wk->scroll)) {
            func_ov110_021bc8fc(&wk->blend, GX_BLEND_PLANEMASK_BG1, 0, -10, 16);
            wk->seq++;
        }
        break;
    case 1:
        return func_ov110_021bc93c(&wk->blend);
    }
    return FALSE;
}

// pokebw2's PMSIVCategory_SetDisableBG
void func_ov110_021bb804(PMSIVCategory *wk) {
    GFL_BGSysMoveBG(1, BG_MOVE_SET_Y, 16);
}

// pokebw2's PMSIVCategory_StartBrightDown
void func_ov110_021bb814(PMSIVCategory *wk) {
    func_ov110_021bc8fc(&wk->blend, GX_BLEND_PLANEMASK_BG1, 0, -10, 16);
}

// pokebw2's PMSIVCategory_WaitBrightDown
BOOL func_ov110_021bb82c(PMSIVCategory *wk) {
    return func_ov110_021bc93c(&wk->blend);
}

// pokebw2's PMSIVCategory_StartFadeOut
void func_ov110_021bb838(PMSIVCategory *wk) {
    func_ov110_021bc868(&wk->blend, GX_BLEND_PLANEMASK_BG1, 0x3f, PMSIV_BLEND_MAX, 0, 12);
}

// pokebw2's PMSIVCategory_WaitFadeOut
BOOL func_ov110_021bb854(PMSIVCategory *wk) {
    if (func_ov110_021bc898(&wk->blend)) {
        GFL_BGSysSetBGEnabled(1, FALSE);
        return TRUE;
    }
    return FALSE;
}

// pokebw2's PMSIVCategory_StartFadeIn
void func_ov110_021bb870(PMSIVCategory *wk) {
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, 0x3f, 0, 16);
    GFL_BGSysSetBGEnabled(1, TRUE);
    func_ov110_021bc868(&wk->blend, GX_BLEND_PLANEMASK_BG1, 0x3f, 0, PMSIV_BLEND_MAX, 12);
}

// pokebw2's PMSIVCategory_WaitFadeIn
BOOL func_ov110_021bb8ac(PMSIVCategory *wk) {
    return func_ov110_021bc898(&wk->blend);
}

// pokebw2's PMSIVCategory_ChangeModeBG
void func_ov110_021bb8b8(PMSIVCategory *wk) {
    int x;

    if (func_ov110_021bf6dc(wk->mwk) == 0) {
        x = -4;
    } else {
        x = 0;
    }
    GFL_BGSysMoveBGReq(1, BG_MOVE_SET_X, x);
}

// pokebw2's PMSIVCategory_ChangeModeScreen
void func_ov110_021bb8d8(PMSIVCategory *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0x2b, HEAPID_PMS_INPUT);

    if (func_ov110_021bf6dc(wk->mwk) == 0) {
        int i;

        GFL_G2DIOLoadNSCRAsync(arc, 27, 1, 0, 0, 0, FALSE, HEAPID_PMS_INPUT);
        for (i = 0; i < PMSI_CATEGORY_COUNT; i++) {
            BmpWin_FlushMap(wk->groupWin[i]);
        }
    } else {
        GFL_G2DIOLoadNSCRAsync(arc, 26, 1, 0, 0, 0, FALSE, HEAPID_PMS_INPUT);
        BmpWin_FlushMap(wk->initialWin);
        BmpWin_FlushMap(wk->searchWin);
    }
    GFL_ArcToolFree(arc);
    GFL_BGSysQueueScrLoad(1);
}

// pokebw2's PMSIVCategory_WaitModeChange
BOOL func_ov110_021bb954(PMSIVCategory *wk) {
    return TRUE;
}

// pokebw2's PMSIVCategory_PrintSearchInput
void func_ov110_021bb958(PMSIVCategory *wk) {
    Font *font = func_ov110_021bac44(wk->vwk);
    BmpWin *win = wk->searchWin;
    StrBuf *str = GFL_StrBufCreate(14, HEAPID_PMS_INPUT);

    func_ov110_021bf7d4(wk->mwk, str);
    GFL_BitmapFill(BmpWin_GetBitmap(win), 1);
    GFL_TextRndUpdateColorIndexLUT(15, 14, 1);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(win), 0, 0, str, font);
    GFL_StrBufFree(str);
    BmpWin_FlushMap(win);
    BmpWin_FlushChar(win);
    GFL_BGSysQueueScrLoad(1);
}

// pokebw2's PMSIVCategory_StartResultList
void func_ov110_021bb9c0(PMSIVCategory *wk, BOOL show) {
    u32 count = func_ov110_021bf7fc(wk->mwk);

    if (count > PMSIV_CATEGORY_RESULT_MAX) {
        count = PMSIV_CATEGORY_RESULT_MAX;
    }
    if (show) {
        func_ov110_021bba10(wk, count);
    } else {
        func_ov110_021bb9e8(wk);
    }
}

// pokebw2's PMSIVCategory_HideResultList
static void func_ov110_021bb9e8(PMSIVCategory *wk) {
    int y = -192 - GFL_BGSysGetBGOffsetY(6);

    func_ov110_021bc78c(&wk->scroll, 6, PMSIV_TOOL_SCROLL_Y, y, 8);
}

void func_ov110_021bba10(PMSIVCategory *wk, u32 count) {
    PrintQueue *queue = func_ov110_021bac4c(wk->vwk);
    Font *font = func_ov110_021bac44(wk->vwk);
    StrBuf *str = GFL_StrBufCreate(15, HEAPID_PMS_INPUT);
    int i;
    int height, offset;

    GFL_BitmapFill(BmpWin_GetBitmap(wk->resultWin), 15);
    wk->resultPrint.window = wk->resultWin;
    wk->resultPrint.flushPending = 0;
    for (i = 0; i < count; i++) {
        int x;

        if (i % 2 == 0) {
            x = 8;
        } else {
            x = 128;
        }
        func_ov110_021bf814(wk->mwk, i, str);
        func_0201c724(queue, BmpWin_GetBitmap(wk->resultPrint.window), x, (i / 2) * 16, str, font, 0x44f);
        wk->resultPrint.flushPending = 1;
    }
    GFL_StrBufFree(str);
    offset = GFL_BGSysGetBGOffsetY(6) + 192;
    height = ((count + 1) / 2) * 16 + 8;
    if (height > 192) {
        height = 192;
    } else if (height < 48) {
        height = 48;
    }
    func_ov110_021bc78c(&wk->scroll, 6, PMSIV_TOOL_SCROLL_Y, height - offset, 8);
    BmpWin_FlushMap(wk->resultWin);
    GFL_BGSysQueueScrLoad(6);
    GFL_BGSysSetBGEnabled(6, TRUE);
}


// pokebw2's PMSIVCategory_WaitResultList
BOOL func_ov110_021bbb0c(PMSIVCategory *wk, BOOL show) {
    PrintQueue *queue = func_ov110_021bac4c(wk->vwk);
    BOOL printed = TRUE;
    BOOL scrolled;

    if (show) {
        PrintWindow_Flush(&wk->resultPrint, queue);
        printed = PrintWindow_IsPrinted(&wk->resultPrint);
    }
    scrolled = func_ov110_021bc7dc(&wk->scroll);
    if (printed && scrolled) {
        if (!show) {
            GFL_BGSysSetBGEnabled(6, FALSE);
        }
        return TRUE;
    }
    return FALSE;
}

// pokebw2's PMSIVCategory_StartCursorDecide
void func_ov110_021bbb84(PMSIVCategory *wk, u32 pos) {
    ClActorPos actPos;
    u32 anim;

    wk->cursorVisible = func_0204b3f4(wk->cursor);
    func_0204b3dc(wk->cursor, TRUE);
    if (func_ov110_021bf6dc(wk->mwk) == 0 && pos != PMSIV_CATEGORY_POS_BACK) {
        actPos.x = (pos % 3) * 80 + 48;
        actPos.y = (pos / 3) * 24 + 64;
        anim = CURSOR_ANIM_GROUP_DECIDE;
    } else {
        anim = CURSOR_ANIM_BUTTON;
        if (pos != PMSIV_CATEGORY_POS_BUTTON_0 && pos != PMSIV_CATEGORY_POS_BUTTON_1 &&
            pos != PMSIV_CATEGORY_POS_BACK) {
            u32 x, y;

            func_ov110_021bb224(pos, &x, &y);
            actPos.x = x + 18;
            actPos.y = y + 56;
            anim = CURSOR_ANIM_INITIAL_DECIDE;
        }
    }
    func_0204b404(wk->cursor, &actPos, CLACT_SURFACE_MAIN);
    func_0204b878(wk->cursor, anim);
}

// pokebw2's PMSIVCategory_WaitCursorDecide
BOOL func_ov110_021bbc10(PMSIVCategory *wk) {
    u16 anim = func_0204b898(wk->cursor);

    if (anim != CURSOR_ANIM_GROUP_DECIDE && anim != CURSOR_ANIM_INITIAL_DECIDE) {
        return TRUE;
    }
    if (!func_0204b98c(wk->cursor)) {
        func_0204b3dc(wk->cursor, wk->cursorVisible);
        func_0204b878(wk->cursor, func_ov110_021bf6dc(wk->mwk) == 0 ? CURSOR_ANIM_GROUP : CURSOR_ANIM_INITIAL);
        wk->cursorVisible = FALSE;
        return TRUE;
    }
    return FALSE;
}
