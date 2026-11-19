#include "global.h"

#include "unk_02015DD8.h"


void sub_02015FD8(void *arg0, s16 arg1, s16 arg2);
void sub_02015FE0(void *arg0, s16 arg1, s16 arg2);

s32 sub_02015FCC(void *arg0) {
    return (*(u16 *)((u8 *)arg0 + 2) << 0x10) | *(u16 *)arg0;
}

void sub_02015FD8(void *arg0, s16 arg1, s16 arg2) {
    *(s16 *)((u8 *)arg0 + 4) = arg1;
    *(s16 *)((u8 *)arg0 + 6) = arg2;
}

void sub_02015FE0(void *arg0, s16 arg1, s16 arg2) {
    *(s16 *)((u8 *)arg0 + 8) = arg1;
    *(s16 *)((u8 *)arg0 + 0xA) = arg2;
}
