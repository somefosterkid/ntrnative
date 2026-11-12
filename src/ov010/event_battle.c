// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/event_battle.c, ported by tools/port_file.py.
// The events that run battles from the field: wild, trainer, Trial House and capture demonstration battles, and what
// the game records after them. The name is the ROM's own, from GFL_HeapAllocate's file argument.
// Function and data names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "app/event_battle_return.h"
#include "battle/battle_proc.h"
#include "battle/btl_result.h"
#include "battle/btl_setup.h"
#include "battle/trainer_data.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "constants/version.h"
#include "field/encounter.h"
#include "field/event_battle.h"
#include "field/event_battle_lose.h"
#include "field/event_sound.h"
#include "field/festival.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_event.h"
#include "field/field_map.h"
#include "field/field_status.h"
#include "field/game_beacon_set.h"
#include "field/move_pokemon.h"
#include "field/player_state.h"
#include "field/trial_house.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/os.h"
#include "pml/poke_party.h"
#include "save/encounter.h"
#include "save/pokedex.h"
#include "save/records.h"
#include "save/save_control.h"
#include "system/aeabi.h"
#include "system/area_data.h"
#include "system/comm_player_support.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/zone_weather.h"
#include "nitro/mi.h"

#include "app/mystery/mystery_gift_data.h"
#include "battle/regulation.h"
#include "save/traded_pokemon.h"
#include "dsprot/dsprot.h"

extern const GameProcFunctions data_ov092_021b9d94;
extern u32 g_AntiPiracyTaskCount;
void func_ov010_0216f064(void);
void func_ov010_0216f070(void);
void func_ov010_0216f07c(void);

static u32 func_ov010_0216edfc(u16 trainerId);

// ov018's, which no header declares yet: an event of the communication support
GameEvent *func_ov018_02181d8c(GameSystem *gsys, u32 a1);

// The work of EventBattleCall_Callback, the battle itself
typedef struct {
    GameSystem *gsys;
    GameData *gameData;
    BtlSetup *setup;
    EventBattleReturnParam battleReturnParam;
    // Whether the caller returns to the field itself
    u32 unk14;
    // Whether losing doesn't black the player out
    u32 unk18;
    // Whether it is the Trial House's
    u32 unk1C;
    // Whether the caller frees the setup
    u32 unk20;
    u32 encEffect;
} EventBattleCallWork;

// The work of EventWildBattleCall_Callback
typedef struct {
    GameSystem *gsys;
    BtlSetup *setup;
    u32 unk08;
    u32 unk0C;
} EventWildBattleCallWork;

static GameEventReturnCode EventBattleCall_Callback(GameEvent *event, u32 *state, void *work);
void func_ov010_0216ee20(u16 trainerId, u32 kind);
void func_ov010_0216ee40(BtlSetup *setup, u32 kind);
void func_ov010_0216ee7c(EventBattleCallWork *work);
void func_ov010_0216ef84(EventBattleCallWork *work);
BOOL EventBattleCall_IsResultDefeat(EventBattleCallWork *work);
void EventBattleCall_SyncGameDataOnReturn(EventBattleCallWork *work, GameData *gameData);
void EventBattleCall_Setup(EventBattleCallWork *work, GameSystem *gsys, BtlSetup *setup);
void EventBattleCall_ReleaseBtlSetup(EventBattleCallWork *work);

// The result of a wild battle for each battle result
static const u8 WILD_BATTLE_RESULT[7] = {0xff, 2, 0xff, 1, 1, 0, 0};

// The beacons a trainer battle sends, for its kind of trainer and whether it starts or ends
static void (*const data_ov010_021725c0[3][2])(u16 trainerId) = {
    {func_ov010_02165174, func_ov010_021651ac},
    {func_ov010_021651e4, func_ov010_0216521c},
    {func_ov010_02165254, func_ov010_0216528c},
};

// Whether each battle result is not a defeat, for each battle type
static const u8 BTL_RESULT_LUT[7][5] = {
    {0, 0, 0, 0, 0},
    {1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0},
    {1, 2, 2, 0, 0},
    {1, 2, 1, 1, 0},
    {1, 2, 2, 2, 0},
    {0, 0, 0, 0, 0},
};

static GameEventReturnCode EventWildBattleCall_Callback(GameEvent *event, u32 *state, void *work) {
    EventWildBattleCallWork *wk = work;
    GameSystem *gsys = wk->gsys;
    GameData *gameData = GSYS_GetGameData(gsys);

    switch (*state) {
    case 0: {
        GameEvent *battle = GameEvent_Create(gsys, NULL, EventBattleCall_Callback, sizeof(EventBattleCallWork));
        EventBattleCallWork *battleWork = GameEvent_GetData(battle);
        PartyPkm *pkm;
        u32 species;
        Field *field;

        EventBattleCall_Setup(battleWork, gsys, wk->setup);
        battleWork->unk14 = TRUE;
        pkm = PokeParty_GetPkm(BtlSetup_GetParty(wk->setup, 1), 0);
        species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
        field = GSYS_GetField(gsys);
        EventBattleCall_DecideEnvWild(species, wk->unk0C, wk->setup->battleStyle == 1, field, &battleWork->encEffect,
                                      &battleWork->setup->fieldSituation.bgm);
        GameEvent_ChainNext(event, battle);
        (*state)++;
        break;
    }
    case 1:
        if (wk->unk08 == 1) {
            func_0202e79c(func_02012f20(gameData));
            return GAMEEVENT_DONE;
        }
        if (IsBattleResultDefeat(GameData_GetLastBtlResult(gameData), 0) == TRUE) {
            func_0202e79c(func_02012f20(gameData));
            GameEvent_Replace(event, EventBattleLose_Create(gsys));
        } else {
            GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, GSYS_GetField(gsys), 0, 0, 1, 0, 0));
        }
        (*state)++;
        break;
    case 2:
        if (func_0202e818(func_02012f20(gameData)) == TRUE) {
            GameEvent_ChainNext(event, func_ov018_02181d8c(gsys, 4));
        }
        (*state)++;
        break;
    case 3:
        func_0202e79c(func_02012f20(gameData));
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *EventWildBattleCall_Create(GameSystem *gsys, Field *field, BtlSetup *setup, u8 a3, u32 a4) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventWildBattleCall_Callback, sizeof(EventWildBattleCallWork));
    EventWildBattleCallWork *wk = GameEvent_GetData(event);

    wk->gsys = gsys;
    wk->setup = setup;
    wk->unk08 = a3;
    wk->unk0C = a4;
    func_ov010_0216ee40(setup, 0);
    RecordAddOne(GameData_GetRecords(GSYS_GetGameData(gsys)), 5);
    func_ov021_021aaabc(Field_GetEncountSystem(field));
    return event;
}

GameEvent *EventTrainerBattleCall_Create(GameSystem *gsys, Field *field, u32 a2, u32 a3, u32 trainerId, u32 a5,
                                         u32 flags) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventBattleCall_Callback, sizeof(EventBattleCallWork));
    EventBattleCallWork *work = GameEvent_GetData(event);
    EncountSystem *encountSystem;
    GameRecords *records;
    BtlSetup *setup;

    encountSystem = Field_GetEncountSystem(field);
    records = GameData_GetRecords(GSYS_GetGameData(gsys));
    setup = BtlSetup_Create(4);
    BtlSetup_SetTrainerLocal(encountSystem, setup, a2, a3, trainerId, a5, 4);
    func_ov010_0216ee20(trainerId, 0);
    RecordAddOne(records, 6);
    RecordAddOne(records, 0x56);
    EventBattleCall_Setup(work, gsys, setup);
    work->unk14 = TRUE;
    if (flags & 1) {
        BtlSetup_SetFlag(setup, 0x100);
        work->unk18 = TRUE;
    }
    EventBattleCall_DecideEnvTrainer(trainerId, field, &work->encEffect, &work->setup->fieldSituation.bgm);
    func_ov021_021aaabc(Field_GetEncountSystem(field));
    return event;
}

GameEvent *func_ov010_0216e8cc(GameSystem *gsys, Field *field, BtlSetup *setup) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventBattleCall_Callback, sizeof(EventBattleCallWork));
    EventBattleCallWork *work = GameEvent_GetData(event);

    EventBattleCall_Setup(work, gsys, setup);
    work->unk14 = FALSE;
    work->unk18 = TRUE;
    work->unk20 = TRUE;
    work->encEffect = 9;
    func_ov021_021aaabc(Field_GetEncountSystem(field));
    return event;
}

GameEvent *CreateTrialHouseBattleEvent(GameSystem *gsys, Field *field, BtlSetup *setup) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventBattleCall_Callback, sizeof(EventBattleCallWork));
    EventBattleCallWork *work = GameEvent_GetData(event);

    EventBattleCall_Setup(work, gsys, setup);
    work->unk14 = FALSE;
    work->unk1C = TRUE;
    work->unk18 = TRUE;
    work->encEffect = 9;
    work->setup->fieldSituation.bgm = 0x46b;
    func_ov021_021aaabc(Field_GetEncountSystem(field));
    return event;
}

GameEvent *EventCaptureDemo_Create(GameSystem *gsys, Field *field, HeapID heapId) {
    HeapID tempHeapId = HEAPID_TAIL(heapId);
    GameData *gameData = GSYS_GetGameData(gsys);
    BtlSetup *setup = BtlSetup_Create(heapId);
    PokeParty *party = PokeParty_Create(tempHeapId);
    PokeParty *enemyParty = PokeParty_Create(tempHeapId);
    PartyPkm *pkm = GFL_HeapAllocate(tempHeapId, PokeParty_GetPkmRawSize(), TRUE, "event_battle.c", 474);
    BtlFieldStatus status;
    GameEvent *event;
    EventBattleCallWork *work;

    PokeParty_CreateTempPkm(pkm, SPECIES_MINCCINO, 7, 0);
    PokeParty_SetParam(pkm, PKM_PARAM_MOVE1, MOVE_POUND);
    PokeParty_SetParam(pkm, PKM_PARAM_MOVE2, MOVE_GROWL);
    PokeParty_SetParam(pkm, PKM_PARAM_MOVE3, 0);
    PokeParty_SetParam(pkm, PKM_PARAM_MOVE4, 0);
    PokeParty_AddPkm(party, pkm);
    PokeParty_CreateTempPkm(pkm, SPECIES_PATRAT, 2, 0);
    PokeParty_SetParam(pkm, PKM_PARAM_MOVE1, MOVE_TACKLE);
    PokeParty_SetParam(pkm, PKM_PARAM_MOVE2, MOVE_LEER);
    PokeParty_SetParam(pkm, PKM_PARAM_MOVE3, 0);
    PokeParty_SetParam(pkm, PKM_PARAM_MOVE4, 0);
    PokeParty_AddPkm(enemyParty, pkm);
    SaveBtlFieldStatus(&status, gameData, field);
    status.terrain = 5;
    status.bgId = 0;
    BtlSetup_SetCaptureDemo(setup, gameData, party, enemyParty, &status, heapId);
    GFL_HeapFree(pkm);
    GFL_HeapFree(enemyParty);
    GFL_HeapFree(party);
    event = GameEvent_Create(gsys, NULL, EventBattleCall_Callback, sizeof(EventBattleCallWork));
    work = GameEvent_GetData(event);
    EventBattleCall_Setup(work, gsys, setup);
    work->unk14 = TRUE;
    EventBattleCall_DecideEnvWild(SPECIES_PATRAT, 5, FALSE, field, &work->encEffect, &work->setup->fieldSituation.bgm);
    work->encEffect = 0;
    return event;
}

GameEvent *LoadTradedPokemonBattleStats(GameSystem *gsys, Field *field, u32 a2, u32 a3, u32 trainerId, u32 a5,
                                        u32 flags, u32 index) {
    void *block = GetTradedPokemonBlock(GSYS_GetGameData(gsys));
    GameEvent *event = EventTrainerBattleCall_Create(gsys, field, a2, a3, trainerId, a5, flags);

    if (func_0200e740(block, index)) {
        EventBattleCallWork *work = GameEvent_GetData(event);
        PartyPkm *pkm = PokeParty_GetPkm(work->setup->party[1], 0);
        u32 species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
        u32 tradedSpecies = func_0200e788(block, index, GAME_VERSION);
        u8 level = func_0200e74c(block, index);
        u32 id = func_0200e7b4(block, index);

        PokeParty_CreatePkm(pkm, tradedSpecies, level, id, PKM_IVS_RANDOM, func_0200e7a4(block, index));
        if (func_0200e76c(block, index)) {
            PokeParty_SetParam(pkm, PKM_PARAM_ABILITY, func_0200e780(block, index));
        }
        PokeParty_SetParam(pkm, PKM_PARAM_NICKNAME_RAW, (uintptr_t)func_0200e7ac(block, index));
        setChangedPkmSpecies(pkm, (u16)species);
        PokeParty_SetDefaultMoves(pkm);
    }
    return event;
}

static GameEventReturnCode EventBattleCall_Callback(GameEvent *event, u32 *state, void *work) {
    EventBattleCallWork *wk = work;
    GameSystem *gsys = wk->gsys;
    GameData *gameData = wk->gameData;
    Field *field = GSYS_GetField(gsys);
    u32 seed = 0x13a1ab5;

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, EventBattleBGMPlay_Create(gsys, wk->setup->fieldSituation.bgm));
        (*state)++;
        break;
    case 1:
        EncEff_StartEvent(Field_GetEncEff(field), event, wk->encEffect);
        (*state)++;
        break;
    case 2:
        GameEvent_ChainNext(event, CreateFieldCloseEvent(gsys, field));
        (*state)++;
        break;
    case 3:
        FieldStatus_SetBusyFlag(GameData_GetFieldStatus(gameData), TRUE);
        func_0202e79c(func_02012f20(gameData));
        GSYS_QueueProcAsEvent(event, OVERLAY_ID(93), &data_ov093_021efdac, wk->setup);
        (*state)++;
        break;
    case 4:
        wk->battleReturnParam.setup = wk->setup;
        wk->battleReturnParam.gameData = gameData;
        GSYS_QueueProcAsEvent(event, OVERLAY_ID(92), &data_ov092_021b9d94, &wk->battleReturnParam);
        (*state)++;
        break;
    case 5:
        GFL_OvlLoad(OVERLAY_ID(230));
        FieldStatus_SetBusyFlag(GameData_GetFieldStatus(gameData), FALSE);
        func_0202e80c(func_02012f20(gameData));
        (*state)++;
        break;
    case 6: {
        u32 flag = func_ov230_021882dc(func_ov010_0216e680, func_ov010_0216f064, 0) ==
                   (u32)~(uintptr_t)func_ov010_0216f064;

        seed += flag ? 0 : 0x3d1;
        EventBattleCall_SyncGameDataOnReturn(wk, gameData);
        if (wk->unk1C != 0) {
            SyncTrialHouseWkStatsFromBattle(*GetTrialHouseWkPPtr(gameData), wk->setup);
        }
        (*state)++;
        flag = func_ov230_02188354(func_ov010_0216e680, func_ov010_0216f070, 0) == (u32)~(uintptr_t)func_ov010_0216f070;
        seed += flag ? 0 : 0x5d1;
        break;
    }
    case 7: {
        u32 flag = func_ov230_02188390(func_ov010_0216e680, func_ov010_0216f07c, 0) ==
                   (u32)~(uintptr_t)func_ov010_0216f07c;

        seed += flag * 0x9d;
        if (wk->unk18 == 0 && EventBattleCall_IsResultDefeat(wk) == 1) {
            GameEvent_ChainNext(event, EventBGMFadeStop_Create(gsys, 0x1e));
        } else {
            GameEvent_ChainNext(event, EventBGMFadePop_Create(gsys));
        }
        GFL_OvlUnload(OVERLAY_ID(230));
        (*state)++;
        break;
    }
    case 8:
        if (wk->unk18 == 0 && EventBattleCall_IsResultDefeat(wk) == 1) {
            if (wk->unk14 == 1) {
                EventBattleCall_ReleaseBtlSetup(wk);
                return GAMEEVENT_DONE;
            }
            EventBattleCall_ReleaseBtlSetup(wk);
            GameEvent_Replace(event, EventBattleLose_Create(gsys));
            return GAMEEVENT_CONTINUE;
        }
        GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(gsys));
        (*state)++;
        break;
    case 9:
        GameEvent_ChainNext(event, EventBGMFadeWait_Create(gsys));
        (*state)++;
        break;
    case 10:
        if (wk->unk14 == 0) {
            GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, field, 0, 0, 1, 0, 0));
        }
        (*state)++;
        break;
    case 11:
        EventBattleCall_ReleaseBtlSetup(wk);
        return GAMEEVENT_DONE;
    }
    if (seed % 0x1933 != 0) {
        GFL_VBlankTCBAdd(func_02011d34, NULL, 0x7f);
        g_AntiPiracyTaskCount++;
    }
    return GAMEEVENT_CONTINUE;
}


// The kind of beacon for the trainer: 0 for some classes, 1 for others and 2 for the rest
static u32 func_ov010_0216edfc(u16 trainerId) {
    u32 group = GetTrainerClassBGMGroupId(TrainerData_GetParam(trainerId, 1));

    if (group == 12 || group == 9) {
        return 0;
    }
    return group == 2 ? 1 : 2;
}

void func_ov010_0216ee20(u16 trainerId, u32 kind) {
    data_ov010_021725c0[func_ov010_0216edfc(trainerId)][kind](trainerId);
}

void func_ov010_0216ee40(BtlSetup *setup, u32 kind) {
    u16 species = PokeParty_GetParam(PokeParty_GetPkm(BtlSetup_GetParty(setup, 1), 0), PKM_PARAM_SPECIES, NULL);

    switch (kind) {
    case 0:
        func_ov010_0216505c(species);
        break;
    case 1:
        func_ov010_02165078(species);
        break;
    case 2:
        func_ov010_021653f4(species);
        break;
    }
}

void func_ov010_0216ee7c(EventBattleCallWork *work) {
    BtlSetup *setup = work->setup;
    PokeParty *party = GameData_GetParty(work->gameData);
    PartyPkm *pkm = PokeParty_GetPkm(party, isEggInParty(party));

    if (setup->unk8C != 0 && setup->unk8C != 2) {
        StrBuf *name = GFL_StrBufCreate(12, HEAPID_TAIL(4));
        u16 hp, maxHp;
        int i;

        PokeParty_GetParam(pkm, PKM_PARAM_NICKNAME, name);
        hp = PokeParty_GetParam(pkm, PKM_PARAM_HP, NULL);
        maxHp = PokeParty_GetParam(pkm, PKM_PARAM_MAX_HP, NULL);
        if (hp == 0) {
            func_ov010_021654ac(name);
        } else if (hp * 2 <= maxHp) {
            func_ov010_02165424(name);
        }
        for (i = 0; i < 4; i++) {
            if (PokeParty_GetParam(pkm, PKM_PARAM_MOVE1 + i, NULL) != 0 &&
                PokeParty_GetParam(pkm, PKM_PARAM_MOVE1_PP + i, NULL) == 0) {
                func_ov010_02165468(name);
                break;
            }
        }
        if (PokeParty_GetParam(pkm, PKM_PARAM_STATUS, NULL)) {
            func_ov010_021654f0(name);
        }
        GFL_StrBufFree(name);
    }
    switch (setup->battleType) {
    case 0:
        if (setup->unk8C == 3) {
            func_ov010_0216ee40(setup, 2);
        } else if (setup->unk8C == 1) {
            func_ov010_0216ee40(setup, 1);
        }
        break;
    case 1:
        if (setup->unk8C == 1) {
            func_ov010_0216ee20(setup->trainers[1]->trainerId, 1);
        }
        break;
    }
}

void func_ov010_0216ef84(EventBattleCallWork *work) {
    BtlSetup *setup = work->setup;
    GameRecords *records = GameData_GetRecords(GSYS_GetGameData(work->gsys));

    if (setup->battleType == 0 && setup->unk8C == 4) {
        RecordAddOne(records, 0x4f);
    }
}

BOOL IsBattleResultDefeat(u32 result, u32 battleType) {
    if (battleType == 4) {
        return FALSE;
    }
    if (BTL_RESULT_LUT[result][battleType] == 0) {
        return TRUE;
    }
    return FALSE;
}

u32 GetWildBattleResultByCombined(u32 result) {
    u8 wildResult = WILD_BATTLE_RESULT[result];

    if (wildResult == 0xff) {
        wildResult = 1;
    }
    return wildResult;
}

BOOL EventBattleCall_IsResultDefeat(EventBattleCallWork *work) {
    return IsBattleResultDefeat(work->setup->unk8C, work->setup->battleType);
}

void EventBattleCall_SyncGameDataOnReturn(EventBattleCallWork *work, GameData *gameData) {
    GameData_SetLastBtlResult(gameData, work->setup->unk8C);
    func_ov010_0215e5b0(gameData, work->setup);
    func_ov010_0216ee7c(work);
    func_ov010_0216ef84(work);
    ResetWeather(work->gsys, PlayerState_GetZoneID(GameData_GetPlayerState(gameData)));
}

void EventBattleCall_Setup(EventBattleCallWork *work, GameSystem *gsys, BtlSetup *setup) {
    MIi_CpuClear32(0, work, sizeof(EventBattleCallWork));
    work->gsys = gsys;
    work->gameData = GSYS_GetGameData(gsys);
    work->setup = setup;
    work->unk14 = FALSE;
    work->unk18 = FALSE;
    work->unk1C = FALSE;
}

void EventBattleCall_ReleaseBtlSetup(EventBattleCallWork *work) {
    if (work->unk20 == FALSE) {
        BtlSetup_Free(work->setup);
    }
}

void func_ov010_0216f064(void) {
    GFL_RandomMT();
    GFL_RandomMT();
}

void func_ov010_0216f070(void) {
    GFL_RandomMT();
    GFL_RandomMT();
}

void func_ov010_0216f07c(void) {
    GFL_RandomMT();
    GFL_RandomMT();
}


