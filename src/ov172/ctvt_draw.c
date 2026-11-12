// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov257/ctvt_draw.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/comm_tvt/ctvt_draw.h"
#include "types.h"
#include "app/comm_tvt/comm_tvt_sys.h"
#include "app/comm_tvt/ctvt_camera.h"
#include "app/comm_tvt/ctvt_comm.h"
#include "app/comm_tvt/draw_system.h"
#include "constants/sound.h"
#include "constants/version.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/sound.h"
#include "gfl/touchpanel.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "system/app_menu_common.h"
#include "system/bmp_winframe.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "gfl/str.h"
#include "nnsys/gfd.h"

// The Xtransceiver's drawing mode: the members draw on the top screen over the video, with pens of eight shapes and
// a color picked from a palette or from the screen. The strokes are sent to the others through ctvt_comm.c

// The color a pen starts with: black in Black 2, white in White 2
#ifdef BLACK2
#define CTVT_DRAW_DEFAULT_COLOR 0x8000
#else
#define CTVT_DRAW_DEFAULT_COLOR 0xffff
#endif

#define CTVT_DRAW_TOOL_BUTTONS 4
#define CTVT_DRAW_BUTTONS 6
#define CTVT_DRAW_MENU_ITEMS 5
#define CTVT_DRAW_TOUCH_MAX 7

// How far the buttons of the bottom screen slide in, in pixels
#define CTVT_DRAW_SLIDE 24

// The direct color of the top screen's bitmap that is drawn, past the transparent ones
#define CTVT_DRAW_OPAQUE 0x8000

enum {
    DRAW_STATE_FADE_IN,
    DRAW_STATE_WAIT_FADE_IN,
    DRAW_STATE_FADE_OUT,
    DRAW_STATE_WAIT_FADE_OUT,
    DRAW_STATE_MENU,
    DRAW_STATE_DRAW,
    DRAW_STATE_DISCONNECTED,
    DRAW_STATE_WAIT_DISCONNECTED,
    DRAW_STATE_SEND_PARENT_EXIT,
    DRAW_STATE_PARENT_LEFT,
    DRAW_STATE_WAIT_PARENT_LEFT,
    DRAW_STATE_SYNC_EXIT,
    DRAW_STATE_WAIT_SYNC_EXIT,
    DRAW_STATE_ALONE,
    DRAW_STATE_WAIT_ALONE,
};

// The tools of the drawing mode
enum {
    DRAW_TOOL_PEN,
    DRAW_TOOL_PICK,
    DRAW_TOOL_ERASER,
    DRAW_TOOL_STAMP,
};

struct CtvtDraw {
    int state;
    u8 slideTarget;
    u8 slide;
    BOOL inputHandled;
    BOOL stroking;
    BOOL penMenuOpen;
    BOOL stampMenuOpen;
    BOOL drawEnabled;
    BOOL exit;
    BOOL waitSound;
    BOOL stampChosen;
    int tool;
    u32 lastX;
    u32 lastY;
    u16 color;
    u8 pen;
    u8 stamp;
    BOOL menuWindowPending;
    BOOL menuExpanded;
    BOOL titleWindowPending;
    BOOL messageWindowPending;
    BmpWin *menuWindow;
    BmpWin *titleWindow;
    BmpWin *messageWindow;
    u16 waitFrames;
    ClActor *buttons[CTVT_DRAW_BUTTONS];
    ClActor *menuItems[CTVT_DRAW_MENU_ITEMS];
    ClActor *penMenu;
    ClActor *stampMenu;
    ClActor *menuCursor;
    ClActor *selectFrame;
    u16 cursorColor;
    u16 cursorPhase;
};

static void func_ov172_021bae58(CommTvtWork *sys, CtvtDraw *draw);
static void func_ov172_021baee0(CommTvtWork *sys, CtvtDraw *draw);
static void func_ov172_021bb288(CommTvtWork *sys, CtvtDraw *draw);
static void func_ov172_021bb4d0(CommTvtWork *sys, CtvtDraw *draw);
static void func_ov172_021bb54c(CommTvtWork *sys, CtvtDraw *draw);
static void func_ov172_021bb65c(CommTvtWork *sys, CtvtDraw *draw, BOOL expanded);
static BOOL func_ov172_021bb8f4(CommTvtWork *sys, CtvtDraw *draw);
static void func_ov172_021bb938(CommTvtWork *sys, CtvtDraw *draw, u32 msgId);
static void func_ov172_021bb9e4(CommTvtWork *sys, CtvtDraw *draw);
BOOL func_ov172_021b77b4(CommTvtWork *sys, CtvtCamera *camera);
static void CtvtDraw_UpdateCancel(CommTvtWork *sys, CtvtDraw *draw);

// Nothing reads them (pokebw2's). Under -ipa function the data waiting to be written out goes with each function
// that MWCC emits, each batch sorted by size: defined above the first function, these come first in .rodata,
// and the tables below, defined above their first user, after them (pokebw2's docs/matching.md)
static const u16 sUnusedColors[] = { 0x1c13, 0x1313, 0x1c1c, 0x4e73, 0x739c };

// pokebw2's CtvtDraw_Create
CtvtDraw *func_ov172_021ba280(CommTvtWork *sys, HeapID heapId) {
    CtvtDraw *draw = GFL_HeapAllocate(heapId, sizeof(CtvtDraw), TRUE, "ctvt_draw.c", 207);

    draw->pen = DRAW_PEN_DOT;
    draw->stamp = DRAW_PEN_HEART;
    draw->tool = 0;
    draw->stampChosen = FALSE;
#if GAME_VERSION == VERSION_WHITE
    draw->color = 0xFFFF;
#else
    draw->color = 1 << 15;
#endif
    return draw;
}

// pokebw2's CtvtDraw_Delete
void func_ov172_021ba2b8(CommTvtWork *sys, CtvtDraw *draw) {
    GFL_HeapFree(draw);
}

void func_ov172_021ba2c4(CommTvtWork *sys, CtvtDraw *draw) {
    HeapID heapId = func_ov172_021be00c(sys);
    ArcTool *arc = func_ov172_021be010(sys);
    ClActorSetup setup;
    ClActUnit *unit;
    u32 chars;
    u32 palette;
    u32 cellAnims;
    GFLBitmap *bitmap;
    StrBuf *strbuf;
    MsgData *msgData;
    Font *font;
    PrintQueue *queue;
    u8 i;

    loadBGScrToVramByFileNoReserveNegAlign(arc, 0x10, 7, 0, 0, 0, heapId);
    GFL_BGSysLoadScr(7);
    GFL_BGSysSetBGEnabled(5, 0);
    *(u16 *)0x04000304 = *(u16 *)0x04000304 & ~0x8000;

    arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), heapId);
    GFL_BGSysClearScr(0);
    GFL_G2DIOLoadNSCRSync(arc, func_0202762c(), 0, 0x60, 0, 0x600, 0, heapId);
    GFL_BGSysSetScrPaletteNo(0, 0, 0x15, 0x20, 3, 0);
    GFL_BGSysLoadScr(0);
    GFL_BGSysMoveBG(0, 3, 0);
    GFL_ArcToolFree(arc);

    setup.x = 0x10;
    setup.y = 0xcc;
    setup.sequence = 1;
    setup.priority = 8;
    setup.bgPriority = 0;
    unit = func_ov172_021be024(sys);
    chars = func_ov172_021be018(sys, 4);
    palette = func_ov172_021be018(sys, 0);
    cellAnims = func_ov172_021be018(sys, 8);
    draw->buttons[0] = func_0204b294(unit, chars, palette, cellAnims, &setup, 0, heapId);

    setup.x = 0x38;
    setup.sequence = 3;
    unit = func_ov172_021be024(sys);
    chars = func_ov172_021be018(sys, 4);
    palette = func_ov172_021be018(sys, 0);
    cellAnims = func_ov172_021be018(sys, 8);
    draw->buttons[1] = func_0204b294(unit, chars, palette, cellAnims, &setup, 0, heapId);

    setup.x = 0x60;
    setup.sequence = 5;
    unit = func_ov172_021be024(sys);
    chars = func_ov172_021be018(sys, 4);
    palette = func_ov172_021be018(sys, 0);
    cellAnims = func_ov172_021be018(sys, 8);
    draw->buttons[2] = func_0204b294(unit, chars, palette, cellAnims, &setup, 0, heapId);

    setup.x = 0x88;
    setup.sequence = 7;
    unit = func_ov172_021be024(sys);
    chars = func_ov172_021be018(sys, 4);
    palette = func_ov172_021be018(sys, 0);
    cellAnims = func_ov172_021be018(sys, 8);
    draw->buttons[3] = func_0204b294(unit, chars, palette, cellAnims, &setup, 0, heapId);

    setup.x = 0xc8;
    if (func_ov172_021be0b4(sys) == 1) {
        setup.sequence = 0xc;
    } else {
        setup.sequence = 0xa;
    }
    unit = func_ov172_021be024(sys);
    chars = func_ov172_021be018(sys, 4);
    palette = func_ov172_021be018(sys, 0);
    cellAnims = func_ov172_021be018(sys, 8);
    draw->buttons[4] = func_0204b294(unit, chars, palette, cellAnims, &setup, 0, heapId);

    setup.x = 0xe0;
    setup.y = 0xc0;
    setup.sequence = 1;
    unit = func_ov172_021be024(sys);
    chars = func_ov172_021be018(sys, 7);
    palette = func_ov172_021be018(sys, 3);
    cellAnims = func_ov172_021be018(sys, 10);
    draw->buttons[5] = func_0204b294(unit, chars, palette, cellAnims, &setup, 0, heapId);
    func_0204b92c(draw->buttons[5], TRUE);
    for (i = 0; i < CTVT_DRAW_BUTTONS; i++) {
        func_0204b3dc(draw->buttons[i], TRUE);
    }

    setup.x = 0x38;
    setup.y = 0x48;
    setup.sequence = 0xe;
    unit = func_ov172_021be024(sys);
    chars = func_ov172_021be018(sys, 5);
    palette = func_ov172_021be018(sys, 1);
    cellAnims = func_ov172_021be018(sys, 9);
    draw->menuItems[0] = func_0204b294(unit, chars, palette, cellAnims, &setup, 1, heapId);

    setup.y = 0x5c;
    setup.sequence = 0xf;
    unit = func_ov172_021be024(sys);
    chars = func_ov172_021be018(sys, 5);
    palette = func_ov172_021be018(sys, 1);
    cellAnims = func_ov172_021be018(sys, 9);
    draw->menuItems[1] = func_0204b294(unit, chars, palette, cellAnims, &setup, 1, heapId);

    setup.y = 0x70;
    setup.sequence = 0x10;
    unit = func_ov172_021be024(sys);
    chars = func_ov172_021be018(sys, 5);
    palette = func_ov172_021be018(sys, 1);
    cellAnims = func_ov172_021be018(sys, 9);
    draw->menuItems[2] = func_0204b294(unit, chars, palette, cellAnims, &setup, 1, heapId);

    setup.y = 0x84;
    setup.sequence = 0x11;
    unit = func_ov172_021be024(sys);
    chars = func_ov172_021be018(sys, 5);
    palette = func_ov172_021be018(sys, 1);
    cellAnims = func_ov172_021be018(sys, 9);
    draw->menuItems[3] = func_0204b294(unit, chars, palette, cellAnims, &setup, 1, heapId);

    setup.x = 0x2c;
    setup.y = 0x8c;
    setup.sequence = 1;
    unit = func_ov172_021be024(sys);
    chars = func_ov172_021be018(sys, 6);
    palette = func_ov172_021be018(sys, 2);
    cellAnims = func_ov172_021be018(sys, 10);
    draw->menuItems[4] = func_0204b294(unit, chars, palette, cellAnims, &setup, 1, heapId);
    for (i = 0; i < CTVT_DRAW_MENU_ITEMS; i++) {
        func_0204b3dc(draw->menuItems[i], FALSE);
    }

    setup.x = 0x10;
    setup.y = 0xa0;
    setup.sequence = 0xf;
    unit = func_ov172_021be024(sys);
    chars = func_ov172_021be018(sys, 4);
    palette = func_ov172_021be018(sys, 0);
    cellAnims = func_ov172_021be018(sys, 8);
    draw->penMenu = func_0204b294(unit, chars, palette, cellAnims, &setup, 0, heapId);
    func_0204b3dc(draw->penMenu, FALSE);

    setup.x = 0x88;
    setup.y = 0xa0;
    setup.sequence = 0x10;
    unit = func_ov172_021be024(sys);
    chars = func_ov172_021be018(sys, 4);
    palette = func_ov172_021be018(sys, 0);
    cellAnims = func_ov172_021be018(sys, 8);
    draw->stampMenu = func_0204b294(unit, chars, palette, cellAnims, &setup, 0, heapId);
    func_0204b3dc(draw->stampMenu, FALSE);

    setup.priority = 0;
    unit = func_ov172_021be024(sys);
    chars = func_ov172_021be018(sys, 4);
    palette = func_ov172_021be018(sys, 0);
    cellAnims = func_ov172_021be018(sys, 8);
    draw->menuCursor = func_0204b294(unit, chars, palette, cellAnims, &setup, 0, heapId);
    func_0204b3dc(draw->menuCursor, FALSE);

    setup.sequence = 0x19;
    setup.priority = 2;
    unit = func_ov172_021be024(sys);
    chars = func_ov172_021be018(sys, 4);
    palette = func_ov172_021be018(sys, 0);
    cellAnims = func_ov172_021be018(sys, 8);
    draw->selectFrame = func_0204b294(unit, chars, palette, cellAnims, &setup, 0, heapId);
    func_0204b3dc(draw->selectFrame, FALSE);

    func_ov172_021bb54c(sys, draw);
    draw->menuWindow = NULL;
    draw->messageWindow = NULL;

    font = func_ov172_021be034(sys);
    msgData = func_ov172_021be03c(sys);
    queue = func_ov172_021be044(sys);
    draw->titleWindow = BmpWin_CreateDynamic(4, 3, 1, 0x1c, 2, 10, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(draw->titleWindow), 0);
    strbuf = GFL_MsgDataLoadStrbufNew(msgData, 0xb);
    bitmap = BmpWin_GetBitmap(draw->titleWindow);
    func_0201c724(queue, bitmap, 1, 0, strbuf, font, 0x440);
    GFL_StrBufFree(strbuf);
    draw->titleWindowPending = TRUE;

    draw->state = 0;
    draw->slideTarget = CTVT_DRAW_SLIDE;
    draw->slide = CTVT_DRAW_SLIDE;
    draw->cursorPhase = 0;
    draw->stroking = FALSE;
    draw->inputHandled = FALSE;
    draw->penMenuOpen = FALSE;
    draw->stampMenuOpen = FALSE;
    draw->menuWindowPending = FALSE;
    draw->messageWindowPending = FALSE;
    draw->drawEnabled = FALSE;
    draw->exit = FALSE;
    draw->waitSound = FALSE;
    ((u16 *)HW_OBJ_PLTT)[13] = draw->color & 0x7fff;
    func_ov172_021bb65c(sys, draw, FALSE);
    func_0203d34c(TRUE, heapId);
    func_ov172_021be2e0(sys);
}

static inline void CtvtDraw_ClearWindow(BmpWin *window) {
    func_020456f8(window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
}

// pokebw2's CtvtDraw_Leave
void func_ov172_021ba8c4(CommTvtWork *sys, CtvtDraw *draw) {
    HeapID heapId = func_ov172_021be00c(sys);
    u8 i;

    if (draw->tool == DRAW_TOOL_PICK) {
        if (draw->stampChosen == FALSE) {
            draw->tool = DRAW_TOOL_PEN;
        } else {
            draw->tool = DRAW_TOOL_STAMP;
        }
    }
    func_0203d34c(FALSE, heapId);
    func_0201f7a8(draw->menuWindow, 0);
    CtvtDraw_ClearWindow(draw->menuWindow);
    BmpWin_Free(draw->menuWindow);
    CtvtDraw_ClearWindow(draw->titleWindow);
    BmpWin_Free(draw->titleWindow);
    if (draw->messageWindow != NULL) {
        CtvtDraw_ClearWindow(draw->messageWindow);
        BmpWin_Free(draw->messageWindow);
        draw->messageWindow = NULL;
    }
    for (i = 0; i < CTVT_DRAW_MENU_ITEMS; i++) {
        func_0204b3b4(draw->menuItems[i]);
    }
    for (i = 0; i < CTVT_DRAW_BUTTONS; i++) {
        func_0204b3b4(draw->buttons[i]);
    }
    func_0204b3b4(draw->selectFrame);
    func_0204b3b4(draw->menuCursor);
    func_0204b3dc(draw->stampMenu, FALSE);
    func_0204b3b4(draw->stampMenu);
    func_0204b3dc(draw->penMenu, FALSE);
    func_0204b3b4(draw->penMenu);
    GFL_BGSysClearScr(0);
    GFL_BGSysLoadScr(0);
    GFL_BGSysMoveBG(0, 3, 0);
    GFL_BGSysClearScr(6);
    GFL_BGSysLoadScr(6);
    GFL_BGSysSetBGEnabled(5, TRUE);
    GX_SetDispSelect(1);
    func_ov172_021be1a4(sys);
}

// pokebw2's CtvtDraw_UpdateMenu
int func_ov172_021ba9ec(CommTvtWork *sys, CtvtDraw *draw) {
    HeapID heapId = func_ov172_021be00c(sys);
    CtvtCamera *camera;
    CtvtComm *comm;
    u8 i;

    draw->inputHandled = FALSE;
    switch (draw->state) {
    case DRAW_STATE_FADE_IN:
        if (func_ov172_021be02c(sys) == TRUE) {
            GFL_WipeSet(0, 1, 1, 0, 6, 1, heapId);
        } else {
            GFL_WipeSet(4, 1, 1, 0, 6, 1, heapId);
        }
        draw->state = DRAW_STATE_WAIT_FADE_IN;
        break;
    case DRAW_STATE_WAIT_FADE_IN:
        if (GFL_WipeIsFinished() == TRUE) {
            draw->drawEnabled = TRUE;
            draw->state = DRAW_STATE_MENU;
        }
        break;
    case DRAW_STATE_FADE_OUT:
        camera = func_ov172_021bdfdc(sys);
        if (draw->waitSound && func_ov172_021b7590(sys, camera) != TRUE) {
            break;
        }
        GFL_WipeSet(0, 0, 0, 0, 6, 1, heapId);
        draw->state = DRAW_STATE_WAIT_FADE_OUT;
        draw->drawEnabled = FALSE;
        break;
    case DRAW_STATE_WAIT_FADE_OUT:
        if (GFL_WipeIsFinished() == TRUE) {
            if (draw->exit == FALSE) {
                return COMM_TVT_MODE_TALK;
            }
            return COMM_TVT_MODE_GAME;
        }
        break;
    case DRAW_STATE_MENU:
        if (func_ov172_021bb8f4(sys, draw) == FALSE) {
            func_ov172_021bae58(sys, draw);
        }
        break;
    case DRAW_STATE_DRAW:
        if (func_ov172_021bb8f4(sys, draw) == FALSE) {
            func_ov172_021baee0(sys, draw);
        }
        break;
    case DRAW_STATE_DISCONNECTED:
        comm = func_ov172_021bdfe4(sys);
        func_ov172_021b8084(sys, comm);
        func_ov172_021bb938(sys, draw, 36);
        func_ov172_021be0b0(sys, TRUE);
        draw->state = DRAW_STATE_WAIT_DISCONNECTED;
        camera = func_ov172_021bdfdc(sys);
        func_ov172_021b7b80(sys, camera);
        draw->waitSound = TRUE;
        break;
    case DRAW_STATE_WAIT_DISCONNECTED:
        if (func_02035c60() == TRUE || (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            draw->exit = TRUE;
            draw->state = DRAW_STATE_FADE_OUT;
            func_ov172_021be0b0(sys, TRUE);
            camera = func_ov172_021bdfdc(sys);
            func_ov172_021b7578(sys, camera);
            draw->waitSound = TRUE;
        }
        break;
    case DRAW_STATE_SEND_PARENT_EXIT:
        comm = func_ov172_021bdfe4(sys);
        if (func_ov172_021b8740(sys, comm, CTVT_PACKET_UNK_A, 0) == TRUE) {
            draw->state = DRAW_STATE_PARENT_LEFT;
        }
        break;
    case DRAW_STATE_PARENT_LEFT:
        comm = func_ov172_021bdfe4(sys);
        func_ov172_021b8084(sys, comm);
        func_ov172_021bb938(sys, draw, 37);
        func_ov172_021be0b0(sys, TRUE);
        draw->state = DRAW_STATE_WAIT_PARENT_LEFT;
        draw->waitFrames = 0;
        camera = func_ov172_021bdfdc(sys);
        func_ov172_021b7b80(sys, camera);
        draw->waitSound = TRUE;
        break;
    case DRAW_STATE_WAIT_PARENT_LEFT:
        draw->waitFrames++;
        if (func_02035c60() == TRUE || (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))
            || draw->waitFrames >= 300) {
            draw->state = DRAW_STATE_SYNC_EXIT;
        }
        break;
    case DRAW_STATE_SYNC_EXIT:
        comm = func_ov172_021bdfe4(sys);
        func_ov172_021b8b34(sys, comm, 47);
        draw->state = DRAW_STATE_WAIT_SYNC_EXIT;
        break;
    case DRAW_STATE_WAIT_SYNC_EXIT:
        comm = func_ov172_021bdfe4(sys);
        if (func_ov172_021b8b48(sys, comm, 47) == TRUE) {
            draw->exit = TRUE;
            draw->state = DRAW_STATE_FADE_OUT;
            func_ov172_021be0b0(sys, TRUE);
            camera = func_ov172_021bdfdc(sys);
            func_ov172_021b7578(sys, camera);
            draw->waitSound = TRUE;
        }
        break;
    case DRAW_STATE_ALONE:
        comm = func_ov172_021bdfe4(sys);
        func_ov172_021b8084(sys, comm);
        func_ov172_021bb938(sys, draw, 38);
        func_ov172_021be0b0(sys, TRUE);
        draw->state = DRAW_STATE_WAIT_ALONE;
        camera = func_ov172_021bdfdc(sys);
        func_ov172_021b7b80(sys, camera);
        draw->waitSound = TRUE;
        break;
    case DRAW_STATE_WAIT_ALONE:
        if (func_02035c60() == TRUE || (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            draw->exit = TRUE;
            draw->state = DRAW_STATE_FADE_OUT;
            func_ov172_021be0b0(sys, TRUE);
            camera = func_ov172_021bdfdc(sys);
            func_ov172_021b7578(sys, camera);
            draw->waitSound = TRUE;
        }
        break;
    }

    func_ov172_021bb4d0(sys, draw);
    if (draw->drawEnabled == TRUE) {
        func_ov172_021bb288(sys, draw);
    }
    if (draw->menuWindowPending == TRUE) {
        PrintQueue *queue = func_ov172_021be044(sys);

        if (func_0201c6c4(queue, BmpWin_GetBitmap(draw->menuWindow)) == FALSE) {
            BmpWin_DrawFrame(draw->menuWindow, 2, 0x140, 9);
            BmpWin_FlushChar(draw->menuWindow);
            BmpWin_FlushMap(draw->menuWindow);
            GFL_BGSysQueueScrLoad(4);
            draw->menuWindowPending = FALSE;
            if (draw->menuExpanded == TRUE) {
                for (i = 0; i < CTVT_DRAW_MENU_ITEMS; i++) {
                    func_0204b3dc(draw->menuItems[i], TRUE);
                }
            } else {
                for (i = 0; i < CTVT_DRAW_MENU_ITEMS; i++) {
                    func_0204b3dc(draw->menuItems[i], FALSE);
                }
            }
        }
    }
    if (draw->titleWindowPending == TRUE) {
        PrintQueue *queue = func_ov172_021be044(sys);

        if (func_0201c6c4(queue, BmpWin_GetBitmap(draw->titleWindow)) == FALSE) {
            BmpWin_FlushChar(draw->titleWindow);
            BmpWin_FlushMap(draw->titleWindow);
            GFL_BGSysLoadScr(4);
            draw->titleWindowPending = FALSE;
        }
    }
    if (draw->messageWindowPending == TRUE && draw->messageWindow != NULL) {
        PrintQueue *queue = func_ov172_021be044(sys);

        if (func_0201c6c4(queue, BmpWin_GetBitmap(draw->messageWindow)) == FALSE) {
            BmpWin_FlushChar(draw->messageWindow);
            BmpWin_FlushMap(draw->messageWindow);
            GFL_BGSysLoadScr(0);
            draw->messageWindowPending = FALSE;
        }
    }
    if (draw->state <= DRAW_STATE_DRAW && func_ov172_021be0b4(sys) == FALSE) {
        camera = func_ov172_021bdfdc(sys);
        if (GCTX_HIDGetPressedKeys() & PAD_KEY_UP) {
            func_ov172_021b7b3c(sys, camera, CAMERA_SIZE_DSI_VGA);
        }
        if (GCTX_HIDGetPressedKeys() & PAD_KEY_RIGHT) {
            func_ov172_021b7b3c(sys, camera, CAMERA_SIZE_DS_LCD);
        }
        if (GCTX_HIDGetPressedKeys() & PAD_KEY_DOWN) {
            func_ov172_021b7b3c(sys, camera, CAMERA_SIZE_QVGA);
        }
        if (GCTX_HIDGetPressedKeys() & PAD_KEY_LEFT) {
            func_ov172_021b7b3c(sys, camera, CAMERA_SIZE_CIF);
        }
    }
    if (func_ov172_021be0a4(sys) == 0 && (GCTX_HIDGetPressedKeys() & PAD_BUTTON_L)) {
        camera = func_ov172_021bdfdc(sys);
        if (func_ov172_021b77b4(sys, camera) == FALSE) {
            func_ov172_021be08c(sys);
            GFL_SndSEPlay(SEQ_SE_SYS_45);
        }
    }

    if (draw->cursorPhase + 0x400 >= 0x10000) {
        draw->cursorPhase = draw->cursorPhase + 0x400 - 0x10000;
    } else {
        draw->cursorPhase += 0x400;
    }
    {
        s16 cos = FX_CosIdx(draw->cursorPhase);
        s16 half = (s16)((cos + FX32_ONE) / 2);
        u8 level = ((half * -9) >> FX32_SHIFT) + 28;

        draw->cursorColor = GX_RGB(level, level, level);
    }
    NNS_GfdRegisterNewVramTransferTask(14, 0xde, &draw->cursorColor, sizeof(draw->cursorColor));
    return COMM_TVT_MODE_DRAW;
}

static void func_ov172_021bae58(CommTvtWork *sys, CtvtDraw *draw) {
    if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) && draw->menuWindowPending == FALSE) {
        if (func_0204b98c(draw->buttons[5]) == FALSE || func_0204b898(draw->buttons[5]) == 1) {
            draw->state = DRAW_STATE_DRAW;
            draw->slideTarget = 0;
            func_ov172_021bb65c(sys, draw, TRUE);
            func_0204b878(draw->buttons[5], 1);
            GFL_SndSEPlay(SEQ_SE_SYS_45);
        }
    } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        draw->state = DRAW_STATE_FADE_OUT;
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
    }
    if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_SELECT | PAD_BUTTON_START)) {
        func_ov172_021be0b8(sys);
        func_ov172_021bb54c(sys, draw);
        GFL_SndSEPlay(SEQ_SE_SYS_48);
    }
}

// The animations of each tool's button, when not chosen and when chosen
static const u8 data_ov172_021c6fb2[] = { 1, 2 };
static const u8 data_ov172_021c6fb4[] = { 3, 4 };
static const u8 data_ov172_021c6fb6[] = { 5, 6 };
static const u8 data_ov172_021c6fb8[] = { 7, 8 };

// pokebw2's sPens
static const int data_ov172_021c6fbc[] = { DRAW_PEN_ROUND, DRAW_PEN_SMALL, DRAW_PEN_DOT };

// pokebw2's sStamps
static const int data_ov172_021c6fc8[] = { DRAW_PEN_HEART, DRAW_PEN_POKE_BALL, DRAW_PEN_FACE, DRAW_PEN_STAR, DRAW_PEN_DROP };

// pokebw2's sStampButtons
static const TouchRect data_ov172_021c6fdc[CTVT_DRAW_TOUCH_MAX] = {
    { 0x98, 0xa8, 0x60, 0x70 }, { 0x98, 0xa8, 0x70, 0x80 }, { 0x98, 0xa8, 0x80, 0x90 },
    { 0x98, 0xa8, 0x90, 0xa0 }, { 0x98, 0xa8, 0xa0, 0xb0 }, { TOUCH_RECT_END },
};

// pokebw2's sToolButtons
static const TouchRect data_ov172_021c6ff8[CTVT_DRAW_TOUCH_MAX] = {
    { 0xa8, 0xc0, 0x04, 0x1c }, { 0xa8, 0xc0, 0x2c, 0x44 }, { 0xa8, 0xc0, 0x54, 0x6c }, { 0xa8, 0xc0, 0x7c, 0x94 },
    { 0xa8, 0xc0, 0xbc, 0xd4 }, { 0xa8, 0xc0, 0xe0, 0xf8 }, { TOUCH_RECT_END },
};

// pokebw2's sPenButtons
static const TouchRect data_ov172_021c7014[CTVT_DRAW_TOUCH_MAX] = {
    { 0x98, 0xa8, 0x08, 0x18 },
    { 0x98, 0xa8, 0x18, 0x28 },
    { 0x98, 0xa8, 0x28, 0x38 },
    { TOUCH_RECT_END },
};

// pokebw2's CtvtDraw_UpdateStroke
static void func_ov172_021baee0(CommTvtWork *sys, CtvtDraw *draw) {
    s32 hit = func_02035c08(data_ov172_021c6ff8);
    s32 touch;

    if (hit == DRAW_TOOL_PEN && func_0204b3f4(draw->menuCursor) == FALSE) {
        draw->tool = DRAW_TOOL_PEN;
        draw->stampChosen = FALSE;
        func_ov172_021bb54c(sys, draw);
        draw->inputHandled = TRUE;
        draw->penMenuOpen = TRUE;
        func_0204b3dc(draw->penMenu, TRUE);
        func_0204b92c(draw->penMenu, TRUE);
        GFL_SndSEPlay(SEQ_SE_SYS_45);
    }
    if (hit == DRAW_TOOL_PICK) {
        draw->tool = DRAW_TOOL_PICK;
        func_ov172_021bb54c(sys, draw);
        draw->inputHandled = TRUE;
        GFL_SndSEPlay(SEQ_SE_SYS_45);
    }
    if (hit == DRAW_TOOL_ERASER) {
        draw->tool = DRAW_TOOL_ERASER;
        func_ov172_021bb54c(sys, draw);
        draw->inputHandled = TRUE;
        GFL_SndSEPlay(SEQ_SE_SYS_45);
    }
    if (hit == DRAW_TOOL_STAMP && func_0204b3f4(draw->menuCursor) == FALSE) {
        draw->tool = DRAW_TOOL_STAMP;
        draw->stampChosen = TRUE;
        func_ov172_021bb54c(sys, draw);
        draw->inputHandled = TRUE;
        draw->stampMenuOpen = TRUE;
        func_0204b3dc(draw->stampMenu, TRUE);
        func_0204b92c(draw->stampMenu, TRUE);
        GFL_SndSEPlay(SEQ_SE_SYS_45);
    }
    if (hit == 4) {
        func_ov172_021be0b8(sys);
        func_ov172_021bb54c(sys, draw);
        draw->inputHandled = TRUE;
        GFL_SndSEPlay(SEQ_SE_SYS_48);
    }

    if (draw->penMenuOpen == TRUE) {
        touch = func_02035c08(data_ov172_021c7014);
        if (touch != TOUCH_RECT_NONE) {
            draw->pen = data_ov172_021c6fbc[touch];
            draw->inputHandled = TRUE;
            ClActorPos pos = { 0x10, 0xa0 };

            func_0204b3dc(draw->menuCursor, TRUE);
            func_0204b9a0(draw->menuCursor);
            func_0204b92c(draw->menuCursor, TRUE);
            func_0204b878(draw->menuCursor, touch + 17);
            func_0204b404(draw->menuCursor, &pos, 0);
            func_0204b3dc(draw->selectFrame, FALSE);
            GFL_SndSEPlay(SEQ_SE_SYS_45);
        }
    }
    if (draw->stampMenuOpen == TRUE) {
        touch = func_02035c08(data_ov172_021c6fdc);
        if (touch != TOUCH_RECT_NONE) {
            draw->stamp = data_ov172_021c6fc8[touch];
            draw->inputHandled = TRUE;
            GFL_SndSEPlay(SEQ_SE_SYS_45);
            ClActorPos pos = { 0x88, 0xa0 };

            func_0204b3dc(draw->menuCursor, TRUE);
            func_0204b9a0(draw->menuCursor);
            func_0204b92c(draw->menuCursor, TRUE);
            func_0204b878(draw->menuCursor, touch + 20);
            func_0204b404(draw->menuCursor, &pos, 0);
            func_0204b3dc(draw->selectFrame, FALSE);
        }
    }

    if (((GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) || (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) || hit == 5)
        && draw->menuWindowPending == FALSE) {
        draw->state = DRAW_STATE_MENU;
        draw->slideTarget = CTVT_DRAW_SLIDE;
        draw->inputHandled = TRUE;
        func_ov172_021bb65c(sys, draw, FALSE);
        func_ov172_021bb9e4(sys, draw);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_0204b878(draw->buttons[5], 9);
    }
    if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_SELECT | PAD_BUTTON_START)) {
        func_ov172_021be0b8(sys);
        func_ov172_021bb54c(sys, draw);
        GFL_SndSEPlay(SEQ_SE_SYS_48);
    }
    if (func_0204b3f4(draw->menuCursor) == TRUE && func_0204b98c(draw->menuCursor) == FALSE) {
        func_ov172_021bb9e4(sys, draw);
    }
    if (draw->penMenuOpen == TRUE && func_0204b98c(draw->penMenu) == FALSE
        && func_0204b3f4(draw->selectFrame) == FALSE) {
        ClActorPos pos = { 0x10, 0xa0 };

        switch (draw->pen) {
        case DRAW_PEN_SMALL:
            pos.x += 16;
            break;
        case DRAW_PEN_DOT:
            pos.x += 32;
            break;
        case DRAW_PEN_ROUND:
            break;
        }
        func_0204b404(draw->selectFrame, &pos, 0);
        func_0204b3dc(draw->selectFrame, TRUE);
    }
    if (draw->stampMenuOpen == TRUE && func_0204b98c(draw->stampMenu) == FALSE
        && func_0204b3f4(draw->selectFrame) == FALSE) {
        ClActorPos pos = { 0x88, 0xa0 };

        switch (draw->stamp) {
        case DRAW_PEN_HEART:
            pos.x -= 32;
            break;
        case DRAW_PEN_POKE_BALL:
            pos.x -= 16;
            break;
        case DRAW_PEN_STAR:
            pos.x += 16;
            break;
        case DRAW_PEN_DROP:
            pos.x += 32;
            break;
        }
        func_0204b404(draw->selectFrame, &pos, 0);
        func_0204b3dc(draw->selectFrame, TRUE);
    }
}

static void func_ov172_021bb288(CommTvtWork *sys, CtvtDraw *draw) {
    u32 x, y;
    u8 memberCount;
    CtvtComm *comm;
    DrawSystem *drawSystem;
    DrawCommand *cmd;

    func_02035d20(&x, &y);
    if (func_ov172_021be0c8(sys) == TRUE) {
        return;
    }
    if (func_02035c38() == FALSE) {
        draw->stroking = FALSE;
    }
    if (func_02035c60() == TRUE && draw->inputHandled == FALSE && draw->state == DRAW_STATE_DRAW && y >= 168) {
        if (draw->tool == DRAW_TOOL_PICK) {
            if (draw->stampChosen == FALSE) {
                draw->tool = DRAW_TOOL_PEN;
            } else {
                draw->tool = DRAW_TOOL_STAMP;
            }
            func_ov172_021bb54c(sys, draw);
        }
        func_ov172_021bb9e4(sys, draw);
        return;
    }
    if ((draw->state == DRAW_STATE_MENU || draw->state == DRAW_STATE_DRAW) && func_02035c60() == TRUE &&
        draw->inputHandled == FALSE && draw->tool == DRAW_TOOL_STAMP) {
        BOOL full;

        memberCount = func_ov172_021be054(sys);
        comm = func_ov172_021bdfe4(sys);
        drawSystem = func_ov172_021bdffc(sys);
        cmd = func_ov172_021b8bb8(sys, comm, &full);
        if (full == FALSE) {
            cmd->x0 = x;
            cmd->y0 = y;
            cmd->x1 = x;
            cmd->y1 = y;
            cmd->pen = draw->stamp;
            cmd->color = draw->color;
            if (memberCount == 1) {
                func_ov172_021b6e88(drawSystem, cmd);
            } else {
                func_ov172_021b8bfc(sys, comm);
            }
        }
    }
    if (func_02035c60() == TRUE && draw->inputHandled == FALSE) {
        if (draw->tool == DRAW_TOOL_PICK) {
            u16 *screen = (u16 *)(y * 512 + (x * 2 + (uintptr_t)G2_GetBG2ScrPtr()));
            u16 *camera;

            G2_GetBG3ScrPtr();
            if (func_ov172_021be078(sys) == TRUE) {
                camera = (u16 *)((y / 2) * 512 + ((x / 2) * 2 + (uintptr_t)G2_GetBG3ScrPtr()));
            } else {
                camera = (u16 *)(y * 512 + (x * 2 + (uintptr_t)G2_GetBG3ScrPtr()));
            }
            if (*screen & CTVT_DRAW_OPAQUE) {
                draw->color = *screen;
            } else if (*camera & CTVT_DRAW_OPAQUE) {
                draw->color = *camera;
            } else {
                draw->color = CTVT_DRAW_OPAQUE;
            }
            ((vu16 *)HW_OBJ_PLTT)[13] = draw->color & 0x7fff;
            if (draw->stampChosen == FALSE) {
                draw->tool = DRAW_TOOL_PEN;
            } else {
                draw->tool = DRAW_TOOL_STAMP;
            }
            func_ov172_021bb54c(sys, draw);
            GFL_SndSEPlay(SEQ_SE_SYS_46);
        } else {
            draw->stroking = TRUE;
            draw->lastX = 0xffff;
            draw->lastY = 0xffff;
            func_ov172_021bb9e4(sys, draw);
        }
    }
    if ((draw->tool == DRAW_TOOL_PEN || draw->tool == DRAW_TOOL_ERASER) && draw->stroking == TRUE) {
        u32 touchX, touchY;

        func_02035cb8(&touchX, &touchY);
        if (draw->stroking == TRUE) {
            if (draw->lastX != 0xffff && draw->lastY != 0xffff) {
                BOOL full;

                memberCount = func_ov172_021be054(sys);
                comm = func_ov172_021bdfe4(sys);
                drawSystem = func_ov172_021bdffc(sys);
                cmd = func_ov172_021b8bb8(sys, comm, &full);
                if (full == FALSE) {
                    cmd->x0 = draw->lastX;
                    cmd->y0 = draw->lastY;
                    cmd->x1 = touchX;
                    cmd->y1 = touchY;
                    cmd->pen = draw->pen;
                    if (draw->tool == DRAW_TOOL_ERASER) {
                        cmd->color = 0;
                        cmd->pen = DRAW_PEN_ROUND;
                    } else {
                        cmd->color = draw->color;
                    }
                    if (memberCount == 1) {
                        func_ov172_021b6e88(drawSystem, cmd);
                    } else {
                        func_ov172_021b8bfc(sys, comm);
                    }
                } else {
                    draw->stroking = FALSE;
                }
            }
            draw->lastX = touchX;
            draw->lastY = touchY;
        }
    }
}

// pokebw2's CtvtDraw_UpdateSlide
static void func_ov172_021bb4d0(CommTvtWork *sys, CtvtDraw *draw) {
    BOOL moved = FALSE;
    u8 i;

    if (func_0204b98c(draw->buttons[5]) == FALSE || func_0204b898(draw->buttons[5]) == 1) {
        if (draw->slideTarget < draw->slide) {
            draw->slide -= CTVT_DRAW_SLIDE;
            moved = TRUE;
        } else if (draw->slideTarget > draw->slide) {
            draw->slide += CTVT_DRAW_SLIDE;
            moved = TRUE;
        }
    }
    if (moved == TRUE) {
        GFL_BGSysMoveBGReq(0, 3, CTVT_DRAW_SLIDE - draw->slide);
        for (i = 0; i < CTVT_DRAW_BUTTONS; i++) {
            ClActorPos pos;

            func_0204b454(draw->buttons[i], &pos, 0);
            pos.y = draw->slide + 180;
            if (i == 5) {
                pos.y -= 12;
            }
            func_0204b404(draw->buttons[i], &pos, 0);
        }
    }
}

// pokebw2's CtvtDraw_RefreshButtons
static void func_ov172_021bb54c(CommTvtWork *sys, CtvtDraw *draw) {
    func_0204b878(draw->buttons[0], data_ov172_021c6fb2[draw->tool == DRAW_TOOL_PEN ? 1 : 0]);
    func_0204b878(draw->buttons[1], data_ov172_021c6fb4[draw->tool == DRAW_TOOL_PICK ? 1 : 0]);
    func_0204b878(draw->buttons[2], data_ov172_021c6fb6[draw->tool == DRAW_TOOL_ERASER ? 1 : 0]);
    func_0204b878(draw->buttons[3], data_ov172_021c6fb8[draw->tool == DRAW_TOOL_STAMP ? 1 : 0]);
    if (func_ov172_021be0b4(sys) == TRUE) {
        func_0204b878(draw->buttons[4], 12);
    } else {
        func_0204b878(draw->buttons[4], 10);
    }
    if (draw->tool != DRAW_TOOL_PEN && draw->penMenuOpen == TRUE) {
        draw->penMenuOpen = FALSE;
        func_0204b3dc(draw->penMenu, FALSE);
        func_0204b9a0(draw->penMenu);
        func_0204b8c4(draw->penMenu, 0);
        func_0204b92c(draw->penMenu, FALSE);
        func_0204b3dc(draw->selectFrame, FALSE);
    }
    if (draw->tool != DRAW_TOOL_STAMP && draw->stampMenuOpen == TRUE) {
        draw->stampMenuOpen = FALSE;
        func_0204b3dc(draw->stampMenu, FALSE);
        func_0204b9a0(draw->stampMenu);
        func_0204b8c4(draw->stampMenu, 0);
        func_0204b92c(draw->stampMenu, FALSE);
        func_0204b3dc(draw->selectFrame, FALSE);
    }
}

static inline void CtvtDraw_PrintMenuItem(PrintQueue *queue, CtvtDraw *draw, MsgData *msgData, u32 msgId, int x, int y,
                                          Font *font) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(msgData, msgId);

    func_0201c724(queue, BmpWin_GetBitmap(draw->menuWindow), x, y, str, font, 0x440);
    GFL_StrBufFree(str);
}

// pokebw2's CtvtDraw_PrintMenu
static void func_ov172_021bb65c(CommTvtWork *sys, CtvtDraw *draw, BOOL expanded) {
    Font *font = func_ov172_021be034(sys);
    MsgData *msgData = func_ov172_021be03c(sys);
    PrintQueue *queue = func_ov172_021be044(sys);
    BmpWin *oldWindow = draw->menuWindow;

    if (oldWindow != NULL) {
        func_0201c6ec(queue);
        func_0201f7a8(draw->menuWindow, 2);
        func_020456f8(draw->menuWindow);
    }
    draw->menuExpanded = expanded;
    if (expanded == TRUE) {
        draw->menuWindow = BmpWin_CreateDynamic(4, 2, 8, 28, 13, 10, TRUE);
        GFL_BitmapFill(BmpWin_GetBitmap(draw->menuWindow), 15);
        BmpWin_FlushChar(draw->menuWindow);
        CtvtDraw_PrintMenuItem(queue, draw, msgData, 6, 64, 4, font);
        CtvtDraw_PrintMenuItem(queue, draw, msgData, 7, 64, 24, font);
        CtvtDraw_PrintMenuItem(queue, draw, msgData, 8, 64, 44, font);
        CtvtDraw_PrintMenuItem(queue, draw, msgData, 10, 64, 64, font);
        CtvtDraw_PrintMenuItem(queue, draw, msgData, 9, 64, 84, font);
    } else {
        draw->menuWindow = BmpWin_CreateDynamic(4, 2, 16, 28, 6, 10, TRUE);
        GFL_BitmapFill(BmpWin_GetBitmap(draw->menuWindow), 15);
        BmpWin_FlushChar(draw->menuWindow);
        CtvtDraw_PrintMenuItem(queue, draw, msgData, 0, 8, 0, font);
        CtvtDraw_PrintMenuItem(queue, draw, msgData, 1, 8, 16, font);
        CtvtDraw_PrintMenuItem(queue, draw, msgData, 2, 8, 32, font);
        CtvtDraw_PrintMenuItem(queue, draw, msgData, 3, 80, 0, font);
        CtvtDraw_PrintMenuItem(queue, draw, msgData, 4, 80, 16, font);
        CtvtDraw_PrintMenuItem(queue, draw, msgData, 5, 80, 32, font);
    }
    draw->menuWindowPending = TRUE;
    if (oldWindow != NULL) {
        BmpWin_Free(oldWindow);
    }
}

// pokebw2's CtvtDraw_CheckDisconnect
static BOOL func_ov172_021bb8f4(CommTvtWork *sys, CtvtDraw *draw) {
    if (func_ov172_021be0d0(sys) == TRUE) {
        if (func_ov172_021b80e4(sys, func_ov172_021bdfe4(sys)) == CTVT_CONNECT_EXISTING) {
            draw->state = DRAW_STATE_PARENT_LEFT;
        } else {
            draw->state = DRAW_STATE_DISCONNECTED;
        }
        return TRUE;
    }
    if (func_ov172_021be054(sys) <= 1) {
        draw->state = DRAW_STATE_ALONE;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's CtvtDraw_PrintMessage
static void func_ov172_021bb938(CommTvtWork *sys, CtvtDraw *draw, u32 msgId) {
    Font *font = func_ov172_021be034(sys);
    MsgData *msgData = func_ov172_021be03c(sys);
    PrintQueue *queue = func_ov172_021be044(sys);
    StrBuf *str;
    u8 y;

    if (draw->messageWindow != NULL) {
        BmpWin_Free(draw->messageWindow);
    }
    y = 4;
    if (draw->slideTarget != 0) {
        y = 1;
    }
    draw->messageWindow = BmpWin_CreateDynamic(0, 1, y, 30, 4, 10, TRUE);
    func_0201c6ec(queue);
    GFL_BitmapFill(BmpWin_GetBitmap(draw->messageWindow), 15);
    str = GFL_MsgDataLoadStrbufNew(msgData, msgId);
    func_0201c724(queue, BmpWin_GetBitmap(draw->messageWindow), 0, 0, str, font, 0x440);
    GFL_StrBufFree(str);
    BmpWin_FlushChar(draw->messageWindow);
    BmpWin_FlushMap(draw->messageWindow);
    BmpWin_DrawFrame(draw->messageWindow, 1, 0x200, 9);
    draw->messageWindowPending = TRUE;
}

// pokebw2's CtvtDraw_CloseMenus
static void func_ov172_021bb9e4(CommTvtWork *sys, CtvtDraw *draw) {
    draw->penMenuOpen = FALSE;
    draw->stampMenuOpen = FALSE;
    func_0204b9a0(draw->penMenu);
    func_0204b9a0(draw->stampMenu);
    func_0204b9a0(draw->menuCursor);
    func_0204b8c4(draw->penMenu, 0);
    func_0204b8c4(draw->stampMenu, 0);
    func_0204b92c(draw->penMenu, FALSE);
    func_0204b92c(draw->stampMenu, FALSE);
    func_0204b3dc(draw->penMenu, FALSE);
    func_0204b3dc(draw->stampMenu, FALSE);
    func_0204b3dc(draw->menuCursor, FALSE);
    func_0204b3dc(draw->selectFrame, FALSE);
}
