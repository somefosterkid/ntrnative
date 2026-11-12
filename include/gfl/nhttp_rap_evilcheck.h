// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/nhttp_rap_evilcheck.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_GFL_NHTTP_RAP_EVILCHECK_H
#define BWPORT_GFL_NHTTP_RAP_EVILCHECK_H

#include "types.h"
#include "gfl/heap.h"

// pokebw2's NHttpRapEvilCheck_Sign
// Signs count elements of size bytes with the key of the server's check, in a heap that the signing allocates from
int func_ov114_021b9a1c(const void *data, u32 size, u32 count, void *signature, HeapID heapId);
// pokebw2's NHttpRapEvilCheck_AllocArray
// Allocates count elements of size bytes in the heap, cleared
void *func_ov114_021b9a64(u32 size, u32 count, HeapID heapId);
// pokebw2's NHttpRapEvilCheck_SetElement
// Copies the element of size bytes to an index of the array
void func_ov114_021b9a94(void *array, const void *element, u32 size, u32 index);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's NHttpRapEvilCheck_FreeArray
void func_ov114_021b9a8c(void *ptr);

#endif // BWPORT_GFL_NHTTP_RAP_EVILCHECK_H
