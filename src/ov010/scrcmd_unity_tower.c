// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/scrcmd_unity_tower.c, ported by tools/port_file.py.
#include "types.h"
#include "field/field_script.h"
#include "field/unity_tower.h"
#include "gfl/overlay.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/vm.h"

// The script commands of the Unity Tower (a descriptive name). Names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#define UNITY_TOWER_COUNTRY_MAX 232
#define UNITY_TOWER_VISITOR_MAX 20

// What overlay 311's floor select gets
typedef struct {
    u8 floor;
    u8 selectedFloor;
    u16 selectedCountry;
    BOOL selected;
    u16 visitorCountries[UNITY_TOWER_VISITOR_MAX];
    // For each country, whether a visitor from it has come
    u8 countryVisited[UNITY_TOWER_COUNTRY_MAX];
    u32 unk118;
    u16 *floorResult;
    u16 *countryResult;
    u16 *selectedResult;
} UnityTowerFloorSelectEvent;

static u8 func_ov010_02166824(const UnityTowerFloor *state, u8 index);
static u8 UnityTowerState_GetParam(const UnityTowerFloor *state, u16 param);
static GameEvent *func_ov010_02166854(GameSystem *gsys, u16 floor, u16 *floorResult, u16 *countryResult,
                                      u16 *selectedResult);
static GameEventReturnCode func_ov010_02166908(GameEvent *event, u32 *state, void *data);

static u8 func_ov010_02166824(const UnityTowerFloor *state, u8 index) {
    if (index >= state->count) {
        return 11;
    }
    return state->trainerClasses[index];
}

static u8 UnityTowerState_GetParam(const UnityTowerFloor *state, u16 param) {
    switch (param) {
    case 0:
        return state->count;
    case 1:
        return state->floor;
    case 2:
        return state->value;
    }
    return 0;
}

static GameEvent *func_ov010_02166854(GameSystem *gsys, u16 floor, u16 *floorResult, u16 *countryResult,
                                      u16 *selectedResult) {
    GameEvent *event =
        GameEvent_Create(gsys, NULL, func_ov010_02166908, sizeof(UnityTowerFloorSelectEvent));
    UnityTowerFloorSelectEvent *work = GameEvent_GetData(event);
    UnityTowerSurveySave *survey;
    int country;
    int i;
    int count;

    work->floor = floor;
    work->selectedResult = selectedResult;
    work->floorResult = floorResult;
    work->countryResult = countryResult;
    survey = getUnityTower_SurveySaveBlkAddrress(GameData_GetSaveControl(GSYS_GetGameData(gsys)));
    for (country = 1; country < UNITY_TOWER_COUNTRY_MAX + 1; country++) {
        i = country - 1;
        if (func_02009794(survey, country)) {
            work->countryVisited[i] = TRUE;
        } else {
            work->countryVisited[i] = FALSE;
        }
    }
    count = func_020095c8(survey);
    if (count > UNITY_TOWER_VISITOR_MAX) {
        count = UNITY_TOWER_VISITOR_MAX;
    }
    for (i = 0; i < count; i++) {
        work->visitorCountries[i] = UnityTowerVisitor_GetCountry(UnityTower_GetVisitor(survey, i));
    }
    for (; i < UNITY_TOWER_VISITOR_MAX; i++) {
        work->visitorCountries[i] = 0;
    }
    return event;
}

extern const GameProcFunctions UN_SELECT_PROC_FUNCTIONS;
extern u32 OVERLAY_218_ID[];
static GameEventReturnCode func_ov010_02166908(GameEvent *event, u32 *state, void *data) {
    UnityTowerFloorSelectEvent *work = GameEvent_GetData(event);

    switch (*state) {
    case 0:
        GSYS_QueueProcAsEvent(event, OVERLAY_ID(218), &UN_SELECT_PROC_FUNCTIONS, work);
        (*state)++;
        break;
    case 1:
        if (work->selected == TRUE) {
            *work->selectedResult = TRUE;
            *work->floorResult = work->selectedFloor;
            *work->countryResult = work->selectedCountry;
        } else {
            *work->selectedResult = FALSE;
            *work->floorResult = 0;
            *work->countryResult = 0;
        }
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


BOOL func_ov010_02166980(VM *vm, FieldScriptEnv *env) {
    const UnityTowerFloor *state;
    u8 index;
    u16 *result;

    FieldScriptEnv_GetScriptWork(env);
    state = GameData_GetUnityTowerSave(GSYS_GetGameData(FieldScriptEnv_GetGameSystem(env)));
    index = VM_Read16(vm);
    result = ScriptReadVar(vm, env);
    *result = func_ov010_02166824(state, index);
    return FALSE;
}

BOOL s01CF_UnityTowerGetStateParam(VM *vm, FieldScriptEnv *env) {
    const UnityTowerFloor *state;
    u16 param;
    u16 *result;

    FieldScriptEnv_GetScriptWork(env);
    state = GameData_GetUnityTowerSave(GSYS_GetGameData(FieldScriptEnv_GetGameSystem(env)));
    param = ScriptReadAny(vm, env);
    result = ScriptReadVar(vm, env);
    *result = UnityTowerState_GetParam(state, param);
    return FALSE;
}

BOOL s01D0_UnityTowerCallFloorSelect(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    u16 floor;
    u16 *floorResult;
    u16 *countryResult;
    u16 *selectedResult;
    GameEvent *event;

    GameData_GetUnityTowerSave(GSYS_GetGameData(gsys));
    floor = ScriptReadAny(vm, env);
    floorResult = ScriptReadVar(vm, env);
    countryResult = ScriptReadVar(vm, env);
    selectedResult = ScriptReadVar(vm, env);
    event = func_ov010_02166854(gsys, floor, floorResult, countryResult, selectedResult);
    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}
