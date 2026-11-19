#include "global.h"

#include "constants/heap.h"

struct PasswordWordBank;

struct TrainerInfo;

typedef struct TrainerInfo TrainerInfo;

u16 PlayerProfile_GetTrainerID_VisibleHalf(const TrainerInfo *info);

typedef struct PasswordWordBank PasswordWordBank;

PasswordWordBank *WallpaperPasswordBank_Create(enum HeapID heapID);

void WallpaperPasswordBank_Delete(PasswordWordBank *wordBank);

int ov02_0224CAB8(const PasswordWordBank *wordBank, u16 trainerID, u16 word1, u16 word2, u16 word3, u16 word4);

int ov02_0224CD38(const TrainerInfo *trainerInfo, u16 word1, u16 word2, u16 word3, u16 word4, enum HeapID heapID);

int ov02_0224CD38(const TrainerInfo *trainerInfo, u16 word1, u16 word2, u16 word3, u16 word4, enum HeapID heapID) {
    PasswordWordBank *wordBank = WallpaperPasswordBank_Create(heapID);
    u16 trainerID = PlayerProfile_GetTrainerID_VisibleHalf(trainerInfo);
    int result = ov02_0224CAB8(wordBank, trainerID, word1, word2, word3, word4);

    WallpaperPasswordBank_Delete(wordBank);

    return result;
}

int ov02_0224CBF8(const PasswordWordBank *wordBank, u16 trainerID, u16 word1, u16 word2, u16 word3, u16 word4);

int ov02_0224CD74(const TrainerInfo *trainerInfo, u16 word1, u16 word2, u16 word3, u16 word4, enum HeapID heapID);

int ov02_0224CD74(const TrainerInfo *trainerInfo, u16 word1, u16 word2, u16 word3, u16 word4, enum HeapID heapID) {
    PasswordWordBank *wordBank = WallpaperPasswordBank_Create(heapID);
    u16 trainerID = PlayerProfile_GetTrainerID_VisibleHalf(trainerInfo);
    int result = ov02_0224CBF8(wordBank, trainerID, word1, word2, word3, word4);

    WallpaperPasswordBank_Delete(wordBank);

    return result;
}
