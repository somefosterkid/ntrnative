#include "global.h"

#include "sys_task.h"
#include "filesystem.h"
#include "constants/heap.h"

void * NARC_AllocAndReadWholeMember(NARC *narc, u32 file_id, enum HeapID heapID);

void * ov07_0221FEDC(NARC *narc, u32 file_id, enum HeapID heapID);

void * ov07_0221FEDC(NARC *narc, u32 file_id, enum HeapID heapID) {
    return NARC_AllocAndReadWholeMember(narc, file_id, heapID);
}
