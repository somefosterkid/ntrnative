#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "heap.h"
#include "unk_02096C88.h"
#include "unk_struct_0209_bdf8.h"
#include "unk_struct_0209_c194.h"

UnkStruct_0209BDF8 *sub_02096C88(UnkStruct_0209C194 *param_1, enum HeapID param_2);

UnkStruct_0209BDF8 *sub_02096C88(UnkStruct_0209C194 *param_1, enum HeapID param_2) {
    UnkStruct_0209BDF8 *puVar1;

    puVar1 = Heap_Alloc(param_2, sizeof(UnkStruct_0209BDF8));
    if (puVar1 == NULL) {
        GF_AssertFail();
    }
    memset(puVar1, 0, sizeof(UnkStruct_0209BDF8));
    puVar1->unk_00 = param_1;
    puVar1->unk_4C = Heap_Alloc(param_2, 0x1bd0);
    puVar1->unk_50 = Heap_Alloc(param_2, 0x1bd0);
    return puVar1;
}

void sub_02096CC8(UnkStruct_0209BDF8 *param_1) {
    Heap_Free(param_1->unk_4C);
    Heap_Free(param_1->unk_50);
    Heap_Free(param_1);
    return;
}
