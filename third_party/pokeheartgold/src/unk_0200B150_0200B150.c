#include "global.h"

#include "constants/heap.h"

#include "unk_0200B150.h"

void sub_0200B194(int mainOamStart, int mainOamCount, int mainAffineOamStart, int mainAffineOamCount, int subOamStart, int subOamCount, int subAffineOamStart, int subAffineOamCount, enum HeapID heapID);

void OamManager_Create(int mainOamStart, int mainOamCount, int mainAffineOamStart, int mainAffineOamCount, int subOamStart, int subOamCount, int subAffineOamStart, int subAffineOamCount, enum HeapID heapID) {
    int trueMainOamStart;
    int trueMainOamCount;
    int trueMainAffineOamStart;
    int trueMainAffineOamCount;

    if (mainOamStart < 4) {
        trueMainOamStart = 4;

        if (mainOamCount > 128 - 4) {
            trueMainOamCount = mainOamCount - (4 - mainOamStart);
        } else {
            trueMainOamCount = mainOamCount;
        }
    } else {
        trueMainOamStart = mainOamStart;
        trueMainOamCount = mainOamCount;
    }

    if (mainAffineOamStart < 1) {
        trueMainAffineOamStart = 1;

        if (mainAffineOamCount > 32 - 1 - 1) {
            trueMainAffineOamCount = mainAffineOamCount - (1 - mainAffineOamStart);
        } else {
            trueMainAffineOamCount = mainAffineOamCount;
        }
    } else {
        trueMainAffineOamStart = mainAffineOamStart;
        trueMainAffineOamCount = mainAffineOamCount;
    }

    sub_0200B194(trueMainOamStart,
        trueMainOamCount,
        trueMainAffineOamStart,
        trueMainAffineOamCount,
        subOamStart,
        subOamCount,
        subAffineOamStart,
        subAffineOamCount,
        heapID);
}
