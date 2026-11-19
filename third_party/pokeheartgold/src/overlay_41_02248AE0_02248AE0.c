#include "global.h"

void *ov41_02248ABC();

s32 ov41_02248AE0(void);

s32 ov41_02248AE0(void) {
    s32 v2;
    void *v0;
    void *v1;

    v2 = 0;
    v0 = ov41_02248ABC();
    v1 = *(void **)((u8 *)v0 + 8);
    if (v1 != v0) {
        do {
            v1 = *(void **)((u8 *)v1 + 8);
            v2 += 1;
        } while (v1 != v0);
    }
    return v2;
}
