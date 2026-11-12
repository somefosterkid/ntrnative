// Black's code at main 0x02029488-0x02029cf0. The name is pokebw2's file between field_sound_system.c and field_sound.c
// (35 functions there, 35 here).

#include "types.h"
#include "constants/sound.h"
#include "field/calender.h"
#include "field/city_state.h"
#include "field/encounter.h"
#include "field/event_data.h"
#include "field/event_sound.h"
#include "field/field_actor_internal.h"
#include "field/field_map.h"
#include "field/field_sound.h"
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
struct GameEvent {
    GameEvent *parent;
    GameEventCallback callback;
    u32 state;
    void *data;
    GameSystem *gsys;
};
struct BGMInfo {
    u8 count;
    u16 *bgms;
    u8 *subsystems;
};

// The work of every event in this file (pokebw2's EventSoundWork); each uses only some of it
typedef struct {
    GameSystem *gsys;
    FieldSound *fieldSound;
    u32 bgm;
    u16 fadeInFrames;
    u16 fadeOutFrames;
} EventSoundWork;

GameEventReturnCode func_020294f4(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_02029560(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_020295d8(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventBGMPop_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_02029868(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_02029938(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventBGMPlayEx_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_02029aa8(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventBGMFadeStop_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_02029b5c(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_02029c38(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_02029c94(GameEvent *event, u32 *state, void *data);


GameEventReturnCode func_02029488(GameEvent *event, u32 *state, void *data) {
    EventSoundWork *work = data;
    FieldSound *fieldSound = work->fieldSound;

    switch (*state) {
    case 0:
        FieldSnd_SendRequest(fieldSound, FIELD_SND_FADE_IN, 0, 0, work->fadeInFrames);
        (*state)++;
        break;
    case 1:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *EventBGMFadeIn_Create(GameSystem *gsys, u16 fadeInFrames) {
    GameData *gameData;
    GameEvent *event;
    EventSoundWork *work;

    gameData = GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, func_02029488, sizeof(EventSoundWork));
    work = GameEvent_GetData(event);
    work->fieldSound = GameData_GetFieldSoundSystem(gameData);
    work->fadeInFrames = fadeInFrames;
    return event;
}

GameEventReturnCode func_020294f4(GameEvent *event, u32 *state, void *data) {
    EventSoundWork *work = data;
    FieldSound *fieldSound = work->fieldSound;

    switch (*state) {
    case 0:
        FieldSnd_SendRequest(fieldSound, FIELD_SND_FADE_OUT, 0, work->fadeOutFrames, 0);
        (*state)++;
        break;
    case 1:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *EventBGMFadeOut_Create(GameSystem *gsys, u16 fadeOutFrames) {
    GameData *gameData;
    GameEvent *event;
    EventSoundWork *work;

    gameData = GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, func_020294f4, sizeof(EventSoundWork));
    work = GameEvent_GetData(event);
    work->fieldSound = GameData_GetFieldSoundSystem(gameData);
    work->fadeOutFrames = fadeOutFrames;
    return event;
}

GameEventReturnCode func_02029560(GameEvent *event, u32 *state, void *data) {
    EventSoundWork *work = data;
    FieldSound *fieldSound = work->fieldSound;

    switch (*state) {
    case 0:
        FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_PUSH, 0, work->fadeOutFrames, 0);
        (*state)++;
        break;
    case 1:
        if (!FieldSnd_IsBusy(fieldSound)) {
            (*state)++;
        }
        break;
    case 2:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *EventBGMPushWait_Create(GameSystem *gsys, u32 fadeOutFrames) {
    GameData *gameData;
    GameEvent *event;
    EventSoundWork *work;

    gameData = GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, func_02029560, sizeof(EventSoundWork));
    work = GameEvent_GetData(event);
    work->fieldSound = GameData_GetFieldSoundSystem(gameData);
    work->fadeOutFrames = fadeOutFrames;
    return event;
}

GameEventReturnCode func_020295d8(GameEvent *event, u32 *state, void *data) {
    EventSoundWork *work = data;
    FieldSound *fieldSound = work->fieldSound;

    switch (*state) {
    case 0:
        FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_POP, 0, work->fadeOutFrames, work->fadeInFrames);
        (*state)++;
        break;
    case 1:
        if (!FieldSnd_IsBusy(fieldSound)) {
            (*state)++;
        }
        break;
    case 2:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *EventPushBGMFinish_Create(GameSystem *gsys, u32 fadeOutFrames, u32 fadeInFrames) {
    GameData *gameData;
    GameEvent *event;
    EventSoundWork *work;

    gameData = GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, func_020295d8, sizeof(EventSoundWork));
    work = GameEvent_GetData(event);
    work->fieldSound = GameData_GetFieldSoundSystem(gameData);
    work->fadeInFrames = fadeInFrames;
    work->fadeOutFrames = fadeOutFrames;
    return event;
}

GameEventReturnCode EventBGMPop_Callback(GameEvent *event, u32 *state, void *data) {
    EventSoundWork *work = data;
    FieldSound *fieldSound = work->fieldSound;

    switch (*state) {
    case 0:
        FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_POP, 0, work->fadeOutFrames, work->fadeInFrames);
        (*state)++;
        break;
    case 1:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *EventBGMPop_CreateEx(GameSystem *gsys, u32 fadeOutFrames, u32 fadeInFrames) {
    GameData *gameData;
    GameEvent *event;
    EventSoundWork *work;

    gameData = GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, EventBGMPop_Callback, sizeof(EventSoundWork));
    work = GameEvent_GetData(event);
    work->fieldSound = GameData_GetFieldSoundSystem(gameData);
    work->fadeInFrames = fadeInFrames;
    work->fadeOutFrames = fadeOutFrames;
    return event;
}

GameEventReturnCode func_020296c8(GameEvent *event, u32 *state, void *data) {
    EventSoundWork *work = data;
    FieldSound *fieldSound = work->fieldSound;
    s32 depth;
    s32 i;

    switch (*state) {
    case 0:
        depth = FieldSnd_BGMGetStackIndex(fieldSound);
        if (depth > 0) {
            // Pops all but the last at once, and fades in the BGM under them
            for (i = 0; i < depth - 1; i++) {
                FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_POP, 0, 0, 0);
            }
            FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_POP, 0, 0, work->fadeInFrames);
        }
        (*state)++;
        break;
    case 1:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *EventBGMPopAll_Create(GameSystem *gsys, u32 fadeInFrames) {
    GameData *gameData;
    GameEvent *event;
    EventSoundWork *work;

    gameData = GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, func_020296c8, sizeof(EventSoundWork));
    work = GameEvent_GetData(event);
    work->fieldSound = GameData_GetFieldSoundSystem(gameData);
    work->fadeInFrames = fadeInFrames;
    return event;
}

GameEventReturnCode func_02029760(GameEvent *event, u32 *state, void *data) {
    EventSoundWork *work = data;
    FieldSound *fieldSound = work->fieldSound;

    switch (*state) {
    case 0:
        FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_PUSH, 0, work->fadeOutFrames, 0);
        FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_CHANGE, work->bgm, 0, work->fadeInFrames);
        (*state)++;
        break;
    case 1:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *func_020297ac(GameSystem *gsys, u32 bgm, u16 fadeOutFrames, u16 fadeInFrames) {
    GameData *gameData;
    GameEvent *event;
    EventSoundWork *work;

    gameData = GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, func_02029760, sizeof(EventSoundWork));
    work = GameEvent_GetData(event);
    work->fieldSound = GameData_GetFieldSoundSystem(gameData);
    work->bgm = bgm;
    work->fadeInFrames = fadeInFrames;
    work->fadeOutFrames = fadeOutFrames;
    return event;
}

GameEventReturnCode EventMEPlay_Callback(GameEvent *event, u32 *state, void *data) {
    EventSoundWork *work = data;
    FieldSound *fieldSound = work->fieldSound;

    switch (*state) {
    case 0:
        FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_PUSH, 0, 6, 0);
        FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_PLAY, work->bgm, 0, 0);
        (*state)++;
        break;
    case 1:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *EventMEPlay_Create(GameSystem *gsys, u32 meId) {
    GameData *gameData;
    GameEvent *event;
    EventSoundWork *work;

    gameData = GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, EventMEPlay_Callback, sizeof(EventSoundWork));
    work = GameEvent_GetData(event);
    work->fieldSound = GameData_GetFieldSoundSystem(gameData);
    work->bgm = meId;
    work->fadeOutFrames = 0;
    return event;
}

GameEventReturnCode func_02029868(GameEvent *event, u32 *state, void *data) {
    EventSoundWork *work = data;
    FieldSound *fieldSound = work->fieldSound;

    switch (*state) {
    case 0:
        FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_CHANGE, work->bgm, work->fadeOutFrames, work->fadeInFrames);
        (*state)++;
        break;
    case 1:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *EventBGMChange_Create(GameSystem *gsys, u32 bgm, u32 fadeOutFrames, u32 fadeInFrames) {
    GameData *gameData;
    GameEvent *event;
    EventSoundWork *work;

    gameData = GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, func_02029868, sizeof(EventSoundWork));
    work = GameEvent_GetData(event);
    work->fieldSound = GameData_GetFieldSoundSystem(gameData);
    work->bgm = bgm;
    work->fadeInFrames = fadeInFrames;
    work->fadeOutFrames = fadeOutFrames;
    return event;
}

GameEventReturnCode EventBGMFadeStop_Callback(GameEvent *event, u32 *state, void *data) {
    EventSoundWork *work = data;
    GameData *gameData = GSYS_GetGameData(work->gsys);
    FieldSound *fieldSound = work->fieldSound;

    switch (*state) {
    case 0:
        FieldSnd_SendRequest(fieldSound, FIELD_SND_FADE_OUT, 0, work->fadeOutFrames, 0);
        (*state)++;
        break;
    case 1:
        if (!FieldSnd_IsBusy(fieldSound)) {
            (*state)++;
        }
        break;
    case 2:
        FieldSnd_Release(fieldSound, gameData);
        (*state)++;
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEventReturnCode func_02029938(GameEvent *event, u32 *state, void *data) {
    EventSoundWork *work = data;
    FieldSound *fieldSound = work->fieldSound;

    switch (*state) {
    case 0:
        FieldSnd_SendRequest(fieldSound, FIELD_SND_FADE_OUT, 0, 6, 0);
        FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_PLAY, work->bgm, 0, 0);
        (*state)++;
        break;
    case 1:
        if (!FieldSnd_IsBusy(fieldSound)) {
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *EventBGMPlay_Create(GameSystem *gsys, u32 bgm) {
    GameData *gameData;
    GameEvent *event;
    EventSoundWork *work;

    gameData = GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, func_02029938, sizeof(EventSoundWork));
    work = GameEvent_GetData(event);
    work->fieldSound = GameData_GetFieldSoundSystem(gameData);
    work->bgm = bgm;
    return event;
}

GameEventReturnCode EventBGMPlayEx_Callback(GameEvent *event, u32 *state, void *data) {
    EventSoundWork *work = data;
    FieldSound *fieldSound = work->fieldSound;

    switch (*state) {
    case 0:
        FieldSnd_SendRequest(fieldSound, FIELD_SND_FADE_OUT, 0, work->fadeOutFrames, 0);
        FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_PLAY, work->bgm, 0, 0);
        (*state)++;
        break;
    case 1:
        if (!FieldSnd_IsBusy(fieldSound)) {
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *EventBGMPlayEx_Create(GameSystem *gsys, u32 bgm, u32 fadeOutFrames) {
    GameData *gameData;
    GameEvent *event;
    EventSoundWork *work;

    gameData = GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, EventBGMPlayEx_Callback, sizeof(EventSoundWork));
    work = GameEvent_GetData(event);
    work->fieldSound = GameData_GetFieldSoundSystem(gameData);
    work->bgm = bgm;
    work->fadeOutFrames = fadeOutFrames;
    return event;
}

GameEventReturnCode func_02029a3c(GameEvent *event, u32 *state, void *data) {
    EventSoundWork *work = data;
    FieldSound *fieldSound = work->fieldSound;

    if (*state == 0) {
        FieldSnd_SendRequest(fieldSound, FIELD_SND_FADE_OUT, 0, work->fadeOutFrames, 0);
        FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_PLAY, SEQ_BGM_SILENCE_FIELD, 0, 0);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *func_02029a74(GameSystem *gsys, u16 fadeOutFrames) {
    GameData *gameData;
    GameEvent *event;
    EventSoundWork *work;

    gameData = GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, func_02029a3c, sizeof(EventSoundWork));
    work = GameEvent_GetData(event);
    work->fieldSound = GameData_GetFieldSoundSystem(gameData);
    work->fadeOutFrames = fadeOutFrames;
    return event;
}

GameEventReturnCode func_02029aa8(GameEvent *event, u32 *state, void *data) {
    EventSoundWork *work = data;
    FieldSound *fieldSound = work->fieldSound;

    switch (*state) {
    case 0:
        FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_PUSH, 0, 6, 0);
        FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_PLAY, work->bgm, 0, 0);
        (*state)++;
        break;
    case 1:
        if (!FieldSnd_IsBusy(fieldSound)) {
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *EventBGMPlayPush_Create(GameSystem *gsys, u32 bgm) {
    GameData *gameData;
    GameEvent *event;
    EventSoundWork *work;

    gameData = GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, func_02029aa8, sizeof(EventSoundWork));
    work = GameEvent_GetData(event);
    work->fieldSound = GameData_GetFieldSoundSystem(gameData);
    work->bgm = bgm;
    return event;
}

GameEvent *EventBGMFadeStop_Create(GameSystem *gsys, u16 fadeOutFrames) {
    GameData *gameData;
    GameEvent *event;
    EventSoundWork *work;

    gameData = GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, EventBGMFadeStop_Callback, sizeof(EventSoundWork));
    work = GameEvent_GetData(event);
    work->gsys = gsys;
    work->fieldSound = GameData_GetFieldSoundSystem(gameData);
    work->fadeOutFrames = fadeOutFrames;
    return event;
}

GameEventReturnCode func_02029b5c(GameEvent *event, u32 *state, void *data) {
    EventSoundWork *work = data;
    FieldSound *fieldSound = work->fieldSound;

    switch (*state) {
    case 0:
        if (FieldSnd_BGMGetStackIndex(fieldSound) == 0) {
            FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_PUSH, 0, 6, 0);
        } else {
            FieldSnd_SendRequest(fieldSound, FIELD_SND_FADE_OUT, 0, 6, 0);
        }
        (*state)++;
        break;
    case 1:
        if (!FieldSnd_IsBusy(fieldSound)) {
            (*state)++;
        }
        break;
    case 2:
        FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_PLAY, work->bgm, 0, 0);
        (*state)++;
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *EventBattleBGMPlay_Create(GameSystem *gsys, u32 bgm) {
    GameData *gameData;
    GameEvent *event;
    EventSoundWork *work;

    gameData = GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, func_02029b5c, sizeof(EventSoundWork));
    work = GameEvent_GetData(event);
    work->fieldSound = GameData_GetFieldSoundSystem(gameData);
    work->bgm = bgm;
    return event;
}

GameEvent *EventBGMFadePop_Create(GameSystem *gsys) {
    GameData *gameData;
    GameEvent *event;
    EventSoundWork *work;

    gameData = GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, EventBGMPop_Callback, sizeof(EventSoundWork));
    work = GameEvent_GetData(event);
    work->fieldSound = GameData_GetFieldSoundSystem(gameData);
    work->fadeInFrames = 0x3c;
    work->fadeOutFrames = 0x1e;
    return event;
}

GameEventReturnCode func_02029c38(GameEvent *event, u32 *state, void *data) {
    EventSoundWork *work = data;
    FieldSound *fieldSound = work->fieldSound;

    switch (*state) {
    case 0:
        if (!func_02028988(fieldSound)) {
            (*state)++;
        }
        break;
    case 1:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *func_02029c64(GameSystem *gsys) {
    GameData *gameData;
    GameEvent *event;
    EventSoundWork *work;

    gameData = GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, func_02029c38, sizeof(EventSoundWork));
    work = GameEvent_GetData(event);
    work->fieldSound = GameData_GetFieldSoundSystem(gameData);
    return event;
}

GameEventReturnCode func_02029c94(GameEvent *event, u32 *state, void *data) {
    EventSoundWork *work = data;
    FieldSound *fieldSound = work->fieldSound;

    switch (*state) {
    case 0:
        if (func_020289d4(fieldSound) == TRUE) {
            (*state)++;
        }
        break;
    case 1:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *EventBGMFadeWait_Create(GameSystem *gsys) {
    GameData *gameData;
    GameEvent *event;
    EventSoundWork *work;

    gameData = GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, func_02029c94, sizeof(EventSoundWork));
    work = GameEvent_GetData(event);
    work->fieldSound = GameData_GetFieldSoundSystem(gameData);
    return event;
}
