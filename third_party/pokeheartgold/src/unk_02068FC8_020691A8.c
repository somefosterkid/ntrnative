#include "global.h"
#include "trainer_case.h"
#include "igt.h"

struct IGT;
struct TrainerCase;
struct TrainerCaseBadge;
enum Badge {
    BADGE_ID_COAL = 0,
    BADGE_ID_FOREST = 1,
    BADGE_ID_COBBLE = 2,
    BADGE_ID_FEN = 3,
    BADGE_ID_RELIC = 4,
    BADGE_ID_MINE = 5,
    BADGE_ID_ICICLE = 6,
    BADGE_ID_BEACON = 7,
    MAX_BADGES = 8,
};
void *Heap_Alloc(u32 heapID, u32 size);

TrainerCase *sub_020691A8(u16 heapID);

TrainerCase *sub_020691A8(u16 heapID) {
    TrainerCase *trainerCase = Heap_Alloc(heapID, sizeof(TrainerCase));

    memset(trainerCase, 0, sizeof(TrainerCase));
    return trainerCase;
}
