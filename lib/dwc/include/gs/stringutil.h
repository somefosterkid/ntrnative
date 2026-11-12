// GameSpy's string helpers (lib/dwc/src/gs/stringutil.c) that other files call
#ifndef BWPORT_GS_STRINGUTIL_H
#define BWPORT_GS_STRINGUTIL_H

#include "types.h"

// a UCS-2 copy of a UTF-8 string, from gsimalloc; NULL when out of memory
u16 *UTF8ToUCS2StringAlloc(s8 *arg0);

#endif
