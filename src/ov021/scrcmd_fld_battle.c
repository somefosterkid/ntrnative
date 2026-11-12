// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/scrcmd_fld_battle.c, ported by tools/port_file.py.
// The script commands that check the party and the Battle Box against a battle's regulation, list the Pokémon it
// allows, and let the player pick which team enters. The name is the ROM's own, from GFL_HeapAllocate's file
// argument. Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "battle/regulation.h"
#include "field/field.h"
#include "field/field_script.h"
#include "field/scrcmd_fld_battle.h"
#include "gfl/heap.h"
#include "save/box.h"
#include "save/pokedex.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/vm.h"
#include "system/wordset.h"

GameEvent *func_ov021_021b58b0(GameSystem *gsys, u32 a1, Regulation *regulation, u16 *result, HeapID heapId);

#define REGULATION_SPECIES_COUNT 686
#define REGULATION_LIST_MAX 32

// The result when neither team can enter
#define TEAM_SELECT_NONE 3

typedef struct {
    GameSystem *gsys;
    Field *field;
    void *select;
    PokeParty *party;
    PokeParty *battleBoxParty;
    u8 partyOk;
    u8 battleBoxOk;
    u16 heapId;
    u16 *result;
} TeamSelectEvent;

static u16 func_ov021_021b57cc(GameData *gameData, WordSet *wordSet, u16 regulationId, HeapID heapId);
u16 *func_ov021_021b5824(Regulation *regulation, GameData *gameData, u16 *count, HeapID heapId);
static GameEventReturnCode func_ov021_021b59b0(GameEvent *event, u32 *state, void *data);

// Set the words to the species of the regulation that the player has caught; their count
BOOL func_ov021_021b5700(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    WordSet *wordSet = ScriptWork_GetWordSet(work);
    u16 regulationId = ScriptReadAny(vm, env);
    u16 *result = ScriptReadVar(vm, env);

    *result = func_ov021_021b57cc(gameData, wordSet, regulationId, FieldScriptEnv_GetHeapID(env));
    return FALSE;
}

// Let the player pick the team for a battle under the regulation
BOOL func_ov021_021b574c(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    u16 regulationId = ScriptReadAny(vm, env);
    u16 *result = ScriptReadVar(vm, env);
    HeapID heapId = FieldScriptEnv_GetHeapID(env);
    Regulation *regulation = func_0201a354(regulationId, heapId);
    GameEvent *event = func_ov021_021b58b0(gsys, regulationId == 20 || regulationId == 21 || regulationId == 22,
                                           regulation, result, heapId);

    GFL_HeapFree(regulation);
    if (event == NULL) {
        return FALSE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

static u16 func_ov021_021b57cc(GameData *gameData, WordSet *wordSet, u16 regulationId, HeapID heapId) {
    u16 i;
    u16 *species;
    u16 count = 0;
    Regulation *regulation = func_0201a354(regulationId, heapId);

    species = func_ov021_021b5824(regulation, gameData, &count, heapId);
    for (i = 0; i < count; i++) {
        WordSet_LoadSpeciesName(wordSet, i, species[i]);
    }
    GFL_HeapFree(species);
    GFL_HeapFree(regulation);
    return count;
}

u16 *func_ov021_021b5824(Regulation *regulation, GameData *gameData, u16 *count, HeapID heapId) {
    PokeDexSave *pokedex;
    u16 *list;
    s32 species;
    u16 n;

    *count = 0;
    pokedex = GameData_GetPokedex(gameData);
    list = GFL_HeapAllocate((heapId & 0x7fff) | 0x8000, REGULATION_LIST_MAX * sizeof(u16), TRUE, "scrcmd_fld_battle.c", 172);
    for (species = 0; species < 0x28b; species++) {
        if (regulation->unkA[species / 8] & (1 << (species % 8))) {
            if (PokeDex_IsCaught(pokedex, species)) {
                n = *count;
                if (n >= REGULATION_LIST_MAX) {
                    break;
                }
                *count = n + 1;
                list[n] = species;
            }
        }
    }
    return list;
}

static inline u32 TeamCheck(Regulation *regulation, u32 a1, PokeParty *party, u32 *flags) {
    if (a1 != 0) {
        return func_02019ed4(regulation, party);
    }
    return func_0201a090(regulation, party, flags);
}

static inline GameEvent *CreateTeamSelect(GameSystem *gsys, u32 partyResult, u32 battleBoxResult, PokeParty *party, PokeParty *battleBoxParty, u16 *result, HeapID heapId) {
    GameEvent *event;
    TeamSelectEvent *select;
    u8 partyOk;

    partyOk = FALSE;
    event = GameEvent_Create(gsys, NULL, func_ov021_021b59b0, sizeof(TeamSelectEvent));
    select = GameEvent_GetData(event);
    select->gsys = gsys;
    select->field = GSYS_GetField(gsys);
    select->result = result;
    select->heapId = heapId;
    select->party = party;
    select->battleBoxParty = battleBoxParty;
    if (partyResult != 0) {
        partyOk = TRUE;
    }
    select->partyOk = partyOk;
    select->battleBoxOk = battleBoxResult != 0 ? TRUE : FALSE;
    return event;
}

GameEvent *func_ov021_021b58b0(GameSystem *gsys, u32 a1, Regulation *regulation, u16 *result, HeapID heapId) {
    u32 flags = 0;
    PokeParty *party;
    u32 battleBoxResult;
    GameEvent *event;
    BattleBoxSave *battleBox;
    TeamSelectEvent *data;
    GameData *gameData;
    PokeParty *battleBoxParty;
    u32 partyResult;

    gameData = GSYS_GetGameData(gsys);
    flags = 0;
    party = GameData_GetParty(gameData);
    switch (a1) {
    default:
        partyResult = func_02019ed4(regulation, party);
        break;
    case 0:
        partyResult = func_0201a090(regulation, party, &flags);
        break;
    }
    battleBox = getBattleBox(GameData_GetSaveControl(gameData));
    if (func_0200bba8(battleBox) == TRUE) {
        flags = 0;
        battleBoxParty = convertBoxedPokeSetToParty(battleBox, heapId);
        switch (a1) {
        default:
            battleBoxResult = func_02019ed4(regulation, battleBoxParty);
            break;
        case 0:
            battleBoxResult = func_0201a090(regulation, battleBoxParty, &flags);
            break;
        }
    } else {
        if (partyResult != 0) {
            *result = TEAM_SELECT_NONE;
        } else {
            *result = 0;
        }
        return NULL;
    }

    if (partyResult != 0 && battleBoxResult != 0) {
        if (battleBoxParty != NULL) {
            GFL_HeapFree(battleBoxParty);
        }
        *result = TEAM_SELECT_NONE;
        return NULL;
    }

    event = GameEvent_Create(gsys, NULL, func_ov021_021b59b0, sizeof(TeamSelectEvent));
    data = GameEvent_GetData(event);
    data->gsys = gsys;
    data->field = GSYS_GetField(gsys);
    data->result = result;
    data->heapId = heapId;
    data->party = party;
    data->battleBoxParty = battleBoxParty;
    data->partyOk = partyResult != 0;
    data->battleBoxOk = battleBoxResult != 0;
    return event;
}

static GameEventReturnCode func_ov021_021b59b0(GameEvent *event, u32 *state, void *data) {
    TeamSelectEvent *select = data;
    u32 cancelled;

    switch (*state) {
    case 0:
        select->select = func_ov021_021cc720(select->field, select->party, select->battleBoxParty, select->partyOk,
                                             select->battleBoxOk, select->heapId);
        (*state)++;
        break;
    case 1:
        if (func_ov021_021cc810(select->select)) {
            *select->result = func_ov021_021cc7b4(select->select, &cancelled);
            if (cancelled == TRUE) {
                *select->result = TEAM_SELECT_NONE;
            }
            GFL_HeapFree(select->battleBoxParty);
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}
