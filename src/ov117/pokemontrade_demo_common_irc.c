// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov192/pokemontrade_demo_common_irc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/pokemon_trade_local.h"
#include "app/pokemontrade_ircdemo.h"
#include "constants/pokemon.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/ioreg.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "save/player_info.h"
#include "system/mcss.h"
#include "system/wordset.h"
#include "text/system/ability_handlers_3.h"

// pokemontrade_demo_common.c as the ROM's string has it, the part of the IR trade's demo that its steps share. Overlay
// 193's trade demos are built from the same template; this overlay's copy is the file's first half, and
// pokemontrade_ircdemo.c is the second. Names are descriptive guesses. Overlay 193 also embeds
// "pokemontrade_demo_common.c", for a file of other code (src/ov193/pokemontrade_demo_common.c), and the build can't
// link two files of one name, so this one is named _irc after its overlay's other file

// pokebw2's sIrcDemoPosSecond
static const VecFx32 data_ov117_021d51e8 = { 0, -0x17000, -0xa000 };
// pokebw2's sIrcDemoPosEnd
static const VecFx32 data_ov117_021d51dc = { 0, -0x17000, 0xa000 };
// pokebw2's sIrcDemoPosFirst
static const VecFx32 data_ov117_021d51f4 = { 0, -0x17000, 0xa000 };

void func_ov117_021d4240(TradeFade *fade);
void func_ov117_021d4290(PokemonTradeWork *wk, u8 *ownedByPlayer, u8 *noBounce, PlayerInfo *player, BOOL partner);
void func_ov117_021d4434(PokemonTradeWork *wk);
void func_ov117_021d4490(PokemonTradeWork *wk);
void func_ov117_021d449c(PokemonTradeWork *wk);
void func_ov117_021d4520(PokemonTradeWork *wk);
void func_ov117_021d4544(PokemonTradeWork *wk);
void func_ov117_021d4664(PokemonTradeWork *wk);
void func_ov117_021d4820(PokemonTradeWork *wk);
void func_ov117_021d48f0(PokemonTradeWork *wk);

// The hops of a Pokémon's sprite, as offsets from its place: one hop of 18 frames, the same three frames later, or
// two of 40
// pokebw2's sIrcDemoHopPath
static VecFx32 data_ov117_021d5220[18] = {
#include "asset/pokemontrade_demo_common_irc_data_ov117_021d5220.inc"
};

// pokebw2's sIrcDemoDelayedHopPath
static VecFx32 data_ov117_021d52f8[18] = {
#include "asset/pokemontrade_demo_common_irc_data_ov117_021d52f8.inc"
};

// pokebw2's sIrcDemoLongHopPath
static VecFx32 data_ov117_021d53d0[40] = {
#include "asset/pokemontrade_demo_common_irc_data_ov117_021d53d0.inc"
};

// The path that the sprites follow in the other scenes, which they fill
// pokebw2's sIrcDemoScenePath
static VecFx32 data_ov117_021d5600[20];

// pokebw2's PokemonTradeIrcDemo_UpdateFade
void func_ov117_021d4240(TradeFade *fade) {
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

void func_ov117_021d4290(PokemonTradeWork *wk, u8 *ownedByPlayer, u8 *noBounce, PlayerInfo *player, BOOL partner) {
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


void func_ov117_021d434c(PokemonTradeWork *wk) {
    ClActorSetup setup;

    if (wk->work3D->actor == NULL) {
        setup.x = 128;
        setup.y = 99;
        setup.sequence = 0;
        setup.priority = 0;
        setup.bgPriority = 0;
        wk->work3D->actor = func_0204b294(wk->clactUnit, wk->objRes[TRADE_OBJRES_CHAR_DEMO],
            wk->objRes[TRADE_OBJRES_PLTT_DEMO], wk->objRes[TRADE_OBJRES_CELL_DEMO], &setup, 0, wk->heapId);
        func_0204b92c(wk->work3D->actor, 1);
        func_0204b3dc(wk->work3D->actor, 1);
    } else {
        func_0204b9a0(wk->work3D->actor);
    }
}

void func_ov117_021d43c8(PokemonTradeWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0x67, wk->heapId);

    wk->objRes[TRADE_OBJRES_CHAR_DEMO] = func_0204a6c8(arc, 2, FALSE, CLACT_VRAM_MAIN, wk->heapId);
    wk->objRes[TRADE_OBJRES_PLTT_DEMO] = func_0204ac18(arc, 3, CLACT_VRAM_MAIN, 0xe0, 0, 1, wk->heapId);
    wk->objRes[TRADE_OBJRES_CELL_DEMO] = func_0204af28(arc, 1, 0, wk->heapId);
    GFL_ArcToolFree(arc);
    wk->work3D->actor = NULL;
}

void func_ov117_021d4434(PokemonTradeWork *wk) {
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




// pokebw2's PokemonTradeIrcDemo_Frames
int func_ov117_021d4480(int frame) {
    return frame;
}

void func_ov117_021d4484(PokemonTradeWork *wk, int timer) {
    wk->timer = timer;
}

void func_ov117_021d4490(PokemonTradeWork *wk) {
    wk->bgmTimer = 1;
}

void func_ov117_021d449c(PokemonTradeWork *wk) {
    func_ov119_021e3bb0(wk);
    if (wk->unk854 == 1) {
        func_ov117_021d4290(wk, &wk->unk11ED, &wk->unk11EF, wk->partnerInfo, TRUE);
        func_ov117_021d4290(wk, &wk->unk11EE, &wk->unk11F0, wk->myInfo, FALSE);
        func_ov119_021e3230(wk);
        if (func_ov119_021d8a24(wk)) {
            func_02039c68(func_020399a8(), 0x11, 8);
            func_ov119_021d8e84(wk, func_ov117_021d4520);
        } else {
            func_ov119_021d8e84(wk, func_ov117_021d4544);
        }
    }
}




// pokebw2's PokemonTradeIrcDemo_StateWaitSync
void func_ov117_021d4520(PokemonTradeWork *wk) {
    if (func_02039cb4(func_020399a8(), 0x11, 8)) {
        func_ov119_021d8e84(wk, func_ov117_021d4544);
    }
}

void func_ov117_021d4544(PokemonTradeWork *wk) {
    VecFx32 first;
    VecFx32 second;

    func_ov117_021d4484(wk, 0);
    wk->fade = GFL_HeapAllocate(wk->heapId, sizeof(TradeFade), TRUE, "pokemontrade_demo_common.c", 245);
    wk->fade->frames = func_ov117_021d4480(14);
    wk->fade->frame = 0;
    wk->fade->start = 0;
    wk->fade->end = -16;
    first = data_ov117_021d51f4;
    func_ov119_021dfd24(wk->mcssMoves[0]);
    wk->mcssMoves[0] = func_ov119_021dfb84(wk->mcss[0], func_ov117_021d4480(42), &first, wk->heapId);
    second = data_ov117_021d51e8;
    func_ov119_021dfd24(wk->mcssMoves[1]);
    wk->mcssMoves[1] = func_ov119_021dfb84(wk->mcss[1], func_ov117_021d4480(84), &second, wk->heapId);
    func_02015f2c(wk->mcss[0], (uintptr_t)wk, (MCSSAnimationCallback)func_ov119_021dfd0c, TRUE);
    func_02015f50(wk->mcss[1], 0, 16, (s8)(func_ov117_021d4480(14) / 16), 0);
    func_ov119_021e106c(wk);
    GFL_BGSysResetStdPalette(1, 0);
    GFL_BGSysResetStdPalette(5, 0);
    func_ov117_021d4484(wk, 0);
    func_ov119_021d8e84(wk, func_ov117_021d4664);
}

void func_ov117_021d4664(PokemonTradeWork *wk) {
    PartyPkm *pkm;

    func_ov117_021d4240(wk->fade);
    func_ov119_021dfc68(wk->mcssMoves[0], wk);
    func_ov119_021dfc68(wk->mcssMoves[1], wk);
    if (wk->timer > func_ov117_021d4480(42)) {
        func_ov119_021e71f0(wk, 0x11);
        func_ov119_021e7208(wk, 0);
        func_ov119_021e64b8(wk);
        func_ov119_021e4888(wk);
        func_ov119_021e2680(wk);
        func_ov117_021d4484(wk, 0);
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
                wk->mcssMoves[0] = func_ov119_021dfbb4(wk->mcss[0], 20, &pos, data_ov117_021d5600, wk->heapId);
            } else {
                wk->mcssMoves[0] = func_ov119_021dfbb4(wk->mcss[0], 18, &pos, data_ov117_021d5220, wk->heapId);
            }
            func_ov119_021e36c8(wk, 2, 0, func_ov119_021d8fa4(wk, func_ov119_021d8aa8(wk)), TRUE);
            func_02015e04(wk->mcss[2]);
            func_02015ea0(wk->mcss[2]);
        } else {
            VecFx32 pos;

            func_02015c30(wk->mcss[0], &pos);
            func_ov119_021dfd24(wk->mcssMoves[0]);
            if (wk->unk11EF != 0) {
                wk->mcssMoves[0] = func_ov119_021dfbb4(wk->mcss[0], 20, &pos, data_ov117_021d5600, wk->heapId);
            } else {
                wk->mcssMoves[0] = func_ov119_021dfbb4(wk->mcss[0], 40, &pos, data_ov117_021d53d0, wk->heapId);
            }
        }
        func_ov119_021d8e84(wk, func_ov117_021d48f0);
    }
}



void func_ov117_021d4820(PokemonTradeWork *wk) {
    func_0203d34c(FALSE, wk->heapId);
    // Black's message IDs here are one below the ability_handlers_3.h names
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
    func_ov119_021e0ff4(wk, 1, 0);
    func_ov119_021e7208(wk, 0x14);
    *(u16 *)REG_DB_BLDCNT_ADDR = 0;
}

void func_ov117_021d48f0(PokemonTradeWork *wk) {
    VecFx32 pos;

    if (wk->timer == 10) {
        func_ov117_021d4820(wk);
    }
    if (wk->unk11ED == 0 && wk->timer == 6) {
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
        func_ov119_021d8e84(wk, func_ov117_021d4bf4);
    }
    func_ov119_021dfc68(wk->mcssMoves[0], wk);
}



void func_ov117_021d49c4(PokemonTradeWork *wk) {
    func_ov117_021d4434(wk);
    func_ov119_021e3034(wk->work3D, 6);
    func_ov119_021e2f2c(wk->work3D);
    GFL_HeapFree(wk->work3D);
    wk->work3D = NULL;
    func_ov119_021d8e84(wk, func_ov119_021dfda0);
}

void func_ov117_021d49f4(MCSS *mcss, PokemonTradeWork *wk) {
    VecFx32 pos;

    func_02015fbc(mcss, 31);
    func_02015eb0(mcss);
    pos.x = 0;
    pos.y = FX32_CONST(-23);
    pos.z = FX32_CONST(10);
    func_02015c48(mcss, &pos);
    func_02015e04(mcss);
    func_02015cac(mcss, &wk->work3D->ircSpriteSizes[0]);
    func_02015f50(mcss, 16, 16, 0, 0x7fff);
}

void func_ov117_021d4a48(PokemonTradeWork *wk, int index) {
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
            wk->mcssMoves[3] = func_ov119_021dfbb4(wk->mcss[index], 20, &pos, data_ov117_021d5600, wk->heapId);
        } else {
            wk->mcssMoves[3] = func_ov119_021dfbb4(wk->mcss[index], 40, &pos, data_ov117_021d53d0, wk->heapId);
        }
    } else {
        u32 pair[4] = { 2, 3, 0, 1 };
        MCSS *target = wk->mcss[pair[index]];

        func_02015c30(wk->mcss[index], &pos);
        func_ov119_021dfd24(wk->mcssMoves[3]);
        if (wk->unk11F0 != 0) {
            wk->mcssMoves[3] = func_ov119_021dfbb4(target, 20, &pos, data_ov117_021d5600, wk->heapId);
        } else {
            wk->mcssMoves[3] = func_ov119_021dfbb4(target, 18, &pos, data_ov117_021d52f8, wk->heapId);
        }
    }
}


void func_ov117_021d4b88(PokemonTradeWork *wk) {
    VecFx32 pos;

    func_ov117_021d4490(wk);
    wk->unk854 = 1;
    func_02016028(wk->mcss[0], 0);
    func_02015e04(wk->mcss[0]);
    if (wk->type == 4) {
        pos = data_ov117_021d51dc;
        func_02015c48(wk->mcss[0], &pos);
    }
    func_ov117_021d4484(wk, 0);
    func_ov119_021d8e84(wk, func_ov117_021d449c);
}




