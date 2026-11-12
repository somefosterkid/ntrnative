// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov189/nhttp_rap_evilcheck.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// nhttp_rap_evilcheck.c: signs data for the server's check of Pokémon, with a key and a heap of its own. The name is
// the ROM's own; the functions' names are ours

#include "gfl/nhttp_rap_evilcheck.h"
#include "types.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "dwc/crypt.h"

#define HEAPID_NONE 0xffff

static void *func_ov114_021b9aa8(u32 size);
static void func_ov114_021b9af8(void *ptr);

static HeapID data_ov114_021ce31c = HEAPID_NONE;
static u8 data_ov114_021ce320[128] = {
#include "asset/nhttp_rap_evilcheck_data_ov114_021ce320.inc"
};

// pokebw2's NHttpRapEvilCheck_Sign
int func_ov114_021b9a1c(const void *data, u32 size, u32 count, void *signature, HeapID heapId) {
    int result;

    data_ov114_021ce31c = heapId;
    func_ov114_021cab1c(func_ov114_021b9aa8, func_ov114_021b9af8, 0);
    result = func_ov114_021caa70(data, size * count, signature, data_ov114_021ce320);
    data_ov114_021ce31c = HEAPID_NONE;
    return result;
}

// pokebw2's NHttpRapEvilCheck_AllocArray
void *func_ov114_021b9a64(u32 size, u32 count, HeapID heapId) {
    u32 total = count * size;
    void *ptr = GFL_HeapAllocate(heapId, total, FALSE, "nhttp_rap_evilcheck.c", 0xb9);

    MI_CpuFill8(ptr, 0, total);
    return ptr;
}

// pokebw2's NHttpRapEvilCheck_FreeArray
void func_ov114_021b9a8c(void *ptr) {
    GFL_HeapFree(ptr);
}

// pokebw2's NHttpRapEvilCheck_SetElement
void func_ov114_021b9a94(void *array, const void *element, u32 size, u32 index) {
    MI_CpuCopy8(element, (u8 *)array + size * index, size);
}

// pokebw2's NHttpRapEvilCheck_Alloc
static void *func_ov114_021b9aa8(u32 size) {
    void *ptr;

    GFL_ASSERT(data_ov114_021ce31c != HEAPID_NONE);
    ptr = GFL_HeapAllocate(data_ov114_021ce31c, size, FALSE, "nhttp_rap_evilcheck.c", 0xe9);
    GFL_ASSERT(ptr != NULL);
    return ptr;
}

// pokebw2's NHttpRapEvilCheck_Free
static void func_ov114_021b9af8(void *ptr) {
    GFL_HeapFree(ptr);
}
