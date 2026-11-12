// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov209/mus_poke_draw.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/musical/mus_poke_draw.h"
#include "app/musical/musical_mcss.h"
#include "constants/arc.h"
#include "field/musical.h"
#include "gfl/heap.h"
#include "pml/poke_graphic.h"
#include "pml/poke_party.h"
#include "system/mcss.h"
#include "app/musical/musical_system.h"

// Overlay 209's mus_poke_draw.c: the musical's Pokémon, each a sprite of musical_mcss.c with its front and maybe its
// back, and the places of its props, which the sprite's marker cells give as it is drawn

// The marker cells: two of kinds 4 and 5, and one for each prop position from kind 7
#define MUS_POKE_DRAW_CELL_MARK4 4
#define MUS_POKE_DRAW_CELL_MARK5 5
#define MUS_POKE_DRAW_CELL_EQUIP 7

static void func_ov112_021f546c(u32 kind, MusicalMcssCellInfo *info, void *work);
static void func_ov112_021f5518(MusicalPoke *musPoke, MCSSLoadInfo *info, BOOL back);

// pokebw2's MusPokeDraw_InitSystem
MusPokeDrawSys *func_ov112_021f5164(HeapID heapId) {
    int i;
    MusPokeDrawSys *sys = GFL_HeapAllocate(heapId, sizeof(MusPokeDrawSys), FALSE, "mus_poke_draw.c", 76);

    sys->heapId = heapId;
    sys->mcssSys = func_ov112_021f55f8(8, heapId);
    for (i = 0; i < 8; i++) {
        sys->pokes[i].active = FALSE;
    }
    func_ov112_021f621c(sys->mcssSys);
    return sys;
}

// pokebw2's MusPokeDraw_TermSystem
void func_ov112_021f51b0(MusPokeDrawSys *sys) {
    int i;

    for (i = 0; i < 8; i++) {
        if (sys->pokes[i].active == TRUE) {
            func_ov112_021f5330(sys, &sys->pokes[i]);
        }
    }
    func_ov112_021f5644(sys->mcssSys);
    GFL_HeapFree(sys);
}

// pokebw2's MusPokeDraw_UpdateSystem
void func_ov112_021f51e8(MusPokeDrawSys *sys) {
    func_ov112_021f5678(sys->mcssSys);
}

// pokebw2's MusPokeDraw_DrawSystem
void func_ov112_021f51f4(MusPokeDrawSys *sys) {
    func_ov112_021f56bc(sys->mcssSys, func_ov112_021f546c);
}

// pokebw2's MusPokeDraw_AddPoke
MusPokeDraw *func_ov112_021f5204(MusPokeDrawSys *sys, MusicalPoke *musPoke, BOOL withBack) {
    int i;
    int j;
    MusPokeDraw *poke;
    VecFx32 scale;
    MCSSLoadInfo info;

    for (i = 0; i < 8; i++) {
        if (sys->pokes[i].active == FALSE) {
            break;
        }
    }
    sys->pokes[i].active = TRUE;
    poke = &sys->pokes[i];
    func_ov112_021f5518(musPoke, &info, FALSE);
    sys->pokes[i].front = func_ov112_021f611c(sys->mcssSys, 0, 0, 0, &info, poke, FALSE);
    if (withBack == TRUE) {
        func_ov112_021f5518(musPoke, &info, TRUE);
        sys->pokes[i].back = func_ov112_021f611c(sys->mcssSys, 0, 0, 0, &info, poke, FALSE);
        func_ov112_021f62dc(sys->pokes[i].back);
    } else {
        sys->pokes[i].back = NULL;
    }
    sys->pokes[i].isFront = TRUE;
    sys->pokes[i].mcss = sys->pokes[i].front;
    scale.x = FX32_CONST(16);
    scale.y = FX32_CONST(16);
    scale.z = FX32_ONE;
    for (j = 0; j < 9; j++) {
        sys->pokes[i].equips[j].valid = FALSE;
    }
    sys->pokes[i].markValid4 = FALSE;
    sys->pokes[i].markValid5 = FALSE;
    sys->pokes[i].markPos4.x = 0;
    sys->pokes[i].markPos4.y = 0;
    sys->pokes[i].markPos4.z = 0;
    sys->pokes[i].markPos5.x = 0;
    sys->pokes[i].markPos5.y = 0;
    sys->pokes[i].markPos5.z = 0;
    func_ov112_021f535c(&sys->pokes[i], &scale);
    func_ov112_021f6290(sys->pokes[i].mcss);
    return &sys->pokes[i];
}

// pokebw2's MusPokeDraw_DelPoke
void func_ov112_021f5330(MusPokeDrawSys *sys, MusPokeDraw *poke) {
    if (poke->back != NULL) {
        func_ov112_021f61d8(sys->mcssSys, poke->back);
    }
    func_ov112_021f61d8(sys->mcssSys, poke->front);
    poke->active = FALSE;
}

// pokebw2's MusPokeDraw_SetPosition
void func_ov112_021f5350(MusPokeDraw *poke, VecFx32 *pos) {
    func_ov112_021f6234(poke->mcss, pos);
}

// pokebw2's MusPokeDraw_SetScale
void func_ov112_021f535c(MusPokeDraw *poke, VecFx32 *scale) {
    func_ov112_021f624c(poke->mcss, scale);
}

// pokebw2's MusPokeDraw_SetRotation
void func_ov112_021f5368(MusPokeDraw *poke, u16 rotation) {
    func_ov112_021f6264(poke->mcss, rotation);
}

// pokebw2's MusPokeDraw_SetVisible
void func_ov112_021f5374(MusPokeDraw *poke, BOOL visible) {
    if (visible == TRUE) {
        func_ov112_021f62ec(poke->mcss);
    } else {
        func_ov112_021f62dc(poke->mcss);
    }
}

// pokebw2's MusPokeDraw_IsVisible
BOOL func_ov112_021f5388(MusPokeDraw *poke) {
    if (func_ov112_021f62d0(poke->mcss) == TRUE) {
        return FALSE;
    }
    return TRUE;
}

// pokebw2's MusPokeDraw_StartAnime
void func_ov112_021f539c(MusPokeDraw *poke) {
    func_ov112_021f62a0(poke->front);
    if (poke->back != NULL) {
        func_ov112_021f62a0(poke->back);
    }
}

// pokebw2's MusPokeDraw_StopAnime
void func_ov112_021f53b4(MusPokeDraw *poke) {
    func_ov112_021f6290(poke->front);
    if (poke->back != NULL) {
        func_ov112_021f6290(poke->back);
    }
}

// pokebw2's MusPokeDraw_ChangeAnime
void func_ov112_021f53cc(MusPokeDraw *poke, u16 anime) {
    func_ov112_021f62b0(poke->mcss, anime);
}

// pokebw2's MusPokeDraw_TurnAround
void func_ov112_021f53d8(MusPokeDraw *poke) {
    if (poke->isFront == TRUE) {
        func_ov112_021f62fc(poke->front, poke->back);
        poke->mcss = poke->back;
        poke->isFront = FALSE;
        func_ov112_021f62dc(poke->front);
    } else {
        func_ov112_021f62fc(poke->back, poke->front);
        poke->mcss = poke->front;
        poke->isFront = TRUE;
        func_ov112_021f62dc(poke->back);
    }
}

// pokebw2's MusPokeDraw_SetFront
void func_ov112_021f5414(MusPokeDraw *poke, BOOL front) {
    if (front != poke->isFront) {
        func_ov112_021f53d8(poke);
    }
}

// pokebw2's MusPokeDraw_SetFlip
void func_ov112_021f5424(MusPokeDraw *poke, BOOL flip) {
    if (flip == TRUE) {
        func_ov112_021f626c(poke->mcss);
    } else {
        func_ov112_021f6280(poke->mcss);
    }
}

// pokebw2's MusPokeDraw_SetTexBase
void func_ov112_021f5438(MusPokeDrawSys *sys, u32 base) {
    func_ov112_021f622c(sys->mcssSys, base);
}

// pokebw2's MusPokeDraw_SetPlttBase
void func_ov112_021f5444(MusPokeDrawSys *sys, u32 base) {
    func_ov112_021f6230(sys->mcssSys, base);
}

// pokebw2's MusPokeDraw_GetEquipPos
MusPokeDrawEquipPos *func_ov112_021f5450(MusPokeDraw *poke, u32 pos) {
    return &poke->equips[pos];
}

// pokebw2's MusPokeDraw_GetMarkPos4
VecFx32 *func_ov112_021f545c(MusPokeDraw *poke) {
    return &poke->markPos4;
}

// pokebw2's MusPokeDraw_GetMarkPos5
VecFx32 *func_ov112_021f5464(MusPokeDraw *poke) {
    return &poke->markPos5;
}

// pokebw2's MusPokeDraw_CellCallback
static void func_ov112_021f546c(u32 kind, MusicalMcssCellInfo *info, void *work) {
    MusPokeDraw *poke = work;

    if (kind >= MUS_POKE_DRAW_CELL_EQUIP) {
        enum { MUS_POKE_DRAW_EQUIP_MAX = 9 } pos = (int)(kind - MUS_POKE_DRAW_CELL_EQUIP);

        poke->equips[pos].valid = TRUE;
        poke->equips[pos].info.pos = info->pos;
        poke->equips[pos].info.offset = info->offset;
        poke->equips[pos].info.rotation = info->rotation;
        poke->equips[pos].info.cellRotation = info->cellRotation;
        poke->equips[pos].info.scale = info->scale;
        poke->equips[pos].info.center = info->center;
    } else if (kind == MUS_POKE_DRAW_CELL_MARK4) {
        fx32 x = info->offset.x;
        fx32 y = info->offset.y;

        poke->markPos4.x = x;
        poke->markPos4.y = y;
        poke->markPos4.z = 0;
        poke->markValid4 = TRUE;
    } else if (kind == MUS_POKE_DRAW_CELL_MARK5) {
        fx32 x = info->offset.x;
        fx32 y = info->offset.y;

        poke->markPos5.x = x;
        poke->markPos5.y = y;
        poke->markPos5.z = 0;
        poke->markValid5 = TRUE;
    }
}

// pokebw2's MusPokeDraw_GetLoadInfo
static void func_ov112_021f5518(MusicalPoke *musPoke, MCSSLoadInfo *info, BOOL back) {
    u32 species;
    u32 sex;
    u32 form;
    u8 rare;
    u32 dir;

    func_0201d624(musPoke->pkm);
    species = musPoke->species;
    form = PML_PkmSanitizeForme(species, musPoke->form);
    sex = musPoke->sex;
    rare = musPoke->rare;
    dir = POKEGRA_DIR_FRONT;
    form = func_02019d58(musPoke->species, form);
    if (back == TRUE) {
        dir = POKEGRA_DIR_BACK;
    }
    info->arcId = 0x34;
    info->character = GetPokemonCharacterDataNo(info->arcId, species, form, sex, rare, dir, FALSE);
    info->palette = GetPokemonPaletteDataNo(info->arcId, species, form, sex, rare, dir, FALSE);
    info->cells = GetPokemonCellsDataNo(info->arcId, species, form, sex, rare, dir, FALSE);
    info->cellAnime = GetPokemonCellAnimeDataNo(info->arcId, species, form, sex, rare, dir, FALSE);
    info->multiCells = GetPokemonMultiCellsDataNo(info->arcId, species, form, sex, rare, dir, FALSE);
    info->multiCellAnime = GetPokemonMultiCellAnimeDataNo(info->arcId, species, form, sex, rare, dir, FALSE);
    info->bin = GetPokemonBinFileDataNo(info->arcId, species, form, sex, rare, dir, FALSE);
    info->unk20 = musPoke->personality;
}
