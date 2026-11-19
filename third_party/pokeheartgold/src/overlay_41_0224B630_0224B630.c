#include "global.h"

#include "heap.h"
#include "pokemon.h"
#include "overlay_41_unk_struct_ov22_0225b1bc.h"
#include "overlay_41_unk_struct_ov22_0225b4e4.h"

void ov41_02246544(UnkStruct_ov22_0225A0E4_Head *param0, struct BgConfig *param1, enum HeapID heapID);

void ov41_022499B4(UnkStruct_ov22_02259484 *param0, int param1, enum HeapID heapID);

void ov41_0224B4E8(UnkStruct_ov22_0225AF68 *param0, const UnkStruct_ov22_02257964 *param1, enum HeapID heapID);

void ov41_0224B6CC(UnkStruct_ov22_0225B1BC *param0, const UnkStruct_ov22_0225B4E4 *param1);

void ov41_0224B780(UnkStruct_ov22_0225B1BC *param0, const UnkStruct_ov22_0225B4E4 *param1);

void ov41_0224B848(UnkStruct_ov22_0225B1BC *param0, const UnkStruct_ov22_0225B4E4 *param1);

void ov41_0224B888(UnkStruct_ov22_0225B1BC *param0, const UnkStruct_ov22_0225B4E4 *param1);

void *sub_0202BEF4(void *, Pokemon *);

UnkStruct_ov22_0225B1BC *ov41_0224B630(const UnkStruct_ov22_0225B4E4 *param0);

UnkStruct_ov22_0225B1BC *ov41_0224B630(const UnkStruct_ov22_0225B4E4 *param0) {
    UnkStruct_ov22_0225B1BC *v0;

    v0 = Heap_Alloc(param0->heapID, sizeof(UnkStruct_ov22_0225B1BC));
    memset(v0, 0, sizeof(UnkStruct_ov22_0225B1BC));
    v0->heapID = param0->heapID;
    v0->unk_234 = AllocMonZeroed(param0->heapID);
    sub_0202BEF4(param0->photoMon, v0->unk_234);
    ov41_0224B6CC(v0, param0);
    ov41_02246544(&v0->unk_00, param0->unk_58, param0->heapID);
    ov41_022499B4(&v0->unk_184, 21, param0->heapID);
    v0->unk_18C = ov41_02245EA0(20, param0->heapID);
    ov41_0224B780(v0, param0);
    ov41_0224B848(v0, param0);
    ov41_0224B4E8(&v0->unk_218, &v0->unk_190, param0->heapID);
    v0->unk_260 = 1;
    v0->unk_264 = 1;
    ov41_0224B888(v0, param0);
    return v0;
}
