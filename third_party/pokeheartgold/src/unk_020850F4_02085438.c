#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "options.h"
#include "pm_string.h"

typedef struct {
    int unk_00;
    int unk_04[5];
    int unk_18;
    String *unk_1C;
    Options *options;
    int unk_24;
    u32 unk_28;
    u32 unk_2C;
    u32 unk_30;
} UnkStruct_02089438;

void sub_02085438(UnkStruct_02089438 *param0);

void sub_02085438(UnkStruct_02089438 *param0) {
    ((param0->unk_1C != 0L) ? (void)0 : GF_AssertFail());
    ((param0 != 0L) ? (void)0 : GF_AssertFail());

    String_Delete(param0->unk_1C);
    Heap_Free(param0);
}
