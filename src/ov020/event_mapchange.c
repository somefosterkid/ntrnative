// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov035/event_mapchange.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/flags.h"
#include "constants/sound.h"
#include "constants/version.h"
#include "constants/vars.h"
#include "constants/zones.h"
#include "dsprot/dsprot.h"
#include "field/event_3d_demo.h"
#include "field/event_data.h"
#include "field/event_mapchange.h"
#include "field/event_season_banner.h"
#include "field/event_sound.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_event.h"
#include "field/field_map.h"
#include "field/field_script.h"
#include "field/field_sound.h"
#include "field/field_status.h"
#include "field/intrude_field.h"
#include "field/intrude_work.h"
#include "field/player_state.h"
#include "field/shaymin_form.h"
#include "field/union_main.h"
#include "field/zone.h"
#include "field/zone_change.h"
#include "field/field_wfbc_data_local.h"
#include "gfl/arc.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_system.h"
#include "gfl/overlay.h"
#include "gfl/rtc_cache.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_vec.h"
#include "pml/poke_party.h"
#include "save/adventure.h"
#include "system/adventure_savedata.h"
#include "save/dream_world.h"
#include "save/high_link.h"
#include "save/event_work.h"
#include "save/key_info.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "save/trainer_card.h"
#include "struct_decls.h"
#include "system/area_data.h"
#include "system/game_beacon.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/iss_switch_sys.h"
#include "system/iss_sys.h"
#include "system/new_game.h"
#include "system/playtime_ctrl.h"
#include "system/season.h"
#include "system/zone_weather.h"
#include "field/city_state.h"
#include "field/event_action_call.h"
#include "save/gimmick_savedata.h"

struct EventGameOpening {
    GameSystem *gsys;
    GameSystemProcData *procData;
};

struct EventFieldFirst {
    GameSystem *gsys;
    GameData *gameData;
    ZoneSpawnInfo spawn;
};

struct EventFieldContinue {
    GameSystem *gsys;
    GameData *gameData;
    u16 zoneId;
    BOOL continueFromSave;
};

struct EventMapChange {
    GameSystem *gsys;
    GameData *gameData;
    Field *field;
    u16 zoneId;
    ZoneSpawnInfo spawn;
    u32 unk2C;
    u8 mode;
    VecFx32 unk34;
    BOOL unk40;
    BOOL seasonChanged;
    u16 prevSeason;
    u16 season;
    WarpSequence warp;
};

// Black's size (ARM's 32-bit pointers; a PC target's record is bigger)
#if defined(__MWERKS__) || defined(__arm__)
typedef char EventMapChangeSizeCheck[sizeof(EventMapChange) == 0x9c ? 1 : -1];
#endif

struct EventMapChangeCore {
    EventMapChange *mapChange;
};

struct EventMapChangeBlackout {
    GameSystem *gsys;
    GameData *gameData;
    ZoneSpawnInfo spawn;
};

struct ZoneGimmick {
    u32 zoneId;
    u16 gimmickId;
};

struct EventEntralinkWarp {
    ZoneSpawnInfo spawn;
    GameSystem *gsys;
    GameData *gameData;
    Field *field;
    u32 unk28;
    u8 unk2C;
};

GameEventReturnCode EventEntralinkWarp_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventEntralinkWarpIn_Callback(GameEvent *event, u32 *state, void *data);

GameEventReturnCode EventGameOpening_Callback(GameEvent *event, u32 *state, void *data);
void func_ov020_02183c58(GameSystem *gsys);
GameEventReturnCode EventFieldFirst_Callback(GameEvent *event, u32 *state, void *data);
void func_ov020_02183dd4(GameSystem *gsys);
GameEventReturnCode EventFieldContinue_Callback(GameEvent *event, u32 *state, void *data);
void EventMapChange_LoadSeasons(EventMapChange *wk);
void EventMapChange_SetupWarpSequenceOut(EventMapChange *wk, GameEvent *parent);
void EventMapChange_SetupWarpSequenceIn(EventMapChange *wk, GameEvent *parent);
GameEventReturnCode EventMapChangeCore_Callback(GameEvent *event, u32 *state, void *data);
GameEvent *EventMapChangeCore_Create(EventMapChange *wk, u8 mode);
GameEventReturnCode EventMapChangeWarp_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChange_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeFakeWarp_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeQuicksand_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeEscapeRope_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeDig_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeTeleport_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeDiveOut_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeDiveIn_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeWarpPad_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeUnionRoomExit_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventUnionRoomWarp_Callback(GameEvent *event, u32 *state, void *data);
void InitMapChangeEvent(EventMapChange *wk, GameSystem *gsys);
GameEventReturnCode EventMapChangeBlackout_Callback(GameEvent *event, u32 *state, void *data);

// DS Protect's checks (ov230) run this with the function to call: it calls it and returns the complement of its
// address, which the check compares
uintptr_t func_ov020_02183b60(void (*callback)(void)) {
    if (callback != NULL) {
        callback();
    }
    return ~(uintptr_t)callback;
}

static inline void ClearLCDCVram(void) {
    GX_SetBankForLCDC(0x1ff);
    MIi_CpuClearFast(0, (void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    GX_DisableBankForLCDC();
}

GameEvent *CreateGameEntryPointEvent(GameSystem *gsys, GameSystemProcData *procData) {
    switch (procData->entryPoint) {
    case GAME_ENTRYPOINT_OPENING:
        return EventGameOpening_Create(gsys, procData);
    case GAME_ENTRYPOINT_FIELD_CONTINUE:
        return EventFieldContinue_Create(gsys, procData);
    case GAME_ENTRYPOINT_DEBUG:
        return EventFieldFirst_Create(gsys, procData);
    }
}

GameEventReturnCode EventGameOpening_Callback(GameEvent *event, u32 *state, void *data) {
    EventGameOpening *wk = data;
    GameSystem *gsys = wk->gsys;
    GameData *gameData = GSYS_GetGameData(gsys);
    u32 realTime;
    u8 season;

    switch (*state) {
    case 0:
        ClearLCDCVram();
        (*state)++;
        break;
    case 1:
        realTime = Season_GetRealTime();
        Season_Set(gameData, realTime);
        season = GameData_GetSeason(gameData);
        GameEvent_ChainNext(event, EventSeasonBanner_CreateStandalone(gsys, season, season));
        (*state)++;
        break;
    case 2:
        GameEvent_ChainNext(event, Event3DDemo_Create(gsys, event, 5, 0, 1));
        (*state)++;
        break;
    case 3:
        GameEvent_Replace(event, EventFieldFirst_Create(gsys, wk->procData));
        break;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *EventGameOpening_Create(GameSystem *gsys, GameSystemProcData *procData) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventGameOpening_Callback, sizeof(EventGameOpening));
    EventGameOpening *wk = GameEvent_GetData(event);

    wk->gsys = gsys;
    wk->procData = procData;
    return event;
}

void func_ov020_021860d4(CityState *city, PlayerInfo *player, u16 id);
CityResidentInfo *func_020127f0(GameData *gameData);
void func_ov020_02183c58(GameSystem *gsys) {
    GameData *gameData = GSYS_GetGameData(gsys);
    CityState *city = func_020127c4(gameData);

    func_ov020_021860d4(city, GetGameDataPlayerInfo(gameData), 1);
    func_ov010_02161d64(func_020127c4(gameData));
    func_ov010_02161f14(func_020127f0(gameData), func_020127c4(gameData), 1);
}


GameEventReturnCode EventFieldFirst_Callback(GameEvent *event, u32 *state, void *data) {
    EventFieldFirst *wk = data;
    GameSystem *gsys = wk->gsys;
    GameData *gameData = GSYS_GetGameData(gsys);

    GameData_GetEventWork(gameData);
    switch (*state) {
    case 0:
        FieldScript_CallPlayerInitSetup(gsys, 1);
        FieldMapControl_LoadZone(gsys, wk->spawn.zoneId);
        FieldMapControl_InitSpawn(gsys, &wk->spawn);
        (*state)++;
        break;
    case 1:
        GameEvent_ChainNext(
            event,
            EventBGMChange_Create(
                gsys, GetMapBGMIDByPlayerState2(gameData, wk->spawn.zoneId, GameData_GetSeason(gameData)), 0, 60));
        (*state)++;
        break;
    case 2:
        GameEvent_ChainNext(event, EventFieldOpen_Create(gsys));
        (*state)++;
        break;
    case 3:
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, GSYS_GetField(gsys), 1, 0, 1, 0, 0));
        (*state)++;
        break;
    case 4: {
        Field *field = GSYS_GetField(gsys);

        if (Field_GetPlaceName(field) != NULL) {
            func_ov021_021bc578(Field_GetPlaceName(field), wk->spawn.zoneId);
        }
        return GAMEEVENT_DONE;
    }
    }
    return GAMEEVENT_CONTINUE;
}

void func_ov020_02185f6c(GameData *gameData);

GameEvent *EventFieldFirst_Create(GameSystem *gsys, GameSystemProcData *procData) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventFieldFirst_Callback, sizeof(EventFieldFirst));
    EventFieldFirst *wk = GameEvent_GetData(event);

    wk->gsys = gsys;
    wk->gameData = GSYS_GetGameData(gsys);
    LoadAspertiaCitySpawnInfo(&wk->spawn);
    func_ov020_02183c58(gsys);
    setAdvTimeBlkRtcOffsetOwnerMacBdayMonthDay(getSaveAdventureDataBlk(GameData_GetSaveControl(wk->gameData)));
    GameSystemTimer_Start();
    func_ov020_02185f6c(wk->gameData);
    ClearLCDCVram();
    return event;
}


void func_ov010_02161d64(CityState *state);

void func_ov020_02183dd4(GameSystem *gsys) {
    GameData *gameData = GSYS_GetGameData(gsys);
    func_ov010_02161d64(func_020127c4(gameData));
}

GameEventReturnCode EventFieldContinue_Callback(GameEvent *event, u32 *state, void *data) {
    EventFieldContinue *wk = data;
    GameSystem *gsys = wk->gsys;
    GameData *gameData = GSYS_GetGameData(gsys);
    EventWork *eventWork = GameData_GetEventWork(gameData);

    switch (*state) {
    case 0: {
        ZoneSpawnInfo *next;
        s32 cacheIdx;
        MMSys *mmSys;

        FieldStatus_SetContinueFlag(GameData_GetFieldStatus(gameData), TRUE);
        if (wk->continueFromSave) {
            next = GameData_GetNextZone(gameData);
            wk->zoneId = next->zoneId;
            FieldMapControl_DeleteAllActors(gsys);
            FieldMapControl_LoadZone(gsys, wk->zoneId);
            func_ov020_0218601c(gameData);
            FieldMapControl_InitSpawn(gsys, next);
            func_0202735c(next->zoneId, gameData);
        } else {
            FieldMapControl_LoadZone(gsys, wk->zoneId);
            func_ov020_0218601c(gameData);
            func_ov010_02168eec(gameData);
            func_ov010_02164474(gameData, wk->zoneId);
            UpdateWeatherToDefault(gameData, wk->zoneId);
            cacheIdx = GetZoneNPCInfoCacheIdx(wk->zoneId);
            mmSys = GameData_GetMMSys(gameData);
            if (cacheIdx < 0x10) {
                LoadMModelSystemInfoCache(mmSys, cacheIdx);
            } else {
                FldActSys_ClearCache(mmSys);
            }
        }
        (*state)++;
        break;
    }
    case 1:
        GameEvent_ChainNext(event, EventBGMChange_Create(gsys, GetMapBGMIDByPlayerState2(gameData, wk->zoneId, GameData_GetSeason(gameData)), 0, 0x3c));
        (*state)++;
        break;
    case 2:
        GameEvent_ChainNext(event, EventFieldOpen_Create(gsys));
        (*state)++;
        break;
    case 3:
        if (wk->continueFromSave && *EventWork_GetWkPtr(eventWork, EVENT_WORK_CONTINUE_SCRIPT) != 0) {
            EventScriptCall_Start(event, 0x83b, NULL, 0, HEAPID_FIELDMAP);
        } else {
            u8 season = GameData_GetSeason(gameData);

            GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, GSYS_GetField(gsys), 3, 0, 0, season, season));
        }
        (*state)++;
        break;
    case 4: {
        Field *field = GSYS_GetField(gsys);

        if (Field_GetPlaceName(field) != NULL) {
            func_ov021_021bc578(Field_GetPlaceName(field), wk->zoneId);
        }
        return GAMEEVENT_DONE;
    }
    default:
        break;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *EventFieldContinue_Create(GameSystem *gsys, GameSystemProcData *procData) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventFieldContinue_Callback, sizeof(EventFieldContinue));
    EventFieldContinue *wk = GameEvent_GetData(event);
    EventWork *eventWork;
    SaveControl *save;
    PokeParty *party;
    AdventureSave *adventure;
    TrainerCardSave *trainerCard;

    wk->gsys = gsys;
    wk->gameData = GSYS_GetGameData(gsys);
    wk->zoneId = procData->zoneId;
    eventWork = GameData_GetEventWork(wk->gameData);
    wk->continueFromSave = EventWork_FlagGet(eventWork, EVENT_FLAG_CONTINUE_SCRIPT);
    EventWork_FlagReset(eventWork, EVENT_FLAG_CONTINUE_SCRIPT);

    save = GameData_GetSaveControl(wk->gameData);
    adventure = getSaveAdventureDataBlk(save);
    party = SaveControl_GetPokePartySave(save);
    trainerCard = getTrainerCardDataBlkAddress(wk->gameData);
    if (func_02008b4c(adventure) == 0 || !hasClockNotBeenTampered(adventure)) {
        setNewDayForCountdown(getSaveAdventureTimeBlock(save));
        func_ov010_0216a390(wk->gameData, party);
        setSecondsCurrentTimeInTrainerCard(trainerCard, RTC_ConvertSecondsCached());
    }
    setAdvTimeBlkRtcOffsetOwnerMacBdayMonthDay(adventure);
    TransformVsPokePartyBySeason(wk->gameData, party, GameData_GetSeason(wk->gameData));

    func_ov020_02183dd4(gsys);
    GameSystemTimer_Start();
    ClearLCDCVram();
    return event;
}

void EventMapChange_LoadSeasons(EventMapChange *wk) {
    u16 prevSeason;
    u16 season;
    HeapID heapId;
    AreaData *areaData;
    BOOL isExterior;

    GameData_GetSeasons(wk->gameData, &prevSeason, &season);
    heapId = Field_GetHeapID(wk->field);
    areaData = AreaData_Create(heapId, ZoneData_GetAreaID(wk->spawn.zoneId), 0);
    isExterior = FALSE;
    if (AreaData_IsExterior(areaData)) {
        isExterior = TRUE;
    }
    AreaData_Free(areaData);

    if (season != prevSeason && isExterior) {
        wk->seasonChanged = TRUE;
        wk->prevSeason = prevSeason;
        wk->season = season;
    }
}

u32 func_ov021_021be8b8(u16 fromZone, u16 toZone);
void EventMapChange_SetupWarpSequenceOut(EventMapChange *wk, GameEvent *parent) {
    WarpSequence *warp = &wk->warp;

    warp->parent = parent;
    warp->gsys = wk->gsys;
    warp->gameData = wk->gameData;
    warp->field = wk->field;
    warp->unk10 = wk->unk2C;
    warp->zoneId = wk->zoneId;
    warp->spawn = wk->spawn;
    warp->outTransition = GetOutTransitionTypeBetweenZones(wk->zoneId, wk->spawn.zoneId);
    warp->inTransition = func_ov021_021be8b8(wk->zoneId, wk->spawn.zoneId);
    if (wk->unk40 && wk->seasonChanged) {
        warp->seasonChanged = TRUE;
    } else {
        warp->seasonChanged = FALSE;
    }
    warp->startSeason = Season_GetNext(wk->prevSeason);
    warp->endSeason = wk->season;
    warp->unk48 = 0;
    warp->unk4C = 0;
}

void EventMapChange_SetupWarpSequenceIn(EventMapChange *wk, GameEvent *parent) {
    WarpSequence *warp = &wk->warp;

    warp->parent = parent;
    warp->gsys = wk->gsys;
    warp->gameData = wk->gameData;
    warp->field = wk->field;
    warp->unk10 = wk->unk2C;
    warp->zoneId = wk->zoneId;
    warp->spawn = wk->spawn;
    warp->outTransition = GetOutTransitionTypeBetweenZones(wk->zoneId, wk->spawn.zoneId);
    warp->inTransition = func_ov021_021be8b8(wk->zoneId, wk->spawn.zoneId);
    if (wk->unk40 && wk->seasonChanged) {
        warp->seasonChanged = TRUE;
    } else {
        warp->seasonChanged = FALSE;
    }
    warp->startSeason = Season_GetNext(wk->prevSeason);
    warp->endSeason = wk->season;
    if (wk->spawn.changeType == ZONE_SPAWN_CHANGE_TYPE_POSITION) {
        warp->transitionType = 0;
    } else {
        warp->transitionType = GetWarpTransitionType(GetZoneWarpByID(GameData_GetEventData(wk->gameData), wk->spawn.warpId));
    }
}

void *func_ov020_0218604c(void);
void *func_ov020_0218606c(void);
void func_ov020_0218608c(void);
void func_ov010_02164454(GameData *gameData, u16 zoneId);
void func_ov010_02164464(GameData *gameData, u16 zoneId);
void func_ov010_0216445c(GameData *gameData, u16 zoneId);


GameEventReturnCode EventMapChangeCore_Callback(GameEvent *event, u32 *state, void *data) {
    EventMapChange *wk = ((EventMapChangeCore *)data)->mapChange;
    GameSystem *gsys = wk->gsys;
    GameData *gameData = wk->gameData;
    Field *field = wk->field;
    GameCommSys *comm = GSYS_GetGameCommSystem(gsys);

    switch (*state) {
    case 0:
        FieldStatus_SetBusyFlag(GameData_GetFieldStatus(gameData), 2);
        GameEvent_ChainNext(event, EventFieldCloseKeepSound_Create(gsys, field));
        (*state)++;
        break;
    case 1:
        GFL_OvlLoad(OVERLAY_ID(230));
        if (func_02013d34(wk->spawn.zoneId) == TRUE) {
            u8 boot = GameCommSys_BootCheck(comm);

            if (boot == 1 || boot == 2) {
                func_02026400(comm);
                *state = 2;
                break;
            }
        }
        *state = 3;
        break;
    case 2:
        if (GameCommSys_BootCheck(comm) == 0) {
            *state = 3;
        }
        break;
    case 3:
        func_ov230_021882a0(func_ov020_02183b60, func_ov020_0218604c, 0);
        FieldMapControl_DeleteAllActors(gsys);
        if (wk->unk40 && wk->seasonChanged) {
            AdventureTime *adventureTime;
            u8 season;
            PokeParty *party;

            Season_Set(gameData, wk->season);
            adventureTime = getSaveAdventureTimeBlock(GameData_GetSaveControl(gameData));
            season = GameData_GetSeason(gameData);
            party = GameData_GetParty(gameData);
            TransformVsPokePartyBySeason(gameData, party, season);
            func_ov010_0216a358(gameData, party, &adventureTime->time, season);
        }
        func_ov230_02188318(func_ov020_02183b60, func_ov020_0218606c, 0);
        FieldMapControl_LoadZone(gsys, wk->spawn.zoneId);
        FieldMapControl_InitSpawn(gsys, &wk->spawn);
        if (wk->mode != 4) {
            func_0202735c(wk->spawn.zoneId, gameData);
        }
        func_ov230_021883cc(func_ov020_02183b60, func_ov020_0218608c, 0);
        switch (wk->mode) {
        case 1:
            func_ov010_02164454(gameData, wk->spawn.zoneId);
            break;
        case 2:
            func_ov010_02164464(gameData, wk->spawn.zoneId);
            break;
        case 3:
            func_ov010_0216445c(gameData, wk->spawn.zoneId);
            break;
        }
        GFL_OvlUnload(OVERLAY_ID(230));
        func_ov010_0215874c(comm);
        (*state)++;
        break;
    case 4:
        GameEvent_ChainNext(event, EventFieldOpen_Create(gsys));
        (*state)++;
        break;
    case 5:
        GameEvent_ChainNext(event, func_02029c64(gsys));
        (*state)++;
        break;
    case 6:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *EventMapChangeCore_Create(EventMapChange *wk, u8 mode) {
    GameEvent *event = GameEvent_Create(wk->gsys, NULL, EventMapChangeCore_Callback, sizeof(EventMapChangeCore));
    EventMapChangeCore *core = GameEvent_GetData(event);

    core->mapChange = wk;
    wk->mode = mode;
    return event;
}

GameEventReturnCode EventMapChangeWarp_Callback(GameEvent *event, u32 *state, void *data) {
    EventMapChange *wk = data;
    Field *field = wk->field;

    switch (*state) {
    case 0:
        EventMapChange_LoadSeasons(wk);
        DisableAllActorsMovement(Field_GetActorSystem(field));
        EventMapChange_SetupWarpSequenceOut(wk, event);
        GameEvent_ChainNext(event, EventWarpSequence_CreateOut(&wk->warp));
        (*state)++;
        return GAMEEVENT_CONTINUE_DIRECT;
    case 1:
        GameEvent_ChainNext(event, EventMapChangeCore_Create(wk, 0));
        (*state)++;
        break;
    case 2:
        EventMapChange_SetupWarpSequenceIn(wk, event);
        GameEvent_ChainNext(event, EventWarpSequence_CreateIn(&wk->warp));
        (*state)++;
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

u8 func_ov010_0215841c(GameData *gameData);
u8 func_ov010_02158428(IntrudeComm *work);
GameEvent *EventWarpSequence_CreateOut(WarpSequence *warp);
GameEvent *EventWarpSequence_CreateIn(WarpSequence *warp);


GameEventReturnCode func_ov020_02184410(GameEvent *event, u32 *state, void *data) {
    EventMapChange *wk = data;
    Field *field = wk->field;
    GameData *gameData = wk->gameData;
    IntrudeComm *work = func_ov010_02158344(GSYS_GetGameCommSystem(wk->gsys));

    switch (*state) {
    case 0:
        DisableAllActorsMovement(Field_GetActorSystem(field));
        if (func_02013dd0(wk->spawn.zoneId) == 1) {
            *state = 5;
        } else {
            *state = 1;
        }
        break;
    case 1:
        EventMapChange_SetupWarpSequenceOut(wk, event);
        GameEvent_ChainNext(event, EventWarpSequence_CreateOut(&wk->warp));
        (*state)++;
        return GAMEEVENT_CONTINUE_DIRECT;
    case 2:
        GameEvent_ChainNext(event, EventMapChangeCore_Create(wk, 0));
        (*state)++;
        break;
    case 3:
        EventMapChange_SetupWarpSequenceIn(wk, event);
        GameEvent_ChainNext(event, EventWarpSequence_CreateIn(&wk->warp));
        (*state)++;
        break;
    case 4:
        return GAMEEVENT_DONE;
    case 5:
        if (work == NULL) {
            *state = 7;
            break;
        }
        if (func_ov011_021746ac(work, func_ov010_0215841c(gameData)) == 1) {
            (*state)++;
        }
        break;
    case 6:
        if (work == NULL) {
            *state = 7;
            break;
        }
        if (func_ov010_02158428(work) == 1) {
            *state = 1;
        }
        break;
    case 7:
        EnableAllActorsMovement(Field_GetActorSystem(field));
        return GAMEEVENT_DONE;
    default:
        break;
    }
    return GAMEEVENT_CONTINUE;
}


GameEventReturnCode EventMapChange_Callback(GameEvent *event, u32 *state, void *data) {
    EventMapChange *wk = data;
    GameSystem *gsys = wk->gsys;
    Field *field = wk->field;
    GameData *gameData = wk->gameData;
    FieldSound *fieldSound = GameData_GetFieldSoundSystem(gameData);

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, CallEventPrepareResidentActorsForZoneChange(gsys, field));
        (*state)++;
        break;
    case 1:
        FieldSnd_SetZoneBGM(fieldSound, gameData, wk->spawn.zoneId, wk->season);
        FieldSnd_FadeInImmediate(fieldSound, gameData);
        (*state)++;
        break;
    case 2:
        GameEvent_ChainNext(event, EventMapChangeCore_Create(wk, wk->mode));
        (*state)++;
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode EventMapChangeFakeWarp_Callback(GameEvent *event, u32 *state, void *data) {
    EventMapChange *wk = data;
    GameSystem *gsys = wk->gsys;
    Field *field = wk->field;
    u32 type;

    switch (*state) {
    case 0:
        EventMapChange_LoadSeasons(wk);
        GameEvent_ChainNext(event, CallEventPrepareResidentActorsForZoneChange(gsys, field));
        (*state)++;
        break;
    case 1:
        if (wk->unk40 && wk->seasonChanged) {
            GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, field, 0, 0));
        } else {
            type = GetOutTransitionTypeBetweenZones(wk->zoneId, wk->spawn.zoneId);
            GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, field, type, 0));
        }
        (*state)++;
        break;
    case 2:
        GameEvent_ChainNext(event, EventMapChangeCore_Create(wk, wk->mode));
        (*state)++;
        break;
    case 3:
        if (wk->unk40 && wk->seasonChanged) {
            GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, field, 3, 0, 0, wk->prevSeason, wk->season));
        } else {
            type = func_ov021_021be8b8(wk->zoneId, wk->spawn.zoneId);
            GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, field, type, 0, 1, 0, 0));
        }
        (*state)++;
        break;
    case 4:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov020_0218466c(GameEvent *event, u32 *state, void *data) {
    EventMapChange *wk = data;
    GameSystem *gsys = wk->gsys;
    GameData *gameData = wk->gameData;
    Field *field = wk->field;
    FieldSound *fieldSound = GameData_GetFieldSoundSystem(gameData);

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, CallEventPrepareResidentActorsForZoneChange(gsys, field));
        (*state)++;
        break;
    case 8:
        GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, field, 2, 0));
        (*state)++;
        break;
    case 9:
        FieldSnd_SetZoneBGM(fieldSound, gameData, wk->spawn.zoneId, wk->season);
        FieldSnd_FadeInImmediate(fieldSound, gameData);
        (*state)++;
        break;
    case 10:
        GameEvent_ChainNext(event, EventMapChangeCore_Create(wk, wk->mode));
        (*state)++;
        break;
    case 11:
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, field, 2, 0, 0, 0, 0));
        (*state)++;
        break;
    case 12:
        func_ov021_021bc550(Field_GetPlaceName(field), wk->spawn.zoneId);
        return GAMEEVENT_DONE;
    default:
        (*state)++;
        break;
    }
    return GAMEEVENT_CONTINUE;
}


GameEventReturnCode EventMapChangeQuicksand_Callback(GameEvent *event, u32 *state, void *data) {
    EventMapChange *wk = data;
    GameSystem *gsys = wk->gsys;
    GameData *gameData = wk->gameData;
    Field *field = wk->field;
    FieldSound *fieldSound = GameData_GetFieldSoundSystem(gameData);

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, CallEventPrepareResidentActorsForZoneChange(gsys, field));
        (*state)++;
        break;
    case 1:
        GameEvent_ChainNext(event, EventQuicksandDrawIn_Create(event, gsys, field, &wk->unk34));
        (*state)++;
        break;
    case 2:
        FieldSnd_SetZoneBGM(fieldSound, gameData, wk->spawn.zoneId, wk->season);
        FieldSnd_FadeInImmediate(fieldSound, gameData);
        (*state)++;
        break;
    case 3:
        GameEvent_ChainNext(event, EventMapChangeCore_Create(wk, 0));
        (*state)++;
        break;
    case 4:
        GameEvent_ChainNext(event, EventQuicksandArrive_Create(event, gsys, field));
        (*state)++;
        break;
    case 5:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode EventMapChangeEscapeRope_Callback(GameEvent *event, u32 *state, void *data) {
    EventMapChange *wk = data;
    GameSystem *gsys = wk->gsys;
    GameData *gameData = wk->gameData;
    Field *field = wk->field;
    FieldSound *fieldSound = GameData_GetFieldSoundSystem(gameData);

    switch (*state) {
    case 0:
        EventMapChange_LoadSeasons(wk);
        GameEvent_ChainNext(event, CallEventPrepareResidentActorsForZoneChange(gsys, field));
        (*state)++;
        break;
    case 1:
        GameEvent_ChainNext(event, func_ov021_021bfd70(event, gsys, field, wk->seasonChanged));
        (*state)++;
        break;
    case 2:
        GameData_AdjustPlayerStateOnDiveOut(gameData);
        FieldSnd_SetZoneBGM(fieldSound, gameData, wk->spawn.zoneId, wk->season);
        FieldSnd_FadeInImmediate(fieldSound, gameData);
        (*state)++;
        break;
    case 3:
        GameEvent_ChainNext(event, EventMapChangeCore_Create(wk, 2));
        (*state)++;
        break;
    case 4:
        GameEvent_ChainNext(event, func_ov021_021bf5b4(event, gsys, field, wk->seasonChanged, wk->prevSeason, wk->season));
        (*state)++;
        break;
    case 5:
        func_ov021_021bc550(Field_GetPlaceName(field), wk->spawn.zoneId);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEventReturnCode EventMapChangeDig_Callback(GameEvent *event, u32 *state, void *data) {
    EventMapChange *wk = data;
    GameSystem *gsys = wk->gsys;
    GameData *gameData = wk->gameData;
    Field *field = wk->field;
    FieldSound *fieldSound = GameData_GetFieldSoundSystem(gameData);

    switch (*state) {
    case 0:
        EventMapChange_LoadSeasons(wk);
        GameEvent_ChainNext(event, CallEventPrepareResidentActorsForZoneChange(gsys, field));
        (*state)++;
        break;
    case 1:
        GameEvent_ChainNext(event, EventDig_Create(event, gsys, field, wk->seasonChanged));
        (*state)++;
        break;
    case 2:
        GameData_AdjustPlayerStateOnDiveOut(gameData);
        FieldSnd_SetZoneBGM(fieldSound, gameData, wk->spawn.zoneId, wk->season);
        FieldSnd_FadeInImmediate(fieldSound, gameData);
        (*state)++;
        break;
    case 3:
        GameEvent_ChainNext(event, EventMapChangeCore_Create(wk, 2));
        (*state)++;
        break;
    case 4:
        GameEvent_ChainNext(event, func_ov021_021bf5e8(event, gsys, field, wk->seasonChanged, wk->prevSeason, wk->season));
        (*state)++;
        break;
    case 5:
        func_ov021_021bc550(Field_GetPlaceName(field), wk->spawn.zoneId);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode EventMapChangeTeleport_Callback(GameEvent *event, u32 *state, void *data) {
    EventMapChange *wk = data;
    GameSystem *gsys = wk->gsys;
    GameData *gameData = wk->gameData;
    Field *field = wk->field;
    FieldSound *fieldSound = GameData_GetFieldSoundSystem(gameData);

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, CallEventPrepareResidentActorsForZoneChange(gsys, field));
        (*state)++;
        break;
    case 1:
        GameEvent_ChainNext(event, func_ov021_021bfdd0(event, gsys, field, TRUE));
        (*state)++;
        break;
    case 2:
        SetPlayerSpecialState(GameData_GetPlayerState(gameData), 0);
        FieldSnd_SetZoneBGM(fieldSound, gameData, wk->spawn.zoneId, wk->season);
        FieldSnd_FadeInImmediate(fieldSound, gameData);
        (*state)++;
        break;
    case 3:
        GameEvent_ChainNext(event, EventMapChangeCore_Create(wk, 3));
        (*state)++;
        break;
    case 4:
        GameEvent_ChainNext(event, func_ov021_021bf61c(event, gsys, field));
        (*state)++;
        break;
    case 5:
        func_ov021_021bc550(Field_GetPlaceName(field), wk->spawn.zoneId);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEventReturnCode EventMapChangeDiveOut_Callback(GameEvent *event, u32 *state, void *data) {
    EventMapChange *wk = data;
    GameSystem *gsys = wk->gsys;
    Field *field = wk->field;
    FieldTaskManager *taskManager = Field_GetTaskManager(field);

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, field, 1, 0));
        (*state)++;
        break;
    case 1:
        if (FieldTaskManager_IsIdle(taskManager)) {
            GameEvent_ChainNext(event, CallEventPrepareResidentActorsForZoneChange(gsys, field));
            (*state)++;
        }
        break;
    case 2:
        GameEvent_ChainNext(event, EventMapChangeCore_Create(wk, 0));
        (*state)++;
        break;
    case 3:
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, field, 1, 0, 1, 0, 0));
        (*state)++;
        break;
    case 4:
        func_ov021_021bc550(Field_GetPlaceName(field), wk->spawn.zoneId);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode EventMapChangeDiveIn_Callback(GameEvent *event, u32 *state, void *data) {
    EventMapChange *wk = data;
    GameSystem *gsys = wk->gsys;
    Field *field = wk->field;

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, CallEventPrepareResidentActorsForZoneChange(gsys, field));
        (*state)++;
        break;
    case 1:
        GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, field, 0, 0));
        (*state)++;
        break;
    case 2: {
        GameEvent *mapChange = GameEvent_Create(gsys, NULL, EventMapChange_Callback, sizeof(EventMapChange));
        EventMapChange *mapChangeWk = GameEvent_GetData(mapChange);

        *mapChangeWk = *wk;
        GameEvent_ChainNext(event, mapChange);
        (*state)++;
        break;
    }
    case 3:
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, field, 0, 0, 1, 0, 0));
        (*state)++;
        break;
    case 4:
        func_ov021_021bc550(Field_GetPlaceName(field), wk->spawn.zoneId);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode EventMapChangeWarpPad_Callback(GameEvent *event, u32 *state, void *data) {
    EventMapChange *wk = data;
    GameSystem *gsys = wk->gsys;
    GameData *gameData = wk->gameData;
    Field *field = wk->field;
    FieldSound *fieldSound = GameData_GetFieldSoundSystem(gameData);

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, CallEventPrepareResidentActorsForZoneChange(gsys, field));
        (*state)++;
        break;
    case 1:
        GameEvent_ChainNext(event, func_ov021_021bfe00(event, gsys, field));
        (*state)++;
        break;
    case 2:
        FieldSnd_SetZoneBGM(fieldSound, gameData, wk->spawn.zoneId, wk->season);
        FieldSnd_FadeInImmediate(fieldSound, gameData);
        GameEvent_ChainNext(event, EventMapChangeCore_Create(wk, 0));
        (*state)++;
        break;
    case 3:
        GameEvent_ChainNext(event, EventPlayerSpinDown_Create(event, gsys, field));
        (*state)++;
        break;
    case 4:
        func_ov021_021bc550(Field_GetPlaceName(field), wk->spawn.zoneId);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEventReturnCode EventMapChangeUnionRoomExit_Callback(GameEvent *event, u32 *state, void *data) {
    EventMapChange *wk = data;
    GameSystem *gsys = wk->gsys;
    Field *field = wk->field;

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, func_ov021_021bfdd0(event, gsys, field, FALSE));
        (*state)++;
        break;
    case 1:
        GameEvent_ChainNext(event, EventMapChangeCore_Create(wk, 0));
        (*state)++;
        break;
    case 2:
        GFL_OvlUnload(OVERLAY_ID(12));
        GFL_OvlLoad(OVERLAY_ID(11));
        EventScriptCall_Start(event, 0x83a, NULL, 0, HEAPID_FIELDMAP);
        (*state)++;
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode EventUnionRoomWarp_Callback(GameEvent *event, u32 *state, void *data) {
    EventMapChange *wk = data;
    GameSystem *gsys = wk->gsys;
    Field *field = wk->field;

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, CallEventPrepareResidentActorsForZoneChange(gsys, field));
        (*state)++;
        break;
    case 1:
        GFL_OvlUnload(OVERLAY_ID(11));
        GFL_OvlLoad(OVERLAY_ID(12));
        GameEvent_ChainNext(event, EventMapChangeCore_Create(wk, 4));
        (*state)++;
        break;
    case 2:
        func_ov012_02173744(gsys);
        (*state)++;
        break;
    case 3:
        GameEvent_ChainNext(event, func_ov021_021bf66c(event, gsys, field));
        (*state)++;
        break;
    case 4:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


void InitMapChangeEvent(EventMapChange *wk, GameSystem *gsys) {
    GameData *gameData = GSYS_GetGameData(gsys);
    Field *field = GSYS_GetField(gsys);
    u8 season = GameData_GetSeason(gameData);

    wk->gsys = gsys;
    wk->gameData = gameData;
    wk->field = field;
    wk->zoneId = Field_GetPlayerStateZoneID(field);
    wk->unk40 = FALSE;
    wk->seasonChanged = FALSE;
    wk->prevSeason = season;
    wk->season = season;
    func_ov021_021aaaf0(Field_GetEncountSystem(wk->field), 1);
}

GameEvent *EventMapChangeWarp_CreateGrid(GameSystem *gsys, Field *field, u16 zoneId, const VecFx32 *pos, u16 dir,
                                         BOOL unk40) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventMapChangeWarp_Callback, sizeof(EventMapChange));
    EventMapChange *wk = GameEvent_GetData(event);

    InitMapChangeEvent(wk, gsys);
    dir = ConvDirToWarpDir(dir);
    CreateZoneChangeData(&wk->spawn, zoneId, dir, pos->x, pos->y, pos->z);
    wk->unk2C = 0;
    wk->unk40 = unk40;
    return event;
}

GameEvent *func_ov020_02184d90(GameSystem *gsys, u32 unk1, u8 zoneId, const u16 *pos, u16 dir, BOOL unk40) {
    GameEvent *event;
    EventMapChange *wk;

    event = GameEvent_Create(gsys, NULL, EventMapChangeWarp_Callback, sizeof(EventMapChange));
    wk = GameEvent_GetData(event);
    InitMapChangeEvent(wk, gsys);
    dir = ConvDirToWarpDir(dir);
    func_020141a8(&wk->spawn, zoneId, (s16)dir, pos[0], pos[3], (s16)pos[2]);
    wk->unk2C = 0;
    wk->unk40 = unk40;
    return event;
}

GameEvent *func_ov020_02184de8(GameSystem *gsys, u32 unk1, u8 zoneId, const u16 *pos, u16 dir, BOOL unk40) {
    GameEvent *event;
    EventMapChange *wk;

    event = GameEvent_Create(gsys, NULL, EventMapChange_Callback, sizeof(EventMapChange));
    wk = GameEvent_GetData(event);
    InitMapChangeEvent(wk, gsys);
    dir = ConvDirToWarpDir(dir);
    func_020141a8(&wk->spawn, zoneId, (s16)dir, pos[0], pos[3], (s16)pos[2]);
    wk->unk2C = 0;
    wk->unk40 = unk40;
    return event;
}


GameEvent *EventMapChangeQuicksand_Create(GameSystem *gsys, Field *field, const VecFx32 *effectPos, u16 zoneId,
                                          VecFx32 *pos) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventMapChangeQuicksand_Callback, sizeof(EventMapChange));
    EventMapChange *wk = GameEvent_GetData(event);

    InitMapChangeEvent(wk, gsys);
    CreateZoneChangeData(&wk->spawn, zoneId, 1, pos->x, pos->y, pos->z);
    VEC_Set(&wk->unk34, effectPos->x, effectPos->y, effectPos->z);
    wk->unk2C = 0;
    return event;
}

GameEvent *func_ov020_02184e94(Field *field, GameSystem *gsys) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventMapChangeEscapeRope_Callback, sizeof(EventMapChange));
    EventMapChange *wk = GameEvent_GetData(event);

    InitMapChangeEvent(wk, gsys);
    wk->spawn = *GameData_GetEscapeRopeZone(wk->gameData);
    func_ov020_02185f18(wk->gameData, &wk->spawn);
    wk->spawn.changeType = ZONE_SPAWN_CHANGE_TYPE_POSITION;
    wk->unk2C = 0;
    wk->unk40 = TRUE;
    return event;
}


GameEvent *EventMapChangeDig_Create(GameSystem *gsys) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventMapChangeDig_Callback, sizeof(EventMapChange));
    EventMapChange *wk = GameEvent_GetData(event);

    InitMapChangeEvent(wk, gsys);
    wk->spawn = *GameData_GetEscapeRopeZone(wk->gameData);
    func_ov020_02185f18(wk->gameData, &wk->spawn);
    wk->spawn.changeType = ZONE_SPAWN_CHANGE_TYPE_POSITION;
    wk->unk2C = 0;
    wk->unk40 = TRUE;
    return event;
}

GameEvent *EventMapChangeTeleport_Create(GameSystem *gsys) {
    u16 returnLocation = GetReturnLocationIdx(GSYS_GetGameData(gsys));
    GameEvent *event = GameEvent_Create(gsys, NULL, EventMapChangeTeleport_Callback, sizeof(EventMapChange));
    EventMapChange *wk = GameEvent_GetData(event);

    InitMapChangeEvent(wk, gsys);
    LoadZoneSpawnInfoCheckRail(&wk->spawn, GetRespawnZoneMainZone(returnLocation));
    wk->spawn.changeType = ZONE_SPAWN_CHANGE_TYPE_POSITION;
    wk->unk2C = 0;
    return event;
}

GameEvent *EventMapChangeDiveOut_Create(GameSystem *gsys) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventMapChangeDiveOut_Callback, sizeof(EventMapChange));
    EventMapChange *wk = GameEvent_GetData(event);

    InitMapChangeEvent(wk, gsys);
    wk->spawn = *GameData_GetNextZone(wk->gameData);
    wk->spawn.changeType = ZONE_SPAWN_CHANGE_TYPE_POSITION;
    wk->spawn.warpDir = WARP_DIR_DOWN;
    wk->unk2C = 0;
    return event;
}

GameEvent *EventMapChangeDiveIn_Create(GameSystem *gsys, u16 zoneId) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventMapChangeDiveIn_Callback, sizeof(EventMapChange));
    EventMapChange *wk = GameEvent_GetData(event);
    ZoneSpawnInfo returnSpawn;
    PlayerState *playerState;
    VecFx32 *pos;
    u16 currentZoneId;

    InitMapChangeEvent(wk, gsys);
    LoadZoneSpawnInfoCheckRail(&wk->spawn, zoneId);
    wk->unk2C = 0;
    wk->spawn.warpDir = WARP_DIR_UP;

    // Resurface where the dive started
    playerState = GameData_GetPlayerState(wk->gameData);
    pos = PlayerState_GetWPos(playerState);
    currentZoneId = PlayerState_GetZoneID(playerState);
    CreateZoneChangeData(&returnSpawn, currentZoneId, PlayerState_CalcDirection(playerState), pos->x, pos->y, pos->z);
    GameData_SetNextZone(wk->gameData, &returnSpawn);
    return event;
}

GameEvent *EventMapChangeWarpPad_Create(GameSystem *gsys, Field *field, u16 zoneId, const VecFx32 *pos, u16 dir) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventMapChangeWarpPad_Callback, sizeof(EventMapChange));
    EventMapChange *wk = GameEvent_GetData(event);

    InitMapChangeEvent(wk, gsys);
    dir = ConvDirToWarpDir(dir);
    CreateZoneChangeData(&wk->spawn, zoneId, dir, pos->x, pos->y, pos->z);
    wk->unk2C = 5;
    wk->unk40 = FALSE;
    return event;
}

GameEvent *EventUnionRoomWarp_Create(GameSystem *gsys, Field *field) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventUnionRoomWarp_Callback, sizeof(EventMapChange));
    EventMapChange *wk = GameEvent_GetData(event);

    InitMapChangeEvent(wk, gsys);
    CreateZoneChangeData(&wk->spawn, ZONE_UNION_ROOM, ConvDirToWarpDir(0), FX32_CONST(184), 0, FX32_CONST(248));
    wk->unk2C = 0;
    return event;
}

GameEvent *func_ov020_021850fc(GameSystem *gsys, Field *field) {
    GameData *gameData = GSYS_GetGameData(gsys);
    ZoneSpawnInfo *next = GameData_GetNextZone(gameData);
    GameEvent *event = GameEvent_Create(gsys, NULL, EventMapChangeUnionRoomExit_Callback, sizeof(EventMapChange));
    EventMapChange *wk = GameEvent_GetData(event);

    InitMapChangeEvent(wk, gsys);
    wk->spawn = *next;
    return event;
}

void func_ov020_021852f0(ZoneSpawnInfo *spawn, Field *field);


GameEvent *EventEntralinkWarpIn_Create(GameSystem *gsys, u16 zoneId, const VecFx32 *pos, u32 a3) {
    Field *field = GSYS_GetField(gsys);
    GameData *gameData = GSYS_GetGameData(gsys);
    VecFx32 spawnPos = *pos;
    ZoneSpawnInfo returnSpawn;
    ZoneSpawnInfo spawn;
    IntrudeComm *work;
    u32 value;
    s32 diff;
    u32 offset = 0;

    func_ov020_021852f0(&returnSpawn, field);
    GameData_SetEntralinkParentSpawnInfo(gameData, &returnSpawn);
    func_0200bfa4(getHighLinkBlockAddress(GameData_GetSaveControl(gameData)), func_02012fcc(gameData), FALSE);
    work = func_ov010_02158344(GSYS_GetGameCommSystem(gsys));
    if (work == NULL || func_ov010_02158590(gameData) == 0) {
        value = 0;
        diff = 0;
    } else {
        value = func_ov011_02176230(work, &work->unk_1AC);
        diff = value - func_02012b58(gameData);
        if (diff < 0) {
            diff += work->unk_902;
        }
        offset = 0x10000;
    }
    spawnPos.x += offset + (diff << 22);
    CreateZoneChangeData(&spawn, zoneId, ConvDirToWarpDir(0), spawnPos.x, spawnPos.y, spawnPos.z);
    return EventEntralinkWarpIn_CreateCore(gsys, field, &spawn, a3, value);
}


u32 func_ov021_021a3238(FieldPlayer *player);
void func_ov018_0217bcdc(IntrudeComm *comm, GameSystem *gsys, u16 param_3, u8 param_4, u8 param_5);
void func_02026484(GameCommSys *comm);
GameEvent *func_ov020_02185214(GameSystem *gsys) {
    GameData *gameData = GSYS_GetGameData(gsys);
    Field *field = GSYS_GetField(gsys);
    ZoneSpawnInfo spawn = *GameData_GetEntralinkParentSpawnInfo(gameData);
    GameEvent *event;
    HighLinkSave *highLink;
    GameCommSys *comm;

    if (func_02013dd0(spawn.zoneId) == TRUE) {
        LoadZoneSpawnInfoCheckRail(&spawn, spawn.zoneId);
    }
    event = EventEntralinkWarp_Create(gsys, field, &spawn);
    highLink = getHighLinkBlockAddress(GameData_GetSaveControl(gameData));
    func_0200bfa4(highLink, func_02012fcc(gameData), TRUE);
    comm = GSYS_GetGameCommSystem(gsys);
    func_0202648c(comm);
    if (GameCommSys_BootCheck(comm) == 0) {
        func_02026484(comm);
    }
    if (func_ov021_021a3238(Field_GetPlayer(field)) == 0) {
        func_ov018_0217bcdc(0, gsys, 0, 0, 0);
    }
    func_02012b78(gameData, 0);
    if (func_ov010_02158344(GSYS_GetGameCommSystem(gsys)) == NULL) {
        func_02012b64(gameData, 0);
        func_02012b8c(gameData, 0);
        GameData_SetLastSubscreen(gameData, 1);
    }
    return event;
}


void func_ov021_021a36bc(FieldPlayer *player, RailPosition *rail);
void func_ov020_021852f0(ZoneSpawnInfo *spawn, Field *field) {
    FieldPlayer *player = Field_GetPlayer(field);
    RailPosition rail;

    if (Field_GetResolvedControllerTypeID(field) == 0) {
        VecFx32 *pos = GetMModelWPosPtr(FieldPlayer_GetActor(player));
        CreateZoneChangeData(spawn, Field_GetPlayerStateZoneID(field), 1, pos->x, pos->y, pos->z);
        return;
    }
    func_ov021_021a36bc(player, &rail);
    func_020141a8(spawn, Field_GetPlayerStateZoneID(field), 2, rail.componentId, rail.posFront, rail.posSide);
}

BOOL IsWarpDestId256(ZoneWarp *warp);
BOOL GetIsZoneMatrix0(u16 zoneId);
FieldStatus *GameData_GetFieldStatus(GameData *gameData);
u32 FieldStatus_SetBusyFlag(FieldStatus *status, u32 flag);

GameEvent *func_ov020_02185360(GameSystem *gsys, Field *field, ZoneWarp *warp, u32 dir) {
    EventMapChange *wk;
    GameEvent *event;
    GameData *gameData = GSYS_GetGameData(gsys);
    ZoneSpawnInfo *remember;
    ZoneSpawnInfo *next;

    event = GameEvent_Create(gsys, NULL, EventMapChangeWarp_Callback, sizeof(EventMapChange));
    wk = GameEvent_GetData(event);
    InitMapChangeEvent(wk, gsys);
    wk->unk40 = TRUE;
    if (IsWarpDestId256(warp)) {
        next = GameData_GetNextZone(gameData);
        wk->spawn = *next;
    } else {
        SetupWarpParamByWarp(warp, &wk->spawn, dir);
    }
    wk->unk2C = GetWarpTransitionType(warp);
    remember = GetOutboundWarpRememberSpawnInfo(gameData);
    if (GetIsZoneMatrix0(remember->zoneId) == 1 && GetIsZoneMatrix0(wk->spawn.zoneId) == 0) {
        GameData_SetEscapeRopeZone(gameData, remember);
    }
    func_ov020_02185eb0(gameData, &wk->spawn);
    FieldStatus_SetBusyFlag(GameData_GetFieldStatus(gameData), 2);
    return event;
}


GameEventReturnCode func_ov020_02184410(GameEvent *event, u32 *state, void *data);
GameEvent *func_ov020_02185410(GameSystem *gsys, Field *field, ZoneWarp *warp, u32 dir) {
    EventMapChange *wk;
    GameEvent *event;

    GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, func_ov020_02184410, sizeof(EventMapChange));
    wk = GameEvent_GetData(event);
    InitMapChangeEvent(wk, gsys);
    wk->unk40 = TRUE;
    SetupWarpParamByWarp(warp, &wk->spawn, dir);
    wk->unk2C = GetWarpTransitionType(warp);
    return event;
}


GameEvent *EventMapChange_CreateGrid(GameSystem *gsys, Field *field, u8 mode, u16 zoneId, VecFx32 *pos, u16 dir) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventMapChange_Callback, sizeof(EventMapChange));
    EventMapChange *wk = GameEvent_GetData(event);

    InitMapChangeEvent(wk, gsys);
    dir = ConvDirToWarpDir(dir);
    CreateZoneChangeData(&wk->spawn, zoneId, dir, pos->x, pos->y, pos->z);
    wk->unk2C = 0;
    wk->mode = mode;
    return event;
}

GameEvent *EventMapChange_CreateGridDefault(GameSystem *gsys, Field *field, u16 zoneId, VecFx32 *pos, u16 dir) {
    return EventMapChange_CreateGrid(gsys, field, 0, zoneId, pos, dir);
}

void func_ov020_02185eb0(GameData *gameData, const ZoneSpawnInfo *spawn);

GameEvent *EventMapChange_CreateForFly(GameSystem *gsys, Field *field, u16 unused, ZoneSpawnInfo *spawn, u16 warpDir) {
    GameData *gameData = GSYS_GetGameData(gsys);
    GameEvent *event;
    EventMapChange *wk;

    SetPlayerSpecialState(GameData_GetPlayerState(gameData), 0);
    event = GameEvent_Create(gsys, NULL, EventMapChange_Callback, sizeof(EventMapChange));
    wk = GameEvent_GetData(event);
    InitMapChangeEvent(wk, gsys);
    wk->spawn = *spawn;
    wk->spawn.warpDir = warpDir;
    wk->unk2C = 0;
    wk->mode = 1;
    func_ov020_02185eb0(gameData, &wk->spawn);
    return event;
}


GameEvent *EventMapChangeFakeWarp_Create(GameSystem *gsys, Field *field, u16 zoneId, const VecFx32 *pos, u16 dir) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventMapChangeFakeWarp_Callback, sizeof(EventMapChange));
    EventMapChange *wk = GameEvent_GetData(event);

    InitMapChangeEvent(wk, gsys);
    dir = ConvDirToWarpDir(dir);
    CreateZoneChangeData(&wk->spawn, zoneId, dir, pos->x, pos->y, pos->z);
    wk->unk2C = 0;
    return event;
}

GameEvent *EventMapChangeEnding_Create(GameSystem *gsys, u16 zoneId, const VecFx32 *pos) {
    VecFx32 spawnPos = *pos;
    GameData *gameData = GSYS_GetGameData(gsys);
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov020_0218466c, sizeof(EventMapChange));
    EventMapChange *wk = GameEvent_GetData(event);
    s32 diff = 0;

    if (IsZoneEntralinkHub(zoneId) == TRUE) {
        IntrudeComm *work = func_ov010_02158344(GSYS_GetGameCommSystem(gsys));

        if (work != NULL) {
            diff = work->unk_070.unk_16_0;
            diff = diff - func_02012b58(gameData);
            if (diff < 0) {
                diff += work->unk_902;
            }
        }
        spawnPos.x += diff << 22;
    }
    InitMapChangeEvent(wk, gsys);
    CreateZoneChangeData(&wk->spawn, zoneId, ConvDirToWarpDir(0), spawnPos.x, spawnPos.y, spawnPos.z);
    wk->unk2C = 0;
    wk->unk40 = 0;
    return event;
}

void func_ov010_0216446c(GameData *gameData, u16 zoneId);
void FieldMapControl_LoadBlackoutZone(GameSystem *gsys) {
    GameData *gameData = GSYS_GetGameData(gsys);
    ZoneSpawnInfo spawn;
    ZoneSpawnInfo escapeRope;

    SetupTeleportZoneChange(GetReturnLocationIdx(gameData), &spawn);
    LoadZoneSpawnInfoCheckRail(&escapeRope, GetRespawnZoneMainZone(GetReturnLocationIdx(gameData)));
    GameData_SetEscapeRopeZone(gameData, &escapeRope);
    FieldMapControl_DeleteAllActors(gsys);
    FieldMapControl_LoadZone(gsys, spawn.zoneId);
    FieldMapControl_InitSpawn(gsys, &spawn);
    func_0202735c(spawn.zoneId, gameData);
    func_ov010_02168eec(gameData);
    func_ov010_0216446c(gameData, spawn.zoneId);
}


GameEventReturnCode EventMapChangeBlackout_Callback(GameEvent *event, u32 *state, void *data) {
    EventMapChangeBlackout *wk = data;

    switch (*state) {
    case 0:
        FieldMapControl_LoadBlackoutZone(wk->gsys);
        (*state)++;
        break;
    case 1:
        SetPlayerSpecialState(GameData_GetPlayerState(GSYS_GetGameData(wk->gsys)), 0);
        func_0202c110(ISS_GetSwitchSys(GameSystem_GetISS(wk->gsys)));
        GameEvent_ChainNext(event, EventBGMChange_Create(wk->gsys,
                                                         GetMapBGMIDByPlayerState2(wk->gameData, wk->spawn.zoneId,
                                                                                   GameData_GetSeason(wk->gameData)),
                                                         0, 60));
        (*state)++;
        break;
    case 2:
        GameEvent_ChainNext(event, EventFieldOpen_Create(wk->gsys));
        (*state)++;
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *EventMapChangeBlackout_Create(GameSystem *gsys) {
    GSYS_GetGameData(gsys);
    GameEvent *event = GameEvent_Create(gsys, NULL, EventMapChangeBlackout_Callback, sizeof(EventMapChangeBlackout));
    EventMapChangeBlackout *wk = GameEvent_GetData(event);

    wk->gsys = gsys;
    wk->gameData = GSYS_GetGameData(gsys);
    SetupTeleportZoneChange(GetReturnLocationIdx(wk->gameData), &wk->spawn);
    return event;
}

u16 ConvWarpDirToAngle(s32 warpDir) {
    switch (warpDir) {
    default:
    case WARP_DIR_UP:
        return 0;
    case WARP_DIR_LEFT:
        return 0x4000;
    case WARP_DIR_DOWN:
        return 0x8000;
    case WARP_DIR_RIGHT:
        return 0xc000;
    }
}

void SetupZoneChangeSpawn(EventData *eventData, ZoneSpawnInfo *next, ZoneSpawnInfo *spawn) {
    if (next->changeType == ZONE_SPAWN_CHANGE_TYPE_POSITION) {
        *spawn = *next;
    } else if (!SetupZoneWarpArrival(eventData, spawn, next->warpId, next->posWeightBits)) {
        LoadZoneSpawnInfoCheckRail(spawn, next->zoneId);
    }
}

void func_ov010_021643e8(GameData *gameData, u16 zoneId);
void SetupZoneChangeSpawn(EventData *eventData, ZoneSpawnInfo *next, ZoneSpawnInfo *spawn);
void GameData_SetNextZone(GameData *gameData, ZoneSpawnInfo *spawn);
ZoneSpawnInfo *GetOutboundWarpRememberSpawnInfo(GameData *gameData);
BOOL GetZoneSpawnInfoIsRail(ZoneSpawnInfo *spawn);
void SetCurrentTeleportOrDeathZone(GameData *gameData, u16 respawnLocation);
u32 GetRespawnLocationIndexForRespawnZone(s32 zoneId);
void FieldScript_CallOnZoneInit(GameSystem *gsys, u32 a1);
void func_ov020_021859fc(GameData *gameData, const ZoneSpawnInfo *spawn);
void ResetWeather(GameSystem *gsys, u32 zoneId);
void func_ov010_02168eec(GameData *gameData);

void FieldMapControl_InitSpawn(GameSystem *gsys, ZoneSpawnInfo *next) {
    GameData *gameData = GSYS_GetGameData(gsys);
    PlayerState *playerState = GameData_GetPlayerState(gameData);
    EventData *eventData = GameData_GetEventData(gameData);
    ZoneSpawnInfo spawn;
    u16 respawnLocation;
    s32 cacheIdx;
    MMSys *mmSys;

    FieldStatus_SetNewLoadFlag(GameData_GetFieldStatus(gameData), TRUE);
    SetupZoneChangeSpawn(eventData, next, &spawn);
    if (spawn.changeType == ZONE_SPAWN_CHANGE_TYPE_3) {
        GameData_SetNextZone(gameData, GetOutboundWarpRememberSpawnInfo(gameData));
    }

    PlayerState_SetZoneID(playerState, spawn.zoneId);
    PlayerState_SetRotation(playerState, ConvWarpDirToAngle(spawn.warpDir));
    if (!GetZoneSpawnInfoIsRail(&spawn)) {
        PlayerState_SetWPos(playerState, &spawn.pos.vec);
        PlayerState_SetIsRail(playerState, FALSE);
    } else {
        PlayerState_SetRailPos(playerState, &spawn.pos.rail);
        PlayerState_SetIsRail(playerState, TRUE);
    }

    ISS_ChangeZone(GameSystem_GetISS(gsys), spawn.zoneId);
    SetGameDataNowSpawnZone(gameData, &spawn);
    respawnLocation = GetRespawnLocationIndexForRespawnZone(spawn.zoneId);
    if (respawnLocation != 0) {
        SetCurrentTeleportOrDeathZone(gameData, respawnLocation);
    }
    GameData_SetGimmickByZone(gameData, spawn.zoneId);
    func_ov010_02168eec(gameData);
    func_ov010_021643e8(gameData, spawn.zoneId);
    SetTeleportZoneDiscover(gameData, spawn.zoneId);
    FieldScript_CallOnZoneInit(gsys, 4);
    CallSpawnAllZoneNPCs(gameData, next);
    GameData_UpdateFlashStatus(gameData, spawn.zoneId);
    func_ov020_021859fc(gameData, &spawn);
    ResetWeather(gsys, spawn.zoneId);

    cacheIdx = GetZoneNPCInfoCacheIdx(spawn.zoneId);
    mmSys = GameData_GetMMSys(gameData);
    if (cacheIdx < 0x10) {
        LoadMModelSystemInfoCache(mmSys, cacheIdx);
    } else {
        FldActSys_ClearCache(mmSys);
    }
}


void CallSpawnAllZoneNPCs(GameData *gameData, const ZoneSpawnInfo *spawn) {
    EventData *eventData = GameData_GetEventData(gameData);
    u32 count = GetZoneNPCsCount(eventData);

    if (count != 0) {
        EventWork *eventWork = GameData_GetEventWork(gameData);
        MMSys *mmSys = GameData_GetMMSys(gameData);

        SpawnAllZoneNPCs(mmSys, GetZoneNPCs(eventData), spawn->zoneId, count, eventWork);
    }
}


void GameData_DeleteAllActors(GameData *gameData, const ZoneSpawnInfo *spawn) {
    MMSys *mmSys;
    EventWork *eventWork;
    CityState *city;
    ZoneNPC *npcs;
    u16 *ids;
    u32 count;
    BOOL flag;

    if (func_02013dd0(spawn->zoneId)) {
        flag = func_0201444c(GameData_GetFieldStatus(gameData));
        mmSys = GameData_GetMMSys(gameData);
        eventWork = GameData_GetEventWork(gameData);
        if (flag == FALSE) {
            city = func_020127c4(gameData);
        } else {
            city = func_020127d0(gameData, 0);
        }
        npcs = func_ov020_02186538(city, flag, TRUE);
        count = func_ov020_02186278(city, flag);
        if (npcs != NULL && count != 0) {
            SpawnAllZoneNPCs(mmSys, npcs, spawn->zoneId, count, eventWork);
        }
        if (npcs != NULL) {
            GFL_HeapFree(npcs);
        }

        ids = func_020127e0(gameData);
        count = func_ov020_02186854(ids);
        npcs = func_ov020_021868b8(ids, flag, city->city, TRUE);
        if (npcs != NULL && count != 0) {
            SpawnAllZoneNPCs(mmSys, npcs, spawn->zoneId, count, eventWork);
        }
        if (npcs != NULL) {
            GFL_HeapFree(npcs);
        }
    }
}


void func_ov020_021859f0(GameData *gameData) {
    FldActSys_DeleteAllActors(GameData_GetMMSys(gameData));
}

void func_ov020_02186470(GameData *gameData, const ZoneSpawnInfo *spawn);
void func_ov020_021864dc(CityState *city);

void func_ov020_021859fc(GameData *gameData, const ZoneSpawnInfo *spawn) {
    func_ov020_02186470(gameData, spawn);
    if (func_02013dd0(spawn->zoneId)) {
        GameData_DeleteAllActors(gameData, spawn);
        if (!func_0201444c(GameData_GetFieldStatus(gameData))) {
            func_ov020_021864dc(func_020127c4(gameData));
        }
    }
}


void GameData_UpdateFlashStatus(GameData *gameData, u16 zoneId) {
    FieldStatus *status = GameData_GetFieldStatus(gameData);
    u32 flags = GetZoneFlashFlags(zoneId);

    if ((flags & 1) && FieldStatus_CheckFlashUsed(status)) {
        flags &= ~1;
        flags |= 2;
    }
    FieldStatus_SetFlashPerms(status, flags);
}

void EventData_LoadZone(EventData *eventData, u16 zoneId, u8 season);


void FieldMapControl_LoadZone(GameSystem *gsys, u16 zoneId) {
    GameData *gameData = GSYS_GetGameData(gsys);
    EventData *eventData = GameData_GetEventData(gameData);
    MapMatrix *matrix;
    u8 subscreen;

    GSYS_GetField(gsys);
    if (GameData_IsForceSeasonSync(gameData) == 1 && IsZoneEntralinkHub(zoneId) == 0) {
        func_02014450(GameData_GetFieldStatus(gameData), TRUE);
    } else {
        func_02014450(GameData_GetFieldStatus(gameData), FALSE);
    }

    if (GetZoneIsUnionRoom(zoneId)) {
        func_020129b8(gameData, 2);
    } else if (IsZone150Or151(zoneId) || func_02013df0(zoneId) || IsZoneRoyalUnova(zoneId)) {
        func_020129b8(gameData, 5);
    } else {
        if (GameData_IsForceSeasonSync(gameData) == 1 && func_020129ac(gameData) != 3) {
            func_020129b8(gameData, 3);
        } else if (GameData_IsForceSeasonSync(gameData) == 0 && func_020129ac(gameData) == 3) {
            func_020129b8(gameData, 0);
        }
        subscreen = func_020129ac(gameData);
        if (subscreen != 3 && subscreen != 4 && subscreen != 6) {
            func_020129b8(gameData, 0);
        }
    }

    EventData_LoadZone(eventData, zoneId, GameData_GetSeason(gameData));
    matrix = GetMapMatrixSystem(gameData);
    MapMatrix_Load(matrix, GetZoneMatrixId(zoneId), zoneId, HEAPID_TAIL(HEAPID_USER));
    MapMatrix_Patch(matrix, gsys, HEAPID_TAIL(HEAPID_USER));
    GameData_UpdateFlashStatus(gameData, zoneId);
}


void func_ov020_021859f0(GameData *gameData);


void FieldMapControl_DeleteAllActors(GameSystem *gsys) {
    GameData *gameData = GSYS_GetGameData(gsys);

    GSYS_GetField(gsys);
    func_ov020_021859f0(gameData);
}

void Field_SetPlayerHidden(Field *field, BOOL hidden) {
    SetActorHidden(FieldPlayer_GetActor(Field_GetPlayer(field)), hidden);
}

GimmickState *GameData_GetGimmickState(GameData *gameData);

void GimmickState_Reset(GimmickState *gimmick);
void *GimmickState_SetID(GimmickState *gimmick, u32 gimmickId);


void GameData_SetGimmickByZone(GameData *gameData, s32 zoneId) {
    GimmickState *gimmick = GameData_GetGimmickState(gameData);
    u32 i;
    ZoneGimmick *gimmicks;
    ArcTool *handle;
    u32 count;

    GimmickState_Reset(gimmick);
    handle = GFL_ArcSysCreateFileHandle(ARCID_GIMMICK_TBL, HEAPID_TAIL(HEAPID_USER));
    gimmicks = GFL_ArcToolReadHeapNew(handle, 0, HEAPID_TAIL(HEAPID_USER));
    count = GFL_ArcToolGetDataLength(handle, 0) / sizeof(ZoneGimmick);
    for (i = 0; i < count; i++) {
        if (zoneId == gimmicks[i].zoneId) {
            GimmickState_SetID(gimmick, gimmicks[i].gimmickId);
            break;
        }
    }
    GFL_HeapFree(gimmicks);
    GFL_ArcToolFree(handle);
}


GameEvent *EventEntralinkWarpIn_CreateCore(GameSystem *gsys, Field *field, ZoneSpawnInfo *spawn, u32 a3, u32 a4) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventEntralinkWarpIn_Callback, sizeof(EventEntralinkWarp));
    EventEntralinkWarp *wk = GameEvent_GetData(event);

    wk->spawn = *spawn;
    wk->gsys = gsys;
    wk->gameData = GSYS_GetGameData(gsys);
    wk->field = field;
    wk->unk28 = a3;
    wk->unk2C = a4;
    return event;
}

GameEvent *EventEntralinkWarp_Create(GameSystem *gsys, Field *field, ZoneSpawnInfo *spawn) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventEntralinkWarp_Callback, sizeof(EventEntralinkWarp));
    EventEntralinkWarp *wk = GameEvent_GetData(event);

    wk->spawn = *spawn;
    wk->gsys = gsys;
    wk->gameData = GSYS_GetGameData(gsys);
    wk->field = field;
    return event;
}

GameEvent *func_ov021_021be9f0(GameSystem *gsys, Field *field, u32 unk2, u32 unk3, u32 unk4);
GameEventReturnCode EventEntralinkWarp_Callback(GameEvent *event, u32 *state, void *data) {
    EventEntralinkWarp *wk = data;
    GameSystem *gsys = wk->gsys;
    Field *field = wk->field;

    switch (*state) {
    case 0:
        GFL_SndSEPlay(SEQ_SE_FLD_131);
        EncEff_StartEvent(Field_GetEncEff(field), event, 0x24);
        (*state)++;
        break;
    case 1: {
        GameEvent *mapChange = GameEvent_Create(gsys, NULL, EventMapChange_Callback, sizeof(EventMapChange));
        EventMapChange *mapChangeWk = GameEvent_GetData(mapChange);

        InitMapChangeEvent(mapChangeWk, gsys);
        mapChangeWk->spawn = wk->spawn;
        mapChangeWk->unk2C = 0;
        mapChangeWk->unk40 = FALSE;
        GameEvent_ChainNext(event, mapChange);
        (*state)++;
        break;
    }
    case 2:
        GameEvent_ChainNext(event, func_ov021_021be9f0(gsys, field, 0, 0, 2));
        (*state)++;
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

BOOL func_ov021_021d2750(GameSystem *gsys);
GameEventReturnCode EventEntralinkWarpIn_Callback(GameEvent *event, u32 *state, void *data) {
    EventEntralinkWarp *wk = data;
    GameCommSys *comm = GSYS_GetGameCommSystem(wk->gsys);
    IntrudeComm *intrude;
    u32 flag;
    u32 unk7;

    switch (*state) {
    case 0:
        intrude = func_ov010_02158344(comm);
        flag = FALSE;
        unk7 = 0;
        if (intrude != NULL) {
            unk7 = func_ov018_0217c070(intrude);
            if (func_ov010_02158630(comm) == 3) {
                flag = TRUE;
            }
        }
        if (unk7 == 0 && func_ov021_021d2750(wk->gsys)) {
            EventScriptCall_Start(event, flag == FALSE ? 0x279a : 0x279b, NULL, 0, Field_GetHeapID(wk->field));
            if (func_ov010_02158590(wk->gameData) == 0) {
                func_02026400(comm);
            }
            *state = 1;
        } else {
            EventScriptCall_Start(event, unk7 == 0 ? 0x279c : 0x279d, NULL, 0, Field_GetHeapID(wk->field));
            *state = 3;
        }
        break;
    case 1:
        if (func_ov010_02158590(wk->gameData) == 0) {
            if (GameCommSys_BootCheck(comm) != 0) {
                break;
            }
            func_02026484(comm);
        }
        if (wk->unk28 == 1 && func_ov010_02158630(comm) != 3) {
            EventScriptCall_Start(event, 0x279f, NULL, 0, Field_GetHeapID(wk->field));
            *state = 3;
            break;
        }
        intrude = func_ov010_02158344(comm);
        if (intrude != NULL) {
            func_ov011_0217574c(intrude, 1);
            func_ov011_02175710(intrude, wk->unk2C);
            if (wk->unk28 == 1) {
                func_ov011_02175b4c(intrude, &intrude->unk_1AC);
            }
        }
        func_02012b78(wk->gameData, 1);
        func_02012b64(wk->gameData, func_02039338());
        GameEvent_ChainNext(event, EventEntralinkWarp_Create(wk->gsys, wk->field, &wk->spawn));
        *state = 2;
        break;
    case 2:
        intrude = func_ov010_02158344(comm);
        if (intrude != NULL) {
            func_ov011_0217574c(intrude, 0);
        }
        *state = 3;
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


void func_ov020_02185eb0(GameData *gameData, const ZoneSpawnInfo *spawn) {
    ZoneSpawnInfo *remember = GetOutboundWarpRememberSpawnInfo(gameData);
    ZoneSpawnInfo *now = GetGameDataNowSpawnZone(gameData);
    ZoneSpawnInfo escapeRope;

    if (func_02014068(spawn->zoneId)) {
        if (now->zoneId == ZONE_POKEMON_LEAGUE) {
            GameData_SetEscapeRopeZone(gameData, remember);
        } else if (!func_02014068(now->zoneId)) {
            LoadZoneSpawnInfoCheckRail(&escapeRope, ZONE_VICTORY_ROAD);
            GameData_SetEscapeRopeZone(gameData, &escapeRope);
        }
    }
    if (now->zoneId == ZONE_DESERT_RESORT_2 && func_02013c50(spawn->zoneId)) {
        GameData_SetEscapeRopeZone(gameData, remember);
    }
}


// An Escape Rope used outside the Abyssal Ruins leads to the next zone instead
// pokebw2's AdjustEscapeRopeSpawn
void func_ov020_02185f18(GameData *gameData, ZoneSpawnInfo *spawn) {
    if (IsZoneAbyssalRuinsOutside(GetGameDataNowSpawnZone(gameData)->zoneId)) {
        *spawn = *GameData_GetNextZone(gameData);
    }
}

void GameData_AdjustPlayerStateOnDiveOut(GameData *gameData) {
    PlayerState *playerState = GameData_GetPlayerState(gameData);

    if (FieldPlayerState_GetExState(playerState) != 3) {
        SetPlayerSpecialState(playerState, 0);
    } else {
        SetPlayerSpecialState(playerState, 2);
    }
}


MsgData *GFL_MsgSysLoadData(BOOL preload, u16 arcId, u16 fileId, HeapID heapId);
StrBuf *GFL_MsgDataLoadStrbufNew(MsgData *msgData, u32 messageId);
void GFL_StrBufStoreString(const StrBuf *strbuf, u16 *dest, u32 size);
void GFL_StrBufFree(StrBuf *strbuf);
void GFL_MsgDataFree(MsgData *msgData);
void func_02008f88(MailData *mail, const u16 *name);
void func_02008fa0(MailData *mail, u8 gender);
void func_02008fa8(MailData *mail, u32 value);
void func_02008fb4(MailData *mail, u8 region);
void func_02008fbc(MailData *mail, u8 gameVersion);

static const PMSData data_ov020_02187dac = { 5, 0xa, { 0x0675, 0xffff } };
static const PMSData data_ov020_02187da4 = { 5, 9, { 0x0582, 0xffff } };
static const PMSData data_ov020_02187db4 = { 5, 0x14, { 0x0582, 0x080a } };


void func_ov020_02185f6c(GameData *gameData) {
    MsgData *msgData = GFL_MsgSysLoadData(FALSE, 2, 0xbe, HEAPID_USER);
    StrBuf *strbuf = GFL_MsgDataLoadStrbufNew(msgData, 0x3b);
    u16 name[16];
    MailData *mail;
    MailData *mails;

    GFL_StrBufStoreString(strbuf, name, 0x10);
    mail = CreateMailData(HEAPID_USER);
    func_02008f88(mail, name);
    func_02008fa0(mail, 1);
    func_02008fa8(mail, 3);
    func_02008fb4(mail, 2);
    func_02008fbc(mail, GAME_VERSION);
    func_02008fd8(mail, &data_ov020_02187da4, 0);
    func_02008fd8(mail, &data_ov020_02187dac, 1);
    func_02008fd8(mail, &data_ov020_02187db4, 2);
    func_02008fc4(mail, 0x58f);
    mails = func_02008fec(gameData);
    func_0200903c(mails, 0, 0, mail);
    GFL_HeapFree(mail);
    GFL_StrBufFree(strbuf);
    GFL_MsgDataFree(msgData);
}


// Applies the form changes of party Pokemon that happen at night. In the base game, only Sky Forme Shaymin changes,
// reverting to Land Forme.
// pokebw2's GameData_UpdatePartyForTimeOfDay
void func_ov020_0218601c(GameData *gameData) {
    SaveControl *save = GameData_GetSaveControl(gameData);
    PokeParty *party = SaveControl_GetPokePartySave(save);
    u8 season = GameData_GetSeason(gameData);

    func_ov010_0216a358(gameData, party, &getSaveAdventureTimeBlock(save)->time, season);
}

void *func_ov020_0218604c(void) {
    return GFL_HeapAllocate(0x8004, 0x1000, FALSE, "event_mapchange.c", 0xe35);
}

void *func_ov020_0218606c(void) {
    return GFL_HeapAllocate(0x8004, 0x1000, FALSE, "event_mapchange.c", 0xe3a);
}


void func_ov020_0218608c(void) {
    return;
}

s32 func_ov020_02186090(FieldActor *actor) {
    u16 uid = GetActorUID(actor);
    return uid - 0x80;
}

void func_ov020_0218609c(CityState *city) {
    int i;

    for (i = 0; i < CITY_RESIDENT_MAX; i++) {
        func_ov010_02161e78(&city->residents[i]);
    }
}

void func_ov020_021860b8(CityState *city) {
    int i;

    for (i = 0; i < CITY_RESIDENT_MAX; i++) {
        func_ov010_02161e78(&city->visitors[i]);
    }
}

void *func_ov020_02186b60(CityResident *residents);
CityResidentData *func_ov020_021869e0(CityState *city, u32 random, void *handle, u32 *out);
void func_ov020_02186b84(void *slot, u8 unk, PlayerInfo *player, u16 value, BOOL flag);
u32 GFL_RandomLC(u32 max);

void func_ov020_021860d4(CityState *city, PlayerInfo *player, u16 id) {
    u32 selected;
    int i;
    void *slot;
    void *handle;
    CityResidentData *data;
    BOOL flag;
    u32 random;
    u32 param;
    u32 max = 0x1ce;

    city->initialized = 1;
#if GAME_VERSION == VERSION_WHITE
    city->city = 1;
#else
    city->city = 0;
#endif
    city->unk4 = getIDAsUInt(player);
    handle = func_ov020_021867c4(0, HEAPID_TAIL(id));
    func_ov020_0218682c(handle);
    for (i = 0; i < CITY_RESIDENT_MAX; i++) {
        random = GFL_RandomLC(max);
        slot = func_ov020_02186b60(city->residents);
        data = func_ov020_021869e0(city, random, handle, &selected);
        if (city->city == 0) {
            param = data->unk6;
            flag = TRUE;
        } else {
            param = data->unk4;
            flag = FALSE;
        }
        func_ov020_02186b84(slot, selected, player, param, flag);
        max -= data->unk2C;
    }
    for (i = 0; i < 3; i++) {
        random = GFL_RandomLC(max);
        slot = func_ov020_02186b60(city->visitors);
        data = func_ov020_021869e0(city, random, handle, &selected);
        func_ov020_02186b84(slot, selected, player, 0, flag);
        max -= data->unk2C;
    }
    func_ov020_02186804(handle);
}


BOOL func_ov020_021861bc(CityState *city) {
    int i;

    if (city->initialized == 1 && city->city >= 2) {
        return FALSE;
    }
    for (i = 0; i < CITY_RESIDENT_MAX; i++) {
        if (!func_ov010_02161e84(&city->residents[i])) {
            return FALSE;
        }
    }
    return TRUE;
}

void func_ov020_021866f8(CityResident *resident);
u32 getIDAsUInt(PlayerInfo *info);


void func_ov020_021861f4(CityState *city, PlayerInfo *player) {
    int i;
    u16 version;
    u32 id;

    if (city->initialized > 1) {
        city->initialized = 1;
    }
    if (city->initialized == 0 && city->city != 2) {
        city->city = 2;
    }
    if (city->initialized == 1 && city->city >= 2) {
        version = func_0200855c(player);
        id = getIDAsUInt(player);
        if (func_ov010_021584f4(version, id) == 0x15) {
            city->city = 0;
        } else {
            city->city = 1;
        }
    }
    for (i = 0; i < CITY_RESIDENT_MAX; i++) {
        func_ov020_021866f8(&city->residents[i]);
    }
}


// pokebw2's CityState_IsSet
BOOL func_ov020_0218625c(CityState *state) {
    if (!state->initialized) {
        return FALSE;
    }
    if (func_ov020_021861bc(state)) {
        return TRUE;
    }
    return FALSE;
}
