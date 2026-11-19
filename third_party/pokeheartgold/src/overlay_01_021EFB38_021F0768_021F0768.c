#include "global.h"

#include "heap.h"
#include "overlay_01_021EFB38.h"
#include "overlay_01_unk_struct_ov5_021de6bc.h"

void *ov01_021F0768(enum HeapID heapID) {
    UnkStruct_ov5_021DE6BC *v0;

    v0 = Heap_Alloc(heapID, sizeof(UnkStruct_ov5_021DE6BC));
    memset(v0, 0, sizeof(UnkStruct_ov5_021DE6BC));
    return v0;
}
