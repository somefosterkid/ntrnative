#include "global.h"

#include "comm_cmd_manager.h"
#include "constants/heap.h"



void *Heap_Alloc(u32 heapID, u32 size);

void sub_0203410C(const CommCmdTable *cmdTable, int param1, void *param2);

void sub_0203410C(const CommCmdTable *cmdTable, int param1, void *param2) {
    int v0;

    if (!_021D412C) {
        _021D412C = Heap_Alloc(HEAP_ID_15, sizeof(CommCmdManager));
    }

    _021D412C->cmdTable = cmdTable;
    _021D412C->unk_04 = param1;
    _021D412C->unk_08 = param2;

    for (v0 = 0; v0 < (7 + 1); v0++) {
        _021D412C->unk_0C[v0] = 0;
    }

    _021D412C->unk_14 = 0;
}
