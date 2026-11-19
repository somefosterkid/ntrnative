#include "global.h"

#include "overlay_80_0222AEF8.h"
#include "unk_02035900.h"

s32 ov80_0222AF54(void *arg0) {
    s32 v3;
    u16 *v0;
    u16 *v4;
    u16 v1;
    void *v2;

    v2 = arg0;
    v3 = 0;
    v0 = v2 + 0x72C;
    v4 = v0;
    do {
        v1 = *(u16 *)((u8 *)v2 + 0x18);
        v3 += 1;
        v2 += 2;
        *v4 = v1;
        v4 += 1;
    } while (v3 < 0x14);
    if (sub_02037030(0x23, v0, 0x2C) == 1) {
        return 1;
    }
    return 0;
}
