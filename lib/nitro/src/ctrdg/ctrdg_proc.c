// Black's code at main 0x020911a0-0x020911d8: the cartridge library's dummy start-up and the callback it sets (pokebw2's
// lib/nitro/src/ctrdg/ctrdg_proc.c)

#include "types.h"
#include "nitro/pxi.h"

void func_02090f18(void);
void func_02091150(u32 param_1);
void func_020911d4(int param_1, uPtr param_2, BOOL param_3);

#pragma thumb off

void func_020911a0(void) {
    func_02090f18();
    PXI_SetFifoRecvCallback(0xd, NULL);
    PXI_SetFifoRecvCallback(0xd, func_020911d4);
    func_02091150(1);
}

void func_020911d4(int param_1, uPtr param_2, BOOL param_3) {
}
