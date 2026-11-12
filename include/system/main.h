// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/main.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_SYSTEM_MAIN_H
#define BWPORT_SYSTEM_MAIN_H

#include "types.h"
#include "nitro/os.h"

void main(void);
void func_020053ec(void);

// nitro/mi.h's once main_0207e3d0.c's declaration agrees: NitroSDK's MI_WaitDma
void MI_WaitDma(u32 dma);

#endif // BWPORT_SYSTEM_MAIN_H
