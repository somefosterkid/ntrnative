// GameSpy's big-number code (lib/dwc/src/gs/largeint.c)
#ifndef BWPORT_GS_LARGEINT_H
#define BWPORT_GS_LARGEINT_H

#include "types.h"

typedef struct {
    u32 length;
    u32 data[64];
} LargeIntBuf;

// byte-swaps the number in place (called before and after the modular exponentiation); FALSE for an empty one
BOOL func_ov009_0217e934(LargeIntBuf *n);
// modular exponentiation: dest = b ^ p mod m
BOOL func_ov009_0217e078(const LargeIntBuf *b, const LargeIntBuf *p, const LargeIntBuf *m, LargeIntBuf *dest);

#endif
