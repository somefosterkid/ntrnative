#include "global.h"

void sub_02037E1C(u8 arg0);

extern void *ov74_0223D0A4;

void ov74_02229BC0(void (*func)(void), s16 arg1);

void ov74_02229C80(void);

void ov74_02229C60(void);

void ov74_02229C60(void) {
    sub_02037E1C(*(u8 *)((u8 *)ov74_0223D0A4 + 0x27));
    ov74_02229BC0(ov74_02229C80, 0);
}
