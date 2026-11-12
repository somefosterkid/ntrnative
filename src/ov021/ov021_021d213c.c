// Black's code at ov021 0x021d213c-0x021d2278. No name is known: the module and address.

#include "types.h"
#include "field/calender.h"
#include "field/city_state.h"
#include "field/encounter.h"
#include "field/event_data.h"
#include "field/field.h"
#include "field/field_actor_internal.h"
#include "field/field_ambience.h"
#include "field/field_map.h"
#include "field/field_script_event.h"
#include "field/field_script.h"
#include "field/field_status.h"
#include "field/player_state.h"
#include "field/pleasure_boat.h"
#include "field/trial_house.h"
#include "field/unity_tower.h"
#include "field/zone.h"
#include "pml/poke_party.h"
#include "system/pms_word.h"
#include "save/bag.h"
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

// A sound effect and its flag
typedef struct SeFlagEntry {
    u32 unk_0;
    u32 unk_4;
} SeFlagEntry;

const SeFlagEntry data_ov021_021dd614[0x15] = {
#include "asset/ov021_021d213c_data_ov021_021dd614.inc"
};



u32 func_ov021_021d213c(VM *vm, FieldScriptEnv *env) {
    u16 type;
    u16 first;
    u16 second;
    u16 *result;
    u16 lookupA;
    u16 lookupB;

    type = ScriptReadAny(vm, env);
    first = ScriptReadAny(vm, env);
    second = ScriptReadAny(vm, env);
    result = ScriptReadVar(vm, env);
    switch (type) {
    case 1:
        lookupA = 0x5c0;
        lookupB = 0x67b;
        break;
    case 2:
        lookupA = 0x664;
        lookupB = 0x6b7;
        break;
    default:
        *result = 0;
        return 0;
    }
    if (func_020249b0(lookupA, first & 0x7ff) != 0 && func_020249b0(lookupB, second & 0x7ff) != 0) {
        *result = 1;
    } else {
        *result = 0;
    }
    return 0;
}

BOOL FieldAmbience_CheckSoundID(u32 se) {
    u32 i;

    i = 0;
    do {
        if (se == data_ov021_021dd614[i].unk_0) {
            return 1;
        }
        i = i + 1;
    } while (i < 0x15);
    return 0;
}

BOOL FieldAmbience_IsLooped(u32 se) {
    u32 i;

    i = 0;
    do {
        if (se == data_ov021_021dd614[i].unk_0 &&
            data_ov021_021dd614[i].unk_4 == 1) {
            return 1;
        }
        i = i + 1;
    } while (i < 0x15);
    return 0;
}


GameEvent *CheckAbyssalRuinsStepEvent(GameSystem *param_1, Field *param_2) {
    u16 heapId;
    GameData *gameData;
    SaveControl *save;
    PlayerSave *playerSave;
    GameEvent *pGVar2;
    u32 i;
    static const struct {
        u16 unk_0;
        u16 unk_2;
    } table[5] = {
        {0x0000, 0x28dd},
        {0x0064, 0x28de},
        {0x012c, 0x28df},
        {0x01c2, 0x28e0},
        {0x01f4, 0x28e1},
    };

    gameData = GSYS_GetGameData(param_1);
    save = GameData_GetSaveControl(gameData);
    playerSave = SaveControl_GetPlayerSave(save);
    heapId = PlayerSave_GetAbyssalRuinsStepCounter(playerSave);
    i = 0;
    do {
        if (heapId == table[i].unk_0) {
            heapId = Field_GetHeapID(param_2);
            pGVar2 = EventScriptCall_Create
                                                  (param_1, table[i].unk_2, (FieldActor *)0x0, heapId
                                                  );
            return pGVar2;
        }
        i = i + 1;
    } while (i < 5);
    if (500 < heapId) {
        heapId = Field_GetHeapID(param_2);
        pGVar2 = EventScriptCall_Create(param_1, 0x28e1, (FieldActor *)0x0, heapId);
        return pGVar2;
    }
    return NULL;
}
