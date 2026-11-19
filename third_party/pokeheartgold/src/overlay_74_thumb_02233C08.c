#include "global.h"

u16 ov74_02233B58(int param0, void *param1, u32 *param2);

u16 ov74_02233C08(void *param0, int *param1, u32 *param2);

u16 ov74_02233C08(void *param0, int *param1, u32 *param2) {
    u16 v0, v1;
    u32 v2, v3;

    v0 = ov74_02233B58(0, param0, &v2);
    v1 = ov74_02233B58(1, param0, &v3);

    if ((v0 == 0x1) && (v1 == 0x1)) {
        if (((v2 == 0xffffffff) && (v3 == 0x0)) || ((v2 == 0x0) && (v3 == 0xffffffff))) {
            if ((v2 + 1) < (v3 + 1)) {
                *param2 = v3;
                *param1 = 1;
            } else {
                *param2 = v2;
                *param1 = 0;
            }
        } else {
            if (v2 < v3) {
                *param2 = v3;
                *param1 = 1;
            } else {
                *param2 = v2;
                *param1 = 0;
            }
        }

        return 0x1;
    }

    if ((v0 == 0x1) && (v1 != 0x1)) {
        *param2 = v2;

        if (v1 == 0xff) {
            *param1 = 0;
            return 0xff;
        } else {
            *param1 = 0;
            return 0x1;
        }
    }

    if ((v1 == 0x1) && (v0 != 0x1)) {
        *param2 = v3;

        if (v0 == 0xff) {
            *param1 = 1;
            return 0xff;
        } else {
            *param1 = 1;
            return 0x1;
        }
    }

    if ((v0 == 0x0) && (v1 == 0x0)) {
        *param2 = 0;
        *param1 = 0;

        return 0x0;
    }

    *param2 = 0;
    *param1 = 0;

    return 0x2;
}
