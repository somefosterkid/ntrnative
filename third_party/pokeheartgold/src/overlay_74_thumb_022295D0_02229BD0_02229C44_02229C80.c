#include "global.h"

#include "unk_02034354.h"
#include "unk_020379A0.h"
#include "unk_02037C94.h"

s32 ov74_02229BC0(s32 *, s16);

extern s32 ov74_02229CB4;

extern void *ov74_0223D0A4;

void ov74_02229C80(void);

void ov74_02229C80(void) {
    if (sub_02037EC8() != 0) {
        *(s8 *)((u8 *)ov74_0223D0A4 + 0x3C0) = 0;
        sub_02034434();
        sub_02037AC0(0xABU);
        ov74_02229BC0(&ov74_02229CB4, 0);
    }
}
