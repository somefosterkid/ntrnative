// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov166/event_battle_return.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// Named after the string event_battle_return.c the overlay embeds
#include "types.h"
#include "app/event_battle_return.h"
#include "app/event_pdc_return.h"
#include "app/name_entry.h"
#include "app/zukan_toroku.h"
#include "battle/btl_setup.h"
#include "constants/abilities.h"
#include "constants/arc.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "constants/text_banks.h"
#include "demo/shinka_demo.h"
#include "field/player_state.h"
#include "field/zone.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "pml/evolution.h"
#include "pml/met_data.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "save/event_work.h"
#include "save/player_info.h"
#include "save/pokedex.h"
#include "save/records.h"
#include "save/save_control.h"
#include "system/game_data.h"

#define HEAPID_BATTLE_RETURN 0x52

// The battle types of the field's wild and trainer battles
#define BATTLE_TYPE_WILD 0
#define BATTLE_TYPE_TRAINER 1

// BtlSetup's unkA8: the player won, or caught the wild Pokémon
#define BATTLE_RESULT_WIN 1
#define BATTLE_RESULT_CAPTURE 5

// The battle backgrounds, by BtlFieldEnv's bgType, with the terrain Burmy takes its form from for each of its
// terrains
#define ARCID_BATTLE_BG_DATA 0x98

// Set once the player knows whose PC the boxes are
#define FLAG_BATTLE_RETURN_BOX_MESSAGE 0x96d

typedef struct {
    u8 unk00[0x13];
    u8 burmyTerrain[0x11];
} BattleBgData;

typedef struct {
    NameEntryParam *nameEntry;
    // The player's name, then the nickname
    StrBuf *name;
    // The caught Pokémon
    PartyPkm *pkm;
    ZukanTorokuParam zukan;
    // Where the Pokémon goes when the party is full
    StrBuf *boxMessage;
    u32 box;
    ShinkaDemoParam *shinkaDemo;
    // The party slot evolveMask's lowest bit stands for
    u16 evolveSlot;
    // The party slots whose Pokémon leveled up in the battle, which may evolve
    u16 evolveMask;
    u16 heapId;
    GameProcManager *procManager;
    // Set once the music is ready for the evolutions
    BOOL musicDone;
    BOOL musicFading;
} EventBattleReturnWork;

static BOOL func_ov092_021b95a0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov092_021b95e8(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov092_021b9614(GameProc *proc, u32 *state, void *param, void *work);
void func_ov092_021b9a14(EventBattleReturnWork *wk, EventBattleReturnParam *prm);
void func_ov092_021b9aa0(PokeParty *party);
void func_ov092_021b9bd8(PokeParty *party);
void func_ov092_021b9c34(EventBattleReturnParam *prm, PokeParty *party, u32 terrain);
u8 func_ov092_021b9c80(BtlFieldSituation *situation, HeapID heapId);
void func_ov092_021b9ca4(EventBattleReturnParam *prm, PokeParty *party);
void func_ov092_021b9d04(PartyPkm *pkm);

// Pickup's chances, in percent, cumulative: an item of data_ov092_021b9d6e from the slot for the level onwards
// pokebw2's sPickupChances
static const u8 data_ov092_021b9d44[9] = {30, 40, 50, 60, 70, 80, 90, 94, 98};

// Honey Gather's chance of Honey, in percent, by tens of levels
// pokebw2's sHoneyGatherChances
static const u8 data_ov092_021b9d4d[10] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50};

// Pickup's rare items, for the last two percent, from the slot for the level onwards
// pokebw2's sPickupRareItems
static const u16 data_ov092_021b9d58[11] = {
    ITEM_HYPER_POTION, ITEM_NUGGET,      ITEM_KINGS_ROCK,  ITEM_FULL_RESTORE, ITEM_ETHER,       ITEM_IRON_BALL,
    ITEM_PRISM_SCALE,  ITEM_ELIXIR,      ITEM_PRISM_SCALE, ITEM_LEFTOVERS,    ITEM_PRISM_SCALE,
};

// Pickup's items, from the slot for the level onwards
// pokebw2's sPickupItems
static const u16 data_ov092_021b9d6e[18] = {
    ITEM_POTION,       ITEM_ANTIDOTE, ITEM_SUPER_POTION, ITEM_GREAT_BALL, ITEM_REPEL,       ITEM_ESCAPE_ROPE,
    ITEM_FULL_HEAL,    ITEM_HYPER_POTION, ITEM_ULTRA_BALL, ITEM_REVIVE,   ITEM_RARE_CANDY,  ITEM_SUN_STONE,
    ITEM_MOON_STONE,   ITEM_HEART_SCALE, ITEM_FULL_RESTORE, ITEM_MAX_REVIVE, ITEM_PP_UP,    ITEM_MAX_ELIXIR,
};

// pokebw2's EVENT_BATTLE_RETURN_PROC_FUNCTIONS
const GameProcFunctions data_ov092_021b9d94 = {
    func_ov092_021b95a0,
    func_ov092_021b9614,
    func_ov092_021b95e8,
};

// pokebw2's EventBattleReturn_Init
static BOOL func_ov092_021b95a0(GameProc *proc, u32 *state, void *param, void *work) {
    EventBattleReturnWork *wk;

    GFL_HeapCreateChild(1, HEAPID_BATTLE_RETURN, 0x1000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(EventBattleReturnWork), HEAPID_BATTLE_RETURN);
    wk->name = GFL_StrBufCreate(32, HEAPID_BATTLE_RETURN);
    wk->nameEntry = NULL;
    wk->pkm = NULL;
    wk->boxMessage = NULL;
    wk->box = 0;
    wk->shinkaDemo = NULL;
    wk->heapId = HEAPID_BATTLE_RETURN;
    wk->procManager = CreateGameProcManager(wk->heapId);
    wk->musicDone = FALSE;
    wk->musicFading = FALSE;
    return TRUE;
}

// pokebw2's EventBattleReturn_Exit
static BOOL func_ov092_021b95e8(GameProc *proc, u32 *state, void *param, void *work) {
    EventBattleReturnWork *wk = work;

    FreeGameProcManager(wk->procManager);
    if (wk->boxMessage != NULL) {
        GFL_StrBufFree(wk->boxMessage);
    }
    GFL_StrBufFree(wk->name);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_BATTLE_RETURN);
    return TRUE;
}

// The battle's end: the party's levels, the money, the caught Pokémon, the evolutions
BOOL func_ov092_021b9614(GameProc *proc, u32 *state, void *param, void *work) {
    EventBattleReturnWork *wk = work;
    EventBattleReturnParam *prm = param;
    BOOL procRunning;
    PokeParty *party;
    PlayerInfo *playerInfo;
    PokeDexSave *pokedex;
    BoxSaveAccessor *boxes;
    s32 money;
    MsgData *msgData;
    PlayerState *playerState;
    BOOL nationalDex;
    BOOL firstCatch;
    int box;
    int slot;
    BOOL nickname;
    u16 species;
    u32 evolveSlot;
    PartyPkm *pkm;
    u32 method;
    ShinkaDemoParam *demo;

    procRunning = GFL_ProcMgrUpdate(wk->procManager);
    if (procRunning == TRUE) {
        return FALSE;
    }
    switch (*state) {
    case 0:
        party = GameData_GetParty(prm->gameData);
        playerInfo = GetGameDataPlayerInfo(prm->gameData);
        pokedex = GameData_GetPokedex(prm->gameData);
        boxes = GameData_GetBoxSaveAccessor(prm->gameData);
        wk->evolveSlot = 0;
        wk->evolveMask = 0;
        if (prm->setup->battleType <= BATTLE_TYPE_TRAINER) {
            func_ov092_021b9a14(wk, prm);
            PokeParty_Copy(prm->setup->party[0], party);
            pokerusHandler(party);
            pokerusSpread(party);
            if (prm->setup->unk8C == BATTLE_RESULT_WIN) {
                func_ov092_021b9aa0(party);
            }
            func_ov092_021b9c34(prm, party, func_ov092_021b9c80(&prm->setup->fieldSituation, wk->heapId));
            func_ov092_021b9ca4(prm, party);
            money = prm->setup->unk88;
            if (money > 0) {
                addCashToTotal(getTrainerCardDataBlkAddress(prm->gameData), money);
            } else if (money < 0) {
                subCashFromTotal(getTrainerCardDataBlkAddress(prm->gameData), -money);
            }
        }
        func_ov092_021b9bd8(party);
        if (prm->setup->unk8C == BATTLE_RESULT_CAPTURE) {
            wk->pkm = PokeParty_GetPkm(prm->setup->party[1], prm->setup->unk90);
            textCopy(playerInfo->name, wk->name);
            PokeParty_SetParam(wk->pkm, PKM_PARAM_OT_NAME, (uintptr_t)wk->name);
            if (PokeParty_GetPkmCount(party) >= PokeParty_GetCapacity(party)) {
                box = BoxSaveAccessor_GetLastOpenedBox(boxes);
                slot = 0;
                BoxSaveAccessor_GetNextFreeBoxSlot(boxes, &box, &slot);
                wk->box = box;
                msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_SERVER_FLOW_TITLE, HEAPID_BATTLE_RETURN);
                wk->boxMessage = GFL_MsgDataLoadStrbufNew(
                    msgData,
                    EventWork_FlagGet(GameData_GetEventWork(prm->gameData), FLAG_BATTLE_RETURN_BOX_MESSAGE) ? 0xb1 : 0xb0);
                GFL_MsgDataFree(msgData);
            }
            playerState = GameData_GetPlayerState(prm->gameData);
            PokeParty_SetupMetData(wk->pkm, 0, playerInfo, ZoneData_GetPlaceNameID(PlayerState_GetZoneID(playerState)), wk->heapId);
            nationalDex = PokeDex_IsNationalObtained(pokedex);
            firstCatch = FALSE;
            if (!PokeDex_IsCaught(pokedex, PokeParty_GetParam(wk->pkm, PKM_PARAM_SPECIES, NULL))) {
                firstCatch = TRUE;
            }
            PokeDex_RegistPkm(pokedex, wk->pkm);
            addPkmToDex(pokedex, wk->pkm);
            wk->zukan.pkm = wk->pkm;
            wk->zukan.nationalDex = nationalDex;
            wk->zukan.boxMessage = wk->boxMessage;
            wk->zukan.boxes = boxes;
            wk->zukan.box = wk->box;
            wk->zukan.gameData = prm->gameData;
            if (firstCatch) {
                wk->zukan.mode = 0;
            } else {
                wk->zukan.mode = 1;
            }
            QueueGameProc(wk->procManager, OVERLAY_ID(207), &data_ov207_0220bab0, &wk->zukan);
            (*state)++;
        } else {
            *state = 4;
        }
        break;
    case 1:
        if (procRunning != TRUE) {
            boxes = GameData_GetBoxSaveAccessor(prm->gameData);
            nickname = FALSE;
            if (wk->zukan.nickname == TRUE) {
                nickname = TRUE;
            }
            if (nickname) {
                wk->nameEntry = pokemonNameEntry(wk->heapId, wk->pkm, 10, NULL, wk->boxMessage, boxes, wk->box,
                                                 getTrainerGameInfoAddress(GameData_GetSaveControl(prm->gameData)));
                QueueGameProc(wk->procManager, OVERLAY_ID(194), &data_ov194_021f5580, wk->nameEntry);
                (*state)++;
            } else {
                *state = 3;
            }
        } else {
            return FALSE;
        }
        break;
    case 2:
        if (procRunning != TRUE) {
            if (func_ov010_0216c0e4(wk->nameEntry) == FALSE) {
                func_ov010_0216c0d4(wk->nameEntry, wk->name);
                PokeParty_SetParam(wk->pkm, PKM_PARAM_NICKNAME, (uintptr_t)wk->name);
                RecordAddOne(GameData_GetRecords(prm->gameData), 0x1e);
            }
            func_ov010_0216c0c0(wk->nameEntry);
            wk->nameEntry = NULL;
            (*state)++;
        } else {
            return FALSE;
        }
        break;
    case 3:
        party = GameData_GetParty(prm->gameData);
        func_ov092_021b9d04(wk->pkm);
        if (wk->boxMessage == NULL) {
            PokeParty_AddPkm(party, wk->pkm);
        } else {
            BoxSaveAccessor_InsertPkm(GameData_GetBoxSaveAccessor(prm->gameData), func_020185f0(wk->pkm));
        }
        (*state)++;
        break;
    case 4:
        if (!wk->musicDone) {
            if (prm->setup->unk8C == BATTLE_RESULT_CAPTURE && wk->evolveMask != 0) {
                if (!wk->musicFading) {
                    GFL_SndBGMFadeOut(30);
                    wk->musicFading = TRUE;
                } else if (!GFL_SndBGMIsFading()) {
                    func_02005d28();
                    wk->musicDone = TRUE;
                }
            } else {
                wk->musicDone = TRUE;
            }
        } else if (wk->evolveMask != 0) {
            species = 0;
            party = GameData_GetParty(prm->gameData);
            *state = 6;
            while (wk->evolveMask != 0) {
                if (wk->evolveMask & 1) {
                    pkm = PokeParty_GetPkm(party, wk->evolveSlot);
                    species = CheckEvolveSpecies(party, pkm, 0, prm->setup->fieldSituation.env.zoneId,
                                                 GameData_GetSeason(prm->gameData), &method, wk->heapId);
                    evolveSlot = wk->evolveSlot;
                }
                wk->evolveMask = wk->evolveMask >> 1;
                wk->evolveSlot++;
                if (species != 0) {
                    demo = GFL_HeapAllocate(wk->heapId, sizeof(ShinkaDemoParam), FALSE, "event_battle_return.c", 458);
                    demo->gameData = prm->gameData;
                    demo->party = party;
                    demo->species = species;
                    demo->partyIndex = evolveSlot;
                    demo->method = method;
                    demo->unkC = 0;
                    demo->canCancel = TRUE;
                    wk->shinkaDemo = demo;
                    QueueGameProc(wk->procManager, OVERLAY_ID(195), &data_ov195_02208c40, demo);
                    *state = 5;
                    break;
                }
            }
        } else {
            *state = 6;
        }
        break;
    case 5:
        if (procRunning != TRUE) {
            GFL_HeapFree(wk->shinkaDemo);
            wk->shinkaDemo = NULL;
            if (wk->evolveMask != 0) {
                *state = 4;
            } else {
                (*state)++;
            }
        } else {
            return FALSE;
        }
        break;
    case 6:
    default:
        return TRUE;
    }
    return FALSE;
}


void func_ov092_021b9a14(EventBattleReturnWork *wk, EventBattleReturnParam *prm) {
    PokeParty *party;
    int count;
    int i;
    u32 levelBefore;

    switch (prm->setup->unk8C) {
    case BATTLE_RESULT_WIN:
    case 3:
    case BATTLE_RESULT_CAPTURE:
        switch (prm->setup->battleType) {
        case BATTLE_TYPE_WILD:
        case BATTLE_TYPE_TRAINER:
            party = GameData_GetParty(prm->gameData);
            count = PokeParty_GetPkmCount(party);
            for (i = 0; i < count; i++) {
                PartyPkm *before = PokeParty_GetPkm(party, i);
                PartyPkm *after = PokeParty_GetPkm(prm->setup->party[0], i);

                levelBefore = PokeParty_GetParam(before, PKM_PARAM_LEVEL, NULL);
                if (levelBefore < PokeParty_GetParam(after, PKM_PARAM_LEVEL, NULL)) {
                    wk->evolveMask |= 1 << i;
                }
            }
            break;
        }
        break;
    }
}

// Pickup and Honey Gather
// pokebw2's EventBattleReturn_PickupItems
void func_ov092_021b9aa0(PokeParty *party) {
    int i;
    int j;
    PartyPkm *pkm;
    u16 species;
    u16 item;
    u8 ability;
    int rand;
    u8 levelSlot;
    u8 level;
    int level10;

    for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
        pkm = PokeParty_GetPkm(party, i);
        species = PokeParty_GetParam(pkm, PKM_PARAM_LEGAL_SPECIES, NULL);
        item = PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL);
        ability = PokeParty_GetParam(pkm, PKM_PARAM_ABILITY, NULL);
        if (ability == ABILITY_PICKUP && species != 0 && species != SPECIES_EGG && item == 0 && GFL_RandomLC(10) == 0) {
            rand = GFL_RandomLC(100);
            levelSlot = (PokeParty_GetParam(pkm, PKM_PARAM_LEVEL, NULL) - 1) / 10;
            if (levelSlot >= 10) {
                levelSlot = 9;
            }
            for (j = 0; j < 9; j++) {
                if (data_ov092_021b9d44[j] > rand) {
                    PokeParty_SetParam(pkm, PKM_PARAM_ITEM, data_ov092_021b9d6e[levelSlot + j]);
                    break;
                } else if (rand >= 98 && rand <= 99) {
                    PokeParty_SetParam(pkm, PKM_PARAM_ITEM, data_ov092_021b9d58[levelSlot + (99 - rand)]);
                    break;
                }
            }
        }
        if (ability == ABILITY_HONEY_GATHER && species != 0 && species != SPECIES_EGG && item == 0) {
            j = 0;
            level10 = 10;
            level = PokeParty_GetParam(pkm, PKM_PARAM_LEVEL, NULL);
            while (level10 < level) {
                level10 += 10;
                j++;
            }
            if (GFL_RandomLC(100) < data_ov092_021b9d4d[j]) {
                PokeParty_SetParam(pkm, PKM_PARAM_ITEM, ITEM_HONEY);
            }
        }
    }
}

// Natural Cure cures the party's status after the battle
// pokebw2's EventBattleReturn_NaturalCure
void func_ov092_021b9bd8(PokeParty *party) {
    int i;
    PartyPkm *pkm;
    u16 species;
    u8 ability;

    for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
        pkm = PokeParty_GetPkm(party, i);
        species = PokeParty_GetParam(pkm, PKM_PARAM_LEGAL_SPECIES, NULL);
        ability = PokeParty_GetParam(pkm, PKM_PARAM_ABILITY, NULL);
        if (ability == ABILITY_NATURAL_CURE && species != 0 && species != SPECIES_EGG) {
            PokeParty_SetStatusCond(pkm, 0);
        }
    }
}

void func_ov010_0216e62c(GameData *gameData, PartyPkm *pkm, u32 terrain);
void func_ov092_021b9c34(EventBattleReturnParam *prm, PokeParty *party, u32 terrain) {
    int i;

    if (prm->setup->battleType == 2 || prm->setup->battleType == 3) {
        return;
    }
    for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
        if (prm->setup->unkC2[i]) {
            func_ov010_0216e62c(prm->gameData, PokeParty_GetPkm(party, i), terrain);
        }
    }
}


// The terrain Burmy changes its form for, from the battle's background
// pokebw2's EventBattleReturn_GetBurmyTerrain
u8 func_ov092_021b9c80(BtlFieldSituation *situation, HeapID heapId) {
    BattleBgData *data = GFL_ArcSysReadHeapNew(ARCID_BATTLE_BG_DATA, 0, heapId);
    u8 terrain = data[situation->env.bgType].burmyTerrain[situation->env.terrain];

    GFL_HeapFree(data);
    return terrain;
}

// Registers each Shaymin that unkE1 marks in the Pokédex again, for its form
// pokebw2's EventBattleReturn_ShayminForms
void func_ov092_021b9ca4(EventBattleReturnParam *prm, PokeParty *party) {
    int i;
    PartyPkm *pkm;

    switch (prm->setup->battleType) {
    case BATTLE_TYPE_WILD:
    case BATTLE_TYPE_TRAINER:
        for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
            if (prm->setup->unkC2[i]) {
                pkm = PokeParty_GetPkm(party, i);
                if (PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL) == SPECIES_SHAYMIN) {
                    PokeDex_RegistPkm(GameData_GetPokedex(prm->gameData), pkm);
                }
            }
        }
        break;
    }
}

// A Pokémon caught in a Heal Ball comes out healed
// pokebw2's EventBattleReturn_HealBall
void func_ov092_021b9d04(PartyPkm *pkm) {
    BOOL wasEncrypted = PokeParty_DecryptPkm(pkm);

    if (PokeParty_GetParam(pkm, PKM_PARAM_POKEBALL, NULL) == ITEM_HEAL_BALL) {
        PokeParty_SetParam(pkm, PKM_PARAM_HP, PokeParty_GetParam(pkm, PKM_PARAM_MAX_HP, NULL));
        PokeParty_SetStatusCond(pkm, 0);
    }
    PokeParty_EncryptPkm(pkm, wasEncrypted);
}
