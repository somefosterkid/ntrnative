// Black's code at ov037 0x021f36c0-0x021f3834. No name is known: the module and address.

#include "types.h"
#include "field/calender.h"
#include "field/city_state.h"
#include "field/encounter.h"
#include "field/event_data.h"
#include "field/field_actor_internal.h"
#include "field/field_map.h"
#include "field/field_status.h"
#include "field/player_state.h"
#include "field/pleasure_boat.h"
#include "field/trial_house.h"
#include "field/unity_tower.h"
#include "field/zone.h"
#include "pml/poke_party.h"
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
#include "system/game_system.h"
#include "system/shortcut_util.h"
#include "ov037_021f36c0.h"

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

// The saved states of the gimmicks whose script commands this overlay serves, as the gimmicks' own files lay them
// out. Gimmick slot 2, Nimbasa City's gym: gimmick_roller_coaster.c's RollerCoasterSave
typedef struct {
    fx32 frames[4];
    u8 switches[4];
    // Whether the first ride of cars 1 and 2 has been shown
    u16 introSeen[2];
} RollerCoasterSave;

// Gimmick slot 3, Nacrene City's gym (gym_nacrene.c reads unk_04)
typedef struct {
    BOOL unk_00;
    u8 unk_04;
} Ov037NacreneState;

// Gimmick slot 6, Castelia City's gym: gym_insect.c's Ov038State
typedef struct Ov038State {
    u32 unk_0[8];
    u32 unk_20[10];
    u32 unk_48[7];
} Ov038State;

// Gimmick slot 10, three of ov046_021f36c0.c's Ov046Table
typedef struct Ov046Table {
    u32 unk_00;
    u32 unk_04[2];
} Ov046Table;

void func_ov037_021f36c0(GameSystem *gsys, BOOL seen1, BOOL seen2) {
    GameData *gameData;
    GimmickState *gimmick;
    RollerCoasterSave *save;

    gameData = GSYS_GetGameData(gsys);
    gimmick = GameData_GetGimmickState(gameData);
    save = GimmickState_GetUserData(gimmick, 2);
    if (seen1 != 0) {
        save->introSeen[0] = 1;
    }
    if (seen2 != 0) {
        save->introSeen[1] = 1;
    }
}

void func_ov037_021f36e8(GameSystem *gsys, BOOL flag) {
    GameData *gameData = GSYS_GetGameData(gsys);
    Ov037NacreneState *state = GimmickState_GetUserData(GameData_GetGimmickState(gameData), 3);

    GameData_GetPlayerState(gameData);
    if (!(flag == FALSE)) {
        state->unk_00 = 1;
    } else {
        state->unk_00 = 0;
    }
    if (state->unk_00 != 0) {
        state->unk_04 = 1;
        return;
    }
    state->unk_04 = 0;
}

// Gimmick slot 4 (sp_poke_gimmick.c): three bytes, set from bits 0-2 of flags
void func_ov037_021f3724(GameSystem *gsys, u32 flags) {
    GameData *gameData;
    GimmickState *gimmick;
    u8 *userData;
    u32 i;

    gameData = GSYS_GetGameData(gsys);
    gimmick = GameData_GetGimmickState(gameData);
    userData = GimmickState_GetUserData(gimmick, 4);
    i = 0;
    do {
        userData[i] = 0;
        i = i + 1 & 0xff;
    } while (i < 3);
    if ((flags & 1) != 0) {
        *userData = 1;
    }
    if (((s32)flags >> 1 & 1U) != 0) {
        userData[1] = 1;
    }
    if (((s32)flags >> 2 & 1U) != 0) {
        userData[2] = 1;
    }
}

static const u8 data_ov037_021f3834[12] = { 0, 1, 2, 3, 4, 5, 6, 6, 7, 7, 8, 9 };

static const s8 data_ov037_021f3840[16] = { 0, -1, 1, -1, 2, -1, 3, 4, 5, -1, 6, -1, 7, 8, 9, -1 };

void func_ov037_021f3768(GameSystem *gsys, u16 **vars) {
    GameData *gameData;
    GimmickState *gimmick;
    Ov038State *state;
    u16 *ptr;
    s32 i;
    s32 j;

    gameData = GSYS_GetGameData(gsys);
    gimmick = GameData_GetGimmickState(gameData);
    state = GimmickState_GetUserData(gimmick, 6);
    ptr = func_0200b3a8(gimmick, 6);
    j = 0;
    do {
        if (ptr[j] != 0) {
            state->unk_0[j] = 1;
            *vars[j] = 1;
            i = 0;
            do {
                if (data_ov037_021f3840[i + j * 2] != -1) {
                    state->unk_20[data_ov037_021f3840[i + j * 2]] = 1;
                }
                i = i + 1;
            } while (i < 2);
        }
        j = j + 1;
    } while (j < 8);
    j = 0;
    do {
        i = 0;
        do {
            if (state->unk_20[data_ov037_021f3834[i + j * 2]] == 0)
            break;
            i = i + 1;
        } while (i < 2);
        if (i == 2) {
            state->unk_48[j] = 1;
        }
        j = j + 1;
        if (j >= 6) {
            state->unk_48[6] = 1;
            return;
        }
    } while( TRUE );
}

void func_ov037_021f3800(GameSystem *gsys) {
    Ov046Table *table;
    Ov046Table *second;
    Ov046Table *third;

    table = GimmickState_GetUserData(GameData_GetGimmickState(GSYS_GetGameData(gsys)), 0xAU);
    table[0].unk_00 = 3;
    table[0].unk_04[0] = 0;
    table[0].unk_04[1] = 1;
    second = &table[1];
    table[1].unk_00 = 1;
    second->unk_04[0] = 1;
    second->unk_04[1] = 1;
    third = &table[2];
    table[2].unk_00 = 2;
    third->unk_04[0] = 1;
    third->unk_04[1] = 0;
}
