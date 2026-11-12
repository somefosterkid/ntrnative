// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/src/gx/gxasm.c, ported by tools/port_file.py.
// NitroSDK libgx's gxasm.c: copies of a matrix to a graphics FIFO port (GX_SendFifo48B for a 4x3 matrix and
// GX_SendFifo64B for a 4x4 one), which the SDK wrote in ARM assembly. The file has no strings; its name is the SDK's,
// from the layout of libgx.
#include "nitro/gx.h"

// clang-format off
// The SDK wrote these in assembly: each stmia without write-back starts again at the FIFO port, whose 64 bytes all reach
// the FIFO. dest is the port, which the geometry engine reads
#ifndef __MWERKS__
// C for a PC build: MWCC, which defines __MWERKS__, compiles the asm under #else. The same words to the same
// addresses in the same order: each three to dest's first three words
void GX_SendFifo48B(const void *src, volatile void *dest) {
    const u32 *s = (const u32 *)src;
    vu32 *d = (vu32 *)dest;
    int i;

    for (i = 0; i < 4; i++, s += 3) {
        d[0] = s[0];
        d[1] = s[1];
        d[2] = s[2];
    }
}
#else
asm void GX_SendFifo48B(const void *src, volatile void *dest) {
    ldmia r0!, {r2, r3, r12}
    stmia r1, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1, {r2, r3, r12}
    bx lr
}
#endif

// The same for a 4x4 matrix, in assembly in the SDK; dest is the port, which the geometry engine reads
#ifndef __MWERKS__
// C for a PC build: each eight words to dest's first eight
void GX_SendFifo64B(const void *src, volatile void *dest) {
    const u32 *s = (const u32 *)src;
    vu32 *d = (vu32 *)dest;
    int i;
    int k;

    for (i = 0; i < 2; i++, s += 8) {
        for (k = 0; k < 8; k++) {
            d[k] = s[k];
        }
    }
}
#else
asm void GX_SendFifo64B(const void *src, volatile void *dest) {
    stmdb sp!, {r4, r5, r6, r7, r8}
    ldmia r0!, {r2, r3, r4, r5, r6, r7, r8, r12}
    stmia r1, {r2, r3, r4, r5, r6, r7, r8, r12}
    ldmia r0!, {r2, r3, r4, r5, r6, r7, r8, r12}
    stmia r1, {r2, r3, r4, r5, r6, r7, r8, r12}
    ldmia sp!, {r4, r5, r6, r7, r8}
    bx lr
}
#endif
// clang-format on
