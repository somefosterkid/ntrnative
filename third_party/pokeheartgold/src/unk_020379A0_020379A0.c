#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "unk_020379A0.h"
#include "comm_tool.h"


void sub_020379A0(enum HeapID heapID) {
    s32 iVar1;
    s32 iVar2;

    if (_021D414C == NULL) {
        _021D414C = Heap_Alloc(heapID, sizeof(CommTool));
        MI_CpuFill8(_021D414C, 0, sizeof(CommTool));
    }
    iVar1 = 0;
    do {
        _021D414C->syncNo[iVar1] = 0xff;
        iVar2 = iVar1 + 1;
        iVar1 = iVar2;
    } while (iVar2 < 8);
    _021D414C->syncNoPersonal = 0xff;
    _021D414C->sendTiming = 0xff;
    _021D414C->sendTimingPending = 0;
    _021D414C->syncState = 0;
    return;
}
