// Black's code at ov010 0x0216075c-0x021616a4. The name is pokebw2's file between event_shortcut_menu.c and
// event_save.c (37 functions there, 31 here).

#include "types.h"
#include "field/app_call.h"
#include "field/calender.h"
#include "field/city_state.h"
#include "field/encounter.h"
#include "field/event_data.h"
#include "field/event_save.h"
#include "field/field.h"
#include "field/field_actor_internal.h"
#include "field/field_map.h"
#include "field/field_sound.h"
#include "field/field_status.h"
#include "field/mailtool.h"
#include "field/player_state.h"
#include "field/pleasure_boat.h"
#include "field/trial_house.h"
#include "field/unity_tower.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/mi.h"
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
#include "save/trainer_data_savedata.h"
#include "save/wifi_list.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/iss_3ds_sys.h"
#include "system/iss_switch_set.h"
#include "system/shortcut_util.h"
#include "field/itemuse_event.h"
#include "field/player_action.h"
#include "field/trcard_sys.h"
#include "app/pokelist.h"
#include "app/p_status.h"
#include "app/bag.h"
#include "demo/shinka_demo.h"
#include "pml/evolution.h"
#include "pml/item.h"
#include "constants/pokemon.h"
#include "save/pokedex.h"
#include "app/battle_recorder.h"
#include "app/config_panel.h"
#include "app/itemmenu.h"
#include "field/mailsys.h"
#include "app/townmap.h"
#include "app/wifi_note.h"
#include "app/zukan_common.h"
#include "field/field_event.h"
#include "gfl/overlay.h"

struct ISSRoadSys {
    BOOL enabled;
    int volume;
    PlayerState *player;
    VecFx32 lastPos;
};
// The town map's parameters (Black's layout, as ov072's townmap.c has it: the callers declare it themselves)
typedef struct {
    // TOWNMAP_MODE_*
    u32 mode;
    // TOWNMAP_RESULT_*
    u32 result;
    // The zone the player is in, then the zone picked to fly to
    u16 zoneId;
    u16 escapeZoneId;
    // Where to land in the zone picked
    u32 warpX;
    u32 warpZ;
    GameSystem *gsys;
} TownMapParam;
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
typedef struct {
    u16 zoneId;
    u16 padding;
    s16 pitch[4];
    u16 tempo[4];
    s16 pan[4];
} ISSDungeonData;
typedef struct {
    u8 count;
    ISSDungeonData *data;
} ISSDungeonList;
struct ISSDungeonSys {
    HeapID heapId;
    PlayerState *playerState;
    GameData *gameData;
    BOOL enabled;
    u16 zoneId;
    u16 nextZoneId;
    ISSDungeonList *list;
    ISSDungeonData *current;
    ISSDungeonData defaultData;
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
typedef struct {
    u32 bgm;
    u16 trackMask;
} ISS3DSoundConfig;
typedef struct {
    BOOL enabled;
    VecFx32 pos;
    fx32 range;
    s32 volume;
    u16 trackMask;
} ISS3DSoundUnit;
struct ISS3DSoundSys {
    HeapID heapId;
    BOOL enabled;
    u8 targetMasterVolume;
    u8 masterVolume;
    u8 masterVolumeStep;
    VecFx32 listenerPos;
    VecFx32 listenerTarget;
    ISS3DSoundUnit units[ISS_3DS_UNIT_COUNT];
    s32 unitVolume[ISS_3DS_UNIT_COUNT];
    s32 unitPan[ISS_3DS_UNIT_COUNT];
    ISS3DSoundConfig *configs;
    u8 configCount;
    u8 usedConfigId;
};
struct ISSCitySys {
    PlayerState *player;
    BOOL enabled;
    u32 state;
    int emitterVolume;
    int masterVolume;
    ISSCityUnit **units;
    u8 unitCount;
    u8 nowUnitIndex;
};
struct ISSCityUnit {
    u16 zoneId;
    s32 x;
    s32 y;
    s32 z;
    u8 volume[6];
    u8 rangeX[6];
    u8 rangeY[6];
    u8 rangeZ[6];
};
typedef struct {
    u16 zoneId;
    u16 trackMaskIn;
    u16 trackMaskOut;
    u16 fadeTime;
} ISSZoneConfig;
struct ISSZoneSys {
    BOOL isEnabled;
    u8 zoneCount;
    ISSZoneConfig *zones;
    u8 lastZoneDesc;
    u16 trackMaskIn;
    u16 trackMaskOut;
    u16 fadeTime;
    u16 counter;
};
struct ISS {
    HeapID heapId;
    GameData *gameData;
    u32 bgm;
    u32 frame;
    ISSCitySys *citySys;
    ISSRoadSys *roadSys;
    ISSDungeonSys *dungeonSys;
    ISSZoneSys *zoneSys;
    ISSSwitchSys *switchSys;
    ISS3DSoundSys *soundSys3D;
};
struct ISSSwitchSet {
    u32 bgmId;
    u16 zoneIds[9];
    u8 zoneIdCount;
    ISSSwitch *switches[ISS_SWITCH_COUNT];
};
typedef struct {
    BOOL enabled;
    BOOL isUnmute;
    u32 bgmId;
    ISSSwitchIndex switchIndex;
} ISSSwitchMuteStateChangeRequest;
typedef struct {
    BOOL enabled;
    u32 bgmId;
    int targetVolume;
    u16 interval;
} ISSSwitchMasterVolumeChangeRequest;
struct ISSSwitchSys {
    BOOL isEnabled;
    u8 setCount;
    ISSSwitchSet **switchSets;
    ISSSwitchSet *nowSwitchSet;
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


GameEventReturnCode func_ov010_021607e0(GameEvent *event, u32 *state, void *data);
void func_ov010_02160a28(FieldAppCallParam *param, FieldAppCallInput *input, FieldAppCallPredicate canRetry, FieldAppCallPredicate callback1, FieldAppCallPredicate callback2, void *arg);
BOOL func_ov010_02160a4c(FieldAppCallParam *param);
BOOL func_ov010_02160a64(FieldAppCallParam *param);
BOOL func_ov010_02160a7c(FieldAppCallParam *param);
PokeListParam *func_ov010_02160a94(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam);
u32 func_ov010_02160c88(FieldAppCallWork *work, void *data);
ZukanCommonParam *func_ov010_02160d6c(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam);
u32 func_ov010_02160ddc(FieldAppCallWork *work, void *data);
PStatusParam *func_ov010_02160e08(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam);
u32 func_ov010_02160f88(FieldAppCallWork *work, void *data);
BagProcessData *func_ov010_02160fe0(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const PokeListParam *partyParam);
u32 func_ov010_02161070(FieldAppCallWork *work, void *data);
TrainerCardParam *func_ov010_0216117c(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam);
u32 func_ov010_021611e0(FieldAppCallWork *work, void *data);
TownMapParam *func_ov010_02161214(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam);
u32 func_ov010_02161280(FieldAppCallWork *work, void *data);
ConfigPanelParam *func_ov010_02161308(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam);
u32 func_ov010_02161340(FieldAppCallWork *work, void *data);
void func_ov010_02161350(FieldAppCallWork *work, s32 appParam);
WifiNoteParam *func_ov010_02161378(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam);
u32 func_ov010_021613a4(FieldAppCallWork *work, void *data);
MailParam *func_ov010_021613cc(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam);
u32 func_ov010_02161484(FieldAppCallWork *work, MailParam *param);
void func_ov010_02161534(MailParam *param);
ShinkaDemoParam *func_ov010_02161554(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam);
u32 func_ov010_02161610(FieldAppCallWork *work, void *data);
BattleRecorderParam *func_ov010_0216163c(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam);
u32 func_ov010_02161668(FieldAppCallWork *work, void *data);

typedef void *(*FieldProcCreateParam)(FieldAppCallWork *work, s32 appParam, s32 prevAppId, void *prevParam);
typedef u32 (*FieldProcGetResult)(FieldAppCallWork *work, void *param);
typedef void (*FieldProcCall)(FieldAppCallWork *work, s32 appParam);
typedef void (*FieldProcFreeParam)(void *param);

// The apps the field opens, by FIELD_APP_ id (pokebw2's FIELD_PROC_LINK_LIST)
static const FieldProcLink data_ov010_021712cc[12] = {
    { OVERLAY_ID(91), &data_ov091_021bed00, (FieldProcCreateParam)func_ov010_02160a94, func_ov010_02160c88, NULL, func_ov010_0216171c },
    { OVERLAY_ID(212), &data_ov212_021c9938, (FieldProcCreateParam)func_ov010_02160d6c, func_ov010_02160ddc, NULL, func_ov010_0216171c },
    { OVERLAY_ID(70), &BAG_PROC_FUNCTIONS, (FieldProcCreateParam)func_ov010_02160fe0, func_ov010_02161070, NULL, func_ov010_0216171c },
    { OVERLAY_ID(111), &data_ov010_02172614, (FieldProcCreateParam)func_ov010_0216117c, func_ov010_021611e0, NULL, func_ov010_0216171c },
    { 0, NULL, NULL, NULL, func_ov010_02161350, func_ov010_0216171c },
    { OVERLAY_ID(68), &data_ov068_021bb5c8, (FieldProcCreateParam)func_ov010_02161308, func_ov010_02161340, NULL, func_ov010_0216171c },
    { OVERLAY_ID(131), &data_ov131_021dc800, (FieldProcCreateParam)func_ov010_02160e08, func_ov010_02160f88, NULL, func_ov010_0216171c },
    { OVERLAY_ID(72), &data_ov072_021bbb80, (FieldProcCreateParam)func_ov010_02161214, func_ov010_02161280, NULL, func_ov010_0216171c },
    { OVERLAY_ID(128), &data_ov128_021ca658, (FieldProcCreateParam)func_ov010_02161378, func_ov010_021613a4, NULL, func_ov010_0216171c },
    { OVERLAY_NONE, &MAILSYS_PROC_FUNCTIONS, (FieldProcCreateParam)func_ov010_021613cc, (FieldProcGetResult)func_ov010_02161484, NULL, (FieldProcFreeParam)func_ov010_02161534 },
    { OVERLAY_ID(195), &data_ov195_02208c40, (FieldProcCreateParam)func_ov010_02161554, func_ov010_02161610, NULL, func_ov010_0216171c },
    { OVERLAY_ID(187), &data_ov187_0220b294, (FieldProcCreateParam)func_ov010_0216163c, func_ov010_02161668, NULL, func_ov010_0216171c },
};



GameEvent *EventFieldAppCall_Create(FieldAppCallInput *input, u16 code) {
    GameEvent *event = GameEvent_Create(input->gameSystem, input->parent, func_ov010_021607e0, sizeof(FieldAppCallWork));
    FieldAppCallWork *work = GameEvent_GetData(event);

    MI_CpuFill8(work, 0, sizeof(FieldAppCallWork));
    work->code = code;
    work->input = input;
    work->result = FIELD_APP_RESULT_NEXT;
    work->event = event;
    work->appId = input->appId;
    work->prevAppId = input->appId;
    work->nextAppId = input->appId;
    work->unk60 = 0;
    work->unk62 = 0;
    work->unk68 = 0;
    func_ov010_02160a28(&work->params, work->input, work->input->canRetry, work->input->callback1, work->input->callback2, work->input->arg);
    PlayerActionPerms_Create(&work->perms, work->input->gameSystem, work->input->field);
    CalcPlayerActionPossibilities(work->input->field, &work->action);
    return event;
}

GameEventReturnCode func_ov010_021607e0(GameEvent *event, u32 *state, void *data) {
    FieldAppCallWork *work = data;
    void *param;

    switch (*state) {
    case 0:
        *state = 1;
        break;
    case 1:
        if (work->result == 0 || work->result == 1) {
            *state = 2;
        } else if (work->result == FIELD_APP_RESULT_NEXT) {
            if (data_ov010_021712cc[work->appId].call == NULL) {
                *state = 6;
            } else {
                *state = 3;
            }
        } else if (work->result == 3) {
            work->input->eventType = 3;
            *state = 2;
        } else if (work->result == 2) {
            work->input->eventType = 2;
            *state = 2;
        }
        break;
    case 2:
        if (work->result == 0) {
            work->input->eventType = 0;
        } else if (work->result == 1) {
            work->input->eventType = 1;
        }
        return GAMEEVENT_DONE;
    case 3:
        if (func_ov010_02160a64(&work->params)) {
            data_ov010_021712cc[work->appId].call(work, work->input->appParam);
            *state = 4;
        }
        break;
    case 4:
        *state = 5;
        break;
    case 5:
        if (work->result == 0) {
            if (func_ov010_02160a4c(&work->params)) {
                *state = 1;
            }
        } else {
            *state = 1;
        }
        break;
    case 6:
        GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(work->input->gameSystem, work->input->field, 0, 0));
        *state = 7;
        break;
    case 7:
        if (func_ov010_02160a64(&work->params)) {
            GameEvent_ChainNext(event, CreateFieldCloseEvent(work->input->gameSystem, work->input->field));
            *state = 11;
        }
        break;
    case 8:
        func_ov010_02160a7c(&work->params);
        GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(work->input->gameSystem));
        *state = 9;
        break;
    case 9:
        work->input->field = GSYS_GetField(work->input->gameSystem);
        FieldG2D_SetLCDConfig();
        FieldG2D_Prepare3DSurface(work->input->field);
        *state = 10;
        break;
    case 10:
        if (func_ov010_02160a4c(&work->params)) {
            GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(work->input->gameSystem, work->input->field, 0, 0, 0, 0, 0));
            *state = 1;
        }
        break;
    case 11:
        param = data_ov010_021712cc[work->appId].createParam(work, work->input->appParam, work->prevAppId, work->appParam);
        if (work->appParam != NULL) {
            data_ov010_021712cc[work->prevAppId].freeParam(work->appParam);
            work->appParam = NULL;
        }
        work->appParam = param;
        if (data_ov010_021712cc[work->appId].functions != NULL) {
            GSYS_QueueProc(work->input->gameSystem, data_ov010_021712cc[work->appId].overlayId, data_ov010_021712cc[work->appId].functions, param);
            *state = 12;
        }
        break;
    case 12:
        if (GSYS_GetProcMgrState(work->input->gameSystem)) {
            break;
        }
        *state = 13;
        break;
    case 13:
        work->result = data_ov010_021712cc[work->appId].getResult(work, work->appParam);
        if (work->result == 0) {
            work->input->eventType = 0;
        } else if (work->result == 1) {
            work->input->eventType = 1;
        } else if (work->result == 3) {
            work->input->eventType = 3;
        } else if (work->result == 2) {
            work->input->eventType = 2;
        }
        if (work->result == FIELD_APP_RESULT_NEXT) {
            work->prevAppId = work->appId;
            work->appId = work->nextAppId;
            *state = 11;
        } else {
            if (work->appParam != NULL) {
                GFL_HeapFree(work->appParam);
                work->appParam = NULL;
            }
            *state = 8;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}


void func_ov010_02160a28(FieldAppCallParam *param, FieldAppCallInput *input, FieldAppCallPredicate canRetry, FieldAppCallPredicate callback1, FieldAppCallPredicate callback2, void *arg) {
    MI_CpuFill8(param, 0, sizeof(FieldAppCallParam));
    param->canRetry = canRetry;
    param->callback2 = callback2;
    param->callback1 = callback1;
    param->arg = arg;
    param->input = input;
}

BOOL func_ov010_02160a4c(FieldAppCallParam *param) {
    if (param->canRetry != NULL) {
        return param->canRetry(param->input, param->arg);
    }
    return TRUE;
}

BOOL func_ov010_02160a64(FieldAppCallParam *param) {
    if (param->callback1 != NULL) {
        return param->callback1(param->input, param->arg);
    }
    return TRUE;
}

BOOL func_ov010_02160a7c(FieldAppCallParam *param) {
    if (param->callback2 != NULL) {
        return param->callback2(param->input, param->arg);
    }
    return TRUE;
}

PokeListParam *func_ov010_02160a94(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam) {
    GameData *gameData = GSYS_GetGameData(work->input->gameSystem);
    PokeListParam *param = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(PokeListParam), TRUE, "event_field_proclink.c", 0x330);

    param->party = GameData_GetParty(gameData);
    param->action = work->action;
    param->bag = GameData_GetBag(gameData);
    param->unk08 = func_02008fec(gameData);
    param->pokedex = GameData_GetPokedex(gameData);
    param->mode = 0;
    param->index = work->input->partySlot;
    param->move = 0;
    param->learnMask = 0xff;
    param->zoneId = GameData_GetPlayerState(gameData)->zoneId;
    param->unk5F = GameData_GetSeason(gameData);

    if (appParam == 5) {
        param->index = 0;
        param->mode = 5;
        param->item = 0x1d2;
        work->unk6C[0] = 1;
    } else if (prevAppId == FIELD_APP_POKELIST) {
        param->index = 0;
    } else if (prevAppId == FIELD_APP_BAG) {
        const BagProcessData *bag = prevParam;

        param->item = bag->item;
        if (bag->result != 3 && bag->result != 4 && bag->result != 1 && bag->result != 18) {
            param->index = 0;
        }
        switch (bag->result) {
        case 1:
            if (bag->mode == 2) {
                param->mode = 0;
            }
            break;
        case 12:
            param->mode = 16;
            break;
        case 2:
            param->mode = 9;
            break;
        case 3:
        case 18:
            param->mode = 10;
            break;
        case 4:
            param->mode = 10;
            break;
        case 5:
            param->mode = 5;
            break;
        case 6:
            param->mode = 6;
            break;
        default:
            param->mode = 9;
            break;
        }
    } else if (prevAppId == FIELD_APP_POKESTATUS) {
        const PStatusParam *status = prevParam;

        switch (status->result) {
        case 0:
        case 1:
            if (status->mode == 2) {
                if (work->unk68 == 6) {
                    param->mode = 8;
                    param->learnIndex = work->unk64;
                } else {
                    param->mode = 7;
                }
                param->item = work->unk62;
                param->move = status->move;
                if (status->result == 0) {
                    param->moveSlot = status->slot;
                } else {
                    param->moveSlot = 0xff;
                }
            } else {
                param->mode = 0;
                param->index = status->partyIndex;
            }
            break;
        }
    } else if (prevAppId == 9) {
        BOOL mailResult;

        GFL_OvlLoad(OVERLAY_ID(136));
        mailResult = func_ov136_021c3e00((MailParam *)prevParam);
        GFL_OvlUnload(OVERLAY_ID(136));
        if (work->unk68 == 0) {
            param->mode = 11;
            param->index = work->unk60;
            param->item = work->unk62;
        } else if (work->unk68 == 2) {
            if (mailResult == 1) {
                param->mode = 12;
                param->index = work->unk60;
                param->item = work->unk62;
            } else {
                param->mode = 0;
                param->index = work->unk60;
            }
        }
    } else if (prevAppId == 10) {
        param->mode = 5;
        param->item = work->unk62;
    }
    return param;
}


u32 func_ov010_02160c88(FieldAppCallWork *work, void *data) {
    PokeListParam *param = data;

    work->input->partySlot = param->index;
    work->unk60 = param->index;
    if (work->unk6C[0] == 1) {
        if (param->index == 8) {
            return 1;
        }
        return 0;
    }
    switch (param->result) {
    case 0:
        if (param->mode == 6 || work->input->appId == FIELD_APP_BAG) {
            work->nextAppId = FIELD_APP_BAG;
            return FIELD_APP_RESULT_NEXT;
        }
        if (param->index == 8) {
            return 1;
        }
        return 0;
    case 1:
        work->nextAppId = FIELD_APP_POKESTATUS;
        return FIELD_APP_RESULT_NEXT;
    case 3:
        work->nextAppId = FIELD_APP_BAG;
        return FIELD_APP_RESULT_NEXT;
    case 10:
        work->nextAppId = FIELD_APP_BAG;
        return FIELD_APP_RESULT_NEXT;
    case 4:
    case 5:
        work->nextAppId = FIELD_APP_POKESTATUS;
        work->unk62 = param->item;
        work->unk64 = param->learnIndex;
        return FIELD_APP_RESULT_NEXT;
    case 6:
    case 7:
        work->nextAppId = 9;
        return FIELD_APP_RESULT_NEXT;
    case 8:
    case 9:
        work->nextAppId = 10;
        work->unk62 = param->item;
        return FIELD_APP_RESULT_NEXT;
    case 11:
    case 12:
    case 13:
    case 14:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
        work->input->eventId = param->result - 11;
        return 2;
    case 15:
        work->nextAppId = 7;
        return FIELD_APP_RESULT_NEXT;
    }
    return 1;
}





ZukanCommonParam *func_ov010_02160d6c(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam) {
    ZukanCommonParam *param = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(ZukanCommonParam), FALSE, "event_field_proclink.c", 1179);
    FieldSound *sound;

    MI_CpuFill8(param, 0, sizeof(ZukanCommonParam));
    param->gameData = GSYS_GetGameData(work->input->gameSystem);
    param->pokedex = GameData_GetPokedex(param->gameData);
    param->playerInfo = GetGameDataPlayerInfo(param->gameData);
    sound = GameData_GetFieldSoundSystem(param->gameData);
    func_02029dbc(sound, GameSystem_GetISS(work->input->gameSystem));
    if (appParam != -1) {
        param->mode = appParam;
    } else {
        param->mode = 0;
    }
    return param;
}

u32 func_ov010_02160ddc(FieldAppCallWork *work, void *data) {
    ZukanCommonParam *param = data;
    FieldSound *sound = GameData_GetFieldSoundSystem(param->gameData);

    func_02029dd8(sound, GameSystem_GetISS(work->input->gameSystem));
    switch (param->result) {
    case 0:
        return 0;
    }
    return 1;
}

PStatusParam *func_ov010_02160e08(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam) {
    PStatusParam *param = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(PStatusParam), TRUE, "event_field_proclink.c", 1246);
    GameData *gameData = GSYS_GetGameData(work->input->gameSystem);
    SaveControl *save = GameData_GetSaveControl(gameData);
    PokeDexSave *pokedex = GameData_GetPokedex(gameData);
    const PokeListParam *partyParam;
    PokeParty *party;
    PartyPkm *pkm;
    s32 i;

    param->dataType = 1;
    param->gameData = gameData;
    param->forceExit = FALSE;
    param->isNationalDex = PokeDex_IsNationalObtained(pokedex);
    if (prevAppId == FIELD_APP_POKELIST) {
        PokeParty *listParty;
        TrainerDataSave *trainerData;
        partyParam = prevParam;
        listParty = partyParam->party;
        trainerData = partyParam->trainerData;
        param->party = listParty;
        param->trainerData = trainerData;
        param->partyCount = PokeParty_GetPkmCount(listParty);
        param->partyIndex = partyParam->index;
        switch (partyParam->result) {
        case 4:
            param->move = partyParam->move;
            param->mode = 2;
            param->page = 1;
            param->fromFieldMenu = 0;
            work->unk68 = 5;
            break;
        case 5:
            param->move = partyParam->move;
            param->mode = 2;
            param->page = 1;
            param->fromFieldMenu = 0;
            work->unk68 = 6;
            break;
        default:
            param->page = 0;
            param->mode = 0;
            param->fromFieldMenu = 1;
            break;
        }
    } else if (prevAppId == FIELD_APP_POKESTATUS) {
        param->trainerData = getTrainerDataBlkAddress(save);
        param->party = GameData_GetParty(gameData);
        param->partyCount = PokeParty_GetPkmCount(GameData_GetParty(gameData));
        param->partyIndex = 0;
        param->mode = 0;
        param->fromFieldMenu = 1;
        if (appParam != -1) {
            param->page = appParam;
            if (param->page == 2) {
                party = GameData_GetParty(gameData);
                for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
                    pkm = PokeParty_GetPkm(party, i);
                    if (PokeParty_GetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL) && PokeParty_CheckAnyRibbon(pkm) && !PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL)) {
                        param->partyIndex = i;
                        break;
                    }
                }
            } else if (param->page == 1) {
                party = GameData_GetParty(gameData);
                for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
                    pkm = PokeParty_GetPkm(party, i);
                    if (PokeParty_GetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL) && !PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL)) {
                        param->partyIndex = i;
                        break;
                    }
                }
            }
        } else {
            param->page = 0;
        }
    }
    return param;
}


u32 func_ov010_02160f88(FieldAppCallWork *work, void *data) {
    PStatusParam *param = data;

    switch (param->result) {
    case 0:
    case 1:
        if (work->input->appId == FIELD_APP_POKESTATUS) {
            return 0;
        }
        work->nextAppId = FIELD_APP_POKELIST;
        return FIELD_APP_RESULT_NEXT;
    case 2:
        return 1;
    }
    return 1;
}



u32 func_ov010_02160fb4(GameSystem *param_1) {
    u16 zoneId;
    PlayerState *playerState;
    BOOL BVar1;

    playerState = GSYS_GetPlayerState(param_1);
    zoneId = PlayerState_GetZoneID(playerState);
    BVar1 = GetZoneIsUnionRoom(zoneId);
    if (BVar1 == 1) {
        return 1;
    }
    BVar1 = IsZone150Or151(zoneId);
    if (BVar1 == 1) {
        return 3;
    }
    return 0;
}

BagProcessData *func_ov010_02160fe0(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const PokeListParam *partyParam) {
    GameData *gameData = GSYS_GetGameData(work->input->gameSystem);
    BagProcessData *param;

    switch (prevAppId) {
    case FIELD_APP_BAG:
        param = func_0202e474(gameData, &work->perms, func_ov010_02160fb4(work->input->gameSystem), HEAPID_GAMEEVENT);
        if (appParam != -1) {
            func_02008298(param->cursor, appParam);
        }
        break;
    case FIELD_APP_POKELIST:
        if (partyParam->result == 3) {
            param = func_0202e474(gameData, &work->perms, 2, HEAPID_GAMEEVENT);
        } else {
            param = func_0202e474(gameData, &work->perms, func_ov010_02160fb4(work->input->gameSystem), HEAPID_GAMEEVENT);
        }
        break;
    case 1:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    default:
        param = func_0202e474(gameData, &work->perms, func_ov010_02160fb4(work->input->gameSystem), HEAPID_GAMEEVENT);
        break;
    }
    return param;
}

u32 func_ov010_02161070(FieldAppCallWork *work, void *data) {
    BagProcessData *param = data;
    GameData *gameData;

    switch (param->result) {
    case 0:
        return 1;
    case 1:
        if (param->mode == 2) {
            work->nextAppId = FIELD_APP_POKELIST;
            return FIELD_APP_RESULT_NEXT;
        }
        return 0;
    case 12:
        work->nextAppId = FIELD_APP_POKELIST;
        return FIELD_APP_RESULT_NEXT;
    case 10:
    case 11:
        work->input->eventId = 0;
        return 3;
    case 13:
        work->input->eventId = 1;
        return 3;
    case 15:
        work->input->eventId = 3;
        return 3;
    case 16:
        work->input->eventId = 4;
        return 3;
    case 14:
        work->input->eventId = 2;
        return 3;
    case 8:
        work->nextAppId = 8;
        return FIELD_APP_RESULT_NEXT;
    case 7:
        work->nextAppId = 7;
        return FIELD_APP_RESULT_NEXT;
    case 17:
        work->nextAppId = 11;
        return FIELD_APP_RESULT_NEXT;
    case 19:
        work->nextAppId = 9;
        return FIELD_APP_RESULT_NEXT;
    case 18:
        gameData = GSYS_GetGameData(work->input->gameSystem);
        if (PokeParty_GetParam(PokeParty_GetPkm(GameData_GetParty(gameData), work->input->partySlot), 6, NULL) != 0) {
            work->nextAppId = FIELD_APP_POKELIST;
        } else {
            work->nextAppId = 9;
        }
        work->unk68 = 2;
        return FIELD_APP_RESULT_NEXT;
    case 20:
        work->input->eventId = 5;
        return 3;
    case 2:
        if (PML_ItemIsMail(param->item) == 1) {
            work->unk68 = 0;
        }
        work->nextAppId = FIELD_APP_POKELIST;
        return FIELD_APP_RESULT_NEXT;
    default:
        work->nextAppId = FIELD_APP_POKELIST;
        return FIELD_APP_RESULT_NEXT;
    }
}



TrainerCardParam *func_ov010_0216117c(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam) {
    GameData *gameData = GSYS_GetGameData(work->input->gameSystem);
    int zoneId = GameData_GetPlayerState(gameData)->zoneId;
    BOOL canEdit = TRUE;
    FieldSound *sound = GameData_GetFieldSoundSystem(gameData);
    TrainerCardParam *param;

    func_02029dbc(sound, GameSystem_GetISS(work->input->gameSystem));
    if (GetZoneIsUnionRoom(zoneId) || IsZone150Or151(zoneId)) {
        canEdit = FALSE;
    }
    param = func_ov010_0216f7bc(gameData, HEAPID_GAMEEVENT, canEdit);
    if (appParam != -1) {
        param->appParam = appParam;
    }
    return param;
}


u32 func_ov010_021611e0(FieldAppCallWork *work, void *data) {
    TrainerCardParam *param = data;
    FieldSound *sound = GameData_GetFieldSoundSystem(GSYS_GetGameData(work->input->gameSystem));

    func_02029dd8(sound, GameSystem_GetISS(work->input->gameSystem));
    if (param->result == 1) {
        return 1;
    }
    return 0;
}

TownMapParam *func_ov010_02161214(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam) {
    TownMapParam *param = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(TownMapParam), TRUE, "event_field_proclink.c", 1674);
    PlayerState *playerState;
    GameData *gameData;

    param->mode = TOWNMAP_MODE_VIEW;
    param->gsys = work->input->gameSystem;
    playerState = GameData_GetPlayerState(GSYS_GetGameData(work->input->gameSystem));
    gameData = GSYS_GetGameData(work->input->gameSystem);
    param->zoneId = PlayerState_GetZoneID(playerState);
    param->escapeZoneId = GameData_GetEscapeRopeZone(gameData)->zoneId;
    if (work->input->appId == FIELD_APP_POKELIST) {
        param->mode = TOWNMAP_MODE_FLY;
    } else {
        param->mode = TOWNMAP_MODE_VIEW;
    }
    return param;
}

u32 func_ov010_02161280(FieldAppCallWork *work, void *data) {
    TownMapParam *param = data;

    if (work->input->appId == 7) {
        switch (param->result) {
        case 0:
            return 0;
        case 1:
            return 1;
        default:
            return 1;
        }
    } else if (work->input->appId == 2) {
        switch (param->result) {
        case 0:
            work->nextAppId = FIELD_APP_BAG;
            return FIELD_APP_RESULT_NEXT;
        case 1:
            return 1;
        default:
            return 1;
        }
    } else if (work->input->appId == 0) {
        switch (param->result) {
        case 0:
            work->nextAppId = FIELD_APP_POKELIST;
            return FIELD_APP_RESULT_NEXT;
        case 1:
            return 1;
        case 2:
            work->input->eventId = 4;
            work->input->eventValue = param->zoneId;
            work->input->unk34 = param->warpX;
            work->input->unk38 = param->warpZ;
            work->input->unk3C = 0;
            return 2;
        default:
            return 1;
        }
    }
    return 1;
}


ConfigPanelParam *func_ov010_02161308(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam) {
    ConfigPanelParam *param;

    getTrainerDataBlkAddress(GameData_GetSaveControl(GSYS_GetGameData(work->input->gameSystem)));
    param = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(ConfigPanelParam), TRUE, "event_field_proclink.c", 1797);
    param->gsys = work->input->gameSystem;
    return param;
}

u32 func_ov010_02161340(FieldAppCallWork *work, void *data) {
    ConfigPanelParam *param = data;

    if (param->result) {
        return 1;
    }
    return 0;
}

void func_ov010_02161350(FieldAppCallWork *work, s32 appParam) {
    FieldAppCallInput *input = work->input;
    void *msgBgSys = Field_GetMsgBGSys(input->field);
    GameEvent *next = EventSave_Create(input->gameSystem, input->field, work->code, msgBgSys, &work->result);

    GameEvent_ChainNext(work->event, next);
}

WifiNoteParam *func_ov010_02161378(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam) {
    WifiNoteParam *param = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(WifiNoteParam), TRUE, "event_field_proclink.c", 1868);

    param->gameData = GSYS_GetGameData(work->input->gameSystem);
    return param;
}

u32 func_ov010_021613a4(FieldAppCallWork *work, void *data) {
    WifiNoteParam *param = data;

    if (work->input->appId == 8) {
        switch (param->result) {
        case 0:
            return 0;
        }
        return 1;
    }
    if (param->result == 0) {
        work->nextAppId = FIELD_APP_BAG;
        return FIELD_APP_RESULT_NEXT;
    }
    return 1;
}

MailParam *func_ov010_021613cc(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam) {
    GameData *gameData = GSYS_GetGameData(work->input->gameSystem);
    const PokeListParam *partyParam;
    const BagProcessData *bag;
    PokeParty *party;
    MailParam *param;

    GFL_OvlLoad(OVERLAY_ID(136));
    if (prevAppId == FIELD_APP_POKELIST) {
        partyParam = prevParam;
        party = GameData_GetParty(gameData);
        work->unk60 = partyParam->index;
        work->unk62 = partyParam->item;
        if (partyParam->result == 6) {
            return func_ov136_021c3cc0(gameData, 2, partyParam->index, PML_ItemGetMailID(partyParam->item), HEAPID_GAMEEVENT);
        }
        work->unk68 = 1;
        return func_ov136_021c3d44(gameData, PokeParty_GetPkm(party, partyParam->index), HEAPID_GAMEEVENT);
    }
    bag = prevParam;
    if (bag->mode == 2) {
        param = func_ov136_021c3cc0(gameData, 2, bag->item, PML_ItemGetMailID(bag->item), HEAPID_GAMEEVENT);
        work->unk62 = bag->item;
        return param;
    }
    work->unk68 = 1;
    return func_ov136_021c3da4(gameData, PML_ItemGetMailID(bag->item), HEAPID_GAMEEVENT);
}


u32 func_ov010_02161484(FieldAppCallWork *work, MailParam *param) {
    GameData *gameData = GSYS_GetGameData(work->input->gameSystem);

    if (work->prevAppId == FIELD_APP_POKELIST) {
        if (work->unk68 == 0 || work->unk68 == 2) {
            if (func_ov136_021c3e00(param) == TRUE) {
                func_ov136_021c3e04(param, PokeParty_GetPkm(GameData_GetParty(gameData), work->unk60));
                work->nextAppId = FIELD_APP_POKELIST;
            } else if (work->unk68 == 0) {
                work->nextAppId = FIELD_APP_BAG;
            } else {
                work->nextAppId = FIELD_APP_POKELIST;
            }
        } else {
            work->nextAppId = FIELD_APP_POKELIST;
        }
        GFL_OvlUnload(OVERLAY_ID(136));
        return FIELD_APP_RESULT_NEXT;
    } else if (work->prevAppId == FIELD_APP_BAG) {
        if (work->unk68 == 2) {
            if (func_ov136_021c3e00(param) == TRUE) {
                func_ov136_021c3e04(param, PokeParty_GetPkm(GameData_GetParty(gameData), work->unk60));
                work->nextAppId = FIELD_APP_POKELIST;
            } else {
                work->nextAppId = FIELD_APP_POKELIST;
            }
        } else {
            work->nextAppId = FIELD_APP_BAG;
        }
        GFL_OvlUnload(OVERLAY_ID(136));
        return FIELD_APP_RESULT_NEXT;
    }
    work->nextAppId = FIELD_APP_BAG;
    GFL_OvlUnload(OVERLAY_ID(136));
    return FIELD_APP_RESULT_NEXT;
}


void func_ov010_02161534(MailParam *param) {
    GFL_OvlLoad(OVERLAY_ID(136));
    func_ov136_021c3e24(param);
    GFL_OvlUnload(OVERLAY_ID(136));
}

ShinkaDemoParam *func_ov010_02161554(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam) {
    GameData *gameData = GSYS_GetGameData(work->input->gameSystem);
    ShinkaDemoParam *param = NULL;
    const PokeListParam *partyParam = prevParam;
    PokeParty *party;
    PartyPkm *pkm;
    PlayerState *playerState;
    u32 species;
    u32 method;

    if (prevAppId == FIELD_APP_POKELIST) {
        party = GameData_GetParty(gameData);
        if (partyParam->result == 8) {
            work->unk68 = 3;
            pkm = PokeParty_GetPkm(party, partyParam->index);
            species = CheckEvolveSpecies(party, pkm, 3, partyParam->item, GameData_GetSeason(gameData), &method, HEAPID_GAMEEVENT);
        } else {
            work->unk68 = 4;
            pkm = PokeParty_GetPkm(party, partyParam->index);
            playerState = GameData_GetPlayerState(gameData);
            species = CheckEvolveSpecies(party, pkm, 0, playerState->zoneId, GameData_GetSeason(gameData), &method, HEAPID_GAMEEVENT);
        }
        param = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(ShinkaDemoParam), FALSE, "event_field_proclink.c", 2152);
        param->gameData = gameData;
        param->party = party;
        param->species = species;
        param->partyIndex = partyParam->index;
        param->method = method;
        param->unkC = 1;
        param->canCancel = TRUE;
    }
    return param;
}




u32 func_ov010_02161610(FieldAppCallWork *work, void *data) {
    GSYS_GetGameData(work->input->gameSystem);
    if (work->unk68 == 3) {
        work->nextAppId = FIELD_APP_BAG;
        return FIELD_APP_RESULT_NEXT;
    }
    if (work->unk68 == 4) {
        work->nextAppId = FIELD_APP_POKELIST;
        return FIELD_APP_RESULT_NEXT;
    }
    return 0;
}

BattleRecorderParam *func_ov010_0216163c(FieldAppCallWork *work, s32 appParam, s32 prevAppId, const void *prevParam) {
    BattleRecorderParam *param = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(BattleRecorderParam), TRUE, "event_field_proclink.c", 2220);

    param->gameData = GSYS_GetGameData(work->input->gameSystem);
    return param;
}

u32 func_ov010_02161668(FieldAppCallWork *work, void *data) {
    BattleRecorderParam *param = data;

    if (work->input->appId == 11) {
        switch (param->result) {
        case 0:
            return 0;
        case 1:
            return 1;
        default:
            return 0;
        }
    }
    switch (param->result) {
    case 0:
        work->nextAppId = FIELD_APP_BAG;
        return FIELD_APP_RESULT_NEXT;
    case 1:
        return 1;
    default:
        return 0;
    }
}

