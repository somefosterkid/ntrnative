// Black's code at ov021 0x02189bd0-0x0218b754. The name is pokebw2's file between fieldmap.c and field_buildmodel.c (55
// functions there, 53 here).

#include "types.h"
#include "field/c_gear.h"
#include "field/c_gear_sw.h"
#include "field/encounter.h"
#include "field/field.h"
#include "field/field_map.h"
#include "field/ov021_021cfa58.h"
#include "field/field_player.h"
#include "field/field_slope_slip.h"
#include "field/field_water_current.h"
#include "field/field_script_event.h"
#include "field/field_sound.h"
#include "field/field_status.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "pml/poke_party.h"
#include "save/player_savedata.h"
#include "save/records.h"
#include "save/save_control.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/pass_power.h"
#include "save/high_link.h"
#include "field/field_daycare.h"
#include "field/field_pass_power.h"
#include "field/ov021_0219d524.h"
#include "field/ov021_021bd068.h"
#include "field/field_actor.h"
#include "field/event_mapchange.h"
#include "gfl/key.h"
#include "field/event_data.h"
#include "pml/poke_friendship.h"
#include "field/event_wild_battle.h"
#include "field/field_rail.h"
#include "field/ov021_021bddc0.h"
#include "field/field_gimmick_d06.h"
#include "field/gimmick_roller_coaster.h"
#include "field/eventdata_system.h"
#include "system/wipe.h"
#include "field/shortcut_menu.h"
#include "field/subscreen.h"
#include "field/field_menu.h"
#include "field/field_script.h"
#include "field/field_event.h"
#include "field/castelia_rush.h"
#include "system/game_comm.h"
#include "gfl/net.h"
#include "field/union_main.h"
#include "field/union_oneself.h"
#include "field/union_actor.h"
#include "field/pleasure_boat.h"
#include "field/event_trainer_eye.h"
#include "save/event_work.h"
#include "field/intrude_comm.h"
#include "field/intrude_field.h"
#include "field/scrcmd_entree_forest.h"

// A surf exit's work (pokebw2's SurfExitWork)
typedef struct SurfExitWork {
    u32 unused;
    u16 dir;
    u16 overEdge;
    GameSystem *gsys;
    Field *field;
} SurfExitWork;

// What the field's event checks know of the player this frame (pokebw2's FieldInteractWk; Black lacks its stepStarted)
typedef struct FieldInteractWk {
    HeapID heapId;
    GameSystem *gsys;
    GameData *gameData;
    EventData *eventData;
    u16 zoneId;
    Field *field;
    FieldPlayer *player;
    s32 observeStatus;
    s32 moveStatus;
    u16 faceDir;
    u16 reserveScript;
    u32 pressed;
    u32 held;
    u16 moveDir;
    VecFx32 *pos;
    u32 tileType;
    u32 aPressed : 1;
    u32 xPressed : 1;
    u32 stepped : 1;
    u32 walking : 1;
    u32 turning : 1;
    u32 dirHeld : 1;
    u32 yPressed : 1;
    u32 idle : 1;
    u32 unk_3c_8 : 1;
    u32 verticalOnly : 1;
    u32 canAct : 1;
    u32 unk_3c_11 : 1;
} FieldInteractWk;

typedef struct Ov021TileEvent Ov021TileEvent;
struct Ov021TileEvent {
    BOOL (*isTile)(u32 tileClass);
    u16 key;
    u16 result;
};

static const Ov021TileEvent data_ov021_021d3ce8[13] = {
    { MapTile_IsPC, 0, 0x276a },
    { MapTile_IsUnused0xD7, 9, 0xffff },
    { MapTile_IsTV, 0, 0x2918 },
    { MapTile_IsBookcaseA, 9, 0x09c4 },
    { MapTile_IsBookcaseB, 9, 0x09c5 },
    { MapTile_IsBookcaseC, 9, 0x09c6 },
    { MapTile_IsBookcaseD, 9, 0x09c7 },
    { MapTile_IsTrashCanA, 9, 0x09c8 },
    { MapTile_IsTrashCanB, 9, 0x09c8 },
    { MapTile_IsAssortmentA, 9, 0x09c9 },
    { MapTile_IsAssortmentB, 9, 0x09ca },
    { MapTile_IsAssortmentC, 9, 0x09cb },
    { MapTile_IsVendingMachine, 0, 0x28a0 },
};

GameEventReturnCode EventSurfExit_Callback(GameEvent *event, u32 *state, void *data);

GameEvent *FieldEventProvider_Grid(GameSystem *gsys, Field *field);
GameEvent *FieldEventProvider_NoGrid(GameSystem *gsys, Field *field);
void *func_ov021_021a1358(FieldSubscreen *param_1);
GameEvent *FieldScript_CheckSceneChangeEvent(GameSystem *param_1, HeapID param_2);
GameEvent *CheckQuicksandEvent(FieldInteractWk *work);
GameEvent *CheckTriggerScrEvent(FieldInteractWk *work, u32 param_2);
u16 func_ov010_02162928(const struct EventData *eventData, const struct EventWork *eventWork, const VecFx32 *position, u32 type);
GameEvent *CheckAbyssalRuinsStepEvent(GameSystem *param_1, Field *param_2);
u32 EncountSave_DecrementRepelSteps(EncountSave *param_1);

void PokeParty_UtilUpdateHatchSteps(PokeParty *party);
u32 DayCare_Update(DayCare *param_1);
u32 func_ov021_0218aeac(ZoneWarp *warp);
BOOL CheckWarpDirectionMatch2(const ZoneWarp *warp, u16 direction);
s32 GetWarpForWalkIn(FieldInteractWk *work, VecFx32 *position);
void func_ov021_0218adc8(FieldInteractWk *work, u32 warpId, VecFx32 *position);
GameEvent *func_ov021_0218ad80(FieldInteractWk *work, Field *field, u32 warpId, VecFx32 *position);
GameEvent *func_ov021_021aa248(EncountSystem *system, void *mapEncount);
void *func_ov021_0218dc70(FieldG3DMapper *mapper);
GameEvent *func_ov021_021c46f8(GameSystem *gsys, Field *field, u16 direction);
typedef struct FieldmapEventWork {
    u16 unk_00;
    u8 unk_02[2];
    u16 unk_04;
    u16 unk_06;
} FieldmapEventWork;

void func_ov021_0218b2a8(FieldInteractWk *work, u32 warpId, FieldmapEventWork *warpInfo);
GameEvent *func_ov021_0218b2e8(FieldInteractWk *work, Field *field, u32 warpId, RailPosition *position);
GameEvent *func_ov021_0218ac84(Field *param_1, GameSystem *param_2, GameData *param_3);
void func_ov021_0218ab9c(GameData *gameData, Field *field);
GameEvent *func_ov021_0218ac08(GameSystem *param_1, Field *param_2, u16 param_3);
GameEvent *func_ov021_0218ab50(FieldInteractWk *work, Field *param_2, GameSystem *param_3, GameData *param_4);
GameEvent *func_ov021_0218ac4c(Field *param_1, GameSystem *param_2, GameData *param_3);
BOOL func_ov021_0218b6f8(FieldInteractWk *work, Field *field);
u32 func_ov021_0218b630(GameData *gameData, u32 param_2);
GameEvent *func_ov038_021f3a88(GameSystem *, s32, s32);
GameEvent *func_ov041_021f37e0(GameSystem *);
u32 func_ov021_021a3080(FieldPlayer *player);
u32 func_ov021_021a308c(FieldPlayer *player);
void FieldEventProvider_InitCheckData(FieldInteractWk *work, GameSystem *gsys, Field *field);
GameEvent *func_ov021_0218a738(FieldInteractWk *work);
void func_ov021_0218a728(GameData *param_1);
GameEvent *func_ov021_0218b0cc(FieldInteractWk *work, GameSystem *gsys, Field *field);
GameEvent *func_ov021_0218a9b0(FieldInteractWk *work, Field *field);
GameEvent *func_ov021_0218b208(FieldInteractWk *work, GameSystem *gsys, Field *field);
FieldActor *func_ov021_0218b53c(FieldInteractWk *work, Field *field);
GameEvent *CreatePlayerXActorScriptCall(GameSystem *, Field *, u32, FieldActor *, FieldActor *, HeapID);
BOOL func_ov021_0218b310(FieldPlayer *player, FieldInteractWk *args);
GameEvent *func_ov021_0218b13c(FieldInteractWk *work, GameSystem *gsys, Field *field, RailPosition *position);
GameEvent *func_ov021_0218b24c(FieldInteractWk *work, GameSystem *gsys, Field *field);
GameEvent *func_ov021_021c06cc(GameSystem *gsys, Field *field);
GameEvent *func_ov021_021a0eb0(FieldSubscreen *, u32);
GameEvent *func_ov021_0218aca8(GameSystem *gsys);
GameEvent *FieldEventProvider_NoGrid_CheckForEvent(GameSystem *gsys, Field *field);
CasteliaRush *Field_GetCasteliaRush(Field *field);
void func_ov021_0218a6e8(GameSystem *param_1, Field *param_2);
GameEvent *FieldEventProvider_Grid_CheckForEvent(GameSystem *gsys, Field *field, u32 *cancel, u32 *keep);
FieldActor *FindActorForInteraction(FieldInteractWk *param_1, Field *param_2);
GameEvent *func_ov021_0218a76c(FieldInteractWk *work);
GameEvent *func_ov021_0218acd0(FieldInteractWk *work, Field *field, VecFx32 *position);
GameEvent *func_ov021_0218ad28(FieldInteractWk *work, Field *param_2);
GameEvent *func_ov021_0218a7f0(FieldInteractWk *work);
GameEvent *func_ov021_0218a824(FieldInteractWk *work);
GameEvent *func_ov021_0218a878(FieldInteractWk *work, GameSystem *gsys, Field *field);
GameEvent *func_ov018_0217ca84(GameSystem *param_1, u32 param_2);
GameEvent *func_ov018_0217fc6c(GameSystem *param_1, Field *param_2, IntrudeComm *param_3, FieldActor *param_4, u32 param_5, u16 param_6);
GameEvent *func_ov018_0217fb5c(GameSystem *param_1, Field *param_2, IntrudeComm *param_3, FieldActor *param_4, u32 param_5, u16 param_6);
u16 func_ov021_0218b56c(FieldInteractWk *work, Field *field);
GameEvent *func_ov021_0218b408(FieldInteractWk *work, GameSystem *gsys, Field *field);
GameEvent *func_ov021_0218aed4(FieldInteractWk *work, GameSystem *gsys, Field *field);
GameEvent *func_ov021_0218af90(FieldInteractWk *work, GameSystem *gsys, Field *field);





GameEvent *FieldEventProvider_Grid_CheckForEvent(GameSystem *gsys, Field *field, u32 *cancel, u32 *keep) {
    FieldInteractWk ctx;
    GameEvent *event;
    GameCommSys *commSys;
    IntrudeComm *intrude;
    FieldActor *actor;
    FieldActor *actor2;
    FieldActor *targetActor;
    FieldSubscreen *subscreen;
    EventWork *eventWork;
    VecFx32 pos;
    u32 out;
    u16 faceDir;
    u32 scrId;
    HighLinkSave *highLink;
    u16 zone;
    u32 flag;
    u16 result;
    u32 linkResult;
    s32 cx;
    s32 cy;
    s32 cz;
    *cancel = 0;
    *keep = 0;
    FieldEventProvider_InitCheckData(&ctx, gsys, field);
    event = func_ov021_0218a738(&ctx);
    if (event != NULL) {
        *cancel = 1;
        return event;
    }
    event = func_ov021_021c868c(gsys, *func_02012ed0(ctx.gameData));
    if (event != NULL) {
        return event;
    }
    event = EventTrainerEye_CheckAll(field);
    if (event != NULL) {
        *cancel = 1;
        return event;
    }
    if (ctx.stepped) {
        func_ov021_0218a728(ctx.gameData);
    }
    if (ctx.stepped) {
        event = func_ov021_0218a76c(&ctx);
        if (event != NULL) {
            *cancel = 1;
            return event;
        }
        event = func_ov021_0218acd0(&ctx, field, ctx.pos);
        if (event != NULL) {
            return event;
        }
        event = func_ov021_0218ad28(&ctx, field);
        if (event != NULL) {
            return event;
        }
    }
    event = func_ov021_0218a9b0(&ctx, field);
    if (event != NULL) {
        return event;
    }
    if (ctx.walking) {
        event = func_ov021_0218a7f0(&ctx);
        if (event != NULL) {
            return event;
        }
    }
    event = func_ov021_0218a824(&ctx);
    if (event != NULL) {
        return event;
    }
    event = func_ov021_0218a878(&ctx, gsys, field);
    if (event != NULL) {
        return event;
    }
    event = func_ov021_0218b208(&ctx, gsys, field);
    if (event != NULL) {
        return event;
    }
    flag = 0;
    if (EventWork_FlagGet(GameData_GetEventWork(ctx.gameData), 0x964) == 1) {
        flag |= 1;
    }
    event = FieldPlayer_CheckObjContactEvent(ctx.player, ctx.pressed, ctx.held, flag);
    if (event != NULL) {
        return event;
    }
    commSys = GSYS_GetGameCommSystem(gsys);
    intrude = func_ov010_02158344(commSys);
    actor = FieldPlayer_GetActor(ctx.player);
    if (func_ov018_0217bfd8(gsys, ctx.zoneId) == 1) {
        return func_ov018_0217ca84(gsys, 0);
    }
    if (GameCommSys_BootCheck(commSys) == 2 && intrude != NULL) {
        if (GameData_IsForceSeasonSync(ctx.gameData) == 1 && func_ov018_0217c070(intrude) == 1) {
            return func_ov018_0217b810(gsys, field);
        }
        if (func_ov011_0217689c(intrude, ctx.zoneId) == 1) {
            return EventEntreeForestWarp_CreateToAnother(gsys, 9, GameData_Is30FPSMode(ctx.gameData));
        }
        if (ctx.canAct == 1 && func_ov011_021754b4(intrude, &out) == 1) {
            FieldPlayer_ForceBrake(ctx.player);
            return func_ov018_0217fc6c(gsys, field, intrude, actor, out, ctx.heapId);
        }
        if (ctx.walking) {
            event = func_ov018_0217bde0(gsys, intrude, ctx.player);
            if (event != NULL) {
                return event;
            }
        }
        if (ctx.aPressed) {
            if (func_ov018_0217afc4(intrude, ctx.player, &out) == 1) {
                FieldPlayer_ForceBrake(ctx.player);
                return func_ov018_0217fb5c(gsys, field, intrude, actor, out, ctx.heapId);
            }
        }
    }
    if (ctx.aPressed) {
        faceDir = FieldPlayer_GetFaceDir(ctx.player);
        FieldPlayer_GetWPos(ctx.player, &pos);
        ExpandVecInGridDir(faceDir, &pos, 0x10000);
        targetActor = FindActorForInteraction(&ctx, field);
        if (targetActor != NULL) {
            scrId = FldAct_GetSCRID(targetActor);
            if (FieldScript_CheckSCRID(scrId) == 1) {
                actor2 = FieldPlayer_GetActor(ctx.player);
                FieldPlayer_ForceBrake(ctx.player);
                *cancel = 1;
                return CreatePlayerXActorScriptCall(gsys, field, scrId, actor2, targetActor, ctx.heapId);
            }
        }
        eventWork = GameData_GetEventWork(ctx.gameData);
        result = func_ov010_02162a54(ctx.eventData, eventWork, &pos, faceDir);
        if (result != 0xffff) {
            return EventScriptCall_Create(gsys, result, NULL, ctx.heapId);
        }
        result = func_ov021_0218b56c(&ctx, field);
        if (result != 0xffff) {
            return EventScriptCall_Create(gsys, result, NULL, ctx.heapId);
        }
        cx = pos.x >> FX32_SHIFT;
        cy = pos.y >> FX32_SHIFT;
        cz = pos.z >> FX32_SHIFT;
        highLink = getHighLinkBlockAddress(GameData_GetSaveControl(ctx.gameData));
        linkResult = func_ov021_021cfac0(highLink, Field_GetPlayerStateZoneID(field), cx / 16, cy / 16, cz / 16);
        if (linkResult != 0xff) {
            return func_ov021_021cfc14(gsys, ctx.heapId, linkResult);
        }
        if (GetAbyssalRuinsDiveZoneID(field, &zone) == 1) {
            return EventScriptCall_Create(gsys, 0x271b, NULL, ctx.heapId);
        }
        if (IsZoneAbyssalRuinsInside(ctx.zoneId)) {
            result = func_ov010_02162940(ctx.eventData, GameData_GetEventWork(ctx.gameData), &pos);
            if (result != 0xffff) {
                return EventScriptCall_Create(gsys, result, NULL, ctx.heapId);
            }
        }
    }
    if (ctx.observeStatus == 3 || ctx.observeStatus == 0) {
        event = func_ov021_0218b408(&ctx, gsys, field);
        if (event != NULL) {
            return event;
        }
    }
    if (ctx.dirHeld) {
        event = func_ov021_0218aed4(&ctx, gsys, field);
        if (event != NULL) {
            return event;
        }
        event = func_ov021_0218af90(&ctx, gsys, field);
        if (event != NULL) {
            return event;
        }
    }
    if (ctx.yPressed && GFL_WipeIsFinished()) {
        subscreen = Field_GetSubscreen(field);
        if (IsExistAnyYShortcut(gsys) && FieldSubscreen_IsReady(subscreen)) {
            event = CallYButtonShortcutMenu(gsys, field, ctx.heapId);
            if (event != NULL) {
                return event;
            }
        }
    }
    if (func_0201212c(gsys) == 1 && GFL_WipeIsFinished() && ctx.canAct) {
        return func_ov021_021c06cc(gsys, field);
    }
    if (ctx.xPressed && GFL_WipeIsFinished()) {
        subscreen = Field_GetSubscreen(field);
        if (FieldSubscreen_GetScreenID(subscreen) == 6) {
            return NULL;
        }
        *keep = 1;
        return EventFieldMenu_Create(gsys, field, ctx.heapId);
    }
    if (GFL_WipeIsFinished()) {
        subscreen = Field_GetSubscreen(field);
        event = func_ov021_021a0eb0(subscreen, ctx.idle);
        if (event != NULL) {
            return event;
        }
    }
    if (ctx.idle && GFL_WipeIsFinished()) {
        event = func_ov021_0218aca8(gsys);
        if (event != NULL) {
            return event;
        }
    }
    event = func_ov018_0217b410(field, gsys, ctx.player);
    if (event != NULL) {
        return event;
    }
    return NULL;
}

GameEvent *FieldEventProvider_Grid(GameSystem *gsys, Field *field) {
    Field *nowField = GSYS_GetField(gsys);
    EncountSystem *encount = Field_GetEncountSystem(nowField);
    PlaceName *placeName;
    CasteliaRush *rush;
    GameEvent *event;
    BOOL cancelPhenomenon;
    BOOL keepBalance;
    EncountSystem_CancelPhenomenonIfActorHit(encount);
    event = FieldEventProvider_Grid_CheckForEvent(gsys, field, &cancelPhenomenon, &keepBalance);
    if (event != NULL) {
        if (cancelPhenomenon) {
            EncountSystem_CancelPhenomenon(encount);
        }
        placeName = Field_GetPlaceName(nowField);
        func_ov021_021bc5bc(placeName);
        FieldPlayer_InterruptCatwalkBalance(Field_GetPlayer(nowField), keepBalance);
        func_ov021_021a356c(Field_GetPlayer(nowField));
        rush = Field_GetCasteliaRush(nowField);
        if (rush != NULL) {
            func_ov055_021f3b10(rush);
        }
        func_ov021_0218a6e8(gsys, nowField);
    }
    return event;
}

GameEvent *func_ov021_0218a0e4(GameSystem *gsys, Field *field) {
    FieldInteractWk ctx;
    GameCommSys *comm = GSYS_GetGameCommSystem(gsys);
    UnionSystem *sys = func_02026474(comm);
    GameEvent *event;
    FieldActor *actor;
    FieldActor *playerActor;
    int scrId;
    u16 uid;
    FieldEventProvider_InitCheckData(&ctx, gsys, field);
    if (sys == NULL) {
        if (func_0200fb24() != 0) {
            if (GFL_WipeIsFinished()) {
                return func_ov021_021c06cc(gsys, field);
            }
            return NULL;
        }
        return func_ov020_021850fc(gsys, field);
    }
    if (func_ov012_0217391c(sys) == 1) {
        if (func_ov012_02173928(sys) == 0) {
            func_02026400(comm);
        }
        return NULL;
    }
    if (ctx.stepped) {
        event = CheckTriggerScrEvent(&ctx, 9);
        if (event != NULL) {
            return event;
        }
    }
    event = func_ov019_0217b80c(gsys, field);
    if (event != NULL) {
        return event;
    }
    if (ctx.aPressed) {
        event = func_ov019_0217b83c(gsys, field);
        if (event != NULL) {
            return event;
        }
    }
    if (ctx.aPressed && func_ov012_0217394c(gsys) == 1) {
        actor = FindActorForInteraction(&ctx, field);
        if (actor != NULL) {
            scrId = FldAct_GetSCRID(actor);
            if (FieldScript_CheckSCRID(scrId) == 1) {
                playerActor = FieldPlayer_GetActor(ctx.player);
                uid = GetActorUID(actor);
                if (func_ov019_0217af38(uid) == 0) {
                    FieldPlayer_ForceBrake(ctx.player);
                    return CreatePlayerXActorScriptCall(gsys, field, scrId, playerActor, actor, ctx.heapId);
                }
            }
        }
    }
    if (func_0201212c(gsys) == 1 && GFL_WipeIsFinished()) {
        return func_ov021_021c06cc(gsys, field);
    }
    if (func_ov012_0217394c(gsys) == 0) {
        return NULL;
    }
    if (ctx.xPressed && GFL_WipeIsFinished()) {
        return EventFieldMenu_CreateUnionRoom(gsys, field, ctx.heapId);
    }
    return NULL;
}


GameEvent *FieldEventProvider_NoGrid(GameSystem *gsys, Field *field) {
    GameEvent *event = FieldEventProvider_NoGrid_CheckForEvent(gsys, field);
    Field *nowField;
    PlaceName *placeName;
    CasteliaRush *rush;
    if (event != NULL) {
        nowField = GSYS_GetField(gsys);
        placeName = Field_GetPlaceName(nowField);
        func_ov021_021bc5bc(placeName);
        rush = Field_GetCasteliaRush(nowField);
        if (rush != NULL) {
            func_ov055_021f3b10(rush);
        }
        func_ov021_0218a6e8(gsys, nowField);
    }
    return event;
}

GameEvent *FieldEventProvider_NoGrid_CheckForEvent(GameSystem *gsys, Field *field) {
    FieldInteractWk ctx;
    FieldActor *playerActor;
    FieldActor *targetActor;
    EventWork *eventWork;
    RailPosition rail;
    RailPosition railPos;
    RailPosition posCopy;
    GameEvent *event;
    FieldSubscreen *subscreen;
    u16 scrId;
    u32 scrIdU32;
    FieldActor *actor2;
    FieldActor *railActor;
    u32 dir;
    FieldEventProvider_InitCheckData(&ctx, gsys, field);
    playerActor = FieldPlayer_GetActor(ctx.player);
    func_ov021_0219e634(playerActor, &rail);
    event = func_ov021_0218a738(&ctx);
    if (event != NULL) {
        return event;
    }
    if (ctx.stepped) {
        func_ov021_0218a728(ctx.gameData);
    }
    if (ctx.stepped) {
        railActor = FieldPlayer_GetActor(ctx.player);
        eventWork = GameData_GetEventWork(ctx.gameData);
        GetActorRailPos(railActor, &railPos);
        scrId = func_ov010_02162958(ctx.eventData, eventWork, &railPos);
        if (scrId != 0xffff) {
            return EventScriptCall_Create(gsys, scrId, NULL, ctx.heapId);
        }
        event = func_ov021_0218b0cc(&ctx, gsys, field);
        if (event != NULL) {
            return event;
        }
        event = func_ov021_0218a9b0(&ctx, field);
        if (event != NULL) {
            return event;
        }
    }
    event = func_ov021_0218b208(&ctx, gsys, field);
    if (event != NULL) {
        return event;
    }
    if (ctx.aPressed) {
        targetActor = func_ov021_0218b53c(&ctx, field);
        if (targetActor != NULL) {
            scrIdU32 = FldAct_GetSCRID(targetActor);
            if (FieldScript_CheckSCRID(scrIdU32) == 1) {
                actor2 = FieldPlayer_GetActor(ctx.player);
                FieldPlayer_ForceBrake(ctx.player);
                return CreatePlayerXActorScriptCall(gsys, field, scrIdU32, actor2, targetActor, ctx.heapId);
            }
        }
        eventWork = GameData_GetEventWork(ctx.gameData);
        dir = GetActorFaceDir(playerActor);
        posCopy = rail;
        scrId = func_ov010_02162a5c(ctx.eventData, eventWork, &posCopy, dir);
        if (scrId != 0xffff) {
            return EventScriptCall_Create(gsys, scrId, NULL, ctx.heapId);
        }
    }
    if (ctx.dirHeld) {
        if (func_ov021_0218b310(Field_GetPlayer(field), &ctx) == 0) {
            event = func_ov021_0218b13c(&ctx, gsys, field, &rail);
            if (event != NULL) {
                return event;
            }
        }
    }
    event = func_ov021_0218b24c(&ctx, gsys, field);
    if (event != NULL) {
        return event;
    }
    if (ctx.yPressed && GFL_WipeIsFinished()) {
        subscreen = Field_GetSubscreen(field);
        if (IsExistAnyYShortcut(gsys) && FieldSubscreen_IsReady(subscreen)) {
            event = CallYButtonShortcutMenu(gsys, field, ctx.heapId);
            if (event != NULL) {
                return event;
            }
        }
    }
    if (func_0201212c(gsys) == 1 && GFL_WipeIsFinished() && ctx.canAct) {
        return func_ov021_021c06cc(gsys, field);
    }
    if (ctx.xPressed && GFL_WipeIsFinished()) {
        subscreen = Field_GetSubscreen(field);
        if (FieldSubscreen_GetScreenID(subscreen) == 6) {
            return NULL;
        }
        return EventFieldMenu_Create(gsys, field, ctx.heapId);
    }
    {
        u32 wipeFlag = (ctx.idle && GFL_WipeIsFinished()) ? 1 : 0;
        subscreen = Field_GetSubscreen(field);
        event = func_ov021_021a0eb0(subscreen, wipeFlag);
        if (event != NULL) {
            return event;
        }
    }
    if (ctx.idle && GFL_WipeIsFinished()) {
        event = func_ov021_0218aca8(gsys);
        if (event != NULL) {
            return event;
        }
    }
    return NULL;
}

void func_ov021_0218a4bc(GameSystem *param_1, Field *param_2) {
    if (Field_GetResolvedControllerTypeID(param_2) == 0) {
        FieldEventProvider_Grid(param_1, param_2);
        return;
    }
    FieldEventProvider_NoGrid(param_1, param_2);
}

void FieldEventProvider_InitCheckData(FieldInteractWk *work, GameSystem *gsys, Field *field) {
    FieldActor *actor;
    u8 count;
    MI_CpuFill8(work, 0, sizeof(FieldInteractWk));
    work->heapId = Field_GetHeapID(field);
    work->gsys = gsys;
    work->gameData = GSYS_GetGameData(gsys);
    work->eventData = GameData_GetEventData(work->gameData);
    work->zoneId = Field_GetPlayerStateZoneID(field);
    work->pressed = GCTX_HIDGetPressedKeys();
    work->held = GCTX_HIDGetHeldKeys();
    work->moveDir = 9;
    if (work->held & 0x40) {
        work->moveDir = 0;
    } else if (work->held & 0x80) {
        work->moveDir = 1;
    } else if (work->held & 0x20) {
        work->moveDir = 2;
    } else if (work->held & 0x10) {
        work->moveDir = 3;
    }
    work->field = field;
    work->player = Field_GetPlayer(field);
    FieldPlayer_UpdateActionStatusObserver(work->player);
    work->moveStatus = func_ov021_021a3080(work->player);
    work->observeStatus = func_ov021_021a308c(work->player);
    actor = FieldPlayer_GetActor(work->player);
    work->faceDir = GetActorFaceDir(actor);
    work->pos = GetMModelWPosPtr(actor);
    work->reserveScript = FieldStatus_GetReserveScript(GameData_GetFieldStatus(work->gameData));
    work->tileType = FieldPlayer_GetTileTypeUnder(work->player);
    if (work->observeStatus == 3 || work->observeStatus == 0) {
        work->canAct = 1;
        work->aPressed = (work->pressed & 1) != 0;
        work->xPressed = (work->pressed & 0x400) != 0;
        work->yPressed = (work->pressed & 0x800) != 0;
        count = 0;
        count += work->aPressed;
        count += work->xPressed;
        count += work->yPressed;
        if (count == 0) {
            work->idle = 1;
        }
    }
    work->stepped = 0;
    work->verticalOnly = 0;
    if (work->moveStatus == 1) {
        work->stepped = (work->observeStatus == 3);
    }
    if (FieldPlayer_GetVerticalMoveOnlyFlag(work->player)) {
        work->verticalOnly = 1;
    }
    work->walking = (work->observeStatus == 3);
    work->turning = 0;
    if (work->observeStatus != 3 && work->moveStatus == 2) {
        work->turning = 1;
    }
    if ((work->faceDir == 0 && (work->held & 0x40)) || (work->faceDir == 1 && (work->held & 0x80)) || (work->faceDir == 2 && (work->held & 0x20)) || (work->faceDir == 3 && (work->held & 0x10))) {
        work->dirHeld = 1;
    } else {
        work->dirHeld = 0;
    }
}


void func_ov021_0218a6e8(GameSystem *param_1, Field *param_2) {
    FieldSubscreen *pFVar1;
    CGearSw *pCVar2;
    GameData *pGVar3;
    FieldSound *pFVar4;

    pFVar1 = Field_GetSubscreen(param_2);
    pCVar2 = func_ov021_021a1358(pFVar1);
    if (pCVar2 == (CGearSw *)0x0) {
        pGVar3 = GSYS_GetGameData(param_1);
        pFVar4 = GameData_GetFieldSoundSystem(pGVar3);
        func_02028a70(pFVar4);
        return;
    }
    if (func_ov029_021f2ab8(pCVar2) == 0) {
        pGVar3 = GSYS_GetGameData(param_1);
        pFVar4 = GameData_GetFieldSoundSystem(pGVar3);
        func_02028a70(pFVar4);
        func_ov029_021f2ad4(pCVar2);
    }
}

void func_ov021_0218a728(GameData *param_1) {
    GameRecords *records;

    records = GameData_GetRecords(param_1);
    RecordAddOne(records, 0);
}

GameEvent *func_ov021_0218a738(FieldInteractWk *work) {
    FieldStatus *status;
    GameEvent *pGVar1;

    if (work->reserveScript != 0) {
        status = GameData_GetFieldStatus(work->gameData);
        FieldStatus_ReserveScript(status, 0);
        pGVar1 = EventScriptCall_Create
                                              (work->gsys, work->reserveScript, (FieldActor *)0x0, work->heapId);
        return pGVar1;
    }
    pGVar1 = FieldScript_CheckSceneChangeEvent(work->gsys, work->heapId);
    if (pGVar1 == (GameEvent *)0x0) {
        pGVar1 = (GameEvent *)0x0;
    }
    return pGVar1;
}

GameEvent *func_ov021_0218a76c(FieldInteractWk *work) {
    GameEvent *pGVar1;

    pGVar1 = CheckQuicksandEvent(work);
    if (pGVar1 == (GameEvent *)0x0) {
        pGVar1 = CheckTriggerScrEvent(work, 9);
    }
    return pGVar1;
}

GameEvent *CheckTriggerScrEvent(FieldInteractWk *work, u32 param_2) {
    u16 tileClass;
    Field *field;
    EventWork *eventWork;
    FieldG3DMapper *mapper;
    u32 tileType;
    BOOL BVar2;
    GameEvent *pGVar3;
    VecFx32 VStack_20;

    field = GSYS_GetField(work->gsys);
    eventWork = GameData_GetEventWork(work->gameData);
    FieldPlayer_GetWPos(work->player, &VStack_20);
    mapper = Field_GetG3DMapper(field);
    tileType = GetTileTypeAtPos(mapper, &VStack_20);
    tileClass = GetTileClass(tileType);
    BVar2 = MapTile_IsQuicksand((u32)tileClass);
    if (BVar2 == 1) {
        return (GameEvent *)0x0;
    }
    tileClass = func_ov010_02162928(work->eventData, eventWork, &VStack_20, param_2);
    if (tileClass != 0xffff) {
        pGVar3 = EventScriptCall_Create(work->gsys, tileClass, (FieldActor *)0x0, work->heapId);
        return pGVar3;
    }
    return (GameEvent *)0x0;
}

GameEvent *func_ov021_0218a7f0(FieldInteractWk *work) {
    GameEvent *pGVar2;
    u32 moveDirByKey;

    pGVar2 = CheckTriggerScrEvent(work, (u32)work->faceDir);
    if (pGVar2 == (GameEvent *)0x0) {
        moveDirByKey = (u32)FieldPlayer_GetMoveDirByKey(work->player, work->held);
        if ((moveDirByKey == 9) || (moveDirByKey == work->faceDir)) {
            return NULL;
        }
        pGVar2 = CheckTriggerScrEvent(work, moveDirByKey);
        if (pGVar2 == (GameEvent *)0x0) {
            pGVar2 = (GameEvent *)0x0;
        }
    }
    return pGVar2;
}

GameEvent *func_ov021_0218a824(FieldInteractWk *work) {
    s32 state = work->observeStatus;
    u32 dir = 9;
    if (work->held & PAD_KEY_UP) {
        dir = 0;
    } else if (work->held & PAD_KEY_DOWN) {
        dir = 1;
    } else if (work->held & PAD_KEY_LEFT) {
        dir = 2;
    } else if (work->held & PAD_KEY_RIGHT) {
        dir = 3;
    }
    if (dir == work->faceDir) {
        return NULL;
    }
    if (dir == 9) {
        return NULL;
    }
    switch (state) {
    case 1:
    case 2:
        return NULL;
    }
    return CheckTriggerScrEvent(work, dir);
}

GameEvent *func_ov021_0218a878(FieldInteractWk *work, GameSystem *gsys, Field *field) {
    u16 dir;
    if (!IsZoneEntralinkHub(work->zoneId)) {
        return NULL;
    }
    dir = FieldPlayer_GetMoveDirByKey(work->player, work->held);
    if (dir == 9) {
        return NULL;
    }
    return func_ov018_0217b4d8(gsys, field, work->player, work->pos, work->faceDir, dir);
}

GameEvent *CheckQuicksandEvent(FieldInteractWk *work) {
    Field *field = GSYS_GetField(work->gsys);
    FieldPlayer *player = Field_GetPlayer(field);
    VecFx32 pos;
    FieldG3DMapper *mapper;
    ZoneTrigger *trigger;
    FieldActor *actor;
    u32 exState;
    u16 acmd;
    u32 gx;
    u32 gz;
    const u16 *values;
    FieldPlayer_GetWPos(player, &pos);
    mapper = Field_GetG3DMapper(field);
    if (MapTile_IsQuicksand(GetTileClass(GetTileTypeAtPos(mapper, &pos))) != TRUE) {
        return NULL;
    }
    trigger = FindQuicksandTrigger(work->eventData, GameData_GetEventWork(work->gameData), &pos, 9);
    if (trigger == NULL) {
        return NULL;
    }
    if (trigger->isRail != 0) {
        return NULL;
    }
    values = &trigger->pos.grid.x;
    gz = values[1] + (u32)values[3] / 2;
    gx = trigger->pos.grid.x + (u32)values[2] / 2;
    if (gx == (pos.x >> FX32_SHIFT) / 16 && gz == (pos.z >> FX32_SHIFT) / 16) {
        return EventScriptCall_Create(work->gsys, trigger->scrId, NULL, work->heapId);
    }
    exState = FieldPlayer_GetExState(player);
    actor = FieldPlayer_GetActor(player);
    acmd = FldAct_GetAcmd(actor);
    if (exState != 0 || acmd == 0x58 || acmd == 0x59 || acmd == 0x5a || acmd == 0x5b || acmd == 0xa3 || acmd == 0xa4 || acmd == 0xa5 || acmd == 0xa6) {
        return EventScriptCall_Create(work->gsys, trigger->scrId, NULL, work->heapId);
    }
    return NULL;
}



GameEvent *func_ov021_0218a9b0(FieldInteractWk *work, Field *field) {
    GameSystem *gsys;
    GameData *gameData;
    GameEvent *event;
    Field_GetHeapID(field);
    gsys = Field_GetGameSystem(field);
    gameData = GSYS_GetGameData(gsys);
    if (work->stepped || work->verticalOnly) {
        event = func_ov021_0218ac84(field, gsys, gameData);
        if (event != NULL) {
            return event;
        }
    }
    if (work->stepped == 0) {
        return NULL;
    }
    func_ov021_0218ab9c(work->gameData, field);
    event = func_ov021_0218ac08(gsys, field, work->zoneId);
    if (event != NULL) {
        return event;
    }
    event = func_ov021_0218ab50(work, field, gsys, gameData);
    if (event != NULL) {
        return event;
    }
    event = func_ov021_0218ac4c(field, gsys, gameData);
    if (event != NULL) {
        return event;
    }
    return NULL;
}



u32 PokeParty_UtilGetEggHatchSpeed(PokeParty *param_1) {
    s32 pkmCount;
    PartyPkm *pkm;
    u32 result;
    u32 index;

    pkmCount = PokeParty_GetPkmCount(param_1);
    index = 0;
    if (0 < pkmCount) {
        do {
            pkm = PokeParty_GetPkm(param_1, index);
            result = PokeParty_GetParam(pkm, 10, (void *)0x0);
            if ((PokeParty_GetParam(pkm, 0x4c, (void *)0x0) == 0) && ((result == 0x31 || (result == 0x28)))) {
                return 2;
            }
            index = index + 1;
        } while ((s32)index < pkmCount);
    }
    return 1;
}

void PokeParty_UtilUpdateHatchSteps(PokeParty *party) {
    s32 count = PokeParty_GetPkmCount(party);
    u32 limit = PokeParty_UtilGetEggHatchSpeed(party);
    s32 i;
    for (i = 0; i < count; i++) {
        PartyPkm *pkm = PokeParty_GetPkm(party, i);
        u32 flag = PokeParty_GetParam(pkm, 0x4c, NULL);
        u32 value = PokeParty_GetParam(pkm, 9, NULL);
        if (flag == 1) {
            u32 newValue = 0;
            if (value >= limit) {
                newValue = value - limit;
            }
            PokeParty_SetParam(pkm, 9, newValue);
        }
    }
}


void GameData_UpdateEggCycles(GameData *param_1, PokeParty *param_2) {
    SaveControl *save;
    PlayerSave *playerSave;
    u32 uStack_10;

    save = GameData_GetSaveControl(param_1);
    playerSave = SaveControl_GetPlayerSave(save);
    PlayerSave_GetEggCycleSubsteps(playerSave, &uStack_10);
    uStack_10 = uStack_10 + PassPower_ApplyHatch(0x100);
    if (0x10000 < uStack_10) {
        PokeParty_UtilUpdateHatchSteps(param_2);
        uStack_10 = 0;
    }
    PlayerSave_SetEggCycleSubsteps(playerSave, uStack_10);
}

u32 isEggReadyToHatch(PokeParty *param_1) {
    s32 pkmCount;
    PartyPkm *pkm;
    u32 result;
    u32 result2;
    u32 index;

    pkmCount = PokeParty_GetPkmCount(param_1);
    index = 0;
    if (0 < pkmCount) {
        do {
            pkm = PokeParty_GetPkm(param_1, index);
            result = PokeParty_GetParam(pkm, 0x4c, (void *)0x0);
            result2 = PokeParty_GetParam(pkm, 9, (void *)0x0);
            if ((result == 1) && (result2 == 0)) {
                return 1;
            }
            index = index + 1;
        } while ((s32)index < pkmCount);
    }
    return 0;
}

GameEvent *func_ov021_0218ab50(FieldInteractWk *work, Field *param_2, GameSystem *param_3, GameData *param_4) {
    u16 heapId;
    PokeParty *pPVar1;
    DayCare *pDVar2;
    GameEvent *pGVar4;

    heapId = Field_GetHeapID(param_2);
    pPVar1 = GameData_GetParty(param_4);
    pDVar2 = Field_GetDayCare(param_2);
    DayCare_Update(pDVar2);
    GameData_UpdateEggCycles(param_4, pPVar1);
    if (isEggReadyToHatch(pPVar1) != 0) {
        pGVar4 = EventScriptCall_Create(param_3, 0x8ca, (FieldActor *)0x0, heapId);
        return pGVar4;
    }
    return (GameEvent *)0x0;
}

void func_ov021_0218ab9c(GameData *gameData, Field *field) {
    SaveControl *save = GameData_GetSaveControl(gameData);
    PlayerSave *playerSave = SaveControl_GetPlayerSave(save);
    u16 steps;
    BOOL reset = FALSE;
    steps = PlayerSave_GetHappinesStepCounter(playerSave) + 1;
    if (steps >= 0x80) {
        steps = 0;
        reset = TRUE;
    }
    PlayerSave_SetHappinessStepCounter(playerSave, steps);
    if (reset) {
        PokeParty *party = GameData_GetParty(gameData);
        u16 zoneId = Field_GetPlayerStateZoneID(field);
        u16 heapId = Field_GetHeapID(field);
        s32 count = PokeParty_GetPkmCount(party);
        s32 i;
        for (i = 0; i < count; i++) {
            happinessAdd(PokeParty_GetPkm(party, i), zoneId, heapId);
        }
    }
}


GameEvent *func_ov021_0218ac08(GameSystem *param_1, Field *param_2, u16 param_3) {
    GameData *gameData;
    SaveControl *save;
    PlayerSave *playerSave;
    BOOL BVar2;
    GameEvent *pGVar3;

    gameData = GSYS_GetGameData(param_1);
    save = GameData_GetSaveControl(gameData);
    playerSave = SaveControl_GetPlayerSave(save);
    pGVar3 = (GameEvent *)0x0;
    BVar2 = IsZoneAbyssalRuinsInside(param_3);
    if (BVar2 != 0) {
        pGVar3 = CheckAbyssalRuinsStepEvent(param_1, param_2);
        PlayerSave_SetAbyssalRuinsStepCounter(playerSave, PlayerSave_GetAbyssalRuinsStepCounter(playerSave) + 1);
    }
    return pGVar3;
}

GameEvent *func_ov021_0218ac4c(Field *param_1, GameSystem *param_2, GameData *param_3) {
    u16 heapId;
    SaveControl *save;
    EncountSave *pEVar1;
    GameEvent *pGVar3;

    save = GameData_GetSaveControl(param_3);
    pEVar1 = SaveControl_GetEncountSave(save);
    if ((EncountSave_DecrementRepelSteps(pEVar1) & 0xff) != 0) {
        heapId = Field_GetHeapID(param_1);
        pGVar3 = EventScriptCall_Create(param_2, 0x2792, (FieldActor *)0x0, heapId);
        return pGVar3;
    }
    return (GameEvent *)0x0;
}

GameEvent *func_ov021_0218ac84(Field *param_1, GameSystem *param_2, GameData *param_3) {
    EncountSystem *system;
    GameEvent *pGVar1;

    GameData_GetSaveControl(param_3);
    system = Field_GetEncountSystem(param_1);
    pGVar1 = CreateRandomPhenomenonEvent(system);
    if (pGVar1 == (GameEvent *)0x0) {
        UpdatePhenomenon(system);
        pGVar1 = (GameEvent *)0x0;
    }
    return pGVar1;
}

GameEvent *func_ov021_0218aca8(GameSystem *gsys) {
    if (PassPower_GetDepletedID() == 0x30) {
        return NULL;
    }
    return GameEvent_CreateOverlayDelegate(gsys, OVERLAY_ID(83), func_ov083_021f532c, NULL);
}

GameEvent *func_ov021_0218acd0(FieldInteractWk *work, Field *field, VecFx32 *position) {
    s32 warpId = GetWarpForWalkIn(work, position);
    ZoneWarp *warp;
    if (warpId == 0xffff) {
        return NULL;
    }
    warp = GetZoneWarpByID(work->eventData, warpId);
    if (IsWarpZoneOrWarpID0xFFFF(warp) == 1 || func_ov021_0218aeac(warp) == 0) {
        return NULL;
    }
    func_ov021_0218adc8(work, warpId, position);
    return func_ov021_0218ad80(work, field, warpId, position);
}



GameEvent *func_ov021_0218ad28(FieldInteractWk *work, Field *param_2) {
    u16 tileClass;
    FieldG3DMapper *mapper;
    u32 tileType;
    BOOL BVar2;
    GameEvent *pGVar3;
    VecFx32 VStack_20;

    FieldPlayer_GetWPos(work->player, &VStack_20);
    mapper = Field_GetG3DMapper(param_2);
    tileType = GetTileTypeAtPos(mapper, &VStack_20);
    tileClass = GetTileClass(tileType);
    BVar2 = IsTileSlopeSlip((u32)tileClass);
    if (BVar2 != 0) {
        pGVar3 = EventSlopeSlip_Create((GameEvent *)0x0, work->gsys, param_2);
        return pGVar3;
    }
    BVar2 = IsTileWaterCurrent((u32)tileClass);
    if (BVar2 != 0) {
        pGVar3 = EventWaterCurrentMove_Create((GameEvent *)0x0, work->gsys, param_2);
        return pGVar3;
    }
    return (GameEvent *)0x0;
}

GameEvent *func_ov021_0218ad80(FieldInteractWk *work, Field *field, u32 warpId, VecFx32 *position) {
    ZoneWarp *warp = GetZoneWarpByID(work->eventData, warpId);
    u32 dir;
    if (position != NULL) {
        dir = ZoneWarp_CalcPosWeightBitsGrid(warp, position);
    } else {
        dir = 0;
    }
    if (GetWarpTransitionType(warp) == 6) {
        return func_ov020_02185410(work->gsys, field, warp, dir);
    }
    return func_ov020_02185360(work->gsys, field, warp, dir);
}


void func_ov021_0218adc8(FieldInteractWk *work, u32 warpId, VecFx32 *position) {
    ZoneSpawnInfo spawn;
    u16 zoneId = Field_GetPlayerStateZoneID(work->field);
    SetupZoneWarpArrivalGrid(&spawn, zoneId, warpId, 0, 0, position->x, position->y, position->z);
    SetGameDataOutboundWarpRememberInfo(work->gameData, &spawn);
}

s32 GetWarpForWalkIn(FieldInteractWk *work, VecFx32 *position) {
    s32 warpId = GetWarpIDByPlayerPos(work->eventData, position);
    ZoneWarp *warp;
    if (warpId == 0xffff) {
        return 0xffff;
    }
    warp = GetZoneWarpByID(work->eventData, warpId);
    if (func_ov021_0218aeac(warp) == 0) {
        if (CheckWarpDirectionMatch2(warp, work->faceDir) == 0) {
            return 0xffff;
        }
    }
    return warpId;
}

void FieldInteractWk_CalcPlayerFrontPos(FieldInteractWk *work, VecFx32 *dst) {
    *dst = *work->pos;
    switch (work->faceDir) {
    case 0:
        dst->z -= 0x10000;
        break;
    case 1:
        dst->z += 0x10000;
        break;
    case 2:
        dst->x -= 0x10000;
        break;
    case 3:
        dst->x += 0x10000;
        break;
    }
}




u32 func_ov021_0218aeac(ZoneWarp *warp) {
    u32 type = GetWarpTransitionType(warp);

    return type == 0 || type == 5 || type == 6;
}


BOOL CheckWarpDirectionMatch2(const ZoneWarp *warp, u16 direction) {
    return CheckWarpDirectionMatch(warp, direction);
}

GameEvent *func_ov021_0218aed4(FieldInteractWk *work, GameSystem *gsys, Field *field) {
    VecFx32 pos;
    FieldG3DMapper *mapper;
    ZoneWarp *warp;
    s32 warpId;
    FieldInteractWk_CalcPlayerFrontPos(work, &pos);
    mapper = Field_GetG3DMapper(field);
    if ((GetTileFlags(GetTileTypeAtPos(mapper, &pos)) & 1) == 0) {
        return NULL;
    }
    warpId = GetWarpForWalkIn(work, work->pos);
    if (warpId != 0xffff) {
        warp = GetZoneWarpByID(work->eventData, warpId);
        if (IsWarpZoneOrWarpID0xFFFF(warp) == 0 && GetWarpTransitionType(warp) == 1) {
            func_ov021_0218adc8(work, warpId, work->pos);
            return func_ov021_0218ad80(work, field, warpId, work->pos);
        }
    }
    warpId = GetWarpForWalkIn(work, &pos);
    if (warpId != 0xffff) {
        warp = GetZoneWarpByID(work->eventData, warpId);
        if (IsWarpZoneOrWarpID0xFFFF(warp) == 0) {
            func_ov021_0218adc8(work, warpId, work->pos);
            return func_ov021_0218ad80(work, field, warpId, &pos);
        }
    }
    return NULL;
}

GameEvent *func_ov021_0218af90(FieldInteractWk *work, GameSystem *gsys, Field *field) {
    VecFx32 pos;
    VecFx32 dirPos;
    FieldG3DMapper *mapper;
    FieldActor *actor;
    fx32 height;
    u32 tileType;
    FieldInteractWk_CalcPlayerFrontPos(work, &pos);
    if (func_ov021_021be288(field, 6)) {
        return func_ov038_021f3a88(gsys, pos.x / 0x10000, pos.z / 0x10000);
    }
    if (func_ov021_021be288(field, 0x19) || func_ov021_021be288(field, 0x1a) || func_ov021_021be288(field, 0x1b)) {
        return func_ov051_021f38a4(field, work->faceDir);
    }
    mapper = Field_GetG3DMapper(field);
    tileType = GetTileTypeAtPos(mapper, &pos);
    if ((GetTileFlags(tileType) & 1) == 0) {
        return NULL;
    }
    if (FieldPlayer_GetExState(work->player) == 2 && func_ov021_0218b630(work->gameData, 0x7f) != 6) {
        if (IsTileWaterfall(GetTileClass(tileType)) == 1) {
            dirPos = pos;
            actor = FieldPlayer_GetActor(work->player);
            ExpandVecInGridDir(work->faceDir, &dirPos, 0x10000);
            if (GetHeightFromMap(actor, &dirPos, &height) == 1) {
                if (pos.y > height) {
                    return EventScriptCall_Create(work->gsys, 0x271f, NULL, work->heapId);
                }
            }
        }
    }
    if (GetWarpForWalkIn(work, &pos) == 0xffff) {
        if (func_ov021_021be288(field, 2)) {
            return func_ov039_021f404c(gsys);
        }
        if (func_ov021_021be288(field, 5)) {
            return func_ov041_021f37e0(gsys);
        }
    }
    return NULL;
}



GameEvent *func_ov021_0218b0cc(FieldInteractWk *work, GameSystem *gsys, Field *field) {
    RailPosition pos;
    FieldPlayer *player = Field_GetPlayer(field);
    FieldActor *actor = FieldPlayer_GetActor(player);
    s32 warpId;
    ZoneWarp *warp;
    GetActorRailPos(actor, &pos);
    warpId = GetWarpIDByPlayerPosRail(work->eventData, &pos);
    if (warpId == 0xffff) {
        return NULL;
    }
    warp = GetZoneWarpByID(work->eventData, warpId);
    if (IsWarpZoneOrWarpID0xFFFF(warp) == 1 || func_ov021_0218aeac(warp) == 0) {
        return NULL;
    }
    func_ov021_0218b2a8(work, warpId, (FieldmapEventWork *)(&pos.componentId));
    return func_ov021_0218b2e8(work, field, warpId, &pos);
}

GameEvent *func_ov021_0218b13c(FieldInteractWk *work, GameSystem *gsys, Field *field, RailPosition *position) {
    RailPosition rail;
    FieldPlayer *player = Field_GetPlayer(field);
    FieldActor *actor = FieldPlayer_GetActor(player);
    s32 warpId;
    ZoneWarp *warp;
    GetActorRailPos(actor, &rail);
    warpId = GetWarpIDByPlayerPosRail(work->eventData, &rail);
    if (warpId != 0xffff) {
        warp = GetZoneWarpByID(work->eventData, warpId);
        if (IsWarpZoneOrWarpID0xFFFF(warp) == 0 && CheckWarpDirectionMatch2(warp, work->faceDir) != 0 && GetWarpTransitionType(warp) == 1) {
            func_ov021_0218b2a8(work, warpId, (FieldmapEventWork *)(&rail.componentId));
            return func_ov021_0218b2e8(work, field, warpId, &rail);
        }
    }
    warpId = GetWarpIDByPlayerPosRail(work->eventData, position);
    if (warpId == 0xffff) {
        return NULL;
    }
    warp = GetZoneWarpByID(work->eventData, warpId);
    if (IsWarpZoneOrWarpID0xFFFF(warp) == 0 && CheckWarpDirectionMatch2(warp, work->faceDir) != 0) {
        func_ov021_0218b2a8(work, warpId, (FieldmapEventWork *)(&rail.componentId));
        return func_ov021_0218b2e8(work, field, warpId, &rail);
    }
    return NULL;
}



GameEvent *func_ov021_0218b208(FieldInteractWk *work, GameSystem *gsys, Field *field) {
    EncountSystem *encount = Field_GetEncountSystem(field);
    if (work->walking == 0) {
        return NULL;
    }
    if (func_02013dd0(work->zoneId)) {
        FieldG3DMapper *mapper = Field_GetG3DMapper(field);
        return func_ov021_021aa248(encount, func_ov021_0218dc70(mapper));
    }
    return EventWildBattleCall_CreateRandom(encount, 0);
}

GameEvent *func_ov021_0218b24c(FieldInteractWk *work, GameSystem *gsys, Field *field) {
    FieldPlayer *player = Field_GetPlayer(field);
    FieldActor *actor = FieldPlayer_GetActor(player);
    u16 dir = FieldPlayer_GetMoveDirByKey(player, work->held);
    if (dir == 1) {
        if (work->observeStatus == 0 || work->observeStatus == 3) {
            if (func_ov021_021bb188(GetTileClass(work->tileType)) && (func_ov021_0219e418(actor, dir) & 3)) {
                return func_ov021_021c46f8(gsys, field, dir);
            }
        }
    }
    return NULL;
}



void func_ov021_0218b2a8(FieldInteractWk *work, u32 warpId, FieldmapEventWork *warpInfo) {
    ZoneSpawnInfo spawn;
    u16 zoneId = Field_GetPlayerStateZoneID(work->field);
    SetupZoneWarpArrivalRail(&spawn, zoneId, warpId, 0, 0, warpInfo->unk_00, warpInfo->unk_06, (s16)warpInfo->unk_04);
    SetGameDataOutboundWarpRememberInfo(work->gameData, &spawn);
}

GameEvent *func_ov021_0218b2e8(FieldInteractWk *work, Field *field, u32 warpId, RailPosition *position) {
    ZoneWarp *warp = GetZoneWarpByID(work->eventData, warpId);
    u16 dir = func_ov010_021626b0(warp, position);
    return func_ov020_02185360(work->gsys, field, warp, dir);
}



BOOL func_ov021_0218b310(FieldPlayer *player, FieldInteractWk *args) {
    if (func_ov021_0219e418(FieldPlayer_GetActor(player), args->faceDir) & 3) {
        return FALSE;
    }
    return TRUE;
}

GameEventReturnCode EventSurfExit_Callback(GameEvent *event, u32 *state, void *data) {
    SurfExitWork *work = data;
    FieldPlayer *player = Field_GetPlayer(work->field);
    FieldActor *actor;
    FieldEffectTask *task;
    u16 acmd;
    FieldPlayer_GetExState(player);
    actor = FieldPlayer_GetActor(player);
    switch (*state) {
    case 0:
        DisableAllActorsMovement(GetActorMModelSystem(actor));
        CheckSetActorFaceDir(actor, work->dir);
        (*state)++;
        break;
    case 1:
        task = func_ov021_021a2f98(player);
        func_ov021_021acbb8(task, 0);
        FieldPlayer_SetSpecialSeq(player, 1);
        func_ov021_021a2f60(player);
        acmd = GetAcmdForDir(work->dir, 0x34);
        if (work->overEdge == 1) {
            acmd = GetAcmdForDir(work->dir, 0x38);
        }
        FldAct_SetAcmd(actor, acmd);
        (*state)++;
        break;
    case 2:
        if (func_ov010_0216d454(actor) != 1) {
            break;
        }
        func_ov021_021a2fd0(player);
        (*state)++;
        break;
    case 3:
        EnableAllActorsMovement(GetActorMModelSystem(actor));
        func_ov021_021a35a0(player, 1);
        return 1;
    }
    return 0;
}


GameEvent *func_ov021_0218b3dc(FieldInteractWk *param_1, GameSystem *gsys, Field *param_3, u16 param_4, u32 param_5) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventSurfExit_Callback, sizeof(SurfExitWork));
    SurfExitWork *work = GameEvent_GetData(event);
    work->gsys = gsys;
    work->field = param_3;
    work->dir = param_4;
    work->overEdge = param_5;
    return event;
}

GameEvent *func_ov021_0218b408(FieldInteractWk *work, GameSystem *gsys, Field *field) {
    FieldPlayer *player;
    FieldG3DMapper *mapper;
    VecFx32 pos;
    u16 dir;
    u32 isEdge;
    u32 tileType;
    u32 tileClass;
    u32 flags;
    player = Field_GetPlayer(field);
    if (FieldPlayer_GetExState(player) == 2) {
        mapper = Field_GetG3DMapper(field);
        dir = FieldPlayer_GetMoveDirByKey(work->player, work->held);
        if (dir != 9) {
            FieldPlayer_GetWPosInDir(player, dir, &pos);
            tileType = GetTileTypeAtPos(mapper, &pos);
            flags = GetTileFlags(tileType);
            tileClass = GetTileClass(tileType);
            isEdge = 0;
            if (MapTile_IsSurfEdge(tileClass) == 1) {
                ExpandVecInGridDir(dir, &pos, 0x10000);
                tileType = GetTileTypeAtPos(mapper, &pos);
                flags = GetTileFlags(tileType);
                isEdge = 1;
            }
            if (func_ov021_0218b6f8(work, field) == 1 && (flags & 1) == 0 && (flags & 2) == 0) {
                return func_ov021_0218b3dc(work, gsys, field, dir, isEdge);
            }
        }
    }
    return NULL;
}





FieldActor *FindActorForInteraction(FieldInteractWk *param_1, Field *param_2) {
    FieldActor *actor;
    s16 sStack_26;
    s16 sStack_28;
    VecFx32 VStack_20;
    FieldG3DMapper *mapper;
    u32 tileType;
    u16 tileClass;
    BOOL BVar2;
    MMSys *system;
    fx32 y;
    s16 asStack_24 [2];


    actor = FieldPlayer_GetActor(param_1->player);
    FieldPlayer_GetGPosInFront(param_1->player, asStack_24, &sStack_26, &sStack_28);
    VecGPosToWPos((s32)asStack_24[0], (s32)sStack_26, (s32)sStack_28, &VStack_20);
    mapper = Field_GetG3DMapper(param_2);
    tileType = GetTileTypeAtPos(mapper, &VStack_20);
    tileClass = GetTileClass(tileType);
    BVar2 = MapTile_IsReceptionCounter((u32)tileClass);
    if (BVar2 == 1) {
        tileClass = GetActorFaceDir(actor);
        AdjusGridXZByDir((u32)tileClass, asStack_24, &sStack_28, 1);
    }
    system = Field_GetActorSystem(param_2);
    y = GetActorPosY(actor);
    return FindActorByGPos(system, asStack_24[0], sStack_28, y, 0x10000, 0);
}

FieldActor *func_ov021_0218b53c(FieldInteractWk *work, Field *field) {
    RailPosition pos;
    FieldActor *actor = FieldPlayer_GetActor(work->player);
    if (func_ov021_0219e634(actor, &pos)) {
        return FindActorByRailPos(Field_GetActorSystem(field), &pos, FALSE);
    }
    return NULL;
}

u16 func_ov021_0218b56c(FieldInteractWk *work, Field *field) {
    u32 tileUnder;
    u32 tileInDir;
    u16 tileClass;
    u32 i;
    if (GameData_IsForceSeasonSync(work->gameData) == 1) {
        return 0xffff;
    }
    tileUnder = FieldPlayer_GetTileTypeUnder(work->player);
    tileInDir = FieldPlayer_GetTileTypeInDir(work->player, work->faceDir);
    tileClass = GetTileClass(tileInDir);
    for (i = 0; i < 13; i++) {
        if ((work->faceDir == data_ov021_021d3ce8[i].key || data_ov021_021d3ce8[i].key == 9) && data_ov021_021d3ce8[i].isTile(tileClass)) {
            return data_ov021_021d3ce8[i].result;
        }
    }
    if (func_02013e04(work->zoneId) == 0 || GameData_IsForceSeasonSync(work->gameData) == 1) {
        return 0xffff;
    }
    if (CheckSurfHeightAllow(work->player, tileUnder, tileInDir) != 0 && func_ov021_0218b630(work->gameData, 0x39) != 6) {
        return 0x2712;
    }
    if (CheckCanInteractWaterfall(work->player, tileUnder, tileInDir) != 0) {
        return 0x2716;
    }
    return 0xffff;
}


u32 func_ov021_0218b630(GameData *gameData, u32 param_2) {
    s32 count;
    PokeParty *party = GameData_GetParty(gameData);
    s32 i;
    count = PokeParty_GetPkmCount(party);
    for (i = 0; i < count; i++) {
        PartyPkm *pkm = PokeParty_GetPkm(party, i);
        if (PokeParty_GetParam(pkm, 0x4c, NULL) == 0) {
            if (param_2 == PokeParty_GetParam(pkm, 0x36, NULL) || param_2 == PokeParty_GetParam(pkm, 0x37, NULL) || param_2 == PokeParty_GetParam(pkm, 0x38, NULL) || param_2 == PokeParty_GetParam(pkm, 0x39, NULL)) {
                return i;
            }
        }
    }
    return 6;
}



s32 func_ov021_0218b6ac(GameSystem *param_1) {
    GameData *gameData;
    PokeParty *party;
    s32 pkmCount;
    PartyPkm *pkm;
    u32 result;
    u32 index;
    s32 i;

    gameData = GSYS_GetGameData(param_1);
    party = GameData_GetParty(gameData);
    pkmCount = PokeParty_GetPkmCount(party);
    index = 0;
    i = 0;
    if (0 < pkmCount) {
        do {
            pkm = PokeParty_GetPkm(party, index);
            result = PokeParty_GetParam(pkm, 0x4c, (void *)0x0);
            if ((result == 0) && (result = PokeParty_GetParam(pkm, 0xa0, (void *)0x0), result != 0)) {
                i = i + 1;
            }
            index = index + 1;
        } while ((s32)index < pkmCount);
    }
    return i;
}

BOOL func_ov021_0218b6f8(FieldInteractWk *work, Field *field) {
    FieldActor *actor = FieldPlayer_GetActor(work->player);
    u32 dir = work->moveDir;
    s16 x = GetGPosX(actor);
    s16 z = GetGPosZ(actor);
    s32 i;
    for (i = 0; i < 2; i++) {
        x += GetDirectionVectorCompX(dir);
        z += GetDirectionVectorCompZ(dir);
        if (GetFirstActorOnGPos(Field_GetActorSystem(field), x, z, TRUE) != NULL) {
            return FALSE;
        }
    }
    return TRUE;
}

