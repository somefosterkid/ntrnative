// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/field_player.c, ported by tools/port_file.py.
#include "types.h"
#include "constants/flags.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_g3d_mapper.h"
#include "field/field_map.h"
#include "field/field_nogrid_mapper.h"
#include "field/field_player.h"
#include "field/field_player_core.h"
#include "field/field_player_grid.h"
#include "field/field_player_nogrid.h"
#include "field/field_rail.h"
#include "field/player_state.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_vec.h"
#include "save/event_work.h"
#include "system/game_data.h"
#include "system/game_system.h"

u32 FieldPlayer_GetTileTypeInDirEx(FieldPlayer *player, u16 direction, fx32 *height);

// The controller types that Field_GetResolvedControllerTypeID returns
#define CONTROLLER_GRID 0

#define PLAYER_FORM_COUNT 10

#define GRID_TO_FX32(n) ((n) * FX32_CONST(16))

// How far below the player the water may be for them to start surfing
#define SURF_HEIGHT_MAX FX32_CONST(20)

// Black 2's; twinned code confirms the offsets of objCode in Black
typedef struct {
    u16 objCode;
    u16 exState;
} PlayerSprite;

// The object code and extra state of each form of the player, by sex
static const PlayerSprite PLAYER_SPRITE_MAP[2][PLAYER_FORM_COUNT] = {
    {
        {0x1, FLD_PLAYER_EXSTATE_NONE},
        {0x2, FLD_PLAYER_EXSTATE_CYCLING},
        {0x3, FLD_PLAYER_EXSTATE_SURF},
        {0x3, FLD_PLAYER_EXSTATE_DIVE},
        {0x88, 4},
        {0x8a, 4},
        {0x8c, 4},
        {0x9e, 4},
        {0xa0, 4},
        {0x9c, 4},
    },
    {
        {0x4, FLD_PLAYER_EXSTATE_NONE},
        {0x5, FLD_PLAYER_EXSTATE_CYCLING},
        {0x6, FLD_PLAYER_EXSTATE_SURF},
        {0x6, FLD_PLAYER_EXSTATE_DIVE},
        {0x89, 4},
        {0x8b, 4},
        {0x8d, 4},
        {0x9f, 4},
        {0xa1, 4},
        {0x9d, 4},
    },
};

u32 GetActorRailPosForDir(FieldActor *param_1, u32 param_2, RailPosition *param_3);

FieldPlayer *FieldPlayer_Create(PlayerState *state, Field *field, const VecFx32 *pos, u32 sex, HeapID heapId) {
    FieldPlayer *player = GFL_HeapAllocate(heapId, sizeof(FieldPlayer), TRUE, "field_player.c", 76);

    player->field = field;
    player->core = FieldPlayerCore_Create(state, field, pos, sex, heapId);
    return player;
}

void FieldPlayer_Free(FieldPlayer *player) {
    if (player->grid != NULL) {
        func_ov021_021a41a8(player->grid);
    }
    if (player->nogrid != NULL) {
        func_ov021_021a5920(player->nogrid);
    }
    FieldPlayerCore_Free(player->core);
    GFL_HeapFree(player);
}

void FieldPlayer_SyncState(FieldPlayer *player) {
    VecFx32 pos;
    RailPosition railPos;
    PlayerState *state = FieldPlayerCore_GetPlayerState(player->core);
    FieldActor *actor = FieldPlayerCore_GetActor(player->core);

    CopyActorWPos(actor, &pos);
    PlayerState_SetWPos(state, &pos);
    PlayerState_SetRotationByDir(state, GetActorFaceDir(actor));
    if (Field_GetResolvedControllerTypeID(player->field) == CONTROLLER_GRID) {
        PlayerState_SetIsRail(state, FALSE);
    } else {
        GetActorRailPos(actor, &railPos);
        PlayerState_SetRailPos(state, &railPos);
        PlayerState_SetIsRail(state, TRUE);
    }
}

GameEvent *FieldPlayer_CheckObjContactEvent(FieldPlayer *player, u32 pressedKeys, u32 heldKeys, u32 flags) {
    GameEvent *event = NULL;

    if (Field_GetResolvedControllerTypeID(player->field) == CONTROLLER_GRID) {
        event = FieldPlayerGrid_CheckObjContactEvent(player->grid, pressedKeys, heldKeys, flags);
    }
    return event;
}

void FieldPlayer_SetSpecialSeq(FieldPlayer *player, u32 seq) {
    FieldPlayerCore_SetSpecialSeq(player->core, seq);
}

BOOL func_ov021_021a2f54(FieldPlayer *player) {
    return func_ov021_021a3cc0(player->core);
}

void func_ov021_021a2f60(FieldPlayer *player) {
    func_ov021_021a3ce8(player->core);
}

void FieldPlayer_ForceBrake(FieldPlayer *player) {
    if (Field_GetResolvedControllerTypeID(player->field) == CONTROLLER_GRID) {
        FieldPlayerGrid_ForceBrake(player->grid);
    } else {
        func_ov021_021a5a34(player->nogrid);
    }
}

void func_ov021_021a2f8c(FieldPlayer *player, FieldEffectTask *task) {
    func_ov021_021a3ad0(player->core, task);
}

FieldEffectTask *func_ov021_021a2f98(FieldPlayer *player) {
    return FieldPlayerCore_GetTerrainEffectTCB(player->core);
}

u16 FieldPlayer_GetMoveDirByKey(FieldPlayer *player, u32 heldKeys) {
    return FieldPlayerCore_GetMoveDirByKey(player->core, heldKeys);
}

BOOL func_ov021_021a2fb0(FieldPlayer *player) {
    if (Field_GetResolvedControllerTypeID(player->field) == CONTROLLER_GRID) {
        return func_ov021_021a5778(player->grid);
    }
    return func_ov021_021a59dc(player->nogrid);
}

void func_ov021_021a2fd0(FieldPlayer *player) {
    FieldPlayerCore_SetSpecialSeq(player->core, 1);
    func_ov021_021a3ce8(player->core);
}

void FieldPlayer_UpdateActionStatusObserver(FieldPlayer *player) {
    if (Field_GetResolvedControllerTypeID(player->field) == CONTROLLER_GRID) {
        if (FieldPlayerGrid_IsConstMoving(player->grid) == TRUE) {
            FieldPlayerCore_ForceMoveObserveStatus(player->core, 2);
            return;
        }
        if (FieldPlayerGrid_IsCatwalkExiting(player->grid) == TRUE) {
            FieldPlayerCore_ForceMoveObserveStatus(player->core, 2);
            return;
        }
    } else if (func_ov021_021a59ec(player->nogrid) == TRUE) {
        FieldPlayerCore_ForceMoveObserveStatus(player->core, 2);
        return;
    }
    FieldPlayerCore_UpdateActionStatusObserver(player->core);
}

void FieldPlayer_GetWPos(FieldPlayer *player, VecFx32 *pos) {
    FieldPlayerCore_GetWPos(player->core, pos);
}

void func_ov021_021a38a4(FieldPlayerCore *core, VecFx32 *pos);
void FieldPlayer_SetWPos(FieldPlayer *player, VecFx32 *pos) {
    func_ov021_021a38a4(player->core, pos);
}


u16 FieldPlayer_GetFaceDir(FieldPlayer *player) {
    return FieldPlayerCore_GetFaceDir(player->core);
}

void FieldPlayer_SetDirection(FieldPlayer *player, u16 dir) {
    func_ov021_021a392c(player->core, dir);
}

Field *FieldPlayerCore_GetField(FieldPlayerCore *core);
Field *FieldPlayer_GetField(FieldPlayer *player) {
    return FieldPlayerCore_GetField(player->core);
}



FieldActor *FieldPlayer_GetActor(FieldPlayer *player) {
    return FieldPlayerCore_GetActor(player->core);
}

u32 func_ov021_021a3968(FieldPlayerCore *core);
u32 func_ov021_021a3080(FieldPlayer *player) {
    return func_ov021_021a3968(player->core);
}

u32 func_ov021_021a3970(FieldPlayerCore *core);
u32 func_ov021_021a308c(FieldPlayer *player) {
    return func_ov021_021a3970(player->core);
}



u32 FieldPlayer_GetExState(FieldPlayer *player) {
    return FieldPlayerCore_GetExState(player->core);
}

void FieldPlayerCore_SetExState(FieldPlayerCore *core, u32 state);
void FieldPlayer_SetSpecialState(FieldPlayer *player, u32 state) {
    FieldPlayerCore_SetExState(player->core, state);
}

u32 FieldPlayerCore_GetSex(FieldPlayerCore *core);
u32 func_ov021_021a30b0(FieldPlayer *player) {
    return FieldPlayerCore_GetSex(player->core);
}

GameSystem *FieldPlayerCore_GetGameSystem(FieldPlayerCore *core);
GameSystem *FieldPlayer_GetGameSystem(FieldPlayer *player) {
    return FieldPlayerCore_GetGameSystem(player->core);
}




#define CONTROLLER_NOGRID 1
u32 FieldPlayer_GetTileTypeUnder(FieldPlayer *player) {
    u32 tileType;

    if (Field_GetResolvedControllerTypeID(player->field) == CONTROLLER_GRID) {
        VecFx32 pos;
        FieldG3DMapper *mapper = Field_GetG3DMapper(player->field);
        FieldPlayer_GetWPos(player, &pos);
        tileType = GetTileTypeAtPos(mapper, &pos);
    } else if (Field_GetResolvedControllerTypeID(player->field) == CONTROLLER_NOGRID) {
        RailPosition pos;
        FieldActor *actor = FieldPlayer_GetActor(player);
        NoGridMapper *mapper = Field_GetNoGridMapper(player->field);
        GetActorRailPos(actor, &pos);
        tileType = FieldNoGridMapper_GetTileAtPos(mapper, &pos);
    }
    return tileType;
}

u32 FieldPlayer_GetTileTypeInDir(FieldPlayer *player, u16 direction) {
    VecFx32 pos;
    RailPosition railPos;
    u32 tileType;
    FieldG3DMapper *mapper;
    NoGridMapper *nogrid;
    FieldActor *actor;

    if (Field_GetResolvedControllerTypeID(player->field) == CONTROLLER_GRID) {
        mapper = Field_GetG3DMapper(player->field);
        FieldPlayer_GetWPosInDir(player, direction, &pos);
        tileType = GetTileTypeAtPos(mapper, &pos);
    } else if (Field_GetResolvedControllerTypeID(player->field) == 1) {
        actor = FieldPlayer_GetActor(player);
        nogrid = Field_GetNoGridMapper(player->field);
        GetActorRailPosForDir(actor, direction, &railPos);
        tileType = FieldNoGridMapper_GetTileAtPos(nogrid, &railPos);
    }
    return tileType;
}

u16 FieldPlayer_GetObjCodeByForme(u32 sex, u32 forme) {
    return PLAYER_SPRITE_MAP[sex][forme].objCode;
}

u16 FieldPlayer_GetObjCodeByExState(u32 sex, u32 exState) {
    const PlayerSprite *sprite;
    int i;

    for (i = 0, sprite = PLAYER_SPRITE_MAP[sex]; i < PLAYER_FORM_COUNT; i++, sprite++) {
        if (sprite->exState == exState) {
            return sprite->objCode;
        }
    }
    return 0x1;
}

// The form with the object code, PLAYER_FORM_COUNT if it isn't one of the player's
u32 FieldPlayer_GetExStateByObjCode(u32 objCode) {
    int sex, i;

    for (sex = 0; sex < 2; sex++) {
        const PlayerSprite *sprite = PLAYER_SPRITE_MAP[sex];

        for (i = 0; i < PLAYER_FORM_COUNT; i++, sprite++) {
            if (sprite->objCode == objCode) {
                return i;
            }
        }
    }
    return PLAYER_FORM_COUNT;
}

BOOL func_ov021_021a3208(FieldPlayer *player) {
    switch (FieldPlayer_GetExState(player)) {
    case FLD_PLAYER_EXSTATE_SURF:
    case FLD_PLAYER_EXSTATE_DIVE:
        return FALSE;
    }
    return TRUE;
}

void FieldPlayer_ForceObjCode(FieldPlayer *player, u16 objCode) {
    FieldPlayerCore_ForceObjCode(player->core, objCode);
}

void FieldPlayer_ClearObjCode(FieldPlayer *player) {
    FieldPlayerCore_ClearObjCode(player->core);
}

u32 func_ov021_021a3238(FieldPlayer *player) {
    return func_ov021_021a3a20(player->core);
}

BOOL func_ov021_021a3244(FieldPlayer *player) {
    return func_ov021_021a3a3c(player->core);
}

FieldActor *FieldPlayer_GetActorInFront(FieldPlayer *player) {
    s16 x, y, z;
    RailPosition railPos;
    MMSys *system = Field_GetActorSystem(player->field);

    if (Field_GetResolvedControllerTypeID(player->field) == CONTROLLER_GRID) {
        FieldPlayer_GetGPosInFront(player, &x, &y, &z);
        return GetFirstActorOnGPos(system, x, z, FALSE);
    }
    func_ov021_0219e634(FieldPlayerCore_GetActor(player->core), &railPos);
    return FindActorByRailPos(system, &railPos, FALSE);
}

FieldActor *FieldPlayer_GetActorInFrontEx(FieldPlayer *player, fx32 maxHeightDiff) {
    s16 x, y, z;
    RailPosition railPos;
    MMSys *system = Field_GetActorSystem(player->field);

    if (Field_GetResolvedControllerTypeID(player->field) == CONTROLLER_GRID) {
        FieldPlayer_GetGPosInFront(player, &x, &y, &z);
        return FindActorByGPos(system, x, z, GRID_TO_FX32(y), maxHeightDiff, FALSE);
    }
    func_ov021_0219e634(FieldPlayerCore_GetActor(player->core), &railPos);
    return FindActorByRailPos(system, &railPos, FALSE);
}

void GetPlayerGPosPlusDir(FieldPlayer *player, u16 dir, s16 *x, s16 *y, s16 *z) {
    FieldActor *actor = FieldPlayer_GetActor(player);

    if (Field_GetResolvedControllerTypeID(player->field) == CONTROLLER_GRID) {
        *x = GetGPosX(actor);
        *y = FldAct_GetGPosY(actor);
        *z = GetGPosZ(actor);
        *x += GetDirectionVectorCompX(dir);
        *z += GetDirectionVectorCompZ(dir);
    }
}

void FieldPlayer_GetWPosInDir(FieldPlayer *player, u16 direction, VecFx32 *position) {
    VecFx32 pos;
    VecFx16 dirVec;
    RailPosition railPos;
    s16 x, y, z;
    FieldActor *actor = FieldPlayerCore_GetActor(player->core);

    if (Field_GetResolvedControllerTypeID(player->field) == CONTROLLER_GRID) {
        GetPlayerGPosPlusDir(player, direction, &x, &y, &z);
        ConvGXZToVector(x, z, position);
        position->y = GRID_TO_FX32(y);
    } else {
        FieldRailSystem *rail = FieldNoGridMapper_GetRailSystem(Field_GetNoGridMapper(player->field));
        fx32 step;

        GetActorRailPos(actor, &railPos);
        func_ov021_021b7d38(rail, &railPos, &pos);
        func_ov021_0219e784(actor, direction, &dirVec);
        step = func_ov021_021b7c38(rail);
        position->x = pos.x + FX_Mul(dirVec.x, step);
        position->y = pos.y + FX_Mul(dirVec.y, step);
        position->z = pos.z + FX_Mul(dirVec.z, step);
    }
}

void FieldPlayer_GetGPosInFront(FieldPlayer *player, s16 *x, s16 *y, s16 *z) {
    GetPlayerGPosPlusDir(player, GetActorFaceDir(FieldPlayer_GetActor(player)), x, y, z);
}

void func_ov021_021a3484(FieldPlayer *player, u32 dir, VecFx32 *vec) {
    VecFx16 dirVec;
    FieldActor *actor = FieldPlayerCore_GetActor(player->core);

    if (Field_GetResolvedControllerTypeID(player->field) == CONTROLLER_GRID) {
        vec->y = 0;
        vec->x = GetDirectionVectorCompX(dir);
        vec->z = GetDirectionVectorCompZ(dir);
        vec->x = GRID_TO_FX32(vec->x);
        vec->z = GRID_TO_FX32(vec->z);
        VEC_Normalize(vec, vec);
    } else {
        func_ov021_0219e784(actor, dir, &dirVec);
        VEC_Set(vec, dirVec.x, dirVec.y, dirVec.z);
    }
}

u32 FieldPlayer_DeriveExState(FieldPlayer *player) {
    return FieldPlayerCore_DeriveExState(player->core);
}

BOOL func_ov021_021a34f8(FieldPlayer *player) {
    return func_ov021_021a39e4(player->core);
}

// Whether the player can start surfing onto the tile in the direction: they aren't surfing or diving, the tiles allow
// it, and the water is level with them or less than SURF_HEIGHT_MAX below
BOOL CheckSurfHeightAllow(FieldPlayer *player, u32 tileUnder, u32 frontTileType) {
    u32 exState = FieldPlayer_GetExState(player);
    if (exState == FLD_PLAYER_EXSTATE_SURF || exState == FLD_PLAYER_EXSTATE_DIVE) {
        return FALSE;
    }
    return CheckSurfBeginTiles(tileUnder, frontTileType);
}

BOOL CheckCanInteractWaterfall(FieldPlayer *player, u32 tileUnder, u32 tileInFront) {
    u32 exState = FieldPlayer_GetExState(player);

    if (MapTile_BlocksCollision(tileUnder) || exState != FLD_PLAYER_EXSTATE_SURF) {
        return FALSE;
    }
    return IsTileWaterfall(GetTileClass(tileInFront));
}

void FieldPlayer_InterruptCatwalkBalance(FieldPlayer *player, BOOL keep) {
    if (Field_GetResolvedControllerTypeID(player->field) == CONTROLLER_GRID) {
        FieldPlayerGrid_InterruptCatwalkBalance(player->grid, keep);
    }
}

void func_ov021_021a356c(FieldPlayer *player) {
    if (Field_GetResolvedControllerTypeID(player->field) == CONTROLLER_GRID) {
        func_ov021_021a575c(player->grid);
    }
}

BOOL FieldPlayer_GetVerticalMoveOnlyFlag(FieldPlayer *player) {
    if (Field_GetResolvedControllerTypeID(player->field) == CONTROLLER_GRID) {
        return FieldPlayerGrid_GetVerticalMoveOnlyFlag(player->grid);
    }
    return FALSE;
}

void func_ov021_021a5768(FieldPlayerGrid *grid, BOOL keep);
void func_ov021_021a35a0(FieldPlayer *player, BOOL keep) {
    if (Field_GetResolvedControllerTypeID(player->field) == CONTROLLER_GRID) {
        func_ov021_021a5768(player->grid, keep);
    }
}


BOOL FieldPlayer_CheckRunningShoesFlag(FieldPlayer *player) {
    EventWork *eventWork = GameData_GetEventWork(GSYS_GetGameData(FieldPlayer_GetGameSystem(player)));

    if (EventWork_FlagGet(eventWork, EVENT_FLAG_RUNNING_SHOES)) {
        return TRUE;
    }
    return FALSE;
}

void FieldPlayer_InitGrid(FieldPlayer *player, HeapID heapId) {
    player->grid = func_ov021_021a4140(player->core, heapId);
}

void FieldPlayer_UpdateGridMovement(FieldPlayer *player, u32 pressedKeys, u32 heldKeys, u32 flags) {
    FieldPlayerGrid_UpdateMovement(player->grid, pressedKeys, heldKeys, flags);
}

void FieldPlayer_GetGPos(FieldPlayer *player, s16 *x, s16 *y, s16 *z) {
    FieldActor *actor = FieldPlayerCore_GetActor(player->core);

    *x = GetGPosX(actor);
    *y = FldAct_GetGPosY(actor);
    *z = GetGPosZ(actor);
}

BOOL func_ov021_021a3624(FieldPlayer *player) {
    if (Field_GetResolvedControllerTypeID(player->field) == CONTROLLER_GRID
        && FieldPlayerGrid_IsCatwalkExiting(player->grid) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void func_ov021_021a3644(FieldPlayer *player) {
    if (Field_GetResolvedControllerTypeID(player->field) == CONTROLLER_GRID
        && FieldPlayerGrid_IsCatwalkExiting(player->grid) == TRUE) {
        FieldPlayerGrid_SanitizeHeightMismatch(player->grid);
    }
}

void func_ov021_021a3664(FieldPlayer *player, HeapID heapId) {
    player->nogrid = func_ov021_021a5880(player->core, heapId);
}

void FieldPlayer_UpdateRailMovement(FieldPlayer *player, u32 pressedKeys, u32 heldKeys) {
    func_ov021_021a596c(player->nogrid, pressedKeys, heldKeys);
}

void func_ov021_021a3680(FieldPlayer *player, RailPosition *pos)
{
    func_ov021_021a5928(player->nogrid, pos);
}

void func_ov021_021a368c(FieldPlayer *player)
{
    func_ov021_021a5964(player->nogrid);
}

BOOL func_ov021_021a3698(FieldPlayer *player)
{
    return func_ov021_021a59ec(player->nogrid);
}

RailUnit *func_ov021_021a36a4(FieldPlayer *player)
{
    return func_ov021_021a5a30(player->nogrid);
}

void FieldPlayer_SetRailPos(FieldPlayer *player, const RailPosition *pos) {
    func_ov021_021a5a00(player->nogrid, pos);
}

