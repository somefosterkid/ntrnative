// Black's code at itcm 0x01ff87d4-0x01ff8804. Placed by tools/gap_files.py from 1 pokebw2.

#include "types.h"

#include "twl/scfg.h"

#pragma thumb off
asm void func_01ff87d4(SCFGCpuSpeed speed) {
    ldr r2, =0x04004004
    ldrh r1, [r2]
    bic r1, r1, #1
    orr r1, r0, r1
    strh r1, [r2]
    cmp r0, #0
    moveq r0, #10
    movne r0, #20
wait:
    subs r0, r0, #4
    bhs wait
    bx lr
}
#pragma thumb reset

