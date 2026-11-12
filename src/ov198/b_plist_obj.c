// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov287/b_plist_obj.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "battle/b_plist_obj.h"
#include "battle/b_app_tool.h"
#include "battle/b_plist_main.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "pml/item.h"
#include "pml/poke_party.h"
#include "pml/waza.h"
#include "system/app_menu_common.h"
#include "system/hp_gauge.h"
#include "system/palanm.h"

// The battle party list's OBJ resources and cell actors: the Pokémon icons with their status and item icons, the
// type and category icons of the summary pages, and overlay 285's cursor. The file name is our guess, after the
// ROM's b_plist_main.c and b_plist_anm.c and the b_plist_* files of the earlier games.

// How func_ov198_0220e684 creates each of the list's actors, by index into the work's resources
typedef struct {
    u32 chars;
    u32 palette;
    u32 cellAnims;
    u32 bgPriority;
    u32 priority;
} BPlistActorSetup;

void func_ov198_0220e2f8(BPlistWork *wk);
static void func_ov198_0220e340(BPlistWork *wk);
void func_ov198_0220e3f0(BPlistWork *wk);
void func_ov198_0220e460(BPlistWork *wk);
void func_ov198_0220e518(BPlistWork *wk);
void func_ov198_0220e584(BPlistWork *wk);
void func_ov198_0220e5fc(BPlistWork *wk);
ClActor *func_ov198_0220e684(BPlistWork *wk, const BPlistActorSetup *data);
static void func_ov198_0220e6e0(BPlistWork *wk);
static void func_ov198_0220e7cc(ClActor *actor, const ClActorPos *pos);
void func_ov198_0220e7e4(BPlistWork *wk);
void func_ov198_0220e82c(BPlistWork *wk, ClActor *actor, u32 chars, u32 type);
void func_ov198_0220e8b4(BPlistWork *wk, u32 actor, u32 chars, u32 category);
static void func_ov198_0220e974(u8 status, ClActor *actor, const ClActorPos *pos);
void func_ov198_0220e9a0(BPlistWork *wk, BPlistPokemon *pkm, const ClActorPos *pos);
static void func_ov198_0220ea30(u16 item, ClActor *actor, const ClActorPos *pos);
static void func_ov198_0220eaf0(BPlistWork *wk);
static void func_ov198_0220ebcc(BPlistWork *wk);
void func_ov198_0220ec2c(BPlistWork *wk);
void func_ov198_0220eccc(BPlistWork *wk);
void func_ov198_0220ed54(BPlistWork *wk);
void func_ov198_0220ee44(BPlistWork *wk);
void func_ov198_0220eecc(BPlistWork *wk);
void func_ov198_0220efd8(BPlistWork *wk);
static void func_ov198_0220f138(ClActor *actor, u16 sequence);
static u16 func_ov198_0220f158(BPlistPokemon *pkm);
void func_ov198_0220f1f4(BPlistWork *wk);

// Declared in the order that MWCC's sort by size lays out in address order (tools/data_order.py's solution). The
// positions at d34, d38, ... are one 4-byte object each
static const BPlistActorSetup data_ov198_02211dfc[41] = {
#include "asset/b_plist_obj_data_ov198_02211dfc.inc"
};
static const ClActorPos data_ov198_02211db4[6] = {
#include "asset/b_plist_obj_data_ov198_02211db4.inc"
};
static const ClActorPos data_ov198_02211dcc[6] = {
#include "asset/b_plist_obj_data_ov198_02211dcc.inc"
};
static const ClActorPos data_ov198_02211de4[6] = {
#include "asset/b_plist_obj_data_ov198_02211de4.inc"
};
static const ClActorPos data_ov198_02211da0[5] = {
    { 0x1a, 0x50 }, { 0x9a, 0x50 }, { 0x1a, 0x80 }, { 0x9a, 0x80 }, { 0x5a, 0xb0 }
};
static const ClActorPos data_ov198_02211d78[2] = { { 0x82, 0x10 }, { 0xa4, 0x10 } };
static const ClActorPos data_ov198_02211d80[2] = { { 0x82, 0x10 }, { 0xa4, 0x10 } };
static const ClActorPos data_ov198_02211d88[2] = { { 0x82, 0x10 }, { 0xa4, 0x10 } };
static const ClActorPos data_ov198_02211d90[2] = { { 0x82, 0x10 }, { 0xa4, 0x10 } };
static const ClActorPos data_ov198_02211d98[2] = { { 0x82, 0x10 }, { 0xa4, 0x10 } };
static const ClActorPos data_ov198_02211d70 = { 0x18, 0x0c };
static const ClActorPos data_ov198_02211d6c = { 0xc6, 0x14 };
static const ClActorPos data_ov198_02211d68 = { 0xc6, 0x14 };
static const ClActorPos data_ov198_02211d64 = { 0x18, 0x0c };
static const ClActorPos data_ov198_02211d60 = { 0xc6, 0x14 };
static const ClActorPos data_ov198_02211d50 = { 0x18, 0x0c };
static const ClActorPos data_ov198_02211d3c = { 0xc6, 0x14 };
static const ClActorPos data_ov198_02211d40 = { 0x88, 0x28 };
static const ClActorPos data_ov198_02211d74 = { 0xdf, 0x34 };
static const ClActorPos data_ov198_02211d4c = { 0xc6, 0x14 };
static const ClActorPos data_ov198_02211d48 = { 0x18, 0x50 };
static const ClActorPos data_ov198_02211d44 = { 0x18, 0x0c };
static const ClActorPos data_ov198_02211d54 = { 0x18, 0x0c };
static const ClActorPos data_ov198_02211d34 = { 0x80, 0x48 };
static const ClActorPos data_ov198_02211d38 = { 0x14, 0x84 };
static const ClActorPos data_ov198_02211d58 = { 0x88, 0x28 };
static const ClActorPos data_ov198_02211d5c = { 0x18, 0x50 };

// pokebw2's BPlistObj_Init
void func_ov198_0220e2b4(BPlistWork *wk) {
    func_ov198_0220e2f8(wk);
    func_ov198_0220e340(wk);
    func_ov198_0220e3f0(wk);
    func_ov198_0220e518(wk);
    func_ov198_0220e460(wk);
    func_ov198_0220e584(wk);
    func_ov198_0220e5fc(wk);
    func_ov198_0220e6e0(wk);
    func_ov198_0220f1f4(wk);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

void func_ov198_0220e2f8(BPlistWork *wk) {
    u32 i;

    for (i = 0; i < NELEMS(wk->unk_24a4); i++) {
        wk->unk_24a4[i] = -1;
    }
    for (i = 0; i < NELEMS(wk->unk_250c); i++) {
        wk->unk_250c[i] = -1;
    }
    for (i = 0; i < NELEMS(wk->unk_2524); i++) {
        wk->unk_2524[i] = -1;
    }
}

static void func_ov198_0220e340(BPlistWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, HEAPID_TAIL(wk->param->heapId));
    u32 *res = wk->unk_24a4;
    u16 i;

    for (i = 0; i < 6; i++) {
        if (wk->pokemon[i].species != 0) {
            res[i] = func_0204a6c8(arc, func_0201ba68(func_0201d624(wk->pokemon[i].pkm)), FALSE, CLACT_VRAM_SUB, wk->param->heapId);
        } else {
            res[i] = func_0204a6c8(arc, PokeParty_GetIconIndex(0, 0, 0, FALSE), FALSE, CLACT_VRAM_SUB, wk->param->heapId);
        }
    }
    wk->unk_250c[0] = func_0204acec(arc, func_0201bc3c(), CLACT_VRAM_SUB, 0, wk->param->heapId);
    wk->unk_2524[0] = func_0204af28(arc, func_0201bc78(), getOBJTileMapping_SubEng(), wk->param->heapId);
    GFL_ArcToolFree(arc);
}

void func_ov198_0220e3f0(BPlistWork *wk) {
    ArcTool *arc;

    arc = GFL_ArcSysCreateFileHandle(ARCID_APP_MENU_COMMON, HEAPID_TAIL(wk->param->heapId));
    wk->unk_24a4[6] = func_0204a6c8(arc, 12, FALSE, CLACT_VRAM_SUB, wk->param->heapId);
    wk->unk_250c[1] = func_0204abf0(arc, 11, CLACT_VRAM_SUB, 0x60, wk->param->heapId);
    wk->unk_2524[1] = func_0204af28(arc, 13, 16, wk->param->heapId);
    GFL_ArcToolFree(arc);
}

void func_ov198_0220e460(BPlistWork *wk) {
    u32 i;
    ArcTool *arc;

    arc = GFL_ArcSysCreateFileHandle(ARCID_APP_MENU_COMMON, HEAPID_TAIL(wk->param->heapId));
    wk->unk_250c[3] = func_0204abf0(arc, 33, CLACT_VRAM_SUB, 0xa0, wk->param->heapId);
    wk->unk_2524[3] = func_0204af28(arc, 59, 62, wk->param->heapId);
    for (i = 8; i <= 21; i++) {
        wk->unk_24a4[i] = func_0204a6c8(arc, func_020275f8(0), FALSE, CLACT_VRAM_SUB, wk->param->heapId);
    }
    wk->unk_24a4[22] = func_0204a6c8(arc, func_02027610(0), FALSE, CLACT_VRAM_SUB, wk->param->heapId);
    wk->unk_24a4[23] = func_0204a6c8(arc, func_02027610(0), FALSE, CLACT_VRAM_SUB, wk->param->heapId);
    GFL_ArcToolFree(arc);
}

void func_ov198_0220e518(BPlistWork *wk) {
    ArcTool *arc;

    arc = GFL_ArcSysCreateFileHandle(ARCID_APP_MENU_COMMON, HEAPID_TAIL(wk->param->heapId));
    wk->unk_24a4[7] = func_0204a6c8(arc, 65, FALSE, CLACT_VRAM_SUB, wk->param->heapId);
    wk->unk_250c[2] = func_0204abf0(arc, 66, CLACT_VRAM_SUB, 0x80, wk->param->heapId);
    wk->unk_2524[2] = func_0204af28(arc, 67, 70, wk->param->heapId);
    GFL_ArcToolFree(arc);
}

void func_ov198_0220e584(BPlistWork *wk) {
    ArcTool *arc;

    arc = GFL_ArcSysCreateFileHandle(11, HEAPID_TAIL(wk->param->heapId));
    wk->unk_24a4[24] = func_0204a6c8(arc, 0x77, FALSE, CLACT_VRAM_SUB, wk->param->heapId);
    wk->unk_250c[4] = func_0204ac18(arc, 0x74, CLACT_VRAM_SUB, 0x100, 4, 1, wk->param->heapId);
    wk->unk_2524[4] = func_0204af28(arc, 0x78, 0x79, wk->param->heapId);
    GFL_ArcToolFree(arc);
}

void func_ov198_0220e5fc(BPlistWork *wk) {
    ArcTool *arc;

    arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), HEAPID_TAIL(wk->param->heapId));
    wk->unk_24a4[25] = func_0204a6c8(arc, func_0202775c(0), FALSE, CLACT_VRAM_SUB, wk->param->heapId);
    wk->unk_250c[5] = func_0204abf0(arc, func_02027758(), CLACT_VRAM_SUB, 0x120, wk->param->heapId);
    wk->unk_2524[5] = func_0204af28(arc, func_02027760(0), func_02027764(0), wk->param->heapId);
    GFL_ArcToolFree(arc);
}

ClActor *func_ov198_0220e684(BPlistWork *wk, const BPlistActorSetup *data) {
    ClActorSetup setup;

    setup.x = 0;
    setup.y = 0;
    setup.sequence = 0;
    setup.priority = data->priority;
    setup.bgPriority = data->bgPriority;
    return func_0204b294(wk->unk_1eb8, wk->unk_24a4[data->chars], wk->unk_250c[data->palette],
                         wk->unk_2524[data->cellAnims], &setup, CLACT_SURFACE_SUB, wk->param->heapId);
}

static void func_ov198_0220e6e0(BPlistWork *wk) {
    u32 i;

    wk->unk_1eb8 = func_0204b100(45, 0, HEAPID_TAIL(wk->param->heapId));
    for (i = 0; i < 41; i++) {
        wk->actors[i] = func_ov198_0220e684(wk, &data_ov198_02211dfc[i]);
    }
    func_0203d34c(0, wk->param->heapId);
    func_020216d4(wk->paletteFade, PALFADE_VRAM_SUB_OBJ, 0, 0x1e0);
    func_ov198_0220e7e4(wk);
}

void func_ov198_0220e74c(BPlistWork *wk) {
    u32 i;

    for (i = 0; i < 41; i++) {
        func_0204b3b4(wk->actors[i]);
    }
    func_ov196_0220afc4(wk->cursor);
    for (i = 0; i < 26; i++) {
        func_0204a8d4(wk->unk_24a4[i]);
    }
    for (i = 0; i < 6; i++) {
        func_0204ada4(wk->unk_250c[i]);
    }
    for (i = 0; i < 6; i++) {
        func_0204afd8(wk->unk_2524[i]);
    }
    func_0204b1cc(wk->unk_1eb8);
}











// pokebw2's BPlistObj_ShowAt
static void func_ov198_0220e7cc(ClActor *actor, const ClActorPos *pos) {
    func_0204b3dc(actor, TRUE);
    func_0204b404(actor, pos, CLACT_SURFACE_SUB);
}

void func_ov198_0220e7e4(BPlistWork *wk) {
    s16 i;

    for (i = 0; i < 6; i++) {
        if (wk->pokemon[i].species != 0) {
            func_0204b6f4(wk->actors[BPLIST_ACTOR_POKE + i],
                          func_0201bb5c(wk->pokemon[i].species, wk->pokemon[i].form, wk->pokemon[i].sex,
                                        wk->pokemon[i].isEgg),
                          1);
        }
    }
}

void func_ov198_0220e82c(BPlistWork *wk, ClActor *actor, u32 chars, u32 type) {
    NNSG2dCharacterData *charData;
    ArcTool *arc;
    void *file;

    arc = GFL_ArcSysCreateFileHandle(ARCID_APP_MENU_COMMON, HEAPID_TAIL(wk->param->heapId));
    file = GFL_G2DIOReadOBJNCGRArc(arc, func_020275f8(type), FALSE, &charData, HEAPID_TAIL(wk->param->heapId));
    func_0204a9d0(wk->unk_24a4[chars], charData);
    GFL_HeapFree(file);
    GFL_ArcToolFree(arc);
    func_0204b6f4(actor, func_020275ec(type), 1);
}

void func_ov198_0220e8b4(BPlistWork *wk, u32 actor, u32 chars, u32 category) {
    NNSG2dCharacterData *charData;
    ArcTool *arc;
    void *file;

    actor += wk->categoryIconBuf;
    chars += wk->categoryIconBuf;
    arc = GFL_ArcSysCreateFileHandle(ARCID_APP_MENU_COMMON, HEAPID_TAIL(wk->param->heapId));
    file = GFL_G2DIOReadOBJNCGRArc(arc, func_02027610(category), FALSE, &charData, HEAPID_TAIL(wk->param->heapId));
    func_0204a9d0(wk->unk_24a4[chars], charData);
    GFL_HeapFree(file);
    GFL_ArcToolFree(arc);
    func_0204b6f4(wk->actors[actor], func_02027604(category), 1);
    wk->categoryIconBuf ^= 1;
}




// pokebw2's BPlistObj_ShowStatusIcon
static void func_ov198_0220e974(u8 status, ClActor *actor, const ClActorPos *pos) {
    if (status == APP_STATUS_ICON_NONE) {
        func_0204b404(actor, pos, CLACT_SURFACE_SUB);
        return;
    }
    func_0204b878(actor, status);
    func_ov198_0220e7cc(actor, pos);
}

void func_ov198_0220e9a0(BPlistWork *wk, BPlistPokemon *pkm, const ClActorPos *pos) {
    u32 actor;
    u32 chars;

    if (wk->pokeTypeIconBuf == 0) {
        actor = BPLIST_ACTOR_POKE_TYPE;
        chars = 8;
    } else {
        actor = BPLIST_ACTOR_POKE_TYPE + 2;
        chars = 10;
    }
    func_ov198_0220e82c(wk, wk->actors[actor], chars, pkm->type1);
    func_ov198_0220e7cc(wk->actors[actor], pos);
    if (pkm->type1 != pkm->type2) {
        func_ov198_0220e82c(wk, wk->actors[actor + 1], chars + 1, pkm->type2);
        func_ov198_0220e7cc(wk->actors[actor + 1], pos + 1);
    }
    wk->pokeTypeIconBuf ^= 1;
}


// pokebw2's BPlistObj_ShowItemIcon
static void func_ov198_0220ea30(u16 item, ClActor *actor, const ClActorPos *pos) {
    if (item == 0) {
        func_0204b404(actor, pos, CLACT_SURFACE_SUB);
        return;
    }
    if (PML_ItemIsMail(item) == TRUE) {
        func_0204b878(actor, 1);
    } else {
        func_0204b878(actor, 0);
    }
    func_ov198_0220e7cc(actor, pos);
}

void func_ov198_0220ea68(BPlistWork *wk, u8 page) {
    u32 i;

    for (i = 0; i < NELEMS(wk->actors); i++) {
        func_0204b3dc(wk->actors[i], FALSE);
    }
    switch (page) {
    case 0:
    case 8:
        func_ov198_0220eaf0(wk);
        break;
    case 1:
        func_ov198_0220ebcc(wk);
        break;
    case 2:
        func_ov198_0220ec2c(wk);
        break;
    case 3:
        func_ov198_0220eccc(wk);
        break;
    case 4:
        func_ov198_0220ed54(wk);
        break;
    case 5:
        func_ov198_0220efd8(wk);
        break;
    case 6:
        func_ov198_0220ee44(wk);
        break;
    case 7:
        func_ov198_0220eecc(wk);
        break;
    }
}

static void func_ov198_0220eaf0(BPlistWork *wk) {
    s16 i;

    for (i = 0; i < 6; i++) {
        u8 slot = func_ov198_022110d4(wk, i);

        if (wk->pokemon[slot].species != 0) {
            ClActorPos pos;

            func_ov198_0220e7cc(wk->actors[BPLIST_ACTOR_POKE + slot], &data_ov198_02211db4[i]);
            func_ov198_0220e974(wk->pokemon[slot].status, wk->actors[BPLIST_ACTOR_STATUS + slot],
                                &data_ov198_02211dcc[i]);
            if (!wk->pokemon[slot].isEgg) {
                func_ov198_0220e7cc(wk->actors[BPLIST_ACTOR_UNK1F08 + slot], &data_ov198_02211de4[i]);
            }
            pos = data_ov198_02211db4[i];
            pos.x += 8;
            pos.y += 8;
            func_ov198_0220ea30(wk->pokemon[slot].item, wk->actors[BPLIST_ACTOR_ITEM + slot], &pos);
        }
    }
}

static void func_ov198_0220ebcc(BPlistWork *wk) {
    u8 slot;
    ClActorPos pos;

    slot = func_ov198_022110d4(wk, wk->param->partyIndex);
    pos = data_ov198_02211d34;
    func_ov198_0220e7cc((wk->actors + BPLIST_ACTOR_POKE)[slot], &pos);
    pos.x += 8;
    pos.y += 8;
    func_ov198_0220ea30(wk->pokemon[slot].item, (wk->actors + BPLIST_ACTOR_ITEM)[slot], &pos);
}

void func_ov198_0220ec2c(BPlistWork *wk) {
    u8 slot;
    BPlistPokemon *pkm;
    ClActorPos pos;

    slot = func_ov198_022110d4(wk, wk->param->partyIndex);
    pkm = &wk->pokemon[slot];
    pos = data_ov198_02211d44;
    func_ov198_0220e7cc((wk->actors + BPLIST_ACTOR_POKE)[slot], &pos);
    func_ov198_0220e974(pkm->status, (wk->actors + BPLIST_ACTOR_STATUS)[slot], &data_ov198_02211d68);
    func_ov198_0220e9a0(wk, pkm, data_ov198_02211d98);
    pos.x += 8;
    pos.y += 8;
    func_ov198_0220ea30(pkm->item, (wk->actors + BPLIST_ACTOR_ITEM)[slot], &pos);
    func_ov198_0220ea30(pkm->item, wk->actors[6], &data_ov198_02211d38);
    func_ov198_0220e7cc(wk->actors[BPLIST_ACTOR_UNK1F08], &data_ov198_02211d74);
}

void func_ov198_0220eccc(BPlistWork *wk) {
    u8 slot;
    BPlistPokemon *pkm;
    ClActorPos pos;

    slot = func_ov198_022110d4(wk, wk->param->partyIndex);
    pkm = &wk->pokemon[slot];
    pos = data_ov198_02211d70;
    func_ov198_0220e7cc((wk->actors + BPLIST_ACTOR_POKE)[slot], &pos);
    func_ov198_0220e974(pkm->status, (wk->actors + BPLIST_ACTOR_STATUS)[slot], &data_ov198_02211d6c);
    func_ov198_0220e9a0(wk, pkm, data_ov198_02211d78);
    pos.x += 8;
    pos.y += 8;
    func_ov198_0220ea30(pkm->item, (wk->actors + BPLIST_ACTOR_ITEM)[slot], &pos);
    func_ov198_0220f060(wk);
}

void func_ov198_0220ed54(BPlistWork *wk) {
    u8 slot;
    BPlistPokemon *pkm;
    ClActorPos pos;

    slot = func_ov198_022110d4(wk, wk->param->partyIndex);
    pkm = &wk->pokemon[slot];
    pos = data_ov198_02211d64;
    func_ov198_0220e7cc((wk->actors + BPLIST_ACTOR_POKE)[slot], &pos);
    func_ov198_0220e974(pkm->status, (wk->actors + BPLIST_ACTOR_STATUS)[slot], &data_ov198_02211d60);
    func_ov198_0220e9a0(wk, pkm, data_ov198_02211d88);
    pos.x += 8;
    pos.y += 8;
    func_ov198_0220ea30(pkm->item, (wk->actors + BPLIST_ACTOR_ITEM)[slot], &pos);
    if (wk->typeIconBuf == 0) {
        func_ov198_0220e7cc(wk->actors[BPLIST_ACTOR_MOVE_TYPE + 5 + wk->param->slot], &data_ov198_02211d58);
    } else {
        func_ov198_0220e7cc(wk->actors[BPLIST_ACTOR_MOVE_TYPE + wk->param->slot], &data_ov198_02211d58);
    }
    if (wk->categoryIconBuf == 0) {
        func_ov198_0220e7cc(wk->actors[BPLIST_ACTOR_CATEGORY], &data_ov198_02211d48);
    } else {
        func_ov198_0220e7cc(wk->actors[BPLIST_ACTOR_CATEGORY + 1], &data_ov198_02211d48);
    }
    func_ov198_0220e8b4(wk, BPLIST_ACTOR_CATEGORY, 22, pkm->moves[wk->param->slot].category);
}

void func_ov198_0220ee44(BPlistWork *wk) {
    u8 slot;
    BPlistPokemon *pkm;
    ClActorPos pos;

    slot = func_ov198_022110d4(wk, wk->param->partyIndex);
    pkm = &wk->pokemon[slot];
    pos = data_ov198_02211d50;
    func_ov198_0220e7cc((wk->actors + BPLIST_ACTOR_POKE)[slot], &pos);
    func_ov198_0220e974(pkm->status, (wk->actors + BPLIST_ACTOR_STATUS)[slot], &data_ov198_02211d4c);
    func_ov198_0220e9a0(wk, pkm, data_ov198_02211d80);
    pos.x += 8;
    pos.y += 8;
    func_ov198_0220ea30(pkm->item, (wk->actors + BPLIST_ACTOR_ITEM)[slot], &pos);
    func_ov198_0220f060(wk);
}

void func_ov198_0220eecc(BPlistWork *wk) {
    u8 slot;
    BPlistPokemon *pkm;
    ClActorPos pos;

    slot = func_ov198_022110d4(wk, wk->param->partyIndex);
    pkm = &wk->pokemon[slot];
    pos = data_ov198_02211d54;
    func_ov198_0220e7cc((wk->actors + BPLIST_ACTOR_POKE)[slot], &pos);
    func_ov198_0220e974(pkm->status, (wk->actors + BPLIST_ACTOR_STATUS)[slot], &data_ov198_02211d3c);
    func_ov198_0220e9a0(wk, pkm, data_ov198_02211d90);
    pos.x += 8;
    pos.y += 8;
    func_ov198_0220ea30(pkm->item, (wk->actors + BPLIST_ACTOR_ITEM)[slot], &pos);
    if (wk->typeIconBuf == 0) {
        func_ov198_0220e7cc(wk->actors[BPLIST_ACTOR_MOVE_TYPE + 5 + wk->param->slot], &data_ov198_02211d40);
    } else {
        func_ov198_0220e7cc(wk->actors[BPLIST_ACTOR_MOVE_TYPE + wk->param->slot], &data_ov198_02211d40);
    }
    if (wk->categoryIconBuf == 0) {
        func_ov198_0220e7cc(wk->actors[BPLIST_ACTOR_CATEGORY], &data_ov198_02211d5c);
    } else {
        func_ov198_0220e7cc(wk->actors[BPLIST_ACTOR_CATEGORY + 1], &data_ov198_02211d5c);
    }
    if (wk->param->slot < 4) {
        func_ov198_0220e8b4(wk, BPLIST_ACTOR_CATEGORY, 22, pkm->moves[wk->param->slot].category);
    } else {
        func_ov198_0220e8b4(wk, BPLIST_ACTOR_CATEGORY, 22,
                            PML_MoveGetParam(wk->param->move, MOVE_PARAM_CATEGORY));
    }
}

void func_ov198_0220efd8(BPlistWork *wk) {
    u8 slot;
    BPlistPokemon *pkm;
    ClActorPos pos;

    slot = func_ov198_022110d4(wk, wk->param->partyIndex);
    pkm = &wk->pokemon[slot];
    pos = data_ov198_02211d50;
    func_ov198_0220e7cc((wk->actors + BPLIST_ACTOR_POKE)[slot], &pos);
    func_ov198_0220e974(pkm->status, (wk->actors + BPLIST_ACTOR_STATUS)[slot], &data_ov198_02211d4c);
    func_ov198_0220e9a0(wk, pkm, data_ov198_02211d80);
    pos.x += 8;
    pos.y += 8;
    func_ov198_0220ea30(pkm->item, (wk->actors + BPLIST_ACTOR_ITEM)[slot], &pos);
    func_ov198_0220f060(wk);
}

void func_ov198_0220f060(BPlistWork *wk) {
    BPlistPokemon *pkm;
    u8 actor;
    u8 chars;
    u32 i;

    pkm = &wk->pokemon[func_ov198_022110d4(wk, wk->param->partyIndex)];
    if (wk->typeIconBuf == 0) {
        actor = BPLIST_ACTOR_MOVE_TYPE;
        chars = 12;
    } else {
        actor = BPLIST_ACTOR_MOVE_TYPE + 5;
        chars = 17;
    }
    for (i = 0; i < 4; i++) {
        if (pkm->moves[i].move != 0) {
            func_ov198_0220e82c(wk, wk->actors[actor + i], chars + i, pkm->moves[i].type);
            func_ov198_0220e7cc(wk->actors[actor + i], &data_ov198_02211da0[i]);
        }
    }
    if (wk->param->move != 0 && wk->param->unk1F == 4) {
        func_ov198_0220e82c(wk, wk->actors[actor + i], chars + i,
                            PML_MoveGetParam(wk->param->move, MOVE_PARAM_TYPE));
        func_ov198_0220e7cc(wk->actors[actor + i], &data_ov198_02211da0[i]);
    }
    wk->typeIconBuf ^= 1;
}











// pokebw2's BPlistObj_ChangeAnim
static void func_ov198_0220f138(ClActor *actor, u16 sequence) {
    if (sequence != func_0204b898(actor)) {
        func_0204b8c4(actor, 0);
        func_0204b878(actor, sequence);
    }
}

// pokebw2's BPlistObj_GetPokeIconAnim
static u16 func_ov198_0220f158(BPlistPokemon *pkm) {
    if (pkm->hp == 0) {
        return 0;
    }
    if (pkm->status != APP_STATUS_ICON_NONE && pkm->status != APP_STATUS_ICON_FAINTED) {
        return 5;
    }
    if (pkm->hp == pkm->maxHp) {
        return 1;
    }
    switch (func_0202d034(pkm->hp, pkm->maxHp)) {
    case HP_GAUGE_COLOR_GREEN:
        return 2;
    case HP_GAUGE_COLOR_YELLOW:
        return 3;
    case HP_GAUGE_COLOR_RED:
        return 4;
    }
    return 0;
}

void func_ov198_0220f1a4(BPlistWork *wk) {
    u16 i;

    for (i = 0; i < 6; i++) {
        u8 slot = func_ov198_022110d4(wk, i);

        if (wk->pokemon[slot].species != 0) {
            u16 seq = func_ov198_0220f158(&wk->pokemon[slot]);

            func_ov198_0220f138(wk->actors[BPLIST_ACTOR_POKE + slot], seq);
            func_0204b8d4(wk->actors[BPLIST_ACTOR_POKE + slot], FX32_ONE);
        }
    }
}

void func_ov198_0220f1f4(BPlistWork *wk) {
    func_ov196_0220af6c(wk->cursor, wk->unk_1eb8, wk->unk_24a4[24], wk->unk_250c[4], wk->unk_2524[4]);
    func_ov196_0220afdc(wk->cursor, FALSE);
}

void func_ov198_0220f22c(BPlistWork *wk, int pos, s16 dx) {
    ClActorPos p;
    u8 slot;

    slot = func_ov198_022110d4(wk, pos);
    func_0204b454((wk->actors + BPLIST_ACTOR_ITEM)[slot], &p, CLACT_SURFACE_SUB);
    p.x += dx;
    func_0204b404((wk->actors + BPLIST_ACTOR_ITEM)[slot], &p, CLACT_SURFACE_SUB);
    func_0204b454((wk->actors + BPLIST_ACTOR_POKE)[slot], &p, CLACT_SURFACE_SUB);
    p.x += dx;
    func_0204b404((wk->actors + BPLIST_ACTOR_POKE)[slot], &p, CLACT_SURFACE_SUB);
    func_0204b454((wk->actors + BPLIST_ACTOR_STATUS)[slot], &p, CLACT_SURFACE_SUB);
    p.x += dx;
    func_0204b404((wk->actors + BPLIST_ACTOR_STATUS)[slot], &p, CLACT_SURFACE_SUB);
    func_0204b454((wk->actors + BPLIST_ACTOR_UNK1F08)[slot], &p, CLACT_SURFACE_SUB);
    p.x += dx;
    func_0204b404((wk->actors + BPLIST_ACTOR_UNK1F08)[slot], &p, CLACT_SURFACE_SUB);
}

void func_ov198_0220f2dc(BPlistWork *wk, int posA, int posB) {
    ClActorPos a;
    ClActorPos b;
    u8 slotA;
    u8 slotB;

    slotA = func_ov198_022110d4(wk, posA);
    slotB = func_ov198_022110d4(wk, posB);
    func_0204b454((wk->actors + BPLIST_ACTOR_ITEM)[slotA], &a, CLACT_SURFACE_SUB);
    func_0204b454((wk->actors + BPLIST_ACTOR_ITEM)[slotB], &b, CLACT_SURFACE_SUB);
    func_0204b404((wk->actors + BPLIST_ACTOR_ITEM)[slotA], &b, CLACT_SURFACE_SUB);
    func_0204b404((wk->actors + BPLIST_ACTOR_ITEM)[slotB], &a, CLACT_SURFACE_SUB);
    func_0204b454((wk->actors + BPLIST_ACTOR_POKE)[slotA], &a, CLACT_SURFACE_SUB);
    func_0204b454((wk->actors + BPLIST_ACTOR_POKE)[slotB], &b, CLACT_SURFACE_SUB);
    func_0204b404((wk->actors + BPLIST_ACTOR_POKE)[slotA], &b, CLACT_SURFACE_SUB);
    func_0204b404((wk->actors + BPLIST_ACTOR_POKE)[slotB], &a, CLACT_SURFACE_SUB);
    func_0204b454((wk->actors + BPLIST_ACTOR_STATUS)[slotA], &a, CLACT_SURFACE_SUB);
    func_0204b454((wk->actors + BPLIST_ACTOR_STATUS)[slotB], &b, CLACT_SURFACE_SUB);
    func_0204b404((wk->actors + BPLIST_ACTOR_STATUS)[slotA], &b, CLACT_SURFACE_SUB);
    func_0204b404((wk->actors + BPLIST_ACTOR_STATUS)[slotB], &a, CLACT_SURFACE_SUB);
    func_0204b454((wk->actors + BPLIST_ACTOR_UNK1F08)[slotA], &a, CLACT_SURFACE_SUB);
    func_0204b454((wk->actors + BPLIST_ACTOR_UNK1F08)[slotB], &b, CLACT_SURFACE_SUB);
    func_0204b404((wk->actors + BPLIST_ACTOR_UNK1F08)[slotA], &b, CLACT_SURFACE_SUB);
    func_0204b404((wk->actors + BPLIST_ACTOR_UNK1F08)[slotB], &a, CLACT_SURFACE_SUB);
}





