// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/fieldmap.c, ported by tools/port_file.py.
#include "types.h"
#include "constants/sound.h"
#include "field/encounter.h"
#include "field/encounter_effect.h"
#include "field/event_data.h"
#include "field/event_mapchange.h"
#include "field/festival.h"
#include "field/field.h"
#include "field/field_msgbg.h"
#include "field/field_actor.h"
#include "field/field_async_proc.h"
#include "field/field_camera.h"
#include "field/field_controller.h"
#include "field/field_daycare.h"
#include "field/field_disp_control.h"
#include "field/field_effect.h"
#include "field/field_environment.h"
#include "field/field_event.h"
#include "field/field_exp_obj.h"
#include "field/field_fog.h"
#include "field/field_g3d_mapper.h"
#include "field/field_wfbc.h"
#include "field/field_g3dobj.h"
#include "field/field_internal.h"
#include "field/field_map.h"
#include "field/field_nogrid_mapper.h"
#include "field/field_palace.h"
#include "field/field_player.h"
#include "field/field_prop.h"
#include "field/field_rail.h"
#include "field/field_scene_area.h"
#include "field/field_script.h"
#include "field/field_skill_map_eff.h"
#include "field/field_sound.h"
#include "field/field_status.h"
#include "field/field_task.h"
#include "field/field_weather.h"
#include "field/fieldmap_ctrl_hybrid.h"
#include "field/intrude_work.h"
#include "field/medal.h"
#include "field/player_state.h"
#include "field/pleasure_boat.h"
#include "field/scrcmd_ochiba.h"
#include "field/subscreen.h"
#include "field/union_eff.h"
#include "field/union_main.h"
#include "field/wbt.h"
#include "field/zone.h"
#include "field/zone_change.h"
#include "gfl/arc.h"
#include "gfl/blact.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/rtc_cache.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "nitro/mi.h"
#include "save/box.h"
#include "save/medal_box.h"
#include "save/records.h"
#include "save/save_control.h"
#include "system/area_data.h"
#include "system/game_beacon.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/iss_sys.h"
#include "system/main.h"
#include "system/rtc.h"
#include "system/season.h"
#include "system/zone_weather.h"
#include "gfl/calctool.h"
#include "gfl/wm_icon.h"
#include "field/weather.h"
#include "field/ov021_021a0448.h"
#include "field/fld_particle.h"
#include "field/intrude_field.h"



// A kind of map: the setup of its mapper and its controller, whether it takes its chunks from the zone's map matrix,
// and the size of its heap
// Black's order (Black 2's has heapSize first)
typedef struct {
    FieldG3DMapperConfig mapperConfig;
    const FieldmapCtrlVTable *ctrlVTable;
    BOOL useMapMatrix;
    u32 heapSize;
} FieldMapConfig;

extern const FieldmapCtrlVTable data_ov021_021d88f8;
extern const FieldmapCtrlVTable data_ov021_021d8ad8;
extern const FieldmapCtrlVTable data_ov021_021d8aec;

static const FieldMapConfig MAP_CONFIGS[21] = {
#include "asset/fieldmap_MAP_CONFIGS.inc"
};

// A step of the field: 0 to run again next frame, 1 to go on to the next routine, 2 when the field ends
typedef u32 (*FieldRoutine)(GameSystem *gsys, Field *field);
typedef void (*FieldRenderFunc)(Field *field);

u32 func_ov021_021880c8(GameSystem *gsys, Field *field);
u32 func_ov021_021881d8(GameSystem *gsys, Field *field);
u32 func_ov021_021885e8(GameSystem *gsys, Field *field);
u32 func_ov021_02188758(GameSystem *gsys, Field *field);
u32 func_ov021_02188910(GameSystem *gsys, Field *field);
u32 func_ov021_02188990(GameSystem *gsys, Field *field);
u32 func_ov021_02188ad4(GameSystem *gsys, Field *field);
void func_ov021_021897dc(Field *field);
void func_ov021_02189810(Field *field);
void func_ov021_02189998(Field *field);
void func_ov021_021899c4(Field *field);
static const FieldRenderFunc FIELD_RENDER_FUNCS_PHASE2[3] = { func_ov021_02189810, func_ov021_02189998, func_ov021_021899c4 };
static const VecFx32 DEFAULT_FIELDCAMERA_POS = { 0, 0x40000, 0x80000 };
static const FieldRenderFunc FIELD_RENDER_FUNCS_PHASE1[3] = { func_ov021_021897dc, NULL, NULL };
static const BGSysVRAMConfig data_ov021_021d3688 = {
#include "asset/fieldmap_data_ov021_021d3688.inc"
};
static const VecFx32 DEFAULT_FIELDCAMERA_TGT = { 0, 0, 0 };
// pokebw2's FIELDMAP_ROUTINES
static const FieldRoutine data_ov021_021d36b8[6][2] = {
    { func_ov021_021880c8, func_ov021_021880c8 },
    { func_ov021_021881d8, func_ov021_021881d8 },
    { func_ov021_021885e8, func_ov021_021885e8 },
    { func_ov021_02188758, func_ov021_02188910 },
    { func_ov021_02188990, func_ov021_02188990 },
    { func_ov021_02188ad4, func_ov021_02188ad4 },
};
static const ClActSysSetup data_ov021_021d3604 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 72, 64, 64, 64, 16, 16 };
static const BGSysLCDConfig FIELD_LCD_CONFIG = { 1, 0, 0, 1 };
static const GXRgb FIELD_STD_PALETTE[16] = {
#include "asset/fieldmap_FIELD_STD_PALETTE.inc"
};
static const u32 FIELD_DEFAULT_FOG_TABLE[8] = { 0x0c080400, 0x1c181410, 0x2c282420, 0x3c383430, 0x4c484440, 0x5c585450, 0x6c686460, 0x7c787470 };
static const GXRgb FIELD_DEFAULT_EDGE_COLOR_TABLE[8] = { 0x294a, 0x294a, 0x294a, 0x294a, 0x294a, 0x294a, 0x294a, 0x294a };
static const LightSetup data_ov021_021d3660[4] = {
#include "asset/fieldmap_data_ov021_021d3660.inc"
};

static const LightSetupList FIELD_LIGHT_SETUP = { data_ov021_021d3660, 4 };
// The edge colors of the area, loaded from archive 15
static GXRgb g_FieldEdgeColorTable[8];



BOOL func_ov021_02189ae8(u16 arg);
void func_ov021_0218958c(Field *field, u32 zoneId);
void func_ov021_02189684(Field *field, u32 zoneId);
void func_ov021_021c1c4c(void *ci);
void func_ov021_021903e0(FieldMsgBG *param_1);
Field *Field_Create(GameSystem *gsys, HeapID heapId) {
    Field *field = GFL_HeapAllocate(heapId, sizeof(Field), TRUE, "fieldmap.c", 0x209);
    u16 areaId;

    field->heapId = heapId;
    field->routineState = 0;
    field->gameSystem = gsys;
    field->gameData = GSYS_GetGameData(gsys);
    field->zoneId = PlayerState_GetZoneID(GSYS_GetPlayerState(gsys));
    areaId = ZoneData_GetAreaID(field->zoneId);
    field->areaData = AreaData_Create(heapId, areaId, GameSystem_GetSeason(gsys));
    field->ctrlVTable = GetZoneFieldmapCtrlVTable(field->zoneId);
    return field;
}



void Field_Free(Field *field) {
    GameSystem *gsys = field->gameSystem;

    AreaData_Free(field->areaData);
    GFL_HeapFree(field);
    GSYS_SetField(gsys, NULL);
    func_020371ac(0, 0);
}

FieldG3DMapper *func_ov021_0218c9e8(HeapID heapId, u8 season);
FieldTaskManager *func_ov021_021c6d10(u32 count, HeapID heapId);
// pokebw2's FieldRoutine_Open
u32 func_ov021_021880c8(GameSystem *gsys, Field *field) {
    HeapID heapId = field->heapId;

    GFL_BGSysSetVRAMBanks(&data_ov021_021d3688);
    FieldG2D_Init(field);
    ClActSys_Create(&data_ov021_021d3604, &data_ov021_021d3688, 0x89);
    GFL_BGSysSetBGEnabledA(0x10, TRUE);
    GFL_BGSysSetBGEnabledB(0x10, TRUE);
    BmpWin_InitAllocator(heapId);
    func_0201dd60();
    GFL_G3DSysCreate(0, 3, 0, 1, FX32_ONE, field->heapId, FieldG3D_InitCallback);
    FieldG3D_Init(field);
    field->g3DMapper = func_ov021_0218c9e8(field->heapId, GameData_GetSeason(field->gameData));
    field->asyncActorMatLoadTCB = GFL_VBlankTCBAdd(FldActSys_AsyncMatLoadTCBFunc, field, 0);
    field->tcbManagerHeap = GFL_HeapAllocate(heapId, GFL_TCBMgrCalcAllocSize(0x20), FALSE, "fieldmap.c", 0x26b);
    field->tcbManager = GFL_TCBMgrCreate(0x20, field->tcbManagerHeap);
    field->taskManager = func_ov021_021c6d10(10, field->heapId);
    field->dispControl = FieldDispControl_Create(field->heapId);
    if (GetZoneIsUnionRoom(field->zoneId) == TRUE || IsZone150Or151(field->zoneId) == TRUE) {
        GFL_OvlLoad(OVERLAY_ID(19));
    } else {
        GFL_OvlLoad(OVERLAY_ID(18));
    }
    GCTX_HIDChangeFPS(30);
    return 1;
}

void func_ov021_02189614(GameData *gameData, Field *field, u32 zoneId);
void func_ov010_0216447c(GameData *gameData, u16 zoneId);
PlaceName *func_ov021_021bc388(GameSystem *param_1, HeapID param_2, void *param_3);
u32 func_02013bbc(u16 zoneId);
FieldCamera *func_ov021_0218e018(u32 param_1, u32 param_2, G3DCamera *camera, VecFx32 *param_4, HeapID heap);
void func_ov021_021c6534(void *g3DObjSystem, void *luminanceTable);
void func_ov021_0218cee4(FieldG3DMapper *mapper, const FieldG3DMapperConfig *config, const u8 *lut);
BOOL func_02013c20(u16 zoneId);
void func_ov021_021be1dc(Field *field);
void *func_ov021_021c1af4(u32 count, void *particleSystem);
EncEff *func_ov021_021cf45c(HeapID heapId, Field *field);
DayCare *func_ov021_021c31d4(HeapID heapId, Field *field, DayCareSave *dayCare);
// The field effects every map loads, and how many
extern const u32 data_ov021_021d8f10[];
extern const u32 data_ov021_021d8f00;
// pokebw2's FieldRoutine_MapLoad: the field's systems, a step each frame
u32 func_ov021_021881d8(GameSystem *gsys, Field *field) {
    GameData *gameData = GSYS_GetGameData(gsys);
    BOOL loading = TRUE;

    switch (field->subroutinePhase) {
    case 0:
        func_ov010_0216447c(gameData, field->zoneId);
        field->msgBGSys = func_ov021_021901ec(field->heapId, field->g3dCamera);
        field->moneyWin = NULL;
        field->placeName = func_ov021_021bc388(gsys, 0x93, field->msgBGSys);
        field->skillMapEff = func_ov021_021c7f70(FieldStatus_GetFlashPerms(GameData_GetFieldStatus(field->gameData)),
                                                 field->heapId);
        field->cameraSystem = func_ov021_0218e018(func_02013bbc(field->zoneId), 0, field->g3dCamera, &field->playerPos,
                                                  field->heapId);
        field->sceneArea = FieldSceneArea_Create(field->heapId, field->cameraSystem, field);
        field->sceneAreaLoader = FieldSceneAreaLoader_Create(field->heapId);
        break;
    case 1: {
        FieldPropSystem *propSystem;
        void *luminanceTable;

        field->noGridMapper = FieldNoGridMapper_Create(field->heapId, field->cameraSystem, field->sceneArea,
                                                       field->sceneAreaLoader);
        field->palaceSys = FieldPalaceSys_Create(field->heapId, gsys, field, field->zoneId);
        propSystem = FieldG3DMapper_GetBMSystem(field->g3DMapper);
        SetupLoadZoneMapTypeData(field->zoneId, field->areaData, &field->mapperConfig,
                                 GetMapMatrixSystem(field->gameData));
        GSYS_GetGameCommSystem(field->gameSystem);
        luminanceTable = FieldPalaceSys_GetLuminanceTable(field->palaceSys);
        FieldPropSystem_LoadArea(propSystem, field->zoneId, field->areaData, luminanceTable);
        func_ov021_021c6534(field->g3DObjSystem, luminanceTable);
        func_ov021_02189614(gameData, field, field->zoneId);
        break;
    }
    case 2: {
        u32 railId;

        SetupZoneWarpArrivalGrid(&field->playerZoneState, field->zoneId, 0, 0, 0, field->playerPos.x, field->playerPos.y,
                                 field->playerPos.z);
        GSYS_GetGameCommSystem(field->gameSystem);
        func_ov021_0218cee4(field->g3DMapper, &field->mapperConfig, FieldPalaceSys_GetLuminanceTable(field->palaceSys));
        railId = GetRailIDForZone(field->zoneId);
        if (railId != 0xffffffff) {
            FieldNoGridMapper_LoadByHeader(field->noGridMapper, railId, field->heapId);
        }
        field->objectProjectionMatrixOffset = GetObjectProjectionMatrixOffset(field->zoneId);
        FieldCameraBoundary_ChangeZone(field, field->zoneId, field->heapId);
        break;
    }
    case 3:
        Field_InitActorSystem(field);
        field->fieldEffects = FieldEffects_Create(field, 0x18, field->heapId);
        FieldEffects_TCBManagerInit(field->fieldEffects, 0x5e);
        FieldEffects_SetLuminanceTable(field->fieldEffects, FieldPalaceSys_GetLuminanceTable(field->palaceSys));
        FieldEffects_Load(field->fieldEffects, data_ov021_021d8f10, data_ov021_021d8f00);
        if (GetZoneIsUnionRoom(field->zoneId) == TRUE || IsZone150Or151(field->zoneId) == TRUE) {
            field->unk98 = func_ov019_0217f8c0(field->heapId);
        }
        break;
    case 4: {
        PlayerState *playerState = GSYS_GetPlayerState(gsys);
        u16 dir = PlayerState_CalcDirection(playerState);
        VecFx32 *pos = &playerState->position;

        if (FieldPlayerState_GetExState(playerState) == 1 && !func_02013c20(field->zoneId)) {
            SetPlayerSpecialState(playerState, 0);
        }
        field->player = FieldPlayer_Create(playerState, field, pos, getTrainerGender(GetGameDataPlayerInfo(gameData)),
                                           field->heapId);
        field->playerPos = *pos;
        field->ctrlVTable->create(field, &field->playerPos, dir);
        FieldG3DMapper_SetPlayerPos(field->g3DMapper, &field->playerPos);
        func_ov021_02189758(&field->gimmickWork);
        field->expObjSystem = func_ov021_021be2a8(0x3c, 0x23, field->heapId);
        Field_LoadEdgeColorTable(field->areaData, Field_GetPlayerStateZoneID(field));
        field->fog = func_ov021_021a0104(field->heapId);
        field->fogCtrl = func_ov021_021a02c4(field->heapId);
        func_ov021_021a0338(field->fogCtrl, func_02013eec(field->zoneId), GetZoneStaticLightDataIndex(field->zoneId),
                            TRUE);
        {
            u32 lightsId = func_020140dc(field->areaData);
            int daySeconds = RTC_ConvertDaySecondsCached();

            field->lightSystem = func_ov021_021a03d0(lightsId, daySeconds, GameSystem_GetSeason(gsys), field->fog,
                                                     field->g3dLights, field->heapId);
        }
        field->weatherSystem =
            func_ov021_021a19e8(field->cameraSystem, field->lightSystem, field->fog, field->fogCtrl,
                                GameData_GetFieldSoundSystem(field->gameData), GameSystem_GetSeason(gsys), 0x92);
        func_ov021_021a1b74(field->weatherSystem, GetNowWeather(field->gameData), 0x92);
        func_ov021_021be1dc(field);
        break;
    }
    case 5:
        field->subscreen = func_ov021_021a0cf4(field->heapId, field, func_020129ac(gameData));
        break;
    case 6:
        field->encountSystem = EncSys_Create(field);
        func_02026388(GSYS_GetGameCommSystem(gsys), field);
        field->asyncProcManager = FieldAsyncProcManager_Create(field, field->heapId, 0x20);
        field->particleSystem = func_ov021_021c1a08(0x50);
        field->g3dCi = func_ov021_021c1af4(0x50, field->particleSystem);
        field->encEff = func_ov021_021cf45c(field->heapId, field);
        field->dayCare = func_ov021_021c31d4(field->heapId, field,
                                             getDaycareBlockAddress(GameData_GetSaveControl(field->gameData)));
        func_ov021_02189684(field, field->zoneId);
        loading = FALSE;
        break;
    }
    field->subroutinePhase++;
    field->renderMode = 0;
    if (!loading) {
        return 1;
    }
    return 0;
}

void func_ov021_0218ca78(FieldG3DMapper *mapper);
BOOL func_ov021_02189b04(u16 zoneId);
void func_020129dc(GameData *gameData, u8 enabled);
// fieldmap_event.c's event providers: the Union Room's, and those of the maps without a grid, of the hybrid maps and
// of the grid maps
GameEvent *func_ov021_0218a0e4(GameSystem *gsys, Field *field);
GameEvent *FieldEventProvider_NoGrid(GameSystem *gsys, Field *field);
void func_ov021_0218a4bc(GameSystem *param_1, Field *param_2);
GameEvent *FieldEventProvider_Grid(GameSystem *gsys, Field *field);
// pokebw2's FieldRoutine_MapSpawnFinish
u32 func_ov021_021885e8(GameSystem *gsys, Field *field) {
    switch (field->subroutinePhase) {
    case 0:
        func_ov021_0218ca78(field->g3DMapper);
        if (field->actorSystem != NULL) {
            do {
                FldActSys_Update(field->actorSystem);
                FldActSys_FinishAsyncMatLoad(field->actorSystem);
            } while (FldActSys_IsAsyncLoadPending(field->actorSystem) == TRUE);
        }
        while (func_ov021_021a1b50(field->weatherSystem)) {
            func_ov021_021a1af4(field->weatherSystem, 0x92);
        }
        FieldG3D_Update(field);
        FieldG3D_RenderPhase1(field);
        FieldG3D_RenderPhase2(field);
        func_0204a600();
        if (field->msgBGSys != NULL) {
            func_ov021_021903e0(field->msgBGSys);
        }
        FieldEffects_Update(field->fieldEffects);
        FieldAsyncProcManager_Update(field->asyncProcManager);
        field->subroutinePhase++;
    case 1:
        if (FieldStatus_CheckContinueFlag(GameData_GetFieldStatus(field->gameData)) == TRUE
            && func_ov021_02189b04(field->zoneId) == TRUE && func_02005c64()) {
            return 0;
        }
        if (FieldStatus_GetNewLoadFlag(GameData_GetFieldStatus(field->gameData))) {
            FieldScript_CallOnZoneReload(field->gameSystem, field->heapId);
            FieldStatus_SetNewLoadFlag(GameData_GetFieldStatus(field->gameData), FALSE);
        } else {
            FieldScript_CallOnZoneNewLoad(field->gameSystem, field->heapId);
        }
        FieldStatus_SetContinueFlag(GameData_GetFieldStatus(field->gameData), FALSE);
        func_ov021_0218ca78(field->g3DMapper);
        if (GetZoneIsUnionRoom(field->zoneId) == TRUE || IsZone150Or151(field->zoneId) == TRUE) {
            GSYS_SetEventProvider(gsys, func_ov021_0218a0e4, field);
        } else if (Field_GetControllerTypeID(field) == 1) {
            GSYS_SetEventProvider(gsys, FieldEventProvider_NoGrid, field);
        } else if (Field_GetControllerTypeID(field) == 2) {
            GSYS_SetEventProvider(gsys, func_ov021_0218a4bc, field);
        } else {
            GSYS_SetEventProvider(gsys, FieldEventProvider_Grid, field);
        }
        field->routineState = 1;
        func_020129dc(GSYS_GetGameData(gsys), TRUE);
        break;
    }
    return 1;
}

void func_ov021_021888c8(Field *field);
void func_ov021_021bc484(PlaceName *placeName);
void func_ov021_021c7fac(s32 *param_1);
void func_ov021_021a0d68(FieldSubscreen *subscreen);
void func_ov021_021be258(Field *field);
void func_ov021_021c6d90(FieldTaskManager *manager);
void func_ov021_021c8660(PleasureBoat *boat);
// pokebw2's FieldRoutine_Run
u32 func_ov021_02188758(GameSystem *gsys, Field *field) {
    if (GSYS_GetNowEvent(field->gameSystem) == NULL) {
        FieldStatus_SetBusyFlag(GameData_GetFieldStatus(field->gameData), FALSE);
    }
    if (GameData_CheckEventsPaused(field->gameData)) {
        return 0;
    }
    if (field->effectRunningFlag) {
        func_ov021_021888c8(field);
        func_0204a600();
        return 0;
    }
    func_02035648();
    Field_CheckDoRealTimeLoad(field);
    if (!GSYS_GetEventRunningFlag(gsys)) {
        field->ctrlVTable->update(field, &field->playerPos);
        func_ov010_02168eec(field->gameData);
    }
    if (field->placeName != NULL) {
        func_ov021_021bc484(field->placeName);
    }
    func_ov021_021c7fac(field->skillMapEff);
    FieldPlayer_SyncState(field->player);
    if (GetZoneIsUnionRoom(field->zoneId) == TRUE || IsZone150Or151(field->zoneId) == TRUE) {
        func_ov012_021737a4(GSYS_GetGameCommSystem(gsys), field);
    } else {
        func_ov018_0217ae20(field, field->gameSystem, field->player);
    }
    func_ov021_021a0d68(field->subscreen);
    if (field->actorSystem != NULL) {
        MI_SetMainMemoryPriority(MI_PROCESSOR_ARM9);
        FldActSys_Update(field->actorSystem);
        MI_SetMainMemoryPriority(MI_PROCESSOR_ARM7);
    }
    func_ov021_021be258(field);
    FieldEffects_Update(field->fieldEffects);
    if (field->unk98 != NULL) {
        func_ov019_0217f914(field->unk98);
    }
    FieldAsyncProcManager_Update(field->asyncProcManager);
    if (field->msgBGSys != NULL) {
        func_ov021_021903e0(field->msgBGSys);
    }
    GFL_TCBMgrUpdate(field->tcbManager);
    func_ov021_021c6d90(field->taskManager);
    if (field->playerPosPtr != NULL) {
        field->playerPos = *field->playerPosPtr;
    }
    FieldG3DMapper_SetPlayerPos(field->g3DMapper, &field->playerPos);
    FieldG3D_Update(field);
    func_ov021_021c8660(*func_02012ed0(field->gameData));
    func_0204a600();
    func_ov021_021888c8(field);
    return 0;
}





void func_ov021_021888c8(Field *field) {
    GFL_G3DSysResetGeometryCounter();
    FieldCamera_CalcTransform(field->cameraSystem, GCTX_HIDGetHeldKeys());
    field->actorYOffset = func_ov021_02189a80(field);
    MI_SetMainMemoryPriority(MI_PROCESSOR_ARM9);
    FieldG3D_RenderPhase1(field);
    MI_SetMainMemoryPriority(MI_PROCESSOR_ARM7);
}

void func_020129f0(GameData *gameData);
void func_ov021_0218cc34(FieldG3DMapper *mapper);
void func_ov021_021a0e58(FieldSubscreen *subscreen);
void func_ov021_021bc4f0(PlaceName *placeName);
u32 func_ov021_02188910(GameSystem *gsys, Field *field) {
    if (GameData_CheckEventsPaused(field->gameData)) {
        func_020129f0(GSYS_GetGameData(gsys));
        return 0;
    }
    MI_SetMainMemoryPriority(MI_PROCESSOR_ARM9);
    func_ov021_0218cc34(field->g3DMapper);
    MI_SetMainMemoryPriority(MI_PROCESSOR_ARM7);
    func_ov021_021a0e58(field->subscreen);
    if (field->msgBGSys != NULL) {
        func_ov021_021903e0(field->msgBGSys);
    }
    if (field->placeName != NULL) {
        func_ov021_021bc4f0(field->placeName);
    }
    MI_SetMainMemoryPriority(MI_PROCESSOR_ARM9);
    FieldG3D_RenderPhase2(field);
    MI_SetMainMemoryPriority(MI_PROCESSOR_ARM7);
    func_020129f0(GSYS_GetGameData(gsys));
    return 0;
}

void func_ov021_021cf480(EncEff *encEff);
void func_ov021_021c1c04(void *ci);
void func_ov021_021bc424(PlaceName *placeName);
void func_ov021_021be218(Field *field);
void func_ov021_021be2e0(FieldExpObjSystem *expObjSystem);
u8 func_ov021_021a0d44(FieldSubscreen *subscreen);
void func_ov021_021a0448(void *lightSystem);
void func_ov021_021a0138(FieldFog *fog);
void func_ov021_0218e0e4(FieldCamera *camera);
void func_ov021_0218d05c(FieldG3DMapper *mapper);
void func_ov021_021c31fc(DayCare *dayCare);
void func_ov021_021c7f9c(u32 *param_1);
// pokebw2's FieldRoutine_MapUnload
u32 func_ov021_02188990(GameSystem *gsys, Field *field) {
    GameData *gameData;

    func_ov021_021cf480(field->encEff);
    func_ov021_021c1c04(field->g3dCi);
    func_ov021_021c1ad0(field->particleSystem);
    GSYS_SetEventProvider(gsys, NULL, NULL);
    FieldPlayer_SyncState(field->player);
    FieldAsyncProcManager_Free(field->asyncProcManager);
    func_020263b0(GSYS_GetGameCommSystem(gsys), field);
    if (field->placeName != NULL) {
        func_ov021_021bc424(field->placeName);
    }
    func_ov021_021be218(field);
    func_ov021_021be2e0(field->expObjSystem);
    EncSys_Free(field->encountSystem);
    gameData = GSYS_GetGameData(gsys);
    func_020129b8(gameData, func_ov021_021a0d44(field->subscreen));
    FieldEffects_Free(field->fieldEffects);
    if (field->unk98 != NULL) {
        func_ov019_0217f8ec(field->unk98);
        field->unk98 = NULL;
    }
    func_ov021_021a1aac(field->weatherSystem);
    field->weatherSystem = NULL;
    func_ov021_021a0448(field->lightSystem);
    func_ov021_021a02f4(field->fogCtrl);
    func_ov021_021a0138(field->fog);
    FieldPalaceSys_Free(field->palaceSys);
    FieldNoGridMapper_Free(field->noGridMapper);
    FreeFieldSceneAreaLoader(field->sceneAreaLoader);
    FreeFieldSceneArea(field->sceneArea);
    func_ov021_0218e0e4(field->cameraSystem);
    field->ctrlVTable->free(field);
    Field_ResetController(field);
    Field_SuspendActorSystem(field);
    FieldPlayer_Free(field->player);
    func_ov021_0218d05c(field->g3DMapper);
    if (field->moneyWin != NULL) {
        func_ov021_02190728(field->moneyWin);
    }
    if (field->msgBGSys != NULL) {
        func_ov021_021902bc(field->msgBGSys);
    }
    func_020129dc(GSYS_GetGameData(gsys), FALSE);
    GCTX_HIDChangeFPS(60);
    func_ov021_021c31fc(field->dayCare);
    func_ov021_021c7f9c(field->skillMapEff);
    return 1;
}


void func_ov021_021c6d5c(FieldTaskManager *manager);
void func_ov021_0218ca4c(FieldG3DMapper *mapper);
// pokebw2's FieldRoutine_Close
u32 func_ov021_02188ad4(GameSystem *gsys, Field *field) {
    func_02030da4(field->tcbManager);
    GFL_HeapFree(field->tcbManagerHeap);
    FieldDispControl_Free(field->dispControl);
    field->dispControl = NULL;
    func_ov021_021c6d5c(field->taskManager);
    GFL_TCBRemove(field->asyncActorMatLoadTCB);
    func_ov021_0218ca4c(field->g3DMapper);
    FieldG3D_Free(field);
    func_0204a5ac();
    BmpWin_FreeAllocator();
    Field_FreeGraphicsSystems(field);
    if (GetZoneIsUnionRoom(field->zoneId) == TRUE || IsZone150Or151(field->zoneId) == TRUE) {
        GFL_OvlUnload(OVERLAY_ID(19));
    } else {
        GFL_OvlUnload(OVERLAY_ID(18));
    }
    return 2;
}




BOOL Field_CallRoutines(GameSystem *gsys, Field *field) {
    u32 result = 0;
    FieldRoutine routine;

    field->framesSinceStart++;
    routine = data_ov021_021d36b8[field->routineID][field->routineAlternator];
    if (routine != NULL) {
        result = routine(gsys, field);
    }
    switch (result) {
    case 0:
        field->routineAlternator = field->routineAlternator == 0 ? 1 : 0;
        break;
    case 1:
        field->routineID++;
        field->subroutinePhase = 0;
        field->routineAlternator = 0;
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}

void Field_RequestClose(Field *field) {
    field->routineState = 2;
    field->routineID = 4;
}

BOOL Field_CheckMapLoadFinished(Field *field) {
    return field->routineState == 1;
}

BOOL Field_ToggleCycling(Field *field) {
    BOOL changed = FALSE;
    u32 exState = FieldPlayer_GetExState(field->player);

    if (exState == 1) {
        FieldPlayer_SetSpecialSeq(field->player, 1);
        changed = TRUE;
    } else if (exState == 0) {
        changed = TRUE;
        GFL_SndSEPlay(SEQ_SE_BICYCLE);
        FieldPlayer_SetSpecialSeq(field->player, 2);
        RecordAddOne(GameData_GetRecords(field->gameData), 3);
    }

    if (changed == TRUE) {
        func_ov021_021a2f54(field->player);
    }
    return changed;
}


void *Field_GetMsgBGSys(Field *field) {
    return field->msgBGSys;
}

FieldCamera *Field_GetCameraSystem(Field *field) {
    return field->cameraSystem;
}

NoGridMapper *Field_GetNoGridMapper(Field *field) {
    return field->noGridMapper;
}

void *func_ov021_02188c60(Field *field) {
    return field->lightSystem;
}

FieldFog *Field_GetFog(Field *field) {
    return field->fog;
}

void *func_ov021_02188c68(Field *field) {
    return field->fogCtrl;
}

void *Field_GetWeatherSystem(Field *field) {
    return field->weatherSystem;
}

u32 Field_GetWeatherForZone(Field *field, u16 zoneId) {
    return GetWeatherAll(field->gameSystem, zoneId);
}

MMSys *Field_GetActorSystem(Field *field) {
    return field->actorSystem;
}

GameSystem *Field_GetGameSystem(Field *field) {
    return field->gameSystem;
}

u16 Field_GetHeapID(Field *field) {
    return field->heapId;
}

void *Field_GetEffectBlAct(Field *field) {
    return field->effectBlAct;
}

void *Field_GetWildEffectBlAct(Field *field) {
    return field->wildEffectBlAct;
}

FieldG3DMapper *Field_GetG3DMapper(Field *field) {
    return field->g3DMapper;
}

u16 Field_GetPlayerStateZoneID(Field *field) {
    return field->playerZoneState.zoneId;
}

void *Field_GetController(Field *field) {
    return field->controller;
}

void Field_SetController(Field *field, void *controller) {
    field->controller = controller;
}

FieldPlayer *Field_GetPlayer(Field *field) {
    return field->player;
}

BOOL func_ov021_02188cbc(Field *field) {
    if (field->player != NULL) {
        return TRUE;
    }
    return FALSE;
}

FieldSubscreen *Field_GetSubscreen(Field *field) {
    return field->subscreen;
}

void *Field_GetFieldEffects(Field *field) {
    return field->fieldEffects;
}

void *func_ov021_02188cd8(Field *field) {
    return field->g3DObjSystem;
}

void *func_ov021_02188ce0(Field *field) {
    return field->unk98;
}

EncountSystem *Field_GetEncountSystem(Field *field)
{
    return field->encountSystem;
}


u32 Field_GetControllerTypeID(Field *field) {
    return field->ctrlVTable->typeId;
}

static const u8 data_ov021_021d35a8[4] = { 0, 1, 2, 0 };
extern u32 func_ov021_021a6dc0(u32 *param_1);
u32 Field_GetResolvedControllerTypeID(Field *field) {
    u32 type = data_ov021_021d35a8[field->ctrlVTable->typeId];
    if (type == 2) {
        type = func_ov021_021a6dc0(field->controller);
    }
    return type;
}


PlaceName *Field_GetPlaceName(Field *field) {
    return field->placeName;
}

FieldAsyncProcManager *Field_GetAsyncProcMgr(Field *field) {
    return field->asyncProcManager;
}

FieldExpObjSystem *Field_GetExpObjSystem(Field *field) {
    return field->expObjSystem;
}

TCBManager *Field_GetTCBMgr(Field *field) {
    return field->tcbManager;
}

FieldTaskManager *Field_GetTaskManager(Field *field) {
    return field->taskManager;
}

void Field_SetPlayerPosPtr(Field *field, VecFx32 *position) {
    field->playerPosPtr = position;
}

void *Field_GetMoneyWin(Field *field) {
    return field->moneyWin;
}

void Field_SetMoneyWin(Field *field, void *moneyWin) {
    field->moneyWin = moneyWin;
}

DayCare *Field_GetDayCare(Field *field) {
    return field->dayCare;
}

AreaData *func_ov021_02188d58(Field *field) {
    return field->areaData;
}

void FieldG2D_Init(Field *field) {
    GFL_BGSysCreate(field->heapId);
    GFL_BGSysUploadStdPalette(0, FIELD_STD_PALETTE, 0x20, 0);
    GFL_BGSysUploadStdPalette(4, FIELD_STD_PALETTE, 0x20, 0);
    GFL_BGSysSetDisplayLayout(1);
    GFL_BGSysEnableEngines();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
}

void FieldG2D_SetLCDConfig(void) {
    GFL_BGSysSetLCDConfig(&FIELD_LCD_CONFIG);
}

void FieldG2D_Prepare3DSurface(Field *field) {
    MtxFx22 mtx;

    MAT2_SetScaleRot(&mtx, 0, FX32_ONE, FX32_ONE, 0);
    G2_SetBG2Affine(&mtx, 0, 0, 0, 0);
    GFL_BGSysSet3DBGPriority(3);
    func_ov021_02190270(field->msgBGSys);
}

void func_ov021_02188df0(Field *field) {
    MtxFx22 mtx;

    MAT2_SetScaleRot(&mtx, 0, FX32_ONE, FX32_ONE, 0);
    G2_SetBG2Affine(&mtx, 0, 0, 0, 0);
    GFL_BGSysSet3DBGPriority(3);
    func_ov021_0219027c(field->msgBGSys);
}

void *FieldG2D_GetDispControl(Field *field) {
    return field->dispControl;
}

void Field_FreeGraphicsSystems(Field *field) {
    GFL_BGSysSetDisplayLayout(1);
    NNS_G3dGeFlushBuffer();
    GFL_G3DSysFree();
    GFL_BGSysFree();
}

void FieldG3D_InitCallback(void) {
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_SetFog(FALSE, 0, 5, 0xe00);
    G3X_SetFogColor(GX_RGB(31, 31, 31), 0);
    G3X_SetFogTable(FIELD_DEFAULT_FOG_TABLE);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 31, 0x7fff, 0, FALSE);
    G3X_SetEdgeColorTable(FIELD_DEFAULT_EDGE_COLOR_TABLE);
    G3X_EdgeMarking(FALSE);
    G3_ViewPort(0, 0, 255, 191);
    GFL_G3DSysSetSwapBufferParams(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}

// fldmmdl_g3dobj.c's: the 3D object system of the field
// The field's 3D camera: a 40 degree field of view, looking from the default place
static inline G3DCamera *FieldG3D_CreateCamera(HeapID heapId) {
    VecFx32 up = { 0, 0x1000, 0 };

    return GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)), FX_CosIdx(DEG_TO_IDX(20)),
                               FX32_CONST(4.0 / 3.0), 0, FX32_ONE, FX32_CONST(1024), 0, &DEFAULT_FIELDCAMERA_POS, &up,
                               &DEFAULT_FIELDCAMERA_TGT, heapId);
}

void FieldG3D_Init(Field *field) {
    BlActScene *scene;
    u8 polyId;
    VecFx32 scale;

    field->actorBlAct = BlActSys_Create(0x40, 0x80, FldActSys_VRAMUploadFunc, field->heapId);
    scene = BlActSys_GetScene(field->actorBlAct);
    BlActScene_SetGeomOrigin(scene, 2);
    BlActScene_SetTexcoordOffset(scene, 0, -0xa00);
    field->effectBlAct = BlActSys_Create(0x3a, 0x50, FldActSys_VRAMUploadFunc, field->heapId);
    field->wildEffectBlAct = BlActSys_Create(0x1e, 0x28, FldActSys_VRAMUploadFunc, field->heapId);
    scene = BlActSys_GetScene(field->wildEffectBlAct);
    {
        VecFx32 scale2 = { 0x7000, 0x7000, 0x7000 };
        scale = scale2;
    }
    BlActScene_SetScale(scene, &scale);
    polyId = 0;
    BlActScene_SetBasePolyID(scene, &polyId);
    Field_LoadActorMatColorPreset(field);
    field->g3DObjSystem = func_ov021_021c63ac(field->heapId, 0x40, 0x40);
    field->g3dCamera = FieldG3D_CreateCamera(field->heapId);
    field->g3dLights = GFL_G3DLightCreate(&FIELD_LIGHT_SETUP, field->heapId);
    GFL_G3DCameraFlush(field->g3dCamera);
    GFL_G3DLightFlush(field->g3dLights);
}


extern void func_ov021_021a0140(FieldFog *fog);
extern void func_ov021_021a0458(void *lightSystem, u32 rtcSeconds);
extern BOOL func_ov021_0218cb34(FieldG3DMapper *mapper);
void FieldG3D_Update(Field *field) {
    BOOL loading;

    BlActSys_UpdateActors(field->actorBlAct);
    BlActSys_UpdateActors(field->effectBlAct);
    BlActSys_UpdateActors(field->wildEffectBlAct);
    FieldNoGridMapper_UpdateCamera(field->noGridMapper);
    FieldCameraArea_Update(field->sceneArea, &field->playerPos);
    func_ov021_021a030c(field->fogCtrl, field->heapId);
    func_ov021_021a1af4(field->weatherSystem, 0x92);
    func_ov021_021a0140(field->fog);
    func_ov021_021a0458(field->lightSystem, RTC_ConvertDaySecondsCached());
    MI_SetMainMemoryPriority(MI_PROCESSOR_ARM9);
    loading = func_ov021_0218cb34(field->g3DMapper);
    MI_SetMainMemoryPriority(MI_PROCESSOR_ARM7);
    if (loading) {
        func_0203d12c(2);
    }
}


void FieldG3D_RenderPhase1(Field *field) {
    FieldRenderFunc render;

    GFL_G3DSysReset();
    GFL_G3DSysMtxViewFlush();
    render = FIELD_RENDER_FUNCS_PHASE1[field->renderMode];
    if (render != NULL) {
        render(field);
    }
}

void FieldG3D_RenderPhase2(Field *field) {
    FIELD_RENDER_FUNCS_PHASE2[field->renderMode](field);
    GFL_G3DSysReqSwapBuffers();
}

void FieldG3D_Free(Field *field) {
    GFL_G3DLightFree(field->g3dLights);
    GFL_G3DCameraFree(field->g3dCamera);
    BlActSys_Free(field->actorBlAct);
    BlActSys_Free(field->effectBlAct);
    BlActSys_Free(field->wildEffectBlAct);
    FieldG3DObjSystem_Free(field->g3DObjSystem);
}

void FldActSys_AsyncMatLoadTCBFunc(TCB *tcb, void *data) {
    Field *field = data;

    if (!GFL_IRQCartDataTransferIsEnabled()) {
        if (field->actorSystem != NULL) {
            FldActSys_FinishAsyncMatLoadSafe(field->actorSystem);
        }
        if (field->g3DObjSystem != NULL) {
            FieldG3DObjSystem_SetupResources(field->g3DObjSystem);
        }
    }
    func_0204a648();
    if (field->dispControl != NULL) {
        FieldDispControl_Update(field->dispControl);
    }
}

void FldActSys_VRAMUploadFunc(BOOL type, u32 dest, void *src, u32 size) {
    NNS_GfdRegisterNewVramTransferTask(!type ? NNS_GFD_DST_3D_TEX_VRAM : NNS_GFD_DST_3D_TEX_PLTT, dest, src, size);
}

void Field_LoadEdgeColorTable(AreaData *area, u16 zoneId) {
    u32 tableId = AreaData_GetEdgeColorTableID(area);

    if (tableId == 0xff) {
        G3X_EdgeMarking(FALSE);
        return;
    }
    if (IsZoneEntralinkEdgeColorTable(zoneId)) {
        G3X_EdgeMarking(FALSE);
    } else {
        G3X_EdgeMarking(TRUE);
    }
    GFL_ArcSysRead(g_FieldEdgeColorTable, 0xf, tableId);
    g_FieldEdgeColorTable[7] = 0x4210;
    G3X_SetEdgeColorTable(g_FieldEdgeColorTable);
}

void Field_LoadActorMatColorPreset(Field *field) {
    HeapID heapId = HEAPID_TAIL(field->heapId);
    u32 presetId = AreaData_GetActorMatColorID(field->areaData);
    void *preset = FldActMatColorPreset_Create(heapId);

    FldActMatColorPreset_Load(preset, presetId);
    FldActMatColorPreset_Apply(preset, BlActSys_GetScene(field->actorBlAct));
    FldActMatColorPreset_Free(preset);
}

void func_ov021_0219c554(MMSys *param_1, BlActSys *param_2, u16 param_3);
void func_ov021_0219c680(MMSys *system);
void func_ov021_0219c658(MMSys *system, u16 objCode);
void func_ov021_0219d448(MMSys *system, void *g3dObjSystem);
u16 *func_ov021_0218ed50(FieldCamera *camera);
void Field_InitActorSystem(Field *field) {
    GameData *gameData = GSYS_GetGameData(field->gameSystem);
    MMSys *actorSystem = GameData_GetMMSys(gameData);
    u16 zoneId;
    u32 sex;

    field->actorSystem = actorSystem;
    FldActSys_AttachField(actorSystem, field->heapId, gameData, field, field->g3DMapper, field->noGridMapper);
    func_ov021_0219c554(field->actorSystem, field->actorBlAct, 0x20);
    func_ov021_0219c680(field->actorSystem);
    zoneId = field->playerZoneState.zoneId;
    if (!GetZoneIsUnionRoom(zoneId) && !IsZone150Or151(zoneId)) {
        sex = func_ov018_0217c0a8(GSYS_GetGameCommSystem(field->gameSystem));
    } else {
        sex = getTrainerGender(GetGameDataPlayerInfo(gameData));
    }
    func_ov021_0219c658(actorSystem, FieldPlayer_GetObjCodeByExState(sex, 0));
    func_ov021_0219d448(field->actorSystem, field->g3DObjSystem);
    func_ov010_0216cdb8(field->actorSystem, func_ov021_0218ed50(field->cameraSystem));
    func_ov010_0216d2f8(field->actorSystem);
    func_ov010_0216d974(actorSystem, func_ov021_02189ae8(field->zoneId));
}


void Field_SuspendActorSystem(Field *field) {
    FldActSys_SuspendAllActors(field->actorSystem);
    func_ov010_0216cd50(field->actorSystem);
    field->actorSystem = NULL;
}

BOOL Field_CheckDoRealTimeLoad(Field *field) {
    EventData *eventData;

    if (Field_UpdateZoneStatePos(field) == TRUE && Field_ShouldSwapZone(field) == TRUE) {
        Field_SwapZoneByMatrix(field);
        return TRUE;
    }
    eventData = GameData_GetEventData(field->gameData);
    if (!IsEncountDataLoaded(eventData)) {
        MI_SetMainMemoryPriority(MI_PROCESSOR_ARM9);
        EventData_LoadEncData(eventData, field->playerZoneState.zoneId, GameData_GetSeason(field->gameData));
        MI_SetMainMemoryPriority(MI_PROCESSOR_ARM7);
    }
    return FALSE;
}

BOOL Field_UpdateZoneStatePos(Field *field) {
    ZoneSpawnInfo *zoneState = &field->playerZoneState;
    VecFx32 *playerPos = PlayerState_GetWPos(GameData_GetPlayerState(GSYS_GetGameData(field->gameSystem)));
    VecFx32 statePos;

    GetPlayerZoneStateWPos(zoneState, &statePos);
    if (playerPos->x != statePos.x || playerPos->z != statePos.z) {
        SetPlayerZoneStateWPos(zoneState, playerPos);
        return TRUE;
    }
    return FALSE;
}

BOOL Field_ShouldSwapZone(Field *field) {
    ZoneSpawnInfo *zoneState = &field->playerZoneState;
    MapMatrix *matrix = GetMapMatrixSystem(field->gameData);
    VecFx32 pos;

    GetPlayerZoneStateWPos(zoneState, &pos);
    if (RangeCheckChunkCoordinateWorld(matrix, pos.x, pos.z) == TRUE) {
        int zoneId = GetZoneIDAtMatrixXZWorld(matrix, pos.x, pos.z);

        if (zoneId != 0xffff && zoneId != zoneState->zoneId) {
            return TRUE;
        }
    }
    return FALSE;
}

void func_ov010_021643ac(GameData *gameData, Field *field);
void Field_SwapZoneByMatrix(Field *field) {
    ZoneSpawnInfo *zoneState = &field->playerZoneState;
    GameData *gameData = GSYS_GetGameData(field->gameSystem);
    EventData *eventData = GameData_GetEventData(gameData);
    MMSys *actorSystem = field->actorSystem;
    MapMatrix *matrix = GetMapMatrixSystem(field->gameData);
    VecFx32 pos;
    u32 zoneId;

    func_0203d12c(2);
    EncountSystem_CancelPhenomenon(field->encountSystem);
    GetPlayerZoneStateWPos(zoneState, &pos);
    zoneId = GetZoneIDAtMatrixXZWorld(matrix, pos.x, pos.z);
    MI_SetMainMemoryPriority(MI_PROCESSOR_ARM9);
    DeleteAllActors(actorSystem);
    EventData_Reset(eventData);
    EventData_LoadEntities(eventData, zoneId, GameData_GetSeason(gameData));
    MI_SetMainMemoryPriority(MI_PROCESSOR_ARM7);
    func_ov010_021643ac(gameData, field);
    Field_SwapZoneBGM(field, zoneId);
    func_ov021_0218958c(field, zoneId);
    Field_SwapWeather(field, zoneId);
    Field_SwapCameraBoundaries(field, zoneId);
    Field_UpdatePlayerStateZoneID(gameData, zoneId);
    func_ov021_02189684(field, zoneId);
    if (field->placeName != NULL && !GSYS_GetEventRunningFlag(field->gameSystem)) {
        func_ov021_021bc550(field->placeName, zoneId);
    }
    zoneState->zoneId = zoneId;
    func_0202735c(zoneId, gameData);
    SetTeleportZoneDiscover(gameData, zoneId);
    FieldScript_CallOnZoneInit(field->gameSystem, 4);
    MI_SetMainMemoryPriority(MI_PROCESSOR_ARM9);
    Field_HotswapSpawnActors(gameData, actorSystem, eventData, zoneId);
    MI_SetMainMemoryPriority(MI_PROCESSOR_ARM7);
    func_ov010_0215874c(GSYS_GetGameCommSystem(field->gameSystem));
}


void Field_HotswapSpawnActors(GameData *gameData, MMSys *actorSystem, EventData *eventData, u32 zoneId) {
    u32 count = GetZoneNPCsCount(eventData);

    if (count != 0) {
        EventWork *eventWork = GameData_GetEventWork(gameData);

        SpawnAllZoneNPCs(actorSystem, GetZoneNPCs(eventData), zoneId, count, eventWork);
    }
}

void Field_SwapZoneBGM(Field *field, u32 zoneId) {
    GameData *gameData = GSYS_GetGameData(field->gameSystem);
    ISS *iss = GameSystem_GetISS(field->gameSystem);
    FieldSound *fieldSound = GameData_GetFieldSoundSystem(gameData);
    u32 exState = FieldPlayerState_GetExState(GameData_GetPlayerState(gameData));

    ISS_ChangeZone(iss, zoneId);
    if (exState != 2) {
        FieldSnd_ChangeZoneBGM(fieldSound, gameData, zoneId);
    }
}

void Field_SwapWeather(Field *field, u32 zoneId) {
    void *weather = Field_GetWeatherSystem(field);
    u16 weatherId;

    ResetWeather(field->gameSystem, zoneId);
    weatherId = GetNowWeather(field->gameData);
    if (weatherId != 15) {
        func_ov021_021a1bdc(weather, weatherId);
    }
}

// The fog of a zone the player walks into
void func_ov021_0218958c(Field *field, u32 zoneId) {
    u32 fogIndex;

    FieldFogCtrl_Reset(func_ov021_02188c68(field));
    fogIndex = func_02013eec(zoneId);
    func_ov021_021a0338(field->fogCtrl, fogIndex, GetZoneStaticLightDataIndex(zoneId), TRUE);
}


void Field_UpdatePlayerStateZoneID(GameData *gameData, u32 zoneId) {
    PlayerState_SetZoneID(GameData_GetPlayerState(gameData), zoneId);
}

void Field_SwapCameraBoundaries(Field *field, u32 zoneId) {
    FieldCameraBoundary_ChangeZone(field, zoneId, field->heapId);
}

void Field_ResetController(Field *field) {
    field->controller = NULL;
}

void FieldCameraBoundary_ChangeZone(Field *field, u32 zoneId, HeapID heapId) {
    u16 boundaryId = GetZoneMatrixCamBoundIdx(zoneId);

    if (boundaryId != 0xffff) {
        FieldCameraBoundary_ChangeID(field->cameraSystem, boundaryId, heapId);
    } else {
        FieldCameraBoundary_LoadDummy(field->cameraSystem);
    }
}

void func_ov021_02189614(GameData *gameData, Field *field, u32 zoneId) {
    FieldStatus *fieldStatus = GameData_GetFieldStatus(gameData);
    if (func_02013dd0(field->zoneId) != 0) {
        u32 value = func_0201444c(fieldStatus);
        CityState *city;
        if (value == 0) {
            city = func_020127c4(field->gameData);
        } else {
            city = func_020127d0(field->gameData, 0);
        }
        func_ov021_0218dc74(field->g3DMapper, city, value);
        FieldWFBC *block = func_ov021_0218dc70(field->g3DMapper);
        void *eventData = GameData_GetEventData(gameData);
        func_ov021_021939c0(block, eventData, value, field->heapId);
        func_ov021_02193abc(block, field->cameraSystem, field->heapId);
        MMSys *mmSys = GameData_GetMMSys(field->gameData);
        func_ov021_02193afc(block, mmSys);
    }
}


void func_ov021_02189684(Field *field, u32 zoneId) {
    u16 cameraId = func_02013fa8(zoneId);
    if (cameraId != 0xffff) {
        ResetSceneArea(field->sceneArea);
        ResetSceneAreaLoader(field->sceneAreaLoader);
        LoadCameraDataToSceneAreaLoader(field->sceneAreaLoader, 1, cameraId, 0x96);
        BuildSceneArea(field->sceneArea, GetFldSceneAreaLoaderCameraData(field->sceneAreaLoader),
                       GetFldSceneAreaLoaderCamCount(field->sceneAreaLoader),
                       GetFldSceneAreaLoaderCamFuncsStaticOffs(field->sceneAreaLoader));
    } else {
        if (func_02013c80(zoneId) == 0) {
            ResetSceneArea(field->sceneArea);
            ResetSceneAreaLoader(field->sceneAreaLoader);
        }
    }
}


u32 func_ov021_021896f4(GameCommSys *comm) {
    u32 state = GameCommSys_BootCheck(comm);

    switch (state) {
    case 0:
        if (func_0200fb24()) {
            state = func_0202647c(comm);
        }
        return state;
    case 1:
    case 2:
    case 5:
        return state;
    }
    return 2;
}

BOOL func_ov021_02189734(GameCommSys *comm) {
    if (GameCommSys_BootCheck(comm) == 0) {
        return TRUE;
    }
    if (!func_02026460(comm)) {
        func_02026400(comm);
    }
    return FALSE;
}

void func_ov021_02189758(FieldGimmickWorkBlock *block) {
    block->password = 0xffffffff;
    block->work = NULL;
}

void *Field_AllocGimmickWorkBlock(Field *field, u32 password, HeapID heapId, u32 size) {
    FieldGimmickWorkBlock *block = &field->gimmickWork;

    if (password == 0xffffffff) {
        return NULL;
    }
    if (block->password != 0xffffffff) {
        return NULL;
    }
    block->password = password;
    block->work = GFL_HeapAllocate(heapId, size, TRUE, "fieldmap.c", 0xd7c);
    return block->work;
}

void Field_DeleteGimmickWorkBlock(Field *field, u32 password) {
    FieldGimmickWorkBlock *block = &field->gimmickWork;

    if (password != 0xffffffff) {
        GFL_HeapFree(block->work);
        block->password = 0xffffffff;
    }
}

void *Field_GetGimmickWorkBlock(Field *field, u32 password) {
    FieldGimmickWorkBlock *block = &field->gimmickWork;

    if (password == 0xffffffff) {
        return NULL;
    }
    if (password != block->password) {
        return NULL;
    }
    return block->work;
}

void func_ov021_021a0170(FieldFog *fog);
void func_ov021_021a0568(void *arg, u32 flag);
void func_ov021_0218cc64(void *arg, G3DCamera *camera, u32 flag);
void func_ov021_021897dc(Field *field) {
    func_ov021_021a0170(field->fog);
    func_ov021_021a0568(field->lightSystem, 0);
    GFL_G3DCameraFlush(field->g3dCamera);
    GFL_G3DLightFlush(field->g3dLights);
    func_ov021_0218cc64(field->g3DMapper, field->g3dCamera, 0);
}

void func_ov021_021c7fb4(u32 *param_1);
void func_ov021_021c64e0(FieldG3DObjSystem *sys);
void func_ov021_021be4f8(FieldExpObjSystem *expObjSystem);
// pokebw2's FieldRenderPhase2_Fieldmap: the map, then the actors and effects with their depth offsets
void func_ov021_02189810(Field *field) {
    MtxFx44 saved;
    MtxFx44 mtx;

    if (field->msgBGSys != NULL) {
        func_ov021_0219047c(field->msgBGSys);
    }
    func_ov021_0218cc64(field->g3DMapper, field->g3dCamera, 1);
    func_ov021_021c7fb4(field->skillMapEff);
    saved = NNS_G3dGlb.projMtx;
    mtx = saved;
    mtx.m[3][2] += FX_Mul(mtx.m[2][2], field->objectProjectionMatrixOffset);
    NNS_G3dGlbSetProjectionMtx(&mtx);
    NNS_G3dGlbFlushP();
    NNS_G3dGeFlushBuffer();
    FieldEffects_Draw(field->fieldEffects);
    func_ov021_021c64e0(field->g3DObjSystem);
    BlActSys_Draw(field->actorBlAct, field->g3dCamera, field->g3dLights);
    BlActSys_Draw(field->wildEffectBlAct, field->g3dCamera, field->g3dLights);
    if (field->unk98 != NULL) {
        func_ov019_0217f928(field->unk98);
    }
    mtx = saved;
    mtx.m[3][2] += FX_Mul(mtx.m[2][2], field->objectProjectionMatrixOffset / 2);
    NNS_G3dGlbSetProjectionMtx(&mtx);
    NNS_G3dGlbFlushP();
    NNS_G3dGeFlushBuffer();
    BlActSys_Draw(field->effectBlAct, field->g3dCamera, field->g3dLights);
    NNS_G3dGlbSetProjectionMtx(&saved);
    NNS_G3dGlbFlushP();
    FieldAsyncProcManager_Draw(field->asyncProcManager);
    func_ov021_021a1b38(field->weatherSystem);
    func_ov021_021be4f8(field->expObjSystem);
    func_ov021_021c1aec();
    func_ov021_021c1c4c(field->g3dCi);
}


void func_ov021_02189998(Field *field) {
    G3X_SetClearColor(0, 0, 0x7fff, 0, 0);
    func_ov021_021c1aec();
    func_ov021_021c1c4c(field->g3dCi);
}

void func_ov021_021cf520(EncEff *encEff);
void func_ov021_021899c4(Field *field)
{
    G3X_SetClearColor(0x4210, 0x1f, 0x7fff, 0, 0);
    func_ov021_021cf520(field->encEff);
}

u32 Field_GetRenderMode(Field *field)
{
    return field->renderMode;
}





void Field_SetRenderMode(Field *field, u32 mode) {
    field->renderMode = mode;
}

void *Field_Get3DCi(Field *field) {
    return field->g3dCi;
}

EncEff *Field_GetEncEff(Field *field) {
    return field->encEff;
}


void *Field_GetSkillMapEff(Field *field) {
    return field->skillMapEff;
}

void *Field_GetSceneArea(Field *field) {
    return field->sceneArea;
}

void Field_SetFadeFlag(Field *field, BOOL flag) {
    field->fadeFlag = flag;
}

u32 Field_GetFadeFlag(Field *field) {
    return field->fadeFlag;
}

u32 Field_IsEventRunning(Field *field) {
    return GSYS_GetEventRunningFlag(field->gameSystem);
}


u32 Field_GetDayPeriod(Field *field) {
    return GetRealTimeDayPeriod(GameData_GetSeason(field->gameData));
}

BOOL func_ov021_02189a48(Field *field) {
    return field->seasonBannerOverdrawFlag;
}

void Field_SetSeasonBannerOverdrawFlag(Field *field, BOOL flag) {
    field->seasonBannerOverdrawFlag = flag;
}

void func_ov021_02189a58(Field *field, BOOL flag) {
    field->effectRunningFlag = flag;
}

void *Field_GetNDemoDataHandle(Field *field) {
    return &field->nDemoData;
}

void Field_SetCasteliaRush(Field *field, CasteliaRush *rush) {
    field->casteliaRush = rush;
}

CasteliaRush *Field_GetCasteliaRush(Field *field) {
    return field->casteliaRush;
}

fx32 func_ov021_02189a78(Field *field) {
    return field->actorYOffset;
}

// How far below the camera the actors are drawn, from the camera's pitch
fx32 func_ov021_02189a80(Field *field) {
    u16 *pitch = func_ov021_0218ed3c(field->cameraSystem);
    fx32 cos = FX_CosIdx(*pitch);
    fx32 offset;

    if (cos < 0) {
        cos = -cos;
    }
    offset = FX_Mul(cos, FX32_CONST(4)) - FX32_CONST(4);
    if (offset < FX32_CONST(-2)) {
        offset = FX32_CONST(-2);
    }
    return offset;
}

u32 GetObjectProjectionMatrixOffset(u16 zoneId) {
    if (ZoneData_GetObjectProjectionMatrixType(zoneId) == 1) {
        return 0x1ee;
    }
    return 0x136;
}

BOOL func_ov021_02189ae8(u16 arg)
{
    if (arg == 0x3F)
        return FALSE;
    if (IsZone150Or151(arg) == 1)
        return FALSE;
    return TRUE;
}

BOOL func_ov021_02189b04(u16 mapId)
{
    if (mapId == 0x3F)
        return TRUE;
    if (mapId == 0x8C)
        return TRUE;
    if (mapId == 0x8D)
        return TRUE;
    if (mapId == 0x8E)
        return TRUE;
    return FALSE;
}

u8 func_02013b50(u16 param_1);
u32 GetZoneMapType2(u16 zoneId) {
    return func_02013b50(zoneId);
}




void SetupLoadZoneMapTypeData(u16 zoneId, AreaData *area, FieldG3DMapperConfig *config, MapMatrix *matrix) {
    u32 type = GetZoneMapType2(zoneId);

    *config = MAP_CONFIGS[type].mapperConfig;
    if (MAP_CONFIGS[type].useMapMatrix) {
        config->matrixWidth = GetMapMatrixWidth(matrix);
        config->matrixHeight = GetMapMatrixHeight(matrix);
        config->chunkIdCount = GetMapMatrixChunkIDCount(matrix);
        config->chunkDatIds = GetMapMatrixChunkIDs(matrix);
    }
    config->isSharedTexResource = TRUE;
    config->textures.arcId = 14;
    config->textures.fileId = AreaData_GetTexSetID(area);
    config->terrainAnmInfo.srtAnimeId = AreaData_GetSRTAnmID(area);
    config->terrainAnmInfo.patAnimeId = AreaData_GetPatAnmID(area);
}

const FieldmapCtrlVTable *GetZoneFieldmapCtrlVTable(u16 zoneId) {
    return MAP_CONFIGS[GetZoneMapType2(zoneId)].ctrlVTable;
}

u32 GetFieldmapZoneHeapSize(u16 zoneId) {
    return MAP_CONFIGS[GetZoneMapType2(zoneId)].heapSize;
}
