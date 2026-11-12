// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/scrcmd_entree_forest.c, ported by tools/port_file.py.
// The Entree Forest's script commands and events: its Pokémon, catching one, and the warps between its areas. Names
// from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0); the file's name is descriptive
#include "types.h"
#include "app/event_pdc_return.h"
#include "app/pdc.h"
#include "constants/pokemon.h"
#include "field/entree_forest.h"
#include "field/event_mapchange.h"
#include "field/event_sound.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_event.h"
#include "field/field_script.h"
#include "field/intrude_comm.h"
#include "field/scrcmd_entree_forest.h"
#include "field/symbol_map.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/vm.h"
#include "system/wordset.h"
#include "save/trainer_data_savedata.h"
#include "save/pokedex.h"

#define ITEM_DREAM_BALL 576
#define SEQ_SE_WARP 0x559
// The script variable that holds the actor the player talks to
#define VAR_TALK_ACTOR 0x8011
// The area of the forest that isn't one, for warps out of it
#define ENTREE_FOREST_DIR_EXIT 9

typedef struct {
    Field *field;
    PdcWork *pdcWork;
    EventPdcReturnParam *returnWork;
    PartyPkm *pkm;
    BtlFieldStatus status;
    EntreeForestPokemon pokemon;
    u16 heapId;
    u16 *result;
    u16 crc;
    u8 unk2E[0xe];
} EntreeForestBattleData;

typedef struct {
    u16 *result;
    u16 zoneId;
    VecFx32 pos;
    u16 dir;
    u8 area;
    // Whether the comm partner let the player in
    u16 joinResult;
    BOOL playSound;
    u32 unk20;
} EntreeForestWarpData;

static const VecFx32 data_ov012_0216dac4 = {0x1f8000, 0x20000, 0x168000};

static FieldActor *FieldScriptEnv_FindActor(FieldScriptEnv *env, u16 actorId);
static void func_ov010_0216aee8(FieldScriptEnv *env, EntreeForestPokemon *pokemon);
static PartyPkm *CreateEntreeForestPkmFromActor(FieldScriptEnv *env, FieldActor *actor, HeapID heapId,
                                                EntreeForestPokemon *pokemon);
GameEvent *EventEntreeForestBattleCall_Create(GameSystem *gsys, Field *field, PartyPkm *pkm, EntreeForestPokemon pokemon, u16 *result, HeapID heapId);
GameEventReturnCode func_ov010_0216b004(GameEvent *event, u32 *state, void *work);
static void func_ov010_0216b2e4(const VecFx32 *pos, u32 dir, VecFx32 *dest);
GameEvent *EventEntreeForestWarp_CreateToAnother(GameSystem *gsys, u32 dir, u32 area);
GameEventReturnCode EventEntreeForestWarp_Callback(GameEvent *event, u32 *state, void *work);

BOOL s021C_EntreeForestStartBattle(VM *vm, FieldScriptEnv *env) {
    u16 actorId = ScriptReadAny(vm, env);
    u16 *result = ScriptReadVar(vm, env);
    FieldActor *actor;
    PartyPkm *pkm;
    EntreeForestPokemon pokemon;
    ScriptWork *work;
    GameSystem *gsys;

    if ((actor = FieldScriptEnv_FindActor(env, actorId)) != NULL &&
        (pkm = CreateEntreeForestPkmFromActor(env, actor, HEAPID_GAMEEVENT, &pokemon)) != NULL) {
        work = FieldScriptEnv_GetScriptWork(env);
        gsys = FieldScriptEnv_GetGameSystem(env);
        ScriptWork_CallEvent(work, EventEntreeForestBattleCall_Create(gsys, GSYS_GetField(gsys), pkm, pokemon, result,
                                                                      HEAPID_GAMEEVENT));
    }
    return TRUE;
}

BOOL s021B_EntreeForestSpawnAllPkm(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    Field *field = GSYS_GetField(gsys);
    GSYS_GetGameCommSystem(gsys);
    u32 actorIdBase;
    SymbolMapList *list = func_ov010_021664cc(FieldScriptEnv_GetHeapID(env), gsys, &actorIdBase);

    if (list != NULL) {
        EntreeForest_SpawnAllPokemon(field, actorIdBase, list->pokemon, list->count, list->unk50_18);
        GFL_HeapFree(list);
    }
    return FALSE;
}

void func_ov010_0216af3c(FieldScriptEnv *env);
BOOL func_ov010_0216ac90(VM *vm, FieldScriptEnv *env) {
    u16 actorId = ScriptReadAny(vm, env);
    AreaNPCSave *npcData = getAreaNPCData(GameData_GetSaveControl(FieldScriptEnv_GetGameData(env)));
    FieldActor *actor = FieldScriptEnv_FindActor(env, actorId);

    if (actor != NULL) {
        func_0200e18c(npcData, GetActorUserParam0(actor));
        func_ov010_0216af3c(env);
    }
    return FALSE;
}



BOOL func_ov010_0216accc(VM *vm, FieldScriptEnv *env) {
    u16 actorId = ScriptReadAny(vm, env);
    u16 *result = ScriptReadVar(vm, env);
    AreaNPCSave *npcData = getAreaNPCData(GameData_GetSaveControl(FieldScriptEnv_GetGameData(env)));
    FieldActor *actor = FieldScriptEnv_FindActor(env, actorId);

    if (actor != NULL) {
        *result = func_ov010_0216628c(npcData, GetActorUserParam0(actor));
        func_ov010_0216af3c(env);
    } else {
        *result = FALSE;
    }
    return FALSE;
}

BOOL s021A_WordSetLoadEntreeForestPkmName(VM *vm, FieldScriptEnv *env) {
    u16 actorId = ScriptReadAny(vm, env);
    u16 index = ScriptReadAny(vm, env);
    FieldActor *actor;
    PartyPkm *pkm;
    EntreeForestPokemon pokemon;

    if ((actor = FieldScriptEnv_FindActor(env, actorId)) != NULL &&
        (pkm = CreateEntreeForestPkmFromActor(env, actor, FieldScriptEnv_GetHeapID(env), &pokemon)) != NULL) {
        setPartyPokemonSpeciesNameToStrbuf(ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env)), index, pkm);
        GFL_HeapFree(pkm);
    }
    return FALSE;
}

BOOL func_ov010_0216ad70(VM *vm, FieldScriptEnv *env) {
    u16 mode = ScriptReadAny(vm, env);
    u16 *result = ScriptReadVar(vm, env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    GameCommSys *commSystem;
    EntreeForestPokemon speciesPokemon;
    EntreeForestPokemon formPokemon;

    getAreaNPCData(GameData_GetSaveControl(gameData));
    commSystem = GSYS_GetGameCommSystem(gsys);
    switch (mode) {
    case 0:
        if (func_ov011_021767a0(commSystem, gameData) == 4) {
            *result = TRUE;
        } else {
            *result = FALSE;
        }
        break;
    case 1:
        *result = func_ov010_02166618(GameData_Is30FPSMode(gameData));
        break;
    case 2:
        *result = func_ov010_0216662c(GameData_Is30FPSMode(gameData));
        break;
    case 3:
        func_ov010_0216aee8(env, &speciesPokemon);
        *result = speciesPokemon.species;
        break;
    case 4:
        func_ov010_0216aee8(env, &formPokemon);
        *result = formPokemon.form;
        break;
    }
    return FALSE;
}

BOOL s0217_MapChangeEntreeForest(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    u16 dir = ScriptReadAny(vm, env);
    u32 area = GameData_Is30FPSMode(gameData);
    GameEvent *event;
    Field *field;
    FieldPlayer *player;
    VecFx32 pos;
    VecFx32 playerPos;
    GameCommSys *commSystem;

    if (dir == 1 && func_ov010_0216662c(area)) {
        field = GSYS_GetField(gsys);
        player = Field_GetPlayer(field);
        commSystem = GSYS_GetGameCommSystem(gsys);
        FieldPlayer_GetWPos(player, &playerPos);
        func_ov011_021767f0(commSystem, gameData, &pos);
        pos.x += playerPos.x - FX32_CONST(248);
        event = EventMapChangeWarp_CreateGrid(gsys, field, 0x117, &pos, 1, 0);
    } else {
        if (dir != ENTREE_FOREST_DIR_EXIT) {
            area = func_ov010_021665f0(gsys, GameData_Is30FPSMode(gameData), dir);
        }
        event = EventEntreeForestWarp_CreateToAnother(gsys, dir, area);
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}



static FieldActor *FieldScriptEnv_FindActor(FieldScriptEnv *env, u16 actorId) {
    return FindFieldActor(Field_GetActorSystem(GSYS_GetField(FieldScriptEnv_GetGameSystem(env))), actorId);
}

static void func_ov010_0216aee8(FieldScriptEnv *env, EntreeForestPokemon *pokemon) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    u16 actorId = ScriptWork_ResolveHybridValue(work, FieldScriptEnv_GetGameData(env), VAR_TALK_ACTOR);

    GetEntreeForestActorParamBits(&pokemon->raw, FieldScriptEnv_FindActor(env, actorId));
}

static PartyPkm *CreateEntreeForestPkmFromActor(FieldScriptEnv *env, FieldActor *actor, HeapID heapId,
                                                EntreeForestPokemon *pokemon) {
    GameData *gameData = FieldScriptEnv_GetGameData(env);

    GetEntreeForestActorParamBits(&pokemon->raw, actor);
    // The heap passed is not used
    return func_ov018_0217c2a0(HEAPID_GAMEEVENT, gameData, pokemon);
}

void func_ov010_0216af3c(FieldScriptEnv *env) {
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    IntrudeComm *intrude;

    getAreaNPCData(GameData_GetSaveControl(gameData));
    intrude = func_ov010_02158344(GSYS_GetGameCommSystem(gsys));
    if (intrude != NULL) {
        func_ov011_0217688c(intrude, GameData_Is30FPSMode(gameData));
    }
}

GameEvent *EventEntreeForestBattleCall_Create(GameSystem *gsys, Field *field, PartyPkm *pkm,
                                                     EntreeForestPokemon pokemon, u16 *result, HeapID heapId) {
    GameData *gameData = GSYS_GetGameData(gsys);
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov010_0216b004,
                                        sizeof(EntreeForestBattleData));
    EntreeForestBattleData *data = GameEvent_GetData(event);

    MI_CpuFill8(data, 0, sizeof(EntreeForestBattleData));
    data->field = field;
    data->result = result;
    data->heapId = heapId;
    data->pkm = pkm;
    data->pokemon.raw = pokemon.raw;
    data->crc = getCRC16(pkm, PokeParty_GetPkmRawSize());
    if (func_0200e2bc(GameData_GetSaveControl(gameData), &pokemon.raw, heapId, FALSE) == FALSE) {
        data->pokemon.raw = (data->pokemon.raw & 0xfffff800) | 0x28b;
    }
    SaveBtlFieldStatus(&data->status, gameData, field);
    return event;
}

// The battle against a Pokémon of the forest: the catch screen of the Dream Ball, then back to the field
GameEventReturnCode func_ov010_0216b004(GameEvent *event, u32 *state, void *work) {
    EntreeForestBattleData *data = work;
    EntreeForestPokemon check1;
    EntreeForestPokemon check5;
    TrainerDataSave *trainerData;
    BagSave *bag;
    PlayerInfo *playerInfo;
    SaveControl *save;
    GameSystem *gsys = GameEvent_GetGameSystem(event);
    GameData *gameData = GSYS_GetGameData(gsys);
    BOOL caught;
    u16 heapId1;
    PartyPkm *pkm1;
    u16 heapId5;
    PartyPkm *pkm5;

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, EventBattleBGMPlay_Create(gsys, 0x468));
        (*state)++;
        break;
    case 1:
        pkm1 = data->pkm;
        heapId1 = data->heapId;
        check1.species = PokeParty_GetParam(pkm1, PKM_PARAM_SPECIES, NULL);
        check1.form = PokeParty_GetParam(pkm1, PKM_PARAM_FORM, NULL);
        if (func_0200e2bc(GameData_GetSaveControl(gameData), &check1.raw, heapId1, TRUE) == FALSE) {
            data->pokemon.form = 12;
        }
        EncEff_StartEvent(Field_GetEncEff(data->field), event, 0);
        (*state)++;
        break;
    case 2:
        (*state)++;
        break;
    case 3:
        GameEvent_ChainNext(event, CreateFieldCloseEvent(gsys, data->field));
        (*state)++;
        break;
    case 4:
        save = GameData_GetSaveControl(gameData);
        BagSave_AddItem(GameData_GetBag(gameData), ITEM_DREAM_BALL, 1, data->heapId);
        GFL_OvlLoad(OVERLAY_ID(98));
        playerInfo = GetGameDataPlayerInfo(gameData);
        bag = GameData_GetBag(gameData);
        trainerData = getTrainerDataBlkAddress(save);
        data->pdcWork = func_ov098_021b6100(gameData, data->pkm, &data->status, playerInfo, bag, trainerData,
                                            func_0200ca10(save), data->heapId);
        GSYS_QueueProcAsEvent(event, -1, &data_ov098_021b6d74, data->pdcWork);
        (*state)++;
        break;
    case 5:
        caught = func_ov098_021b6158(data->pdcWork);
        pkm5 = data->pkm;
        heapId5 = data->heapId;
        check5.species = PokeParty_GetParam(pkm5, PKM_PARAM_SPECIES, NULL);
        check5.form = PokeParty_GetParam(pkm5, PKM_PARAM_FORM, NULL);
        if (func_0200e2bc(GameData_GetSaveControl(gameData), &check5.raw, heapId5, TRUE) == FALSE ||
            func_0200e2bc(GameData_GetSaveControl(gameData), &data->pokemon.raw, data->heapId, FALSE) == FALSE) {
            caught = FALSE;
        }
        if (caught == TRUE) {
            *data->result = TRUE;
        } else {
            *data->result = FALSE;
        }
        GFL_OvlUnload(OVERLAY_ID(98));
        GFL_OvlLoad(OVERLAY_ID(227));
        data->returnWork = func_ov227_021b95a0(gameData, caught, data->pkm, data->heapId);
        GSYS_QueueProcAsEvent(event, -1, &data_ov227_021b98a4, data->returnWork);
        BagSave_SubItem(GameData_GetBag(gameData), ITEM_DREAM_BALL, 1, data->heapId);
        (*state)++;
        break;
    case 6:
        func_ov227_021b95c8(data->returnWork);
        GFL_OvlUnload(OVERLAY_ID(227));
        GameEvent_ChainNext(event, EventBGMFadePop_Create(gsys));
        (*state)++;
        break;
    case 7:
        GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(gsys));
        (*state)++;
        break;
    case 8:
        GameEvent_ChainNext(event, EventBGMFadeWait_Create(gsys));
        (*state)++;
        break;
    default:
        GFL_HeapFree(data->pkm);
        GFL_HeapFree(data->pdcWork);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


// Asks the comm partner whose net ID it is to let the player into the forest
typedef struct {
    u16 *result;
    u8 netId;
} EntreeForestJoinData;

static GameEventReturnCode func_ov010_0216b280(GameEvent *event, u32 *state, void *work);
GameEvent *func_ov010_0216b24c(GameSystem *gsys, u16 *result, u8 netId) {
    GameEvent *event;
    EntreeForestJoinData *data;

    GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, func_ov010_0216b280, sizeof(EntreeForestJoinData));
    data = GameEvent_GetData(event);
    MI_CpuFill8(data, 0, sizeof(EntreeForestJoinData));
    data->result = result;
    data->netId = netId;
    return event;
}

static GameEventReturnCode func_ov010_0216b280(GameEvent *event, u32 *state, void *work) {
    EntreeForestJoinData *data = work;
    IntrudeComm *intrude = func_ov010_02158344(GSYS_GetGameCommSystem(GameEvent_GetGameSystem(event)));

    if (intrude == NULL) {
        if (data->result != NULL) {
            *data->result = FALSE;
        }
        return GAMEEVENT_DONE;
    }
    switch (*state) {
    case 0:
        if (func_ov011_02176850(intrude, data->netId) == TRUE) {
            (*state)++;
        }
        break;
    case 1:
        if (func_ov011_02176870(intrude) == TRUE) {
            (*state)++;
        }
        break;
    case 2:
        if (data->result != NULL) {
            *data->result = TRUE;
        }
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}





// Where the player comes out in the area in the direction
static void func_ov010_0216b2e4(const VecFx32 *pos, u32 dir, VecFx32 *dest) {
    *dest = *pos;
    switch (dir) {
    case 0:
        dest->z = FX32_CONST(392);
        break;
    case 1:
        dest->z = FX32_CONST(152);
        break;
    case 2:
        dest->x = FX32_CONST(424);
        break;
    case 3:
        dest->x = FX32_CONST(88);
        break;
    case ENTREE_FOREST_DIR_EXIT:
        break;
    }
}

GameEvent *EventEntreeForestWarp_Create(GameSystem *gsys, u32 mode, const VecFx32 *position, u32 arg3, u32 arg4) {
    GameData *gameData = GSYS_GetGameData(gsys);
    GameEvent *event = GameEvent_Create(gsys, NULL, EventEntreeForestWarp_Callback, sizeof(EntreeForestWarpData));
    EntreeForestWarpData *data = GameEvent_GetData(event);

    MI_CpuFill8(data, 0, sizeof(EntreeForestWarpData));
    data->pos = *position;
    data->dir = arg3;
    data->result = NULL;
    data->area = arg4;
    data->playSound = mode;
    if (func_ov011_021767a0(GSYS_GetGameCommSystem(gsys), gameData) == 4) {
        data->unk20 = 1;
    }
    return event;
}

GameEvent *EventEntreeForestWarp_CreateToAnother(GameSystem *gsys, u32 dir, u32 area) {
    Field *field = GSYS_GetField(gsys);
    VecFx32 pos;

    GSYS_GetGameData(gsys);
    func_ov010_0216b2e4(GetMModelWPosPtr(FieldPlayer_GetActor(Field_GetPlayer(field))), dir, &pos);
    return EventEntreeForestWarp_Create(gsys, dir != ENTREE_FOREST_DIR_EXIT, &pos, dir, area);
}

GameEventReturnCode EventEntreeForestWarp_Callback(GameEvent *event, u32 *state, void *work) {
    EntreeForestWarpData *data = work;
    GameSystem *gsys = GameEvent_GetGameSystem(event);
    GSYS_GetGameCommSystem(gsys);
    GameData *gameData = GSYS_GetGameData(gsys);
    Field *field = GSYS_GetField(gsys);

    switch (*state) {
    case 0:
        DisableAllActorsMovement(Field_GetActorSystem(field));
        if (data->unk20 == 1) {
            *state = 2;
        } else {
            GameEvent_ChainNext(event, func_ov010_0216b24c(gsys, &data->joinResult, data->area));
            (*state)++;
        }
        break;
    case 1:
        if (data->joinResult == FALSE) {
            if (data->result != NULL) {
                *data->result = FALSE;
            }
            *state = 6;
        } else {
            (*state)++;
        }
        break;
    case 2:
        if (data->playSound) {
            GFL_SndSEPlay(SEQ_SE_WARP);
        }
        GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, field, 9, 0));
        (*state)++;
        break;
    case 3:
        data->zoneId = func_ov010_021665d0(gsys, data->area);
        func_02012fa4(gameData, data->area);
        GameEvent_ChainNext(event, EventMapChange_CreateGridDefault(gsys, field, data->zoneId, &data->pos, data->dir));
        (*state)++;
        break;
    case 4:
        if (FldActSys_IsAsyncLoadPending(Field_GetActorSystem(GSYS_GetField(gsys))) != TRUE) {
            GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, field, 9, 0, 0, 0, 0));
            (*state)++;
        }
        break;
    case 5:
        if (data->result != NULL) {
            *data->result = TRUE;
        }
        *state = 6;
        break;
    case 6:
        EnableAllActorsMovement(Field_GetActorSystem(field));
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


