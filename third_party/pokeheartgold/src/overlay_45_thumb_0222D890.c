#include "global.h"

#include "heap.h"

void ov45_0222DFD0(void *);

void ov45_0222D890(void *param_1);

s32 ov45_0222DEA4(u8 *, s32);
s32 ov45_0222E000(void *);
void ov45_0222D8A4(u8 *arg0);
void ov45_0222D8BC(void *arg0, s32 *arg1);

void ov45_0222D890(void *param_1) {
    ov45_0222DFD0((u8 *)param_1 + 0x5c);
    Heap_Free(param_1);
    return;
}

void ov45_0222D8A4(u8 *arg0) {
    ov45_0222E000(arg0 + 0x5C);
    ov45_0222DEA4(arg0 + 8, 0);
}

void ov45_0222D8BC(void *arg0, s32 *arg1) {
    *(s32 *)arg0 = (s32)*arg1;
    *(s32 *)((u8 *)arg0 + 4) = 1;
}
