// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov200/cg_help.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/cg_help.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nnsys/gfd.h"
#include "save/player_info.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "text/system/0024.h"
#include "gfl/bmp.h"

#define PAGE_COUNT 12
// The last page the buttons turn to
#define LAST_PAGE (PAGE_COUNT - 1)
// The titles of the pages are the first messages of the bank and their texts the ones after
#define TITLE_MESSAGE(page) (Bank0024_Text_000 + (page))
#define TEXT_MESSAGE(page) (Bank0024_Text_012 + (page))

// The sub screen's BGs, which show the help
enum {
    BG_TEXT = 4,
    BG_MESSAGE = 5,
    BG_FRAME = 6,
    BG_SCROLLING = 7,
};

enum {
    STATE_WIPE_IN,
    STATE_WAIT_WIPE_IN,
    STATE_CLOSE,
    STATE_WAIT_WIPE_OUT,
    STATE_PAGES,
};

// The touch screen's buttons
enum {
    BUTTON_PREVIOUS,
    BUTTON_NEXT,
    BUTTON_QUIT,
};

// The pages up to this one show a picture on the sub screen's BG
#define LAST_PICTURE_PAGE 7
#define PICTURE_PAGE_COUNT (LAST_PICTURE_PAGE + 1)
// The pages whose animation is shown without its first frame
#define PAGE_ANIMATION_PAUSED 10
#define PAGE_ANIMATION_HIDDEN 11

typedef struct {
    u16 heapId;
    CGHelpParam *param;
    BOOL netError;
    BOOL redraw;
    u32 state;
    TCB *vblankTask;
    u8 page;
    // Whether the page's text is still being printed
    BOOL printing;
    BmpWin *textWindow;
    BmpWin *titleWindow;
    Font *font;
    MsgData *msgData;
    PrintQueue *printQueue;
    AppTaskMenuRes *taskMenuRes;
    AppTaskMenuWin *previousButton;
    AppTaskMenuWin *nextButton;
    AppTaskMenuWin *quitButton;
    // The pages' palettes
    void *palRes;
    NNSG2dPaletteData *palData;
    ClActUnit *clActUnit;
    u32 palette;
    u32 chars;
    u32 cellAnims;
    ClActor *actor;
} CGHelp;

BOOL func_ov124_021d4b14(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov124_021d4bb4(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov124_021d4b84(GameProc *proc, u32 *state, void *param, void *work);
void func_ov124_021d4240(CGHelp *help);
void func_ov124_021d4280(CGHelp *help);
BOOL func_ov124_021d42a0(CGHelp *help);
void func_ov124_021d43f0(TCB *tcb, void *data);
void func_ov124_021d43f8(CGHelp *help);
void func_ov124_021d4504(CGHelp *help);
void func_ov124_021d4534(const BGSetup *setup, u8 bg, u8 mode);
static void func_ov124_021d4558(CGHelp *help);
void func_ov124_021d4698(CGHelp *help);
void func_ov124_021d46bc(CGHelp *help);
void func_ov124_021d485c(CGHelp *help);
void func_ov124_021d48a4(CGHelp *help);
void func_ov124_021d4940(CGHelp *help, u8 page);
void func_ov124_021d49c8(CGHelp *help, u8 page);

// pokebw2's CG_HELP_PROC_FUNCTIONS
GameProcFunctions data_ov124_021d4d40 = {
    func_ov124_021d4b14,
    func_ov124_021d4bb4,
    func_ov124_021d4b84,
};

// The palette of the picture of each page
static const u32 data_ov124_021d4ca4[PICTURE_PAGE_COUNT] = { 1, 1, 1, 1, 1, 1, 1, 1 };

// The animation sequence of each page
static const u32 data_ov124_021d4cf4[PAGE_COUNT] = {
#include "asset/cg_help_data_ov124_021d4cf4.inc"
};

// pokebw2's sLCDConfig
static const BGSysLCDConfig data_ov124_021d4bf4 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

// pokebw2's sBGSetupMessage
static const BGSetup data_ov124_021d4c64 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x6800,
    GX_BG_CHARBASE_0x00000,
    0x6000,
    GX_BG_EXTPLTT_23,
    1,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

// pokebw2's sBGSetupScrolling
static const BGSetup data_ov124_021d4c24 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x7800,
    GX_BG_CHARBASE_0x18000,
    0x8000,
    GX_BG_EXTPLTT_23,
    3,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

// The first tile of the picture of each page
static const u32 data_ov124_021d4c04[PICTURE_PAGE_COUNT] = { 12, 20, 20, 16, 16, 16, 16, 16 };

// pokebw2's sBGSetupText
static const BGSetup data_ov124_021d4c44 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x6000,
    GX_BG_CHARBASE_0x08000,
    0x8000,
    GX_BG_EXTPLTT_23,
    0,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

// pokebw2's sBGSetupFrame
static const BGSetup data_ov124_021d4c84 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x7000,
    GX_BG_CHARBASE_0x10000,
    0x8000,
    GX_BG_EXTPLTT_23,
    2,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

// pokebw2's sVRAMConfig
static const BGSysVRAMConfig data_ov124_021d4cc4 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,      GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's CGHelp_Setup
void func_ov124_021d4240(CGHelp *help) {
    help->state = STATE_WIPE_IN;
    func_ov124_021d43f8(help);
    func_ov124_021d4558(help);
    func_ov124_021d46bc(help);
    help->vblankTask = GFL_VBlankTCBAdd(func_ov124_021d43f0, help, 8);
    help->page = 0;
    help->redraw = TRUE;
    func_ov124_021d4940(help, help->page);
    func_0203d34c(FALSE, help->heapId);
}

// pokebw2's CGHelp_Cleanup
void func_ov124_021d4280(CGHelp *help) {
    GFL_TCBRemove(help->vblankTask);
    func_ov124_021d485c(help);
    func_ov124_021d4698(help);
    func_ov124_021d4504(help);
}

void func_02028454(AppTaskMenuWin *win);

BOOL func_ov124_021d42a0(CGHelp *help) {
    switch (help->state) {
    case STATE_WIPE_IN:
        GFL_WipeSet(WIPE_MODE_SUB, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, help->heapId);
        help->state = STATE_WAIT_WIPE_IN;
        break;
    case STATE_WAIT_WIPE_IN:
        if (GFL_WipeIsFinished() == TRUE) {
            help->state = STATE_PAGES;
        }
        break;
    case STATE_CLOSE:
        if (func_02028230(help->quitButton) == TRUE) {
            GFL_WipeSet(WIPE_MODE_SUB, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, help->heapId);
            help->state = STATE_WAIT_WIPE_OUT;
        }
        break;
    case STATE_WAIT_WIPE_OUT:
        if (GFL_WipeIsFinished() == TRUE) {
            return TRUE;
        }
        break;
    case STATE_PAGES:
        if (help->printing == FALSE) {
            func_ov124_021d48a4(help);
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) {
            GFL_BGSysQueueScrLoad(BG_TEXT);
        }
        break;
    }
    func_0201c4e4(help->printQueue);
    func_0204a600();
    if (help->nextButton != NULL) {
        func_02028178(help->nextButton);
    }
    if (help->previousButton != NULL) {
        func_02028178(help->previousButton);
    }
    func_02028178(help->quitButton);
    if (help->printing == TRUE && func_0201c6b4(help->printQueue) == TRUE) {
        if (func_02028230(help->nextButton) == TRUE || func_02028230(help->previousButton) == TRUE ||
            help->redraw == TRUE) {
            BmpWin_FlushChar(help->textWindow);
            BmpWin_FlushChar(help->titleWindow);
            help->printing = FALSE;
            help->redraw = FALSE;
            func_ov124_021d49c8(help, help->page);
            if (help->page == LAST_PAGE) {
                func_020282a4(help->nextButton);
                func_0202843c(help->nextButton);
            } else {
                func_02028454(help->nextButton);
                func_020282a4(help->nextButton);
            }
            if (help->page == 0) {
                func_020282a4(help->previousButton);
                func_0202843c(help->previousButton);
            } else {
                func_02028454(help->previousButton);
                func_020282a4(help->previousButton);
            }
        }
    }
    GFL_BGSysMoveBG(BG_SCROLLING, BG_MOVE_UP, 1);
    return FALSE;
}


// pokebw2's CGHelp_VBlank
void func_ov124_021d43f0(TCB *tcb, void *data) {
    func_0204a648();
}

// pokebw2's CGHelp_InitBGs
void func_ov124_021d43f8(CGHelp *help) {
    ClActSysSetup clActSetup;

    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    func_0202223c(0, 0);
    func_0202223c(1, 0);
    func_02022224(0);
    func_02022224(1);
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    GFL_BGSysSetVRAMBanks(&data_ov124_021d4cc4);
    GFL_BGSysCreate(help->heapId);
    BmpWin_InitAllocator(help->heapId);
    GFL_BGSysSetLCDConfig(&data_ov124_021d4bf4);
    func_ov124_021d4534(&data_ov124_021d4c44, BG_TEXT, BGMODE_TEXT);
    func_ov124_021d4534(&data_ov124_021d4c64, BG_MESSAGE, BGMODE_TEXT);
    func_ov124_021d4534(&data_ov124_021d4c84, BG_FRAME, BGMODE_TEXT);
    func_ov124_021d4534(&data_ov124_021d4c24, BG_SCROLLING, BGMODE_TEXT);
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG3, 15, 4);
    clActSetup = data_020a1448;
    clActSetup.charCount = 0x40;
    ClActSys_Create(&clActSetup, &data_ov124_021d4cc4, help->heapId);
    help->clActUnit = func_0204b100(0x30, 0, help->heapId);
    func_0204b270(help->clActUnit);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's CGHelp_FreeBGs
void func_ov124_021d4504(CGHelp *help) {
    func_0204b1cc(help->clActUnit);
    func_0204a5ac();
    GFL_BGSysReleaseBG(BG_TEXT);
    GFL_BGSysReleaseBG(BG_MESSAGE);
    GFL_BGSysReleaseBG(BG_FRAME);
    GFL_BGSysReleaseBG(BG_SCROLLING);
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
}

// pokebw2's CGHelp_CreateBG
void func_ov124_021d4534(const BGSetup *setup, u8 bg, u8 mode) {
    GFL_BGSysCreateBG(bg, setup, mode);
    GFL_BGSysSetBGEnabled(bg, TRUE);
    GFL_BGSysClearBG(bg);
    GFL_BGSysLoadScr(bg);
}

static void func_ov124_021d4558(CGHelp *help) {
    ArcTool *arc;
    ClActorSetup actorSetup;

    arc = GFL_ArcSysCreateFileHandle(ARCID_GSYNC_MENU, help->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, 4, 0, 0, help->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 4, BG_FRAME, 0, 0, FALSE, help->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 10, BG_FRAME, 0, 0, FALSE, help->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 4, BG_SCROLLING, 0, 0, FALSE, help->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 9, BG_SCROLLING, 0, 0, FALSE, help->heapId);
    GFL_ArcToolFree(arc);
    arc = GFL_ArcSysCreateFileHandle(ARCID_C_GEAR, help->heapId);
    if (getTrainerGender(help->param->playerInfo) == 0) {
        help->palette = func_0204ac18(arc, 19, CLACT_VRAM_SUB, 0, 0, 15, help->heapId);
    } else {
        help->palette = func_0204ac18(arc, 12, CLACT_VRAM_SUB, 0, 0, 15, help->heapId);
    }
    help->chars = func_0204a6c8(arc, 22, FALSE, CLACT_VRAM_SUB, help->heapId);
    help->cellAnims = func_0204af28(arc, 21, 20, help->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 0, 4, 0, 0, FALSE, help->heapId);
    help->palRes = GFL_G2DIOReadNCLRArc(arc, 13, &help->palData, help->heapId);
    GFL_ArcToolFree(arc);
    actorSetup.x = 32;
    actorSetup.y = 48;
    actorSetup.sequence = 0;
    actorSetup.priority = 0;
    actorSetup.bgPriority = 0;
    help->actor = func_0204b294(help->clActUnit, help->palette, help->chars, help->cellAnims, &actorSetup,
                                CLACT_SURFACE_SUB, help->heapId);
    func_0204b3dc(help->actor, FALSE);
    func_0204b92c(help->actor, TRUE);
}


// pokebw2's CGHelp_FreeGraphics
void func_ov124_021d4698(CGHelp *help) {
    GFL_HeapFree(help->palRes);
    func_0204b3b4(help->actor);
    func_0204ada4(help->palette);
    func_0204a8d4(help->chars);
    func_0204afd8(help->cellAnims);
}

// pokebw2's CGHelp_InitText
void func_ov124_021d46bc(CGHelp *help) {
    AppTaskMenuItem item;

    help->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, help->heapId);
    GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
    help->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_0024, help->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 15 * 32, 0x20, help->heapId);
    LoadSysMsgBox(BG_MESSAGE, 1, 13, 0, help->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_WINFRAME, GetSysMsgBoxPaletteDatID(0), 4, 13 * 32, 0x40, help->heapId);
    func_0201dd68();
    help->printQueue = func_0201c440(help->heapId);
    help->taskMenuRes = func_02027f60(BG_TEXT, 11, help->font, help->printQueue, help->heapId);
    help->titleWindow = BmpWin_CreateDynamic(BG_MESSAGE, 2, 4, 28, 4, 15, 1);
    help->textWindow = BmpWin_CreateDynamic(BG_MESSAGE, 2, 9, 28, 12, 15, 1);
    GFL_BitmapFill(BmpWin_GetBitmap(help->titleWindow), 1);
    BmpWin_FlushChar(help->titleWindow);
    BmpWin_FlushMap(help->titleWindow);
    BmpWin_DrawFrame(help->titleWindow, WINFRAME_TRANSFER_VBLANK, 1, 14);
    GFL_BitmapFill(BmpWin_GetBitmap(help->textWindow), 0);
    BmpWin_FlushChar(help->textWindow);
    BmpWin_FlushMap(help->textWindow);
    GFL_BGSysLoadScr(BG_MESSAGE);
    GFL_BGSysLoadScr(BG_TEXT);
    help->printing = FALSE;
    item.str = GFL_MsgDataLoadStrbufNew(help->msgData, Bank0024_Text_024);
    item.color = 0x39e3;
    item.type = 0;
    help->nextButton = func_02027fec(help->taskMenuRes, &item, 11, 21, 11, help->heapId);
    GFL_StrBufFree(item.str);
    item.str = GFL_MsgDataLoadStrbufNew(help->msgData, Bank0024_Text_026);
    help->previousButton = func_02027fec(help->taskMenuRes, &item, 0, 21, 11, help->heapId);
    GFL_StrBufFree(item.str);
    item.str = GFL_MsgDataLoadStrbufNew(help->msgData, Bank0024_Text_025);
    item.type = APP_TASKMENU_ITEM_RETURN;
    help->quitButton = func_02027fec(help->taskMenuRes, &item, 22, 21, 10, help->heapId);
    GFL_StrBufFree(item.str);
    func_0202843c(help->previousButton);
    *((vu16 *)HW_DB_BG_PLTT + 15 * 16 + 1) = 0;
}

// pokebw2's CGHelp_FreeText
void func_ov124_021d485c(CGHelp *help) {
    if (help->nextButton != NULL) {
        func_02028148(help->nextButton);
    }
    if (help->previousButton != NULL) {
        func_02028148(help->previousButton);
    }
    func_02028148(help->quitButton);
    func_02027fd8(help->taskMenuRes);
    func_0201c4c0(help->printQueue);
    BmpWin_Free(help->textWindow);
    BmpWin_Free(help->titleWindow);
    GFL_MsgDataFree(help->msgData);
    GFL_FontFree(help->font);
    func_0201dd68();
}


// pokebw2's CGHelp_HandleInput
void func_ov124_021d48a4(CGHelp *help) {
    const TouchRect rects[4] = {
        { 168, 192, 0, 88 },
        { 168, 192, 88, 176 },
        { 168, 192, 176, 255 },
        { TOUCH_RECT_END, 0, 0, 0 },
    };

    switch (func_02035c08(rects)) {
    case BUTTON_PREVIOUS:
        if (help->page != 0) {
            help->page--;
            func_ov124_021d4940(help, help->page);
            func_0202822c(help->previousButton, TRUE);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        }
        break;
    case BUTTON_NEXT:
        if (help->page < LAST_PAGE) {
            help->page++;
            func_ov124_021d4940(help, help->page);
            func_0202822c(help->nextButton, TRUE);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        }
        break;
    case BUTTON_QUIT:
        func_0202822c(help->quitButton, TRUE);
        help->state = STATE_CLOSE;
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        break;
    }
    if (func_02012204(help->param->gsys) == TRUE) {
        func_0202822c(help->quitButton, TRUE);
        help->state = STATE_CLOSE;
    }
}

// pokebw2's CGHelp_DrawPage
void func_ov124_021d4940(CGHelp *help, u8 page) {
    StrBuf *title;
    StrBuf *text;

    GFL_BitmapFill(BmpWin_GetBitmap(help->titleWindow), 1);
    GFL_BitmapFill(BmpWin_GetBitmap(help->textWindow), 0);
    title = GFL_MsgDataLoadStrbufNew(help->msgData, TITLE_MESSAGE(page));
    func_0201c724(help->printQueue, BmpWin_GetBitmap(help->titleWindow), 40, 8, title, help->font,
                  PRINT_COLOR(15, 2, 0));
    GFL_StrBufFree(title);
    text = GFL_MsgDataLoadStrbufNew(help->msgData, TEXT_MESSAGE(page));
    func_0201c724(help->printQueue, BmpWin_GetBitmap(help->textWindow), 0, 0, text, help->font, PRINT_COLOR(15, 2, 0));
    GFL_StrBufFree(text);
    help->printing = TRUE;
}

// pokebw2's CGHelp_ShowAnimation
void func_ov124_021d49c8(CGHelp *help, u8 page) {
    u8 x;
    u8 y;

    func_0204b878(help->actor, data_ov124_021d4cf4[page]);
    func_0204b3dc(help->actor, TRUE);
    if (page <= LAST_PICTURE_PAGE) {
        u16 (*tiles)[4];

        NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_SUB, 10 * 32,
                                           (u8 *)help->palData->pRawData + data_ov124_021d4ca4[page] * 32, 32);
        tiles = GFL_HeapAllocate(help->heapId, 32, TRUE, "cg_help.c", 684);
        for (y = 0; y < 4; y++) {
            for (x = 0; x < 4; x++) {
                tiles[y][x] = x + data_ov124_021d4c04[page] + y * 32 + (10 << 12);
            }
        }
        GFL_BGSysLoadScrAreaAll(BG_TEXT, tiles, 2, 4, 4, 4);
        GFL_HeapFree(tiles);
    } else {
        u16 *tiles;

        tiles = GFL_HeapAllocate(help->heapId, 32, TRUE, "cg_help.c", 699);
        GFL_BGSysLoadScrAreaAll(BG_TEXT, tiles, 2, 4, 4, 4);
        GFL_HeapFree(tiles);
    }
    GFL_BGSysLoadScr(BG_TEXT);
    {
        ClActorPos pos = { 32, 48 };

        func_0204b404(help->actor, &pos, CLACT_SURFACE_SUB);
    }
    if (page == PAGE_ANIMATION_PAUSED) {
        func_0204b92c(help->actor, FALSE);
        func_0204b908(help->actor, TRUE);
    } else if (page == PAGE_ANIMATION_HIDDEN) {
        func_0204b92c(help->actor, FALSE);
        func_0204b908(help->actor, FALSE);
    } else {
        func_0204b92c(help->actor, TRUE);
    }
}

BOOL func_ov124_021d4b14(GameProc *proc, u32 *state, void *param, void *work) {
    CGHelp *help;
    CGHelpParam *helpParam = param;

    GFL_HeapCreateChild(HEAPID_USER, 0x8a, 0x60000);
    help = GFL_ProcInitSubsystem(proc, sizeof(CGHelp), 0x8a);
    if (helpParam == NULL) {
        helpParam = GFL_HeapAllocate(0x8a, sizeof(CGHelpParam), TRUE, "cg_help.c", 755);
        helpParam->playerInfo = func_0200846c(0x8a);
        if (GCTX_HIDGetHeldKeys() & PAD_BUTTON_R) {
            setTrainerGender(helpParam->playerInfo, 1);
        } else {
            setTrainerGender(helpParam->playerInfo, 0);
        }
    }
    help->param = helpParam;
    help->heapId = 0x8a;
    func_ov124_021d4240(help);
    return TRUE;
}


BOOL func_ov124_021d4b84(GameProc *proc, u32 *state, void *param, void *work) {
    CGHelp *help = work;

    func_ov124_021d4280(help);
    if (param == NULL) {
        GFL_HeapFree(help->param->playerInfo);
        GFL_HeapFree(help->param);
    }
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(0x8a);
    return TRUE;
}


// pokebw2's CGHelp_Main
static BOOL func_ov124_021d4bb4(GameProc *proc, u32 *state, void *param, void *work) {
    CGHelp *help = work;
    BOOL done = func_ov124_021d42a0(help);

    if (func_0200fb24() && !help->netError) {
        help->netError = TRUE;
        help->state = STATE_CLOSE;
    }
    if (done == TRUE) {
        return TRUE;
    }
    return FALSE;
}
