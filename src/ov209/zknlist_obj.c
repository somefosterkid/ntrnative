// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov299/zknlist_obj.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/ui/frame_list.h"
#include "app/zknlist_main.h"
#include "app/zknlist_obj.h"
#include "app/zukanlist.h"
#include "app/zukan_common.h"
#include "constants/arc.h"
#include "constants/species.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "nitro/gx.h"
#include "nnsys/g2d.h"
#include "pml/personal.h"
#include "pml/poke_graphic.h"
#include "save/pokedex.h"
#include "system/app_menu_common.h"
#include "system/blink_palanm.h"
#include "system/game_data.h"
#include "system/palanm.h"
#include "pml/poke_party.h"

void func_0204a8d4(u32 chars);
void func_0204ada4(u32 palette);
void func_0204afd8(u32 cellAnims);

// The actors of the Pokédex's list: the touch bar's buttons, the scroll bar, the buttons of the Pokédexes, the icons
// of the rows and the picture of the species under the cursor. The ROM doesn't name this file; zknlist_obj.c is a
// guess after the search screen's zknsearch_obj.c

// The resources: the icons of each screen have their own characters, and the two pictures their own of each kind
enum {
    CHAR_RES_ICON,
    CHAR_RES_ICON_SUB = CHAR_RES_ICON + ZKNLIST_ICON_COUNT,
    CHAR_RES_MON = CHAR_RES_ICON_SUB + ZKNLIST_ICON_COUNT,
    CHAR_RES_TOUCHBAR = CHAR_RES_MON + 2,
    CHAR_RES_LIST,
};

enum {
    RES_ICON,
    RES_ICON_SUB,
    RES_MON,
    RES_TOUCHBAR = RES_MON + 2,
    RES_LIST,
};

// The window that an actor's shape cuts out of the blending, which lib/nitro's GX_OAM_MODE_* doesn't have yet
#define OAM_MODE_OBJWND 2

// Where the icons are: their x, the rows' height and the height of the top screen's list
#define ICON_X 112
#define ROW_HEIGHT 24
#define TOP_LIST_HEIGHT 192

// The palettes of the two pictures
#define MON_PALETTE_OFFSET (11 * 0x20)

void func_ov209_021bd984(void);
static void ZknListObj_LoadResources(ZknListWork *wk);
static void ZknListObj_InitBlinkAnm(ZknListWork *wk);
static void ZknListObj_ExitBlinkAnm(ZknListWork *wk);
static void ZknListObj_FreeResources(ZknListWork *wk);
static void ZknListObj_CreateActors(ZknListWork *wk);
static void ZknListObj_DeleteActor(ZknListWork *wk, u32 id);
static void ZknListObj_DeleteActors(ZknListWork *wk);
static void ZknListObj_CreateIcons(ZknListWork *wk);
static void ZknListObj_LoadIcon(ZknListWork *wk, u32 id, u16 species, u16 form, u16 sex, BOOL touchScreen);
static u32 ZknListObj_FindFreeIcon(ZknListWork *wk, BOOL touchScreen);

// pokebw2's sZknListActorTemplates
static const ZukanActorTemplate data_ov209_021be9f0[] = {
    { { 252, 12, 2, 0, 1 }, CHAR_RES_LIST, RES_LIST, RES_LIST, 0, CLACT_SURFACE_MAIN },
    { { 252, 84, 3, 10, 1 }, CHAR_RES_LIST, RES_LIST, RES_LIST, 0, CLACT_SURFACE_MAIN },
    { { 48, 48, 6, 10, 1 }, CHAR_RES_LIST, RES_LIST, RES_LIST, 0, CLACT_SURFACE_MAIN },
    { { 232, 168, 1, 0, 1 }, CHAR_RES_TOUCHBAR, RES_TOUCHBAR, RES_TOUCHBAR, 0, CLACT_SURFACE_MAIN },
    { { 208, 168, 0, 0, 1 }, CHAR_RES_TOUCHBAR, RES_TOUCHBAR, RES_TOUCHBAR, 0, CLACT_SURFACE_MAIN },
    { { 188, 172, 6, 0, 1 }, CHAR_RES_TOUCHBAR, RES_TOUCHBAR, RES_TOUCHBAR, 0, CLACT_SURFACE_MAIN },
    { { 160, 168, 5, 0, 1 }, CHAR_RES_TOUCHBAR, RES_TOUCHBAR, RES_TOUCHBAR, 0, CLACT_SURFACE_MAIN },
    { { 136, 168, 4, 0, 1 }, CHAR_RES_TOUCHBAR, RES_TOUCHBAR, RES_TOUCHBAR, 0, CLACT_SURFACE_MAIN },
    { { 104, 180, 1, 0, 1 }, CHAR_RES_LIST, RES_LIST, RES_LIST, 0, CLACT_SURFACE_MAIN },
    { { 36, 180, 0, 0, 1 }, CHAR_RES_LIST, RES_LIST, RES_LIST, 0, CLACT_SURFACE_MAIN },
};

void func_ov209_021bd8d8(ZknListWork *wk, u32 id, BOOL visible);

BOOL func_ov209_021bd8f4(ZknListWork *wk, u32 id);

void func_ov209_021bd934(ZknListWork *wk, u32 id, s16 x, s16 y, u16 surface);

void func_ov209_021bd958(ZknListWork *wk, u32 id, s16 *x, s16 *y, u16 surface);

// pokebw2's ZknListObj_InitSystem

void func_ov209_021bdd5c(ZknListWork *wk);
void func_ov209_021bdba8(ZknListWork *wk);

void func_ov209_021bd9b4(ZknListWork *wk);
void func_ov209_021bdc08(ZknListWork *wk);
void func_ov209_021be010(ZknListWork *wk);
void func_ov209_021bd7f8(ZknListWork *wk) {
    func_ov209_021bd984();
    func_ov209_021bd9b4(wk);
    func_ov209_021bdc08(wk);
    func_ov209_021be010(wk);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

void func_ov209_021bd824(ZknListWork *wk) {
    func_ov209_021bdd5c(wk);
    func_ov209_021bdba8(wk);
    func_0204a5ac();
}

void func_ov209_021bd838(ZknListWork *wk) {
    u32 i;

    for (i = 0; i < 60; i++) {
        if (wk->actors[i] != NULL && func_0204b944(wk->actors[i]) != 1) {
            func_0204b8d4(wk->actors[i], 0x1000);
        }
    }
    func_0204a600();
}


void func_ov209_021bd86c(ZknListWork *wk, u32 id, u32 seq) {
    func_0204b8c4(wk->actors[id], 0);
    func_0204b878(wk->actors[id], (u16)seq);
}

void func_ov209_021bd890(ZknListWork *wk, u32 id, u32 anm) {
    func_ov209_021bd86c(wk, id, anm);
    func_0204b92c(wk->actors[id], 1);
}



u16 func_ov209_021bd8b0(ZknListWork *wk, u32 id) {
    return func_0204b898(wk->actors[id]);
}

BOOL func_ov209_021bd8c4(ZknListWork *wk, u32 id) {
    return func_0204b98c(wk->actors[id]);
}

void func_ov209_021bd8d8(ZknListWork *wk, u32 id, BOOL visible) {
    ClActor *actor = wk->actors[id];

    if (actor != NULL) {
        func_0204b3dc(actor, visible);
    }
}


BOOL func_ov209_021bd8f4(ZknListWork *wk, u32 id) {
    return func_0204b3f4(wk->actors[id]);
}

void func_ov209_021bd908(ZknListWork *wk, u32 id, u32 mode) {
    if (mode == 1) {
        func_0204b688(wk->actors[id], 1);
    } else {
        func_0204b688(wk->actors[id], 0);
    }
}


void func_ov209_021bd934(ZknListWork *wk, u32 id, s16 x, s16 y, u16 surface) {
    ClActorPos pos;

    pos.x = x;
    pos.y = y;
    func_0204b404(wk->actors[id], &pos, surface);
}

void func_ov209_021bd958(ZknListWork *wk, u32 id, s16 *x, s16 *y, u16 surface) {
    ClActorPos pos;

    func_0204b454(wk->actors[id], &pos, surface);
    *x = pos.x;
    *y = pos.y;
}






void func_ov209_021bd984(void) {
    ClActSysSetup setup = {
        0, 0,  0, 512, 4, 124, 4, 124, 0, ZKNLIST_CHAR_RES_COUNT, ZKNLIST_PALETTE_RES_COUNT, ZKNLIST_CELL_RES_COUNT,
        0, 16, 16
    };

    ClActSys_Create(&setup, func_ov209_021bc380(), HEAPID_ZUKAN_LIST);
}

void func_ov209_021bd9b4(ZknListWork *wk) {
    ArcTool *arc;
    u32 i;
    u32 *chars, *palette, *cells;

    for (i = 0; i < 36; i++) {
        wk->charRes[i] = 0xffffffff;
    }
    for (i = 0; i < 6; i++) {
        wk->paletteRes[i] = 0xffffffff;
    }
    for (i = 0; i < 6; i++) {
        wk->cellRes[i] = 0xffffffff;
    }

    arc = GFL_ArcSysCreateFileHandle(0x9f, HEAPID_ZUKAN_LIST | HEAPID_TAIL_BIT);
    wk->charRes[35] = func_0204a6c8(arc, 0x37, FALSE, 0, HEAPID_ZUKAN_LIST);
    wk->paletteRes[5] = func_0204abf0(arc, 0x33, 0, 0, HEAPID_ZUKAN_LIST);
    wk->cellRes[5] = func_0204af28(arc, 0x3d, 0x47, HEAPID_ZUKAN_LIST);
    GFL_ArcToolFree(arc);

    arc = GFL_ArcSysCreateFileHandle(7, HEAPID_ZUKAN_LIST | HEAPID_TAIL_BIT);
    chars = &wk->charRes[CHAR_RES_ICON];
    palette = &wk->paletteRes[RES_ICON];
    cells = &wk->cellRes[RES_ICON];
    for (i = 0; i < 16; i++) {
        chars[i] = func_0204a6c8(arc, PokeParty_GetIconIndex(0, 0, 0, FALSE), FALSE, CLACT_VRAM_MAIN,
                HEAPID_ZUKAN_LIST);
    }
    *palette = func_0204acec(arc, func_0201bc3c(), CLACT_VRAM_MAIN, 4 * 0x20, HEAPID_ZUKAN_LIST);
    *cells = func_0204af28(arc, func_0201bc40(), getOBJTileMapping_MainEng(), HEAPID_ZUKAN_LIST);
    chars = &wk->charRes[CHAR_RES_ICON_SUB];
    palette = &wk->paletteRes[RES_ICON_SUB];
    cells = &wk->cellRes[RES_ICON_SUB];
    for (i = 0; i < 16; i++) {
        chars[i] = func_0204a6c8(arc, PokeParty_GetIconIndex(0, 0, 0, FALSE), FALSE, CLACT_VRAM_SUB, HEAPID_ZUKAN_LIST);
    }
    *palette = func_0204acec(arc, func_0201bc3c(), CLACT_VRAM_SUB, 0 * 0x20, HEAPID_ZUKAN_LIST);
    *cells = func_0204af28(arc, func_0201bc78(), getOBJTileMapping_SubEng(), HEAPID_ZUKAN_LIST);
    GFL_ArcToolFree(arc);

    arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), HEAPID_ZUKAN_LIST | HEAPID_TAIL_BIT);
    wk->charRes[34] = func_0204a6c8(arc, func_02027618(), 0, 0, HEAPID_ZUKAN_LIST);
    wk->paletteRes[4] = func_0204abf0(arc, func_02027614(), 0, 0xe0, HEAPID_ZUKAN_LIST);
    wk->cellRes[4] = func_0204af28(arc, func_0202761c(2), func_02027620(2), HEAPID_ZUKAN_LIST);
    GFL_ArcToolFree(arc);
}


void func_ov209_021bdba8(ZknListWork *wk) {
    u32 i;

    for (i = 0; i < 36; i++) {
        if (wk->charRes[i] != 0xffffffff) {
            func_0204a8d4(wk->charRes[i]);
        }
    }
    for (i = 0; i < 6; i++) {
        if (wk->paletteRes[i] != 0xffffffff) {
            func_0204ada4(wk->paletteRes[i]);
        }
    }
    for (i = 0; i < 6; i++) {
        if (wk->cellRes[i] != 0xffffffff) {
            func_0204afd8(wk->cellRes[i]);
        }
    }
}

void func_ov209_021bdc08(ZknListWork *wk) {
    ZukanActorTemplate template;
    u32 i;

    wk->actorUnit = func_0204b100(60, 0, 0x6b);
    for (i = 0; i < 60; i++) {
        wk->actors[i] = NULL;
    }
    for (i = 0; i <= 9; i++) {
        template = data_ov209_021be9f0[i];
        template.chars = wk->charRes[template.chars];
        template.palette = wk->paletteRes[template.palette];
        template.cells = wk->cellRes[template.cells];
        wk->actors[i] = func_ov212_021c940c(wk->actorUnit, &template, 0x6b);
        func_0204ba20(wk->actors[i], 0);
    }
    if (wk->param->count <= 7) {
        func_ov209_021bd8d8(wk, 0, 0);
        func_ov209_021bd8d8(wk, 1, 0);
    }
    if (func_0200ca54(wk->param->pokedex) == 0) {
        func_ov209_021bd8d8(wk, 9, 0);
    }
    if (PokeDex_IsNationalObtained(wk->param->pokedex) == 0) {
        func_ov209_021bd8d8(wk, 8, 0);
    }
    if (wk->param->searched == 1) {
        func_ov209_021bd8d8(wk, 8, 0);
        func_ov209_021bd8d8(wk, 5, 0);
        return;
    }
    if (GameData_IsShortcutRegistered(wk->param->gameData, 0xf) == 1) {
        func_ov209_021bd890(wk, 5, 7);
    }
}



void func_ov209_021bdd40(ZknListWork *wk, u32 id) {
    if (wk->actors[id] != NULL) {
        func_0204b3b4(wk->actors[id]);
        wk->actors[id] = NULL;
    }
}

void func_ov209_021bdd40(ZknListWork *wk, u32 id);
void func_ov209_021bdd5c(ZknListWork *wk) {
    u32 i;

    for (i = 0; i < 60; i++) {
        func_ov209_021bdd40(wk, i);
    }
    func_0204b1cc(wk->actorUnit);
}

static const ZukanActorTemplate data_ov209_021be9bc = { { 0x30, 0x30, 0, 0, 1 }, 0x20, 2, 2, 0, 0 };
static const ZukanActorTemplate data_ov209_021be9a4 = { { 0x70, 0x08, 0, 0, 2 }, 0, 0, 0, 0, 0 };

void func_ov209_021bdd7c(ZknListWork *wk, u16 species) {
    ZukanActorTemplate template;
    ArcTool *arc;
    ClActor *actor;
    void *personal;
    u32 index;
    u32 hide;
    u32 size;
    u32 unk34;
    u32 unk30;
    u32 unk2c;
    u32 res;

    template = data_ov209_021be9bc;
    if (wk->monIndex == 0) {
        hide = 0x3b;
        index = 0x3a;
        size = 0x160;
    } else {
        hide = 0x3a;
        template.chars = 0x21;
        index = 0x3b;
        template.palette = 3;
        template.cells = 3;
        size = 0x180;
    }
    if (wk->actors[index] != NULL) {
        func_ov209_021bdd40(wk, index);
        if (wk->charRes[template.chars] != -1) {
            func_0204a8d4(wk->charRes[template.chars]);
        }
        if (wk->paletteRes[template.palette] != -1) {
            func_0204ada4(wk->paletteRes[template.palette]);
        }
        if (wk->cellRes[template.cells] != -1) {
            func_0204afd8(wk->cellRes[template.cells]);
        }
    }
    if (species == 0) {
        arc = GFL_ArcSysCreateFileHandle(0x9f, 0x806b);
        wk->charRes[template.chars] = func_0204a6c8(arc, 0x36, 0, 0, 0x6b);
        wk->paletteRes[template.palette] = func_0204ac18(arc, 0x32, 0, size, 0, 1, 0x6b);
        wk->cellRes[template.cells] = func_0204af28(arc, 0x3c, 0x46, 0x6b);
        GFL_ArcToolFree(arc);
        func_ov209_021bd8d8(wk, 2, 0);
    } else {
        func_0200cc40(wk->param->pokedex, species, &unk34, &unk2c, &unk30, 0x806b);
        arc = MakePokeGraArcHandle(0x806b);
        if (species == 0x147) {
            res = func_0200d288(wk->param->pokedex, 0);
        } else {
            res = 0;
        }
        wk->charRes[template.chars] = func_0202d81c(arc, species, unk30, unk34, unk2c, 0, 0, res, 0, 0x6b);
        wk->paletteRes[template.palette] = func_0202d7d8(arc, species, unk30, unk34, unk2c, 0, 0, 0, size, 0x6b);
        wk->cellRes[template.cells] = func_0202d898(species, unk30, unk34, unk2c, 0, 0, 2, 0, 0x6b);
        GFL_ArcToolFree(arc);
        func_ov209_021bd8d8(wk, 2, 1);
    }
    template.chars = wk->charRes[template.chars];
    template.palette = wk->paletteRes[template.palette];
    template.cells = wk->cellRes[template.cells];
    actor = (wk->actors[index] = func_ov212_021c940c(wk->actorUnit, &template, 0x6b));
    func_0204ba20(actor, 0);
    personal = PML_PersonalLoad(species, unk30, 0x806b);
    if (species != 0 && PML_PersonalGetParam(personal, 0x22) == 0) {
        func_0204b604(wk->actors[index], 1, 1);
    }
    PML_PersonalFree(personal);
    func_ov209_021bd8d8(wk, hide, 0);
    func_020216d4(wk->paletteFade, 2, (size >> 5) << 4, 0x20);
    wk->monIndex ^= 1;
}

void func_ov209_021be010(ZknListWork *wk) {
    ZukanActorTemplate template;
    u32 i;

    template = data_ov209_021be9a4;
    template.palette = wk->paletteRes[0];
    template.cells = wk->cellRes[0];
    for (i = 0; i < 16; i++) {
        template.setup.y = i * 0x18 + 8;
        template.chars = wk->charRes[i];
        wk->actors[10 + i] = func_ov212_021c940c(wk->actorUnit, &template, 0x6b);
        func_0204ba20(wk->actors[10 + i], 0);
    }
    template = data_ov209_021be9a4;
    template.palette = wk->paletteRes[1];
    template.cells = wk->cellRes[1];
    for (i = 0; i < 16; i++) {
        template.setup.y = i * 0x18 + 0x20;
        template.chars = wk->charRes[16 + i];
        template.surface = 1;
        wk->actors[26 + i] = func_ov212_021c940c(wk->actorUnit, &template, 0x6b);
        func_0204ba20(wk->actors[26 + i], 0);
        func_ov209_021bd908(wk, i + 0x1a, 1);
        wk->actors[42 + i] = func_ov212_021c940c(wk->actorUnit, &template, 0x6b);
        func_0204ba20(wk->actors[42 + i], 0);
        func_0204b688(wk->actors[42 + i], 2);
    }
    for (i = 0; i < 16; i++) {
        func_ov209_021bd8d8(wk, i + 10, 0);
    }
    for (i = 0; i < 16; i++) {
        func_ov209_021bd8d8(wk, i + 0x1a, 0);
        func_ov209_021bd8d8(wk, i + 0x2a, 0);
    }
}



void func_ov209_021bd890(ZknListWork *wk, u32 id, u32 anm);



void func_ov209_021be1b8(ZknListWork *wk, u32 id, u32 species, u16 form, u16 sex, BOOL touchScreen) {
    ArcTool *arc;
    void *ptr;
    NNSG2dCharacterData *charData;

    arc = GFL_ArcSysCreateFileHandle(7, 0x806b);
    ptr = GFL_G2DIOReadOBJNCGRArc(arc, PokeParty_GetIconIndex(species, form, sex, 0), 0, &charData, 0x806b);
    if (touchScreen == 1) {
        func_0204aa94(func_0204b7f0(wk->actors[id]), charData->pRawData, 0x400, 0, 0);
    } else {
        func_0204aa94(func_0204b7f0(wk->actors[id]), charData->pRawData, 0x400, 0, 1);
    }
    GFL_HeapFree(ptr);
    GFL_ArcToolFree(arc);
    func_0204b6f4(wk->actors[id], func_0201bb5c(species, form, sex, 0), 1);
}


u32 func_ov209_021be25c(ZknListWork *wk, u32 mode) {
    u32 i;

    if (mode == 1) {
        for (i = 10; i < 26; i++) {
            if (func_ov209_021bd8f4(wk, i) == 0) {
                return i;
            }
        }
    } else {
        for (i = 26; i < 42; i++) {
            if (func_ov209_021bd8f4(wk, i) == 0) {
                return i;
            }
        }
    }
    return 0;
}



void func_ov209_021be1b8(ZknListWork *wk, u32 id, u32 species, u16 form, u16 sex, BOOL touchScreen);
u32 func_ov209_021be25c(ZknListWork *wk, u32 mode);


void func_ov209_021be29c(ZknListWork *wk, u16 species, s16 y, BOOL touchScreen) {
    u32 sex;
    u32 form;
    u32 rare;
    u32 id;

    func_0200cc40(wk->param->pokedex, species, &sex, &rare, &form, HEAPID_ZUKAN_LIST | HEAPID_TAIL_BIT);
    id = func_ov209_021be25c(wk, touchScreen);
    func_ov209_021be1b8(wk, id, species, form, sex, touchScreen);
    func_ov209_021bd8d8(wk, id, 1);
    func_ov209_021bd890(wk, id, 0);
    if (touchScreen == 1) {
        func_ov209_021bd934(wk, id, ICON_X, y + 8, CLACT_SURFACE_MAIN);
    } else {
        func_ov209_021bd934(wk, id, ICON_X, y + 8, CLACT_SURFACE_SUB);
        func_ov209_021bd934(wk, id + ZKNLIST_ICON_COUNT, ICON_X, y + 8, CLACT_SURFACE_SUB);
        func_ov209_021bd8d8(wk, id + ZKNLIST_ICON_COUNT, 1);
    }
}





// pokebw2's ZknListObj_ScrollIcons
void func_ov209_021be34c(ZknListWork *wk, s8 delta) {
    u32 i;
    s16 x, y;

    for (i = ZKNLIST_OBJ_ICON; i < ZKNLIST_OBJ_ICON + ZKNLIST_ICON_COUNT; i++) {
        if (func_ov209_021bd8f4(wk, i)) {
            func_ov209_021bd958(wk, i, &x, &y, CLACT_SURFACE_MAIN);
            func_ov209_021bd934(wk, i, x, y + delta, CLACT_SURFACE_MAIN);
            if (y + delta <= -16 || y + delta >= TOP_LIST_HEIGHT + 8) {
                func_ov209_021bd8d8(wk, i, FALSE);
            }
        }
    }
    for (i = ZKNLIST_OBJ_ICON_SUB; i < ZKNLIST_OBJ_ICON_SUB + ZKNLIST_ICON_COUNT; i++) {
        if (func_ov209_021bd8f4(wk, i)) {
            func_ov209_021bd958(wk, i, &x, &y, CLACT_SURFACE_SUB);
            func_ov209_021bd934(wk, i, x, y + delta, CLACT_SURFACE_SUB);
            func_ov209_021bd934(wk, i + ZKNLIST_ICON_COUNT, x, y + delta, CLACT_SURFACE_SUB);
            if (y + delta <= -16 || y + delta >= TOP_LIST_HEIGHT + 8) {
                func_ov209_021bd8d8(wk, i, FALSE);
                func_ov209_021bd8d8(wk, i + ZKNLIST_ICON_COUNT, FALSE);
            }
        }
    }
}

// pokebw2's ZknListObj_SaveIcons
void func_ov209_021be43c(ZknListWork *wk) {
    u32 i;

    wk->shownIcons = 0;
    wk->shownIconsSub = 0;
    for (i = ZKNLIST_OBJ_ICON; i < ZKNLIST_OBJ_ICON + ZKNLIST_ICON_COUNT; i++) {
        if (func_ov209_021bd8f4(wk, i)) {
            wk->shownIcons |= 1 << (i - ZKNLIST_OBJ_ICON);
        }
    }
    for (i = ZKNLIST_OBJ_ICON_SUB; i < ZKNLIST_OBJ_ICON_SUB + ZKNLIST_ICON_COUNT; i++) {
        if (func_ov209_021bd8f4(wk, i)) {
            wk->shownIconsSub |= 1 << (i - ZKNLIST_OBJ_ICON_SUB);
        }
    }
}

// pokebw2's ZknListObj_HideSavedIcons
void func_ov209_021be4a8(ZknListWork *wk) {
    u32 i;

    for (i = ZKNLIST_OBJ_ICON; i < ZKNLIST_OBJ_ICON + ZKNLIST_ICON_COUNT; i++) {
        if (wk->shownIcons & (1 << (i - ZKNLIST_OBJ_ICON))) {
            func_ov209_021bd8d8(wk, i, FALSE);
        }
    }
    for (i = ZKNLIST_OBJ_ICON_SUB; i < ZKNLIST_OBJ_ICON_SUB + ZKNLIST_ICON_COUNT; i++) {
        if (wk->shownIconsSub & (1 << (i - ZKNLIST_OBJ_ICON_SUB))) {
            func_ov209_021bd8d8(wk, i, FALSE);
            func_ov209_021bd8d8(wk, i + ZKNLIST_ICON_COUNT, FALSE);
        }
    }
    wk->shownIcons = 0;
    wk->shownIconsSub = 0;
}

void func_ov209_021be51c(ZknListWork *wk, u32 row) {
    u32 i;
    s16 x;
    s16 y;

    row = row * ROW_HEIGHT + 8;
    for (i = 10; i < 26; i++) {
        if (func_ov209_021bd8f4(wk, i) == 0) {
            func_ov209_021bd890(wk, i, 0);
        } else {
            func_ov209_021bd958(wk, i, &x, &y, 0);
            if (row == y) {
                if (func_ov209_021bd8b0(wk, i) != 1) {
                    func_ov209_021bd890(wk, i, 1);
                }
            } else {
                func_ov209_021bd890(wk, i, 0);
            }
        }
    }
}


// pokebw2's ZknListObj_UpdateScrollBar
void func_ov209_021be588(ZknListWork *wk) {
    s16 x, y;
    u32 pos;

    func_ov209_021bd958(wk, ZKNLIST_OBJ_SCROLL_BAR, &x, &y, CLACT_SURFACE_MAIN);
    pos = func_ov067_021b8ae4(wk->frameList, y);
    if (pos < 12) {
        pos = 12;
    } else if (pos > 156) {
        pos = 156;
    }
    func_ov209_021bd934(wk, ZKNLIST_OBJ_SCROLL_BAR, 252, pos, CLACT_SURFACE_MAIN);
}

u16 func_ov209_021bd8b0(ZknListWork *wk, u32 id);
void func_ov209_021be5d4(ZknListWork *wk, BOOL keep) {
    s16 cursor = func_ov067_021b93b0(wk->frameList);

    if (func_ov209_021bd8b0(wk, ZKNLIST_OBJ_PAGE_UP) != 12 || keep == FALSE) {
        if (func_ov067_021b93b8(wk->frameList) == 0 && cursor == 0) {
            func_ov209_021bd890(wk, ZKNLIST_OBJ_PAGE_UP, 18);
        } else {
            func_ov209_021bd890(wk, ZKNLIST_OBJ_PAGE_UP, 4);
        }
    }
    if (func_ov209_021bd8b0(wk, ZKNLIST_OBJ_PAGE_DOWN) == 13 && keep != FALSE) {
        return;
    }
    if (func_ov067_021b93c0(wk->frameList) == FALSE && cursor == wk->listRows - 1) {
        func_ov209_021bd890(wk, ZKNLIST_OBJ_PAGE_DOWN, 19);
    } else {
        func_ov209_021bd890(wk, ZKNLIST_OBJ_PAGE_DOWN, 5);
    }
}


