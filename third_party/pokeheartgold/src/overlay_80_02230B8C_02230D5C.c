#include "global.h"

#include "heap.h"
#include "overlay_80_02230B8C.h"
#include "pokemon_types_def.h"
#include "save.h"

struct BattleHall;
typedef struct BattleHall BattleHall;
void ov80_022314DC(BattleHall *battleHall, void *args);
typedef struct BattleHallAppArgs {
    SaveData *saveData;
    u8 challengeType;
    u8 cursorPos;
    u16 selectedCell;
    u8 *typeRanks;
    Party *party;
    BattleHall *battleHall;
    u16 *partnersLevel;
    u16 currentStreak;
    u8 padding_1A[6];
} BattleHallAppArgs;

void ov80_02230D5C(void *data) {
    BattleHallAppArgs *args = data;
    ov80_022314DC(args->battleHall, data);
    Heap_Free(data);
}
