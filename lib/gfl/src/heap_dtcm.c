// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/heap_dtcm.c, which is the same code in
// Black 2/White 2. The data keeps its dsd name; pokebw2 calls it sDTCMHeap.
#include "types.h"
#include "gfl/heapsys.h"
#include "nitro/os.h"

// A heap of DTCM, an OS heap in the DTCM arena. The name is a guess, from GFL_HeapDTCMInit

#define DTCM_HEAP_MAX_SIZE 0x2f80
// Where the heap's arena ends in DTCM: the last 0x1000 bytes are left to the stacks
#define DTCM_ARENA_HI_OFFSET 0x3000

typedef struct {
    u32 unk0;
    int handle;
    u32 unk8;
} DTCMHeap;

static DTCMHeap data_02146964;

BOOL GFL_HeapDTCMInitCore(u32 size) {
    void *start;
    void *lo;
    BOOL result;

    if (size > DTCM_HEAP_MAX_SIZE) {
        return FALSE;
    }
    start = OS_GetArenaLo(OS_ARENA_DTCM);
    OS_SetArenaHi(OS_ARENA_DTCM, (u8 *)SDK_AUTOLOAD_DTCM_START + DTCM_ARENA_HI_OFFSET);
    result = TRUE;
    lo = OS_InitAlloc(OS_ARENA_DTCM, start, (u8 *)start + size, 1);
    OS_SetArenaLo(OS_ARENA_DTCM, lo);
    data_02146964.handle = OS_CreateHeap(OS_ARENA_DTCM, lo, (u8 *)start + size);
    if (data_02146964.handle == OS_HEAP_INVALID) {
        result = FALSE;
    }
    return result;
}

void *GFL_HeapDTCMAllocateCore(u32 size) {
    return OS_AllocFromHeap(OS_ARENA_DTCM, data_02146964.handle, size);
}

BOOL freeBlkFromDTCM(void *ptr) {
    OS_FreeToHeap(OS_ARENA_DTCM, data_02146964.handle, ptr);
    return TRUE;
}
