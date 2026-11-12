#ifndef MAIN_02083644_H
#define MAIN_02083644_H

#include "types.h"

// Read, write and verify size bytes of the NVRAM at address, waiting for the ARM7: 0 when done, 1 while another
// call keeps the library busy
u32 func_02083844(u32 address, u32 size, void *buf);
u32 func_02083dc8(u32 address, u32 size, const void *src);
u32 func_020840f8(u32 address, u32 size, const void *src);
void NVRAM_Init(void);

#endif
