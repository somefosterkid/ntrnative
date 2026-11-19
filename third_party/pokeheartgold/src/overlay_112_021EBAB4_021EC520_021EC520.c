#include "global.h"

typedef int (*UnkFunc_ov112_021EC520)(void *work, void *self, int offset, int r3);

extern UnkFunc_ov112_021EC520 ov112_021FF7CC[];

void ov112_021EA864(void *arg0);

int ov112_021EC520(unsigned char *work, int r1, int r2, int r3);

int ov112_021EC520(unsigned char *work, int r1, int r2, int r3) {
    int offset = *(int *)(work + 8) * 4;
    UnkFunc_ov112_021EC520 func = *(UnkFunc_ov112_021EC520 *)((unsigned char *)ov112_021FF7CC + offset);
    *(int *)(work + 8) = func(work, (void *)func, offset, r3);
    if (*(int *)(work + 8) == 0x43) {
        *(int *)(work + 8) = 0;
        return 3;
    }
    ov112_021EA864(work);
    return 2;
}
