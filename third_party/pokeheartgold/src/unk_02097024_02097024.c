#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "unk_struct_0209_bdf8.h"
#include "unk_struct_0209_c194.h"

UnkStruct_0209BDF8 *sub_02096C88(UnkStruct_0209C194 *, enum HeapID);

UnkStruct_0209C194 *sub_02097024(const UnkStruct_0209C194_1 *param_1, enum HeapID param_2);

UnkStruct_0209C194 *sub_02097024(const UnkStruct_0209C194_1 *param_1, enum HeapID param_2) {
    UnkStruct_0209C194 *pvVar1;

    pvVar1 = Heap_Alloc(param_2, sizeof(UnkStruct_0209C194));
    memset(pvVar1, 0, sizeof(UnkStruct_0209C194));
    pvVar1->unk_14 = *param_1;
    pvVar1->unk_30 = sub_02096C88(pvVar1, param_2);
    return pvVar1;
}
