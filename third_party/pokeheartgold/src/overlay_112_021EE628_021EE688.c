#include "global.h"

typedef int (*UnkFunc_ov112_021EE688)(void *work, void *self, int offset, int r3);

extern UnkFunc_ov112_021EE688 ov112_021FF8D8[];

int ov112_021EE688(unsigned char *work, int r1, int r2, int r3);

int ov112_021EE688(unsigned char *work, int r1, int r2, int r3) {
    int offset = *(int *)(work + 8) * 4;
    UnkFunc_ov112_021EE688 func = *(UnkFunc_ov112_021EE688 *)((unsigned char *)ov112_021FF8D8 + offset);
    *(int *)(work + 8) = func(work, (void *)func, offset, r3);
    if (*(int *)(work + 8) == 8) {
        *(int *)(work + 8) = 0;
        return 3;
    }
    return 2;
}
