#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "unk_0205C924.h"

void sub_0205B380(UnkStruct_0205C924 *);

static void sub_0205B3A0(UnkStruct_0205C95C *param_1);

static void sub_0205B3A0(UnkStruct_0205C95C *param_1) {
    int i;

    for (i = 0; i < 30; i++) {
        sub_0205B380(&param_1->unk_00[i]);
    }
}

void sub_0205B35C(UnkStruct_0205C95C *param0);

UnkStruct_0205C95C *sub_0205B3B8(enum HeapID param_1);

UnkStruct_0205C95C *sub_0205B3B8(enum HeapID param_1) {
    UnkStruct_0205C95C *log;

    log = Heap_Alloc(param_1, sizeof(UnkStruct_0205C95C));
    sub_0205B35C(log);
    return log;
}

void sub_0205B3CC(UnkStruct_0205C95C *param_1);

void sub_0205B3CC(UnkStruct_0205C95C *param_1) {
    sub_0205B3A0(param_1);
    Heap_Free(param_1);
}
