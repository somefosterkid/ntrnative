#include "global.h"

#include "heap.h"
#include "unk_02096C88.h"
#include "unk_struct_0209_c194.h"

void sub_02097060(UnkStruct_0209C194 *param_1);

void sub_02097060(UnkStruct_0209C194 *param_1) {
    sub_02096CF4(param_1->unk_30);
    sub_02096CC8(param_1->unk_30);
    Heap_Free(param_1);
    return;
}

BOOL sub_02097078(const UnkStruct_0209C194 *param_1);

BOOL sub_02097078(const UnkStruct_0209C194 *param_1) {
    return param_1->unk_10;
}
