#include "global.h"

#include "heap.h"
#include "overlay_trainer_card.h"
#include "unk_02068FC8.h"

TrainerCardAppArgs *sub_020691C4(enum HeapID heapID) {
    TrainerCardAppArgs *args = Heap_Alloc(heapID, sizeof(TrainerCardAppArgs));

    memset(args, 0, sizeof(TrainerCardAppArgs));
    return args;
}
