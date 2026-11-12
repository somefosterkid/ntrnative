// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov193/pokemontrade_demo_down.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/pokemon_trade_local.h"
#include "constants/pokemon.h"
#include "gfl/arc.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/particle.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "system/mcss.h"
#include "system/wipe.h"

// pokemontrade_demo_down.c, a guessed name: one of the trade demo's three scenes, a copy of the template that
// pokemontrade_demo_common.c and pokemontrade_demo_up.c share, whose sprites fly by in front of the effects

// The scene's frame counter starts at this frame
#define DOWN_START_FRAME 0x2b5

int func_ov118_021d56e0(int frame);
void func_ov119_021e2f2c(PokemonTrade3DWork *work);
void func_ov119_021dfa60(int timer);
void func_ov119_021dfda0(PokemonTradeWork *wk);
BOOL func_ov119_021d8aa8(PokemonTradeWork *wk);
u32 getIDAsUInt(PlayerInfo *info);
void func_ov119_021e36c8(PokemonTradeWork *wk, int side, BOOL front, PartyPkm *pkm, BOOL check);
static void func_ov118_021d54f0(PokemonTradeWork *wk, u8 *ownedByPlayer, u8 *noBounce, PlayerInfo *player, BOOL partner);
void func_ov118_021d55ac(PokemonTradeWork *wk);
void func_ov118_021d5628(PokemonTradeWork *wk);
void func_ov118_021d5694(PokemonTradeWork *wk);
void func_ov118_021d56e4(PokemonTradeWork *wk, int timer);
void func_ov118_021d56f0(PokemonTradeWork *wk);
void func_ov118_021d56fc(PokemonTradeWork *wk);
void func_ov118_021d572c(PokemonTradeWork *wk, int index);
void func_ov118_021d5748(PokemonTradeWork *wk, int index);
void func_ov118_021d5888(PokemonTradeWork *wk, int index);
void func_ov118_021d58f0(SPLEmitter *emitter, u32 type);
void func_ov118_021d5930(SPLEmitter *emitter);
void func_ov118_021d5950(SPLEmitter *emitter, u32 type);
void func_ov118_021d5990(SPLEmitter *emitter);
void func_ov118_021d5a60(PokemonTradeWork *wk);
void func_ov118_021d5b74(PokemonTradeWork *wk);
void func_ov118_021d5b98(PokemonTradeWork *wk);
static void func_ov118_021d5bb8(PokemonTradeWork *wk);

// The hops of a Pokémon's sprite, as offsets from its place: one hop of 18 frames, or two of 40. The common file has
// the same
// pokebw2's sTradeDemoDownHopPath
static VecFx32 data_ov118_021d7fec[18] = {
#include "asset/pokemontrade_demo_down_data_ov118_021d7fec.inc"
};

// pokebw2's sTradeDemoDownLongHopPath
static VecFx32 data_ov118_021d80c4[40] = {
#include "asset/pokemontrade_demo_down_data_ov118_021d80c4.inc"
};

// The path that the sprites follow when they don't hop, which nothing fills
// pokebw2's sTradeDemoDownScenePath
static VecFx32 data_ov118_021d8790[20];

// Does nothing and returns its argument. The frame that a step happens at goes through it
// pokebw2's PokemonTradeDemoDown_Frame




static void func_ov118_021d54f0(PokemonTradeWork *wk, u8 *ownedByPlayer, u8 *noBounce, PlayerInfo *player,
                                BOOL partner) {
    PartyPkm *pkm;
    int side;
    u32 id;
    u32 species;
    u32 form;
    u32 egg;
    u32 noBounceParam;
    void *personal;

    if (partner) {
        if (func_ov119_021d8aa8(wk)) {
            side = 1;
        } else {
            side = 0;
        }
    } else {
        if (func_ov119_021d8aa8(wk)) {
            side = 0;
        } else {
            side = 1;
        }
    }
    pkm = func_ov119_021d8fa4(wk, side);
    id = PokeParty_GetParam(pkm, PKM_PARAM_ID, NULL);
    species = PokeParty_GetParam(pkm, PKM_PARAM_LEGAL_SPECIES, NULL);
    form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
    egg = PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL);
    if (egg == 0) {
        personal = PML_PersonalLoad(species, form, wk->heapId);
        noBounceParam = PML_PersonalGetParam(personal, PERSONAL_NO_BOUNCE);
        PML_PersonalFree(personal);
    }
    if (egg != 0) {
        *noBounce = TRUE;
    } else if (noBounceParam != 0) {
        *noBounce = TRUE;
    } else {
        *noBounce = FALSE;
    }
    if (id != getIDAsUInt(player)) {
        *ownedByPlayer = FALSE;
    } else {
        *ownedByPlayer = TRUE;
    }
}

void func_ov118_021d55ac(PokemonTradeWork *wk) {
    if (wk->work3D->actor == NULL) {
        ClActorSetup setup;

        setup.x = 0x80;
        setup.y = 0x63;
        setup.sequence = 0;
        setup.priority = 0;
        setup.bgPriority = 0;
        wk->work3D->actor = func_0204b294(wk->clactUnit, wk->objRes[11], wk->objRes[4], wk->objRes[16], &setup, 0, wk->heapId);
        func_0204b92c(wk->work3D->actor, 1);
        func_0204b3dc(wk->work3D->actor, 1);
    } else {
        func_0204b9a0(wk->work3D->actor);
    }
}

void func_ov118_021d5628(PokemonTradeWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0x67, wk->heapId);

    wk->objRes[11] = func_0204a6c8(arc, 2, FALSE, 0, wk->heapId);
    wk->objRes[4] = func_0204ac18(arc, 3, 0, 0xe0, 0, 1, wk->heapId);
    wk->objRes[16] = func_0204af28(arc, 1, 0, wk->heapId);
    GFL_ArcToolFree(arc);
    wk->work3D->actor = NULL;
}

void func_ov118_021d5694(PokemonTradeWork *wk) {
    if (wk->work3D->actor != NULL) {
        func_0204b3b4(wk->work3D->actor);
    }
    if (wk->objRes[11] != 0) {
        func_0204a8d4(wk->objRes[11]);
        func_0204ada4(wk->objRes[4]);
        func_0204afd8(wk->objRes[16]);
    }
    wk->objRes[11] = 0;
    wk->objRes[4] = 0;
    wk->objRes[16] = 0;
    wk->work3D->actor = NULL;
}

int func_ov118_021d56e0(int frame) {
    return frame;
}

void func_ov118_021d56e4(PokemonTradeWork *wk, int timer) {
    wk->timer = timer;
}

void func_ov118_021d56f0(PokemonTradeWork *wk) {
    wk->bgmTimer = 1;
}

void func_ov118_021d56fc(PokemonTradeWork *wk) {
    func_ov118_021d5694(wk);
    func_ov119_021e3034(wk->work3D, 6);
    func_ov119_021e2f2c(wk->work3D);
    GFL_HeapFree(wk->work3D);
    wk->work3D = NULL;
    func_ov119_021d8e84(wk, func_ov119_021dfda0);
}


void func_ov118_021d572c(PokemonTradeWork *wk, int index) {
    if (wk->mcssMoves[0] != NULL) {
        GFL_HeapFree(wk->mcssMoves[0]);
        wk->mcssMoves[0] = NULL;
    }
}

void func_ov118_021d5748(PokemonTradeWork *wk, int index) {
    PartyPkm *pkm;
    VecFx32 pos;

    pkm = func_ov119_021d8fa4(wk, 1 - func_ov119_021d8aa8(wk));
    PokeParty_GetParam(pkm, PKM_PARAM_ID, NULL);
    if (PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL) == 0) {
        u32 species = PokeParty_GetParam(pkm, PKM_PARAM_LEGAL_SPECIES, NULL);
        u32 form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);

        PokeVoice_Play(species, form, 0x40, 0, 0, 0, 0, NULL);
    }
    if (wk->unk11EE == 0) {
        func_02015c30(wk->mcss[index], &pos);
        func_ov119_021dfd24(wk->mcssMoves[3]);
        if (wk->unk11F0 != 0) {
            wk->mcssMoves[3] = func_ov119_021dfbb4(wk->mcss[index], 20, &pos, data_ov118_021d8790, wk->heapId);
        } else {
            wk->mcssMoves[3] = func_ov119_021dfbb4(wk->mcss[index], 40, &pos, data_ov118_021d80c4, wk->heapId);
        }
    } else {
        u32 pair[4] = { 2, 3, 0, 1 };
        MCSS *target = wk->mcss[pair[index]];

        func_02015c30(wk->mcss[index], &pos);
        func_ov119_021dfd24(wk->mcssMoves[3]);
        if (wk->unk11F0 != 0) {
            wk->mcssMoves[3] = func_ov119_021dfbb4(target, 20, &pos, data_ov118_021d8790, wk->heapId);
        } else {
            wk->mcssMoves[3] = func_ov119_021dfbb4(target, 18, &pos, data_ov118_021d7fec, wk->heapId);
        }
    }
}





// Shows a sprite still, at the start of the scene, and scales it by its size
// pokebw2's PokemonTradeDemoDown_ShowSprite
void func_ov118_021d5888(PokemonTradeWork *wk, int index) {
    VecFx32 pos;

    func_02015eb0(wk->mcss[index]);
    pos.x = 0;
    pos.y = FX32_CONST(-23);
    pos.z = FX32_CONST(10);
    func_02015c48(wk->mcss[index], &pos);
    func_02015e04(wk->mcss[index]);
    func_02015c94(wk->mcss[index], &pos);
    if (pos.x < 0) {
        pos.x = -FX32_ONE * 16;
    } else {
        pos.x = FX32_ONE * 16;
    }
    pos.y = FX32_ONE * 16;
    pos.z = FX32_ONE;
    func_02015cac(wk->mcss[index], &pos);
}

void func_ov118_021d58f0(SPLEmitter *emitter, u32 type) {
    PokemonTradeWork *wk;

    if (type != 0 && emitter != NULL) {
        wk = func_02051a34(emitter);
        if (wk->work3D->emitterPaths[0] != NULL) {
            VecFx32 pos = { 0, 0, 0 };

            GFL_G3DCurveGetNowTranslation(wk->work3D->emitterPaths[0], &pos);
            func_02051838(emitter, &pos);
        }
    }
}


// pokebw2's PokemonTradeDemoDown_AttachEmitterPath0
void func_ov118_021d5930(SPLEmitter *emitter) {
    func_02051a2c(emitter, func_02051774());
    func_02051a24(emitter, func_ov118_021d58f0);
}

void func_ov118_021d5950(SPLEmitter *emitter, u32 type) {
    PokemonTradeWork *wk;

    if (type != 0 && emitter != NULL) {
        wk = func_02051a34(emitter);
        if (wk->work3D->emitterPaths[1] != NULL) {
            VecFx32 pos = { 0, 0, 0 };

            GFL_G3DCurveGetNowTranslation(wk->work3D->emitterPaths[1], &pos);
            func_02051838(emitter, &pos);
        }
    }
}


// pokebw2's PokemonTradeDemoDown_AttachEmitterPath1
void func_ov118_021d5990(SPLEmitter *emitter) {
    func_02051a2c(emitter, func_02051774());
    func_02051a24(emitter, func_ov118_021d5950);
}

void func_ov118_021d59b0(PokemonTradeWork *wk) {
    func_ov118_021d56f0(wk);
    wk->unk854 = TRUE;
    func_02016028(wk->mcss[0], 0);
    func_02015ea0(wk->mcss[0]);
    func_02015ea0(wk->mcss[1]);
    if (wk->type == 6) {
        VecFx32 pos = { 0, FX32_CONST(-23), FX32_CONST(10) };

        func_02015c48(wk->mcss[0], &pos);
    }
    func_ov119_021e71f0(wk, 0x11);
    func_ov119_021e7208(wk, 0);
    func_ov118_021d54f0(wk, &wk->unk11ED, &wk->unk11EF, wk->partnerInfo, TRUE);
    func_ov118_021d54f0(wk, &wk->unk11EE, &wk->unk11F0, wk->myInfo, FALSE);
    func_ov119_021d8e84(wk, func_ov118_021d5a60);
}

void func_ov118_021d5a60(PokemonTradeWork *wk) {
    func_ov119_021dc720(wk);
    func_0204a5ac();
    func_ov119_021e3e40(wk);
    wk->clactUnit = func_0204b100(3, 0, wk->heapId);
    wk->work3D = GFL_HeapAllocate(wk->heapId, sizeof(PokemonTrade3DWork), TRUE, "pokemontrade_demo_down.c", 168);
    wk->work3D->heapId = wk->heapId;
    func_ov119_021e5284(wk, 0);
    reg_G2S_DB_BLDCNT = 0;
    reg_G2_BLDCNT = 0;
    func_ov119_021e3208(wk, 5);
    func_ov119_021e36c8(wk, 1, FALSE, func_ov119_021d8fa4(wk, 1), TRUE);
    func_ov119_021e36c8(wk, 2, FALSE, func_ov119_021d8fa4(wk, 0), TRUE);
    func_ov119_021e36c8(wk, 3, TRUE, func_ov119_021d8fa4(wk, 1), TRUE);
    func_02015ea0(wk->mcss[0]);
    func_02015ea0(wk->mcss[1]);
    func_02015ea0(wk->mcss[2]);
    func_02015ea0(wk->mcss[3]);
    func_ov119_021e2f34(wk->work3D);
    func_ov119_021e2d70(wk->work3D, 3);
    func_ov119_021e2fb0(wk->work3D);
    func_ov118_021d56e4(wk, DOWN_START_FRAME);
    func_ov119_021dfa60(wk->timer);
    func_ov119_021d8e84(wk, func_ov118_021d5b74);
    GFL_BGSysSetEnabledBGsB(0x18);
}



void func_ov118_021d5b74(PokemonTradeWork *wk) {
    func_ov119_021dfa60(wk->timer);
    func_ov119_021e3004(wk->work3D);
    func_ov119_021d8e84(wk, func_ov118_021d5b98);
}

void func_ov118_021d5b98(PokemonTradeWork *wk) {
    func_ov119_021dfa60(wk->timer);
    func_ov119_021d8e84(wk, func_ov118_021d5bb8);
}

static void func_ov118_021d5bb8(PokemonTradeWork *wk) {
    VecFx32 pos1;
    VecFx32 pos2;
    VecFx32 pos3;
    VecFx32 pos4;
    VecFx32 pos5;
    VecFx32 pos6;
    VecFx32 pos7;
    VecFx32 pos8;

    func_ov119_021dfa60(wk->timer);
    if (wk->timer == func_ov118_021d56e0(1)) {
        func_02015f50(wk->mcss[0], 8, 8, 0, 0xffff);
    }
    if (wk->timer == func_ov118_021d56e0(2)) {
        func_02015f50(wk->mcss[0], 16, 16, 0, 0xffff);
    }
    if (wk->timer == func_ov118_021d56e0(3)) {
        func_ov118_021d5628(wk);
        func_ov118_021d55ac(wk);
    }
    if (wk->timer == func_ov118_021d56e0(4)) {
        func_ov119_021e71f0(wk, 0x11);
        func_ov119_021e7208(wk, 0x18);
    }
    if (wk->timer == func_ov118_021d56e0(4)) {
        func_02015ea0(wk->mcss[0]);
    }
    if (wk->timer == func_ov118_021d56e0(0x356)) {
        func_020515cc(wk->work3D->systems[0], 10, NULL, wk);
    }
    if (wk->timer == func_ov118_021d56e0(0x65)) {
        func_020515cc(wk->work3D->systems[1], 1, NULL, wk);
    }
    if (wk->timer == func_ov118_021d56e0(0x9a)) {
        func_020515cc(wk->work3D->systems[2], 2, NULL, wk);
    }
    if (wk->timer == func_ov118_021d56e0(0x356)) {
        func_020515cc(wk->work3D->systems[8], 2, NULL, wk);
    }
    if (wk->timer == func_ov118_021d56e0(0xe)) {
        func_020515cc(wk->work3D->systems[3], 3, NULL, wk);
    }
    if (wk->timer == func_ov118_021d56e0(0x483)) {
        func_020515cc(wk->work3D->systems[7], 3, NULL, wk);
    }
    if (wk->timer == func_ov118_021d56e0(0x9a)) {
        func_020515cc(wk->work3D->systems[4], 4, func_ov118_021d5930, wk);
    }
    if (wk->timer == func_ov118_021d56e0(0x152)) {
        func_020515cc(wk->work3D->systems[5], 5, NULL, wk);
    }
    if (wk->timer == func_ov118_021d56e0(0x2bc)) {
        func_020515cc(wk->work3D->systems[6], 6, func_ov118_021d5990, wk);
    }
    if (wk->timer == func_ov118_021d56e0(0x47f)) {
        func_ov118_021d5628(wk);
        func_ov118_021d55ac(wk);
    }
    if (wk->timer == func_ov118_021d56e0(0x10a)) {
        GFL_WipeSet(0, 0, 0, 0x7fff, func_ov118_021d56e0(20), TRUE, wk->heapId);
    }
    if (wk->timer == func_ov118_021d56e0(0x12d)) {
        GFL_WipeSet(0, 1, 1, 0x7fff, func_ov118_021d56e0(25), TRUE, wk->heapId);
    }
    if (wk->timer == func_ov118_021d56e0(0x14a)) {
        func_02015eb0(wk->mcss[2]);
        pos1.x = FX32_CONST(-50);
        pos1.y = FX32_CONST(-150);
        pos1.z = FX32_CONST(10);
        func_02015c48(wk->mcss[2], &pos1);
        pos1.x = FX32_CONST(-50);
        pos1.y = FX32_CONST(-55);
        pos1.z = FX32_CONST(10);
        func_ov119_021dfd24(wk->mcssMoves[0]);
        wk->mcssMoves[0] = func_ov119_021dfb84(wk->mcss[2], func_ov118_021d56e0(15), &pos1, wk->heapId);
        func_02015e04(wk->mcss[2]);
        func_02015c94(wk->mcss[2], &pos1);
        pos1.x *= 1.2;
        pos1.y *= 1.2;
        func_02015cac(wk->mcss[2], &pos1);
    }
    if (wk->timer == func_ov118_021d56e0(0x14b)) {
        func_02015eb0(wk->mcss[3]);
        pos2.x = FX32_CONST(60);
        pos2.y = FX32_CONST(80);
        pos2.z = FX32_CONST(-10);
        func_02015c48(wk->mcss[3], &pos2);
        pos2.y = FX32_CONST(5);
        pos2.x = FX32_CONST(60);
        pos2.z = FX32_CONST(-10);
        func_ov119_021dfd24(wk->mcssMoves[1]);
        wk->mcssMoves[1] = func_ov119_021dfb84(wk->mcss[3], func_ov118_021d56e0(15), &pos2, wk->heapId);
        func_02015e04(wk->mcss[3]);
        func_02015c94(wk->mcss[3], &pos2);
        pos2.x *= 1.2;
        pos2.y *= 1.2;
        func_02015cac(wk->mcss[3], &pos2);
    }
    if (wk->timer == func_ov118_021d56e0(0x154)) {
        pos3.x = FX32_CONST(-50);
        pos3.y = FX32_CONST(-55);
        pos3.z = FX32_CONST(10);
        func_ov119_021dfb4c(wk->mcssMoves[0], func_ov118_021d56e0(28), &pos3);
        pos3.x = FX32_CONST(60);
        pos3.y = FX32_CONST(5);
        pos3.z = FX32_CONST(-10);
        func_ov119_021dfb4c(wk->mcssMoves[1], func_ov118_021d56e0(28), &pos3);
    }
    if (wk->timer == func_ov118_021d56e0(0x170)) {
        func_ov118_021d572c(wk, 0);
        func_ov118_021d572c(wk, 1);
        pos4.x = FX32_CONST(-150);
        pos4.y = FX32_CONST(-55);
        pos4.z = 0;
        func_ov119_021dfd24(wk->mcssMoves[0]);
        wk->mcssMoves[0] = func_ov119_021dfb84(wk->mcss[2], func_ov118_021d56e0(46), &pos4, wk->heapId);
        pos4.x = FX32_CONST(150);
        pos4.y = FX32_CONST(5);
        pos4.z = 0;
        func_ov119_021dfd24(wk->mcssMoves[1]);
        wk->mcssMoves[1] = func_ov119_021dfb84(wk->mcss[3], func_ov118_021d56e0(46), &pos4, wk->heapId);
        wk->mcssMoves[0]->bounce = 1;
        wk->mcssMoves[1]->bounce = 1;
    }
    if (wk->timer == func_ov118_021d56e0(0x1b2)) {
        GFL_HeapFree(wk->mcssMoves[0]);
        wk->mcssMoves[0] = NULL;
        func_02015eb0(wk->mcss[0]);
        func_02015f50(wk->mcss[0], 0, 0, 0, 0x7fff);
        pos5.x = FX32_CONST(-150);
        pos5.y = FX32_CONST(5);
        pos5.z = FX32_CONST(-10);
        func_02015c48(wk->mcss[0], &pos5);
        pos5.x = FX32_CONST(60);
        pos5.y = FX32_CONST(5);
        pos5.z = FX32_CONST(-10);
        func_ov119_021dfd24(wk->mcssMoves[0]);
        wk->mcssMoves[0] = func_ov119_021dfb84(wk->mcss[0], func_ov118_021d56e0(103), &pos5, wk->heapId);
        func_02015c94(wk->mcss[0], &pos5);
        pos5.x *= 1.2;
        pos5.y *= 1.2;
        func_02015cac(wk->mcss[0], &pos5);
        GFL_HeapFree(wk->mcssMoves[1]);
        wk->mcssMoves[1] = NULL;
        func_02015eb0(wk->mcss[1]);
        pos6.x = FX32_CONST(150);
        pos6.y = FX32_CONST(-55);
        pos6.z = FX32_CONST(10);
        func_02015c48(wk->mcss[1], &pos6);
        pos6.x = FX32_CONST(-45);
        pos6.y = FX32_CONST(-55);
        pos6.z = FX32_CONST(10);
        func_ov119_021dfd24(wk->mcssMoves[1]);
        wk->mcssMoves[1] = func_ov119_021dfb84(wk->mcss[1], func_ov118_021d56e0(103), &pos6, wk->heapId);
        func_02015c94(wk->mcss[1], &pos6);
        pos6.x *= 1.2;
        pos6.y *= 1.2;
        func_02015cac(wk->mcss[1], &pos6);
        wk->mcssMoves[0]->bounce = 1;
        wk->mcssMoves[1]->bounce = 1;
    }
    if (wk->timer == func_ov118_021d56e0(0x1f8)) {
        func_ov119_021dfb4c(wk->mcssMoves[0], func_ov118_021d56e0(128), &wk->mcssMoves[0]->end);
        func_ov119_021dfb4c(wk->mcssMoves[1], func_ov118_021d56e0(128), &wk->mcssMoves[1]->end);
    }
    if (wk->timer == func_ov118_021d56e0(0x27d)) {
        func_ov118_021d572c(wk, 0);
        func_ov118_021d572c(wk, 1);
        pos7.x = FX32_CONST(60);
        pos7.y = FX32_CONST(100);
        pos7.z = FX32_CONST(-10);
        func_ov119_021dfd24(wk->mcssMoves[0]);
        wk->mcssMoves[0] = func_ov119_021dfb84(wk->mcss[0], func_ov118_021d56e0(7), &pos7, wk->heapId);
        pos7.x = FX32_CONST(-45);
        pos7.y = FX32_CONST(-160);
        pos7.z = FX32_CONST(10);
        func_ov119_021dfd24(wk->mcssMoves[1]);
        wk->mcssMoves[1] = func_ov119_021dfb84(wk->mcss[1], func_ov118_021d56e0(7), &pos7, wk->heapId);
    }
    func_ov118_021d56e0(0x48b);
    if (wk->timer == func_ov118_021d56e0(0x48e)) {
        if (wk->unk11EE == 0) {
            func_ov118_021d5888(wk, 3);
            func_ov118_021d5748(wk, 3);
            func_02015f50(wk->mcss[3], 16, 16, 0, 0x7fff);
        } else {
            func_ov118_021d5888(wk, 3);
            func_02015ea0(wk->mcss[3]);
            func_ov118_021d5888(wk, 1);
            func_ov118_021d5748(wk, 3);
            func_02015f50(wk->mcss[1], 16, 16, 0, 0x7fff);
        }
    }
    if (wk->timer == func_ov118_021d56e0(0x48f)) {
        func_02015f50(wk->mcss[3], 8, 8, 0, 0x7fff);
    }
    if (wk->timer == func_ov118_021d56e0(0x490)) {
        func_02015f50(wk->mcss[3], 0, 0, 0, 0x7fff);
    }
    if (func_ov118_021d56e0(0x48e) < wk->timer && wk->unk11EE != 0 && wk->mcssMoves[3] != NULL
        && wk->mcssMoves[3]->frame == 9) {
        func_02015c30(wk->mcss[1], &pos8);
        func_02015c48(wk->mcss[3], &pos8);
        func_02015eb0(wk->mcss[3]);
        func_02015ea0(wk->mcss[1]);
        wk->mcssMoves[3]->mcss = wk->mcss[3];
    }
    if (wk->timer == func_ov118_021d56e0(0x4a0)) {
        func_02015e4c(wk->mcss[3]);
    }
    if (wk->timer == func_ov118_021d56e0(0x28b)) {
        GFL_WipeSet(0, 0, 0, 0x7fff, func_ov118_021d56e0(10), TRUE, wk->heapId);
    }
    if (wk->timer == func_ov118_021d56e0(0x2b5)) {
        GFL_WipeSet(0, 1, 1, 0x7fff, func_ov118_021d56e0(25), TRUE, wk->heapId);
    }
    if (wk->timer == func_ov118_021d56e0(0x380)) {
        GFL_WipeSet(0, 0, 0, 0x7fff, func_ov118_021d56e0(30), TRUE, wk->heapId);
    }
    if (wk->timer == func_ov118_021d56e0(0x3ca)) {
        GFL_WipeSet(0, 1, 1, 0x7fff, func_ov118_021d56e0(30), TRUE, wk->heapId);
    }
    if (wk->timer == func_ov118_021d56e0(0x4c1)) {
        func_ov118_021d572c(wk, 0);
        func_ov118_021d572c(wk, 1);
        func_ov119_021e3230(wk);
        func_ov119_021d8e84(wk, func_ov118_021d56fc);
    }
    func_ov119_021dfc68(wk->mcssMoves[0], wk);
    func_ov119_021dfc68(wk->mcssMoves[1], wk);
    func_ov119_021dfc68(wk->mcssMoves[2], wk);
    func_ov119_021dfc68(wk->mcssMoves[3], wk);
}



