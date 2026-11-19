#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "sprite.h"
#include "error_handling.h"
#include "unk_struct_02013034.h"

UnkStruct_02013034 *sub_02013F78(enum HeapID heapID);

void sub_02013F94(void *arg0);

UnkStruct_02013034 *sub_02013F78(enum HeapID heapID) {
    UnkStruct_02013034 *v0 = Heap_AllocAtEnd(heapID, sizeof(UnkStruct_02013034));
    ((v0) ? (void)0 : GF_AssertFail());

    v0->unk_0C = 0L;
    v0->unk_10 = 0L;

    return v0;
}

void sub_02013F94(void *ptr) {
    if (ptr == NULL) {
        GF_AssertFail();
    }
    Heap_Free(ptr);
}
