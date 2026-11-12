// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/waza_tool.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "pml/waza.h"
#include "constants/arc.h"
#include "constants/battle.h"
#include "constants/moves.h"
#include "gfl/arc.h"
#include "gfl/heap.h"

// The moves' data, read from ARCID_WAZAINFO one move at a time, or kept in a cache of the moves asked for last while
// a battle runs. The functions' names are swan's

// The accuracy of a move that never misses, and the critical hit stage of one that always lands a critical hit
#define MOVE_ACCURACY_ALWAYS_HIT 101
#define MOVE_CRIT_STAGE_ALWAYS 6

#define MOVE_STAT_CHANGE_COUNT 3
#define MOVE_MAX_PP_UPS 3

// A move's file in ARCID_WAZAINFO
typedef struct {
    u8 type;
    u8 quality;
    u8 category;
    u8 power;
    u8 accuracy;
    u8 pp;
    s8 priority;
    u8 hitMin : 4;
    u8 hitMax : 4;
    u16 condition;
    u8 conditionChance;
    u8 conditionDuration;
    u8 conditionMinTurns;
    u8 conditionMaxTurns;
    u8 critStage;
    u8 flinchChance;
    u16 effect;
    // The share of the damage dealt that the user recovers, or loses if negative
    s8 drain;
    // The share of its HP that the user recovers, or loses if negative
    s8 heal;
    u8 target;
    u8 statChangeStats[MOVE_STAT_CHANGE_COUNT];
    s8 statChangeStages[MOVE_STAT_CHANGE_COUNT];
    u8 statChangeChances[MOVE_STAT_CHANGE_COUNT];
    u32 flags;
} MoveData;

// The cache: the moves whose data is loaded, 0 for a free entry
typedef struct {
    MoveData **data;
    u16 *moves;
    s32 count;
} MoveDataCache;

static s32 PML_MoveGetParamCore(const MoveData *data, u32 param);
static BOOL func_0201bf44(const MoveData *data, u32 flag);

// pokebw2's sMoveDataCache
static MoveDataCache *data_02146530;
// Where a move's data goes without the cache
// pokebw2's sMoveData
static MoveData data_02146534;

// A move's data. A full cache gives its last entry to a move it doesn't have
static inline MoveData *PML_MoveGetData(u16 move) {
    if (data_02146530 != NULL) {
        int i;

        for (i = 0; i < data_02146530->count; i++) {
            if (data_02146530->moves[i] == MOVE_NONE) {
                data_02146530->moves[i] = move;
                GFL_ArcSysRead(data_02146530->data[i], ARCID_WAZAINFO, move);
                break;
            }
            if (move == data_02146530->moves[i]) {
                break;
            }
        }
        if (i == data_02146530->count) {
            i--;
            data_02146530->moves[i] = move;
            GFL_ArcSysRead(data_02146530->data[i], ARCID_WAZAINFO, move);
        }
        return data_02146530->data[i];
    }
    GFL_ArcSysRead(&data_02146534, ARCID_WAZAINFO, move);
    return &data_02146534;
}

s32 PML_MoveGetParam(u16 move, u32 param) {
    return PML_MoveGetParamCore(PML_MoveGetData(move), param);
}

static s32 PML_MoveGetParamCore(const MoveData *data, u32 param) {
    switch (param) {
        case MOVE_PARAM_TYPE:
            return data->type;
        case MOVE_PARAM_QUALITY:
            return data->quality;
        case MOVE_PARAM_CATEGORY:
            return data->category;
        case MOVE_PARAM_POWER:
            return data->power;
        case MOVE_PARAM_ACCURACY:
            if (data->accuracy == MOVE_ACCURACY_ALWAYS_HIT) {
                return 100;
            }
            return data->accuracy;
        case MOVE_PARAM_PP:
            return data->pp;
        case MOVE_PARAM_PRIORITY:
            return data->priority;
        case MOVE_PARAM_CRIT_STAGE:
            if (data->critStage == MOVE_CRIT_STAGE_ALWAYS) {
                return 0;
            }
            return data->critStage;
        case MOVE_PARAM_HIT_MAX:
            return data->hitMax;
        case MOVE_PARAM_HIT_MIN:
            return data->hitMin;
        case MOVE_PARAM_FLINCH_CHANCE:
            return data->flinchChance;
        case MOVE_PARAM_CONDITION:
            return data->condition;
        case MOVE_PARAM_CONDITION_CHANCE:
            return data->conditionChance;
        case MOVE_PARAM_CONDITION_DURATION:
            return data->conditionDuration;
        case MOVE_PARAM_CONDITION_MIN_TURNS:
            return data->conditionMinTurns;
        case MOVE_PARAM_CONDITION_MAX_TURNS:
            return data->conditionMaxTurns;
        case MOVE_PARAM_EFFECT:
            return data->effect;
        // Only the first of each three gets what its number says: the next two read the entries after it
        case MOVE_PARAM_STAT_CHANGE_STAT:
        case MOVE_PARAM_STAT_CHANGE_STAT + 3:
        case MOVE_PARAM_STAT_CHANGE_STAT + 6:
            return data->statChangeStats[param - MOVE_PARAM_STAT_CHANGE_STAT];
        case MOVE_PARAM_STAT_CHANGE_STAGE:
        case MOVE_PARAM_STAT_CHANGE_STAGE + 3:
        case MOVE_PARAM_STAT_CHANGE_STAGE + 6:
            return data->statChangeStages[param - MOVE_PARAM_STAT_CHANGE_STAGE];
        case MOVE_PARAM_STAT_CHANGE_CHANCE:
        case MOVE_PARAM_STAT_CHANGE_CHANCE + 3:
        case MOVE_PARAM_STAT_CHANGE_CHANCE + 6:
            return data->statChangeChances[param - MOVE_PARAM_STAT_CHANGE_CHANCE];
        case MOVE_PARAM_DRAIN:
            if (data->drain > 0) {
                return data->drain;
            }
            return 0;
        case MOVE_PARAM_RECOIL:
            if (data->drain < 0) {
                return -data->drain;
            }
            return 0;
        case MOVE_PARAM_HEAL:
            if (data->heal > 0) {
                return data->heal;
            }
            return 0;
        case MOVE_PARAM_HP_COST:
            if (data->heal < 0) {
                return -data->heal;
            }
            return 0;
        case MOVE_PARAM_TARGET:
            return data->target;
        default:
            return 0;
    }
}

BOOL getMoveFlag(u16 move, u32 flag) {
    return func_0201bf44(PML_MoveGetData(move), flag);
}

// pokebw2's PML_MoveGetFlagCore
static BOOL func_0201bf44(const MoveData *data, u32 flag) {
    return (data->flags & (1 << flag)) != 0;
}

// pokebw2's PML_MoveGetConditionParam
MoveConditionParam func_0201bf58(u16 move) {
    MoveConditionParam param;
    MoveData *data = PML_MoveGetData(move);

    if (data->conditionDuration != 0) {
        param.type = data->conditionDuration;
        param.min = data->conditionMinTurns;
        param.max = data->conditionMaxTurns;
    }
    return param;
}

// pokebw2's PML_MoveGetStatChangeStat
u8 PML_MoveGetStatChangeStat(u16 move) {
    MoveData *data = PML_MoveGetData(move);
    u32 i;
    u32 count = 0;

    for (i = 0; i < MOVE_STAT_CHANGE_COUNT; i++) {
        if (data->statChangeStats[i] != 0) {
            count++;
        }
    }
    return count;
}

u32 PML_MoveGetStatChangeStage(u16 move, u32 index, s32 *stage) {
    MoveData *data = PML_MoveGetData(move);
    u32 stat = data->statChangeStats[index];

    if (stat != 0) {
        *stage = data->statChangeStages[index];
    } else {
        *stage = 0;
    }
    return stat;
}

u8 PML_MoveGetMaxPP(u16 move, u8 bonus) {
    u8 pp;

    if (bonus > MOVE_MAX_PP_UPS) {
        bonus = MOVE_MAX_PP_UPS;
    }
    pp = PML_MoveGetParam(move, MOVE_PARAM_PP);
    pp += pp * 20 * bonus / 100;
    return pp;
}

u32 GetMoveWeather(u16 move) {
    switch (move) {
        case MOVE_RAIN_DANCE:
            return BTL_WEATHER_RAIN;
        case MOVE_SUNNY_DAY:
            return BTL_WEATHER_SUN;
        case MOVE_SANDSTORM:
            return BTL_WEATHER_SANDSTORM;
        case MOVE_HAIL:
            return BTL_WEATHER_HAIL;
        default:
            return 0;
    }
}

s32 PML_MoveGetQuality(u16 move) {
    return PML_MoveGetParam(move, MOVE_PARAM_QUALITY);
}

u8 PML_MoveGetType(u16 move) {
    return PML_MoveGetParam(move, MOVE_PARAM_TYPE);
}

u16 PML_MoveGetBasePower(u16 move) {
    return PML_MoveGetParam(move, MOVE_PARAM_POWER);
}

// pokebw2's PML_MoveGetCategory
s32 PML_MoveGetCategory(u16 move) {
    return PML_MoveGetParam(move, MOVE_PARAM_CATEGORY);
}

BOOL PML_MoveIsAlwaysHit(u16 move) {
    MoveData *data = PML_MoveGetData(move);

    return data->accuracy == MOVE_ACCURACY_ALWAYS_HIT;
}

BOOL PML_MoveIsAlwaysCrit(u16 move) {
    MoveData *data = PML_MoveGetData(move);

    return data->critStage == MOVE_CRIT_STAGE_ALWAYS;
}

BOOL PML_MoveIsDamaging(u16 move) {
    return PML_MoveGetParam(move, MOVE_PARAM_POWER) != 0;
}

void PML_MoveInitDataCache(s32 count, HeapID heapId) {
    int i;

    if (data_02146530 == NULL) {
        data_02146530 = GFL_HeapAllocate(heapId, sizeof(MoveDataCache), FALSE, "waza_tool.c", 577);
        data_02146530->data = GFL_HeapAllocate(heapId, count * sizeof(MoveData *), FALSE, "waza_tool.c", 578);
        data_02146530->moves = GFL_HeapAllocate(heapId, count * sizeof(u16), FALSE, "waza_tool.c", 579);
        data_02146530->count = count;
        for (i = 0; i < count; i++) {
            data_02146530->data[i] = GFL_HeapAllocate(heapId, sizeof(MoveData), FALSE, "waza_tool.c", 584);
            data_02146530->moves[i] = MOVE_NONE;
        }
    }
}

void PML_MoveFreeDataCache(void) {
    int i;

    if (data_02146530 != NULL) {
        for (i = 0; i < data_02146530->count; i++) {
            GFL_HeapFree(data_02146530->data[i]);
        }
        GFL_HeapFree(data_02146530->moves);
        GFL_HeapFree(data_02146530->data);
        GFL_HeapFree(data_02146530);
        data_02146530 = NULL;
    }
}

BOOL PML_MoveIsDataCachePresent(void) {
    return data_02146530 != NULL;
}
