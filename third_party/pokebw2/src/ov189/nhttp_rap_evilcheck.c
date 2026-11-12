// nhttp_rap_evilcheck.c: signs data for the server's check of Pokémon, with a key and a heap of its own. The name is
// the ROM's own; the functions' names are ours

#include "gfl/nhttp_rap_evilcheck.h"
#include "types.h"
#include "dwc/crypt.h"
#include "gfl/heap.h"
#include "gfl/std.h"

#define HEAPID_NONE 0xffff

static void *NHttpRapEvilCheck_Alloc(u32 size);
static void NHttpRapEvilCheck_Free(void *ptr);

static HeapID sHeapId = HEAPID_NONE;
static u8 sKey[128] = {
#include "asset/nhttp_rap_evilcheck_sKey.inc"
};

int NHttpRapEvilCheck_Sign(const void *data, u32 size, u32 count, void *signature, HeapID heapId) {
    int result;

    sHeapId = heapId;
    Crypt_SetAllocator(NHttpRapEvilCheck_Alloc, NHttpRapEvilCheck_Free, 0);
    result = Crypt_VerifySignature(data, size * count, signature, sKey);
    sHeapId = HEAPID_NONE;
    return result;
}

void *NHttpRapEvilCheck_AllocArray(u32 size, u32 count, HeapID heapId) {
    u32 total = count * size;
    void *ptr = GFL_HeapAllocate(heapId, total, FALSE, "nhttp_rap_evilcheck.c", 0xb9);

    MI_CpuFill8(ptr, 0, total);
    return ptr;
}

void NHttpRapEvilCheck_FreeArray(void *ptr) {
    GFL_HeapFree(ptr);
}

void NHttpRapEvilCheck_SetElement(void *array, const void *element, u32 size, u32 index) {
    MI_CpuCopy8(element, (u8 *)array + size * index, size);
}

static void *NHttpRapEvilCheck_Alloc(u32 size) {
    void *ptr;

    GFL_ASSERT_MSG(sHeapId != HEAPID_NONE,
                   "heapID\x82\xf0\x90\xdd\x92\xe8\x82\xb5\x82\xc4\x82\xad\x82\xbe\x82\xb3\x82\xa2\n");
    ptr = GFL_HeapAllocate(sHeapId, size, FALSE, "nhttp_rap_evilcheck.c", 0xe9);
    GFL_ASSERT_MSG(ptr != NULL, "CRYPT alloc failed!! size=%d =restHeap%d \n", size, GFL_HeapGetFreeSize(sHeapId));
    return ptr;
}

static void NHttpRapEvilCheck_Free(void *ptr) {
    GFL_HeapFree(ptr);
}
