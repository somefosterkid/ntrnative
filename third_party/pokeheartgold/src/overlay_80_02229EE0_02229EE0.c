#include "global.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"

struct FrontierTrainerBase;
typedef struct FrontierTrainerBase FrontierTrainerBase;
void *AllocAndReadWholeNarcMemberByIdPair(enum NarcId narcID, int memberIndex, enum HeapID heapID);

FrontierTrainerBase *ov80_02229EE0(u16 battleTowerTrainerID, enum HeapID heapID, enum NarcId narcID);

FrontierTrainerBase *ov80_02229EE0(u16 battleTowerTrainerID, enum HeapID heapID, enum NarcId narcID) {
    return AllocAndReadWholeNarcMemberByIdPair(narcID, battleTowerTrainerID, heapID);
}
