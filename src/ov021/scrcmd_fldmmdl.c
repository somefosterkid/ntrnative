// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/scrcmd_fldmmdl.c, ported by tools/port_file.py.
// The script commands of field actors: action commands and walked routes, adding, removing and placing actors, their
// positions, directions and rail positions, and pausing the actors that a script doesn't move. The name is the ROM's
// own, from GFL_HeapAllocate's file argument. Function names from swan (https://github.com/ds-pokemon-hacking/swan,
// GPL-3.0)
#include "types.h"
#include "field/event_action_call.h"
#include "field/event_actor_move.h"
#include "field/event_dive.h"
#include "field/field.h"
#include "field/field_acmd.h"
#include "field/field_actor.h"
#include "field/field_g3d_mapper.h"
#include "field/field_map.h"
#include "field/field_player.h"
#include "field/field_rail.h"
#include "field/field_script.h"
#include "field/field_task.h"
#include "field/player_state.h"
#include "field/scrcmd_fldmmdl.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_vec.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/vm.h"

static u8 data_ov021_021ee71c;

// The world position of a tile's corner, and of its center
#define GRID_TO_FX32(n) ((n) * FX32_CONST(16))
#define GRID_CENTER(n) ((n) * FX32_CONST(16) + FX32_CONST(8))
// The tile of a world position
#define FX32_TO_GRID(x) (((x) >> FX32_SHIFT) / 16)
#define HEIGHT_TO_GRID(height) ((s16)(((height) >> 4) / FX32_ONE))

// The actor IDs of the follower and of the script's own actor
#define SCRIPT_ACTOR_FOLLOWER 0xf2
#define SCRIPT_ACTOR_SELF 0xf1
// The move code of the Pokémon that follows the player
#define MOVE_CODE_FOLLOWER 0x30

// The actors that DisableAllButEventActorsMovement left moving, which WaitAndDisableEventMovement stops once they
// finish
#define EVENT_ACTOR_PLAYER (1 << 0)
#define EVENT_ACTOR_FOLLOWER (1 << 1)
#define EVENT_ACTOR_PARENT (1 << 2)
#define EVENT_ACTOR_PAIR (1 << 3)

// An action command as the walked route writes it
// Black 2's, not checked against Black
typedef struct {
    u16 code;
    u16 count;
} WalkRouteAcmd;

// The route that s024F_ActorWalkRoute has an actor walk: to the tile (x, z), first along x if axis is 0
// Black 2's, not checked against Black
typedef struct {
    FieldScriptEnv *env;
    Field *field;
    u16 x;
    u16 z;
    // Bit 0 walks through actors, bit 1 through blocked tiles, bit 2 checks only the map's bounds
    u16 flags;
    // Frames per tile
    u16 speed;
    u16 axis;
} WalkRoute;

static FieldActor *FindScrEnvPlayerActor(FieldScriptEnv *env);

static const u32 ACMD_QUEUE_TURN_N_8F[2] = { ACMD(0x20, 1), ACMD_END };
static const u32 ACMD_QUEUE_TURN_S_8F[2] = { ACMD(0x21, 1), ACMD_END };
static const u32 ACMD_QUEUE_TURN_W_8F[2] = { ACMD(0x22, 1), ACMD_END };
static const u32 ACMD_QUEUE_TURN_E_8F[2] = { ACMD(0x23, 1), ACMD_END };

static FieldActor *func_ov021_021b1668(FieldScriptEnv *env, u16 id) {
    FieldActor *actor;
    MMSys *mmSys = GetScrEnvMMdlSys(env);

    if (id == SCRIPT_ACTOR_FOLLOWER) {
        actor = FindActorByMoveCode(mmSys, MOVE_CODE_FOLLOWER);
    } else if (id == SCRIPT_ACTOR_SELF) {
        ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
        // BUG: The script's own actor is never looked up, so the actor returned is garbage
#ifdef BUGFIX
        actor = ScriptWork_GetParentActor(work);
#endif
    } else {
        actor = FindFieldActor(mmSys, id);
    }
    return actor;
}

u32 func_ov021_021b1698(VM *param_1, FieldScriptEnv *param_2) {
    u16 id;
    s32 result;
    FieldActor *actor;
    FieldAcmdTCB *task;

    FieldScriptEnv_GetScriptWork(param_2);
    id = ScriptReadAny(param_1, param_2);
    result = VM_Read32(param_1);
    actor = func_ov021_021b1668(param_2, id);
    if (actor == (FieldActor *)0x0) {
        return 0;
    }
    task = FieldAcmdTCB_Create(actor, (u32 *)(param_1->pc + result));
    FieldScriptEnv_AddAcmdTask(param_2, task);
    return 0;
}

static BOOL ScriptNative_AcmdWait(VM *vm, void *data) {
    if (!FieldScriptEnv_CheckAcmdQueueRunning(data)) {
        return TRUE;
    }
    return FALSE;
}

BOOL s0065_ActorCmdWait(VM *vm, FieldScriptEnv *env) {
    VM_SetNativeCallback(vm, ScriptNative_AcmdWait);
    return TRUE;
}

BOOL s0066_ActorGetMoveCode(VM *vm, FieldScriptEnv *env) {
    MMSys *mmSys = GetScrEnvMMdlSys(env);
    u16 *result = ScriptReadVar(vm, env);

    *result = 0;
    *result = GetActorMoveCode(FindFieldActor(mmSys, ScriptReadAny(vm, env)));
    return FALSE;
}

BOOL s0073_ActorSetMoveCode(VM *vm, FieldScriptEnv *env) {
    MMSys *mmSys = GetScrEnvMMdlSys(env);
    u16 id = ScriptReadAny(vm, env);
    u16 moveCode = ScriptReadAny(vm, env);

    ChangeActorMoveCodeSeq(FindFieldActor(mmSys, id), moveCode);
    return FALSE;
}

BOOL s0067_ActorGetGPos(VM *vm, FieldScriptEnv *env) {
    MMSys *mmSys = GetScrEnvMMdlSys(env);
    FieldActor *actor = FindFieldActor(mmSys, ScriptReadAny(vm, env));
    u16 *x = ScriptReadVar(vm, env);
    u16 *z = ScriptReadVar(vm, env);

    *x = GetGPosX(actor);
    *z = GetGPosZ(actor);
    return FALSE;
}

BOOL s0068_PlayerGetGPos(VM *vm, FieldScriptEnv *env) {
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(FieldScriptEnv_GetScriptWork(env));
    FieldActor *actor = FieldPlayer_GetActor(Field_GetPlayer(fieldWork->field));
    u16 *x = ScriptReadVar(vm, env);
    u16 *z = ScriptReadVar(vm, env);

    *x = GetGPosX(actor);
    *z = GetGPosZ(actor);
    return FALSE;
}

BOOL s006E_PlayerGetDir(VM *vm, FieldScriptEnv *env) {
    FieldActor *actor = FindScrEnvPlayerActor(env);
    u16 *result = ScriptReadVar(vm, env);

    *result = GetActorFaceDir(actor);
    return FALSE;
}

BOOL s0069_ActorNew(VM *vm, FieldScriptEnv *env) {
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(FieldScriptEnv_GetScriptWork(env));
    u16 x = ScriptReadAny(vm, env);
    u16 z = ScriptReadAny(vm, env);
    u16 dir = ScriptReadAny(vm, env);
    u16 id = ScriptReadAny(vm, env);
    u16 objCode = ScriptReadAny(vm, env);
    u16 moveCode = ScriptReadAny(vm, env);
    u16 zoneId = Field_GetPlayerStateZoneID(fieldWork->field);
    MMSys *mmSys = GetScrEnvMMdlSys(env);
    FieldActor *actor = CreateNewActorByParam(
        mmSys, x, z, dir, id, ResolvePossibleWKOBJCODE(GameData_GetEventWork(FieldScriptEnv_GetGameData(env)), objCode),
        moveCode, zoneId);

    if (actor != NULL) {
        DisableActorMovement(actor);
    }
    return FALSE;
}

BOOL s006C_ActorDelete(VM *vm, FieldScriptEnv *env) {
    MMSys *mmSys = GetScrEnvMMdlSys(env);
    FieldActor *actor = FindFieldActor(mmSys, ScriptReadAny(vm, env));

    if (actor != NULL) {
        DeleteActor(actor);
    }
    return FALSE;
}

BOOL s006B_ActorAdd(VM *vm, FieldScriptEnv *env) {
    MMSys *mmSys = GetScrEnvMMdlSys(env);
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    EventData *eventData = GameData_GetEventData(gameData);
    u32 count = GetZoneNPCsCount(eventData);
    u16 zoneId = PlayerState_GetZoneID(GameData_GetPlayerState(gameData));
    u16 uid = ScriptReadAny(vm, env);

    if (count != 0) {
        EventWork *eventWork = GameData_GetEventWork(gameData);
        FieldActor *actor = func_ov010_0216cee4(mmSys, GetZoneNPCs(eventData), zoneId, count, eventWork, uid);

        if (actor != NULL) {
            DisableActorMovement(actor);
        }
    }
    return FALSE;
}

BOOL s006D_ActorSetGPos(VM *vm, FieldScriptEnv *env) {
    u16 id = ScriptReadAny(vm, env);
    s16 x = ScriptReadAny(vm, env);
    s16 y = ScriptReadAny(vm, env);
    s16 z = ScriptReadAny(vm, env);
    u16 dir = ScriptReadAny(vm, env);
    FieldActor *actor = FindFieldActor(GetScrEnvMMdlSys(env), id);

    if (actor != NULL) {
        SetActorGPos(actor, x, y, z, dir);
    }
    return FALSE;
}

BOOL s006A_ActorGetSpawnFlag(VM *vm, FieldScriptEnv *env) {
    u16 id = ScriptReadAny(vm, env);
    u16 *result = ScriptReadVar(vm, env);
    FieldActor *actor = FindFieldActor(GetScrEnvMMdlSys(env), id);

    if (actor != NULL) {
        *result = GetActorSpawnFlag(actor);
    }
    return FALSE;
}

BOOL s0079_ActorGetUserParam(VM *vm, FieldScriptEnv *env) {
    u16 id = ScriptReadAny(vm, env);
    u16 index = ScriptReadAny(vm, env);
    u16 *result = ScriptReadVar(vm, env);
    FieldActor *actor = FindFieldActor(GetScrEnvMMdlSys(env), id);

    if (index <= 2 && actor != NULL) {
        *result = GetActorUserParam(actor, index);
    }
    return FALSE;
}

BOOL func_ov021_021b1a20(VM *vm, FieldScriptEnv *env) {
    u16 id = ScriptReadAny(vm, env);
    FieldActor *actor = FindFieldActor(GetScrEnvMMdlSys(env), id);

    if (actor != NULL) {
        SetActorFlag32(actor, TRUE);
    }
    return FALSE;
}

// Stops each actor that DisableAllButEventActorsMovement left moving once it has finished its move
static BOOL WaitAndDisableEventMovement(VM *vm, void *data) {
    FieldScriptEnv *env = data;
    FieldActor *parent = ScriptWork_GetParentActor(FieldScriptEnv_GetScriptWork(env));
    FieldActor *actor = FindScrEnvPlayerActor(env);
    MMSys *mmSys = GetScrEnvMMdlSys(env);

    if ((data_ov021_021ee71c & EVENT_ACTOR_PLAYER) && func_ov010_0216d454(actor) == TRUE) {
        DisableActorMovement(actor);
        data_ov021_021ee71c &= ~EVENT_ACTOR_PLAYER;
    }
    if ((data_ov021_021ee71c & EVENT_ACTOR_PARENT) && IsActorFlag16(parent) == FALSE) {
        DisableActorMovement(parent);
        data_ov021_021ee71c &= ~EVENT_ACTOR_PARENT;
    }
    if (data_ov021_021ee71c & EVENT_ACTOR_FOLLOWER) {
        actor = FindActorByMoveCode(mmSys, MOVE_CODE_FOLLOWER);
        if (IsActorFlag16(actor) == FALSE) {
            DisableActorMovement(actor);
            data_ov021_021ee71c &= ~EVENT_ACTOR_FOLLOWER;
        }
    }
    if (data_ov021_021ee71c & EVENT_ACTOR_PAIR) {
        parent = FindPairedTrainerActor(parent);
        if (parent == NULL) {
            data_ov021_021ee71c &= ~EVENT_ACTOR_PAIR;
        } else if (IsActorFlag16(parent) == FALSE) {
            DisableActorMovement(parent);
            data_ov021_021ee71c &= ~EVENT_ACTOR_PAIR;
        }
    }
    if (data_ov021_021ee71c == 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov021_021b1b08(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work;
    FieldActor *parentActor;
    FieldActor *playerActor;
    FieldActor *pairedTrainer;
    MMSys *mmdlSys;

    work = FieldScriptEnv_GetScriptWork(env);
    mmdlSys = GetScrEnvMMdlSys(env);
    parentActor = ScriptWork_GetParentActor(work);
    playerActor = FindScrEnvPlayerActor(env);
    FindActorByMoveCode(mmdlSys, MOVE_CODE_FOLLOWER);
    pairedTrainer = FindPairedTrainerActor(parentActor);
    data_ov021_021ee71c = 0;
    DisableAllActorsMovement(mmdlSys);
    if (func_ov010_0216d454(playerActor) == 0) {
        data_ov021_021ee71c |= EVENT_ACTOR_PLAYER;
        EnableActorMovement(playerActor);
    }
    if (parentActor != NULL && IsActorFlag16(parentActor) == TRUE) {
        data_ov021_021ee71c |= EVENT_ACTOR_PARENT;
        EnableActorMovement(parentActor);
    }
    if (pairedTrainer != NULL && IsActorFlag16(pairedTrainer) == TRUE) {
        data_ov021_021ee71c |= EVENT_ACTOR_PAIR;
        EnableActorMovement(pairedTrainer);
    }
    VM_SetNativeCallback(vm, WaitAndDisableEventMovement);
    return TRUE;
}

BOOL func_ov021_021b1bb8(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    FieldActor *parent = ScriptWork_GetParentActor(work);
    if (parent == NULL) {
        DisableAllActorsMovement(GetScrEnvMMdlSys(env));
    } else {
        return func_ov021_021b1b08(vm, env);
    }
    return FALSE;
}



void EnableAllActorsMovementScr(FieldScriptEnv *env) {
    EnableAllActorsMovement(GetScrEnvMMdlSys(env));
}

BOOL s0074_ActorSetEyeToEye(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    u16 dir = GetInverseDirection(GetActorFaceDir(FindScrEnvPlayerActor(env)));
    FieldActor *actor = ScriptWork_GetParentActor(work);

    if (actor != NULL) {
        CheckSetActorFaceDir(actor, dir);
        func_ov010_0216d674(actor, 0);
    }
    return FALSE;
}

BOOL s0070_ActorFindByGPos(VM *vm, FieldScriptEnv *env) {
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(FieldScriptEnv_GetScriptWork(env));
    Field *field = fieldWork->field;
    u16 *uid = ScriptReadVar(vm, env);
    u16 *found = ScriptReadVar(vm, env);
    u16 x = ScriptReadAny(vm, env);
    u16 y = ScriptReadAny(vm, env);
    u16 z = ScriptReadAny(vm, env);
    FieldActor *actor = FindActorByGPos(Field_GetActorSystem(field), x, z, GRID_TO_FX32(y), FX32_CONST(8), FALSE);

    if (actor != NULL) {
        *found = TRUE;
        *uid = GetActorUID(actor);
    } else {
        *found = FALSE;
    }
    return FALSE;
}

BOOL s006F_PlayerGetActorInFront(VM *vm, FieldScriptEnv *env) {
    FieldPlayer *player = Field_GetPlayer(GSYS_GetField(FieldScriptEnv_GetGameSystem(env)));
    u16 *uid = ScriptReadVar(vm, env);
    u16 *found = ScriptReadVar(vm, env);
    FieldActor *actor = FieldPlayer_GetActorInFrontEx(player, FX32_CONST(8));

    if (actor != NULL) {
        *found = TRUE;
        *uid = GetActorUID(actor);
    } else {
        *found = FALSE;
    }
    return FALSE;
}

static BOOL func_ov021_021b1cfc(VM *vm, void *data) {
    FieldScriptEnv *env = data;
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(FieldScriptEnv_GetScriptWork(env));

    if (func_ov021_021a2f54(Field_GetPlayer(fieldWork->field)) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

BOOL s0075_PlayerSetSpecialSequence(VM *vm, FieldScriptEnv *env) {
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(FieldScriptEnv_GetScriptWork(env));
    u16 seq = VM_Read16(vm);
    FieldPlayer *player = Field_GetPlayer(fieldWork->field);

    FieldPlayer_SetSpecialSeq(player, seq);
    if (func_ov021_021a2f54(player) == TRUE) {
        return FALSE;
    }
    VM_SetNativeCallback(vm, func_ov021_021b1cfc);
    return TRUE;
}

BOOL s0076_PlayerMoveToYAsync(VM *vm, FieldScriptEnv *env) {
    VecFx32 pos;
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(FieldScriptEnv_GetScriptWork(env));
    Field *field = fieldWork->field;
    FieldTaskManager *taskManager = Field_GetTaskManager(field);
    u16 negate = ScriptReadAny(vm, env);
    u16 frames = ScriptReadAny(vm, env);
    u16 y = ScriptReadAny(vm, env);
    u16 up = ScriptReadAny(vm, env);
    s32 duration = frames;

    if (negate) {
        duration = -frames;
    }
    if (up == 0) {
        VEC_Set(&pos, 0, -y * FX32_ONE, 0);
    } else {
        VEC_Set(&pos, 0, y * FX32_ONE, 0);
    }
    FieldTaskManager_AddTask(
        taskManager, FieldActorMoveTask_CreateAbs(field, duration, &pos, FieldPlayer_GetActor(Field_GetPlayer(field))),
        0);
    return FALSE;
}

BOOL s0248_PlayerMoveToYAsync_(VM *vm, FieldScriptEnv *env) {
    VecFx32 pos;
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(FieldScriptEnv_GetScriptWork(env));
    Field *field = fieldWork->field;
    FieldTaskManager *taskManager = Field_GetTaskManager(field);
    u16 negate = ScriptReadAny(vm, env);
    u16 frames = ScriptReadAny(vm, env);
    u16 y = ScriptReadAny(vm, env);
    u16 up = ScriptReadAny(vm, env);
    s32 duration = frames;

    if (negate) {
        duration = -frames;
    }
    if (up == 0) {
        VEC_Set(&pos, 0, -y * FX32_ONE, 0);
    } else {
        VEC_Set(&pos, 0, y * FX32_ONE, 0);
    }
    FieldTaskManager_AddTask(
        taskManager, FieldActorMoveTask_CreateAbs(field, duration, &pos, FieldPlayer_GetActor(Field_GetPlayer(field))),
        0);
    return FALSE;
}

static FieldActor *FindScrEnvPlayerActor(FieldScriptEnv *env) {
    return FindPlayerFieldActor(GetScrEnvMMdlSys(env));
}

// Turns the player to face the trigger the player stands on
BOOL s0077_PlayerTurnByTrigger(VM *vm, FieldScriptEnv *env) {
    int i;
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(work);
    Field *field = fieldWork->field;
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    u16 dirs[4] = { 0, 1, 2, 3 };
    EventData *eventData = GameData_GetEventData(gameData);
    EventWork *eventWork = GameData_GetEventWork(gameData);
    FieldPlayer *player = Field_GetPlayer(field);
    VecFx32 pos;
    u32 dir;
    u32 trigDir;
    u32 faceDir;
    const u32 *action;

    FieldPlayer_GetWPos(player, &pos);
    for (i = 0; i < 4; i++) {
        trigDir = dirs[i];
        if (FindTriggerAtPosGrid(eventData, eventWork, &pos, trigDir) != NULL) {
            dir = trigDir;
            break;
        }
    }
    faceDir = FieldPlayer_GetFaceDir(player);
    if (faceDir != dir) {
        if (dir == 0) {
            action = ACMD_QUEUE_TURN_N_8F;
        } else if (dir == 1) {
            action = ACMD_QUEUE_TURN_S_8F;
        } else if (dir == 2) {
            action = ACMD_QUEUE_TURN_W_8F;
        } else if (dir == 3) {
            action = ACMD_QUEUE_TURN_E_8F;
        } else {
            return FALSE;
        }
        ScriptWork_CallEvent(work, EventActionCall_Create(gsys, field, 0xff, action));
        return TRUE;
    }
    return FALSE;
}

BOOL s0078_PlayerGetExState(VM *vm, FieldScriptEnv *env) {
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(FieldScriptEnv_GetScriptWork(env));
    FieldPlayer *player = Field_GetPlayer(fieldWork->field);
    u16 *result = ScriptReadVar(vm, env);

    switch (FieldPlayer_GetExState(player)) {
    case 0:
        *result = 0;
        break;
    case 1:
        *result = 1;
        break;
    case 2:
        *result = 2;
        break;
    case 3:
        *result = 3;
        break;
    default:
        *result = 0;
        break;
    }
    return FALSE;
}

BOOL s007B_ActorJumpToGPos(VM *vm, FieldScriptEnv *env) {
    VecFx32 start;
    VecFx32 end;
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    u16 id = ScriptReadAny(vm, env);
    u16 x = ScriptReadAny(vm, env);
    u16 y = ScriptReadAny(vm, env);
    u16 z = ScriptReadAny(vm, env);
    MMSys *mmSys = GetScrEnvMMdlSys(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    FieldActor *actor = FindFieldActor(mmSys, id);

    CopyActorWPos(actor, &start);
    end.x = GRID_CENTER(x);
    end.y = GRID_TO_FX32(y);
    end.z = GRID_CENTER(z);
    ScriptWork_CallEvent(work, EventActorJump_Create(gsys, actor, &start, &end));
    return TRUE;
}

BOOL func_ov021_021b2040(VM *vm, FieldScriptEnv *env) {
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(FieldScriptEnv_GetScriptWork(env));
    FieldPlayer *player = Field_GetPlayer(fieldWork->field);
    u16 *result = ScriptReadVar(vm, env);

    *result = func_ov021_021a3238(player);
    return FALSE;
}

BOOL s0071_PlayerGetRailPos(VM *vm, FieldScriptEnv *env) {
    FieldActor *actor = FindScrEnvPlayerActor(env);
    u16 *a = ScriptReadVar(vm, env);
    u16 *b = ScriptReadVar(vm, env);
    u16 *c = ScriptReadVar(vm, env);

    SetWkToActorRailPos(FldAct_GetRailUnit(actor), a, b, c);
    return FALSE;
}

BOOL s0072_ActorGetRailPos(VM *vm, FieldScriptEnv *env) {
    MMSys *mmSys = GetScrEnvMMdlSys(env);
    FieldActor *actor = FindFieldActor(mmSys, ScriptReadAny(vm, env));
    u16 *a = ScriptReadVar(vm, env);
    u16 *b = ScriptReadVar(vm, env);
    u16 *c = ScriptReadVar(vm, env);

    SetWkToActorRailPos(FldAct_GetRailUnit(actor), a, b, c);
    return FALSE;
}

BOOL s007A_ActorPlayRailSlipdown(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    MMSys *mmSys = GetScrEnvMMdlSys(env);
    FieldActor *actor = FindFieldActor(mmSys, ScriptReadAny(vm, env));
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(work);

    ScriptWork_CallEvent(work, func_ov021_021c474c(gsys, fieldWork->field, actor));
    return TRUE;
}

BOOL s007C_PlayerSetRailPos(VM *vm, FieldScriptEnv *env) {
    RailPosition railPos;
    FieldActor *actor = FindScrEnvPlayerActor(env);
    u16 a = ScriptReadAny(vm, env);
    u16 b = ScriptReadAny(vm, env);
    u16 c = ScriptReadAny(vm, env);
    RailUnit *unit = FldAct_GetRailUnit(actor);

    func_ov021_021b8484(unit, a, b, c);
    func_ov021_021b7dc0(unit, &railPos);
    SetActorInitedPositionRail(actor, &railPos);
    return FALSE;
}

BOOL s007D_ActorSetRailPos(VM *vm, FieldScriptEnv *env) {
    RailPosition railPos;
    MMSys *mmSys = GetScrEnvMMdlSys(env);
    FieldActor *actor = FindFieldActor(mmSys, ScriptReadAny(vm, env));
    u16 a = ScriptReadAny(vm, env);
    u16 b = ScriptReadAny(vm, env);
    u16 c = ScriptReadAny(vm, env);
    RailUnit *unit = FldAct_GetRailUnit(actor);

    func_ov021_021b8484(unit, a, b, c);
    func_ov021_021b7dc0(unit, &railPos);
    SetActorInitedPositionRail(actor, &railPos);
    return FALSE;
}

BOOL s007E_ActorPlayTeleportSeq(VM *vm, FieldScriptEnv *env) {
    VecFx32 offset;
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    u16 id = ScriptReadAny(vm, env);
    FieldActor *actor = FindFieldActor(GetScrEnvMMdlSys(env), id);
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(work);
    Field *field = fieldWork->field;
    FieldTaskManager *taskManager = Field_GetTaskManager(field);
    FieldTask *moveTask;

    GFL_SndSEPlay(0x676);
    offset.x = 0;
    offset.y = FX32_CONST(150);
    offset.z = 0;
    moveTask = FieldActorMoveTask_CreateRel(field, 24, &offset, actor);
    FieldTaskManager_AddTask(taskManager, FieldActorSpinTask_Create(field, 24, 3, actor), 0);
    FieldTaskManager_AddTask(taskManager, moveTask, 0);
    return FALSE;
}

BOOL s0234_ActorFallDownToXZ(VM *vm, FieldScriptEnv *env) {
    VecFx32 pos;
    fx32 height;
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    u16 id = ScriptReadAny(vm, env);
    u16 x = ScriptReadAny(vm, env);
    u16 z = ScriptReadAny(vm, env);
    u16 dir = ScriptReadAny(vm, env);
    FieldActor *actor = FindFieldActor(GetScrEnvMMdlSys(env), id);
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(work);
    Field *field = fieldWork->field;
    FieldTaskManager *taskManager = Field_GetTaskManager(field);

    if (actor == NULL) {
        return FALSE;
    }
    height = 0;
    pos.x = GRID_CENTER(x);
    pos.y = FX32_CONST(250);
    pos.z = GRID_CENTER(z);
    GetHeightFromMap(actor, &pos, &height);
    pos.y = height;
    SetActorGPos(actor, x, HEIGHT_TO_GRID(pos.y), z, dir);
    SetActorWPosValue(actor, &pos);
    {
        VecFx32 offset = { 0, FX32_CONST(250), 0 };

        SetActorWPosOffset(actor, &offset);
    }
    FieldTaskManager_AddTask(taskManager, FieldActorFallTask_Create(field, actor, 40, 250), 0);
    return FALSE;
}
