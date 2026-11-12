// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov190/wifilogin_disp.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Wi-Fi login's display: its BGs, and either a palette that cycles through blends of 15 palettes with a
// scrolling BG, in the style of the Dream World account screens, or a waving BG with two actors, in the style of
// Game Sync. The name is the ROM's string, from GFL_HeapAllocate's call. Function names are ours.

#include "types.h"
#include "app/wifilogin_disp.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "system/palanm.h"
#include "system/sin_wave_table.h"

#define WIFILOGIN_DISP_ACTOR_COUNT 5

// The palettes the cycle blends between, and the 4 steps of each blend
#define WIFILOGIN_PALETTE_COUNT 15
#define WIFILOGIN_BLEND_STEPS 4

#define WIFILOGIN_WAVE_COUNT 192

// Palette 1's colors 4 to 10 cycle through blends of the file's palettes, a step every other frame
typedef struct {
    TCB *task;
    BOOL active;
    u16 palettes[WIFILOGIN_PALETTE_COUNT][16];
    u16 blended[WIFILOGIN_PALETTE_COUNT * WIFILOGIN_BLEND_STEPS][16];
    s16 frame;
    u8 unk96A;
    u8 skip;
    // The BG that scrolls with the cycle
    u8 scrollBg;
    u8 unk96D[3];
} WifiLoginPaletteCycle;

struct WifiLoginDisp {
    // The characters of the Game Sync style's BG 4 and BG 0
    u32 subChars;
    u32 mainChars;
    u32 style;
    HeapID heapId;
    WifiLoginPaletteCycle cycle;
    ClActUnit *clUnit;
    TCB *vblankTask;
    TCB *hblankTask;
    u32 palette;
    u32 chars;
    u32 cellAnims;
    u32 unk998;
    ClActor *actors[WIFILOGIN_DISP_ACTOR_COUNT];
    u8 unk9B0[0x14];
    u32 unk9C4;
    s16 waveOffset;
    s16 wave[WIFILOGIN_WAVE_COUNT];
    // The Game Sync style's scroll of BGs 7 and 2, 0 for none
    s16 scrollX;
    u8 unkB4C[0x970];
};

static void func_ov115_021d4e58(WifiLoginDisp *disp, u32 bg);
static void func_ov115_021d4fa4(WifiLoginDisp *disp, u32 bg);
static void func_ov115_021d50bc(WifiLoginDisp *disp, u32 bg);
static void func_ov115_021d5214(WifiLoginDisp *disp, u32 bg);
static void func_ov115_021d532c(WifiLoginDisp *disp);
static void func_ov115_021d5528(TCB *task, void *work);
static void func_ov115_021d5598(WifiLoginDisp *disp);
static void func_ov115_021d57d0(WifiLoginDisp *disp);
static void func_ov115_021d580c(WifiLoginDisp *disp);
static void func_ov115_021d584c(WifiLoginDisp *disp);
static void func_ov115_021d58b4(TCB *task, void *work);
static void func_ov115_021d5900(TCB *task, void *work);

// pokebw2's sWifiLoginDispVRAMConfig
static BGSysVRAMConfig data_ov115_021d65e4 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// pokebw2's sWifiLoginDispLCDConfig
static BGSysLCDConfig data_ov115_021d65d4 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

// pokebw2's WifiLoginDisp_Create
WifiLoginDisp *func_ov115_021d4d38(HeapID heapId, u32 style, u32 unused) {
    WifiLoginDisp *disp = GFL_HeapAllocate(heapId, sizeof(WifiLoginDisp), TRUE, "wifilogin_disp.c", 146);

    disp->heapId = heapId;
    disp->style = style;
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    G2_BlendNone();
    GFL_BGSysSetDisplayLayout(1);
    GXS_DispOn();
    GX_DispOn();
    GFL_BGSysCreate(disp->heapId);
    GFL_BGSysSetVRAMBanks(&data_ov115_021d65e4);
    GFL_BGSysSetLCDConfig(&data_ov115_021d65d4);
    switch (disp->style) {
    case WIFILOGIN_DISP_STYLE_GSYNC:
        func_ov115_021d4fa4(disp, 0);
        func_ov115_021d5214(disp, 4);
        func_ov115_021d57d0(disp);
        func_ov115_021d5598(disp);
        break;
    case WIFILOGIN_DISP_STYLE_PDWACC:
        func_ov115_021d4e58(disp, 0);
        func_ov115_021d50bc(disp, 4);
        func_ov115_021d532c(disp);
        break;
    }
    GFL_BGSysSetEnabledBGsB(GX_PLANEMASK_ALL);
    GFL_BGSysSetEnabledBGsA(GX_PLANEMASK_ALL);
    return disp;
}

// pokebw2's WifiLoginDisp_Main
void func_ov115_021d4df4(WifiLoginDisp *disp) {
    if (disp->style == WIFILOGIN_DISP_STYLE_GSYNC) {
        func_ov115_021d580c(disp);
    }
}

// pokebw2's WifiLoginDisp_Free
void func_ov115_021d4e04(WifiLoginDisp *disp) {
    if (disp->style == WIFILOGIN_DISP_STYLE_GSYNC) {
        func_ov115_021d584c(disp);
    } else if (disp->cycle.task != NULL) {
        GFL_TCBRemove(disp->cycle.task);
        disp->cycle.task = NULL;
        disp->cycle.active = FALSE;
    }
    GFL_BGSysFreeFilledChar(5, 1, 0);
    GFL_BGSysFreeFilledChar(6, 1, 0);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysSetBGEnabled(5, FALSE);
    GFL_BGSysFree();
    GFL_HeapFree(disp);
}

// pokebw2's WifiLoginDisp_CreatePdwAccMainBGs
static void func_ov115_021d4e58(WifiLoginDisp *disp, u32 bg) {
    {
        BGSetup setup = {
            0,
            0,
            0x1000,
            0,
            BGRES_512x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf800,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(bg, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(bg, TRUE);
        GFL_BGSysLoadScr(bg);
    }
    {
        u32 bg1 = bg + 1;
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf000,
            GX_BG_CHARBASE_0x08000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(bg1, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(bg1, TRUE);
        GFL_BGSysFillChar(bg1, 0, 1, 0);
        GFL_BGSysFillScrArea(bg1, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(bg1);
    }
    {
        u32 bg2 = bg + 2;
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe000,
            GX_BG_CHARBASE_0x10000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(bg2, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(bg2, TRUE);
        GFL_BGSysFillScrArea(bg2, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(bg2);
    }
    {
        u32 bg3 = bg + 3;
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe800,
            GX_BG_CHARBASE_0x18000,
            0x8000,
            GX_BG_EXTPLTT_01,
            3,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(bg3, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(bg3, TRUE);
        GFL_BGSysFillChar(bg3, 0, 1, 0);
        GFL_BGSysFillScrArea(bg3, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(bg3);
    }
}

// pokebw2's WifiLoginDisp_CreateGSyncMainBGs
static void func_ov115_021d4fa4(WifiLoginDisp *disp, u32 bg) {
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
            3,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(bg, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(bg, TRUE);
        GFL_BGSysLoadScr(bg);
    }
    {
        u32 bg1 = bg + 1;
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
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(bg1, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(bg1, TRUE);
        GFL_BGSysLoadScr(bg1);
    }
    {
        u32 bg2 = bg + 2;
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
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(bg2, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(bg2, TRUE);
        GFL_BGSysLoadScr(bg2);
    }
    {
        u32 bg3 = bg + 3;
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe800,
            GX_BG_CHARBASE_0x18000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(bg3, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(bg3, TRUE);
        GFL_BGSysFillChar(bg3, 0, 1, 0);
        GFL_BGSysFillScrArea(bg3, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(bg3);
    }
}

// pokebw2's WifiLoginDisp_CreatePdwAccSubBGs
static void func_ov115_021d50bc(WifiLoginDisp *disp, u32 bg) {
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
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(bg, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(bg, TRUE);
        GFL_BGSysLoadScr(bg);
    }
    {
        u32 bg1 = bg + 1;
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf000,
            GX_BG_CHARBASE_0x08000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(bg1, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(bg1, TRUE);
        GFL_BGSysFillChar(bg1, 0, 1, 0);
        GFL_BGSysFillScrArea(bg1, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(bg1);
    }
    {
        u32 bg2 = bg + 2;
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe000,
            GX_BG_CHARBASE_0x10000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(bg2, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(bg2, TRUE);
        GFL_BGSysFillChar(bg2, 0, 1, 0);
        GFL_BGSysFillScrArea(bg2, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(bg2);
    }
    {
        u32 bg3 = bg + 3;
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe800,
            GX_BG_CHARBASE_0x18000,
            0x8000,
            GX_BG_EXTPLTT_01,
            3,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(bg3, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(bg3, TRUE);
        GFL_BGSysFillChar(bg3, 0, 1, 0);
        GFL_BGSysFillScrArea(bg3, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(bg3);
    }
}

// pokebw2's WifiLoginDisp_CreateGSyncSubBGs
static void func_ov115_021d5214(WifiLoginDisp *disp, u32 bg) {
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
            0x10000,
            GX_BG_EXTPLTT_01,
            3,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(bg, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(bg, TRUE);
        GFL_BGSysLoadScr(bg);
    }
    {
        u32 bg1 = bg + 1;
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf000,
            GX_BG_CHARBASE_0x10000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(bg1, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(bg1, TRUE);
        GFL_BGSysFillChar(bg1, 0, 1, 0);
        GFL_BGSysFillScrArea(bg1, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(bg1);
    }
    {
        u32 bg2 = bg + 2;
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe000,
            GX_BG_CHARBASE_0x00000,
            0x10000,
            GX_BG_EXTPLTT_01,
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(bg2, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(bg2, TRUE);
        GFL_BGSysLoadScr(bg2);
    }
    {
        u32 bg3 = bg + 3;
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe800,
            GX_BG_CHARBASE_0x00000,
            0x10000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(bg3, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(bg3, TRUE);
        GFL_BGSysLoadScr(bg3);
    }
}

// Loads the palettes the cycle blends between, makes the blends and starts the cycle
static inline void WifiLoginDisp_InitCycle(WifiLoginDisp *disp, ArcTool *arc, HeapID heapId) {
    int i;
    int count = 0;
    NNSG2dPaletteData *palette;
    void *file;

    MI_CpuFill8(&disp->cycle, 0, sizeof(disp->cycle));
    file = GFL_G2DIOReadNCLRArc(arc, 2, &palette, heapId);
    MI_CpuCopy8(palette->pRawData, disp->cycle.palettes, sizeof(disp->cycle.palettes));
    MI_CpuCopy8(palette->pRawData, disp->cycle.blended, sizeof(disp->cycle.palettes));
    GFL_HeapFree(file);

    // Colors 4 to 10 of each palette, blended into the next in 4 steps
    for (i = 0; i < WIFILOGIN_PALETTE_COUNT; i++) {
        int next = i + 1;
        fx32 fraction;
        BOOL done;

        if (next >= WIFILOGIN_PALETTE_COUNT) {
            next -= WIFILOGIN_PALETTE_COUNT;
        }
        fraction = 0;
        done = FALSE;
        while (TRUE) {
            int j;

            for (j = 4; j < 11; j++) {
                func_02021f64(&disp->cycle.palettes[i][j], &disp->cycle.blended[count][j], 1, fraction >> 8,
                            disp->cycle.palettes[next][j]);
            }
            count++;
            if (done == TRUE) {
                break;
            }
            fraction += FX32_ONE * 6 / 16;
            if (fraction >= FX32_ONE) {
                fraction = FX32_ONE;
                done = TRUE;
            }
        }
    }
    DC_FlushRange(disp->cycle.blended, sizeof(disp->cycle.blended));
    disp->cycle.active = TRUE;
    disp->cycle.task = GFL_VBlankTCBAdd(func_ov115_021d5528, &disp->cycle, 20);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG3, 8, 16);
    disp->cycle.scrollBg = 0;
}

// pokebw2's WifiLoginDisp_LoadPdwAccGraphics
static void func_ov115_021d532c(WifiLoginDisp *disp) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_PDWACC, disp->heapId);

    GFL_G2DIOLoadArcNCLRDefault(arc, 3, PALTYPE_SUB_BG, 0, 0, disp->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 4, 4, 0, 0, FALSE, disp->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 6, 4, 0, 0, FALSE, disp->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 3, PALTYPE_MAIN_BG, 0, 0, disp->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 4, 0, 0, 0, FALSE, disp->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 5, 0, 0, 0, FALSE, disp->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 4, 3, 0, 0, FALSE, disp->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 7, 3, 0, 0, FALSE, disp->heapId);
    WifiLoginDisp_InitCycle(disp, arc, disp->heapId);
    GFL_ArcToolFree(arc);
}

// pokebw2's WifiLoginDisp_CyclePalette
static void func_ov115_021d5528(TCB *task, void *work) {
    WifiLoginPaletteCycle *cycle = work;

    if (cycle->active) {
        cycle->skip ^= 1;
        if (!(cycle->skip & 1)) {
            GX_LoadBGPltt(&cycle->blended[cycle->frame][4], 0x20 + 4 * 2, 7 * 2);
            GXS_LoadBGPltt(&cycle->blended[cycle->frame][4], 0x20 + 4 * 2, 7 * 2);
            cycle->frame++;
            if (cycle->frame >= WIFILOGIN_PALETTE_COUNT * WIFILOGIN_BLEND_STEPS) {
                cycle->frame = 0;
            }
            GFL_BGSysMoveBG(cycle->scrollBg, BG_MOVE_LEFT, 7);
        }
    }
}

// pokebw2's WifiLoginDisp_LoadGSyncGraphics
static void func_ov115_021d5598(WifiLoginDisp *disp) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_GSYNC, disp->heapId);
    ClActorSetup setup0;
    ClActorSetup setup1;

    GFL_G2DIOLoadArcNCLRDefault(arc, 0, PALTYPE_SUB_BG, 0, 0, disp->heapId);
    disp->subChars = GFL_BGSysLoadArcNCGRDynamic(arc, 7, 4, 0, FALSE, disp->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 18, 4, 0, (u16)disp->subChars, 0, FALSE, disp->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 19, 6, 0, (u16)disp->subChars, 0, FALSE, disp->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 20, 7, 0, (u16)disp->subChars, 0, FALSE, disp->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, PALTYPE_MAIN_BG, 0, 0, disp->heapId);
    disp->mainChars = GFL_BGSysLoadArcNCGRDynamic(arc, 6, 0, 0, FALSE, disp->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 15, 0, 0, (u16)disp->mainChars, 0, FALSE, disp->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 16, 1, 0, (u16)disp->mainChars, 0, FALSE, disp->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 17, 2, 0, (u16)disp->mainChars, 0, FALSE, disp->heapId);
    disp->chars = func_0204a6c8(arc, 9, FALSE, CLACT_VRAM_SUB, disp->heapId);
    disp->palette = func_0204ac18(arc, 3, CLACT_VRAM_SUB, 0, 0, 3, disp->heapId);
    disp->cellAnims = func_0204af28(arc, 12, 25, disp->heapId);
    GFL_ArcToolFree(arc);

    setup0.x = 128;
    setup0.y = 96;
    setup0.sequence = 10;
    setup0.priority = 0;
    setup0.bgPriority = 2;
    disp->actors[0] = func_0204b294(disp->clUnit, disp->chars, disp->palette, disp->cellAnims, &setup0,
                                    CLACT_SURFACE_SUB, disp->heapId);
    func_0204b92c(disp->actors[0], TRUE);
    func_0204b3dc(disp->actors[0], TRUE);

    setup1.x = 128;
    setup1.y = 96;
    setup1.sequence = 11;
    setup1.priority = 1;
    setup1.bgPriority = 2;
    disp->actors[1] = func_0204b294(disp->clUnit, disp->chars, disp->palette, disp->cellAnims, &setup1,
                                    CLACT_SURFACE_SUB, disp->heapId);
    func_0204b92c(disp->actors[1], TRUE);
    func_0204b3dc(disp->actors[1], TRUE);

    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1, 6, 16);
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG3, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ,
                      6, 16);
    disp->hblankTask = GFL_HBlankTCBAdd(func_ov115_021d58b4, disp, 0);
    func_02024230(disp->wave, WIFILOGIN_WAVE_COUNT, 0x5b0, 0x1800);
    disp->scrollX = 10;
}

// pokebw2's WifiLoginDisp_InitClAct
static void func_ov115_021d57d0(WifiLoginDisp *disp) {
    ClActSys_Create(&data_020a1448, &data_ov115_021d65e4, disp->heapId);
    disp->clUnit = func_0204b100(40, 0, disp->heapId);
    disp->vblankTask = GFL_VBlankTCBAdd(func_ov115_021d5900, disp, 0);
}

// pokebw2's WifiLoginDisp_UpdateWave
static void func_ov115_021d580c(WifiLoginDisp *disp) {
    disp->waveOffset++;
    if (disp->waveOffset >= WIFILOGIN_WAVE_COUNT) {
        disp->waveOffset = 0;
    }
    if (disp->scrollX != 0) {
        disp->scrollX++;
        if (disp->scrollX >= 0x101) {
            disp->scrollX = 1;
        }
    }
    func_0204a600();
}

// pokebw2's WifiLoginDisp_FreeClAct
static void func_ov115_021d584c(WifiLoginDisp *disp) {
    int i;

    for (i = 0; i < WIFILOGIN_DISP_ACTOR_COUNT; i++) {
        if (disp->actors[i] != NULL) {
            func_0204b3b4(disp->actors[i]);
            disp->actors[i] = NULL;
        }
    }
    func_0204ada4(disp->palette);
    func_0204a8d4(disp->chars);
    func_0204afd8(disp->cellAnims);
    if (disp->hblankTask != NULL) {
        GFL_TCBRemove(disp->hblankTask);
    }
    GFL_TCBRemove(disp->vblankTask);
    func_0204b1cc(disp->clUnit);
    func_0204a5ac();
}

// Offsets BG 1 of the main screen and BG 2 of the sub screen by the wave, a line at a time
// pokebw2's WifiLoginDisp_HBlank
static void func_ov115_021d58b4(TCB *task, void *work) {
    WifiLoginDisp *disp = work;
    int line = (reg_GX_VCOUNT + (disp->waveOffset + 1)) % WIFILOGIN_WAVE_COUNT;

    if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
        reg_G2_BG1OFS = disp->wave[line] & REG_G2_BG0OFS_HOFFSET_MASK;
        reg_G2S_DB_BG2OFS = disp->wave[line] & REG_G2_BG0OFS_HOFFSET_MASK;
    }
}

// pokebw2's WifiLoginDisp_VBlank
static void func_ov115_021d5900(TCB *task, void *work) {
    WifiLoginDisp *disp = work;

    if (disp->scrollX > 0) {
        GFL_BGSysMoveBG(7, BG_MOVE_SET_X, disp->scrollX - 1);
        GFL_BGSysMoveBG(2, BG_MOVE_SET_X, disp->scrollX - 1);
    }
    func_0204a648();
}

// pokebw2's WifiLoginDisp_SetUnk9C4
void func_ov115_021d592c(WifiLoginDisp *disp) {
    disp->unk9C4 = TRUE;
}

// pokebw2's WifiLoginDisp_ResetBlend
void func_ov115_021d5938(WifiLoginDisp *disp) {
    switch (disp->style) {
    case WIFILOGIN_DISP_STYLE_GSYNC:
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1, 6, 16);
        G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG3,
                          GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 6, 16);
        break;
    case WIFILOGIN_DISP_STYLE_PDWACC:
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG3, 8, 16);
        break;
    }
}
