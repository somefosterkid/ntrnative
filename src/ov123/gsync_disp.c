// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov199/gsync_disp.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// Game Sync's screens: the BGs, the actors, the Pokémon sent's icon, the top screen's wave and progress gauge, and the
// items and Pokémon of the Dream World that drift down it. The name is the ROM's string, from GFL_HeapAllocate's call.
// Function names are ours.

#include "types.h"
#include "app/gsync/gsync_disp.h"
#include "app/ui/ui_scene.h"
#include "constants/arc.h"
#include "constants/species.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "nitro/memorymap.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "pml/item.h"
#include "pml/poke_party.h"
#include "system/bmp_winframe.h"
#include "system/sin_wave_table.h"
#include "system/tpoke_data.h"
#include "constants/items.h"

#define GSYNC_DISP_ACTOR_COUNT 16
#define GSYNC_DISP_FLOAT_COUNT (GSYNC_DISP_FLOAT_ITEM_COUNT + GSYNC_DISP_FLOAT_POKEMON_COUNT)

// The actors that aren't one of the top screen's sequences
#define GSYNC_DISP_ACTOR_POKE_ICON 13
#define GSYNC_DISP_ACTOR_SUB 14

// The OBJ resources: the palettes, then the characters, then the cell animations
enum {
    GSYNC_DISP_RES_PLTT_ACTORS,
    GSYNC_DISP_RES_PLTT_SUB,
    GSYNC_DISP_RES_PLTT_POKE_ICON,
    GSYNC_DISP_RES_PLTT_3,
    GSYNC_DISP_RES_PLTT_4,
    GSYNC_DISP_RES_PLTT_ITEM,
    GSYNC_DISP_RES_CHAR_ACTORS,
    GSYNC_DISP_RES_CHAR_SUB,
    GSYNC_DISP_RES_CHAR_POKE_ICON,
    GSYNC_DISP_RES_CHAR_FLOAT_POKEMON,
    GSYNC_DISP_RES_CHAR_10,
    GSYNC_DISP_RES_CHAR_11,
    GSYNC_DISP_RES_CHAR_ITEM,
    GSYNC_DISP_RES_CELL_ACTORS,
    GSYNC_DISP_RES_CELL_SUB,
    GSYNC_DISP_RES_CELL_POKE_ICON,
    GSYNC_DISP_RES_CELL_16,
    GSYNC_DISP_RES_CELL_17,
    GSYNC_DISP_RES_CELL_18,
    GSYNC_DISP_RES_CELL_ITEM,
    GSYNC_DISP_RES_COUNT,
};

// An item or a Pokémon floating up the top screen
typedef struct {
    // The shared actor it is drawn with while it floats
    ClActor *actor;
    u16 x;
    u16 y;
    // The angles of its sway
    u16 angleX;
    u16 angleY;
    u16 timer;
    u16 active;
    // The item, or the Pokémon's species, form and sex
    u16 id;
    u16 form;
    u16 sex;
    u16 done;
} GSyncDispFloat;

// What a floating item or Pokémon is drawn with
typedef struct {
    u16 palette[16];
    u8 chars[0x200];
    u32 paletteNum;
} GSyncDispFloatGraphics;

// Where each top screen actor is
typedef struct {
    s32 x;
    s32 y;
    s32 priority;
} GSyncDispActorPos;

struct GSyncDisp {
    u32 subBgChars;
    u32 mainBgChars;
    u32 unk8;
    ClActUnit *clUnit;
    TCB *vblankTask;
    TCB *hblankTask;
    NNSG2dPaletteData *bgPalette;
    void *bgPaletteFile;
    u32 res[GSYNC_DISP_RES_COUNT];
    u32 unk70;
    ClActor *actors[GSYNC_DISP_ACTOR_COUNT];
    ClActor *floatPokemonActor;
    ClActor *floatItemActor;
    GSyncDispFloatGraphics floatGraphics[GSYNC_DISP_FLOAT_COUNT];
    GSyncDispFloat floats[GSYNC_DISP_FLOAT_COUNT];
    HeapID heapId;
    // The line of the wave's table the screen's first line takes
    s16 waveLine;
    s16 waveTable[192];
    u8 unk4548[0xc];
    fx32 fade;
    int fadeStep;
    // The line the progress gauge is filled up to
    int progressLine;
    u32 unk4560;
    int paletteTimer;
};

static void func_ov123_021d5f4c(GSyncDisp *disp);
static void func_ov123_021d60f8(TCB *task, void *work);
static void func_ov123_021d614c(GSyncDisp *disp);
static void func_ov123_021d62f8(GSyncDisp *disp, s16 x, s16 y);
static void func_ov123_021d634c(GSyncDisp *disp, int index);
void func_ov123_021d63e4(GSyncDisp *disp);
void func_ov123_021d6404(GSyncDisp *disp);
static void func_ov123_021d6548(TCB *task, void *work);
static void func_ov123_021d6678(GSyncDisp *disp);
static void func_ov123_021d6720(GSyncDisp *disp, int index);
static void func_ov123_021d679c(GSyncDispFloat *fl);
void func_ov123_021d67b4(GSyncDisp *disp, int index);
static void func_ov123_021d6830(GSyncDispFloat *fl);
static void func_ov123_021d6c48(GSyncDisp *disp, int index, ClActor *actor);
static void func_ov123_021d6c90(GSyncDisp *disp, int index, ClActor *actor);
static void func_ov123_021d6cd8(GSyncDisp *disp);
static void func_ov123_021d6d5c(GSyncDisp *disp);

static BGSysVRAMConfig data_ov123_021daa9c = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

static GSyncDispActorPos data_ov123_021daacc[] = {
#include "asset/gsync_disp_data_ov123_021daacc.inc"
};

static BGSysLCDConfig data_ov123_021daa78 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

// pokebw2's GSyncDisp_Main


GSyncDisp *func_ov123_021d5d94(HeapID heapId) {
    GSyncDisp *disp = GFL_HeapAllocate(heapId, sizeof(GSyncDisp), TRUE, "gsync_disp.c", 254);

    disp->heapId = heapId;
    GFL_OvlLoad(OVERLAY_ID(67));
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    GFL_BGSysSetDisplayLayout(1);
    GXS_DispOn();
    GX_DispOn();
    GFL_BGSysCreate(disp->heapId);
    ClActSys_Create(&data_020a1464, &data_ov123_021daa9c, disp->heapId);
    disp->clUnit = func_0204b100(40, 0, disp->heapId);
    GFL_BGSysSetVRAMBanks(&data_ov123_021daa9c);
    GFL_BGSysSetLCDConfig(&data_ov123_021daa78);
    func_ov123_021d5f4c(disp);
    func_ov123_021d614c(disp);
    func_ov123_021d6cd8(disp);
    func_ov123_021d6d5c(disp);
    disp->vblankTask = GFL_VBlankTCBAdd(func_ov123_021d60f8, disp, 0);
    disp->progressLine = 200;
    GFL_BGSysSetEnabledBGsB(GX_PLANEMASK_ALL);
    GFL_BGSysSetEnabledBGsA(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ);
    return disp;
}

void func_ov123_021d5e60(GSyncDisp *disp) {
    disp->waveLine++;
    if (disp->waveLine >= 192) {
        disp->waveLine = 0;
    }
    func_0204a600();
    func_ov123_021d6678(disp);
    func_ov123_021d6934(disp);
}

void func_ov123_021d5e8c(GSyncDisp *disp) {
    int i;

    func_ov123_021d63e4(disp);
    GFL_HeapFree(disp->bgPaletteFile);
    func_0204b3b4(disp->floatPokemonActor);
    func_0204b3b4(disp->floatItemActor);
    for (i = 0; i < 6; i++) {
        if (disp->res[i] != 0) {
            func_0204ada4(disp->res[i]);
        }
    }
    for (; i < 13; i++) {
        if (disp->res[i] != 0) {
            func_0204a8d4(disp->res[i]);
        }
    }
    for (; i < 20; i++) {
        if (disp->res[i] != 0) {
            func_0204afd8(disp->res[i]);
        }
    }
    if (disp->hblankTask != NULL) {
        GFL_TCBRemove(disp->hblankTask);
    }
    GFL_TCBRemove(disp->vblankTask);
    func_0204b1cc(disp->clUnit);
    func_0204a5ac();
    GFL_BGSysFreeFilledChar(1, 1, 0);
    GFL_BGSysFreeFilledChar(6, 1, 0);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysSetBGEnabled(5, FALSE);
    GFL_BGSysFree();
    GFL_HeapFree(disp);
    GFL_OvlUnload(OVERLAY_ID(67));
}


// pokebw2's GSyncDisp_CreateBGs
static void func_ov123_021d5f4c(GSyncDisp *disp) {
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
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(0, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(0, TRUE);
        GFL_BGSysLoadScr(0);
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
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            3,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(1, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(1, TRUE);
        GFL_BGSysLoadScr(1);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x1000,
            0,
            BGRES_512x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf000,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(3, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(3, TRUE);
        GFL_BGSysLoadScr(3);
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
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(4, TRUE);
        GFL_BGSysLoadScr(4);
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
            GX_BG_CHARBASE_0x10000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
        GFL_BGSysFillChar(5, 0, 1, 0);
        GFL_BGSysFillScrArea(5, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysSetBGEnabled(5, TRUE);
        GFL_BGSysLoadScr(5);
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
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(6, TRUE);
        GFL_BGSysFillChar(6, 0, 1, 0);
        GFL_BGSysFillScrArea(6, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(6);
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
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            3,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(7, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(7, TRUE);
        GFL_BGSysLoadScr(7);
    }
}

// Cycles palette 2 of both screens' BGs through the palette file's next 8, each for 10 frames
// pokebw2's GSyncDisp_VBlank
static void func_ov123_021d60f8(TCB *task, void *work) {
    GSyncDisp *disp = work;
    u8 *data;
    int line;
    func_0204a648();
    disp->paletteTimer++;
    if (disp->paletteTimer % 10 == 1) {
        line = disp->paletteTimer / 10 + 2;
        data = disp->bgPalette->pRawData;
        GX_LoadBGPltt(data + line * 0x20, 0x40, 0x20);
        GXS_LoadBGPltt(data + line * 0x20, 0x40, 0x20);
    }
    if (disp->paletteTimer >= 80) {
        disp->paletteTimer = 0;
    }
}

// pokebw2's GSyncDisp_LoadGraphics
static void func_ov123_021d614c(GSyncDisp *disp) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_GSYNC, disp->heapId);

    GFL_G2DIOLoadArcNCLRDefault(arc, 2, PALTYPE_SUB_BG, 0, 0, disp->heapId);
    disp->subBgChars = GFL_BGSysLoadArcNCGRDynamic(arc, 8, 4, 0, FALSE, disp->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 21, 4, 0, CHAR_POS(disp->subBgChars), 0, FALSE, disp->heapId);
    // BUG: BG 7 is on the sub screen and shares BG 4's characters, but its screen is offset by the main BGs'
    // characters, which aren't loaded yet. Both offsets are 0, so it works
#ifdef BUGFIX
    GFL_G2DIOLoadNSCRSync(arc, 24, 7, 0, CHAR_POS(disp->subBgChars), 0, FALSE, disp->heapId);
#else
    GFL_G2DIOLoadNSCRSync(arc, 24, 7, 0, CHAR_POS(disp->mainBgChars), 0, FALSE, disp->heapId);
#endif
    disp->bgPaletteFile = GFL_G2DIOReadNCLRArc(arc, 2, &disp->bgPalette, disp->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 2, PALTYPE_MAIN_BG, 0, 0, disp->heapId);
    disp->mainBgChars = GFL_BGSysLoadArcNCGRDynamic(arc, 8, 0, 0, FALSE, disp->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 23, 0, 0, CHAR_POS(disp->mainBgChars), 0, FALSE, disp->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 24, 1, 0, CHAR_POS(disp->mainBgChars), 0, FALSE, disp->heapId);
    disp->res[GSYNC_DISP_RES_CHAR_ACTORS] = func_0204a6c8(arc, 9, FALSE, CLACT_VRAM_BOTH, disp->heapId);
    disp->res[GSYNC_DISP_RES_PLTT_ACTORS] = func_0204ac18(arc, 3, CLACT_VRAM_BOTH, 3 * 0x20, 0, 4, disp->heapId);
    disp->res[GSYNC_DISP_RES_CELL_ACTORS] = func_0204af28(arc, 12, 25, disp->heapId);
    disp->res[GSYNC_DISP_RES_CHAR_SUB] = func_0204a6c8(arc, 10, FALSE, CLACT_VRAM_BOTH, disp->heapId);
    disp->res[GSYNC_DISP_RES_PLTT_SUB] = func_0204ac18(arc, 4, CLACT_VRAM_BOTH, 7 * 0x20, 0, 0, disp->heapId);
    disp->res[GSYNC_DISP_RES_CELL_SUB] = func_0204af28(arc, 13, 26, disp->heapId);
    func_ov123_021d6404(disp);
    GFL_ArcToolFree(arc);
}

// pokebw2's GSyncDisp_CreateSubActor
void func_ov123_021d62a8(GSyncDisp *disp) {
    func_ov123_021d62f8(disp, 133, 128);
}

// pokebw2's GSyncDisp_CreateActor
void func_ov123_021d62b4(GSyncDisp *disp, int index) {
    func_ov123_021d634c(disp, index);
}

// pokebw2's GSyncDisp_SetActorSequence
void func_ov123_021d62bc(GSyncDisp *disp, int index, int sequence) {
    func_0204b878(disp->actors[index], sequence);
}

// pokebw2's GSyncDisp_SetActorCallback
void func_ov123_021d62d0(GSyncDisp *disp, int index, const ClActorCallback *callback) {
    func_0204ba00(disp->actors[index], callback);
}

// pokebw2's GSyncDisp_DeleteActor
void func_ov123_021d62e0(GSyncDisp *disp, int index) {
    if (disp->actors[index] != NULL) {
        func_0204b3b4(disp->actors[index]);
        disp->actors[index] = NULL;
    }
}

// pokebw2's GSyncDisp_CreateSubActorAt
static void func_ov123_021d62f8(GSyncDisp *disp, s16 x, s16 y) {
    if (disp->actors[GSYNC_DISP_ACTOR_SUB] == NULL) {
        ClActorSetup setup;

        setup.x = x;
        setup.y = y;
        setup.sequence = 0;
        setup.priority = 0;
        setup.bgPriority = 1;
        disp->actors[GSYNC_DISP_ACTOR_SUB] =
            func_0204b294(disp->clUnit, disp->res[GSYNC_DISP_RES_CHAR_SUB], disp->res[GSYNC_DISP_RES_PLTT_SUB],
                          disp->res[GSYNC_DISP_RES_CELL_SUB], &setup, CLACT_SURFACE_SUB, disp->heapId);
        func_0204b92c(disp->actors[GSYNC_DISP_ACTOR_SUB], TRUE);
        func_0204b3dc(disp->actors[GSYNC_DISP_ACTOR_SUB], TRUE);
    }
}

// pokebw2's GSyncDisp_InitActor
static void func_ov123_021d634c(GSyncDisp *disp, int index) {
    ClActorSetup setup;

    if (disp->actors[index] != NULL) {
        func_0204b3b4(disp->actors[index]);
    }
    setup.x = data_ov123_021daacc[index].x;
    setup.y = data_ov123_021daacc[index].y;
    setup.sequence = index;
    setup.priority = data_ov123_021daacc[index].priority;
    setup.bgPriority = 1;
    disp->actors[index] =
        func_0204b294(disp->clUnit, disp->res[GSYNC_DISP_RES_CHAR_ACTORS], disp->res[GSYNC_DISP_RES_PLTT_ACTORS],
                      disp->res[GSYNC_DISP_RES_CELL_ACTORS], &setup, CLACT_SURFACE_MAIN, disp->heapId);
    func_0204b8c4(disp->actors[index], 0);
    func_0204b92c(disp->actors[index], TRUE);
    func_0204b3dc(disp->actors[index], TRUE);
    func_0204b570(disp->actors[index], 2);
}

// pokebw2's GSyncDisp_PokeIconSequence2
void func_ov123_021d63d4(GSyncDisp *disp) {
    func_0204b878(disp->actors[GSYNC_DISP_ACTOR_POKE_ICON], 2);
}

// pokebw2's GSyncDisp_DeleteActors
void func_ov123_021d63e4(GSyncDisp *disp) {
    int i;
    for (i = 0; i < 16; i++) {
        if (disp->actors[i] != NULL) {
            func_0204b3b4(disp->actors[i]);
            disp->actors[i] = NULL;
        }
    }
}

// pokebw2's GSyncDisp_LoadPokeIconRes
void func_ov123_021d6404(GSyncDisp *disp) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(7, disp->heapId);
    disp->res[2] = func_0204acec(arc, func_0201bc3c(), 2, 0, disp->heapId);
    GFL_ArcToolFree(arc);
    arc = GFL_ArcSysCreateFileHandle(ARCID_GSYNC, disp->heapId);
    disp->res[15] = func_0204af28(arc, 14, 27, disp->heapId);
    GFL_ArcToolFree(arc);
}

// pokebw2's GSyncDisp_CreatePokeIcon
void func_ov123_021d6458(GSyncDisp *disp, BoxPkm *pkm, u32 surface) {
    const s32 x[] = { 133, 129 };
    const s32 y[] = { 128, 132 };
    ClActorSetup setup;
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, disp->heapId);

    disp->res[GSYNC_DISP_RES_CHAR_POKE_ICON] =
        func_0204a6c8(arc, func_0201ba68(pkm), FALSE, CLACT_VRAM_BOTH, disp->heapId);
    if (surface == CLACT_SURFACE_MAIN) {
        setup.x = x[1];
        setup.y = y[1];
    } else {
        setup.x = x[0];
        setup.y = y[0];
    }
    setup.sequence = 0;
    setup.priority = 3;
    setup.bgPriority = 1;
    disp->actors[GSYNC_DISP_ACTOR_POKE_ICON] =
        func_0204b294(disp->clUnit, disp->res[GSYNC_DISP_RES_CHAR_POKE_ICON], disp->res[GSYNC_DISP_RES_PLTT_POKE_ICON],
                      disp->res[GSYNC_DISP_RES_CELL_POKE_ICON], &setup, surface, disp->heapId);
    func_0204b92c(disp->actors[GSYNC_DISP_ACTOR_POKE_ICON], FALSE);
    func_0204b3dc(disp->actors[GSYNC_DISP_ACTOR_POKE_ICON], TRUE);
    func_0204b6f4(disp->actors[GSYNC_DISP_ACTOR_POKE_ICON], func_0201bbe8(pkm), 1);
    GFL_ArcToolFree(arc);
}

// pokebw2's GSyncDisp_StartPokeIcon
void func_ov123_021d651c(GSyncDisp *disp) {
    func_0204b92c(disp->actors[GSYNC_DISP_ACTOR_POKE_ICON], TRUE);
}

// pokebw2's GSyncDisp_PokeIconSequence1
void func_ov123_021d652c(GSyncDisp *disp) {
    func_0204b878(disp->actors[GSYNC_DISP_ACTOR_POKE_ICON], 1);
    func_0204b92c(disp->actors[GSYNC_DISP_ACTOR_POKE_ICON], TRUE);
}

// The progress gauge's colors, the line where it is filled and the lines below it, then white
static u16 data_ov123_021daa88[10] = {
    GX_RGB(22, 21, 22), GX_RGB(23, 23, 23), GX_RGB(24, 24, 24), GX_RGB(25, 25, 25), GX_RGB(26, 26, 26),
    GX_RGB(27, 27, 27), GX_RGB(28, 28, 28), GX_RGB(29, 29, 29), GX_RGB(30, 30, 30), GX_RGB(31, 31, 31),
};

// Sways BG 3 line by line, and colors the progress gauge's lines
// pokebw2's GSyncDisp_HBlank
static void func_ov123_021d6548(TCB *task, void *work) {
    GSyncDisp *disp = work;
    s32 vcount = GX_GetVCount();
    int line = (vcount + (disp->waveLine + 1)) % 192;

    if (GX_IsHBlank()) {
        G2_SetBG3Offset(disp->waveTable[line], 0);
        if (disp->progressLine >= 192) {
            GX_LoadBGPltt(data_ov123_021daa88, 10 * 0x20 + 2, 2);
        } else if (disp->progressLine == 0) {
            GX_LoadBGPltt(&data_ov123_021daa88[9], 10 * 0x20 + 2, 2);
        } else if (vcount > 200) {
            GX_LoadBGPltt(data_ov123_021daa88, 10 * 0x20 + 2, 2);
        } else if (disp->progressLine > vcount) {
            GX_LoadBGPltt(data_ov123_021daa88, 10 * 0x20 + 2, 2);
        } else {
            int diff = vcount - disp->progressLine;

            if (diff < NELEMS(data_ov123_021daa88) && diff >= 0) {
                GX_LoadBGPltt(&data_ov123_021daa88[diff], 10 * 0x20 + 2, 2);
            } else {
                GX_LoadBGPltt(&data_ov123_021daa88[9], 10 * 0x20 + 2, 2);
            }
        }
    }
}

// pokebw2's GSyncDisp_StartWave
void func_ov123_021d65f4(GSyncDisp *disp) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_GSYNC, disp->heapId);

    GFL_G2DIOLoadNSCRSync(arc, 22, 3, 0, CHAR_POS(disp->mainBgChars), 0, FALSE, disp->heapId);
    GFL_ArcToolFree(arc);
    disp->hblankTask = GFL_HBlankTCBAdd(func_ov123_021d6548, disp, 0);
    func_02024230(disp->waveTable, 192, 0x5b0, 0x1800);
}

// pokebw2's GSyncDisp_StartFade
void func_ov123_021d6650(GSyncDisp *disp, BOOL fadeIn) {
    if (fadeIn) {
        disp->fade = 0;
        disp->fadeStep = 1;
    } else {
        // BUG: a fade out starts past the 10 that ends a fade, so it stops at once and leaves BG 3 shown
#ifdef BUGFIX
        disp->fade = FX32_CONST(10);
#else
        disp->fade = FX32_CONST(16);
#endif
        disp->fadeStep = -1;
    }
}

// Blends BG 3 in or out over the others
// pokebw2's GSyncDisp_UpdateFade
static void func_ov123_021d6678(GSyncDisp *disp) {
    int alpha;
    if (disp->fadeStep != 0) {
        disp->fade += disp->fadeStep << 10;
        alpha = disp->fade >> 12;
        GFL_BGSysSetBGEnabled(3, TRUE);
        if (alpha > 10) {
            disp->fadeStep = 0;
            return;
        }
        if (alpha < 0) {
            GFL_BGSysSetBGEnabled(3, FALSE);
            alpha = 0;
            disp->fadeStep = 0;
            return;
        }
        G2_SetBlendAlpha(8, 0x37, alpha, 8);
    }
}

// pokebw2's GSyncDisp_SetProgress
void func_ov123_021d66d0(GSyncDisp *disp, int percent) {
    f32 height = (f32)percent * 1.2;
    int line;

    if (percent > 100) {
        line = 0;
    } else {
        line = 120.0f - height;
    }
    if (disp->progressLine > line) {
        disp->progressLine = line;
    }
}

// pokebw2's GSyncDisp_StartPokemonFloat
static void func_ov123_021d6720(GSyncDisp *disp, int index) {
    func_ov123_021d6c48(disp, index, disp->floatPokemonActor);
    {
        ClActor *actor = disp->floatPokemonActor;
        ClActorPos pos = { 128, 0 };
        ClActorScale scale = { FX32_ONE, FX32_ONE };

        func_0204b404(actor, &pos, CLACT_SURFACE_MAIN);
        func_0204b5ac(actor, &scale);
        func_0204b92c(actor, FALSE);
        func_0204b3dc(actor, FALSE);
        func_0204b688(actor, 1);
        func_0204b570(actor, 2);
        disp->floats[index].actor = actor;
    }
}

// pokebw2's GSyncDisp_HideFloat
static void func_ov123_021d679c(GSyncDispFloat *fl) {
    if (fl->actor != NULL) {
        func_0204b3dc(fl->actor, FALSE);
    }
    fl->actor = NULL;
}

void func_ov123_021d67b4(GSyncDisp *disp, int index) {
    func_ov123_021d6c90(disp, index, disp->floatItemActor);
    {
        ClActor *actor = disp->floatItemActor;
        ClActorPos pos = { 156, 28 };
        ClActorScale scale = { FX32_ONE, FX32_ONE };

        func_0204b404(actor, &pos, CLACT_SURFACE_MAIN);
        func_0204b5ac(actor, &scale);
        func_0204b92c(actor, FALSE);
        func_0204b3dc(actor, FALSE);
        func_0204b688(actor, 1);
        func_0204b570(actor, 2);
        disp->floats[index].actor = actor;
    }
}


// pokebw2's GSyncDisp_StartItemFloat
// Moves a float down from the top of the screen, swaying, and shrinks it until it vanishes
// pokebw2's GSyncDisp_UpdateFloat
static void func_ov123_021d6830(GSyncDispFloat *fl) {
    ClActorPos pos;
    ClActorScale scale;
    s16 swayX;
    s16 swayY;

    if (fl == NULL) {
        return;
    }
    swayX = (s16)(FX_SinIdx(fl->angleX) * 4) >> FX32_SHIFT;
    swayY = (s16)(FX_SinIdx(fl->angleY) * 3) >> FX32_SHIFT;
    if (fl->actor != NULL) {
        func_0204b454(fl->actor, &pos, CLACT_SURFACE_MAIN);
        u16 x = fl->x;

        pos.x = x + swayX + swayY;
        pos.y = (fl->y + swayX + swayY + fl->timer) >> 2;
        if (fl->timer % 8 == 0) {
            if (x > 128) {
                fl->x = x - 1;
            } else {
                fl->x = x + 1;
            }
        }
        func_0204b404(fl->actor, &pos, CLACT_SURFACE_MAIN);
        func_0204b3dc(fl->actor, TRUE);
        fl->timer++;
        if (pos.y > 25) {
            func_0204b5c0(fl->actor, &scale);
            scale.x -= 16;
            scale.y -= 16;
            if (scale.x < 0 || scale.y < 0) {
                func_ov123_021d679c(fl);
            } else {
                func_0204b5ac(fl->actor, &scale);
            }
        }
        if (pos.y > 50) {
            func_ov123_021d679c(fl);
        }
    } else {
        fl->done = TRUE;
    }
    if (!fl->done) {
        fl->angleX += (u16)(GFL_RandomLC(12) + 200);
        fl->angleY -= (u16)(GFL_RandomLC(12) + 666);
    }
}

// Moves the floats. Another item starts when no item is floating, and another Pokémon when nothing is, since
// `floating` isn't cleared between the two
// pokebw2's GSyncDisp_UpdateFloats
void func_ov123_021d6934(GSyncDisp *disp) {
    BOOL floating = FALSE;
    int i;

    for (i = 0; i < GSYNC_DISP_FLOAT_ITEM_COUNT; i++) {
        if (disp->floats[i].active && !disp->floats[i].done) {
            func_ov123_021d6830(&disp->floats[i]);
            floating = TRUE;
        }
    }
    if (!floating) {
        for (i = 0; i < GSYNC_DISP_FLOAT_ITEM_COUNT; i++) {
            int index = GFL_RandomLC(GSYNC_DISP_FLOAT_ITEM_COUNT);

            if (!disp->floats[index].active && disp->floats[index].id != 0) {
                disp->floats[index].active = TRUE;
                disp->floats[index].x = GFL_RandomLC(128) + 64;
                disp->floats[index].y = 0;
                func_ov123_021d67b4(disp, index);
                break;
            }
        }
    }
    for (i = 0; i < GSYNC_DISP_FLOAT_POKEMON_COUNT; i++) {
        int index = i + GSYNC_DISP_FLOAT_ITEM_COUNT;

        if (disp->floats[index].active && !disp->floats[index].done) {
            func_ov123_021d6830(&disp->floats[index]);
            floating = TRUE;
        }
    }
    if (!floating) {
        for (i = 0; i < GSYNC_DISP_FLOAT_POKEMON_COUNT; i++) {
            int index = GFL_RandomLC(GSYNC_DISP_FLOAT_POKEMON_COUNT) + GSYNC_DISP_FLOAT_ITEM_COUNT;

            if (!disp->floats[index].active && disp->floats[index].id != 0) {
                disp->floats[index].active = TRUE;
                disp->floats[index].x = GFL_RandomLC(128) + 64;
                disp->floats[index].y = 0;
                func_ov123_021d6720(disp, index);
                return;
            }
        }
    }
}

// Sets a float's Pokémon, if it is one with an overworld model
// pokebw2's GSyncDisp_SetFloatPokemon
void func_ov123_021d6a70(GSyncDisp *disp, int index, int species, int form, int sex) {
    if (species != SPECIES_NONE && species <= SPECIES_ARCEUS) {
        disp->floats[index].id = species;
        disp->floats[index].form = form;
        if (sex > 2) {
            sex = 0;
        }
        disp->floats[index].sex = sex;
    }
}

// pokebw2's GSyncDisp_SetFloatItem
void func_ov123_021d6aa8(GSyncDisp *disp, int index, u16 item) {
    disp->floats[index].id = item;
}

// pokebw2's GSyncDisp_LoadFloatGraphics
void func_ov123_021d6ab8(GSyncDisp *disp) {
    NNSG2dCharacterData *chars;
    NNSG2dPaletteData *palette;
    ArcTool *itemArc;
    ArcTool *iconArc;
    void *file;
    int i;

    itemArc = GFL_ArcSysCreateFileHandle(ARCID_ITEMGRA, disp->heapId);
    for (i = 0; i < GSYNC_DISP_FLOAT_ITEM_COUNT; i++) {
        int item = disp->floats[i].id;

        if (item != ITEM_NONE) {
            file = GFL_ArcToolReadHeapNew(itemArc, GetItemGraphicsDatID(item, ITEM_FILE_ICON_PLTT), disp->heapId);
            NNS_G2dGetUnpackedPaletteData(file, &palette);
            MI_CpuCopy8(palette->pRawData, disp->floatGraphics[i].palette, sizeof(disp->floatGraphics[i].palette));
            GFL_HeapFree(file);
            file = GFL_ArcToolReadHeapNew(itemArc, GetItemGraphicsDatID(item, ITEM_FILE_ICON_CHAR), disp->heapId);
            NNS_G2dGetUnpackedBGCharacterData(file, &chars);
            MI_CpuCopy8(chars->pRawData, disp->floatGraphics[i].chars, sizeof(disp->floatGraphics[i].chars));
            GFL_HeapFree(file);
        }
    }
    GFL_ArcToolFree(itemArc);

    iconArc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, disp->heapId);
    for (i = 0; i < GSYNC_DISP_FLOAT_POKEMON_COUNT; i++) {
        int index = i + GSYNC_DISP_FLOAT_ITEM_COUNT;
        int species = disp->floats[index].id;

        if (species != SPECIES_NONE) {
            int sex = disp->floats[index].sex;
            u32 form = PML_PkmSanitizeForme(species, disp->floats[index].form);
            u32 paletteNum = func_0201bb5c(species, form, sex, FALSE);
            u16 icon = PokeParty_GetIconIndex(species, form, sex, FALSE);

            file = GFL_ArcToolReadHeapNew(iconArc, icon, disp->heapId);
            NNS_G2dGetUnpackedBGCharacterData(file, &chars);
            MI_CpuCopy8(chars->pRawData, disp->floatGraphics[index].chars, sizeof(disp->floatGraphics[index].chars));
            GFL_HeapFree(file);
            disp->floatGraphics[index].paletteNum = paletteNum;
        }
    }
    GFL_ArcToolFree(iconArc);
}

// pokebw2's GSyncDisp_UploadPokemonFloat
static void func_ov123_021d6c48(GSyncDisp *disp, int index, ClActor *actor) {
    NNSG2dImageProxy proxy;

    func_0204b7c8(actor, &proxy);
    MI_CpuCopy8(disp->floatGraphics[index].chars,
               (u8 *)HW_OBJ_VRAM + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN],
               sizeof(disp->floatGraphics[index].chars));
    func_0204b6f4(actor, (u8)disp->floatGraphics[index].paletteNum, 1);
}

// pokebw2's GSyncDisp_UploadItemFloat
static void func_ov123_021d6c90(GSyncDisp *disp, int index, ClActor *actor) {
    NNSG2dImageProxy proxy;

    func_0204b7c8(actor, &proxy);
    MI_CpuCopy8(disp->floatGraphics[index].chars,
               (u8 *)HW_OBJ_VRAM + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN],
               sizeof(disp->floatGraphics[index].chars));
    DC_FlushRange(disp->floatGraphics[index].palette, sizeof(disp->floatGraphics[index].palette));
    GX_LoadOBJPltt(disp->floatGraphics[index].palette, 8 * 0x20, sizeof(disp->floatGraphics[index].palette));
}

// The actor the floating Pokémon are drawn with, by turns
// pokebw2's GSyncDisp_CreatePokemonFloatActor
void func_ov123_021d6cd8(GSyncDisp *disp) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(7, disp->heapId);
    u16 iconIndex = PokeParty_GetIconIndex(1, 0, 0, FALSE);
    disp->res[9] = func_0204a6c8(arc, iconIndex, 0, 0, disp->heapId);
    ClActorSetup setup;
    setup.x = 0x85;
    setup.y = 0x80;
    setup.sequence = 0;
    setup.priority = 3;
    setup.bgPriority = 2;
    disp->actors[16] = func_0204b294(disp->clUnit, disp->res[9], disp->res[2], disp->res[15], &setup, 0, disp->heapId);
    func_0204b92c(disp->actors[16], FALSE);
    func_0204b3dc(disp->actors[16], FALSE);
    GFL_ArcToolFree(arc);
}

// The actor the floating items are drawn with, by turns
// pokebw2's GSyncDisp_CreateItemFloatActor
static void func_ov123_021d6d5c(GSyncDisp *disp) {
    ClActorSetup setup;
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ITEMGRA, disp->heapId);

    disp->res[GSYNC_DISP_RES_PLTT_ITEM] =
        func_0204ac18(arc, GetItemGraphicsDatID(ITEM_MASTER_BALL, ITEM_FILE_ICON_PLTT), CLACT_VRAM_MAIN, 8 * 0x20, 0, 1,
                      disp->heapId);
    disp->res[GSYNC_DISP_RES_CHAR_ITEM] = func_0204a6c8(
        arc, GetItemGraphicsDatID(ITEM_MASTER_BALL, ITEM_FILE_ICON_CHAR), FALSE, CLACT_VRAM_MAIN, disp->heapId);
    disp->res[GSYNC_DISP_RES_CELL_ITEM] = func_0204af28(arc, 1, 0, disp->heapId);
    GFL_ArcToolFree(arc);
    setup.x = 156;
    setup.y = 28;
    setup.sequence = 0;
    setup.priority = 0;
    setup.bgPriority = 2;
    disp->floatItemActor =
        func_0204b294(disp->clUnit, disp->res[GSYNC_DISP_RES_CHAR_ITEM], disp->res[GSYNC_DISP_RES_PLTT_ITEM],
                      disp->res[GSYNC_DISP_RES_CELL_ITEM], &setup, CLACT_SURFACE_MAIN, disp->heapId);
    func_0204b92c(disp->floatItemActor, FALSE);
    func_0204b3dc(disp->floatItemActor, FALSE);
}
