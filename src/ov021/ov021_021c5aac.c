// Black's code at ov021 0x021c5aac-0x021c5c64. No name is known: the module and address.

#include "types.h"
#include "app/name_entry.h"
#include "constants/pokemon.h"
#include "field/calender.h"
#include "field/city_state.h"
#include "field/encounter.h"
#include "field/event_data.h"
#include "field/field_actor_internal.h"
#include "field/field_map.h"
#include "field/field_event.h"
#include "field/field_status.h"
#include "field/fieldmap.h"
#include "field/pc_sound.h"
#include "field/player_state.h"
#include "field/pleasure_boat.h"
#include "field/shortcut_menu.h"
#include "field/trial_house.h"
#include "field/unity_tower.h"
#include "field/zone.h"
#include "gfl/overlay.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/config.h"
#include "save/event_work.h"
#include "save/gimmick_savedata.h"
#include "save/high_link.h"
#include "save/player_info.h"
#include "save/player_savedata.h"
#include "save/playtime.h"
#include "save/records.h"
#include "save/save_control.h"
#include "save/wifi_list.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/shortcut_util.h"

struct StrBuf {
    u16 size;
    u16 length;
    u32 magic;
    u16 chars[1];
};
struct CommPlayerSupport {
    u32 type;
    u32 usedType;
    u32 lastType;
    PlayerInfo supporter;
    PlayerInfo usedSupporter;
};
struct BoxSaveAccessor {
    SaveControl *save;
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

// pokebw2's EventPokeNameEntry, without the name it keeps to compare: Black's sets the nickname without comparing
typedef struct {
    GameSystem *gsys;
    Field *field;
    NameEntryParam *nameEntry;
    PartyPkm *pkm;
    u16 *result;
    BOOL transition;
} EventPokeNameEntry;

GameEventReturnCode func_ov021_021c5b58(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021c5c08(GameEvent *event, u32 *state, void *eventData);

GameEvent *prepareForPokemonNameEntryData(GameSystem *gsys, u16 *result, int slot, BOOL transition) {
    GameData *gameData;
    PokeParty *party;
    GameEvent *event;
    EventPokeNameEntry *work;
    PartyPkm *pkm;
    SaveControl *save;
    TrainerGameInfoSave *gameInfo;
    u32 species;
    u32 form;
    u32 sexAndForm;

    gameData = GSYS_GetGameData(gsys);
    party = GameData_GetParty(gameData);
    if (PokeParty_GetPkmCount(party) <= slot) {
        slot = 0;
    }
    event = GameEvent_Create(gsys, NULL, func_ov021_021c5b58, sizeof(EventPokeNameEntry));
    work = GameEvent_GetData(event);
    work->gsys = gsys;
    work->field = GSYS_GetField(gsys);
    work->result = result;
    work->transition = transition;
    gameData = GSYS_GetGameData(work->gsys);
    party = GameData_GetParty(gameData);
    pkm = PokeParty_GetPkm(party, slot);
    save = GameData_GetSaveControl(gameData);
    gameInfo = getTrainerGameInfoAddress(save);
    species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
    form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
    sexAndForm = PokeParty_GetParam(pkm, PKM_PARAM_SEX, NULL);
    sexAndForm = form | (sexAndForm << 8);
    work->nameEntry = setupNameEntry(HEAPID_GAMEEVENT, NAME_ENTRY_POKEMON, species, sexAndForm, 10, NULL, gameInfo);
    work->pkm = pkm;
    return event;
}

GameEventReturnCode func_ov021_021c5b58(GameEvent *event, u32 *state, void *data) {
    EventPokeNameEntry *work = data;
    GameEvent *next;

    switch (*state) {
    case 0:
        next = func_ov021_021c5a28(work->gsys, work->field, work->nameEntry, work->transition);
        GameEvent_ChainNext(event, next);
        (*state)++;
        break;
    case 1:
        if (work->nameEntry->unk1C != 1) {
            PokeParty_SetParam(work->pkm, PKM_PARAM_NICKNAME, (uintptr_t)work->nameEntry->name);
        }
        *work->result = (work->nameEntry->unk1C != 1);
        if (*work->result == 1) {
            RecordAddOne(GameData_GetRecords(GSYS_GetGameData(work->gsys)), 0x1e);
        }
        func_ov010_0216c0c0(work->nameEntry);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

// The hall of fame PC's event: from gameData on, the process's parameters (the option it opens with, then the
// selection it returns)
typedef struct DendouPcEventData {
    GameSystem *gsys;
    Field *field;
    GameData *gameData;
    u16 option;
    u16 selection;
    u16 *result;
} DendouPcEventData;

GameEvent *func_ov021_021c5bd0(GameSystem *gsys, u16 option, u16 *result) {
    GameEvent *event;
    DendouPcEventData *data;

    event = GameEvent_Create(gsys, NULL, func_ov021_021c5c08, sizeof(DendouPcEventData));
    data = GameEvent_GetData(event);
    data->gsys = gsys;
    data->field = GSYS_GetField(gsys);
    data->result = result;
    data->gameData = GSYS_GetGameData(gsys);
    data->option = option;
    return event;
}

extern u32 OVERLAY_171_ID[];

GameEventReturnCode func_ov021_021c5c08(GameEvent *event, u32 *state, void *eventData) {
    DendouPcEventData *data = eventData;
    GameEvent *next;

    switch (*state) {
    case 0:
        next = EventFieldSubprocessTransition_Create(data->gsys, data->field, OVERLAY_ID(171),
                                                     &DENDOU_PC_PROC_FUNCTIONS, &data->gameData);
        GameEvent_ChainNext(event, next);
        (*state)++;
        break;
    case 1:
        switch (data->selection) {
        case 0:
            *data->result = 0;
            break;
        case 1:
            *data->result = 1;
            break;
        }
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

