// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov214/worldtrade_demo.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/ui/ui_scene.h"
#include "app/pokemon_trade.h"
#include "constants/pokemon.h"
#include "demo/shinka_demo.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "nitro/gx.h"
#include "pml/evolution.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "save/player_info.h"
#include "save/worldtrade_data.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "worldtrade_local.h"
#include "app/worldtrade.h"
#include "save/battle_rec.h"
#include "dpw/dpw_tr.h"

// The Global Trade Station's trade demo, and the evolution demo of a Pokémon that evolves by the trade. The names are
// ours, guessed

// How the demo was reached, the screen's mode
#define DEMO_MODE_UPLOAD 7
#define DEMO_MODE_DOWNLOAD 8
#define DEMO_MODE_EXCHANGE 9
#define DEMO_MODE_DOWNLOAD_EX 10

// The upload screen's mode that saves the evolved Pokémon
#define UPLOAD_MODE_POKEMON_EVO_SAVE 12

PartyPkm *func_ov135_021f7338(WorldTradeWork *wk, int mode);
void func_ov135_021f7374(WorldTradeWork *wk);
PlayerInfo *func_ov135_021f72c4(Dpw_Tr_Data *dtd);
PartyPkm *func_ov135_02202608(HeapID heapId);
extern const GameProcFunctions data_ov119_021e75fc;
extern const GameProcFunctions data_ov119_021e75b4;
extern const GameProcFunctions data_ov119_021e75cc;

// pokebw2's WorldTrade_MakePartnerStatus


int func_ov135_021f6e58(WorldTradeWork *wk, int seq) {
    PokemonTradeGtsParam *param;
    const GameProcFunctions *procs;

    func_ov135_021f50ec(wk);
    wk->subProcParam = GFL_HeapAllocate(HEAPID_WORLDTRADE, sizeof(PokemonTradeGtsParam), FALSE, "worldtrade_demo.c", 94);
    MI_CpuFill8(wk->subProcParam, 0, sizeof(PokemonTradeGtsParam));
    param = wk->subProcParam;
    wk->sentPokemon = func_ov135_02202608(HEAPID_WORLDTRADE);
    param->gameData = GSYS_GetGameData(wk->param->gsys);
    param->myStatus = wk->param->mystatus;

    switch (wk->subProcessMode) {
    case DEMO_MODE_UPLOAD:
        param->recvPkm = param->sendPkm = func_0200adb4(wk->param->worldtrade_data);
        param->partnerStatus = wk->unkD8 = func_ov135_021f72c4(&wk->uploadData);
        procs = &data_ov119_021e75fc;
        break;
    case DEMO_MODE_DOWNLOAD:
        if (wk->uploadData.isTrade != 0) {
            param->recvPkm = (PartyPkm *)&wk->uploadData;
        } else {
            param->recvPkm = func_0200adb4(wk->param->worldtrade_data);
        }
        param->sendPkm = param->recvPkm;
        param->partnerStatus = wk->unkD8 = func_ov135_021f72c4(&wk->uploadData);
        procs = &data_ov119_021e75b4;
        break;
    case DEMO_MODE_DOWNLOAD_EX:
        if (wk->uploadData.isTrade != 0) {
            param->recvPkm = (PartyPkm *)&wk->uploadData;
        } else {
            param->recvPkm = func_0200adb4(wk->param->worldtrade_data);
        }
        func_0200ad9c(wk->param->worldtrade_data, wk->sentPokemon);
        param->sendPkm = wk->sentPokemon;
        param->partnerStatus = wk->unkD8 = func_ov135_021f72c4(&wk->uploadData);
        procs = &data_ov119_021e75cc;
        break;
    case DEMO_MODE_EXCHANGE:
        copyPartyPkm(wk->demoPokemon, wk->sentPokemon);
        param->sendPkm = wk->demoPokemon;
        param->recvPkm = (PartyPkm *)&wk->downloadData[wk->unkD4];
        param->partnerStatus = wk->unkD8 = func_ov135_021f72c4(&wk->downloadData[wk->unkD4]);
        procs = &data_ov119_021e75cc;
        break;
    default:
        GFL_ASSERT(0);
        break;
    }

    QueueGameProc(wk->procManager, OVERLAY_POKEMONTRADE, procs, wk->subProcParam);
    wk->subprocFlag = 1;
    wk->unk12E4 = 0;
    return WT_SEQ_FADEIN;
}

int func_ov135_021f6fd4(WorldTradeWork *wk, int seq) {
    int ret = 2;

    switch (wk->subprocessSeq) {
    case 0:
        if (wk->procResult) {
            break;
        }
        if (wk->subProcessMode == DEMO_MODE_EXCHANGE) {
            PartyPkm *pkm;
            u32 species;
            u32 method;

            pkm = func_ov135_021f7338(wk, wk->subProcessMode);
            PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL);
            species = CheckEvolveSpecies(NULL, pkm, 1, (uintptr_t)wk->sentPokemon,
                                         GameData_GetSeason(GSYS_GetGameData(wk->param->gsys)), &method, HEAPID_WORLDTRADE);
            if (species != 0) {
                ShinkaDemoParam *evolution;

                if (wk->subProcParam != NULL) {
                    GFL_HeapFree(wk->subProcParam);
                    wk->subProcParam = NULL;
                }
                wk->subProcParam =
                    GFL_HeapAllocate(HEAPID_WORLDTRADE, sizeof(ShinkaDemoParam), FALSE, "worldtrade_demo.c", 235);
                MI_CpuFill8(wk->subProcParam, 0, sizeof(ShinkaDemoParam));
                evolution = wk->subProcParam;
                evolution->gameData = GSYS_GetGameData(wk->param->gsys);
                evolution->party = PokeParty_Create(HEAPID_WORLDTRADE);
                PokeParty_AddPkm(evolution->party, pkm);
                evolution->species = species;
                evolution->partyIndex = 0;
                evolution->method = method;
                evolution->unkC = 1;
                evolution->canCancel = FALSE;
                GFL_OvlUnload(OVERLAY_ID(67));
                QueueGameProc(wk->procManager, OVERLAY_SHINKA_DEMO, &data_ov195_02208c40, wk->subProcParam);
                wk->subprocessSeq = 1;
            } else {
                func_ov135_021f4ec0(wk, WORLDTRADE_TITLE, 0);
                ret = 3;
            }
        } else if (wk->subProcessMode == DEMO_MODE_DOWNLOAD || wk->subProcessMode == DEMO_MODE_DOWNLOAD_EX) {
            PartyPkm *pkm;
            PartyPkm *sent;
            u32 species;
            u32 method;

            pkm = func_ov135_021f7338(wk, wk->subProcessMode);
            sent = func_ov135_02202608(HEAPID_WORLDTRADE);
            func_0200ad9c(wk->param->worldtrade_data, sent);
            if (PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL) != PokeParty_GetParam(sent, PKM_PARAM_SPECIES, NULL) ||
                PokeParty_GetParam(pkm, PKM_PARAM_PID, NULL) != PokeParty_GetParam(sent, PKM_PARAM_PID, NULL)) {
                PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL);
                species = 0;
                if (wk->checkEvolution) {
                    wk->checkEvolution = FALSE;
                    species = CheckEvolveSpecies(NULL, pkm, 1, (uintptr_t)sent,
                                                 GameData_GetSeason(GSYS_GetGameData(wk->param->gsys)), &method,
                                                 HEAPID_WORLDTRADE);
                }
                if (species != 0) {
                    ShinkaDemoParam *evolution;

                    if (wk->subProcParam != NULL) {
                        GFL_HeapFree(wk->subProcParam);
                        wk->subProcParam = NULL;
                    }
                    wk->subProcParam =
                        GFL_HeapAllocate(HEAPID_WORLDTRADE, sizeof(ShinkaDemoParam), FALSE, "worldtrade_demo.c", 304);
                    MI_CpuFill8(wk->subProcParam, 0, sizeof(ShinkaDemoParam));
                    evolution = wk->subProcParam;
                    evolution->gameData = GSYS_GetGameData(wk->param->gsys);
                    evolution->party = PokeParty_Create(HEAPID_WORLDTRADE);
                    PokeParty_AddPkm(evolution->party, pkm);
                    evolution->species = species;
                    evolution->partyIndex = 0;
                    evolution->method = method;
                    evolution->unkC = 1;
                    evolution->canCancel = FALSE;
                    GFL_OvlUnload(OVERLAY_ID(67));
                    QueueGameProc(wk->procManager, OVERLAY_SHINKA_DEMO, &data_ov195_02208c40, wk->subProcParam);
                    wk->subprocessSeq = 1;
                } else {
                    func_ov135_021f4ec0(wk, WORLDTRADE_TITLE, 0);
                    ret = 3;
                }
            } else {
                func_ov135_021f4ec0(wk, WORLDTRADE_TITLE, 0);
                ret = 3;
            }
            GFL_HeapFree(sent);
        } else {
            func_ov135_021f4ec0(wk, WORLDTRADE_TITLE, 0);
            ret = 3;
        }
        break;
    case 1:
        if (wk->procResult) {
            break;
        }
        if (wk->subProcParam != NULL) {
            ShinkaDemoParam *evolution = wk->subProcParam;
            PartyPkm *pkm = func_ov135_021f7338(wk, wk->subProcessMode);

            copyPartyPkm(PokeParty_GetPkm(evolution->party, 0), pkm);
            GFL_HeapFree(evolution->party);
            GFL_HeapFree(wk->subProcParam);
            wk->subProcParam = NULL;
        }
        func_ov135_021f7374(wk);
        GX_SetVisibleWnd(GX_WNDMASK_NONE);
        GFL_OvlLoad(OVERLAY_ID(67));
        func_ov135_021f4ec0(wk, WORLDTRADE_UPLOAD, UPLOAD_MODE_POKEMON_EVO_SAVE);
        ret = 3;
        break;
    }
    return ret;
}


int func_ov135_021f7268(WorldTradeWork *wk, int seq) {
    if (wk->unkD8 != NULL) {
        GFL_HeapFree(wk->unkD8);
        wk->unkD8 = NULL;
    }
    GFL_HeapFree(wk->sentPokemon);
    if (wk->subProcParam != NULL) {
        GFL_HeapFree(wk->subProcParam);
        wk->subProcParam = NULL;
    }
    wk->checkEvolution = FALSE;
    func_ov135_021f5074(wk);
    func_ov135_021f4ec8(wk);
    G2_BlendNone();
    return 0;
}

PlayerInfo *func_ov135_021f72c4(Dpw_Tr_Data *dtd) {
    PlayerInfo *info = func_0200846c(HEAPID_WORLDTRADE);

    func_020084a0(info);
    copyTrainerName(info, dtd->name);
    func_02008560(info, dtd->versionCode);
    func_02008568(info, dtd->langCode);
    setTrainerGender(info, dtd->gender);
    func_02008558(info, dtd->trainerType);
    func_02008574(info, dtd->countryCode, dtd->localCode);
    info->id = dtd->trainerID;
    info->profileId = dtd->id;
    return info;
}

PartyPkm *func_ov135_021f7338(WorldTradeWork *wk, int mode) {
    if (mode == DEMO_MODE_EXCHANGE) {
        return (PartyPkm *)&wk->downloadData[wk->unkD4];
    }
    if (mode == DEMO_MODE_DOWNLOAD_EX) {
        return (PartyPkm *)&wk->uploadData;
    }
    if (mode == DEMO_MODE_DOWNLOAD) {
        return (PartyPkm *)&wk->uploadData;
    }
    GFL_ASSERT(0);
    return NULL;
}

void func_ov135_0220262c(PartyPkm *pkm, PartyPkm *partyPkm);
void func_ov135_021f7374(WorldTradeWork *wk) {
    PartyPkm *pkm = func_ov135_021f7338(wk, wk->subProcessMode);

    if (wk->unkDC == 0xff) {
        func_ov135_0220262c(pkm, PokeParty_GetPkm(wk->param->myparty, wk->unkE0));
    } else {
        int box = 0;
        int slot = 0;

        BoxSaveAccessor_ClearPkm(wk->param->mybox, wk->unkDC, wk->unkE0);
        BoxSaveAccessor_GetNextFreeBoxSlot(wk->param->mybox, &box, &slot);
        BoxSaveAccessor_InsertPkmCore(wk->param->mybox, box, func_0201d624(pkm));
    }
}



