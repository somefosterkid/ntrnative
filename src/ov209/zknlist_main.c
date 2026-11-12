// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov299/zknlist_main.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/ui/frame_list.h"
#include "app/zknlist_bmp.h"
#include "app/zknlist_main.h"
#include "app/zknlist_obj.h"
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
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/math.h"
#include "nitro/os.h"
#include "pml/personal.h"
#include "pml/species_names.h"
#include "save/pokedex.h"
#include "system/app_menu_common.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/game_start_3.h"
#include "app/zukanlist.h"

// The Pokédex's list: its BGs and graphics, its fades, the touch bar that slides in and the list of the species. The
// ROM doesn't name this file; zknlist_main.c is a guess after the search screen's zknsearch_main.c

// The frame of the touch bar: where it is in BG 1's screen and how far it slides
#define TOUCHBAR_Y 21
#define TOUCHBAR_HEIGHT 3
#define TOUCHBAR_SLIDE 48

static void func_ov209_021bc2bc(TCB *tcb, void *data);
static void func_ov209_021bc308(TCB *tcb, void *data);
static void func_ov209_021bcb90(void *work, u32 index, PrintWindow *window, s16 y, BOOL firstBG);
static void func_ov209_021bcbe0(void *work, u32 index, BOOL moved);
static void func_ov209_021bcc2c(void *work, s16 delta);

// pokebw2's sZknListVRAMConfig
static const BGSysVRAMConfig data_ov209_021be7d4 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// pokebw2's sZknListCallbacks
static const FrameListCallbacks data_ov209_021be7a0 = {
    func_ov209_021bcb90,
    func_ov209_021bcbe0,
    func_ov209_021bcc2c,
};

// The rows, the scroll bar and the page buttons
// pokebw2's sZknListTouch
static const FrameListTouch data_ov209_021be914[] = {
    { { 0, 23, 128, 231 }, FRAMELIST_TOUCH_ITEM },         { { 24, 47, 128, 231 }, FRAMELIST_TOUCH_ITEM },
    { { 48, 71, 128, 231 }, FRAMELIST_TOUCH_ITEM },        { { 72, 95, 128, 231 }, FRAMELIST_TOUCH_ITEM },
    { { 96, 119, 128, 231 }, FRAMELIST_TOUCH_ITEM },       { { 120, 143, 128, 231 }, FRAMELIST_TOUCH_ITEM },
    { { 144, 167, 128, 231 }, FRAMELIST_TOUCH_ITEM },      { { 8, 160, 232, 255 }, FRAMELIST_TOUCH_BAR },
    { { 168, 191, 136, 159 }, FRAMELIST_TOUCH_PAGE_UP },   { { 168, 191, 160, 183 }, FRAMELIST_TOUCH_PAGE_DOWN },
    { { TOUCH_RECT_END, 0, 0, 0 }, FRAMELIST_TOUCH_ITEM },
};

// The list's setup; its count and its work are set when it is created
// pokebw2's sZknListSetup
static const FrameListSetup data_ov209_021be7ac = {
    { 2, 6, 12, 0, 20, 3, 7, 0, 11, 3, 1, { 24, 12, 8, 6, 4, 3 }, 3, 8, 0 },
    0,
    3,
    0,
    ZKNLIST_ROW_COUNT,
    0,
    data_ov209_021be914,
    &data_ov209_021be7a0,
    NULL,
};

// pokebw2's ZknListMain_InitVBlank
void func_ov209_021bc298(ZknListWork *wk) {
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov209_021bc2bc, wk, 0);
}

// pokebw2's ZknListMain_ExitVBlank
void func_ov209_021bc2b0(ZknListWork *wk) {
    GFL_TCBRemove(wk->vblankTask);
}

// pokebw2's ZknListMain_VBlankTask
static void func_ov209_021bc2bc(TCB *tcb, void *data) {
    ZknListWork *wk = data;

    GFL_BGSysUpdate();
    func_0204a648();
    func_02021d50(wk->paletteFade);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

// pokebw2's ZknListMain_InitHBlank
void func_ov209_021bc2e4(ZknListWork *wk) {
    wk->hblankTask = GFL_HBlankTCBAdd(func_ov209_021bc308, wk, 0);
}

// pokebw2's ZknListMain_ExitHBlank
void func_ov209_021bc2fc(ZknListWork *wk) {
    GFL_TCBRemove(wk->hblankTask);
}

// The blend of the top screen's list changes down the screen, by the height of the list's rows
// pokebw2's ZknListMain_HBlankTask
static void func_ov209_021bc308(TCB *tcb, void *data) {
    s32 vcount = GX_GetVCount();

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

// pokebw2's ZknListMain_InitVRAM
void func_ov209_021bc370(void) {
    GFL_BGSysSetVRAMBanks(&data_ov209_021be7d4);
}

// pokebw2's ZknListMain_GetVRAMConfig
const BGSysVRAMConfig *func_ov209_021bc380(void) {
    return &data_ov209_021be7d4;
}

// pokebw2's ZknListMain_InitBG
void func_ov209_021bc388(void) {
    GFL_BGSysCreate(HEAPID_ZUKAN_LIST);
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
            GX_BG_SCRBASE_0xf800,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            0,
            0,
        };

        GFL_BGSysCreateBG(0, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(0);
        GFL_BGSysClearCharCore(0, 0x20, 0, HEAPID_ZUKAN_LIST);
        GFL_BGSysQueueScrLoad(0);
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
            GX_BG_CHARBASE_0x08000,
            0x4000,
            GX_BG_EXTPLTT_01,
            1,
            0,
            0,
        };

        GFL_BGSysCreateBG(1, &setup, BGMODE_TEXT);
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

        GFL_BGSysCreateBG(2, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(2);
        GFL_BGSysQueueScrLoad(2);
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
            GX_BG_CHARBASE_0x18000,
            0x8000,
            GX_BG_EXTPLTT_01,
            3,
            0,
            0,
        };

        GFL_BGSysCreateBG(3, &setup, BGMODE_TEXT);
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
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            0,
            0,
        };

        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(4);
        GFL_BGSysClearCharCore(4, 0x20, 0, HEAPID_ZUKAN_LIST);
        GFL_BGSysQueueScrLoad(4);
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

        GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
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

        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(6);
        GFL_BGSysQueueScrLoad(6);
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
            GX_BG_CHARBASE_0x18000,
            0x8000,
            GX_BG_EXTPLTT_01,
            3,
            0,
            0,
        };

        GFL_BGSysCreateBG(7, &setup, BGMODE_TEXT);
    }
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, TRUE);
}

// pokebw2's ZknListMain_ExitBG
void func_ov209_021bc52c(void) {
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, FALSE);
    GFL_BGSysReleaseBG(7);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(0);
    GFL_BGSysFree();
}

// pokebw2's ZknListMain_LoadGraphics
void func_ov209_021bc574(void) {
    ArcTool *arc;
    u32 fileId;

    arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, HEAPID_ZUKAN_LIST);
    GFL_G2DIOLoadArcNCLRDefault(arc, 48, PALTYPE_MAIN_BG, 0, 0x80, HEAPID_ZUKAN_LIST);
    GFL_G2DIOLoadArcNCLRDefault(arc, 49, PALTYPE_SUB_BG, 0, 0xa0, HEAPID_ZUKAN_LIST);
    GFL_BGSysLoadArcNCGRStatic(arc, 58, 2, 0, 0, FALSE, HEAPID_ZUKAN_LIST);
    GFL_BGSysLoadArcNCGRStatic(arc, 56, 3, 0, 0, FALSE, HEAPID_ZUKAN_LIST);
    GFL_BGSysLoadArcNCGRStatic(arc, 59, 6, 0, 0, FALSE, HEAPID_ZUKAN_LIST);
    GFL_BGSysLoadArcNCGRStatic(arc, 57, 7, 0, 0, FALSE, HEAPID_ZUKAN_LIST);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 62, 3, 0, 0, FALSE, HEAPID_ZUKAN_LIST);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 69, 5, 0, 0, FALSE, HEAPID_ZUKAN_LIST);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 63, 7, 0, 0, FALSE, HEAPID_ZUKAN_LIST);
    GFL_ArcToolFree(arc);

    arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), HEAPID_ZUKAN_LIST);
    fileId = func_02027628();
    GFL_BGSysLoadArcNCGRDynamic(arc, fileId, 1, 0, 0, HEAPID_ZUKAN_LIST);
    fileId = func_02027624();
    GFL_G2DIOLoadArcNCLRDefault(arc, fileId, PALTYPE_MAIN_BG, 0x1c0, 0x20, HEAPID_ZUKAN_LIST);
    fileId = func_0202762c();
    loadBGScrToVramByFileNoReserveNegAlign(arc, fileId, 1, 0, 0, FALSE, HEAPID_ZUKAN_LIST);
    GFL_BGSysSetScrPaletteNo(1, 0, 0x15, 0x20, 3, 0xe);
    GFL_BGSysQueueScrLoad(1);
    GFL_ArcToolFree(arc);

    GFL_BGSysLoadNCLRDefault(0x17, 5, 0, 0x1e0, 0x20, HEAPID_ZUKAN_LIST);
    GFL_BGSysLoadNCLRDefault(0x17, 5, 4, 0x1e0, 0x20, HEAPID_ZUKAN_LIST);
}

// pokebw2's ZknListMain_InitPaletteFade
void func_ov209_021bc6c4(ZknListWork *wk) {
    wk->paletteFade = func_02021518(HEAPID_ZUKAN_LIST);
    func_0202155c(wk->paletteFade, PALFADE_BUFFER_MAIN_BG, 0x200, HEAPID_ZUKAN_LIST);
    func_0202155c(wk->paletteFade, PALFADE_BUFFER_SUB_BG, 0x200, HEAPID_ZUKAN_LIST);
    func_0202155c(wk->paletteFade, PALFADE_BUFFER_MAIN_OBJ, 0x200, HEAPID_ZUKAN_LIST);
    func_0202155c(wk->paletteFade, PALFADE_BUFFER_SUB_OBJ, 0x200, HEAPID_ZUKAN_LIST);
}

// pokebw2's ZknListMain_ExitPaletteFade
void func_ov209_021bc704(ZknListWork *wk) {
    func_020215a0(wk->paletteFade, PALFADE_BUFFER_MAIN_BG);
    func_020215a0(wk->paletteFade, PALFADE_BUFFER_SUB_BG);
    func_020215a0(wk->paletteFade, PALFADE_BUFFER_MAIN_OBJ);
    func_020215a0(wk->paletteFade, PALFADE_BUFFER_SUB_OBJ);
    func_02021540(wk->paletteFade);
}

// pokebw2's ZknListMain_StartFade
void func_ov209_021bc730(ZknListWork *wk, u8 start, u8 end) {
    func_0202173c(wk->paletteFade, 1 << PALFADE_BUFFER_MAIN_BG, 0xbfff, -14, start, end, 0, GFL_VBlankGetTCBMgr());
    func_0202173c(wk->paletteFade, 1 << PALFADE_BUFFER_SUB_BG, 0xffef, -14, start, end, 0, GFL_VBlankGetTCBMgr());
    func_0202173c(wk->paletteFade, 1 << PALFADE_BUFFER_MAIN_OBJ, 0xf878, -14, start, end, 0, GFL_VBlankGetTCBMgr());
    func_0202173c(wk->paletteFade, 1 << PALFADE_BUFFER_SUB_OBJ, 0xbfff, -14, start, end, 0, GFL_VBlankGetTCBMgr());
}

// pokebw2's ZknListMain_SetBlend
void func_ov209_021bc7b8(void) {
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG3, 6, 10);
}

// The icons' windows on the top screen show the row under them unblended
// pokebw2's ZknListMain_InitWindow
void func_ov209_021bc7d0(void) {
    G2S_SetWndOBJInsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ,
                             TRUE);
    G2S_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 |
                               GX_WND_PLANEMASK_OBJ,
                           TRUE);
    GXS_SetVisibleWnd(GX_WNDMASK_OW);
}

// pokebw2's ZknListMain_ExitWindow
void func_ov209_021bc810(void) {
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
}

// pokebw2's ZknListMain_InitMsg
void func_ov209_021bc824(ZknListWork *wk) {
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_GAME_START_3, HEAPID_ZUKAN_LIST);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_ZUKAN_LIST);
    wk->wordSet = GFL_WordSetSystemCreateDefault(HEAPID_ZUKAN_LIST);
    wk->printQueue = func_0201c440(HEAPID_ZUKAN_LIST);
    wk->strbuf = GFL_StrBufCreate(0x80, HEAPID_ZUKAN_LIST);
}

// pokebw2's ZknListMain_ExitMsg
void func_ov209_021bc868(ZknListWork *wk) {
    GFL_StrBufFree(wk->strbuf);
    func_0201c4c0(wk->printQueue);
    GFL_WordSetSystemFree(wk->wordSet);
    GFL_FontFree(wk->font);
    GFL_MsgDataFree(wk->msgData);
}

// pokebw2's ZknListMain_LoadRegionalNumbers
void func_ov209_021bc88c(ZknListWork *wk) {
    wk->regionalNumbers = PML_PersonalLoadRegionalDexTable(HEAPID_ZUKAN_LIST, NULL);
}

// pokebw2's ZknListMain_FreeRegionalNumbers
void func_ov209_021bc8a4(ZknListWork *wk) {
    GFL_HeapFree(wk->regionalNumbers);
}

// pokebw2's ZknListMain_InitFramePos
void func_ov209_021bc8b4(ZknListWork *wk) {
    u32 i;
    s16 x, y;

    GFL_BGSysMoveBGReq(5, BG_MOVE_SET_Y, TOUCHBAR_SLIDE);
    GFL_BGSysMoveBGReq(1, BG_MOVE_SET_Y, -TOUCHBAR_SLIDE);
    for (i = ZKNLIST_OBJ_RETURN; i <= ZKNLIST_OBJ_START; i++) {
        func_ov209_021bd958(wk, i, &x, &y, CLACT_SURFACE_MAIN);
        y += TOUCHBAR_SLIDE;
        func_ov209_021bd934(wk, i, x, y, CLACT_SURFACE_MAIN);
    }
}

// pokebw2's ZknListMain_StartFrameMove
void func_ov209_021bc90c(ZknListWork *wk, s16 speed) {
    wk->frameMoveLeft = TOUCHBAR_SLIDE;
    wk->frameMoveSpeed = speed;
}

// pokebw2's ZknListMain_UpdateFrameMove
BOOL func_ov209_021bc91c(ZknListWork *wk) {
    u32 i;
    s16 x, y;

    if (wk->frameMoveLeft == 0) {
        return FALSE;
    }
    GFL_BGSysMoveBGReq(5, BG_MOVE_DOWN, wk->frameMoveSpeed);
    GFL_BGSysMoveBGReq(1, BG_MOVE_UP, wk->frameMoveSpeed);
    for (i = ZKNLIST_OBJ_RETURN; i <= ZKNLIST_OBJ_START; i++) {
        func_ov209_021bd958(wk, i, &x, &y, CLACT_SURFACE_MAIN);
        y += wk->frameMoveSpeed;
        func_ov209_021bd934(wk, i, x, y, CLACT_SURFACE_MAIN);
    }
    wk->frameMoveLeft -= MATH_ABS(wk->frameMoveSpeed);
    return TRUE;
}

// pokebw2's ZknListMain_InitList
void func_ov209_021bc9a8(ZknListWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, HEAPID_ZUKAN_LIST | HEAPID_TAIL_BIT);
    u16 last = func_0200ca8c(wk->param->pokedex);
    FrameListSetup setup = data_ov209_021be7ac;
    unsigned long i;

    setup.count = ZKNLIST_SPECIES_COUNT;
    setup.work = wk;
    // The cursor goes on the species that the Pokédex was last on
    for (i = 0; i < wk->param->count; i++) {
        if (last == wk->param->list[i]) {
            if (i > wk->param->count - ZKNLIST_ROW_COUNT) {
                setup.cursorPos = i - (wk->param->count - ZKNLIST_ROW_COUNT);
                setup.scroll = i - setup.cursorPos;
            } else if (i < ZKNLIST_ROW_COUNT) {
                setup.cursorPos = i;
            } else {
                setup.scroll = i;
            }
            break;
        }
    }
    wk->frameList = func_ov067_021b7754(&setup, HEAPID_ZUKAN_LIST);
    if (wk->param->count < ZKNLIST_ROW_COUNT) {
        wk->listRows = wk->param->count;
    } else {
        wk->listRows = ZKNLIST_ROW_COUNT;
    }
    func_ov067_021b7a10(wk->frameList, arc, 66, FALSE, 0);
    func_ov067_021b7a10(wk->frameList, arc, 67, FALSE, 1);
    func_ov067_021b7a10(wk->frameList, arc, 68, FALSE, 2);
    func_ov067_021b7a4c(wk->frameList, arc, 48, 2, 3);
    GFL_ArcToolFree(arc);

    for (i = 0; i < wk->param->count; i++) {
        if (PokeDex_IsCaught(wk->param->pokedex, wk->param->list[i]) == TRUE) {
            StrBuf *name = GFL_MsgDataLoadStrbufNew(g_PMLSpeciesNamesResident, wk->param->list[i]);

            wk->names[i] = GFL_StrBufClone(name, HEAPID_ZUKAN_LIST);
            GFL_StrBufFree(name);
            func_ov067_021b79e4(wk->frameList, 0, wk->param->list[i] | ZKNLIST_ITEM_CAUGHT);
        } else if (PokeDex_IsSeen(wk->param->pokedex, wk->param->list[i]) == TRUE) {
            StrBuf *name = GFL_MsgDataLoadStrbufNew(g_PMLSpeciesNamesResident, wk->param->list[i]);

            wk->names[i] = GFL_StrBufClone(name, HEAPID_ZUKAN_LIST);
            GFL_StrBufFree(name);
            func_ov067_021b79e4(wk->frameList, 1, wk->param->list[i] | ZKNLIST_ITEM_SEEN);
        } else {
            wk->names[i] = GFL_MsgDataLoadStrbufNew(wk->msgData, GameStart3_Text_007);
            func_ov067_021b79e4(wk->frameList, 2, wk->param->list[i]);
        }
    }
}

// pokebw2's ZknListMain_ExitList
void func_ov209_021bcb60(ZknListWork *wk) {
    u32 i;

    func_ov067_021b796c(wk->frameList);
    for (i = 0; i < ZKNLIST_SPECIES_COUNT; i++) {
        if (wk->names[i] != NULL) {
            GFL_StrBufFree(wk->names[i]);
        }
    }
}

// pokebw2's ZknListMain_ListPrint
static void func_ov209_021bcb90(void *work, u32 index, PrintWindow *window, s16 y, BOOL firstBG) {
    ZknListWork *wk = work;
    u32 value = func_ov067_021b9398(wk->frameList, index);
    u32 species = value & ZKNLIST_ITEM_SPECIES_MASK;

    func_ov209_021bd75c(wk, window, wk->names[index], species);
    if ((value & ZKNLIST_ITEM_STATE_MASK) >> ZKNLIST_ITEM_STATE_SHIFT != 0) {
        func_ov209_021be29c(wk, species, y, firstBG);
    }
}

// pokebw2's ZknListMain_ListSelect
static void func_ov209_021bcbe0(void *work, u32 index, BOOL moved) {
    ZknListWork *wk = work;
    u32 value = func_ov067_021b9398(wk->frameList, index);

    if ((value & ZKNLIST_ITEM_STATE_MASK) >> ZKNLIST_ITEM_STATE_SHIFT != 0) {
        func_ov209_021bdd7c(wk, value & ZKNLIST_ITEM_SPECIES_MASK);
    } else {
        func_ov209_021bdd7c(wk, 0);
    }
    func_ov209_021be51c(wk, index - func_ov067_021b93b8(wk->frameList));
}

// pokebw2's ZknListMain_ListScroll
static void func_ov209_021bcc2c(void *work, s16 delta) {
    ZknListWork *wk = work;

    func_ov209_021be34c(wk, -delta);
    func_ov209_021be51c(wk, func_ov067_021b93b0(wk->frameList));
    func_ov209_021be5d4(wk, TRUE);
}
