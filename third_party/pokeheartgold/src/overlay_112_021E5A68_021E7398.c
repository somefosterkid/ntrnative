#include "global.h"

s32 ov112_021E5B98();

s32 ov112_021E5E18(s32);

extern OSMutex ov112_021FFB08;

void ov112_021E7398(void *arg0);

void ov112_021E7398(void *arg0) {
loop_1:
    OS_LockMutex(&ov112_021FFB08);
    if (ov112_021E5B98() == 1) {
        ov112_021E5E18(2);
    }
    OS_UnlockMutex(&ov112_021FFB08);
    OS_Sleep(4U);
    goto loop_1;
}
