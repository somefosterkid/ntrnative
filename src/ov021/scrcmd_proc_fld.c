// Black's code at ov021 0x021b4d14-0x021b5008. The name is pokebw2's file between scrcmd_net.c and scrcmd_encount.c (17
// functions there, 8 here).

#include "types.h"
#include "app/geonet.h"
#include "field/calender.h"
#include "field/city_state.h"
#include "field/encounter.h"
#include "field/event_cgear_poweron.h"
#include "field/event_data.h"
#include "field/event_game_clear.h"
#include "field/event_phrase_input.h"
#include "field/field_actor_internal.h"
#include "field/field_map.h"
#include "field/field_script.h"
#include "field/field_status.h"
#include "field/pc_sound.h"
#include "field/player_state.h"
#include "field/pleasure_boat.h"
#include "field/trial_house.h"
#include "field/unity_tower.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/box.h"
#include "save/c_gear_savedata.h"
#include "save/config.h"
#include "save/event_work.h"
#include "save/gimmick_savedata.h"
#include "save/high_link.h"
#include "save/player_info.h"
#include "save/player_savedata.h"
#include "save/playtime.h"
#include "save/save_control.h"
#include "save/wifi_list.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/shortcut_util.h"
#include "system/vm.h"
#include "field/field_event.h"
#include "save/trainer_data_savedata.h"
#include "save/pokedex.h"
#include "app/box2.h"

#define BOX2_MODE_BATTLE_BOX 4

// What the PC Box's callback stores its result from: pokebw2's PCCallWork
typedef struct {
    GameData *gameData;
    u16 *result;
    Box2Param *param;
} PCCallWork;

struct BoxSaveAccessor {
    SaveControl *save;
};
struct CommPlayerSupport {
    u32 type;
    u32 usedType;
    u32 lastType;
    PlayerInfo supporter;
    PlayerInfo usedSupporter;
};
struct GameData {
    SaveControl *saveControl;
    PlayerState playerStates[5];
    EventData *eventData;
    PlayerSave *playerSave;
    ZoneSpawnInfo *nowSpawn;
    ZoneSpawnInfo *outboundWarpSpawn;
    ZoneSpawnInfo *nextSpawn;
    ZoneSpawnInfo *escapeRopeSpawn;
    ZoneSpawnInfo entralinkParentSpawn;
    void *bagCursor;
    BagSave *bag;
    PokeParty *party;
    Config *config;
    BoxSaveAccessor *boxes;
    WifiList *wifiList;
    BGMInfo *bgmInfo;
    MMSys *mmSys;
    EventWork *eventWork;
    MapMatrix *mapMatrix;
    FieldSound *fieldSound;
    FieldStatus *fieldStatus;
    u32 lastBtlResult;
    u32 unk1C0;
    u8 season;
    u8 weather;
    u8 unk1C6;
    u8 unk1C7;
    u8 counter;
    u8 counterEnabled;
    u8 lastSubscreen;
    u8 netId;
    u8 forceSeasonSync;
    u8 savedCGearPowerRequest;
    u8 saving;
    u8 is30FPSMode;
    u8 seasons[4];
    PassPowerLevel levels[4];
    CityState cityStates[2];
    EncountState *encountState;
    ShortcutMenuPos shortcutMenuPos;
    GimmickState gimmickState;
    PleasureBoat *pleasureBoat;
    CommPlayerSupport *commPlayerSupport;
    u32 unk688;
    void *unk68C;
    Calendar *calendar;
    UnityTowerFloor unityTowerFloor;
    TrialHouseWork *trialHouseWork;
    u32 unk6A8;
    PlayTime *playTime;
};
struct BGMInfo {
    u8 count;
    u16 *bgms;
    u8 *subsystems;
};
struct GameEvent {
    GameEvent *parent;
    GameEventCallback callback;
    u32 state;
    void *data;
    GameSystem *gsys;
};

void func_ov021_021b4d14(void *data) {
    PCCallWork *work = data;
    SaveControl *save;
    BattleBoxSave *battleBox;
    u16 flag;

    if (work->param->mode == BOX2_MODE_BATTLE_BOX) {
        save = GameData_GetSaveControl(work->gameData);
        battleBox = getBattleBox(save);
        copySelectedPkmToBattleBlk(battleBox, work->param->party);
    }
    if (work->param->mode == BOX2_MODE_BATTLE_BOX) {
        GFL_HeapFree(work->param->party);
    }
    if ((work->param->unk28 == 0) || (work->param->unk28 != 1)) {
        flag = 0;
    }
    else {
        flag = 1;
    }
    *work->result = flag;
    GFL_HeapFree(work->param);
}

BOOL func_ov021_021b4d60(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameData *gameData = GSYS_GetGameData(gsys);
    SaveControl *save = GameData_GetSaveControl(gameData);
    PokeDexSave *pokedex = GameData_GetPokedex(gameData);
    Field *field = GSYS_GetField(gsys);
    u16 *result = ScriptReadVar(vm, env);
    u16 mode = ScriptReadAny(vm, env);
    Box2Param *param = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(Box2Param), FALSE, "scrcmd_proc_fld.c", 124);
    PCCallWork *callWork;

    param->gameData = gameData;
    param->boxes = GameData_GetBoxSaveAccessor(gameData);
    param->party = GameData_GetParty(gameData);
    param->bag = GameData_GetBag(gameData);
    param->playerInfo = GetGameDataPlayerInfo(gameData);
    param->trainerData = getTrainerDataBlkAddress(save);
    param->unk1C = PokeDex_IsNationalObtained(pokedex);
    param->mode = mode;
    param->unk14 = 0;
    if (mode == BOX2_MODE_BATTLE_BOX) {
        BattleBoxSave *battleBox = getBattleBox(save);

        param->party = convertBoxedPokeSetToParty(battleBox, HEAPID_GAMEEVENT);
        param->unk14 = func_0200bbfc(battleBox, 3);
    }
    callWork = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(PCCallWork), FALSE, "scrcmd_proc_fld.c", 144);
    callWork->gameData = gameData;
    callWork->result = result;
    callWork->param = param;
    ScriptWork_CallEvent(work, func_02014840(gsys, field, OVERLAY_ID(170), &data_ov170_021ef78c, param, func_ov021_021b4d14, callWork));
    return TRUE;
}


u32 func_ov021_021b4e58(VM *param_1, FieldScriptEnv *param_2) {
    ScriptWork *work;
    GameSystem *gsys;
    Field *field;
    u16 *result;
    GameEvent *event;

    work = FieldScriptEnv_GetScriptWork(param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    field = GSYS_GetField(gsys);
    result = ScriptReadVar(param_1, param_2);
    event = EventPhraseInput_Create(gsys, field, (GameEvent *)0x0, 5, result);
    ScriptWork_CallEvent(work, event);
    return 1;
}

u32 func_ov021_021b4e9c(VM *param_1, FieldScriptEnv *param_2) {
    ScriptWork *work;
    GameSystem *gsys;
    Field *field;
    u16 *result;
    GameEvent *event;

    work = FieldScriptEnv_GetScriptWork(param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    field = GSYS_GetField(gsys);
    result = ScriptReadVar(param_1, param_2);
    event = EventPhraseInput_Create(gsys, field, (GameEvent *)0x0, 6, result);
    ScriptWork_CallEvent(work, event);
    return 1;
}

u32 func_ov021_021b4ee0(VM *param_1, FieldScriptEnv *param_2) {
    ScriptWork *work;
    GameSystem *gsys;
    GameEvent *event;

    work = FieldScriptEnv_GetScriptWork(param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    GSYS_GetField(gsys);
    event = EventGameClear_Create(gsys, (u32)ScriptReadAny(param_1, param_2));
    ScriptWork_SetPostEvent(work, event);
    VM_Halt(param_1);
    return 1;
}

u32 func_ov021_021b4f1c(VM *param_1, FieldScriptEnv *param_2) {
    u16 bootComm;
    GameData *gameData;
    SaveControl *save;
    CGearSettings *settings;
    ScriptWork *work;
    GameSystem *gsys;
    GameEvent *event;

    gameData = FieldScriptEnv_GetGameData(param_2);
    save = GameData_GetSaveControl(gameData);
    settings = func_02009210(save);
    bootComm = ScriptReadAny(param_1, param_2);
    work = FieldScriptEnv_GetScriptWork(param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    func_020090d8(settings, 1);
    event = EventCGearPowerOn_Create(gsys, (u32)bootComm);
    ScriptWork_CallEvent(work, event);
    return 1;
}

BOOL func_ov021_021b4f68(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    Field *field = GSYS_GetField(gsys);

    ScriptWork_CallEvent(work, EventFieldSubprocessTransition_Create(gsys, field, OVERLAY_ID(219), &data_ov219_02185f0c, GSYS_GetGameData(gsys)));
    return TRUE;
}


u32 func_ov021_021b4fb0(VM *param_1, FieldScriptEnv *param_2) {
    u16 option;
    ScriptWork *work;
    GameSystem *gsys;
    GameData *gameData;
    u16 *result;
    GameEvent *event;

    work = FieldScriptEnv_GetScriptWork(param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    gameData = GSYS_GetGameData(gsys);
    GameData_GetSaveControl(gameData);
    GSYS_GetField(gsys);
    option = ScriptReadAny(param_1, param_2);
    result = ScriptReadVar(param_1, param_2);
    if ((option == 0) || (option != 1)) {
        option = 0;
    }
    else {
        option = 1;
    }
    event = func_ov021_021c5bd0(gsys, option, result);
    ScriptWork_CallEvent(work, event);
    return 1;
}
