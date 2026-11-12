// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov255/box2_obj.c, ported by tools/port_file.py.
#include "types.h"
#include "app/box2_obj.h"
#include "app/box2_main.h"
#include "app/box2_bmp.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "pml/item.h"
#include "pml/personal.h"
#include "pml/poke_graphic.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "system/app_menu_common.h"
#include "system/bgwinfrm.h"
#include "system/bmp_oam.h"
#include "system/cursor_move.h"
#include "gfl/bmp.h"
#include "nitro/mi.h"

// The PC box's actors: the cursor, the Pokémon and item icons, the tray icons of the box list and the buttons. The ROM
// doesn't name this file; box2_obj.c is a guess after box2_main.c. Names are ours

// No resource loaded
#define BOX2_RES_NONE 0xffffffff

// An actor to create: where, its resources and its surface
typedef struct {
    ClActorSetup setup;
    u32 chr;
    u32 pal;
    u32 cell;
    // A palette set once the actor is created, by the code that uses it
    u16 palette;
    u16 surface;
} Box2ActorData;

static void func_ov170_021ebdf4(Box2AppWork *app);
ClActor *func_ov170_021ebee0(Box2AppWork *app, const Box2ActorData *data);
void func_ov170_021ebe24(Box2AppWork *app);
void func_ov170_021ebeac(Box2SysWork *syswk);
void func_ov170_021ec060(Box2AppWork *app, u8 id, BOOL blend);
void func_ov170_021eca6c(Box2SysWork *syswk, BOOL blend, u32 start, u32 end, BOOL check);
void func_ov170_021ec0dc(Box2AppWork *app);
void func_ov170_021ec1e8(Box2AppWork *app);
void func_ov170_021ec254(Box2AppWork *app);
void func_ov170_021ed058(Box2AppWork *app);
static void func_ov170_021ecca4(Box2AppWork *app);
void func_ov170_021ecdf4(Box2AppWork *app);
static void func_ov170_021ed824(Box2AppWork *app);
void func_ov170_021ec2a0(Box2AppWork *app);
void func_ov170_021ecf78(Box2AppWork *app);
void func_ov170_021ed370(Box2SysWork *syswk);
void func_ov170_021ebe4c(Box2AppWork *app);
void func_ov170_021ed9c8(Box2SysWork *syswk);
void func_ov170_021edb64(Box2SysWork *syswk, u32 tray, u32 id);
void func_ov170_021edb94(Box2SysWork *syswk, u32 tray, u32 row);
static void func_ov170_021edbe8(Box2SysWork *syswk, u8 *chr, u32 wallpaper, u32 color, u32 size);
static void func_ov170_021edc0c(Box2SysWork *syswk, u32 tray, u8 *chr);
void func_ov170_021edfc4(Box2SysWork *syswk, u32 index);
void func_ov170_021ee120(Box2AppWork *app);
static void func_ov170_021ee200(Box2AppWork *app);


// The outline offsets around the item icon, y then x
static const s8 data_ov170_021efcc8[8] = { 0, 0, 1, -1, 1, -1, 1, -1 };

// The first of the tray's Pokémon icons
// pokebw2's sPokeIconData
static const Box2ActorData data_ov170_021efd04 = {
    { 24, 48, 0, 10, 3 }, 0, 0, 0, 0, 0,
};

// The tray icons of the box list
// pokebw2's sTrayIconData
static const Box2ActorData data_ov170_021efd1c = {
    { 300, 0, 0, 0x50, 1 }, 0x39, 5, 5, 0, 0,
};

// The type icons on the upper screen
// pokebw2's sTypeIconData
static const Box2ActorData data_ov170_021efd34 = {
    { 160, 48, 0, 0, 1 }, 0x28, 4, 4, 0, 1,
};

// The sprite offsets of the party icons, x and y as bytes
static const u8 data_ov170_021efce0[6][2] = {
#include "asset/box2_obj_data_ov170_021efce0.inc"
};

static const s8 data_ov170_021efcd0[8] = { 1, -1, 0, 0, 1, 1, -1, -1 };
static const s16 data_ov170_021efcd8[4] = { 52, 86, 120, 154 };

// The six party slots' icon positions: x and y, four bytes a slot
static const ClActorPos data_ov170_021efd4c[6] = {
#include "asset/box2_obj_data_ov170_021efd4c.inc"
};

// The color of the tray icons' background, by wallpaper
// pokebw2's sTrayIconColors
static const u8 data_ov170_021efcec[24] = {
#include "asset/box2_obj_data_ov170_021efcec.inc"
};

// pokebw2's data_ov170_021efd64
// pokebw2's sClActSysSetup
static const ClActSysSetup data_ov170_021efd64 = {
    0, 0, 0, 512, 4, 124, 4, 124, 0, 153, 11, 96, 0, 16, 16,
};

// The actors to create, one a slot: 0x18 bytes each
const Box2ActorData data_ov170_021efd80[28] = {
#include "asset/box2_obj_data_ov170_021efd80.inc"
};


void func_ov170_021ebd94(Box2SysWork *syswk) {
    func_ov170_021ebdf4(syswk->app);
    func_ov170_021ebe24(syswk->app);
    func_ov170_021ebeac(syswk);
    func_ov170_021ee038(syswk, syswk->unk1F, syswk->unk20, syswk->unk21);
    func_ov170_021ee120(syswk->app);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

void func_ov170_021ebdd8(Box2AppWork *app) {
    func_ov170_021ee200(app);
    func_ov170_021ebf50(app);
    func_ov170_021ebe4c(app);
    func_0204a5ac();
}

static void func_ov170_021ebdf4(Box2AppWork *app) {
    ClActSysSetup setup = data_ov170_021efd64;
    ClActSys_Create(&setup, func_ov170_021dc9d8(), HEAPID_BOX2_APP);
}

void func_ov170_021ebe24(Box2AppWork *app) {
    func_ov170_021ec0dc(app);
    func_ov170_021ec1e8(app);
    func_ov170_021ec254(app);
    func_ov170_021ed058(app);
    func_ov170_021ecca4(app);
    func_ov170_021ecdf4(app);
}

void func_ov170_021ebe4c(Box2AppWork *app) {
    u32 i;

    for (i = 0; i < 0x45; i++) {
        if (app->unk9FF4[i] != 0xffffffff) {
            func_0204a8d4(app->unk9FF4[i]);
        }
    }
    for (i = 0; i < 0xb; i++) {
        if (app->unkA108[i] != 0xffffffff) {
            func_0204ada4(app->unkA108[i]);
        }
    }
    for (i = 0; i < 0xc; i++) {
        if (app->unkA134[i] != 0xffffffff) {
            func_0204afd8(app->unkA134[i]);
        }
    }
}


void func_ov170_021ebeac(Box2SysWork *syswk) {
    syswk->app->unk9E70 = func_0204b100(180, 0, 76);
    func_ov170_021ed824(syswk->app);
    func_ov170_021ec2a0(syswk->app);
    func_ov170_021ed370(syswk);
    func_ov170_021ecf78(syswk->app);
}

ClActor *func_ov170_021ebee0(Box2AppWork *app, const Box2ActorData *data) {
    ClActor *actor;

    actor = func_0204b294(app->unk9E70, app->unk9FF4[data->chr], app->unkA108[data->pal], app->unkA134[data->cell], &data->setup, data->surface, 0x4c);
    func_0204ba20(actor, 0);
    return actor;
}


void func_ov170_021ebf34(Box2AppWork *app, u32 idx) {
    if (app->unk9E74[idx] != NULL) {
        func_0204b3b4(app->unk9E74[idx]);
        app->unk9E74[idx] = NULL;
    }
}

void func_ov170_021ebf50(Box2AppWork *app) {
    u32 i;

    for (i = 0; i < 0x60; i++) {
        func_ov170_021ebf34(app, i);
    }
    func_0204b1cc(app->unk9E70);
}

void func_ov170_021ebf74(Box2AppWork *app) {
    u32 i;

    for (i = 0; i < 0x2d; i++) {
        if (app->unk9E74[i] != NULL) {
            if (func_0204b944(app->unk9E74[i]) != TRUE) {
                func_0204b8d4(app->unk9E74[i], 0x1000);
            }
        }
    }
    func_0204a600();
}



void func_ov170_021ebfa8(Box2AppWork *app, u32 id, u32 anim) {
    func_0204b8c4(app->unk9E74[id], 0);
    func_0204b878(app->unk9E74[id], (u16)anim);
}

void func_ov170_021ebfcc(Box2AppWork *app, u32 id, u32 anim) {
    func_ov170_021ebfa8(app, id, anim);
    func_0204b92c(app->unk9E74[id], TRUE);
}




BOOL func_ov170_021ebfec(Box2AppWork *app, u32 idx) {
    return func_0204b98c(app->unk9E74[idx]);
}

void func_ov170_021ec000(Box2AppWork *app, u32 idx, BOOL param) {
    ClActor *actor = app->unk9E74[idx];
    if (actor != NULL) {
        func_0204b3dc(actor, param);
    }
}


BOOL func_ov170_021ec01c(Box2AppWork *app, u32 idx) {
    return func_0204b3f4(app->unk9E74[idx]);
}

void func_ov170_021ec030(Box2AppWork *app, u32 idx, u32 bgPriority) {
    func_0204b84c(app->unk9E74[idx], (u8)bgPriority);
}

void func_ov170_021ec048(Box2AppWork *app, u32 idx, u32 priority) {
    func_0204b800(app->unk9E74[idx], (u8)priority);
}

void func_ov170_021ec060(Box2AppWork *app, u8 id, BOOL blend) {
    if (blend == TRUE) {
        func_0204b688(app->unk9E74[id], 1);
    } else {
        func_0204b688(app->unk9E74[id], 0);
    }
}


void func_ov170_021ec08c(Box2AppWork *app, u32 idx, s16 x, s16 y, u16 surface) {
    ClActorPos pos;

    pos.x = x;
    pos.y = y;
    func_0204b404(app->unk9E74[idx], &pos, surface);
}

void func_ov170_021ec0b0(Box2AppWork *app, u32 id, s16 *x, s16 *y, u16 surface) {
    ClActorPos pos;

    func_0204b454(app->unk9E74[id], &pos, surface);
    *x = pos.x;
    *y = pos.y;
}

void func_ov170_021ec0dc(Box2AppWork *app) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0x74, 0x4c);
    u32 i;

    for (i = 0; i < 6; i++) {
        app->unk9FF4[57 + i] = func_0204a6c8(arc, 0x47, TRUE, 0, 0x4c);
    }
    app->unkA108[5] = func_0204abf0(arc, 0x48, 0, 0, 0x4c);
    app->unkA134[5] = func_0204af28(arc, 0x49, 0x4a, 0x4c);
    for (i = 0; i < 0x25; i++) {
        app->unk9FF4[i] = func_0204a6c8(arc, 0x4b, TRUE, 0, 0x4c);
    }
    app->unk9FF4[63] = func_0204a6c8(arc, 0x43, TRUE, 0, 0x4c);
    app->unkA108[6] = func_0204abf0(arc, 0x46, 0, 0xc0, 0x4c);
    app->unkA134[6] = func_0204af28(arc, 0x44, 0x45, 0x4c);
    app->unk9FF4[64] = func_0204a6c8(arc, 0x40, TRUE, 0, 0x4c);
    app->unkA134[7] = func_0204af28(arc, 0x41, 0x42, 0x4c);
    app->unk9FF4[39] = func_0204a6c8(arc, 0x4c, TRUE, 0, 0x4c);
    app->unkA134[3] = func_0204af28(arc, 0x4d, 0x4e, 0x4c);
    GFL_ArcToolFree(arc);
}

void func_ov170_021ec1e8(Box2AppWork *app) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), 0x4c);

    app->unk9FF4[65] = func_0204a6c8(arc, func_02027618(), FALSE, 0, 0x4c);
    app->unkA108[7] = func_0204abf0(arc, func_02027614(), 0, 0x100, 0x4c);
    app->unkA134[8] = func_0204af28(arc, func_0202761c(2), func_02027620(2), 0x4c);
    GFL_ArcToolFree(arc);
}

void func_ov170_021ec254(Box2AppWork *app) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(7, 0x4c);

    app->unkA108[0] = func_0204acec(arc, func_0201bc3c(), 0, 0x60, 0x4c);
    app->unkA134[0] = func_0204af28(arc, func_0201bc40(), getOBJTileMapping_MainEng(), 0x4c);
    GFL_ArcToolFree(arc);
}

void func_ov170_021ec2a0(Box2AppWork *app) {
    Box2ActorData work;
    Box2ActorData tmpl = data_ov170_021efd04;
    u32 i;

    for (i = 0; i < 0x25; i++) {
        work = tmpl;
        work.setup.x += (i % 6) * 24;
        work.setup.y += (i / 6) * 24;
        work.setup.priority = 0x75 - i * 2;
        work.chr = i;
        app->unk9E74[51 + i] = func_ov170_021ebee0(app, &work);
        app->pokeIconId[i] = i + 0x33;
    }
}






void *func_ov170_021ec338(Box2AppWork *app, BoxPkm *pkm, NNSG2dCharacterData **character) {
    u32 fileId = func_0201ba68(pkm);
    return GFL_G2DIOReadOBJNCGRArc(app->pokeIconArc, fileId, FALSE, character, 0x4c);
}

void func_ov170_021ec35c(Box2SysWork *syswk, u32 tray) {
    NNSG2dCharacterData *character;
    BoxPkm *pkm;
    void *buf;
    u32 i;

    for (i = 0; i < 0x1e; i++) {
        if (BoxSaveAccessor_GetPkmParam(syswk->param->boxes, tray, i, PKM_PARAM_SPECIES_VALID, NULL) == 0) {
            syswk->app->unk3DF8[i] = 0;
        } else if (syswk->param->mode != 3 && syswk->tray == syswk->getTray && syswk->pos == i) {
            syswk->app->unk3DF8[i] = 0;
        } else {
            pkm = func_ov170_021dd2d0(syswk, tray, i);
            buf = func_ov170_021ec338(syswk->app, pkm, &character);
            MIi_CpuCopy32(character->pRawData, syswk->app->pokeIconChar[i], 0x200);
            GFL_HeapFree(buf);
            syswk->app->unk3DB4[i] = func_0201bbe8(pkm);
            syswk->app->unk3DF8[i] = 1;
        }
    }
}


static void func_ov170_021ec428(Box2AppWork *app, void *src, u32 id, u32 pal) {
    u32 chars = func_0204b7f0(app->unk9E74[id]);

    func_0204aa94(chars, src, 0x200, 0, 0);
    func_0204b6f4(app->unk9E74[id], (u8)pal, 1);
}

void func_ov170_021ec45c(Box2SysWork *syswk, BoxPkm *pkm, u32 id) {
    NNSG2dCharacterData *character;
    void *buf = func_ov170_021ec338(syswk->app, pkm, &character);
    u32 pal = func_0201bbe8(pkm);

    func_ov170_021ec428(syswk->app, character->pRawData, id, pal);
    GFL_HeapFree(buf);
}

void func_ov170_021ec48c(Box2SysWork *syswk, u32 tray, u32 pos, u32 id) {
    Box2AppWork *app = syswk->app;

    func_ov170_021ec000(app, id, FALSE);
    if (func_ov170_021dd25c(syswk, pos, tray, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
        func_ov170_021ec45c(syswk, func_ov170_021dd2d0(syswk, tray, pos), id);
        func_ov170_021ec000(app, id, TRUE);
    }
}












void func_ov170_021ec4e0(Box2SysWork *syswk) {
    u32 i;

    for (i = 0; i < BOX2_TRAY_POKE_MAX; i++) {
        func_ov170_021ec48c(syswk, syswk->tray, i, BOX2_ACTOR_POKEICON + i);
    }
}

void func_ov170_021ec4fc(Box2SysWork *syswk) {
    u32 count = PokeParty_GetPkmCount(syswk->param->party);
    u32 i;

    for (i = 0; i < count; i++) {
        func_ov170_021ec45c(syswk, func_0201d624(PokeParty_GetPkm(syswk->param->party, i)),
            syswk->app->pokeIconId[BOX2_PARTY_POS + i]);
        func_ov170_021ec000(syswk->app, syswk->app->pokeIconId[BOX2_PARTY_POS + i], FALSE);
    }
}


void func_ov170_021ec548(u32 pos, s16 *x, s16 *y, u32 mode) {
    if (pos < BOX2_TRAY_POKE_MAX) {
        *x = (pos % 6 + 1) * 24;
        *y = pos / 6 * 24 + 48;
    } else {
        pos -= BOX2_TRAY_POKE_MAX;
        *x = data_ov170_021efd4c[pos].x;
        *y = data_ov170_021efd4c[pos].y - 144;
        if (mode == 2) {
            *x += 152;
        }
    }
}

void func_ov170_021ec5a0(Box2SysWork *syswk, BOOL flag) {
    u32 count = PokeParty_GetPkmCount(syswk->param->party);
    u32 i;

    for (i = 0; i < 6; i++) {
        u32 id = syswk->app->pokeIconId[BOX2_PARTY_POS + i];

        func_ov170_021ec08c(syswk->app, id, data_ov170_021efd4c[i].x, data_ov170_021efd4c[i].y, 0);
        func_ov170_021ec768(syswk->app, BOX2_PARTY_POS + i, TRUE);
        if (i < count && (flag == FALSE || syswk->pos != BOX2_PARTY_POS + i)) {
            func_ov170_021ec000(syswk->app, id, TRUE);
        } else {
            func_ov170_021ec000(syswk->app, id, FALSE);
        }
    }
}

void func_ov170_021ec61c(Box2SysWork *syswk) {
    u32 count = PokeParty_GetPkmCount(syswk->param->party);
    u32 i;

    for (i = 0; i < 6; i++) {
        u32 id = syswk->app->pokeIconId[BOX2_PARTY_POS + i];

        func_ov170_021ec08c(syswk->app, id, (s16)(data_ov170_021efd4c[i].x + 0x98), data_ov170_021efd4c[i].y, 0);
        func_ov170_021ec768(syswk->app, BOX2_PARTY_POS + i, TRUE);
        if (i < count) {
            func_ov170_021ec000(syswk->app, id, TRUE);
        } else {
            func_ov170_021ec000(syswk->app, id, FALSE);
        }
    }
}

void func_ov170_021ec688(Box2SysWork *syswk) {
    u32 count = PokeParty_GetPkmCount(syswk->param->party);
    u32 i;

    for (i = 0; i < 6; i++) {
        u32 id = syswk->app->pokeIconId[BOX2_PARTY_POS + i];

        func_ov170_021ec08c(syswk->app, id, data_ov170_021efd4c[i].x, (s16)(data_ov170_021efd4c[i].y - 0x90), 0);
        func_ov170_021ec768(syswk->app, BOX2_PARTY_POS + i, TRUE);
        if (i < count) {
            func_ov170_021ec000(syswk->app, id, TRUE);
        } else {
            func_ov170_021ec000(syswk->app, id, FALSE);
        }
    }
}

void func_ov170_021ec6f4(Box2SysWork *syswk) {
    u32 count = PokeParty_GetPkmCount(syswk->param->party);
    u32 i;

    for (i = 0; i < 6; i++) {
        u32 id = syswk->app->pokeIconId[BOX2_PARTY_POS + i];

        func_ov170_021ec08c(syswk->app, id, (s16)(data_ov170_021efd4c[i].x + 0x98), (s16)(data_ov170_021efd4c[i].y - 0x90), 0);
        func_ov170_021ec768(syswk->app, BOX2_PARTY_POS + i, TRUE);
        if (i < count) {
            func_ov170_021ec000(syswk->app, id, TRUE);
        } else {
            func_ov170_021ec000(syswk->app, id, FALSE);
        }
    }
}

void func_ov170_021ec768(Box2AppWork *app, u32 pos, u32 flag) {
    u8 idx = app->pokeIconId[pos];

    if (flag == FALSE) {
        func_ov170_021ec030(app, idx, 0);
        func_ov170_021ec048(app, idx, 0x14);
    } else {
        if (pos < 0x1e || pos == 0x24) {
            func_ov170_021ec030(app, idx, 3);
        } else {
            func_ov170_021ec030(app, idx, 1);
        }
        func_ov170_021ec048(app, idx, 0x75 - pos * 2);
    }
}

void func_ov170_021ec7b8(Box2AppWork *app, u32 index, u32 pos, u32 flag) {
    u8 idx = app->pokeIconId[index];

    if (flag == FALSE) {
        func_ov170_021ec030(app, idx, 0);
        func_ov170_021ec048(app, idx, 0x14);
    } else {
        if (pos < 0x1e) {
            func_ov170_021ec030(app, idx, 3);
        } else {
            func_ov170_021ec030(app, idx, 1);
        }
        func_ov170_021ec048(app, idx, 0x75 - pos * 2);
    }
}

void func_ov170_021ec804(Box2SysWork *syswk, s16 dir) {
    s16 x;
    s16 y;
    s16 reset;
    s32 limit;
    u16 i;

    if (dir >= 0) {
        limit = 0xb0;
        reset = -8;
    } else {
        limit = -8;
        reset = 0xb0;
    }
    for (i = 0; i < 0x1e; i++) {
        u8 id = syswk->app->pokeIconId[i];

        func_ov170_021ec0b0(syswk->app, id, &x, &y, 0);
        x += dir;
        if (x == limit) {
            x = reset;
            func_ov170_021ec428(syswk->app, syswk->app->pokeIconChar[i], id, syswk->app->unk3DB4[i]);
            func_ov170_021ec000(syswk->app, id, syswk->app->unk3DF8[i]);
            if (syswk->app->unkA17A[0] == 0 && (syswk->param->mode == 3 || syswk->param->mode == BOX2_MODE_DREAM_WORLD)) {
                func_ov170_021ecb74(syswk, i);
            }
        }
        func_ov170_021ec08c(syswk->app, id, x, y, 0);
    }
}

void func_ov170_021ec8e4(Box2SysWork *syswk) {
    s8 pos[2];
    s16 px;
    s16 py;
    u16 i;

    func_0202cf98(syswk->app->bgWinFrame, 8, &pos[1], &pos[0]);
    px = pos[0] * 8;
    py = pos[1] * 8;
    for (i = 0; i < 6; i++) {
        func_ov170_021ec08c(syswk->app, syswk->app->pokeIconId[BOX2_PARTY_POS + i],
            py + data_ov170_021efce0[i][0], px + data_ov170_021efce0[i][1], 0);
    }
}









void func_ov170_021ec94c(Box2PokeFreeWork *wk) {
    ClActorScale scale;
    ClActorPos center;

    wk->scale = 4096.0f;
    wk->scaleCnt = 0;
    func_0204b570(wk->cap, 1);
    scale.x = wk->scale;
    scale.y = wk->scale;
    func_0204b5ac(wk->cap, &scale);
    center.x = 0;
    center.y = 8;
    func_0204b588(wk->cap, &center);
}

BOOL func_ov170_021ec994(Box2PokeFreeWork *wk) {
    ClActorScale scale;

    wk->scaleCnt++;
    wk->scale -= 102.0f;
    if (wk->scaleCnt == 40) {
        return FALSE;
    }
    scale.x = wk->scale;
    scale.y = wk->scale;
    func_0204b5ac(wk->cap, &scale);
    return TRUE;
}

BOOL func_ov170_021ec9d8(Box2PokeFreeWork *wk) {
    ClActorScale scale;

    wk->scaleCnt -= 2;
    wk->scale += 102.0f;
    wk->scale += 102.0f;
    if (wk->scaleCnt == 0) {
        return FALSE;
    }
    scale.x = wk->scale;
    scale.y = wk->scale;
    func_0204b5ac(wk->cap, &scale);
    return TRUE;
}

void func_ov170_021eca24(Box2PokeFreeWork *wk) {
    func_0204b3dc(wk->cap, FALSE);
    func_ov170_021eca38(wk);
}

void func_ov170_021eca38(Box2PokeFreeWork *wk) {
    ClActorScale scale;
    ClActorPos center;

    center.x = 0;
    center.y = 0;
    func_0204b588(wk->cap, &center);
    scale.x = FX32_ONE;
    scale.y = FX32_ONE;
    func_0204b5ac(wk->cap, &scale);
    func_0204b570(wk->cap, 0);
}

void func_ov170_021eca6c(Box2SysWork *syswk, BOOL blend, u32 start, u32 end, BOOL check) {
    u32 i;

    if (check == TRUE) {
        if (syswk->param->mode == BOX2_MODE_DREAM_WORLD) {
            for (i = start; i < end; i++) {
                if (func_ov170_021dd908(syswk, i) == TRUE && func_ov170_021dd25c(syswk, i, syswk->tray, PKM_PARAM_IS_EGG, NULL) == 0) {
                    func_ov170_021ec060(syswk->app, syswk->app->pokeIconId[i], FALSE);
                } else {
                    func_ov170_021ec060(syswk->app, syswk->app->pokeIconId[i], TRUE);
                }
            }
        } else {
            for (i = start; i < end; i++) {
                if (func_ov170_021dd25c(syswk, i, syswk->tray, PKM_PARAM_ITEM, NULL) == 0) {
                    func_ov170_021ec060(syswk->app, syswk->app->pokeIconId[i], TRUE);
                } else {
                    func_ov170_021ec060(syswk->app, syswk->app->pokeIconId[i], FALSE);
                }
            }
        }
    } else {
        for (i = start; i < end; i++) {
            func_ov170_021ec060(syswk->app, syswk->app->pokeIconId[i], blend);
        }
    }
}

void func_ov170_021ecb20(Box2SysWork *syswk, u32 flags, BOOL blend) {
    BOOL check;

    if (flags & 0x80) {
        check = TRUE;
    } else {
        check = FALSE;
    }
    if (flags & 1) {
        func_ov170_021eca6c(syswk, blend, 0, BOX2_TRAY_POKE_MAX, check);
    }
    if (flags & 2) {
        func_ov170_021eca6c(syswk, blend, BOX2_TRAY_POKE_MAX, NELEMS(syswk->app->pokeIconId), check);
    }
}



void func_ov170_021ecb60(Box2AppWork *app, u32 iconPos, BOOL blend) {
    func_ov170_021ec060(app, app->pokeIconId[iconPos], blend);
}


void func_ov170_021ecb74(Box2SysWork *syswk, u32 pos) {
    func_ov170_021eca6c(syswk, TRUE, pos, pos + 1, TRUE);
}

void func_ov170_021ecb84(Box2AppWork *app, u32 index) {
    s16 x;
    s16 y;
    u8 tmp;

    func_ov170_021ec0b0(app, app->pokeIconId[index], &x, &y, 0);
    tmp = app->pokeIconId[36];
    app->pokeIconId[36] = app->pokeIconId[index];
    app->pokeIconId[index] = tmp;
    func_ov170_021ec08c(app, app->pokeIconId[index], x, y, 0);
    func_ov170_021ec768(app, index, TRUE);
}

void func_ov170_021ecbd8(Box2AppWork *app, u16 pos) {
    s16 x;
    s16 y;
    u8 tmp;

    func_ov170_021ec0b0(app, app->pokeIconId[36], &x, &y, 0);
    tmp = app->pokeIconId[pos];
    app->pokeIconId[pos] = app->pokeIconId[36];
    app->pokeIconId[36] = tmp;
    func_ov170_021ec08c(app, app->pokeIconId[36], x, y, 0);
}

void func_ov170_021ecc1c(Box2SysWork *syswk) {
    s16 x;
    s16 y;
    u32 id;

    if (syswk->unk18 != 0) {
        func_ov170_021ec0b0(syswk->app, 4, &x, &y, 0);
        if (syswk->param->mode == 3) {
            id = 10;
            y += 8;
        } else {
            id = syswk->app->pokeIconId[36];
            y += 4;
        }
        func_ov170_021ec08c(syswk->app, id, x, y, 0);
        if (syswk->param->mode == 3) {
            func_ov170_021ed310(syswk->app);
            return;
        }
        if (syswk->app->unk9E74[88] == NULL) {
            func_ov170_021ed588(syswk);
        }
        func_ov170_021ed7c4(syswk->app, 0x24);
    }
}

static void func_ov170_021ecca4(Box2AppWork *app) {
    app->unk9FF4[37] = 0xffffffff;
    app->unkA108[1] = 0xffffffff;
    app->unkA134[1] = 0xffffffff;
    app->unk9FF4[38] = 0xffffffff;
    app->unkA108[2] = 0xffffffff;
    app->unkA134[2] = 0xffffffff;
}

void func_ov170_021ecccc(Box2SysWork *syswk, Box2PokeInfo *info, u32 id) {
    u32 *pA;
    u32 *pB;
    u32 *pC;
    u32 other;
    u16 offset;
    ArcTool *handle;
    BOOL wasEncrypted;

    if (syswk->app->unkA164 == 0) {
        pA = &syswk->app->unk9FF4[37];
        pB = &syswk->app->unkA108[1];
        pC = &syswk->app->unkA134[1];
        offset = 0x60;
        other = id + 1;
    } else {
        pA = &syswk->app->unk9FF4[38];
        pB = &syswk->app->unkA108[2];
        pC = &syswk->app->unkA134[2];
        offset = 0x80;
        other = id;
        id++;
    }
    if (syswk->app->unk9E74[id] != NULL) {
        func_0204b3b4(syswk->app->unk9E74[id]);
        func_0204a8d4(*pA);
        func_0204ada4(*pB);
        func_0204afd8(*pC);
    }
    handle = MakePokeGraArcHandle(0x4c);
    wasEncrypted = PML_PkmDecrypt(info->pkm);
    *pA = func_0202d934(handle, info->pkm, 0, 1, 0x4c);
    *pB = func_0202d8d0(handle, info->pkm, 0, 1, offset, 0x4c);
    *pC = func_0202d9a4(info->pkm, 0, 2, 1, 0x4c);
    PML_PkmReEncrypt(info->pkm, wasEncrypted);
    GFL_ArcToolFree(handle);
    syswk->app->unk9E74[id] = func_ov170_021ebee0(syswk->app, &data_ov170_021efd80[id]);
    func_ov170_021ec000(syswk->app, id, TRUE);
    if (syswk->app->unk9E74[other] != NULL) {
        func_ov170_021ec000(syswk->app, other, FALSE);
    }
    syswk->app->unkA164 ^= 1;
}

void func_ov170_021ecdf4(Box2AppWork *app) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), 0x804c);
    u32 i;

    for (i = 0; i < 0x11; i++) {
        app->unk9FF4[40 + i] = func_0204a6c8(arc, func_020275f8(i), FALSE, 1, 0x4c);
    }
    app->unkA134[4] = func_0204af28(arc, func_020275fc(2), func_02027600(2), 0x4c);
    app->unkA108[4] = func_0204abf0(arc, func_020275e8(), 1, 0, 0x4c);
    app->unk9FF4[67] = func_0204a6c8(arc, func_0202774c(2), FALSE, 1, 0x4c);
    app->unkA134[10] = func_0204af28(arc, func_02027750(2), func_02027754(2), 0x4c);
    app->unkA108[9] = func_0204abf0(arc, func_02027748(), 1, 0xa0, 0x4c);
    app->unk9FF4[68] = func_0204a6c8(arc, func_0202776c(2), FALSE, 1, 0x4c);
    app->unkA134[11] = func_0204af28(arc, func_02027770(2), func_02027774(2), 0x4c);
    app->unkA108[10] = func_0204abf0(arc, func_02027768(), 1, 0xc0, 0x4c);
    app->unk9FF4[66] = func_0204a6c8(arc, func_0202774c(2), FALSE, 0, 0x4c);
    app->unkA134[9] = func_0204af28(arc, func_02027750(2), func_02027754(2), 0x4c);
    app->unkA108[8] = func_0204abf0(arc, func_02027748(), 0, 0x1a0, 0x4c);
    GFL_ArcToolFree(arc);
}


void func_ov170_021ecf78(Box2AppWork *app) {
    Box2ActorData data = data_ov170_021efd34;
    u32 i;

    for (i = 0; i < 0x11; i++) {
        data.chr = i + 0x28;
        app->unk9E74[i + 28] = func_ov170_021ebee0(app, &data);
        func_0204b6f4(app->unk9E74[i + 28], func_020275ec((u8)i), 1);
        func_ov170_021ec000(app, i + 28, FALSE);
    }
}

void func_ov170_021ecfdc(Box2AppWork *app, u8 type1, u8 type2) {
    func_ov170_021ec000(app, type1 + 0x1c, TRUE);
    func_ov170_021ec08c(app, type1 + 0x1c, 0x58, 0x38, 1);
    if (type2 != 0 && type1 != type2) {
        func_ov170_021ec000(app, type2 + 0x1c, TRUE);
        func_ov170_021ec08c(app, type2 + 0x1c, 0x7a, 0x38, 1);
    }
}








void func_ov170_021ed028(Box2AppWork *app, Box2PokeInfo *info) {
    u32 i;

    for (i = 0x1c; i < 0x2d; i++) {
        func_ov170_021ec000(app, i, FALSE);
    }
    if (info->egg == 0) {
        func_ov170_021ecfdc(app, info->type1, info->type2);
    }
}

void func_ov170_021ed058(Box2AppWork *app) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0x19, 0x4c);

    app->unkA108[3] = func_0204abf0(arc, GetItemGraphicsDatID(0, 2), 0, 0x180, 0x4c);
    GFL_ArcToolFree(arc);
}

void func_ov170_021ed094(Box2AppWork *app, u16 item) {
    NNSG2dCharacterData *character;
    NNSG2dPaletteData *palette;
    void *buf;

    buf = GFL_G2DIOReadOBJNCGR(func_02020f08(), GetItemGraphicsDatID(item, 1), FALSE, &character, 0x4c);
    func_0204a9d0(func_0204b7f0(app->unk9E74[10]), character);
    GFL_HeapFree(buf);
    buf = GFL_G2DIOReadNCLR(func_02020f08(), GetItemGraphicsDatID(item, 2), &palette, 0x4c);
    func_0204adec(func_0204b7f8(app->unk9E74[10]), palette, 1);
    GFL_HeapFree(buf);
}

void func_ov170_021ed108(Box2AppWork *app, u32 mode) {
    ClActorPos pos;

    if (mode == 1) {
        func_0204b570(app->unk9E74[10], 1);
        pos.x = 12;
        pos.y = 12;
        func_0204b588(app->unk9E74[10], &pos);
    } else {
        pos.x = 0;
        pos.y = 0;
        func_0204b588(app->unk9E74[10], &pos);
        func_0204b570(app->unk9E74[10], 0);
    }
}





void func_ov170_021ed14c(Box2AppWork *app, s16 x, s16 y) {
    func_ov170_021ec08c(app, 10, x, y, 0);
}

void func_ov170_021ed164(Box2AppWork *app, u32 pos, u32 mode) {
    s16 x;
    s16 y;

    func_ov170_021ec548(pos, &x, &y, mode);
    func_ov170_021ed14c(app, x + 8, y + 8);
}


void func_ov170_021ed198(Box2AppWork *app, u32 pos, u32 mode) {
    s16 x;
    s16 y;

    func_ov170_021ec548(pos, &x, &y, mode);
    func_ov170_021ed14c(app, x, y + 4);
}

void func_ov170_021ed1c8(Box2AppWork *app) {
    Box2ActorData data;
    s16 x;
    s16 y;
    u32 i;

    if (app->itemId == 0) {
        return;
    }
    func_ov170_021ec0b0(app, 10, &x, &y, 0);
    if (app->unk9E74[88] == NULL) {
        data = data_ov170_021efd80[10];
        data.setup.priority = func_0204b838(app->unk9E74[10]) + 1;
        data.setup.bgPriority = func_0204b864(app->unk9E74[10]);
        data.chr = 0x27;
        data.pal = 6;
        data.palette = 1;
        for (i = 0; i < 8; i++) {
            data.setup.x = x + data_ov170_021efcd0[i];
            data.setup.y = y + data_ov170_021efcc8[i];
            app->unk9E74[88 + i] = func_ov170_021ebee0(app, &data);
            func_0204b6f4(app->unk9E74[88 + i], (u8)data.palette, 1);
            func_ov170_021ec000(app, 88 + i, FALSE);
        }
    } else {
        u32 priority = func_0204b838(app->unk9E74[10]);
        u32 bgPriority = func_0204b864(app->unk9E74[10]);

        for (i = 0; i < 8; i++) {
            func_ov170_021ec08c(app, 88 + i, x + data_ov170_021efcd0[i], y + data_ov170_021efcc8[i], 0);
            func_ov170_021ec048(app, 88 + i, priority + 1);
            func_ov170_021ec030(app, 88 + i, bgPriority);
            func_ov170_021ec000(app, 88 + i, FALSE);
        }
    }
}



void func_ov170_021ed2f4(Box2AppWork *app) {
    u32 i;

    for (i = 0; i < 8; i++) {
        func_ov170_021ec000(app, i + 0x58, TRUE);
    }
}

void func_ov170_021ed310(Box2AppWork *app) {
    s16 x;
    s16 y;
    u32 i;

    if (app->unk9E74[88] != NULL) {
        func_ov170_021ec0b0(app, 10, &x, &y, 0);
        for (i = 0; i < 8; i++) {
            func_ov170_021ec08c(app, 0x58 + i, x + data_ov170_021efcd0[i], y + data_ov170_021efcc8[i], 0);
        }
    }
}

void func_ov170_021ed370(Box2SysWork *syswk) {
    Box2AppWork *app = syswk->app;
    unsigned long i;

    app->unk9E74[0] = func_ov170_021ebee0(app, &data_ov170_021efd80[0]);
    app->unk9E74[1] = func_ov170_021ebee0(app, &data_ov170_021efd80[1]);
    app->unk9E74[2] = func_ov170_021ebee0(app, &data_ov170_021efd80[2]);
    app->unk9E74[3] = func_ov170_021ebee0(app, &data_ov170_021efd80[3]);
    app->unk9E74[4] = func_ov170_021ebee0(app, &data_ov170_021efd80[4]);
    app->unk9E74[5] = func_ov170_021ebee0(app, &data_ov170_021efd80[5]);
    app->unk9E74[10] = func_ov170_021ebee0(app, &data_ov170_021efd80[10]);
    app->unk9E74[6] = func_ov170_021ebee0(app, &data_ov170_021efd80[6]);
    app->unk9E74[7] = func_ov170_021ebee0(app, &data_ov170_021efd80[7]);
    app->unk9E74[8] = func_ov170_021ebee0(app, &data_ov170_021efd80[8]);
    app->unk9E74[9] = func_ov170_021ebee0(app, &data_ov170_021efd80[9]);
    for (i = 0; i < 6; i++) {
        app->unk9E74[13 + i] = func_ov170_021ebee0(app, &data_ov170_021efd80[13 + i]);
        app->unk9E74[19 + i] = func_ov170_021ebee0(app, &data_ov170_021efd80[19 + i]);
    }
    app->unk9E74[25] = func_ov170_021ebee0(app, &data_ov170_021efd80[25]);
    app->unk9E74[26] = func_ov170_021ebee0(app, &data_ov170_021efd80[26]);
    app->unk9E74[27] = func_ov170_021ebee0(app, &data_ov170_021efd80[27]);
    func_ov170_021ec000(app, 5, FALSE);
    func_ov170_021ec000(app, 10, FALSE);
    func_ov170_021ec000(app, 25, FALSE);
    func_ov170_021ec000(app, 26, FALSE);
    func_ov170_021ec000(app, 27, FALSE);
    func_ov170_021ed508(syswk, 14, TRUE);
}

void func_ov170_021ed508(Box2SysWork *syswk, u32 set, BOOL visible) {
    func_ov170_021ebfa8(syswk->app, 9, set);
    func_ov170_021ec000(syswk->app, 9, visible);
    if (syswk->param->mode == 2 || syswk->param->mode == 4 || syswk->param->mode == 3) {
    } else {
        func_ov170_021ec000(syswk->app, 9, FALSE);
    }
}





void func_ov170_021ed544(Box2SysWork *syswk, u32 anim) {
    if (syswk->moveMode != 0) {
        anim += 4;
    }
    func_ov170_021ebfa8(syswk->app, 4, anim);
}

void func_ov170_021ed55c(Box2SysWork *syswk, BOOL show) {
    if (show == TRUE) {
        if (func_020260f0(syswk->app->cursorMove) == TRUE) {
            func_ov170_021ec000(syswk->app, 4, TRUE);
        }
    } else {
        func_ov170_021ec000(syswk->app, 4, FALSE);
    }
}




void func_ov170_021ed588(Box2SysWork *syswk) {
    func_ov170_021ed594(syswk, syswk->pos);
}

void func_ov170_021ed594(Box2SysWork *syswk, u32 pos) {
    Box2AppWork *app;
    Box2ActorData data;
    s16 x;
    s16 y;
    u32 id;
    u16 i;

    if (pos == BOX2_GET_NONE) {
        return;
    }
    app = syswk->app;
    id = app->pokeIconId[pos];
    func_ov170_021ec0b0(app, id, &x, &y, 0);
    if (app->unk9E74[88] == NULL) {
        ClActor **actors = app->unk9E74;

        data = data_ov170_021efd04;
        data.setup.priority = func_0204b838(actors[id]) + 1;
        data.setup.bgPriority = func_0204b864(actors[id]);
        data.chr = id - BOX2_ACTOR_POKEICON;
        data.pal = 6;
        data.palette = 1;
        for (i = 0; i < 8; i++) {
            data.setup.x = x + data_ov170_021efcd0[i];
            data.setup.y = y + data_ov170_021efcc8[i];
            app->unk9E74[88 + i] = func_ov170_021ebee0(app, &data);
            func_0204b6f4(app->unk9E74[88 + i], (u8)data.palette, 1);
        }
    } else {
        ClActor **actors = app->unk9E74;
        u32 priority;
        u32 bgPriority;
        NNSG2dImageProxy proxy;

        func_0204b7c8(actors[id], &proxy);
        priority = func_0204b838(actors[id]);
        bgPriority = func_0204b864(actors[id]);
        for (i = 0; i < 8; i++) {
            func_0204b784(app->unk9E74[88 + i], &proxy);
            func_ov170_021ec08c(app, 88 + i, x + data_ov170_021efcd0[i], y + data_ov170_021efcc8[i], 0);
            func_ov170_021ec048(app, 88 + i, priority + 1);
            func_ov170_021ec030(app, 88 + i, bgPriority);
            func_ov170_021ec000(app, 88 + i, TRUE);
        }
    }
}

void func_ov170_021ed6e4(Box2SysWork *syswk, BOOL visible) {
    u32 i;
    u8 id;

    for (i = 0; i < 8; i++) {
        if (syswk->app->unk9E74[88 + i] != NULL) {
            func_ov170_021ec000(syswk->app, i + 0x58, visible);
            if (visible == TRUE && syswk->pos != BOX2_GET_NONE) {
                id = syswk->app->pokeIconId[syswk->pos];
                func_ov170_021ec048(syswk->app, i + 0x58, func_0204b838(syswk->app->unk9E74[id]) + 1);
                func_ov170_021ec030(syswk->app, i + 0x58, func_0204b864(syswk->app->unk9E74[id]));
            }
        }
    }
}

void func_ov170_021ed75c(Box2SysWork *syswk, u32 pos) {
    u8 id = syswk->app->pokeIconId[pos];
    u8 priority = func_0204b838(syswk->app->unk9E74[id]);
    u8 bgPriority = func_0204b864(syswk->app->unk9E74[id]);
    u32 i;

    for (i = 0; i < 8; i++) {
        if (syswk->app->unk9E74[88 + i] != NULL && syswk->pos != BOX2_GET_NONE) {
            func_ov170_021ec048(syswk->app, i + 0x58, priority + 1);
            func_ov170_021ec030(syswk->app, i + 0x58, bgPriority);
        }
    }
}

void func_ov170_021ed7c4(Box2AppWork *app, u32 pos) {
    s16 x;
    s16 y;
    u32 i;

    if (pos != BOX2_GET_NONE) {
        func_ov170_021ec0b0(app, app->pokeIconId[pos], &x, &y, 0);
        for (i = 0; i < 8; i++) {
            func_ov170_021ec08c(app, i + 0x58, x + data_ov170_021efcd0[i], y + data_ov170_021efcc8[i], 0);
        }
    }
}

static void func_ov170_021ed824(Box2AppWork *app) {
    Box2ActorData data;
    Box2ActorData base = data_ov170_021efd1c;
    u32 i;

    for (i = 0; i < 6; i++) {
        data = base;
        data.setup.y += (s16)(i * 34);
        data.chr = i + 0x39;
        app->unk9E74[45 + i] = func_ov170_021ebee0(app, &data);
    }
}






void func_ov170_021ed888(Box2AppWork *app, BOOL visible) {
    func_ov170_021ec000(app, 0, visible);
    func_ov170_021ec000(app, 1, visible);
}

void func_ov170_021ed8a4(Box2SysWork *syswk) {
    s8 pos[2];
    s16 px;
    s16 py;
    u32 i;

    func_0202cf98(syswk->app->bgWinFrame, 7, &pos[1], &pos[0]);
    py = pos[0] * 8 + 8;
    px = pos[1] * 8 + 23;
    for (i = 0; i < 6; i++) {
        func_ov170_021ec08c(syswk->app, i + 13, px + (i % 2) * 32, py + (i / 2) * 24, 0);
    }
}



void func_ov170_021ed904(Box2SysWork *syswk) {
    func_ov170_021edab0(syswk, syswk->tray);
    func_ov170_021ed9c8(syswk);
}

void func_ov170_021ed918(Box2SysWork *syswk, s32 mv) {
    s16 x;
    s16 y;
    u32 i;

    func_ov170_021ec0b0(syswk->app, 2, &x, &y, 0);
    func_ov170_021ec08c(syswk->app, 2, x - mv, y, 0);
    for (i = 0; i < 6; i++) {
        func_ov170_021ec0b0(syswk->app, i + 45, &x, &y, 0);
        func_ov170_021ec08c(syswk->app, i + 45, x - mv, y, 0);
        func_020258e8(syswk->app->fontOam[1 + i].oam, &x, &y);
        func_0202590c(syswk->app->fontOam[1 + i].oam, x - mv, y);
    }
}


void func_ov170_021ed9b4(Box2AppWork *app) {
    func_ov170_021ec08c(app, 2, 300, 34, 0);
}

void func_ov170_021ed9c8(Box2SysWork *syswk) {
    Box2ActorData data;
    Box2ActorData base = data_ov170_021efd1c;
    u8 tray;
    s16 i;

    for (i = 0; i < 6; i++) {
        tray = func_ov170_021deb14(syswk, i - 1);
        data = base;
        data.setup.y += (s16)(i * 34);
        func_ov170_021ec08c(syswk->app, i + 45, data.setup.x, data.setup.y, 0);
        func_ov170_021edb64(syswk, tray, i + 45);
        func_0202590c(syswk->app->fontOam[i + 1].oam, 316, i * 34 + 2);
        func_ov170_021eb874(syswk, tray, i + 1);
    }
}

void func_ov170_021eda70(Box2AppWork *app, u32 row, s16 *x, s16 *y) {
    u32 i;

    row = (row + 1) * 34;
    for (i = 0; i < 6; i++) {
        func_ov170_021ec0b0(app, i + 45, x, y, 0);
        if (row == *y) {
            return;
        }
    }
    *x = 0;
    *y = 0;
}

void func_ov170_021edab0(Box2SysWork *syswk, u32 tray) {
    NNSG2dCharacterData *character;
    void *buf;
    u32 size;

    buf = GFL_G2DIOReadOBJNCGR(ARCID_BOX2, 71, TRUE, &character, HEAPID_BOX2_APP);
    size = 0x400;
    MI_CpuCopy8(character->pRawData, syswk->app->trayIconChar[tray], size);
    GFL_HeapFree(buf);
    func_ov170_021edbe8(syswk, syswk->app->trayIconChar[tray], func_ov170_021dedcc(syswk, tray), 8, size);
    func_ov170_021edc0c(syswk, tray, syswk->app->trayIconChar[tray]);
}





void func_ov170_021edb1c(Box2SysWork *syswk) {
    u32 i;

    for (i = 0; i < BOX2_TRAY_MAX; i++) {
        func_ov170_021edab0(syswk, i);
    }
}

void func_ov170_021edb34(Box2SysWork *syswk, u32 tray) {
    s16 i;

    for (i = 0; i < 6; i++) {
        u32 scroll = func_ov170_021deb14(syswk, i - 1);

        if (scroll == tray) {
            func_ov170_021edb94(syswk, tray, i);
            return;
        }
    }
}

void func_ov170_021edb64(Box2SysWork *syswk, u32 tray, u32 id) {
    func_0204aa94(func_0204b7f0(syswk->app->unk9E74[id]), syswk->app->trayIconChar[tray], 0x400, 0, 0);
}

void func_ov170_021edb94(Box2SysWork *syswk, u32 tray, u32 row) {
    s16 x;
    s16 y;
    u32 i;

    row *= 34;
    for (i = 0; i < 6; i++) {
        func_ov170_021ec0b0(syswk->app, i + 45, &x, &y, 0);
        if (row == y) {
            func_ov170_021edb64(syswk, tray, i + 45);
            func_ov170_021eb874(syswk, tray, i + 1);
            return;
        }
    }
}



static void func_ov170_021edbe8(Box2SysWork *syswk, u8 *chr, u32 wallpaper, u32 color, u32 size) {
    u32 i;

    for (i = 0; i < size; i++) {
        if (chr[i] == color) {
            chr[i] = data_ov170_021efcec[wallpaper];
        }
    }
}

static void func_ov170_021edc0c(Box2SysWork *syswk, u32 tray, u8 *chr) {
    BoxPkm *pkm;
    BOOL encrypted;
    u8 j;
    u8 i;
    u8 px;
    u8 py;
    u32 species;
    void *personal;
    u8 y;
    u16 color;
    u16 form;

    py = 11;
    for (j = 0; j < 5; j++) {
        px = 10;
        for (i = 0; i < 6; i++) {
            pkm = BoxSaveAccessor_GetPkm(syswk->param->boxes, tray, i + j * 6);
            encrypted = PML_PkmDecrypt(pkm);
            species = PML_PkmGetParam(pkm, PKM_PARAM_SPECIES, NULL);
            if (PML_PkmGetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
                if (PML_PkmGetParam(pkm, PKM_PARAM_IS_EGG, NULL) == 0) {
                    form = PML_PkmGetParam(pkm, PKM_PARAM_FORM, NULL);
                    personal = PML_PersonalLoad(species, form, HEAPID_BOX2_APP);
                    color = PML_PersonalGetParam(personal, 33);
                    PML_PersonalFree(personal);
                } else if (species == SPECIES_MANAPHY) {
                    color = 1;
                } else {
                    color = 8;
                }
                color += 16;
                color = (color << 8) | color;
                for (y = py; y < py + 2; y++) {
                    MIi_CpuClear16(color, &chr[((y & 7) << 3) + ((((y >> 3) << 2) + (px >> 3)) << 6) + (px & 7)], 2);
                }
            }
            PML_PkmReEncrypt(pkm, encrypted);
            px += 2;
        }
        py += 2;
    }
}

void func_ov170_021edd38(Box2SysWork *syswk, s16 mv) {
    s16 x;
    s16 y;
    s16 fx;
    s16 fy;
    s32 speed;
    s16 ny;
    s16 nfy;
    u32 i;
    u8 tray;

    speed = ((mv < 0 ? -mv : mv) == 5) ? 2 : 8;
    if (mv < 0) {
        speed *= -1;
    }
    for (i = 0; i < 6; i++) {
        func_ov170_021ec0b0(syswk->app, i + 45, &x, &y, 0);
        ny = y + speed;
        func_ov170_021ec08c(syswk->app, i + 45, x, ny, 0);
        func_020258e8(syswk->app->fontOam[i + 1].oam, &fx, &fy);
        nfy = fy + speed;
        func_0202590c(syswk->app->fontOam[i + 1].oam, fx, nfy);
        if (ny < -16) {
            tray = func_ov170_021deb14(syswk, 4);
            func_ov170_021edb64(syswk, tray, i + 45);
            ny += 204;
            func_ov170_021ec08c(syswk->app, i + 45, x, ny, 0);
            func_ov170_021eb874(syswk, tray, i + 1);
            nfy += 204;
            func_0202590c(syswk->app->fontOam[i + 1].oam, fx, nfy);
        } else if (ny >= 186) {
            tray = func_ov170_021deb14(syswk, -1);
            func_ov170_021edb64(syswk, tray, i + 45);
            ny -= 204;
            func_ov170_021ec08c(syswk->app, i + 45, x, ny, 0);
            func_ov170_021eb874(syswk, tray, i + 1);
            nfy -= 204;
            func_0202590c(syswk->app->fontOam[i + 1].oam, fx, nfy);
        }
    }
    if (func_ov170_021ec01c(syswk->app, 2) == TRUE) {
        func_ov170_021ec0b0(syswk->app, 2, &x, &y, 0);
        func_ov170_021ec08c(syswk->app, 2, x, y + speed, 0);
    }
}

void func_ov170_021edecc(Box2SysWork *syswk, s32 mv) {
    s16 x;
    s16 y;

    if (mv < 0 && func_ov170_021deb14(syswk, -1) == syswk->tray) {
        func_ov170_021ec0b0(syswk->app, 2, &x, &y, 0);
        func_ov170_021ec08c(syswk->app, 2, x, -34, 0);
        func_ov170_021ec000(syswk->app, 2, TRUE);
    } else if (mv > 0 && func_ov170_021deb14(syswk, 4) == syswk->tray) {
        func_ov170_021ec0b0(syswk->app, 2, &x, &y, 0);
        func_ov170_021ec08c(syswk->app, 2, x, 204, 0);
        func_ov170_021ec000(syswk->app, 2, TRUE);
    }
}

void func_ov170_021edf5c(Box2SysWork *syswk) {
    s16 x;
    s16 y;
    u32 i;

    for (i = 0; i < 6; i++) {
        if (func_ov170_021deb14(syswk, i - 1) == syswk->tray) {
            func_ov170_021ec0b0(syswk->app, 2, &x, &y, 0);
            func_ov170_021ec08c(syswk->app, 2, x, i * 34, 0);
            func_ov170_021ec000(syswk->app, 2, TRUE);
            return;
        }
    }
    func_ov170_021ec000(syswk->app, 2, FALSE);
}

void func_ov170_021edfc4(Box2SysWork *syswk, u32 index) {
    s16 y = data_ov170_021efcd8[index];

    func_ov170_021ec08c(syswk->app, 3, 0xce, y, 0);
    func_ov170_021ec000(syswk->app, 3, TRUE);
    func_0202590c(syswk->app->fontOam[0].oam, 0x9e, y - 8);
    func_02025774(syswk->app->fontOam[0].oam, TRUE);
}





void func_ov170_021ee008(Box2SysWork *syswk, u32 index, BOOL visible) {
    if (visible == TRUE) {
        func_ov170_021eb85c(syswk, index);
        func_ov170_021edfc4(syswk, index);
    } else {
        func_ov170_021ec000(syswk->app, 3, FALSE);
        func_02025774(syswk->app->fontOam[0].oam, FALSE);
    }
}

void func_ov170_021ee038(Box2SysWork *syswk, u8 button1, u8 button2, u8 button3) {
    syswk->unk20 = button2;
    syswk->unk1F = button1;
    syswk->unk21 = button3;
    if (button1 == 0) {
        func_ov170_021ec000(syswk->app, 6, TRUE);
        func_ov170_021ebfcc(syswk->app, 6, 1);
    } else if (button1 == 1) {
        func_ov170_021ec000(syswk->app, 6, FALSE);
    } else {
        func_ov170_021ec000(syswk->app, 6, TRUE);
        func_ov170_021ebfcc(syswk->app, 6, 15);
    }
    if (button2 == 0) {
        func_ov170_021ec000(syswk->app, 7, TRUE);
        func_ov170_021ebfcc(syswk->app, 7, 0);
    } else if (button2 == 1) {
        func_ov170_021ec000(syswk->app, 7, FALSE);
    } else {
        func_ov170_021ec000(syswk->app, 7, TRUE);
        func_ov170_021ebfcc(syswk->app, 7, 14);
    }
    if (button3 == 0) {
        func_ov170_021ec000(syswk->app, 8, TRUE);
        func_ov170_021ebfcc(syswk->app, 8, 0);
    } else if (button3 == 1) {
        func_ov170_021ec000(syswk->app, 8, FALSE);
    } else {
        func_ov170_021ec000(syswk->app, 8, TRUE);
        func_ov170_021ebfcc(syswk->app, 8, 2);
    }
}

void func_ov170_021ee0fc(Box2SysWork *syswk) {
    func_ov170_021ec000(syswk->app, 6, FALSE);
    func_ov170_021ec000(syswk->app, 7, FALSE);
    func_ov170_021ec000(syswk->app, 8, FALSE);
}

void func_ov170_021ee120(Box2AppWork *app) {
    BmpOamActorSetup setup;
    Box2FontOam *font;
    u32 i;

    app->bmpOam = func_02025538(0x4c, app->unk9E70);
    app->fontOam[0].bitmap = GFL_BitmapCreate(12, 2, 0x20, 0x4c);
    setup.bitmap = app->fontOam[0].bitmap;
    setup.x = 0;
    setup.y = 0;
    setup.palette = app->unkA108[6];
    setup.paletteOffset = 0;
    setup.priority = 4;
    setup.bgPriority = 1;
    setup.surface = 0xffff;
    setup.vramType = CLACT_VRAM_MAIN;
    app->fontOam[0].oam = func_020255a0(app->bmpOam, &setup);
    func_02025774(app->fontOam[0].oam, FALSE);
    for (i = 7; i <= 8; i++) {
        font = &app->fontOam[i];
        font->bitmap = GFL_BitmapCreate(12, 2, 0x20, 0x4c);
        setup.bitmap = font->bitmap;
        setup.x = 36;
        setup.y = 20;
        setup.bgPriority = 3;
        font->oam = func_020255a0(app->bmpOam, &setup);
    }
    for (i = 0; i < 6; i++) {
        font = &app->fontOam[1 + i];
        font->bitmap = GFL_BitmapCreate(2, 1, 0x20, 0x4c);
        setup.bitmap = font->bitmap;
        setup.x = 316;
        setup.y = i * 34 + 2;
        setup.bgPriority = 1;
        font->oam = func_020255a0(app->bmpOam, &setup);
    }
}





void func_ov170_021ee200(Box2AppWork *app) {
    u32 i;

    for (i = 0; i < 9; i++) {
        func_0202570c(app->fontOam[i].oam);
        GFL_BitmapFree(app->fontOam[i].bitmap);
    }
    func_02025588(app->bmpOam);
}

void func_ov170_021ee224(Box2AppWork *app, u32 index, BOOL visible) {
    func_02025774(app->fontOam[index].oam, visible);
}

BOOL func_ov170_021ee234(Box2AppWork *app, u32 index) {
    return func_020257c4(app->fontOam[index].oam);
}

void func_ov170_021ee244(Box2AppWork *app, u32 index, u32 dir) {
    if (dir == 0) {
        func_0202590c(app->fontOam[index].oam, -148, 20);
    } else {
        func_0202590c(app->fontOam[index].oam, 220, 20);
    }
}

void func_ov170_021ee26c(Box2AppWork *app, s32 mv) {
    s16 x;
    s16 y;
    u32 i;

    for (i = 7; i <= 8; i++) {
        func_020258e8(app->fontOam[i].oam, &x, &y);
        func_0202590c(app->fontOam[i].oam, x + mv, y);
        if (x + mv == -148 || x + mv == 220) {
            func_ov170_021ee224(app, i, FALSE);
        }
    }
}
