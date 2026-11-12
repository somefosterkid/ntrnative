// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov193/pokemontrade_demo_common.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/pokemontrade_demo_common.h"
#include "app/pokemontrade_demo_up.h"
#include "app/pokemon_trade_local.h"
#include "constants/pokemon.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/particle.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/ioreg.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "system/mcss.h"
#include "system/wordset.h"
#include "text/system/ability_handlers_3.h"

// A guessed name: the part of the trade demo's scenes that the three overlay-193 files share

// pokebw2's sTradeDemoCommonPosFirst
static const VecFx32 data_ov118_021d7c84 = { 0, -0x17000, 0xa000 };
// pokebw2's sTradeDemoCommonPosEnd
static const VecFx32 data_ov118_021d7c90 = { 0, -0x17000, 0xa000 };
// pokebw2's sTradeDemoCommonPosSecond
static const VecFx32 data_ov118_021d7c9c = { 0, -0x17000, -0xa000 };

void func_ov118_021d4240(TradeFade *fade);
void func_ov118_021d4290(PokemonTradeWork *wk, u8 *ownedByPlayer, u8 *noBounce, PlayerInfo *player, BOOL partner);
void func_ov118_021d4434(PokemonTradeWork *wk);
void func_ov118_021d4490(PokemonTradeWork *wk);
void func_ov118_021d449c(PokemonTradeWork *wk);
void func_ov118_021d4520(PokemonTradeWork *wk);
void func_ov118_021d4544(PokemonTradeWork *wk);
void func_ov118_021d4664(PokemonTradeWork *wk);
void func_ov118_021d4820(PokemonTradeWork *wk);
void func_ov118_021d48f0(PokemonTradeWork *wk);
void func_ov118_021d49f4(SPLEmitter *emitter, u32 type);
void func_ov118_021d4a54(SPLEmitter *emitter, u32 type);
void func_ov119_021e2f2c(PokemonTrade3DWork *work);
void func_ov119_021dfda0(PokemonTradeWork *wk);
u32 getIDAsUInt(PlayerInfo *info);
void func_0204c108(ClActor *actor);
void func_0204bcd0(u32 palette);
void func_0204be64(u32 cellAnims);
void *func_02051a34(SPLEmitter *emitter);
void func_02051838(SPLEmitter *emitter, const VecFx32 *pos);
BOOL GFL_G3DCurveGetNowTranslation(G3DCurve *curve, VecFx32 *translation);

// The hops of a Pokémon's sprite, as offsets from its place: one hop of 18 frames, or two of 40
// pokebw2's sTradeDemoCommonHopPath
static VecFx32 data_ov118_021d7d00[18] = {
#include "asset/pokemontrade_demo_common_data_ov118_021d7d00.inc"
};

// pokebw2's sTradeDemoCommonLongHopPath
static VecFx32 data_ov118_021d7dd8[40] = {
#include "asset/pokemontrade_demo_common_data_ov118_021d7dd8.inc"
};

// The path that the sprites follow in the other scenes, which they fill
// pokebw2's sTradeDemoCommonScenePath
static VecFx32 data_ov118_021d86a0[20];

// pokebw2's PokemonTradeDemoCommon_UpdateFade
void func_ov118_021d4240(TradeFade *fade) {
    if (fade != NULL && fade->frames >= fade->frame) {
        fx32 step;
        int brightness;

        fade->frame++;
        step = ((fade->end - fade->start) << FX32_SHIFT) / fade->frames;
        brightness = step * fade->frame / FX32_ONE;
        G2_SetBlendBrightness(0x1e, brightness);
        G2S_SetBlendBrightness(0x1f, brightness);
    }
}

void func_ov118_021d4290(PokemonTradeWork *wk, u8 *ownedByPlayer, u8 *noBounce, PlayerInfo *player, BOOL partner) {
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


void func_ov118_021d434c(PokemonTradeWork *wk) {
    if (wk->work3D->actor == NULL) {
        ClActorSetup setup;

        setup.x = 0x80;
        setup.y = 0x63;
        setup.sequence = 0;
        setup.priority = 0;
        setup.bgPriority = 0;
        wk->work3D->actor = func_0204b294(wk->clactUnit, wk->objRes[TRADE_OBJRES_CHAR_DEMO],
                                          wk->objRes[TRADE_OBJRES_PLTT_DEMO], wk->objRes[TRADE_OBJRES_CELL_DEMO],
                                          &setup, 0, wk->heapId);
        func_0204b92c(wk->work3D->actor, TRUE);
        func_0204b3dc(wk->work3D->actor, TRUE);
    } else {
        func_0204b9a0(wk->work3D->actor);
    }
}


void func_ov118_021d43c8(PokemonTradeWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0x67, wk->heapId);

    wk->objRes[TRADE_OBJRES_CHAR_DEMO] = func_0204a6c8(arc, 2, 0, 0, wk->heapId);
    wk->objRes[TRADE_OBJRES_PLTT_DEMO] = func_0204ac18(arc, 3, 0, 0xe0, 0, 1, wk->heapId);
    wk->objRes[TRADE_OBJRES_CELL_DEMO] = func_0204af28(arc, 1, 0, wk->heapId);
    GFL_ArcToolFree(arc);
    wk->work3D->actor = NULL;
}


void func_ov118_021d4434(PokemonTradeWork *wk) {
    if (wk->work3D->actor != NULL) {
        func_0204b3b4(wk->work3D->actor);
    }
    if (wk->objRes[TRADE_OBJRES_CHAR_DEMO] != 0) {
        func_0204a8d4(wk->objRes[TRADE_OBJRES_CHAR_DEMO]);
        func_0204ada4(wk->objRes[TRADE_OBJRES_PLTT_DEMO]);
        func_0204afd8(wk->objRes[TRADE_OBJRES_CELL_DEMO]);
    }
    wk->objRes[TRADE_OBJRES_CHAR_DEMO] = 0;
    wk->objRes[TRADE_OBJRES_PLTT_DEMO] = 0;
    wk->objRes[TRADE_OBJRES_CELL_DEMO] = 0;
    wk->work3D->actor = NULL;
}


// The side of the trade whose Pokémon is shown as the partner's or the player's: a nego trade swaps them
static inline int PokemonTradeDemoCommon_GetSide(PokemonTradeWork *wk, BOOL partner) {
    if (partner) {
        if (func_ov119_021d8aa8(wk)) {
            return 1;
        }
        return 0;
    }
    if (func_ov119_021d8aa8(wk)) {
        return 0;
    }
    return 1;
}

// pokebw2's PokemonTradeDemo_Frames
int func_ov118_021d4480(int frame) {
    return frame;
}

void func_ov118_021d4484(PokemonTradeWork *wk, int timer) {
    wk->timer = timer;
}

void func_ov118_021d4490(PokemonTradeWork *wk) {
    wk->bgmTimer = 1;
}

void func_ov118_021d449c(PokemonTradeWork *wk) {
    func_ov119_021e3bb0(wk);
    if (wk->unk854 == 1) {
        func_ov118_021d4290(wk, &wk->unk11ED, &wk->unk11EF, wk->partnerInfo, TRUE);
        func_ov118_021d4290(wk, &wk->unk11EE, &wk->unk11F0, wk->myInfo, FALSE);
        func_ov119_021e3230(wk);
        if (func_ov119_021d8a24(wk)) {
            NetHandle *handle = func_020399a8();

            func_02039c68(handle, 0x11, 8);
            func_ov119_021d8e84(wk, func_ov118_021d4520);
        } else {
            func_ov119_021d8e84(wk, func_ov118_021d4544);
        }
    }
}




// pokebw2's PokemonTradeDemoCommon_StateWaitSync
void func_ov118_021d4520(PokemonTradeWork *wk) {
    if (func_02039cb4(func_020399a8(), 0x11, 8)) {
        func_ov119_021d8e84(wk, func_ov118_021d4544);
    }
}

void func_ov118_021d4544(PokemonTradeWork *wk) {
    VecFx32 first;
    VecFx32 second;

    func_ov118_021d4484(wk, 0);
    wk->fade = GFL_HeapAllocate(wk->heapId, sizeof(TradeFade), TRUE, "pokemontrade_demo_common.c", 245);
    wk->fade->frames = func_ov118_021d4480(14);
    wk->fade->frame = 0;
    wk->fade->start = 0;
    wk->fade->end = -16;
    first = data_ov118_021d7c84;
    func_ov119_021dfd24(wk->mcssMoves[0]);
    wk->mcssMoves[0] = func_ov119_021dfb84(wk->mcss[0], func_ov118_021d4480(42), &first, wk->heapId);
    second = data_ov118_021d7c9c;
    func_ov119_021dfd24(wk->mcssMoves[1]);
    wk->mcssMoves[1] = func_ov119_021dfb84(wk->mcss[1], func_ov118_021d4480(84), &second, wk->heapId);
    func_02015f2c(wk->mcss[0], (uintptr_t)wk, (MCSSAnimationCallback)func_ov119_021dfd0c, TRUE);
    func_02015f50(wk->mcss[1], 0, 16, (s8)(func_ov118_021d4480(14) / 16), 0);
    func_ov119_021e106c(wk);
    GFL_BGSysResetStdPalette(1, 0);
    GFL_BGSysResetStdPalette(5, 0);
    func_ov118_021d4484(wk, 0);
    func_ov119_021d8e84(wk, func_ov118_021d4664);
}

void func_ov118_021d4664(PokemonTradeWork *wk) {
    PartyPkm *pkm;

    func_ov118_021d4240(wk->fade);
    func_ov119_021dfc68(wk->mcssMoves[0], wk);
    func_ov119_021dfc68(wk->mcssMoves[1], wk);
    if (wk->timer > func_ov118_021d4480(42)) {
        func_ov119_021e71f0(wk, 0x11);
        func_ov119_021e7208(wk, 0);
        func_ov119_021e64b8(wk);
        func_ov119_021e4888(wk);
        func_ov119_021e2680(wk);
        func_ov118_021d4484(wk, 0);
        pkm = func_ov119_021d8fa4(wk, func_ov119_021d8aa8(wk));
        PokeParty_GetParam(pkm, PKM_PARAM_ID, NULL);
        if (PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL) == 0) {
            u32 species = PokeParty_GetParam(pkm, PKM_PARAM_LEGAL_SPECIES, NULL);
            u32 form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
            PokeVoiceChatterInfo info;

            PokeVoice_CreateChatterInfo(&info);
            PokeVoice_Play(species, form, 0x40, 0, 0, 0, 0, &info);
        }
        if (wk->mcssMoves[0] != NULL) {
            GFL_HeapFree(wk->mcssMoves[0]);
            wk->mcssMoves[0] = NULL;
        }
        if (wk->unk11ED == 0) {
            VecFx32 pos;

            func_02015c30(wk->mcss[0], &pos);
            func_ov119_021dfd24(wk->mcssMoves[0]);
            if (wk->unk11EF != 0) {
                wk->mcssMoves[0] = func_ov119_021dfbb4(wk->mcss[0], 20, &pos, data_ov118_021d86a0, wk->heapId);
            } else {
                wk->mcssMoves[0] = func_ov119_021dfbb4(wk->mcss[0], 18, &pos, data_ov118_021d7d00, wk->heapId);
            }
            func_ov119_021e36c8(wk, 2, 0, func_ov119_021d8fa4(wk, func_ov119_021d8aa8(wk)), TRUE);
            func_02015e04(wk->mcss[2]);
            func_02015ea0(wk->mcss[2]);
        } else {
            VecFx32 pos;

            func_02015c30(wk->mcss[0], &pos);
            func_ov119_021dfd24(wk->mcssMoves[0]);
            if (wk->unk11EF != 0) {
                wk->mcssMoves[0] = func_ov119_021dfbb4(wk->mcss[0], 20, &pos, data_ov118_021d86a0, wk->heapId);
            } else {
                wk->mcssMoves[0] = func_ov119_021dfbb4(wk->mcss[0], 40, &pos, data_ov118_021d7dd8, wk->heapId);
            }
        }
        func_ov119_021d8e84(wk, func_ov118_021d48f0);
    }
}



void func_ov118_021d4820(PokemonTradeWork *wk) {
    func_0203d34c(FALSE, wk->heapId);
    if (wk->type == 6) {
        GFL_MsgDataLoadStrbuf(wk->msgData, 0x98, wk->strbufTemplate);
    } else if (wk->type == 8) {
        GFL_MsgDataLoadStrbuf(wk->msgData, 0x9b, wk->strbufTemplate);
    } else if (wk->unk11ED != 0) {
        GFL_MsgDataLoadStrbuf(wk->msgData, 0x82, wk->strbufTemplate);
    } else {
        GFL_MsgDataLoadStrbuf(wk->msgData, 0x81, wk->strbufTemplate);
    }
    loadPokemonNicknameToStrbuf(wk->wordSet, 0, func_ov119_021d8fa4(wk, func_ov119_021d8aa8(wk)));
    copyVarForText(wk->wordSet, 1, wk->partnerInfo);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, wk->strbufTemplate);
    GFL_BGSysFillScrArea(4, 0, 0, 0, 32, 24, 0x11);
    func_ov119_021e0ff4(wk, TRUE, FALSE);
    func_ov119_021e7208(wk, 0x14);
    *(u16 *)REG_DB_BLDCNT_ADDR = 0;
}

void func_ov118_021d48f0(PokemonTradeWork *wk) {
    if (wk->timer == 10) {
        func_ov118_021d4820(wk);
    }
    if (wk->unk11ED == 0 && wk->timer == 6) {
        VecFx32 pos;

        func_02015c30(wk->mcss[0], &pos);
        func_02015c48(wk->mcss[2], &pos);
        func_02015eb0(wk->mcss[2]);
        func_02015ea0(wk->mcss[0]);
        func_02015e4c(wk->mcss[2]);
        wk->mcssMoves[0]->mcss = wk->mcss[2];
    }
    if (wk->timer > 200 && func_ov119_021e12ac(wk)) {
        func_ov119_021e1098(wk);
        if (wk->unk11ED == 0) {
            func_ov119_021e36d8(wk, 0);
            wk->mcss[0] = wk->mcss[2];
            wk->mcss[2] = NULL;
        }
        if (wk->mcssMoves[0] != NULL) {
            GFL_HeapFree(wk->mcssMoves[0]);
            wk->mcssMoves[0] = NULL;
        }
        func_ov119_021d8e84(wk, func_ov118_021d4b14);
    }
    func_ov119_021dfc68(wk->mcssMoves[0], wk);
}



void func_ov118_021d49c4(PokemonTradeWork *wk) {
    func_ov118_021d4434(wk);
    func_ov119_021e3034(wk->work3D, 6);
    func_ov119_021e2f2c(wk->work3D);
    GFL_HeapFree(wk->work3D);
    wk->work3D = NULL;
    func_ov119_021d8e84(wk, func_ov119_021dfda0);
}

void func_ov118_021d49f4(SPLEmitter *emitter, u32 type) {
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



// pokebw2's PokemonTradeDemo_InitEmitter0
void func_ov118_021d4a34(SPLEmitter *emitter) {
    func_02051a2c(emitter, func_02051774());
    func_02051a24(emitter, func_ov118_021d49f4);
}

void func_ov118_021d4a54(SPLEmitter *emitter, u32 type) {
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


// pokebw2's PokemonTradeDemo_InitEmitter1
void func_ov118_021d4a94(SPLEmitter *emitter) {
    func_02051a2c(emitter, func_02051774());
    func_02051a24(emitter, func_ov118_021d4a54);
}

void func_ov118_021d4ab4(PokemonTradeWork *wk) {
    func_ov118_021d4490(wk);
    wk->unk854 = 1;
    func_02016028(wk->mcss[0], 0);
    if (wk->type == 6) {
        VecFx32 pos = data_ov118_021d7c90;

        func_02015c48(wk->mcss[0], &pos);
    }
    func_ov118_021d4484(wk, 0);
    func_ov119_021d8e84(wk, func_ov118_021d449c);
}


