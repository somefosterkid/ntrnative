// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov185/pmsiv_wordwin.c, which is the same code
// in Black 2/White 2's overlay 185. The names are Black's dsd names; pokebw2's guesses are in the header.
#include "app/pmsiv_wordwin.h"
#include "types.h"
#include "app/pms_input.h"
#include "app/pms_input_view.h"
#include "app/pmsiv_tool.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/str.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nnsys/g2d.h"
#include "system/gf_font.h"
#include "system/printsys.h"

// The phrase input's word window (pmsiv_wordwin.c, named by the ROM's embedded string): a category's words two to a
// row on BG2, drawn into a window that scrolls around as the list scrolls, with a cursor, a scroll bar and the number
// words as actors

// How many number words there are, shown as actors
#define WORDWIN_NUMBER_COUNT 10
// How many words the window draws at a time, and its rows' height
#define WORDWIN_DRAW_WORDS 16
#define WORDWIN_LINE_HEIGHT 24

// The group of the number words
#define GROUP_NUMBERS 11

// The scroll bar's range
#define SCROLL_BAR_TOP 18
#define SCROLL_BAR_BOTTOM 149
#define SCROLL_BAR_HEIGHT 132

// The cursor's animations
#define CURSOR_ANIM 8
#define CURSOR_ANIM_DECIDE 28

struct PMSIVWordWin {
    PMSInputView *vwk;
    const PMSInputWork *mwk;
    const PMSInputData *dwk;
    BmpWin *win;
    // A word that crosses the bottom of the window is drawn here first
    BmpWin *tmpWin;
    ClActor *cursor;
    BOOL cursorVisible;
    ClActor *scrollBar;
    ClActor *numberAct[WORDWIN_NUMBER_COUNT];
    StrBuf *str;
    PMSIVToolBlendWork blend;
    PMSIVToolScrollWork scroll;
    int seq;
    GXWndPlane savedOutside;
    int savedWnd;
    // Where the next words go in the window, and the first word drawn
    u32 y;
    u32 top;
    int *keyMode;
};

// pokebw2's PMSIVWordWin_SetupActors, PMSIVWordWin_ClearScrollArea, PMSIVWordWin_ResetScroll and
// PMSIVWordWin_PrintWord
static void func_ov110_021bf94c(PMSIVWordWin *wk);
static void func_ov110_021bfed0(PMSIVWordWin *wk, int vector);
static void func_ov110_021bff60(PMSIVWordWin *wk);
static void func_ov110_021bff7c(PMSIVWordWin *wk, u32 index, u32 y);

PMSIVWordWin *func_ov110_021bf844(PMSInputView *vwk, const PMSInputWork *mwk, const PMSInputData *dwk) {
    PMSIVWordWin *wk = GFL_HeapAllocate(HEAPID_PMS_INPUT, sizeof(PMSIVWordWin), FALSE, "pmsiv_wordwin.c", 154);

    wk->vwk = vwk;
    wk->mwk = mwk;
    wk->dwk = dwk;
    wk->str = GFL_StrBufCreate(32, HEAPID_PMS_INPUT);
    wk->win = BmpWin_CreateDynamic(2, 3, 0, 26, 32, 13, TRUE);
    wk->tmpWin = BmpWin_CreateDynamic(2, 0, 0, 12, 4, 13, TRUE);
    wk->cursor = NULL;
    wk->cursorVisible = FALSE;
    wk->keyMode = func_ov110_021bf6d4(wk->mwk);
    return wk;
}

void func_ov110_021bf8bc(PMSIVWordWin *wk) {
    if (wk->cursor) {
        func_0204b3b4(wk->cursor);
    }
    if (wk->scrollBar) {
        func_0204b3b4(wk->scrollBar);
    }
    if (wk->str) {
        GFL_StrBufFree(wk->str);
    }
    BmpWin_Free(wk->tmpWin);
    BmpWin_Free(wk->win);
    GFL_HeapFree(wk);
}

void func_ov110_021bf8f4(PMSIVWordWin *wk) {
    GFL_BGSysClearCharCore(2, 0x20, 0, HEAPID_PMS_INPUT);
    GFL_BGSysFillScrArea(2, 0, 0, 0, 32, 32, 13);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->win), 0);
    BmpWin_FlushMap(wk->win);
    BmpWin_FlushChar(wk->win);
    GFL_BGSysLoadScr(2);
    func_ov110_021bf94c(wk);
    GFL_BGSysSetBGEnabled(2, FALSE);
}

static void func_ov110_021bf94c(PMSIVWordWin *wk) {
    PMSIVObjRes res;
    PMSIVObjRes numberRes;
    int i;

    func_ov110_021bac64(wk->vwk, &res, 0, 0);
    wk->cursor = func_ov110_021bac78(wk->vwk, &res, 62, 24, 4, NNS_G2D_VRAM_TYPE_2DMAIN);
    func_0204b878(wk->cursor, CURSOR_ANIM);
    func_0204b3dc(wk->cursor, FALSE);
    wk->scrollBar = func_ov110_021bac78(wk->vwk, &res, 244, SCROLL_BAR_TOP, 5, NNS_G2D_VRAM_TYPE_2DMAIN);
    func_0204b878(wk->scrollBar, 18);
    func_0204b3dc(wk->scrollBar, FALSE);
    for (i = 0; i < WORDWIN_NUMBER_COUNT; i++) {
        func_ov110_021bac50(wk->vwk, &numberRes, 0);
        wk->numberAct[i] = func_ov110_021bac78(wk->vwk, &numberRes, 0, 0, 0, NNS_G2D_VRAM_TYPE_2DMAIN);
        func_0204b878(wk->numberAct[i], i);
        func_0204b3dc(wk->numberAct[i], FALSE);
        func_0204b84c(wk->numberAct[i], 1);
    }
}

void func_ov110_021bf9f0(PMSIVWordWin *wk) {
    u32 count, y, i;

    GFL_BitmapFill(BmpWin_GetBitmap(wk->win), 0);
    func_ov110_021bff60(wk);
    count = func_ov110_021bf734(wk->mwk);
    if (count > WORDWIN_DRAW_WORDS) {
        count = WORDWIN_DRAW_WORDS;
    }
    y = wk->y;
    for (i = 0; i < count; i++) {
        func_ov110_021bff7c(wk, i, y);
        if (i & 1) {
            y += WORDWIN_LINE_HEIGHT;
        }
    }
    BmpWin_FlushChar(wk->win);
}

void func_ov110_021bfa40(PMSIVWordWin *wk, u32 top) {
    u32 count, y, i;

    GFL_BitmapFill(BmpWin_GetBitmap(wk->win), 0);
    func_ov110_021bff60(wk);
    wk->top = top * 2;
    count = func_ov110_021bf734(wk->mwk);
    if (count > WORDWIN_DRAW_WORDS) {
        count = WORDWIN_DRAW_WORDS;
    }
    y = wk->y;
    for (i = 0; i < count; i++) {
        func_ov110_021bff7c(wk, wk->top + i, y);
        if (i & 1) {
            y += WORDWIN_LINE_HEIGHT;
        }
    }
    BmpWin_FlushChar(wk->win);
}

void func_ov110_021bfaa0(PMSIVWordWin *wk) {
    func_ov110_021bad2c(wk->vwk, TRUE);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, 0x3f, 0, 16);
    GFL_BGSysSetBGEnabled(2, TRUE);
    G2_SetWnd1InsidePlane(GX_PLANEMASK_ALL, TRUE);
    wk->savedOutside = G2_GetWndOutsidePlane();
    wk->savedWnd = GX_GetVisibleWnd();
    G2_SetWndOutsidePlane(GX_PLANEMASK_ALL & ~GX_PLANEMASK_BG2, TRUE);
    G2_SetWnd1Position(0, 0, 255, 168);
    GX_SetVisibleWnd(GX_WNDMASK_W1);
    wk->seq = 0;
    func_ov110_021bc868(&wk->blend, GX_BLEND_PLANEMASK_BG2, 0x3f, 0, PMSIV_BLEND_MAX, 12);
}

BOOL func_ov110_021bfb58(PMSIVWordWin *wk) {
    if (wk->seq == 0) {
        if (func_ov110_021bc898(&wk->blend)) {
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

void func_ov110_021bfb7c(PMSIVWordWin *wk) {
    wk->seq = 0;
    func_ov110_021bc868(&wk->blend, GX_BLEND_PLANEMASK_BG2, 0x3f, PMSIV_BLEND_MAX, 0, 12);
}

BOOL func_ov110_021bfba0(PMSIVWordWin *wk) {
    int i;

    if (wk->seq == 0) {
        if (func_ov110_021bc898(&wk->blend)) {
            for (i = 0; i < WORDWIN_NUMBER_COUNT; i++) {
                func_0204b3dc(wk->numberAct[i], FALSE);
            }
            GFL_BGSysSetBGEnabled(2, FALSE);
            G2_SetWndOutsidePlane(wk->savedOutside.planeMask, wk->savedOutside.effect);
            GX_SetVisibleWnd(wk->savedWnd);
            return TRUE;
        }
    } else {
        return TRUE;
    }
    return FALSE;
}

void func_ov110_021bfc20(PMSIVWordWin *wk, BOOL visible) {
    if (visible) {
        if (*wk->keyMode == 0) {
            func_0204b3dc(wk->cursor, TRUE);
        } else {
            func_0204b3dc(wk->cursor, FALSE);
        }
        func_0204b3dc(wk->scrollBar, func_ov110_021bf734(wk->mwk) > 14 ? TRUE : FALSE);
    } else {
        func_0204b3dc(wk->cursor, visible);
        func_0204b3dc(wk->scrollBar, FALSE);
    }
}

void func_ov110_021bfc64(PMSIVWordWin *wk, u32 pos) {
    ClActorPos actPos;

    if (pos != -1) {
        actPos.x = (pos & 1) * 112 + 62;
        actPos.y = (pos / 2 + 1) * WORDWIN_LINE_HEIGHT;
        func_0204b404(wk->cursor, &actPos, CLACT_SURFACE_MAIN);
        func_0204b878(wk->cursor, CURSOR_ANIM);
    }
}

void func_ov110_021bfca4(PMSIVWordWin *wk, int vector) {
    int count, i;
    u8 y;
    u8 newY;
    u32 newTop;
    u32 start;
    int dy;

    func_ov110_021bfed0(wk, vector);
    count = vector * 2;
    start = wk->top;
    newTop = start + count;
    dy = vector * WORDWIN_LINE_HEIGHT;
    newY = wk->y + dy;
    if (vector < 0) {
        start = newTop;
        y = newY;
        count = -count;
        vector = -vector;
    } else {
        y = wk->y + 192;
        start += WORDWIN_DRAW_WORDS;
        if (count + start > func_ov110_021bf734(wk->mwk)) {
            count--;
        }
    }
    for (i = 0; i < count; i++) {
        func_ov110_021bff7c(wk, start + i, y);
        if (i & 1) {
            y += WORDWIN_LINE_HEIGHT;
        }
    }
    wk->y = newY;
    wk->top = newTop;
    BmpWin_FlushChar(wk->win);
    BmpWin_FlushMap(wk->win);
    func_ov110_021bc78c(&wk->scroll, 2, PMSIV_TOOL_SCROLL_Y, dy, vector);
}

BOOL func_ov110_021bfd60(PMSIVWordWin *wk) {
    u16 top, scrollMax;

    if (func_ov110_021bc7dc(&wk->scroll)) {
        func_ov110_021bf7ec(wk->mwk, &top, &scrollMax);
        func_ov110_021bfdb0(wk, top, scrollMax);
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov110_021bfd94(PMSIVWordWin *wk, ClActorPos *pos) {
    BOOL visible = func_0204b3f4(wk->scrollBar);

    func_0204b454(wk->scrollBar, pos, CLACT_SURFACE_MAIN);
    return visible;
}

void func_ov110_021bfdb0(PMSIVWordWin *wk, u32 top, u32 scrollMax) {
    ClActorPos pos;

    pos.x = 244;
    if (top == 0) {
        pos.y = SCROLL_BAR_TOP;
    } else if (top == scrollMax) {
        pos.y = SCROLL_BAR_BOTTOM;
    } else if (scrollMax != 0) {
        s16 start = top * SCROLL_BAR_HEIGHT / (scrollMax + 1) + SCROLL_BAR_TOP;
        s16 end = (top + 1) * SCROLL_BAR_HEIGHT / (scrollMax + 1) + SCROLL_BAR_TOP - 1;

        pos.y = (start + end) / 2;
        if (pos.y < SCROLL_BAR_TOP) {
            pos.y = SCROLL_BAR_TOP;
        } else if (pos.y > SCROLL_BAR_BOTTOM) {
            pos.y = SCROLL_BAR_BOTTOM;
        }
    } else {
        pos.y = SCROLL_BAR_TOP;
    }
    func_0204b404(wk->scrollBar, &pos, CLACT_SURFACE_MAIN);
}

void func_ov110_021bfe24(PMSIVWordWin *wk, u32 y) {
    ClActorPos pos;

    func_0204b454(wk->scrollBar, &pos, CLACT_SURFACE_MAIN);
    pos.y = y;
    if (pos.y < SCROLL_BAR_TOP) {
        pos.y = SCROLL_BAR_TOP;
    }
    if (pos.y > SCROLL_BAR_BOTTOM) {
        pos.y = SCROLL_BAR_BOTTOM;
    }
    func_0204b404(wk->scrollBar, &pos, CLACT_SURFACE_MAIN);
}

u32 func_ov110_021bfe60(PMSIVWordWin *wk, u32 scrollMax) {
    ClActorPos pos;
    u32 line;
    s16 y, i;

    func_ov110_021bfd94(wk, &pos);
    if (scrollMax != 0) {
        y = pos.y;
        if (y == SCROLL_BAR_TOP) {
            line = 0;
        } else if (y == SCROLL_BAR_BOTTOM) {
            line = scrollMax;
        } else {
            for (i = 0; i <= scrollMax; i++) {
                s16 start = i * SCROLL_BAR_HEIGHT / (scrollMax + 1) + SCROLL_BAR_TOP;
                s16 end = (i + 1) * SCROLL_BAR_HEIGHT / (scrollMax + 1) + SCROLL_BAR_TOP - 1;

                if (start > end) {
                    end = start;
                }
                if (start <= y && y <= end) {
                    line = i;
                    break;
                }
            }
        }
    } else {
        line = 0;
    }
    return line;
}

static void func_ov110_021bfed0(PMSIVWordWin *wk, int vector) {
    int start, end;

    if (vector > 0) {
        start = (wk->y + 192) & 0xff;
        end = (start + vector * WORDWIN_LINE_HEIGHT) & 0xff;
    } else {
        end = wk->y;
        start = (end + vector * WORDWIN_LINE_HEIGHT) & 0xff;
    }
    if (start < end) {
        GFL_BitmapFillArea(BmpWin_GetBitmap(wk->win), 0, start, 208, end - start, 0);
    } else {
        GFL_BitmapFillArea(BmpWin_GetBitmap(wk->win), 0, start, 208, 256 - start, 0);
        GFL_BitmapFillArea(BmpWin_GetBitmap(wk->win), 0, 0, 208, end, 0);
    }
}

static void func_ov110_021bff60(PMSIVWordWin *wk) {
    wk->y = 0;
    wk->top = 0;
    GFL_BGSysMoveBG(2, BG_MOVE_SET_Y, 8);
}

static void func_ov110_021bff7c(PMSIVWordWin *wk, u32 index, u32 y) {
    Font *font = func_ov110_021bac44(wk->vwk);
    int height;
    u32 x;

    if (index < 2 || index >= func_ov110_021bf734(wk->mwk)) {
        return;
    }
    if (func_ov110_021bf6dc(wk->mwk) == 0 && func_ov110_021bf724(wk->mwk) == GROUP_NUMBERS) {
        ClActorPos pos;

        pos.x = ((index - 2) & 1) * 112 + 24;
        pos.y = y - 8;
        func_0204b528(wk->numberAct[index - 2], &pos);
        func_0204b3dc(wk->numberAct[index - 2], TRUE);
        return;
    }
    func_ov110_021bf758(wk->mwk, index - 2, wk->str);
    func_0201dd68();
    if (y <= 240) {
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->win), ((index - 2) & 1) * 112, y, wk->str, font);
        return;
    }
    height = 256 - y;
    GFL_BitmapFill(BmpWin_GetBitmap(wk->tmpWin), 0);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->tmpWin), 0, 0, wk->str, font);
    x = ((index - 2) & 1) * 112;
    GFL_BitmapCopyArea(BmpWin_GetBitmap(wk->tmpWin), BmpWin_GetBitmap(wk->win), 0, 0, x, y, 96, height, 0xffff);
    GFL_BitmapCopyArea(BmpWin_GetBitmap(wk->tmpWin), BmpWin_GetBitmap(wk->win), 0, height, x, 0, 96, 16 - height,
                       0xffff);
}

void func_ov110_021c00ac(PMSIVWordWin *wk, u32 pos) {
    ClActorPos actPos;

    wk->cursorVisible = func_0204b3f4(wk->cursor);
    func_0204b3dc(wk->cursor, TRUE);
    if (pos != -1) {
        actPos.x = (pos & 1) * 112 + 62;
        actPos.y = (pos / 2 + 1) * WORDWIN_LINE_HEIGHT;
        func_0204b404(wk->cursor, &actPos, CLACT_SURFACE_MAIN);
        func_0204b878(wk->cursor, CURSOR_ANIM_DECIDE);
    }
}

BOOL func_ov110_021c00fc(PMSIVWordWin *wk) {
    if (func_0204b898(wk->cursor) != CURSOR_ANIM_DECIDE) {
        return TRUE;
    }
    if (!func_0204b98c(wk->cursor)) {
        func_0204b3dc(wk->cursor, wk->cursorVisible);
        func_0204b878(wk->cursor, CURSOR_ANIM);
        wk->cursorVisible = FALSE;
        return TRUE;
    }
    return FALSE;
}
