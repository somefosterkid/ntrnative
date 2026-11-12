// The Black Tower and White Treehollow's dungeon, its tables and its layout: the templates of the 19 kinds of rooms and
// of the NPCs in them, the 640 trainers and the tables by rank, the shuffles of the generator, the seeds, the trainer
// slots of a room, and the maze that digs the passages of each floor and puts the rooms in it. The ROM has no name for
// this file, which is the one after infinity_dungeon.c: named after what it holds
#include "field/infinity_dungeon_rooms.h"
#include "types.h"
#include "constants/version.h"
#include "constants/zones.h"
#include "field/infinity_dungeon.h"
#include "gfl/std.h"
#include "nitro/math.h"

// The grid a floor's rooms are in is 4 rooms wide, and a walk can go back at most this many rooms
#define INFINITY_DUNGEON_GRID_WIDTH 4
#define INFINITY_DUNGEON_WALK_DEPTH 100

typedef struct {
    u32 width;
    u32 height;
} InfinityDungeonSize;

typedef struct {
    u32 objCode;
    u8 scriptIndex;
    u8 npcIndex;
} InfinityDungeonObjScript;

// A room of a floor's grid
typedef struct {
    int x;
    int y;
} InfinityDungeonGridPos;

// A walk through the grid that digs the passages, with the rooms it can go back to
typedef struct {
    int top;
    InfinityDungeonGridPos cur;
    InfinityDungeonGridPos stack[INFINITY_DUNGEON_WALK_DEPTH];
} InfinityDungeonWalk;

static void InfinityDungeon_ShuffleU8(u8 *array, int count, MATHRandContext32 *rand);
static const u8 *InfinityDungeon_GetRoomCounts(u16 rank, u8 floor, u8 version);
static u8 InfinityDungeon_ListRoomTypes(u32 rank, u8 floor, u8 version, u8 *types);
static u8 InfinityDungeon_ListGoalRoomTypes(u32 rank, u8 floor, u8 version, u8 *types);
static int InfinityDungeon_ListGoalCandidates(InfinityDungeonRoom *rooms, int *list, int width, int height);
static InfinityDungeonRoom *InfinityDungeon_GetGridRoom(const InfinityDungeonGridPos *pos, InfinityDungeonRoom *rooms);
static const InfinityDungeonRoom *InfinityDungeon_GetGridRoomConst(const InfinityDungeonGridPos *pos,
                                                                   const InfinityDungeonRoom *rooms);
static void InfinityDungeonWalk_Dig(u8 direction, InfinityDungeonWalk *walk, InfinityDungeonRoom *rooms);
static int InfinityDungeonWalk_GetDirections(const InfinityDungeonGridPos *pos, const InfinityDungeonRoom *rooms,
                                             int *directions, int width, int height, BOOL allowVisited);
static BOOL InfinityDungeon_IsRoomUndug(const InfinityDungeonGridPos *pos, const InfinityDungeonRoom *rooms);
static void InfinityDungeonWalk_Push(InfinityDungeonWalk *walk, const InfinityDungeonGridPos *pos);
static void InfinityDungeonWalk_Pop(InfinityDungeonWalk *walk);
static BOOL InfinityDungeon_AreAllRoomsDug(const InfinityDungeonRoom *rooms, int width, int height);

static const u8 sIvs[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static const u16 sBlack2Zones[6] = { ZONE_BLACK_TOWER_2, ZONE_BLACK_TOWER_3, ZONE_BLACK_TOWER_5,
                                     ZONE_BLACK_TOWER_4, ZONE_BLACK_TOWER_6, ZONE_BLACK_TOWER };

static const u16 sWhite2Zones[6] = { ZONE_WHITE_TREEHOLLOW_2, ZONE_WHITE_TREEHOLLOW_3, ZONE_WHITE_TREEHOLLOW_4,
                                     ZONE_WHITE_TREEHOLLOW_5, ZONE_WHITE_TREEHOLLOW_6, ZONE_WHITE_TREEHOLLOW };

// clang-format off
static const InfinityDungeonLevelRange sLevelRanges[10] = {
#include "asset/infinity_dungeon_rooms_sLevelRanges.inc"
};

// clang-format on

static const InfinityDungeonPos sPos2c20 = { 6, 0, 0 };

static const InfinityDungeonPos sPos2c38[2] = {
    { 7, 0, 2 },
    { 11, 0, 2 },
};

static const InfinityDungeonPos sPos2c2c = { 6, 0, 0 };

// clang-format off
static const InfinityDungeonSize sSizes[10] = {
#include "asset/infinity_dungeon_rooms_sSizes.inc"
};

// clang-format on

// clang-format off
static const InfinityDungeonObjScript sObjScripts[30] = {
#include "asset/infinity_dungeon_rooms_sObjScripts.inc"
};

// clang-format on

static const u8 sTrainerClassSharesWhite2[10][30] = {
#include "asset/infinity_dungeon_rooms_sTrainerClassSharesWhite2.inc"
};

static const u8 sTrainerClassSharesBlack2[10][30] = {
#include "asset/infinity_dungeon_rooms_sTrainerClassSharesBlack2.inc"
};

static const InfinityDungeonPos sPosBlack2B[6][5] = {
#include "asset/infinity_dungeon_rooms_sPosBlack2B.inc"
};

static const InfinityDungeonPos sPosWhite2B[6][5] = {
#include "asset/infinity_dungeon_rooms_sPosWhite2B.inc"
};

static const InfinityDungeonPos sPosBlack2A[6][5] = {
#include "asset/infinity_dungeon_rooms_sPosBlack2A.inc"
};

static const InfinityDungeonPos sPosWhite2A[6][5] = {
#include "asset/infinity_dungeon_rooms_sPosWhite2A.inc"
};

// clang-format off
static const InfinityDungeonNpc sNpcs[21] = {
#include "asset/infinity_dungeon_rooms_sNpcs.inc"
};

// clang-format on

static const u8 sRoomCountsWhite2[10][5][20] = {
#include "asset/infinity_dungeon_rooms_sRoomCountsWhite2.inc"
};

static const u8 sRoomCountsBlack2[10][5][20] = {
#include "asset/infinity_dungeon_rooms_sRoomCountsBlack2.inc"
};

static const InfinityDungeonRoomTemplate sRoomTemplates[19] = {
#include "asset/infinity_dungeon_rooms_sRoomTemplates.inc"
};

// clang-format off
static const InfinityDungeonTrainer sTrainers[640] = {
#include "asset/infinity_dungeon_rooms_sTrainers.inc"
};

// clang-format on

static inline u32 InfinityDungeonRandom_Next(InfinityDungeonRandom *rnd, u32 max) {
    rnd->state = rnd->multiplier * rnd->state + rnd->increment;
    if (max == 0) {
        return rnd->state >> 32;
    }
    return ((rnd->state >> 32) * max) >> 32;
}

const InfinityDungeonNpc *InfinityDungeon_GetNpc(u16 npc) {
    return &sNpcs[npc];
}

const InfinityDungeonRoomTemplate *InfinityDungeon_GetRoomTemplate(u16 type) {
    return &sRoomTemplates[type];
}

const InfinityDungeonTrainer *InfinityDungeon_GetTrainer(u16 trainer) {
    return &sTrainers[trainer];
}

u8 InfinityDungeon_GetTrainerClass(u16 trainer) {
    return sTrainers[trainer].trainerClass;
}

u8 InfinityDungeon_GetRoomNpcCount(u8 type) {
    if (type != INFINITY_DUNGEON_ROOM_TYPE_NONE) {
        return sRoomTemplates[type].npcCount;
    }
    return 0;
}

void InfinityDungeon_GetFloorSize(u32 rank, u32 *width, u32 *height) {
    *width = sSizes[rank].width;
    *height = sSizes[rank].height;
}

const u16 *InfinityDungeon_GetRoomZones(u8 version) {
    if (version == VERSION_BLACK2) {
        return sBlack2Zones;
    }
    return sWhite2Zones;
}

const InfinityDungeonPos *InfinityDungeon_GetFloorPos(u8 version) {
    if (version == VERSION_BLACK2) {
        return &sPos2c2c;
    }
    return &sPos2c20;
}

const InfinityDungeonPos *InfinityDungeon_GetArrivalPos(u32 index) {
    return &sPos2c38[index];
}

const InfinityDungeonPos *InfinityDungeon_GetRoomEntryPos(u8 version, u8 kind, u16 direction) {
    if (version == VERSION_BLACK2) {
        return &sPosBlack2A[kind][direction];
    }
    return &sPosWhite2A[kind][direction];
}

const InfinityDungeonPos *InfinityDungeon_GetTrainerPos(u8 version, u8 kind, u8 index) {
    if (version == VERSION_BLACK2) {
        return &sPosBlack2B[kind][index];
    }
    return &sPosWhite2B[kind][index];
}

const InfinityDungeonLevelRange *InfinityDungeon_GetLevelRange(u16 mode) {
    return &sLevelRanges[mode];
}

u8 InfinityDungeon_GetIvs(u16 mode) {
    return sIvs[mode];
}

const u8 *InfinityDungeon_GetTrainerClassShares(u16 rank, u8 version) {
    if (version == VERSION_BLACK2) {
        return sTrainerClassSharesBlack2[rank];
    }
    return sTrainerClassSharesWhite2[rank];
}

u32 InfinityDungeon_GetNpcScriptIndex(InfinityDungeon *dungeon, u32 objCode, u8 floor, u8 slot) {
    int i;

    if (!InfinityDungeon_IsTargetSlot(dungeon, floor, slot)) {
        for (i = 0; i < 30; i++) {
            if (objCode == sObjScripts[i].objCode) {
                return sObjScripts[i].scriptIndex;
            }
        }
    } else {
        return 7;
    }
    return 0;
}

u8 InfinityDungeon_GetNpcIndex(u32 objCode) {
    int i;

    for (i = 0; i < 30; i++) {
        if (objCode == sObjScripts[i].objCode) {
            return sObjScripts[i].npcIndex;
        }
    }
    return 0;
}

void InfinityDungeon_ShuffleU32(u32 *array, int count, InfinityDungeonRandom *rnd) {
    int i;

    for (i = 0; i < count; i++) {
        u32 j = InfinityDungeonRandom_Next(rnd, count);
        u32 temp = array[j];

        array[j] = array[i];
        array[i] = temp;
    }
}

static void InfinityDungeon_ShuffleU8(u8 *array, int count, MATHRandContext32 *rand) {
    int i;

    for (i = 0; i < count; i++) {
        u32 j = MATH_Rand32(rand, count);
        u8 temp = array[j];

        array[j] = array[i];
        array[i] = temp;
    }
}

void InfinityDungeon_ShuffleU16(u16 *array, int count, InfinityDungeonRandom *rnd) {
    int i;

    for (i = 0; i < count; i++) {
        u32 j = InfinityDungeonRandom_Next(rnd, count);
        u16 temp = array[j];

        array[j] = array[i];
        array[i] = temp;
    }
}

void InfinityDungeon_Seed(InfinityDungeon *dungeon, s32 seed) {
    MATHRandContext32 rand;
    int floor;
    int i;

    dungeon->seed = seed;
    MATH_InitRand32(&rand, seed);
    dungeon->trainerSeed = MATH_Rand32(&rand, 0);
    dungeon->layoutSeed = MATH_Rand32(&rand, 0);
    dungeon->rewardSeed = MATH_Rand32(&rand, 0);
    for (floor = 0; floor < INFINITY_DUNGEON_FLOOR_COUNT; floor++) {
        for (i = 0; i < INFINITY_DUNGEON_FLOOR_TRAINER_MAX; i++) {
            dungeon->battleSeeds[floor][i] = MATH_Rand32(&rand, 0);
        }
    }
}

static const u8 *InfinityDungeon_GetRoomCounts(u16 rank, u8 floor, u8 version) {
    if (version == VERSION_BLACK2) {
        return sRoomCountsBlack2[rank][floor];
    }
    return sRoomCountsWhite2[rank][floor];
}

u16 InfinityDungeon_IsTargetSlot(InfinityDungeon *dungeon, u8 floor, u8 slot) {
    if (dungeon->targetSlot == slot && dungeon->targetFloor == floor) {
        return 1;
    }
    return 0;
}

u8 InfinityDungeon_GetVersion(InfinityDungeon *dungeon) {
    return dungeon->version;
}

u32 InfinityDungeon_GetSlotRoom(InfinityDungeon *dungeon, u32 floor, int slot) {
    int i;
    int total = 0;

    for (i = 0; i < INFINITY_DUNGEON_ROOM_COUNT; i++) {
        u8 count = InfinityDungeon_GetRoomNpcCount(dungeon->floors[floor].rooms[i].type);

        if (count != 0) {
            total += count;
            if (slot < total) {
                return i;
            }
        }
    }
    return 0;
}

u32 InfinityDungeon_GetTrainerSlot(InfinityDungeon *dungeon, unsigned int floor, unsigned int room, u8 trainer) {
    int total = 0;
    int i;

    for (i = 0; i < INFINITY_DUNGEON_ROOM_COUNT; i++) {
        u32 type = dungeon->floors[floor].rooms[i].type;

        if (room <= i) {
            break;
        }
        if (InfinityDungeon_GetRoomNpcCount(type) != 0) {
            total += InfinityDungeon_GetRoomNpcCount(type);
        }
    }
    return total + trainer;
}

u16 InfinityDungeon_GetTrainerId(InfinityDungeon *dungeon, u8 floor, u8 slot) {
    return dungeon->trainerIds[floor][slot];
}

void InfinityDungeon_LayoutFloors(InfinityDungeon *dungeon, u32 rank, u32 seed, int width, int height, u8 floorCount) {
    InfinityDungeonWalk walk;
    InfinityDungeonWalk extraWalk;
    InfinityDungeonGridPos starts[INFINITY_DUNGEON_FLOOR_COUNT];
    MATHRandContext32 rand;
    int rooms[INFINITY_DUNGEON_ROOM_COUNT];
    int directions[4];
    int extraDirections[4];
    u8 types[INFINITY_DUNGEON_ROOM_COUNT];
    u8 goalTypes[INFINITY_DUNGEON_ROOM_COUNT];
    InfinityDungeonGridPos goal;
    InfinityDungeonGridPos pos;
    int goalFloor;
    u8 goalRoom;
    int count;
    int floor;
    int kind;
    int i;
    int j;

    MATH_InitRand32(&rand, seed);
    for (floor = 0; floor < INFINITY_DUNGEON_FLOOR_COUNT; floor++) {
        for (i = 0; i < INFINITY_DUNGEON_ROOM_COUNT; i++) {
            dungeon->floors[floor].rooms[i].index = i;
            dungeon->floors[floor].rooms[i].kind = 4;
            dungeon->floors[floor].rooms[i].type = INFINITY_DUNGEON_ROOM_TYPE_NONE;
            for (j = 0; j < 4; j++) {
                dungeon->floors[floor].rooms[i].open[j] = 0;
            }
        }
    }
    for (floor = 0; floor < INFINITY_DUNGEON_FLOOR_COUNT; floor++) {
        InfinityDungeonRoom *first = dungeon->floors[floor].rooms;

        first->kind = 0;
        first->type = INFINITY_DUNGEON_ROOM_TYPE_NONE;
        starts[floor].x = 0;
        starts[floor].y = 0;
    }

    // The floor of the goal, and its room among those that have another below them
    goalFloor = MATH_Rand32(&rand, floorCount);
    {
        InfinityDungeonRoom *goalRooms = dungeon->floors[goalFloor].rooms;
        InfinityDungeonRoom *room;

        count = InfinityDungeon_ListGoalCandidates(goalRooms, rooms, width, height);
        goalRoom = rooms[MATH_Rand32(&rand, count)];
        room = &goalRooms[goalRoom];
        room->kind = 1;
        room->type = INFINITY_DUNGEON_ROOM_TYPE_NONE;
    }
    goal.x = goalRoom % INFINITY_DUNGEON_GRID_WIDTH;
    goal.y = goalRoom / INFINITY_DUNGEON_GRID_WIDTH;

    for (floor = 0; floor < floorCount; floor++) {
        InfinityDungeonRoom *floorRooms = dungeon->floors[floor].rooms;

        walk.cur = starts[floor];
        walk.top = -1;
        for (i = 0; i < INFINITY_DUNGEON_WALK_DEPTH; i++) {
            count = InfinityDungeonWalk_GetDirections(&walk.cur, floorRooms, directions, width, height, FALSE);
            if (count > 0) {
                InfinityDungeonWalk_Dig(directions[(u8)MATH_Rand32(&rand, count)], &walk, floorRooms);
            } else {
                InfinityDungeonWalk_Pop(&walk);
                if (InfinityDungeon_AreAllRoomsDug(floorRooms, width, height) == TRUE) {
                    break;
                }
            }
        }
        if (goalFloor == floor) {
            walk.cur = goal;
            InfinityDungeonWalk_Dig(1, &walk, floorRooms);
        }

        extraWalk.top = -1;
        for (pos.y = 0; pos.y < height; pos.y++) {
            for (pos.x = 0; pos.x < width; pos.x++) {
                extraWalk.cur = pos;
                kind = InfinityDungeon_GetGridRoom(&pos, floorRooms)->kind;
                if (kind != 0 && kind != 1) {
                    count = InfinityDungeonWalk_GetDirections(&pos, floorRooms, extraDirections, width, height, TRUE);
                    InfinityDungeonWalk_Dig(extraDirections[(u8)MATH_Rand32(&rand, count)], &extraWalk, floorRooms);
                }
            }
        }
    }

    for (i = 0; i < floorCount; i++) {
        int placed = 0;

        count = InfinityDungeon_ListRoomTypes(rank, i, dungeon->version, types);
        InfinityDungeon_ShuffleU8(types, count, &rand);
        for (j = 0; j < INFINITY_DUNGEON_ROOM_COUNT; j++) {
            if (dungeon->floors[i].rooms[j].kind == 4) {
                int x = j % INFINITY_DUNGEON_GRID_WIDTH;
                int y = j / INFINITY_DUNGEON_GRID_WIDTH;

                if (x < width && y < height) {
                    if (goal.x == x && goal.y + 1 == y && i == goalFloor) {
                        const InfinityDungeonRoomTemplate *roomTemplate;
                        u8 type;

                        count = InfinityDungeon_ListGoalRoomTypes(rank, i, dungeon->version, goalTypes);
                        type = goalTypes[MATH_Rand32(&rand, count)];
                        roomTemplate = InfinityDungeon_GetRoomTemplate(type);
                        dungeon->floors[i].rooms[j].type = type;
                        dungeon->floors[i].rooms[j].kind = roomTemplate->kind;
                    } else {
                        u8 type = types[placed];

                        if (type != INFINITY_DUNGEON_ROOM_TYPE_NONE) {
                            const InfinityDungeonRoomTemplate *roomTemplate = InfinityDungeon_GetRoomTemplate(type);

                            dungeon->floors[i].rooms[j].type = type;
                            dungeon->floors[i].rooms[j].kind = roomTemplate->kind;
                        }
                        placed++;
                    }
                }
            }
        }
    }
}

static u8 InfinityDungeon_ListRoomTypes(u32 rank, u8 floor, u8 version, u8 *types) {
    int count = 0;
    const u8 *counts = InfinityDungeon_GetRoomCounts(rank, floor, version);
    int i;

    MI_CpuFill8(types, INFINITY_DUNGEON_ROOM_TYPE_NONE, INFINITY_DUNGEON_ROOM_COUNT);
    for (i = 0; i < 20; i++) {
        u8 n = counts[i];

        if (n != 0) {
            do {
                types[count++] = i;
            } while (--n != 0);
        }
    }
    return count;
}

static u8 InfinityDungeon_ListGoalRoomTypes(u32 rank, u8 floor, u8 version, u8 *types) {
    int count = 0;
    const u8 *counts = InfinityDungeon_GetRoomCounts(rank, floor, version);
    int i;

    MI_CpuFill8(types, INFINITY_DUNGEON_ROOM_TYPE_NONE, INFINITY_DUNGEON_ROOM_COUNT);
    for (i = 0; i < INFINITY_DUNGEON_ROOM_TYPE_NONE; i++) {
        if (counts[i] != 0 && InfinityDungeon_GetRoomTemplate(i)->kind == 4) {
            types[count++] = i;
        }
    }
    return count;
}

// The rooms of the floor but its last row that have been set apart, by their number
static int InfinityDungeon_ListGoalCandidates(InfinityDungeonRoom *rooms, int *list, int width, int height) {
    int count = 0;
    int x;
    int y;
    InfinityDungeonGridPos pos;

    MI_CpuFill8(list, 16, 16 * sizeof(int));
    for (y = 0; y < height - 1; y++) {
        for (x = 0; x < width; x++) {
            pos.x = x;
            pos.y = y;
            if (InfinityDungeon_GetGridRoom(&pos, rooms)->kind != 0) {
                list[count] = x + y * INFINITY_DUNGEON_GRID_WIDTH;
                count++;
            }
        }
    }
    return count;
}

static InfinityDungeonRoom *InfinityDungeon_GetGridRoom(const InfinityDungeonGridPos *pos, InfinityDungeonRoom *rooms) {
    return &rooms[pos->x + pos->y * INFINITY_DUNGEON_GRID_WIDTH];
}

static const InfinityDungeonRoom *InfinityDungeon_GetGridRoomConst(const InfinityDungeonGridPos *pos,
                                                                   const InfinityDungeonRoom *rooms) {
    return &rooms[pos->x + pos->y * INFINITY_DUNGEON_GRID_WIDTH];
}

// Opens a passage from the walk's room in the direction (up, down, left or right) and moves into the room beyond
static void InfinityDungeonWalk_Dig(u8 direction, InfinityDungeonWalk *walk, InfinityDungeonRoom *rooms) {
    switch (direction) {
    case 0:
        InfinityDungeon_GetGridRoom(&walk->cur, rooms)->open[0] = 1;
        InfinityDungeonWalk_Push(walk, &walk->cur);
        walk->cur.y--;
        InfinityDungeon_GetGridRoom(&walk->cur, rooms)->open[1] = 1;
        break;
    case 1:
        InfinityDungeon_GetGridRoom(&walk->cur, rooms)->open[1] = 1;
        InfinityDungeonWalk_Push(walk, &walk->cur);
        walk->cur.y++;
        InfinityDungeon_GetGridRoom(&walk->cur, rooms)->open[0] = 1;
        break;
    case 3:
        InfinityDungeon_GetGridRoom(&walk->cur, rooms)->open[3] = 1;
        InfinityDungeonWalk_Push(walk, &walk->cur);
        walk->cur.x--;
        InfinityDungeon_GetGridRoom(&walk->cur, rooms)->open[2] = 1;
        break;
    case 2:
        InfinityDungeon_GetGridRoom(&walk->cur, rooms)->open[2] = 1;
        InfinityDungeonWalk_Push(walk, &walk->cur);
        walk->cur.x++;
        InfinityDungeon_GetGridRoom(&walk->cur, rooms)->open[3] = 1;
        break;
    }
}

// The directions that the walk can dig into from a room, which are rooms nobody has dug into yet unless allowVisited
static int InfinityDungeonWalk_GetDirections(const InfinityDungeonGridPos *pos, const InfinityDungeonRoom *rooms,
                                             int *directions, int width, int height, BOOL allowVisited) {
    int count = 0;
    InfinityDungeonGridPos east;
    InfinityDungeonGridPos west;
    InfinityDungeonGridPos south;
    InfinityDungeonGridPos north;
    u8 kind;

    MI_CpuFill8(directions, 4, 4 * sizeof(int));
    east = *pos;
    east.x++;
    if (east.x < width && InfinityDungeon_GetGridRoomConst(&east, rooms)->kind != 1) {
        if (allowVisited == 0) {
            if (InfinityDungeon_IsRoomUndug(&east, rooms)) {
                directions[count] = 2;
                count++;
            }
        } else {
            directions[count] = 2;
            count++;
        }
    }
    west = *pos;
    west.x--;
    if (west.x >= 0 && InfinityDungeon_GetGridRoomConst(&west, rooms)->kind != 1) {
        if (allowVisited == 0) {
            if (InfinityDungeon_IsRoomUndug(&west, rooms)) {
                directions[count] = 3;
                count++;
            }
        } else {
            directions[count] = 3;
            count++;
        }
    }
    south = *pos;
    south.y++;
    if (south.y < height) {
        kind = InfinityDungeon_GetGridRoomConst(&south, rooms)->kind;
        if (kind != 1 && kind != 0) {
            if (allowVisited == 0) {
                if (InfinityDungeon_IsRoomUndug(&south, rooms)) {
                    directions[count] = 1;
                    count++;
                }
            } else {
                directions[count] = 1;
                count++;
            }
        }
    }
    north = *pos;
    if (InfinityDungeon_GetGridRoomConst(&north, rooms)->kind != 0) {
        north.y--;
        if (InfinityDungeon_GetGridRoomConst(&north, rooms)->kind != 1 && north.y >= 0) {
            if (allowVisited == 0) {
                if (InfinityDungeon_IsRoomUndug(&north, rooms)) {
                    directions[count] = 0;
                    count++;
                }
            } else {
                directions[count] = 0;
                count++;
            }
        }
    }
    return count;
}

// Whether no passage has been opened into or out of the room
static BOOL InfinityDungeon_IsRoomUndug(const InfinityDungeonGridPos *pos, const InfinityDungeonRoom *rooms) {
    const InfinityDungeonRoom *room = InfinityDungeon_GetGridRoomConst(pos, rooms);
    int i;

    for (i = 0; i < 4; i++) {
        if (room->open[i] == 1) {
            return FALSE;
        }
    }
    return TRUE;
}

static void InfinityDungeonWalk_Push(InfinityDungeonWalk *walk, const InfinityDungeonGridPos *pos) {
    walk->top++;
    walk->stack[walk->top] = *pos;
}

static void InfinityDungeonWalk_Pop(InfinityDungeonWalk *walk) {
    if (walk->top >= 0) {
        walk->cur = walk->stack[walk->top];
        walk->stack[walk->top].x = 0;
        walk->stack[walk->top].y = 0;
        walk->top--;
    }
}

// Whether every room that isn't the goal has been dug into
static BOOL InfinityDungeon_AreAllRoomsDug(const InfinityDungeonRoom *rooms, int width, int height) {
    int x;
    int y;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            InfinityDungeonGridPos pos = { x, y };

            if (InfinityDungeon_GetGridRoomConst(&pos, rooms)->kind != 1 &&
                InfinityDungeon_IsRoomUndug(&pos, rooms) == TRUE) {
                return FALSE;
            }
        }
    }
    return TRUE;
}
