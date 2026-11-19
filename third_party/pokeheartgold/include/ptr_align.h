#ifndef POKEHEARTGOLD_PTR_ALIGN_H
#define POKEHEARTGOLD_PTR_ALIGN_H

#include "global.h"

// Round a pointer up to a multiple of n (a power of two). Goes through size_t, which is pointer
// sized on the DS (32 bit) and on the PC port (64 bit); casting the address to s32/u32 truncates
// it there. The DS code is identical to the old `(s32)(p + n - 1) & ~(n - 1)` form.
#define PTR_ALIGN_UP(p, n) ((void *)(((size_t)(p) + (n) - 1) & ~((size_t)(n) - 1)))

#endif // POKEHEARTGOLD_PTR_ALIGN_H
