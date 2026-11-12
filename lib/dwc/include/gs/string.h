#ifndef GS_STRING_H
#define GS_STRING_H

#include <stddef.h>

// GameSpy's sources take memset from a header named string.h: MWCC expands a memset of a known size in place only
// when its first declaration comes from one, and Black's gpiBuddy.c has those expansions
void *memset(void *dest, int val, size_t n);

#endif
