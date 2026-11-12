// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov268/br_codein_disp.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The graphics of the Battle Recorder's code input: the actors of the digits, cursors and buttons on the lower
// screen, from resources that the screen has loaded with br_res.c. The ROM has no name for this file;
// "br_codein_disp.c" is a guess after overlay 205's codein_disp.c, which it is a copy of. Our names

#include "types.h"
#include "app/battle_recorder/br_codein.h"
#include "app/battle_recorder/br_codein_pv.h"
#include "app/battle_recorder/br_res.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/bmp.h"
#include "gfl/clact.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/gx.h"
#include "system/bmp_oam.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "text/system/0008.h"

// The digits' animations: the ten large digits after an empty place, then the same small. The bar follows
#define SEQ_SMALL 11
#define SEQ_BAR 22
// The animation of the cursor that flashes on a pressed key
#define SEQ_CURSOR_PRESS 3

#define CODE_X 76
#define CODE_Y 24
#define CODE_PRIORITY 10

static void func_ov183_021e10bc(BrCodeInWork *wk);
static void func_ov183_021e10dc(BrCodeInWork *wk);
static void func_ov183_021e111c(BrCodeInWork *wk, int button, int x, int y, int unused);

// The names of Back and OK
// pokebw2's sBrCodeInButtonMessages
static const u32 data_ov183_021e3850[BR_CODEIN_BUTTON_COUNT] = { Bank0008_Text_004, Bank0008_Text_103 };

// Takes the resources of the digits, the cursors and the buttons from the ones the screen loaded
// pokebw2's BrCodeInDisp_GetResources
void func_ov183_021e0840(BrCodeInWork *wk) {
    BrResObjData data;
    BOOL ret;

    ret = func_ov186_021fc124(wk->param.res, 11, &data);
    GFL_ASSERT(ret);
    wk->plttRes[BR_CODEIN_RES_CODE] = data.plt;
    wk->charRes[BR_CODEIN_RES_CODE] = data.chr;
    wk->cellAnimRes[BR_CODEIN_RES_CODE] = data.cell;
    ret = func_ov186_021fc124(wk->param.res, 12, &data);
    GFL_ASSERT(ret);
    wk->plttRes[BR_CODEIN_RES_CURSOR] = data.plt;
    wk->charRes[BR_CODEIN_RES_CURSOR] = data.chr;
    wk->cellAnimRes[BR_CODEIN_RES_CURSOR] = data.cell;
    ret = func_ov186_021fc124(wk->param.res, 6, &data);
    GFL_ASSERT(ret);
    wk->plttRes[BR_CODEIN_RES_BUTTON] = data.plt;
    wk->charRes[BR_CODEIN_RES_BUTTON] = data.chr;
    wk->cellAnimRes[BR_CODEIN_RES_BUTTON] = data.cell;
}

// pokebw2's BrCodeInDisp_Free
void func_ov183_021e08e4(BrCodeInWork *wk) {
    int i;

    for (i = 0; i < wk->codeMax; i++) {
        func_0204b3b4(wk->code[i].actor);
    }
    for (i = 0; i < BR_CODEIN_BAR_COUNT; i++) {
        func_0204b3b4(wk->bar[i].actor);
        func_0204b3b4(wk->btn[i].actor);
    }
    for (i = 0; i < BR_CODEIN_CURSOR_COUNT; i++) {
        func_0204b3b4(wk->cur[i].actor);
    }
    func_ov183_021e10bc(wk);
}

// Adds the digits' actors, with a bar after each of the first two groups
// pokebw2's BrCodeInDisp_AddCodeActors
void func_ov183_021e0954(BrCodeInWork *wk) {
    int i;
    int bar;
    int digit = 0;
    ClActUnit *unit = wk->unit;
    ClActorSetup setup;

    setup.x = 0;
    setup.y = 0;
    setup.sequence = 0;
    setup.priority = CODE_PRIORITY;
    setup.bgPriority = 0;
    bar = 0;
    for (i = 0; i < wk->codeMax + BR_CODEIN_BAR_COUNT; i++) {
        if (i == wk->bar[bar].state + bar + 1) {
            setup.x = CODE_X + i * 8;
            setup.y = CODE_Y;
            setup.sequence = SEQ_BAR;
            wk->bar[bar].actor =
                func_0204b294(unit, wk->charRes[BR_CODEIN_RES_CODE], wk->plttRes[BR_CODEIN_RES_CODE],
                              wk->cellAnimRes[BR_CODEIN_RES_CODE], &setup, CLACT_SURFACE_SUB, wk->heapId);
            func_0204b688(wk->bar[bar].actor, GX_OAM_MODE_XLU);
            bar++;
        } else {
            setup.x = CODE_X + i * 8;
            setup.y = CODE_Y;
            setup.sequence = func_ov183_021e0e40(wk->code[digit].state, wk->code[digit].size);
            wk->code[digit].actor =
                func_0204b294(unit, wk->charRes[BR_CODEIN_RES_CODE], wk->plttRes[BR_CODEIN_RES_CODE],
                              wk->cellAnimRes[BR_CODEIN_RES_CODE], &setup, CLACT_SURFACE_SUB, wk->heapId);
            func_0204b688(wk->code[digit].actor, GX_OAM_MODE_XLU);
            digit++;
        }
    }
}

// pokebw2's BrCodeInDisp_AddCursorActors
void func_ov183_021e0a88(BrCodeInWork *wk) {
    ClActUnit *unit = wk->unit;
    ClActorSetup setup;

    setup.x = 0;
    setup.y = 0;
    setup.sequence = 0;
    setup.priority = 0;
    setup.bgPriority = 0;
    wk->cur[BR_CODEIN_CURSOR_CODE].actor =
        func_0204b294(unit, wk->charRes[BR_CODEIN_RES_CURSOR], wk->plttRes[BR_CODEIN_RES_CURSOR],
                      wk->cellAnimRes[BR_CODEIN_RES_CURSOR], &setup, CLACT_SURFACE_SUB, wk->heapId);
    func_0204b688(wk->cur[BR_CODEIN_CURSOR_CODE].actor, GX_OAM_MODE_XLU);
    wk->cur[BR_CODEIN_CURSOR_KEY].actor =
        func_0204b294(unit, wk->charRes[BR_CODEIN_RES_CURSOR], wk->plttRes[BR_CODEIN_RES_CURSOR],
                      wk->cellAnimRes[BR_CODEIN_RES_CURSOR], &setup, CLACT_SURFACE_SUB, wk->heapId);
    func_0204b688(wk->cur[BR_CODEIN_CURSOR_KEY].actor, GX_OAM_MODE_XLU);
    wk->cur[BR_CODEIN_CURSOR_PRESS].actor =
        func_0204b294(unit, wk->charRes[BR_CODEIN_RES_CURSOR], wk->plttRes[BR_CODEIN_RES_CURSOR],
                      wk->cellAnimRes[BR_CODEIN_RES_CURSOR], &setup, CLACT_SURFACE_SUB, wk->heapId);
    func_0204b688(wk->cur[BR_CODEIN_CURSOR_PRESS].actor, GX_OAM_MODE_XLU);
    func_ov183_021e0d00(wk, 0);
    func_0204b878(wk->cur[BR_CODEIN_CURSOR_CODE].actor, 0);
    func_0204b92c(wk->cur[BR_CODEIN_CURSOR_CODE].actor, TRUE);
    wk->cur[BR_CODEIN_CURSOR_KEY].move.x = 0;
    wk->cur[BR_CODEIN_CURSOR_KEY].move.y = 0;
    wk->cur[BR_CODEIN_CURSOR_KEY].state = 1;
    func_ov183_021e0d38(wk, 0);
    func_0204b878(wk->cur[BR_CODEIN_CURSOR_KEY].actor, wk->cur[BR_CODEIN_CURSOR_KEY].state);
    func_0204b92c(wk->cur[BR_CODEIN_CURSOR_KEY].actor, TRUE);
    func_0204b688(wk->cur[BR_CODEIN_CURSOR_KEY].actor, GX_OAM_MODE_XLU);
    wk->cur[BR_CODEIN_CURSOR_PRESS].move.x = 0;
    wk->cur[BR_CODEIN_CURSOR_PRESS].move.y = 0;
    wk->cur[BR_CODEIN_CURSOR_PRESS].state = 1;
    func_ov183_021e0d38(wk, 0);
    func_0204b878(wk->cur[BR_CODEIN_CURSOR_PRESS].actor, wk->cur[BR_CODEIN_CURSOR_PRESS].state);
    func_0204b688(wk->cur[BR_CODEIN_CURSOR_PRESS].actor, GX_OAM_MODE_XLU);
    func_0204b92c(wk->cur[BR_CODEIN_CURSOR_PRESS].actor, TRUE);
    func_ov183_021e0ce8(wk, BR_CODEIN_CURSOR_KEY, FALSE);
    func_ov183_021e0ce8(wk, BR_CODEIN_CURSOR_PRESS, FALSE);
}

// pokebw2's BrCodeInDisp_AddButtonActors
void func_ov183_021e0c04(BrCodeInWork *wk) {
    ClActUnit *unit = wk->unit;
    ClActorSetup setup;
    ClActorPos pos;

    setup.x = 0;
    setup.y = 0;
    setup.sequence = 0;
    setup.priority = CODE_PRIORITY;
    setup.bgPriority = 0;
    wk->btn[0].actor = func_0204b294(unit, wk->charRes[BR_CODEIN_RES_BUTTON], wk->plttRes[BR_CODEIN_RES_BUTTON],
                                     wk->cellAnimRes[BR_CODEIN_RES_BUTTON], &setup, CLACT_SURFACE_SUB, wk->heapId);
    func_0204b688(wk->btn[0].actor, GX_OAM_MODE_XLU);
    setup.sequence = 1;
    wk->btn[1].actor = func_0204b294(unit, wk->charRes[BR_CODEIN_RES_BUTTON], wk->plttRes[BR_CODEIN_RES_BUTTON],
                                     wk->cellAnimRes[BR_CODEIN_RES_BUTTON], &setup, CLACT_SURFACE_SUB, wk->heapId);
    func_0204b688(wk->btn[1].actor, GX_OAM_MODE_XLU);
    pos.x = wk->rects[BR_CODEIN_RECT_BACK].left;
    pos.y = wk->rects[BR_CODEIN_RECT_BACK].top + 16;
    func_0204b404(wk->btn[0].actor, &pos, CLACT_SURFACE_SUB);
    func_0204b878(wk->btn[0].actor, 0);
    pos.x = wk->rects[BR_CODEIN_RECT_OK].left;
    pos.y = wk->rects[BR_CODEIN_RECT_OK].top + 16;
    func_0204b404(wk->btn[1].actor, &pos, CLACT_SURFACE_SUB);
    func_0204b878(wk->btn[1].actor, 1);
}

// pokebw2's BrCodeInDisp_SetCursorVisible
void func_ov183_021e0ce8(BrCodeInWork *wk, int cursor, BOOL visible) {
    func_0204b3dc(wk->cur[cursor].actor, visible);
}

// Puts the code's cursor under a digit
// pokebw2's BrCodeInDisp_SetCodeCursor
void func_ov183_021e0d00(BrCodeInWork *wk, int digit) {
    ClActorPos pos;
    ClActor *actor = wk->code[digit].actor;

    wk->cur[BR_CODEIN_CURSOR_CODE].state = digit;
    func_0204b454(actor, &pos, CLACT_SURFACE_SUB);
    pos.y += 16;
    func_0204b404(wk->cur[BR_CODEIN_CURSOR_CODE].actor, &pos, CLACT_SURFACE_SUB);
}

// Puts the keys' cursor on a key and restarts its animation
// pokebw2's BrCodeInDisp_SetKeyCursor
void func_ov183_021e0d38(BrCodeInWork *wk, int key) {
    ClActorPos pos;
    int height = wk->rects[BR_CODEIN_RECT_KEYS + key].bottom - wk->rects[BR_CODEIN_RECT_KEYS + key].top;
    int width = wk->rects[BR_CODEIN_RECT_KEYS + key].right - wk->rects[BR_CODEIN_RECT_KEYS + key].left;

    pos.x = width / 2 + wk->rects[BR_CODEIN_RECT_KEYS + key].left;
    pos.y = height / 2 + wk->rects[BR_CODEIN_RECT_KEYS + key].top;
    func_0204b404(wk->cur[BR_CODEIN_CURSOR_KEY].actor, &pos, CLACT_SURFACE_SUB);
    func_0204b8c4(wk->cur[BR_CODEIN_CURSOR_KEY].actor, 0);
}

// Gives the keys' cursors their animations, and hides the keys' cursor while a press flashes or the touch screen is
// in use
// pokebw2's BrCodeInDisp_UpdateCursors
void func_ov183_021e0d90(BrCodeInWork *wk) {
    int i;

    for (i = BR_CODEIN_CURSOR_KEY; i < BR_CODEIN_CURSOR_COUNT; i++) {
        int sequence = func_0204b898(wk->cur[i].actor);

        if (sequence == SEQ_CURSOR_PRESS) {
            if (!func_0204b98c(wk->cur[i].actor)) {
                func_0204b878(wk->cur[i].actor, wk->cur[i].state);
                if (wk->touchMode == TRUE) {
                    func_ov183_021e0ce8(wk, BR_CODEIN_CURSOR_KEY, FALSE);
                } else {
                    func_ov183_021e0ce8(wk, BR_CODEIN_CURSOR_KEY, TRUE);
                }
                func_ov183_021e0ce8(wk, BR_CODEIN_CURSOR_PRESS, FALSE);
            }
        } else {
            if (sequence != wk->cur[i].state) {
                func_0204b878(wk->cur[i].actor, wk->cur[i].state);
            }
            if (func_0204b898(wk->cur[i].actor) != SEQ_CURSOR_PRESS) {
                if (wk->touchMode == TRUE) {
                    func_ov183_021e0ce8(wk, BR_CODEIN_CURSOR_KEY, FALSE);
                } else {
                    func_ov183_021e0ce8(wk, BR_CODEIN_CURSOR_KEY, TRUE);
                }
            }
        }
    }
}

// pokebw2's BrCodeInDisp_GetCodeSequence
int func_ov183_021e0e40(int state, int size) {
    int base = 0;

    if (size == 0) {
        base = SEQ_SMALL;
    }
    return base + state;
}

// pokebw2's BrCodeInDisp_SetCodeSizes
void func_ov183_021e0e4c(BrCodeInWork *wk) {
    int i;

    for (i = 0; i < wk->codeMax; i++) {
        if (i >= wk->largeStart && i < wk->largeEnd) {
            wk->code[i].size = 1;
        } else {
            wk->code[i].size = 0;
        }
    }
}

// pokebw2's BrCodeInDisp_SetCodePositions
void func_ov183_021e0e90(BrCodeInWork *wk, BOOL move) {
    int i;
    s16 x = wk->focusX[wk->focus];
    int bar = 0;
    ClActorPos pos;
    ClActorPos barPos;

    for (i = 0; i < wk->codeMax; i++) {
        if (i >= wk->largeStart && i < wk->largeEnd) {
            if (i == wk->largeStart) {
                x += 20;
            } else {
                x += 32;
            }
        } else {
            if (i == 0) {
                x += 20;
            } else {
                x += 8;
            }
        }
        func_0204b454(wk->code[i].actor, &pos, CLACT_SURFACE_SUB);
        if (!move) {
            pos.x = x;
            func_0204b404(wk->code[i].actor, &pos, CLACT_SURFACE_SUB);
        } else {
            wk->code[i].move.x = (x - pos.x) / 4;
            wk->code[i].move.y = 0;
            wk->code[i].move.wait = 4;
            wk->code[i].move.scale = 0;
        }
        if (i == wk->bar[bar].state && bar != BR_CODEIN_BAR_COUNT) {
            func_0204b454(wk->bar[bar].actor, &barPos, CLACT_SURFACE_SUB);
            if (wk->largeStart == wk->largeEnd) {
                x += 8;
            } else if (i > wk->largeStart && i < wk->largeEnd) {
                x += 20;
            } else {
                x += 8;
            }
            if (!move) {
                barPos.x = x;
                func_0204b404(wk->bar[bar].actor, &barPos, CLACT_SURFACE_SUB);
            } else {
                wk->bar[bar].move.x = (x - barPos.x) / 4;
                wk->bar[bar].move.y = 0;
                wk->bar[bar].move.wait = 4;
            }
            bar++;
        }
    }
}

// Puts the digits' touch rectangles around their actors
// pokebw2's BrCodeInDisp_UpdateCodeRects
void func_ov183_021e1008(BrCodeInWork *wk) {
    int i;
    int halfWidth;
    int halfHeight;
    ClActorPos pos;

    for (i = 0; i < wk->codeMax; i++) {
        if (i >= wk->largeStart && i < wk->largeEnd) {
            halfWidth = 16;
            halfHeight = 16;
        } else {
            halfWidth = 4;
            halfHeight = 8;
        }
        func_0204b454(wk->code[i].actor, &pos, CLACT_SURFACE_SUB);
        wk->code[i].rect->top = pos.y - halfHeight;
        wk->code[i].rect->left = pos.x - halfWidth;
        wk->code[i].rect->bottom = pos.y + halfHeight;
        wk->code[i].rect->right = pos.x + halfWidth;
    }
}

// pokebw2's BrCodeInDisp_InitBmpOam
void func_ov183_021e10a0(BrCodeInWork *wk) {
    wk->bmpOam = func_02025538(wk->heapId, wk->unit);
}

// pokebw2's BrCodeInDisp_FreeBmpOam
static void func_ov183_021e10bc(BrCodeInWork *wk) {
    func_0202570c(wk->btnText[0]);
    func_0202570c(wk->btnText[1]);
    func_02025588(wk->bmpOam);
}

// What is left of codein_disp.c's function, which loads the font's palette for the buttons' names: the names use the
// buttons' palette
// pokebw2's BrCodeInDisp_LoadButtonTextPltt
static void func_ov183_021e10dc(BrCodeInWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_FONT, wk->heapId);

    GFL_ArcToolFree(arc);
}

// pokebw2's BrCodeInDisp_AddButtonTexts
void func_ov183_021e10f4(BrCodeInWork *wk) {
    func_ov183_021e10dc(wk);
    func_ov183_021e111c(wk, 0, 32, 8, 0);
    func_ov183_021e111c(wk, 1, 32, 8, 0);
}

// Draws the name of Back or OK and shows it on its button, x and y from the button's corner
// pokebw2's BrCodeInDisp_AddButtonText
static void func_ov183_021e111c(BrCodeInWork *wk, int button, int x, int y, int unused) {
    BmpOamActorSetup setup;
    MsgData *msgData = func_ov186_021fc214(wk->param.res);
    StrBuf *str = GFL_MsgDataLoadStrbufNew(msgData, data_ov183_021e3850[button]);
    GFLBitmap *bitmap = GFL_BitmapCreate(10, 2, 0x20, wk->heapId);
    int left;
    int top;

    GFL_TextRendererDrawToBitmap(bitmap, 1, 0, str, wk->font);
    left = wk->rects[BR_CODEIN_RECT_BACK + button].left;
    top = wk->rects[BR_CODEIN_RECT_BACK + button].top;
    setup.bitmap = bitmap;
    setup.x = left + x;
    setup.y = top + y;
    setup.palette = wk->plttRes[BR_CODEIN_RES_BUTTON];
    setup.paletteOffset = 0;
    setup.priority = 0;
    setup.bgPriority = 0;
    setup.surface = CLACT_SURFACE_SUB;
    setup.vramType = CLACT_VRAM_SUB;
    wk->btnText[button] = func_020255a0(wk->bmpOam, &setup);
    func_020257d0(wk->btnText[button]);
    func_0202597c(wk->btnText[button], GX_OAM_MODE_XLU);
    GFL_BitmapFree(bitmap);
    GFL_StrBufFree(str);
}
