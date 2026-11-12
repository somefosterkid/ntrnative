// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/pml/waza.h, the lines in use so far.
#ifndef BWPORT_PML_WAZA_H
#define BWPORT_PML_WAZA_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Move data. Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

u8 PML_MoveGetMaxPP(u16 move, u8 bonus);

#define MOVE_PARAM_PP 5

// What PML_MoveGetParam reads. Type, category, power and effect are swan's names
#define MOVE_PARAM_TYPE 0

#define MOVE_PARAM_QUALITY 1
#define MOVE_PARAM_CATEGORY 2
#define MOVE_PARAM_POWER 3

// 100 for a move that never misses
#define MOVE_PARAM_ACCURACY 4

#define MOVE_PARAM_PRIORITY 6

// 0 for a move that always lands a critical hit
#define MOVE_PARAM_CRIT_STAGE 7

#define MOVE_PARAM_HIT_MAX 8
#define MOVE_PARAM_HIT_MIN 9
#define MOVE_PARAM_FLINCH_CHANCE 10

// The condition the move inflicts, its chance, and how long it lasts
#define MOVE_PARAM_CONDITION 11

#define MOVE_PARAM_CONDITION_CHANCE 12
#define MOVE_PARAM_CONDITION_DURATION 13
#define MOVE_PARAM_CONDITION_MIN_TURNS 14
#define MOVE_PARAM_CONDITION_MAX_TURNS 15

// The first stat the move changes, by how many stages, and its chance. 19 to 24 are meant for the second and third,
// but read the wrong entries
#define MOVE_PARAM_STAT_CHANGE_STAT 16

#define MOVE_PARAM_STAT_CHANGE_STAGE 17
#define MOVE_PARAM_STAT_CHANGE_CHANCE 18

// The share of the damage that the user recovers, and the share of its HP
#define MOVE_PARAM_DRAIN 25

#define MOVE_PARAM_HEAL 26
#define MOVE_PARAM_TARGET 27
#define MOVE_PARAM_EFFECT 28

// The share of the damage that the user takes, and the share of its HP that it loses
#define MOVE_PARAM_RECOIL 30

#define MOVE_PARAM_HP_COST 31

// How long the condition a move inflicts lasts, by its type
struct MoveConditionParam {
    u16 type : 4;
    u16 min : 6;
    u16 max : 6;
};

BOOL PML_MoveIsAlwaysCrit(u16 move);
u32 GetMoveWeather(u16 move);
void PML_MoveInitDataCache(s32 count, HeapID heapId);
// pokebw2's PML_MoveGetConditionParam
MoveConditionParam func_0201bf58(u16 move);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u16 PML_MoveGetBasePower(u16 move);
BOOL getMoveFlag(u16 move, u32 flag);
BOOL PML_MoveIsDamaging(u16 move);
s32 PML_MoveGetCategory(u16 move);
s32 PML_MoveGetQuality(u16 move);
u8 PML_MoveGetType(u16 move);
u8 PML_MoveGetStatChangeStat(u16 move);
u32 PML_MoveGetStatChangeStage(u16 move, u32 index, s32 *stage);
s32 PML_MoveGetParam(u16 move, u32 param);
BOOL PML_MoveIsAlwaysHit(u16 move);
BOOL PML_MoveIsDataCachePresent(void);
void PML_MoveFreeDataCache(void);

#endif // BWPORT_PML_WAZA_H
