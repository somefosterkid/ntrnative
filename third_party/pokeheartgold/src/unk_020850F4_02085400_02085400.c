#include "global.h"
#include "constants/heap.h"
#include "options.h"
#include "pm_string.h"

struct Options;

struct String;

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

extern UnkStruct_02089438 *sub_020853B4(enum HeapID heapID, int param1, int param2[], Options *options, u32 param4, u32 param5);

extern UnkStruct_02089438 *sub_0208541C(enum HeapID heapID, int param1, int param2[], Options *options, u32 param4, u32 param5, u32 param6, u32 param7);

extern UnkStruct_02089438 *sub_02085400(enum HeapID heapID, int param1, int param2[], Options *options, u32 param4, u32 param5);

UnkStruct_02089438 *sub_02085400(enum HeapID heapID, int param1, int param2[], Options *options, u32 param4, u32 param5) {
    UnkStruct_02089438 *v0 = sub_020853B4(heapID, param1, param2, options, param4, param5);

    v0->unk_24 = 0;
    v0->unk_28 = 0;
    return v0;
}

UnkStruct_02089438 *sub_0208541C(enum HeapID heapID, int param1, int param2[], Options *options, u32 param4, u32 param5, u32 param6, u32 param7) {
    UnkStruct_02089438 *v0 = sub_020853B4(heapID, param1, param2, options, param4, param5);

    v0->unk_24 = param6;
    v0->unk_28 = param7;
    return v0;
}
