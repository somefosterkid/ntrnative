// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov107/field_gimmick_d06.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/sound.h"
#include "constants/zones.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "field/field_gimmick_d06.h"
#include "field/field_map.h"
#include "gfl/heap.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"

#define STONE_ACTOR_ID 0xfd
#define STONE_COUNT 12
#define MARK_COUNT 14
#define EFFECT_ID 11

// The action commands that the pushes use
#define ACMD_WALK_16F 0x8
#define ACMD_WALK_8F 0xc
#define ACMD_WALK_4F 0x10
#define ACMD_STAY_WALK_16F 0x1c

// How far a stone can go in a push
#define MAX_PUSH_STEPS 0x20

// A position on the grid. The stones ignore y
typedef struct {
    s32 x;
    s32 y;
    s32 z;
} StoneGridPos;

// The stones and the marks of a zone
typedef struct {
    u32 zone;
    u32 count;
    const StoneGridPos *positions;
} ZoneTable;

// What the save keeps: whether the stones exist already
typedef struct {
    u8 initialized;
    struct StoneWork *work;
} D06State;

// A stone and its effect
typedef struct {
    FieldActor *actor;
    FieldEffectTask *effect;
} StoneSlot;

typedef struct StoneWork {
    u32 zone;
    D06State *state;
    Field *field;
    StoneSlot stones[STONE_COUNT];
    FieldEffectTask *marks[MARK_COUNT];
} StoneWork;

// The event of a push
typedef struct {
    u16 dir;
    u16 slotIndex;
    int distance;
    StoneSlot *slot;
    StoneWork *work;
} PushData;

static const ZoneTable *func_ov051_021f36c0(u32 zone);
static const StoneGridPos *func_ov051_021f36e0(u32 zone);
static u32 func_ov051_021f36ec(u32 zone);
static const ZoneTable *func_ov051_021f36f8(u32 zone);
static const StoneGridPos *func_ov051_021f3718(u32 zone);
static u32 func_ov051_021f3724(u32 zone);
static GameEvent *func_ov051_021f3958(Field *field, FieldActor *stone, u16 dir);
static GameEventReturnCode func_ov051_021f39f8(GameEvent *event, u32 *state, void *data);
static GameEventReturnCode func_ov051_021f3ab4(GameEvent *event, u32 *state, void *data);
static void func_ov051_021f3b44(StoneWork *work, StoneSlot *slot, int x, int z);
static void func_ov051_021f3b88(StoneWork *work, StoneSlot *slot, FieldActor *actor);
static BOOL func_ov051_021f3bc4(StoneWork *work, StoneSlot *slot, u16 dir, u16 maxSteps, u32 *steps);
static u32 func_ov051_021f3c84(StoneWork *work, StoneSlot *slot, u16 dir);
static BOOL func_ov051_021f3c98(StoneWork *work, StoneSlot *slot);
static void func_ov051_021f3ccc(StoneWork *work, FieldEffectTask **mark, int x, int y, int z);
static void func_ov051_021f3d04(Field *field, u32 index);
FieldActor *func_ov051_021f3d38(Field *field, int x, int z);
static StoneWork *func_ov051_021f3d6c(Field *field);
static u32 func_ov051_021f3d94(u32 zone);

// pokebw2's sStonesZone3
static const StoneGridPos data_ov051_021f3e20[5] = {
#include "asset/field_gimmick_d06_data_ov051_021f3e20.inc"
};

// pokebw2's sMarksZone3
static const StoneGridPos data_ov051_021f3e5c[11] = {
#include "asset/field_gimmick_d06_data_ov051_021f3e5c.inc"
};

// pokebw2's sStonesZone4
static const StoneGridPos data_ov051_021f3dd8[3] = {
    { 9, 0, 23 },
    { 21, 0, 9 },
    { 24, 0, 3 },
};

// pokebw2's sMarksZone4
static const StoneGridPos data_ov051_021f3ee0[11] = {
#include "asset/field_gimmick_d06_data_ov051_021f3ee0.inc"
};

// pokebw2's sStonesZone2
static const StoneGridPos data_ov051_021f3f64[12] = {
#include "asset/field_gimmick_d06_data_ov051_021f3f64.inc"
};

// pokebw2's sMarksZone2
static const StoneGridPos data_ov051_021f3ff4[14] = {
#include "asset/field_gimmick_d06_data_ov051_021f3ff4.inc"
};

// pokebw2's sStoneTables
static const ZoneTable data_ov051_021f3db4[3] = {
    { ZONE_CHARGESTONE_CAVE_2, NELEMS(data_ov051_021f3f64), data_ov051_021f3f64 },
    { ZONE_CHARGESTONE_CAVE_3, NELEMS(data_ov051_021f3e20), data_ov051_021f3e20 },
    { ZONE_CHARGESTONE_CAVE_4, NELEMS(data_ov051_021f3dd8), data_ov051_021f3dd8 },
};

// pokebw2's sMarkTables
static const ZoneTable data_ov051_021f3dfc[3] = {
    { ZONE_CHARGESTONE_CAVE_2, NELEMS(data_ov051_021f3ff4), data_ov051_021f3ff4 },
    { ZONE_CHARGESTONE_CAVE_3, NELEMS(data_ov051_021f3e5c), data_ov051_021f3e5c },
    { ZONE_CHARGESTONE_CAVE_4, NELEMS(data_ov051_021f3ee0), data_ov051_021f3ee0 },
};

// pokebw2's FindStoneTable
static const ZoneTable *func_ov051_021f36c0(u32 zone) {
    int i;
    const ZoneTable *table = data_ov051_021f3db4;

    for (i = 0; i < NELEMS(data_ov051_021f3db4); i++, table++) {
        if (table->zone == zone) {
            return table;
        }
    }
    return &data_ov051_021f3db4[0];
}

// pokebw2's GetStonePositions
static const StoneGridPos *func_ov051_021f36e0(u32 zone) {
    return func_ov051_021f36c0(zone)->positions;
}

// pokebw2's GetStoneCount
static u32 func_ov051_021f36ec(u32 zone) {
    return func_ov051_021f36c0(zone)->count;
}

// pokebw2's FindMarkTable
static const ZoneTable *func_ov051_021f36f8(u32 zone) {
    int i;
    const ZoneTable *table = data_ov051_021f3dfc;

    for (i = 0; i < NELEMS(data_ov051_021f3dfc); i++, table++) {
        if (table->zone == zone) {
            return table;
        }
    }
    return &data_ov051_021f3dfc[0];
}

// pokebw2's GetMarkPositions
static const StoneGridPos *func_ov051_021f3718(u32 zone) {
    return func_ov051_021f36f8(zone)->positions;
}

// pokebw2's GetMarkCount
static u32 func_ov051_021f3724(u32 zone) {
    return func_ov051_021f36f8(zone)->count;
}

// pokebw2's GimmickD06_Setup
void func_ov051_021f3730(Field *field) {
    HeapID heapId = Field_GetHeapID(field);
    GimmickState *gimmickState = GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field)));
    void *effects = Field_GetFieldEffects(field);
    u32 zone = Field_GetPlayerStateZoneID(field);
    D06State *state = GimmickState_GetUserData(gimmickState, func_ov051_021f3d94(zone));
    StoneWork *work;
    u32 effectId;
    u32 count;
    const StoneGridPos *positions;
    u32 i;

    work = GFL_HeapAllocate(heapId, sizeof(StoneWork), TRUE, "field_gimmick_d06.c", 202);
    state->work = work;
    work->state = state;
    work->field = field;
    work->zone = zone;
    effectId = EFFECT_ID;
    FieldEffects_Load(effects, &effectId, 1);
    if (!state->initialized) {
        count = func_ov051_021f36ec(work->zone);
        positions = func_ov051_021f36e0(work->zone);
        for (i = 0; i < count; i++, positions++) {
            func_ov051_021f3b44(work, &work->stones[i], positions->x, positions->z);
        }
    } else {
        MMSys *actorSystem;
        u32 index = 0;
        FieldActor *actor;

        i = 0;
        actorSystem = Field_GetActorSystem(field);
        while (NextActor(actorSystem, &actor, &index)) {
            if (GetActorUID(actor) == STONE_ACTOR_ID) {
                func_ov051_021f3b88(work, &work->stones[i], actor);
                i++;
                if (i >= STONE_COUNT) {
                    break;
                }
            }
        }
    }
    count = func_ov051_021f3724(work->zone);
    positions = func_ov051_021f3718(work->zone);
    for (i = 0; i < count; i++, positions++) {
        func_ov051_021f3ccc(work, &work->marks[i], positions->x, positions->y, positions->z);
    }
    state->initialized = TRUE;
}

// pokebw2's GimmickD06_End
void func_ov051_021f3868(Field *field) {
    Field_GetHeapID(field);
    GimmickState *gimmickState = GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field)));
    Field_GetFieldEffects(field);
    D06State *state;

    state = GimmickState_GetUserData(gimmickState, func_ov051_021f3d94(Field_GetPlayerStateZoneID(field)));
    GFL_HeapFree(state->work);
}

// pokebw2's GimmickD06_Update
void func_ov051_021f38a0(Field *field) {
}

// pokebw2's GimmickD06_CheckPush
GameEvent *func_ov051_021f38a4(Field *field, u16 dir) {
    GameEvent *event = NULL;
    StoneWork *work = func_ov051_021f3d6c(field);
    VecFx32 pos;
    s16 gridX;
    s16 gridZ;
    u32 count;
    u32 i;
    const StoneGridPos *positions;
    FieldActor *stone;

    FieldPlayer_GetWPos(Field_GetPlayer(field), &pos);
    ExpandVecInGridDir(dir, &pos, FX32_ONE << 4);
    gridX = (s16)((pos.x >> 4) / FX32_ONE);
    gridZ = (s16)((pos.z >> 4) / FX32_ONE);
    count = func_ov051_021f3724(work->zone);
    positions = func_ov051_021f3718(work->zone);
    for (i = 0; i < count; i++, positions++) {
        if (positions->x == gridX && positions->z == gridZ) {
            func_ov051_021f3d04(field, i);
        }
    }
    stone = GetFirstActorOnGPos(Field_GetActorSystem(field), gridX, gridZ, FALSE);
    if (stone != NULL && GetActorUID(stone) == STONE_ACTOR_ID) {
        event = func_ov051_021f3958(field, stone, dir);
    }
    return event;
}

// pokebw2's StartPush
static GameEvent *func_ov051_021f3958(Field *field, FieldActor *stone, u16 dir) {
    u16 i;
    StoneWork *work = func_ov051_021f3d6c(field);
    u32 distance;
    GameSystem *gsys = Field_GetGameSystem(field);
    StoneSlot *slot;
    GameEvent *event;
    PushData *data;

    for (i = 0; i < STONE_COUNT; i++) {
        if (work->stones[i].actor == stone) {
            break;
        }
    }
    if (i >= STONE_COUNT) {
        i = 0;
    }
    slot = &work->stones[i];
    if (func_ov051_021f3c98(work, slot) == TRUE) {
        return NULL;
    }
    distance = func_ov051_021f3c84(work, slot, dir);
    if (distance == 0) {
        event = GameEvent_Create(gsys, NULL, func_ov051_021f3ab4, sizeof(PushData));
    } else {
        event = GameEvent_Create(gsys, NULL, func_ov051_021f39f8, sizeof(PushData));
    }
    data = GameEvent_GetData(event);
    MI_CpuFill(data, 0, sizeof(PushData));
    data->dir = dir;
    data->work = work;
    data->slotIndex = i;
    data->slot = slot;
    data->distance = distance;
    return event;
}

// pokebw2's PushEvent
static GameEventReturnCode func_ov051_021f39f8(GameEvent *event, u32 *state, void *data) {
    PushData *push = data;
    FieldActor *player = FieldPlayer_GetActor(Field_GetPlayer(push->work->field));

    switch (*state) {
    case 0:
        if (IsAllActorAcmdFinished(player) == TRUE) {
            FldAct_SetAcmd(player, GetAcmdForDir(push->dir, ACMD_STAY_WALK_16F));
            GFL_SndSEPlay(SEQ_SE_FLD_63);
            (*state)++;
        }
        break;
    case 1:
        if (IsAllActorAcmdFinished(push->slot->actor) == TRUE) {
            if (push->distance > 0) {
                u16 acmd = ACMD_WALK_16F;

                if (push->distance <= 1) {
                    acmd = ACMD_WALK_4F;
                } else if (push->distance <= 2) {
                    acmd = ACMD_WALK_4F;
                } else if (push->distance <= 4) {
                    acmd = ACMD_WALK_8F;
                }
                FldAct_SetAcmd(push->slot->actor, GetAcmdForDir(push->dir, acmd));
                push->distance--;
            } else {
                GFL_SndSEPlay(SEQ_SE_FLD_50);
                (*state)++;
            }
        }
        break;
    case 2:
        if (func_ov010_0216d454(player) == TRUE) {
            func_ov010_0216d480(push->slot->actor);
            func_ov010_0216d480(player);
            func_ov021_021c909c(push->slot->effect);
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}

// pokebw2's BlockedEvent
static GameEventReturnCode func_ov051_021f3ab4(GameEvent *event, u32 *state, void *data) {
    PushData *push = data;
    FieldActor *player = FieldPlayer_GetActor(Field_GetPlayer(push->work->field));

    switch (*state) {
    case 0:
        if (IsAllActorAcmdFinished(player) == TRUE) {
            FldAct_SetAcmd(player, GetAcmdForDir(push->dir, ACMD_STAY_WALK_16F));
            GFL_SndSEPlay(SEQ_SE_WALL_HIT);
            (*state)++;
        }
    case 1:
        func_ov021_021c90ac(push->slot->effect);
        (*state)++;
        break;
    case 2:
        if (func_ov021_021c90d0(push->slot->effect) == FALSE) {
            break;
        }
        (*state)++;
    case 3:
        if (func_ov010_0216d454(player) == TRUE) {
            func_ov010_0216d480(player);
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}

// pokebw2's InitStoneAt
static void func_ov051_021f3b44(StoneWork *work, StoneSlot *slot, int x, int z) {
    VecFx32 pos;

    slot->actor = func_ov051_021f3d38(work->field, x, z);
    CopyActorWPos(slot->actor, &pos);
    slot->effect = func_ov021_021c9064(Field_GetFieldEffects(work->field), &pos, slot->actor);
    if (func_ov051_021f3c98(work, slot) == TRUE) {
        func_ov021_021c909c(slot->effect);
    }
}

// pokebw2's InitStoneActor
static void func_ov051_021f3b88(StoneWork *work, StoneSlot *slot, FieldActor *actor) {
    VecFx32 pos;

    slot->actor = actor;
    CopyActorWPos(actor, &pos);
    slot->effect = func_ov021_021c9064(Field_GetFieldEffects(work->field), &pos, slot->actor);
    if (func_ov051_021f3c98(work, slot) == TRUE) {
        func_ov021_021c909c(slot->effect);
    }
}

// Whether the stone, moving in dir over free tiles, reaches an electric rock tile within maxSteps; the steps it takes
// then, or 0
// pokebw2's ProbeElectricRock
static BOOL func_ov051_021f3bc4(StoneWork *work, StoneSlot *slot, u16 dir, u16 maxSteps, u32 *steps) {
    FieldActor *actor = slot->actor;
    u16 i = 0;
    BOOL found = FALSE;
    FieldG3DMapper *mapper = Field_GetG3DMapper(work->field);
    s16 x;
    s16 y;
    s16 z;
    u32 collision;
    VecFx32 pos;
    u32 tileType;
    u32 walkable;

    x = GetGPosX(actor);
    y = FldAct_GetGPosY(actor);
    z = GetGPosZ(actor);
    walkable = 1;
    do {
        AdjusGridXZByDir(dir, &x, &z, 1);
        collision = ActorRouteCollCheck(actor, x, y, z, dir) & ~walkable;
        if (collision) {
            if (collision & 2) {
                ConvGXZToVector(x, z, &pos);
                pos.y = GetActorPosY(actor);
                tileType = GetTileTypeAtPos(mapper, &pos);
                if (tileType != -1 && MapTile_IsElectricRock(GetTileClass(tileType)) == TRUE) {
                    found = TRUE;
                }
            }
            break;
        }
        i++;
    } while (i < maxSteps);
    if (!found) {
        i = 0;
    }
    *steps = i;
    return found;
}

// pokebw2's GetDistanceToElectricRock
static u32 func_ov051_021f3c84(StoneWork *work, StoneSlot *slot, u16 dir) {
    u32 steps;

    func_ov051_021f3bc4(work, slot, dir, MAX_PUSH_STEPS, &steps);
    return steps;
}

// pokebw2's IsNextToElectricRock
static BOOL func_ov051_021f3c98(StoneWork *work, StoneSlot *slot) {
    u16 dir;
    u32 steps;

    for (dir = 0; dir < 4; dir++) {
        if (func_ov051_021f3bc4(work, slot, dir, 1, &steps) == TRUE) {
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's InitMark
static void func_ov051_021f3ccc(StoneWork *work, FieldEffectTask **mark, int x, int y, int z) {
    VecFx32 pos;

    ConvGXZToVector(x, z, &pos);
    pos.y = (y << 16) + 0x8000;
    *mark = func_ov021_021c9258(Field_GetFieldEffects(work->field), &pos);
}

// pokebw2's StopMark
static void func_ov051_021f3d04(Field *field, u32 index) {
    GimmickState *gimmickState = GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field)));
    D06State *state = GimmickState_GetUserData(gimmickState, func_ov051_021f3d94(Field_GetPlayerStateZoneID(field)));

    func_ov021_021c928c(state->work->marks[index]);
}

// pokebw2's CreateStone
FieldActor *func_ov051_021f3d38(Field *field, int x, int z) {
    MMSys *actorSystem = Field_GetActorSystem(field);

    return CreateNewActorByParam(actorSystem, x, z, 0, STONE_ACTOR_ID, 0, 0, Field_GetPlayerStateZoneID(field));
}

// pokebw2's GetWork
static StoneWork *func_ov051_021f3d6c(Field *field) {
    GimmickState *gimmickState = GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field)));
    D06State *state = GimmickState_GetUserData(gimmickState, func_ov051_021f3d94(Field_GetPlayerStateZoneID(field)));

    return state->work;
}

// The gimmick is the same in every zone of the cave
// pokebw2's GetGimmickId: the gimmick state of the zone's stones
static u32 func_ov051_021f3d94(u32 zone) {
    switch (zone) {
    case ZONE_CHARGESTONE_CAVE_2:
        return 0x19;
    case ZONE_CHARGESTONE_CAVE_3:
        return 0x1a;
    case ZONE_CHARGESTONE_CAVE_4:
        return 0x1b;
    default:
        return 0x19;
    }
}
