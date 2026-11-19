#include "global.h"

typedef int (*UnkFunc_ov112_021EAC34)(void *work, void *self, int offset, int r3);

extern UnkFunc_ov112_021EAC34 ov112_021FF6C4[];

int ov112_021EAC34(unsigned char *work, int r1, int r2, int r3);

int ov112_021EAC34(unsigned char *work, int r1, int r2, int r3) {
    int offset = *(int *)(work + 8) * 4;
    UnkFunc_ov112_021EAC34 func = *(UnkFunc_ov112_021EAC34 *)((unsigned char *)ov112_021FF6C4 + offset);
    *(int *)(work + 8) = func(work, (void *)func, offset, r3);
    if (*(int *)(work + 8) == 0x1B) {
        *(int *)(work + 8) = 0;
        return 3;
    }
    return 2;
}
