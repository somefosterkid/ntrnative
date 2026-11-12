// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/effect_encount.c, ported by tools/port_file.py.
// Overworld patches (a grass patch, a dust cloud, a water ripple or a flying shadow) that appear near the player as they walk
// and start a battle or give an item when the player steps on them. The name is the ROM's own, from
// GFL_HeapAllocate's file argument. Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "field/encounter.h"
#include "field/event_wild_battle.h"
#include "field/festival.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "field/field_g3d_mapper.h"
#include "field/field_map.h"
#include "field/field_script_event.h"
#include "field/game_beacon_set.h"
#include "gfl/heap.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "save/high_link.h"
#include "save/save_control.h"
#include "save/trainer_card.h"
#include "system/game_data.h"

// A tile around the player where a phenomenon could appear
typedef struct {
    u8 kind;
    u16 x;
    u16 y;
    u16 z;
    fx32 height;
} EffectEncountCandidate;

typedef struct {
    u16 count;
    // An area of 11 by 11 tiles around the player
    EffectEncountCandidate entries[121];
} EffectEncountCandidates;

struct EffectEncountState {
    void *fieldEffects;
    EffectEncountCandidates candidates;
    // The phenomenon's field effect
    FieldEffectTask *effect;
    // The steps between phenomena, and the chance of one, in tenths of a percent
    u16 steps;
    u16 chance;
    // The item the phenomenon gave
    u16 itemId;
};

// The area around the player that func_ov021_021aabf4 finds, in tiles
typedef struct {
    s16 x;
    s16 z;
    // The player's tile in its block of 32 by 32, the block, and the block's first tile
    s16 blockTileX;
    s16 blockTileZ;
    s16 blockX;
    s16 blockZ;
    u16 originX;
    u16 originZ;
    s16 minX;
    s16 minZ;
    s16 maxX;
    s16 maxZ;
} EffectEncountArea;

void func_ov021_021aaabc(EncountSystem *system);
void func_ov021_021aab54(EncountSystem *system);
void func_ov021_021aab74(EncountSystem *system, EffectEncountState *state);
static BOOL func_ov021_021aabc4(EncountState *encountState, EffectEncountState *state);
void setShakingSpotOff(EncountState *encountState);
static void func_ov021_021aabf4(EncountSystem *system, EffectEncountState *state, EffectEncountArea *area);
static void positionShakingSpot(EncountSystem *system, EffectEncountState *state);
void func_ov021_021aae34(EncountSystem *system, EffectEncountState *state);
static BOOL IsPositionOccupiedByActorAny(EncountSystem *system, s16 x, s16 y, s16 z, BOOL skipPlayer);
BOOL EncountSystem_CancelPhenomenonIfActorHitCore(EncountSystem *system);
void EncountSystem_CancelPhenomenonCore(EncountSystem *system, EffectEncountState *state);
static void func_ov021_021aafc0(EncountSystem *system, EffectEncountState *state, RareEncountSpot *spot);
static void func_ov021_021aaffc(EffectEncountState *state);
static u16 EffectEncount_GenDustCloudItem(u16 zoneId);
static u16 GetRandomWingItemID(void);
static GameEvent *CallGivePhenomenonItem(EncountSystem *system, u16 itemId);

// The steps between phenomena and their chance, for each setting of the zone's encounter data
static const u8 data_ov021_021d8e70[3][2] = {{20, 10}, {20, 10}, {5, 15}};

// The items of a dust cloud by the chance roll, in tenths of the rolls of 100
static const s8 data_ov021_021d8e76[5][4] = {
    {-5, 5, -5, 5}, {0, 5, -5, 5}, {-5, 0, -5, 5}, {-5, 5, -5, 0}, {-5, 5, 0, 5},
};

// The shards a dust cloud gives in the zones of DUST_CLOUD_SHARD_ZONES
static const u16 DUST_CLOUD_ITEMS_SHARDS[4] = {72, 73, 74, 75};

// The phenomena each kind of tile can have: 1 for grass, 2 for water and dust
static const u8 data_ov036_021d02d2[10] = {1, 1, 1, 1, 1, 2, 2, 1, 0, 0};

// The rarer items of a dust cloud
static const u16 DUST_CLOUD_ITEMS_NORMAL[10] = {80, 81, 82, 83, 84, 85, 107, 108, 109, 110};

// The area around the player a phenomenon can appear in, left, right, up and down, all around for the first and in
// one direction for the others
static const u16 data_ov021_021d8e8a[10] = {0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x6b, 0x6c, 0x6d, 0x6e};

static const u16 DUST_CLOUD_SHARD_ZONES[15] = {
    0x1f7, 0x1f8, 0x1f9, 0x213, 0x214, 0x215, 0x216, 0x217, 0x218, 0x219, 0x21a, 0x21b, 0x21c, 0x21d, 0x21e,
};

int RangeCheckChunkCoordinateWorld(MapMatrix *matrix, s32 x, s32 z);
u32 GetZoneIDAtMatrixXZWorld(MapMatrix *matrix, s32 x, s32 z);

static inline BOOL RareEncountSpot_IsAt(const RareEncountSpot *spot, const s16 *pos) {
    if (spot->x != pos[0] || spot->z != pos[2] || spot->y != pos[1]) {
        return FALSE;
    }
    return TRUE;
}

EffectEncountState *EffectEncountState_Create(HeapID heapId) {
    return GFL_HeapAllocate(heapId, sizeof(EffectEncountState), TRUE, "effect_encount.c", 110);
}

void EffectEncountState_Free(EffectEncountState *state) {
    MI_CpuFill8(state, 0, sizeof(EffectEncountState));
    GFL_HeapFree(state);
}

void PrepareFieldEncountSystem(EncountSystem *system, EffectEncountState *state) {
    state->fieldEffects = Field_GetFieldEffects(system->field);
    func_ov021_021aa7dc(system, state);
    func_ov021_021aab74(system, state);
}

void func_ov021_021aa7cc(EncountSystem *system, EffectEncountState *state) {
    func_ov021_021aab54(system);
    func_ov021_021aaffc(state);
}

void func_ov021_021aa7dc(EncountSystem *system, EffectEncountState *state) {
    u8 setting = system->encData->flags;

    if (setting > 2) {
        setting = 0;
    }
    state->steps = data_ov021_021d8e70[setting][0];
    state->chance = data_ov021_021d8e70[setting][1];
}

void UpdatePhenomenon(EncountSystem *system) {
    EffectEncountState *state = system->effectEncountState;
    EncountState *encountState = GameData_GetEncountState(system->gameData);
    MapMatrix *matrix;
    u16 zoneId;
    int matrixZoneId;
    VecFx32 pos;

    if (system == NULL || system->encData == NULL || !system->encData->fishingEnable) {
        return;
    }
    matrix = GetMapMatrixSystem(system->gameData);
    if (system->field == NULL || matrix == NULL) {
        return;
    }
    zoneId = Field_GetPlayerStateZoneID(system->field);
    CopyActorWPos(FieldPlayer_GetActor(Field_GetPlayer(system->field)), &pos);
    if (!RangeCheckChunkCoordinateWorld(matrix, pos.x, pos.z)) {
        return;
    }
    matrixZoneId = GetZoneIDAtMatrixXZWorld(matrix, pos.x, pos.z);
    if (zoneId == 0xffff || matrixZoneId == 0xffff || zoneId != matrixZoneId) {
        return;
    }
    if (!isBadgeObtained(getTrainerCardDataBlkAddress(system->gameData), 0)) {
        return;
    }
    if (Field_GetResolvedControllerTypeID(system->field) != 0) {
        return;
    }
    if (encountState->rareSpot.active) {
        return;
    }
    if (!func_ov021_021aabc4(encountState, state)) {
        return;
    }
    if (GFL_RandomLCAlt(1000) >= state->chance * 10) {
        setShakingSpotOff(encountState);
        return;
    }
    positionShakingSpot(system, state);
    if (state->candidates.count != 0) {
        func_ov021_021aae34(system, state);
    }
}


void EncountSystem_CancelPhenomenon(EncountSystem *system) {
    EncountSystem_CancelPhenomenonCore(system, system->effectEncountState);
}

BOOL EncountSystem_CancelPhenomenonIfActorHit(EncountSystem *system) {
    return EncountSystem_CancelPhenomenonIfActorHitCore(system);
}

BOOL EncountState_CheckSpecialEncountPos(EncountSystem *encounter, const s16 *gridPos) {
    EncountState *encountState = GameData_GetEncountState(encounter->gameData);

    if (!encountState->rareSpot.active) {
        return FALSE;
    }
    if (encountState->rareSpot.x != gridPos[0] || encountState->rareSpot.z != gridPos[2] ||
        encountState->rareSpot.y != gridPos[1]) {
        return FALSE;
    }
    return TRUE;
}

GameEvent *CreateRandomPhenomenonEvent(EncountSystem *system) {
    EffectEncountState *state = system->effectEncountState;
    EncountState *encountState = GameData_GetEncountState(system->gameData);
    RareEncountSpot *spot = &encountState->rareSpot;
    BOOL gaveItem = FALSE;
    GameEvent *event;
    GridPos pos;
    u16 roll;

    if (!spot->active) {
        return NULL;
    }
    FldAct_GetGPos(FieldPlayer_GetActor(Field_GetPlayer(system->field)), &pos);
    if (!RareEncountSpot_IsAt(spot, &pos.x)) {
        return NULL;
    }
    roll = GFL_RandomLCAlt(1000);
    if (spot->kind == 7) {
        if (roll < 200) {
            event = EventWildBattleCall_CreateRandom(system, 1);
        } else {
            event = CallGivePhenomenonItem(system, GetRandomWingItemID());
            gaveItem = TRUE;
        }
    } else if (spot->kind == 4) {
        if (roll < 400) {
            event = EventWildBattleCall_CreateRandom(system, 1);
        } else {
            event = CallGivePhenomenonItem(system, EffectEncount_GenDustCloudItem(400));
            gaveItem = TRUE;
        }
    } else {
        event = EventWildBattleCall_CreateRandom(system, 1);
    }
    if (event == NULL) {
        EncountSystem_CancelPhenomenonCore(system, state);
        return NULL;
    }
    if (gaveItem) {
        EncountSystem_CancelPhenomenonCore(system, state);
    } else {
        FieldPlayer *player;

        if (spot->kind == 5 || spot->kind == 6) {
            func_ov021_021aab1c(system, 1);
        }
        spot->triggered = 1;
        player = Field_GetPlayer(system->field);
        if (func_ov021_021a3624(player)) {
            func_ov021_021a3644(player);
        }
    }
    return event;
}



BOOL func_ov021_021aaa4c(EncountSystem *system, u16 *distance) {
    RareEncountSpot *spot = &GameData_GetEncountState(system->gameData)->rareSpot;
    GridPos pos;
    s16 dx;
    s16 dz;

    if (!spot->active) {
        *distance = 0;
        return FALSE;
    }
    FldAct_GetGPos(FieldPlayer_GetActor(Field_GetPlayer(system->field)), &pos);
    dx = spot->x - pos.x;
    if (dx < 0) {
        dx *= -1;
    }
    dz = spot->z - pos.z;
    if (dz < 0) {
        dz *= -1;
    }
    if (dx > dz) {
        *distance = dx;
    } else {
        *distance = dz;
    }
    return TRUE;
}


void func_ov021_021aaabc(EncountSystem *system) {
    RareEncountSpot *spot = &GameData_GetEncountState(system->gameData)->rareSpot;

    if (spot->active) {
        spot->triggered = 1;
        if (system->effectEncountState->effect != NULL) {
            func_ov021_021adb4c(system->effectEncountState->effect, 1);
        }
    }
}


void func_ov021_021aaaf0(EncountSystem *system, u32 a1) {
    EncountState *encountState = GameData_GetEncountState(system->gameData);
    EffectEncountState *state = system->effectEncountState;

    if (encountState->rareSpot.active && state->effect != NULL) {
        func_ov021_021adb4c(state->effect, a1);
    }
}

void func_ov021_021aab1c(EncountSystem *system, u32 a1) {
    EncountState *encountState = GameData_GetEncountState(system->gameData);
    EffectEncountState *state = system->effectEncountState;

    if (encountState->rareSpot.active && state->effect != NULL) {
        func_ov021_021adb5c(state->effect, a1);
    }
}



u16 func_ov021_021aab48(EncountSystem *system) {
    return system->effectEncountState->itemId;
}

void func_ov021_021aab54(EncountSystem *system) {
    RareEncountSpot *spot = &GameData_GetEncountState(system->gameData)->rareSpot;

    if (spot->active) {
        spot->suspended = 1;
    }
}

void func_ov021_021aab74(EncountSystem *system, EffectEncountState *state) {
    RareEncountSpot *spot = &GameData_GetEncountState(system->gameData)->rareSpot;

    if (spot->active && spot->suspended) {
        u16 zoneId;

        spot->suspended = 0;
        zoneId = Field_GetPlayerStateZoneID(system->field);
        if (zoneId != spot->zoneId || spot->triggered) {
            MI_CpuFill8(spot, 0, sizeof(RareEncountSpot));
        } else {
            func_ov021_021aafc0(system, state, spot);
        }
    }
}


static BOOL func_ov021_021aabc4(EncountState *encountState, EffectEncountState *state) {
    u32 *counter = &encountState->unkC;

    if (*counter < 0xffffffff) {
        (*counter)++;
    }
    if (*counter >= state->steps) {
        return TRUE;
    }
    return FALSE;
}




void setShakingSpotOff(EncountState *encountState) {
    encountState->unkC = 0;
}


static void func_ov021_021aabf4(EncountSystem *system, EffectEncountState *state, EffectEncountArea *area) {
    FieldActor *actor = FieldPlayer_GetActor(Field_GetPlayer(system->field));
    const s8 *offsets;
    s16 minX, maxX, minZ, maxZ;

    area->x = GetGPosX(actor);
    area->z = GetGPosZ(actor);
    area->blockTileX = area->x % 32;
    area->blockTileZ = area->z % 32;
    area->blockX = area->x / 32;
    area->blockZ = area->z / 32;
    area->originX = area->x - area->blockTileX;
    area->originZ = area->z - area->blockTileZ;
    if (system->encData->flags != 2) {
        offsets = data_ov021_021d8e76[GFL_RandomLCAlt(4) + 1];
    } else {
        offsets = data_ov021_021d8e76[0];
    }
    minX = area->blockTileX + offsets[0];
    if (minX < 0) {
        minX = 0;
    }
    maxX = area->blockTileX + offsets[1];
    if (maxX >= 32) {
        maxX = 31;
    }
    minZ = area->blockTileZ + offsets[2];
    if (minZ < 0) {
        minZ = 0;
    }
    maxZ = area->blockTileZ + offsets[3];
    if (maxZ >= 32) {
        maxZ = 31;
    }
    area->minX = minX + area->originX;
    area->maxX = maxX + area->originX;
    area->minZ = minZ + area->originZ;
    area->maxZ = maxZ + area->originZ;
}

static void positionShakingSpot(EncountSystem *system, EffectEncountState *state) {
    VecFx32 pos;
    EffectEncountArea area;
    MapTerrainBuf terrain;
    u32 chunk;
    fx32 startX;
    s16 z;
    FieldG3DMapper *mapper = Field_GetG3DMapper(system->field);
    s16 x;

    MI_CpuFill8(&state->candidates, 0, sizeof(EffectEncountCandidates));
    pos.y = 0;
    chunk = FieldG3DMapper_GetBasePosChunkHandleIdx(mapper);
    if (!FieldG3DMapper_CheckChunkHandleTerrainReady(mapper, chunk)) {
        return;
    }
    func_ov021_021aabf4(system, state, &area);
    z = area.minZ;
    pos.z = FX32_CONST(z * 16) + FX32_CONST(8);
    startX = FX32_CONST(area.minX * 16) + FX32_CONST(8);
    for (; z <= area.maxZ; z++) {
        pos.x = startX;
        for (x = area.minX; x <= area.maxX; x++) {
            u32 kind;

            func_ov021_0218dbac(mapper, chunk, &pos, &terrain);
            kind = func_ov021_021ab504(terrain.tileType);
            if (kind != 8) {
                state->candidates.entries[state->candidates.count].kind = kind;
                state->candidates.entries[state->candidates.count].x = x;
                state->candidates.entries[state->candidates.count].z = z;
                state->candidates.entries[state->candidates.count++].height = terrain.height;
            }
            pos.x += FX32_ONE * 16;
        }
        pos.z += FX32_ONE * 16;
    }
}


void func_ov021_021aae34(EncountSystem *system, EffectEncountState *state) {
    u16 index = GFL_RandomLCAlt(state->candidates.count);
    EffectEncountCandidate *candidate = &state->candidates.entries[index];
    EncountState *encountState = GameData_GetEncountState(system->gameData);
    RareEncountSpot *spot = &encountState->rareSpot;

    candidate->y = (candidate->height >> 4) / FX32_ONE;
    if (IsPositionOccupiedByActorAny(system, candidate->x, candidate->y, candidate->z, FALSE)) {
        return;
    }
    spot->x = candidate->x;
    spot->y = candidate->y;
    spot->z = candidate->z;
    spot->height = candidate->height;
    spot->kind = candidate->kind;
    spot->zoneId = Field_GetPlayerStateZoneID(system->field);
    spot->active = 1;
    setShakingSpotOff(encountState);
    func_ov021_021aafc0(system, state, spot);
}


static BOOL IsPositionOccupiedByActorAny(EncountSystem *system, s16 x, s16 y, s16 z, BOOL skipPlayer) {
    GridPos pos;
    FieldActor *actor;
    u32 index = 0;
    MMSys *mmSys;
    FieldActor *player;

    player = FieldPlayer_GetActor(Field_GetPlayer(system->field));
    mmSys = Field_GetActorSystem(system->field);

    for (;;) {
        if (!NextActor(mmSys, &actor, &index)) {
            break;
        }
        const FieldActorConfig *config;

        if (!func_ov010_0216d454(actor)) {
            continue;
        }
        if (actor == player && skipPlayer) {
            continue;
        }
        config = GetActorMdlInfo(actor);
        FldAct_GetGPos(actor, &pos);
        if (y == pos.y && x >= pos.x && x < pos.x + config->collWidth && z <= pos.z &&
            z > pos.z - config->collHeight) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL EncountSystem_CancelPhenomenonIfActorHitCore(EncountSystem *system) {
    RareEncountSpot *spot = &GameData_GetEncountState(system->gameData)->rareSpot;

    if (!spot->active) {
        return FALSE;
    }
    if (IsPositionOccupiedByActorAny(system, spot->x, spot->y, spot->z, TRUE)) {
        EncountSystem_CancelPhenomenonCore(system, system->effectEncountState);
        return TRUE;
    }
    return FALSE;
}


void EncountSystem_CancelPhenomenonCore(EncountSystem *system, EffectEncountState *state) {
    EncountState *encountState = GameData_GetEncountState(system->gameData);

    func_ov021_021aaffc(state);
    MI_CpuFill8(&encountState->rareSpot, 0, sizeof(RareEncountSpot));
}


static void func_ov021_021aafc0(EncountSystem *system, EffectEncountState *state, RareEncountSpot *spot) {
    if (state->effect == NULL) {
        state->effect = func_ov021_021adaac(system, state->fieldEffects, spot->x, spot->z, spot->height, spot->kind);
        if (state->effect == NULL) {
            MI_CpuFill8(spot, 0, sizeof(RareEncountSpot));
        }
    }
}

static void func_ov021_021aaffc(EffectEncountState *state) {
    if (state->effect != NULL) {
        func_ov021_021ac154(state->effect);
        state->effect = NULL;
    }
}

static u16 EffectEncount_GenDustCloudItem(u16 zoneId) {
    u16 roll = GFL_RandomLCAlt(1000);

    if (roll < 100) {
        return data_ov021_021d8e8a[(u16)(GFL_RandomLCAlt(1000) / 100)];
    }
    if (roll < 950) {
        return (u16)(GFL_RandomLCAlt(1700) / 100) + 548;
    }
    return 229;
}

static u16 GetRandomWingItemID(void) {
    u16 roll = GFL_RandomLCAlt(1000);

    if (roll < 900) {
        return (u16)(GFL_RandomLCAlt(600) / 100) + 565;
    }
    return 571;
}

static GameEvent *CallGivePhenomenonItem(EncountSystem *system, u16 itemId) {
    system->effectEncountState->itemId = itemId;
    return EventScriptCall_Create(system->gsys, 0x2795, NULL, Field_GetHeapID(system->field));
}
