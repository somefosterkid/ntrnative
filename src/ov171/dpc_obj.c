// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov256/dpc_obj.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/dpc_obj.h"
#include "app/dpc_main.h"
#include "gfl/arc.h"
#include "gfl/bmp.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/math_util.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "pml/poke_graphic.h"
#include "pml/poke_party.h"
#include "system/app_menu_common.h"
#include "system/bmp_oam.h"
#include "system/palanm.h"
#include "app/dendou_pc.h"

// The Hall of Fame PC's actors: the Pokémon on their ring, the buttons and the page number. The ROM doesn't name
// this file; the name is a guess, after box2_obj.c

#define DPCOBJ_RES_NONE 0xffffffff

// The common cells for the screen's OBJ VRAM mapping, 128 KB
#define DPCOBJ_MAPPING_128K 2

// The ring's center and radii, in pixels
#define DPCOBJ_RING_X 128
#define DPCOBJ_RING_Y 88
#define DPCOBJ_RING_RADIUS_X 100
#define DPCOBJ_RING_RADIUS_Y 44

// An actor and the resources it uses
typedef struct {
    ClActorSetup setup;
    u32 chars;
    u32 palette;
    u32 cellAnims;
    u16 unk14;
    u16 surface;
} DpcObjSetup;

// Where a Pokémon is on the ring, in degrees, and how dark it is there
typedef struct {
    u16 angle;
    u16 fade;
} DpcObjPlace;

static void func_ov171_021de114(void);
static void func_ov171_021de144(DendouPCWork *wk);
static void func_ov171_021de1f8(DendouPCWork *wk, u32 id);
static void func_ov171_021de218(DendouPCWork *wk, u32 id);
static void func_ov171_021de238(DendouPCWork *wk, u32 id);
static void func_ov171_021de258(DendouPCWork *wk);
static ClActor *func_ov171_021de290(DendouPCWork *wk, const DpcObjSetup *setup);
static void func_ov171_021de2d4(DendouPCWork *wk);
static void func_ov171_021de34c(DendouPCWork *wk, u32 id);
static void func_ov171_021de368(DendouPCWork *wk);
static void func_ov171_021de80c(DendouPCWork *wk);
static void func_ov171_021de884(DendouPCWork *wk);

// pokebw2's sDpcObjButtonSetups
static const DpcObjSetup data_ov171_021deba4[] = {
    { { 8, 168, 4, 0, 0 }, DPCOBJ_RES_BUTTON, DPCOBJ_RES_BUTTON, DPCOBJ_RES_BUTTON, 0, CLACT_SURFACE_MAIN },
    { { 88, 168, 5, 0, 0 }, DPCOBJ_RES_BUTTON, DPCOBJ_RES_BUTTON, DPCOBJ_RES_BUTTON, 0, CLACT_SURFACE_MAIN },
    { { 200, 168, 0, 0, 0 }, DPCOBJ_RES_BUTTON, DPCOBJ_RES_BUTTON, DPCOBJ_RES_BUTTON, 0, CLACT_SURFACE_MAIN },
    { { 232, 168, 1, 0, 0 }, DPCOBJ_RES_BUTTON, DPCOBJ_RES_BUTTON, DPCOBJ_RES_BUTTON, 0, CLACT_SURFACE_MAIN },
};

// The Pokémon; func_ov171_021de608 moves them onto the ring
// pokebw2's sDpcObjPokeSetups
static const DpcObjSetup data_ov171_021dec04[HALL_OF_FAME_TEAM_SIZE] = {
    { { 128, 132, 0, 0, 2 }, 0, 0, 0, 0, CLACT_SURFACE_MAIN }, { { 40, 124, 0, 1, 2 }, 1, 1, 1, 0, CLACT_SURFACE_MAIN },
    { { 224, 124, 0, 2, 2 }, 2, 2, 2, 0, CLACT_SURFACE_MAIN }, { { 128, 44, 0, 3, 2 }, 3, 3, 3, 0, CLACT_SURFACE_MAIN },
    { { 40, 52, 0, 4, 2 }, 4, 4, 4, 0, CLACT_SURFACE_MAIN },   { { 224, 52, 0, 5, 2 }, 5, 5, 5, 0, CLACT_SURFACE_MAIN },
};

// The places of a team's Pokémon by their number, the first at the front
// pokebw2's sDpcObjPlaces
static const DpcObjPlace data_ov171_021dec94[HALL_OF_FAME_TEAM_SIZE][HALL_OF_FAME_TEAM_SIZE] = {
    { { 90, 0 } },
    { { 90, 0 }, { 270, 12 } },
    { { 90, 0 }, { 210, 10 }, { 330, 10 } },
    { { 90, 0 }, { 180, 7 }, { 270, 12 }, { 0, 7 } },
    { { 90, 0 }, { 162, 8 }, { 234, 10 }, { 306, 10 }, { 18, 8 } },
    { { 90, 0 }, { 150, 8 }, { 210, 10 }, { 270, 12 }, { 330, 10 }, { 30, 8 } },
};

// pokebw2's DpcObj_Init
void func_ov171_021ddfe8(DendouPCWork *wk) {
    func_ov171_021de114();
    func_ov171_021de144(wk);
    func_ov171_021de2d4(wk);
    func_ov171_021de80c(wk);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's DpcObj_Exit
void func_ov171_021de014(DendouPCWork *wk) {
    func_ov171_021de884(wk);
    func_ov171_021de368(wk);
    func_ov171_021de258(wk);
    func_0204a5ac();
}

// pokebw2's DpcObj_Animate
void func_ov171_021de030(DendouPCWork *wk) {
    u32 i;

    for (i = 0; i < DPCOBJ_MAX; i++) {
        if (wk->actors[i] != NULL && func_0204b944(wk->actors[i]) != TRUE) {
            func_0204b8d4(wk->actors[i], FX32_ONE);
        }
    }
    func_0204a600();
}

// pokebw2's DpcObj_SetVisible
void func_ov171_021de064(DendouPCWork *wk, u32 id, BOOL visible) {
    if (wk->actors[id] != NULL) {
        func_0204b3dc(wk->actors[id], visible);
    }
}

// pokebw2's DpcObj_SetAnime
void func_ov171_021de080(DendouPCWork *wk, u32 id, u32 sequence) {
    func_0204b8c4(wk->actors[id], 0);
    func_0204b878(wk->actors[id], sequence);
    func_0204b92c(wk->actors[id], TRUE);
}

// pokebw2's DpcObj_IsAnimating
BOOL func_ov171_021de0ac(DendouPCWork *wk, u32 id) {
    return func_0204b98c(wk->actors[id]);
}

// pokebw2's DpcObj_SetPos
void func_ov171_021de0c0(DendouPCWork *wk, u32 id, s16 x, s16 y) {
    ClActorPos pos;

    pos.x = x;
    pos.y = y;
    func_0204b404(wk->actors[id], &pos, CLACT_SURFACE_MAIN);
}

// pokebw2's DpcObj_GetPos
void func_ov171_021de0e4(DendouPCWork *wk, u32 id, s16 *x, s16 *y) {
    ClActorPos pos;

    func_0204b454(wk->actors[id], &pos, CLACT_SURFACE_MAIN);
    *x = pos.x;
    *y = pos.y;
}

// pokebw2's DpcObj_InitClAct
static void func_ov171_021de114(void) {
    ClActSysSetup setup = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 28, DPCOBJ_RES_MAX, 28, 0, 16, 16 };

    ClActSys_Create(&setup, func_ov171_021dc924(), HEAPID_DENDOU_PC);
}

// pokebw2's DpcObj_InitResources
static void func_ov171_021de144(DendouPCWork *wk) {
    ArcTool *arc;
    u32 i;

    for (i = 0; i < DPCOBJ_RES_MAX; i++) {
        wk->chars[i] = DPCOBJ_RES_NONE;
    }
    for (i = 0; i < DPCOBJ_RES_MAX; i++) {
        wk->palettes[i] = DPCOBJ_RES_NONE;
    }
    for (i = 0; i < DPCOBJ_RES_MAX; i++) {
        wk->cellAnims[i] = DPCOBJ_RES_NONE;
    }

    arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), HEAPID_TAIL(HEAPID_DENDOU_PC));
    wk->chars[DPCOBJ_RES_BUTTON] = func_0204a6c8(arc, func_02027618(), FALSE, CLACT_VRAM_MAIN, HEAPID_DENDOU_PC);
    wk->palettes[DPCOBJ_RES_BUTTON] =
        func_0204abf0(arc, func_02027614(), CLACT_VRAM_MAIN, DPCOBJ_RES_BUTTON * 0x20, HEAPID_DENDOU_PC);
    wk->cellAnims[DPCOBJ_RES_BUTTON] =
        func_0204af28(arc, func_0202761c(DPCOBJ_MAPPING_128K), func_02027620(DPCOBJ_MAPPING_128K), HEAPID_DENDOU_PC);
    GFL_ArcToolFree(arc);
}

// pokebw2's DpcObj_FreeChars
static void func_ov171_021de1f8(DendouPCWork *wk, u32 id) {
    if (wk->chars[id] != DPCOBJ_RES_NONE) {
        func_0204a8d4(wk->chars[id]);
        wk->chars[id] = DPCOBJ_RES_NONE;
    }
}

// pokebw2's DpcObj_FreePalette
static void func_ov171_021de218(DendouPCWork *wk, u32 id) {
    if (wk->palettes[id] != DPCOBJ_RES_NONE) {
        func_0204ada4(wk->palettes[id]);
        wk->palettes[id] = DPCOBJ_RES_NONE;
    }
}

// pokebw2's DpcObj_FreeCellAnims
static void func_ov171_021de238(DendouPCWork *wk, u32 id) {
    if (wk->cellAnims[id] != DPCOBJ_RES_NONE) {
        func_0204afd8(wk->cellAnims[id]);
        wk->cellAnims[id] = DPCOBJ_RES_NONE;
    }
}

// pokebw2's DpcObj_FreeResources
static void func_ov171_021de258(DendouPCWork *wk) {
    u32 i;

    for (i = 0; i < DPCOBJ_RES_MAX; i++) {
        func_ov171_021de1f8(wk, i);
    }
    for (i = 0; i < DPCOBJ_RES_MAX; i++) {
        func_ov171_021de218(wk, i);
    }
    for (i = 0; i < DPCOBJ_RES_MAX; i++) {
        func_ov171_021de238(wk, i);
    }
}

// pokebw2's DpcObj_AddActor
static ClActor *func_ov171_021de290(DendouPCWork *wk, const DpcObjSetup *setup) {
    return func_0204b294(wk->unit, wk->chars[setup->chars], wk->palettes[setup->palette],
                         wk->cellAnims[setup->cellAnims], &setup->setup, setup->surface, HEAPID_DENDOU_PC);
}

// pokebw2's DpcObj_AddButtons
static void func_ov171_021de2d4(DendouPCWork *wk) {
    u32 i;

    wk->unit = func_0204b100(31, 0, HEAPID_DENDOU_PC);
    for (i = 0; i < DPCOBJ_MAX; i++) {
        wk->actors[i] = NULL;
    }
    for (i = DPCOBJ_ARROW_LEFT; i <= DPCOBJ_RETURN; i++) {
        u32 off = i * sizeof(DpcObjSetup) - DPCOBJ_ARROW_LEFT * sizeof(DpcObjSetup);
        wk->actors[i] = func_ov171_021de290(wk, (DpcObjSetup *)((u8 *)data_ov171_021deba4 + off));
    }

    // A single page has no arrows
    if (wk->pageMax == 1) {
        func_ov171_021de064(wk, DPCOBJ_ARROW_LEFT, FALSE);
        func_ov171_021de064(wk, DPCOBJ_ARROW_RIGHT, FALSE);
    }
}

// pokebw2's DpcObj_DeleteActor
static void func_ov171_021de34c(DendouPCWork *wk, u32 id) {
    if (wk->actors[id] != NULL) {
        func_0204b3b4(wk->actors[id]);
        wk->actors[id] = NULL;
    }
}

// pokebw2's DpcObj_DeleteActors
static void func_ov171_021de368(DendouPCWork *wk) {
    u32 i;

    for (i = 0; i < DPCOBJ_MAX; i++) {
        func_ov171_021de34c(wk, i);
    }
    func_0204b1cc(wk->unit);
}

// pokebw2's DpcObj_LoadPokemon
void func_ov171_021de388(DendouPCWork *wk) {
    ArcTool *arc;
    DendouPCTeam *team;
    HallOfFamePokemon *poke;
    DpcObjSetup setup;
    ClActor **actor;
    BOOL rare;
    u32 i;
    u16 id;

    for (i = DPCOBJ_POKE_A; i <= DPCOBJ_POKE_B + HALL_OF_FAME_TEAM_SIZE - 1; i++) {
        func_ov171_021de064(wk, i, FALSE);
    }

    team = &wk->teams[wk->page];
    arc = MakePokeGraArcHandle(HEAPID_TAIL(HEAPID_DENDOU_PC));

    for (i = 0; i < team->count; i++) {
        setup = data_ov171_021dec04[i];
        poke = &team->pokemon[i];
        rare = PML_UtilPIDIsRare(poke->trainerId, poke->personality);

        if (wk->objSwap == 1) {
            setup.chars += HALL_OF_FAME_TEAM_SIZE;
            setup.palette += HALL_OF_FAME_TEAM_SIZE;
            setup.cellAnims += HALL_OF_FAME_TEAM_SIZE;
            id = i + DPCOBJ_POKE_B;
            actor = &wk->actors[id];
        } else {
            id = i;
            actor = &wk->actors[id];
        }

        wk->chars[setup.chars] = func_0202d81c(arc, poke->species, poke->form, poke->sex, rare, 0, FALSE,
                                                        poke->personality, CLACT_VRAM_MAIN, HEAPID_DENDOU_PC);
        wk->palettes[setup.palette] = func_0202d7d8(arc, poke->species, poke->form, poke->sex, rare, 0,
                                                               FALSE, CLACT_VRAM_MAIN, id * 0x20, HEAPID_DENDOU_PC);
        wk->cellAnims[setup.cellAnims] =
            func_0202d898(poke->species, poke->form, poke->sex, rare, 0, FALSE,
                                       DPCOBJ_MAPPING_128K, CLACT_VRAM_MAIN, HEAPID_DENDOU_PC);
        *actor = func_ov171_021de290(wk, &setup);

        func_ov171_021de608(wk, id, data_ov171_021dec94[team->count - 1][i].angle);
        wk->angle[i] = data_ov171_021dec94[team->count - 1][i].angle;
    }

    func_020216d4(wk->palFade, PALFADE_VRAM_MAIN_OBJ, 0, 0x200);
    GFL_ArcToolFree(arc);

    // Free the set that the last page used
    if (wk->objSwap == 1) {
        for (i = DPCOBJ_POKE_A; i <= DPCOBJ_POKE_A + HALL_OF_FAME_TEAM_SIZE - 1; i++) {
            func_ov171_021de34c(wk, i);
        }
        for (i = DPCOBJ_POKE_A; i <= DPCOBJ_POKE_A + HALL_OF_FAME_TEAM_SIZE - 1; i++) {
            func_ov171_021de1f8(wk, i);
        }
        for (i = DPCOBJ_POKE_A; i <= DPCOBJ_POKE_A + HALL_OF_FAME_TEAM_SIZE - 1; i++) {
            func_ov171_021de218(wk, i);
        }
        for (i = DPCOBJ_POKE_A; i <= DPCOBJ_POKE_A + HALL_OF_FAME_TEAM_SIZE - 1; i++) {
            func_ov171_021de238(wk, i);
        }
    } else {
        for (i = DPCOBJ_POKE_B; i <= DPCOBJ_POKE_B + HALL_OF_FAME_TEAM_SIZE - 1; i++) {
            func_ov171_021de34c(wk, i);
        }
        for (i = DPCOBJ_POKE_B; i <= DPCOBJ_POKE_B + HALL_OF_FAME_TEAM_SIZE - 1; i++) {
            func_ov171_021de1f8(wk, i);
        }
        for (i = DPCOBJ_POKE_B; i <= DPCOBJ_POKE_B + HALL_OF_FAME_TEAM_SIZE - 1; i++) {
            func_ov171_021de218(wk, i);
        }
        for (i = DPCOBJ_POKE_B; i <= DPCOBJ_POKE_B + HALL_OF_FAME_TEAM_SIZE - 1; i++) {
            func_ov171_021de238(wk, i);
        }
    }
    wk->objSwap ^= 1;

    func_ov171_021de684(wk);
}

// pokebw2's DpcObj_GetPokeBase
u32 func_ov171_021de5f4(DendouPCWork *wk) {
    if (wk->objSwap == 0) {
        return DPCOBJ_POKE_B;
    }
    return DPCOBJ_POKE_A;
}

// pokebw2's DpcObj_SetPokePos
void func_ov171_021de608(DendouPCWork *wk, u32 id, s32 angle) {
    // FX_MUL with an unsigned 64-bit product: the original calls _ll_mul, FX_MUL's signed one calls _ll_mul
    fx32 x = (fx32)(((u64)func_0203f6d8(angle) * (DPCOBJ_RING_RADIUS_X * FX32_ONE) + 0x800LL) >> FX32_SHIFT);
    fx32 y = (fx32)(((u64)func_0203f684(angle) * (DPCOBJ_RING_RADIUS_Y * FX32_ONE) + 0x800LL) >> FX32_SHIFT);

    func_ov171_021de0c0(wk, id, FX_Whole((x + DPCOBJ_RING_X * FX32_ONE) & 0x7ffff000),
                        FX_Whole((y + DPCOBJ_RING_Y * FX32_ONE) & 0x7ffff000));
}

// pokebw2's DpcObj_SortPriority
void func_ov171_021de684(DendouPCWork *wk) {
    u8 ids[HALL_OF_FAME_TEAM_SIZE];
    s16 x, y;
    s16 otherX, otherY;
    u32 base;
    u8 i, j;
    u8 tmp;

    base = func_ov171_021de5f4(wk);
    for (i = 0; i < HALL_OF_FAME_TEAM_SIZE; i++) {
        ids[i] = base + i;
    }

    // The lower on the screen, the nearer
    for (i = 0; i < wk->teams[wk->page].count - 1; i++) {
        func_ov171_021de0e4(wk, ids[i], &x, &y);
        for (j = i + 1; j < wk->teams[wk->page].count; j++) {
            func_ov171_021de0e4(wk, ids[j], &otherX, &otherY);
            if (y < otherY) {
                tmp = ids[i];
                ids[i] = ids[j];
                ids[j] = tmp;
                y = otherY;
            }
        }
    }

    for (i = 0; i < wk->teams[wk->page].count; i++) {
        if (wk->actors[ids[i]] != NULL) {
            func_0204b800(wk->actors[ids[i]], i);
        }
    }
}

// pokebw2's DpcObj_SetPokeFade
void func_ov171_021de76c(DendouPCWork *wk, BOOL on) {
    u16 pos = wk->pokePos;
    u16 i;

    for (i = 0; i < wk->teams[wk->page].count; i++) {
        if (on == TRUE) {
            wk->fade[pos] = data_ov171_021dec94[wk->teams[wk->page].count - 1][i].fade;
            wk->fadeNow[pos] = wk->fade[pos];
            pos++;
            if (pos >= wk->teams[wk->page].count) {
                pos = 0;
            }
        } else {
            wk->fade[i] = 0;
            wk->fadeNow[i] = 0;
        }
    }
}

// pokebw2's DpcObj_InitPageOam
static void func_ov171_021de80c(DendouPCWork *wk) {
    BmpOamActorSetup setup;

    wk->bmpOam = func_02025538(HEAPID_DENDOU_PC, wk->unit);
    wk->pageBitmap = GFL_BitmapCreate(5, 3, 0x20, HEAPID_DENDOU_PC);

    setup.bitmap = wk->pageBitmap;
    setup.x = 40;
    setup.y = 168;
    setup.palette = wk->palettes[DPCOBJ_RES_BUTTON];
    setup.paletteOffset = 1;
    setup.priority = 0;
    setup.bgPriority = 0;
    setup.surface = 0xffff;
    setup.vramType = CLACT_VRAM_MAIN;
    wk->pageActor = func_020255a0(wk->bmpOam, &setup);

    if (wk->pageMax == 1) {
        func_02025774(wk->pageActor, FALSE);
    }
}

// pokebw2's DpcObj_ExitPageOam
static void func_ov171_021de884(DendouPCWork *wk) {
    func_0202570c(wk->pageActor);
    GFL_BitmapFree(wk->pageBitmap);
    func_02025588(wk->bmpOam);
}
