#include "global.h"

s32 ov45_02230AA4(void *arg0);

s32 ov45_02230AA4(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 4) != 0) {
        return 1;
    }
    return 0;
}

void ov45_02230AB4(void *arg0);

void ov45_02230AB4(void *arg0) {
    memset(arg0, 0, 0x90U);
}

s32 sub_020181EC(void *);

void ov45_02230AC0(u8 *arg0);

void ov45_02230AC0(u8 *arg0) {
    sub_020181EC(arg0 + 0xC);
}
