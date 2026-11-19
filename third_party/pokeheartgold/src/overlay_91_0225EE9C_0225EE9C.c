#include "global.h"

s32 ov91_0225EDB0(void *);

s32 ov91_02260D00(void *);

void ov91_0225EE9C(void *arg0);

void ov91_0225EE9C(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x20) = 0;
    ov91_0225EDB0(arg0 + 0x20);
    ov91_02260D00(arg0 + 0x8544);
}
