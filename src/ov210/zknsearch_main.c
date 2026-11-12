// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov300/zknsearch_main.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/zknsearch_bmp.h"
#include "app/zknsearch_main.h"
#include "app/zknsearch_obj.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/math.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "system/app_menu_common.h"
#include "system/bgwinfrm.h"
#include "system/blink_palanm.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/0479.h"
#include "app/zukansearch.h"

// The Pokédex's search screen: its BGs and graphics, its fades and the touch bar that slides in

// The frame of the touch bar: where it is in BG 0's screen and how far it slides
#define TOUCHBAR_Y 21
#define TOUCHBAR_HEIGHT 3
#define TOUCHBAR_SLIDE 48

// The screen of the search's progress bar
#define LOADING_Y 17
#define LOADING_HEIGHT 6

static void func_ov210_021bc2b8(TCB *tcb, void *data);
static void func_ov210_021bc33c(TCB *tcb, void *data);

// pokebw2's sZknSearchVRAMConfig
static const BGSysVRAMConfig data_ov210_021c0754 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// The messages of the types, and of none
// pokebw2's sZknSearchTypeMsgs
static const u16 data_ov210_021c072e[ZKNSEARCH_TYPE_COUNT + 1] = {
    Bank0479_Text_042, Bank0479_Text_034, Bank0479_Text_044,   Bank0479_Text_040,  Bank0479_Text_038,
    Bank0479_Text_032,   Bank0479_Text_047,      Bank0479_Text_037,    Bank0479_Text_043,   Bank0479_Text_045,
    Bank0479_Text_046,  Bank0479_Text_035,    Bank0479_Text_039, Bank0479_Text_033, Bank0479_Text_036,
    Bank0479_Text_041, Bank0479_Text_031,     Bank0479_Text_048,
};

// The shapes in the order of the list
// pokebw2's sZknSearchShapeOrder
static const u8 data_ov210_021c0720[ZKNSEARCH_SHAPE_COUNT] = { 0, 13, 1, 7, 5, 12, 8, 4, 6, 3, 2, 9, 11, 10 };

// pokebw2's ZknSearchMain_InitVBlank
void func_ov210_021bc294(ZknSearchWork *wk) {
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov210_021bc2b8, wk, 0);
}

// pokebw2's ZknSearchMain_ExitVBlank
void func_ov210_021bc2ac(ZknSearchWork *wk) {
    GFL_TCBRemove(wk->vblankTask);
}

// pokebw2's ZknSearchMain_VBlankTask
static void func_ov210_021bc2b8(TCB *tcb, void *data) {
    ZknSearchWork *wk = data;

    GFL_BGSysUpdate();
    func_0204a648();
    func_02021d50(wk->paletteFade);
    if (wk->bgRequest == ZKNSEARCH_BG_REQUEST_ON) {
        GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG1 | GX_PLANEMASK_BG3, TRUE);
        GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG1, TRUE);
        wk->bgRequest = 0;
    } else if (wk->bgRequest == ZKNSEARCH_BG_REQUEST_OFF) {
        GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG1 | GX_PLANEMASK_BG3, FALSE);
        GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG1, FALSE);
        wk->bgRequest = 0;
    }
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

// pokebw2's ZknSearchMain_InitHBlank
void func_ov210_021bc318(ZknSearchWork *wk) {
    wk->hblankTask = GFL_HBlankTCBAdd(func_ov210_021bc33c, wk, 0);
}

// pokebw2's ZknSearchMain_ExitHBlank
void func_ov210_021bc330(ZknSearchWork *wk) {
    GFL_TCBRemove(wk->hblankTask);
}

// The blend of the top screen's list changes down the screen, by the height of the list's rows
// pokebw2's ZknSearchMain_HBlankTask
static void func_ov210_021bc33c(TCB *tcb, void *data) {
    ZknSearchWork *wk = data;
    s32 vcount = GX_GetVCount();

    if (wk->page == ZKNSEARCH_PAGE_SHAPE) {
        if (vcount >= 152) {
            G2S_ChangeBlendAlpha(11, 5);
        } else if (vcount >= 112) {
            G2S_ChangeBlendAlpha(10, 6);
        } else if (vcount >= 72) {
            G2S_ChangeBlendAlpha(9, 7);
        } else if (vcount >= 32) {
            G2S_ChangeBlendAlpha(8, 8);
        } else {
            G2S_ChangeBlendAlpha(6, 10);
        }
    } else {
        if (vcount >= 168) {
            G2S_ChangeBlendAlpha(11, 5);
        } else if (vcount >= 144) {
            G2S_ChangeBlendAlpha(10, 6);
        } else if (vcount >= 120) {
            G2S_ChangeBlendAlpha(9, 7);
        } else if (vcount >= 96) {
            G2S_ChangeBlendAlpha(8, 8);
        } else if (vcount >= 72) {
            G2S_ChangeBlendAlpha(6, 10);
        } else {
            G2S_ChangeBlendAlpha(4, 12);
        }
    }
}

// pokebw2's ZknSearchMain_InitVRAM
void func_ov210_021bc3d4(void) {
    GFL_BGSysInitVRAM(0);
    GFL_BGSysSetVRAMBanks(&data_ov210_021c0754);
}

// pokebw2's ZknSearchMain_GetVRAMConfig
const BGSysVRAMConfig *func_ov210_021bc3e8(void) {
    return &data_ov210_021c0754;
}

// pokebw2's ZknSearchMain_InitBG
void func_ov210_021bc3f0(void) {
    GFL_BGSysCreate(HEAPID_ZUKAN_SEARCH);
    {
        BGSysLCDConfig config = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

        GFL_BGSysSetLCDConfig(&config);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf000,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            0,
            0,
        };

        GFL_BGSysCreateBG(0, &setup, BGMODE_TEXT);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe800,
            GX_BG_CHARBASE_0x10000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            0,
            0,
        };

        GFL_BGSysCreateBG(1, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(1);
        GFL_BGSysQueueScrLoad(1);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe000,
            GX_BG_CHARBASE_0x08000,
            0x4000,
            GX_BG_EXTPLTT_01,
            3,
            0,
            0,
        };

        GFL_BGSysCreateBG(2, &setup, BGMODE_TEXT);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf800,
            GX_BG_CHARBASE_0x18000,
            0x8000,
            GX_BG_EXTPLTT_01,
            2,
            0,
            0,
        };

        GFL_BGSysCreateBG(3, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(3);
        GFL_BGSysQueueScrLoad(3);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf000,
            GX_BG_CHARBASE_0x18000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            0,
            0,
        };

        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe800,
            GX_BG_CHARBASE_0x10000,
            0x8000,
            GX_BG_EXTPLTT_01,
            2,
            0,
            0,
        };

        GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(5);
        GFL_BGSysQueueScrLoad(5);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe000,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            3,
            0,
            0,
        };

        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf800,
            GX_BG_CHARBASE_0x10000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            0,
            0,
        };

        GFL_BGSysCreateBG(7, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(7);
        GFL_BGSysQueueScrLoad(7);
    }
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, TRUE);
}

// pokebw2's ZknSearchMain_ExitBG
void func_ov210_021bc578(void) {
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, FALSE);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(0);
    GFL_BGSysFree();
}

// pokebw2's ZknSearchMain_SetMainBGPriority
void func_ov210_021bc5b4(void) {
    GFL_BGSysSetBGPriority(0, 1);
    GFL_BGSysSetBGPriority(1, 0);
}

// pokebw2's ZknSearchMain_SetListBGPriority
void func_ov210_021bc5c8(void) {
    GFL_BGSysSetBGPriority(0, 0);
    GFL_BGSysSetBGPriority(1, 1);
}

// pokebw2's ZknSearchMain_LoadGraphics
void func_ov210_021bc5dc(void) {
    ArcTool *arc;
    u32 chars;
    u16 *screen;
    u32 i;

    arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, HEAPID_ZUKAN_SEARCH | HEAPID_TAIL_BIT);
    GFL_G2DIOLoadArcNCLRDefault(arc, 73, PALTYPE_MAIN_BG, 0, 0xc0, HEAPID_ZUKAN_SEARCH);
    GFL_G2DIOLoadArcNCLRDefault(arc, 74, PALTYPE_SUB_BG, 0, 0xa0, HEAPID_ZUKAN_SEARCH);
    GFL_BGSysLoadArcNCGRDynamic(arc, 81, 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    GFL_BGSysLoadArcNCGRStatic(arc, 81, 1, 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    GFL_BGSysLoadArcNCGRStatic(arc, 81, 3, 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    GFL_BGSysLoadArcNCGRStatic(arc, 79, 2, 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    GFL_BGSysLoadArcNCGRStatic(arc, 81, 5, 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    GFL_BGSysLoadArcNCGRStatic(arc, 80, 6, 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    GFL_BGSysLoadArcNCGRStatic(arc, 80, 4, 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 95, 2, 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 94, 4, 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 96, 6, 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    GFL_ArcToolFree(arc);

    // The touch bar, at the bottom of BG 0
    arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), HEAPID_ZUKAN_SEARCH | HEAPID_TAIL_BIT);
    chars = GFL_BGSysLoadArcNCGRDynamic(arc, func_02027628(), 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    GFL_G2DIOLoadArcNCLRDefault(arc, func_02027624(), PALTYPE_MAIN_BG, 14 * 0x20, 0x20, HEAPID_ZUKAN_SEARCH);
    loadBGScrToVramByFileNoReserveNegAlign(arc, func_0202762c(), 0, 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    GFL_BGSysSetScrPaletteNo(0, 0, TOUCHBAR_Y, 32, TOUCHBAR_HEIGHT, 14);
    GFL_BGSysQueueScrLoad(0);
    GFL_ArcToolFree(arc);
    screen = GFL_BGSysIsScrHeapExists(0);
    for (i = 0; i < 32 * TOUCHBAR_HEIGHT; i++) {
        screen[TOUCHBAR_Y * 32 + i] += chars;
    }
    GFL_BGSysQueueScrLoad(0);

    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_MAIN_BG, 15 * 0x20, 0x20, HEAPID_ZUKAN_SEARCH);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_SUB_BG, 15 * 0x20, 0x20, HEAPID_ZUKAN_SEARCH);
}

// pokebw2's ZknSearchMain_InitPaletteFade
void func_ov210_021bc790(ZknSearchWork *wk) {
    wk->paletteFade = func_02021518(HEAPID_ZUKAN_SEARCH);
    func_0202155c(wk->paletteFade, PALFADE_BUFFER_MAIN_BG, 0x200, HEAPID_ZUKAN_SEARCH);
    func_0202155c(wk->paletteFade, PALFADE_BUFFER_SUB_BG, 0x200, HEAPID_ZUKAN_SEARCH);
    func_0202155c(wk->paletteFade, PALFADE_BUFFER_MAIN_OBJ, 0x200, HEAPID_ZUKAN_SEARCH);
    func_0202155c(wk->paletteFade, PALFADE_BUFFER_SUB_OBJ, 0x200, HEAPID_ZUKAN_SEARCH);
    func_020216d4(wk->paletteFade, PALFADE_BUFFER_MAIN_BG, 0, 0x200);
    func_020216d4(wk->paletteFade, PALFADE_BUFFER_SUB_BG, 0, 0x200);
    func_020216d4(wk->paletteFade, PALFADE_BUFFER_MAIN_OBJ, 0, 0x200);
    func_020216d4(wk->paletteFade, PALFADE_BUFFER_SUB_OBJ, 0, 0x200);
}

// pokebw2's ZknSearchMain_ExitPaletteFade
void func_ov210_021bc800(ZknSearchWork *wk) {
    func_020215a0(wk->paletteFade, PALFADE_BUFFER_MAIN_BG);
    func_020215a0(wk->paletteFade, PALFADE_BUFFER_SUB_BG);
    func_020215a0(wk->paletteFade, PALFADE_BUFFER_MAIN_OBJ);
    func_020215a0(wk->paletteFade, PALFADE_BUFFER_SUB_OBJ);
    func_02021540(wk->paletteFade);
}

// Fades everything but the touch bar and its buttons, which slide instead
// pokebw2's ZknSearchMain_StartFade
void func_ov210_021bc82c(ZknSearchWork *wk, u8 start, u8 end) {
    func_0202173c(wk->paletteFade, 1 << PALFADE_BUFFER_MAIN_BG, 0xbfff, -14, start, end, GX_RGB(0, 0, 0),
                          GFL_VBlankGetTCBMgr());
    func_0202173c(wk->paletteFade, 1 << PALFADE_BUFFER_SUB_BG, 0x7fef, -14, start, end, GX_RGB(0, 0, 0),
                          GFL_VBlankGetTCBMgr());
    func_0202173c(wk->paletteFade, 1 << PALFADE_BUFFER_MAIN_OBJ, 0xfe1f, -14, start, end, GX_RGB(0, 0, 0),
                          GFL_VBlankGetTCBMgr());
    func_0202173c(wk->paletteFade, 1 << PALFADE_BUFFER_SUB_OBJ, 0xbfff, -14, start, end, GX_RGB(0, 0, 0),
                          GFL_VBlankGetTCBMgr());
}

// pokebw2's ZknSearchMain_SetBlend
void func_ov210_021bc8b4(BOOL enabled) {
    if (enabled == TRUE) {
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG3, GX_BLEND_PLANEMASK_BG2, 16, 6);
        G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2, 6, 10);
    } else {
        G2S_BlendNone();
    }
}

// The marks' windows on the top screen show the row under them unblended
// pokebw2's ZknSearchMain_InitWindow
void func_ov210_021bc8ec(void) {
    G2S_SetWndOBJInsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ,
                             TRUE);
    G2S_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 |
                               GX_WND_PLANEMASK_OBJ,
                           TRUE);
    GXS_SetVisibleWnd(GX_WNDMASK_OW);
}

// pokebw2's ZknSearchMain_ExitWindow
void func_ov210_021bc92c(void) {
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
}

// pokebw2's ZknSearchMain_InitMsg
void func_ov210_021bc940(ZknSearchWork *wk) {
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_0479, HEAPID_ZUKAN_SEARCH);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    wk->wordSet = GFL_WordSetSystemCreateDefault(HEAPID_ZUKAN_SEARCH);
    wk->printQueue = func_0201c440(HEAPID_ZUKAN_SEARCH);
    wk->strbuf = GFL_StrBufCreate(0x80, HEAPID_ZUKAN_SEARCH);
}

// pokebw2's ZknSearchMain_ExitMsg
void func_ov210_021bc99c(ZknSearchWork *wk) {
    GFL_StrBufFree(wk->strbuf);
    func_0201c4c0(wk->printQueue);
    GFL_WordSetSystemFree(wk->wordSet);
    GFL_FontFree(wk->font);
    GFL_MsgDataFree(wk->msgData);
}

// pokebw2's ZknSearchMain_InitBlinkAnm
void func_ov210_021bc9d0(ZknSearchWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, HEAPID_ZUKAN_SEARCH | HEAPID_TAIL_BIT);

    wk->blinkAnm = func_0202e940(3 * 0x10, 0x10, 1, HEAPID_ZUKAN_SEARCH);
    func_0202e9ac(wk->blinkAnm, arc, 73, 2 * 0x10, 3 * 0x10);
    GFL_ArcToolFree(arc);
}

// pokebw2's ZknSearchMain_ExitBlinkAnm
void func_ov210_021bca10(ZknSearchWork *wk) {
    func_0202ea20(wk->blinkAnm);
}

// pokebw2's ZknSearchMain_InitBGWinFrame
void func_ov210_021bca20(ZknSearchWork *wk) {
    u16 *screen = GFL_BGSysIsScrHeapExists(0);

    wk->bgWinFrame = func_0202c9b4(2, 1, HEAPID_ZUKAN_SEARCH);
    func_0202ca3c(wk->bgWinFrame, 0, 0, 32, TOUCHBAR_HEIGHT);
    func_0202cac4(wk->bgWinFrame, 0, &screen[TOUCHBAR_Y * 32]);
    GFL_BGSysFillScrArea(0, 0, 0, TOUCHBAR_Y, 32, TOUCHBAR_HEIGHT, 0);
    GFL_BGSysQueueScrLoad(0);
}

// pokebw2's ZknSearchMain_ExitBGWinFrame
void func_ov210_021bca78(ZknSearchWork *wk) {
    func_0202ca0c(wk->bgWinFrame);
}

// pokebw2's ZknSearchMain_LoadMainScreen
void func_ov210_021bca84(ZknSearchWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, HEAPID_ZUKAN_SEARCH | HEAPID_TAIL_BIT);

    loadBGScrToVramByFileNoReserveNegAlign(arc, 93, 0, 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 92, 3, 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    GFL_ArcToolFree(arc);
}

// pokebw2's ZknSearchMain_LoadOrderScreen
void func_ov210_021bcac8(ZknSearchWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, HEAPID_ZUKAN_SEARCH | HEAPID_TAIL_BIT);

    loadBGScrToVramByFileNoReserveNegAlign(arc, 89, 3, 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    GFL_ArcToolFree(arc);
}

// pokebw2's ZknSearchMain_LoadInitialScreen
void func_ov210_021bcaf8(ZknSearchWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, HEAPID_ZUKAN_SEARCH | HEAPID_TAIL_BIT);

    loadBGScrToVramByFileNoReserveNegAlign(arc, 88, 3, 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    GFL_ArcToolFree(arc);
}

// pokebw2's ZknSearchMain_LoadTypeScreen
void func_ov210_021bcb28(ZknSearchWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, HEAPID_ZUKAN_SEARCH | HEAPID_TAIL_BIT);

    loadBGScrToVramByFileNoReserveNegAlign(arc, 90, 3, 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    GFL_ArcToolFree(arc);
}

// pokebw2's ZknSearchMain_LoadColorScreen
void func_ov210_021bcb58(ZknSearchWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, HEAPID_ZUKAN_SEARCH | HEAPID_TAIL_BIT);

    loadBGScrToVramByFileNoReserveNegAlign(arc, 86, 3, 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    GFL_ArcToolFree(arc);
}

// pokebw2's ZknSearchMain_LoadShapeScreen
void func_ov210_021bcb88(ZknSearchWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, HEAPID_ZUKAN_SEARCH | HEAPID_TAIL_BIT);

    loadBGScrToVramByFileNoReserveNegAlign(arc, 87, 3, 0, 0, FALSE, HEAPID_ZUKAN_SEARCH);
    GFL_ArcToolFree(arc);
}

// pokebw2's ZknSearchMain_LoadLoadingScreen
void func_ov210_021bcbb8(ZknSearchWork *wk) {
    ArcTool *arc;
    NNSG2dScreenData *screen;
    void *file;

    wk->loadingScreen =
        GFL_HeapAllocate(HEAPID_ZUKAN_SEARCH, 32 * LOADING_HEIGHT * sizeof(u16), FALSE, "zknsearch_main.c", 944);
    arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, HEAPID_ZUKAN_SEARCH | HEAPID_TAIL_BIT);
    file = GFL_G2DIOReadNSCRArc(arc, 91, FALSE, &screen, HEAPID_ZUKAN_SEARCH | HEAPID_TAIL_BIT);
    MIi_CpuCopy16(screen->rawData, wk->loadingScreen, 32 * LOADING_HEIGHT * sizeof(u16));
    GFL_HeapFree(file);
    GFL_ArcToolFree(arc);
}

// pokebw2's ZknSearchMain_FreeLoadingScreen
void func_ov210_021bcc1c(ZknSearchWork *wk) {
    GFL_HeapFree(wk->loadingScreen);
}

// pokebw2's ZknSearchMain_ShowLoading
void func_ov210_021bcc2c(ZknSearchWork *wk) {
    GFL_BGSysLoadScrAreaAll(1, wk->loadingScreen, 0, LOADING_Y, 32, LOADING_HEIGHT);
    GFL_BGSysQueueScrLoad(1);
    func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_RETURN, 15);
    func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_EXIT, 14);
    if (GameData_IsShortcutRegistered(wk->param->gameData, SHORTCUT_POKEDEX_SEARCH) == TRUE) {
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_SHORTCUT, 22);
    } else {
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_SHORTCUT, 21);
    }
    func_ov210_021bee80(wk, ZKNSEARCH_OBJ_RETURN, 1);
    func_ov210_021bee80(wk, ZKNSEARCH_OBJ_EXIT, 1);
    func_ov210_021bee80(wk, ZKNSEARCH_OBJ_SHORTCUT, 1);
}

// pokebw2's ZknSearchMain_HideLoading
void func_ov210_021bcca8(ZknSearchWork *wk) {
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_LOADING_BAR, FALSE);
    GFL_BGSysFillScrArea(1, 0, 0, LOADING_Y, 32, LOADING_HEIGHT, 0);
    GFL_BGSysQueueScrLoad(1);
    func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_RETURN, 1);
    func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_EXIT, 0);
    if (GameData_IsShortcutRegistered(wk->param->gameData, SHORTCUT_POKEDEX_SEARCH) == TRUE) {
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_SHORTCUT, 7);
    } else {
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_SHORTCUT, 6);
    }
    func_ov210_021bee80(wk, ZKNSEARCH_OBJ_RETURN, 0);
    func_ov210_021bee80(wk, ZKNSEARCH_OBJ_EXIT, 0);
    func_ov210_021bee80(wk, ZKNSEARCH_OBJ_SHORTCUT, 0);
    func_ov210_021bed1c(wk);
}

// pokebw2's ZknSearchMain_RequestBGOn
void func_ov210_021bcd30(ZknSearchWork *wk) {
    wk->bgRequest = ZKNSEARCH_BG_REQUEST_ON;
}

// pokebw2's ZknSearchMain_ClearPage
void func_ov210_021bcd3c(ZknSearchWork *wk) {
    wk->bgRequest = ZKNSEARCH_BG_REQUEST_OFF;
    GFL_BGSysClearScr(1);
    GFL_BGSysClearScr(3);
    GFL_BGSysClearScr(5);
    GFL_BGSysQueueScrLoad(1);
    GFL_BGSysQueueScrLoad(3);
    GFL_BGSysQueueScrLoad(5);
    GFL_BGSysFillScrArea(0, 0, 0, 0, 32, TOUCHBAR_Y, 0);
    GFL_BGSysQueueScrLoad(0);
    GFL_BGSysMoveBGReq(1, BG_MOVE_SET_Y, 0);
    GFL_BGSysMoveBGReq(5, BG_MOVE_SET_Y, 0);
}

// pokebw2's ZknSearchMain_InitFramePos
void func_ov210_021bcda4(ZknSearchWork *wk) {
    u32 i;
    s16 x, y;

    GFL_BGSysMoveBGReq(4, BG_MOVE_SET_Y, TOUCHBAR_SLIDE);
    GFL_BGSysMoveBGReq(7, BG_MOVE_SET_Y, TOUCHBAR_SLIDE);
    func_0202cb4c(wk->bgWinFrame, 0, 0, TOUCHBAR_Y + TOUCHBAR_SLIDE / 8);
    func_0202cd94(wk->bgWinFrame);
    for (i = ZKNSEARCH_OBJ_RETURN; i <= ZKNSEARCH_OBJ_SHORTCUT; i++) {
        func_ov210_021bee58(wk, i, &x, &y, CLACT_SURFACE_MAIN);
        y += TOUCHBAR_SLIDE;
        func_ov210_021bee38(wk, i, x, y, CLACT_SURFACE_MAIN);
    }
}

// pokebw2's ZknSearchMain_StartFrameMove
void func_ov210_021bce0c(ZknSearchWork *wk, s16 speed) {
    wk->frameMoveLeft = TOUCHBAR_SLIDE;
    wk->frameMoveSpeed = speed;
    func_0202cd64(wk->bgWinFrame, 0, 0, speed / MATH_ABS(speed), TOUCHBAR_SLIDE / MATH_ABS(speed));
}

// pokebw2's ZknSearchMain_UpdateFrameMove
BOOL func_ov210_021bce58(ZknSearchWork *wk) {
    u32 i;
    s16 x, y;

    if (wk->frameMoveLeft == 0) {
        return FALSE;
    }
    func_0202cd94(wk->bgWinFrame);
    GFL_BGSysMoveBGReq(4, BG_MOVE_DOWN, wk->frameMoveSpeed);
    GFL_BGSysMoveBGReq(7, BG_MOVE_DOWN, wk->frameMoveSpeed);
    for (i = ZKNSEARCH_OBJ_RETURN; i <= ZKNSEARCH_OBJ_SHORTCUT; i++) {
        func_ov210_021bee58(wk, i, &x, &y, CLACT_SURFACE_MAIN);
        y += wk->frameMoveSpeed;
        func_ov210_021bee38(wk, i, x, y, CLACT_SURFACE_MAIN);
    }
    wk->frameMoveLeft -= MATH_ABS(wk->frameMoveSpeed);
    return TRUE;
}

// pokebw2's ZknSearchMain_ListPosToType
u32 func_ov210_021bceec(ZknSearchWork *wk, u32 pos) {
    return pos;
}

// pokebw2's ZknSearchMain_TypeToListPos
u32 func_ov210_021bcef0(ZknSearchWork *wk, u32 type) {
    return type;
}

// pokebw2's ZknSearchMain_GetListPosTypeMsg
u32 func_ov210_021bcef4(ZknSearchWork *wk, u32 pos) {
    return data_ov210_021c072e[pos];
}

// pokebw2's ZknSearchMain_GetTypeMsg
u32 func_ov210_021bcf00(ZknSearchWork *wk, u32 type) {
    return data_ov210_021c072e[type];
}

// pokebw2's ZknSearchMain_ShapeToListPos
void func_ov210_021bcf0c(ZknSearchWork *wk) {
    u32 i;

    if (wk->param->criteria->shape != ZKNSEARCH_NONE) {
        for (i = 0; i < ZKNSEARCH_SHAPE_COUNT; i++) {
            if (wk->param->criteria->shape == data_ov210_021c0720[i]) {
                wk->param->criteria->shape = i;
                return;
            }
        }
    }
}

// pokebw2's ZknSearchMain_ListPosToShape
void func_ov210_021bcf38(ZknSearchWork *wk) {
    ZukanSearchCriteria *criteria = wk->param->criteria;

    if (criteria->shape != ZKNSEARCH_NONE) {
        criteria->shape = data_ov210_021c0720[criteria->shape];
    }
}
