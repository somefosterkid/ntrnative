#include "global.h"

u16 *sub_02015550(void *, s32);

void sub_020156A8(void *arg0, u16 *arg1);

void sub_020156A8(void *arg0, u16 *arg1) {
    u16 *v0;

    v0 = sub_02015550(arg0, 3);
    if (v0 != NULL) {
        *v0 = *arg1;
    }
}

void sub_020156BC(void *arg0, u16 *arg1);

void sub_020156BC(void *arg0, u16 *arg1) {
    u16 *v0;

    v0 = sub_02015550(arg0, 3);
    if (v0 == NULL) {
        *arg1 = 0;
        return;
    }
    *arg1 = *v0;
}

void sub_020156D8(void *arg0, u16 *arg1);

void sub_020156D8(void *arg0, u16 *arg1) {
    void *v0;

    v0 = sub_02015550(arg0, 3);
    if (v0 != NULL) {
        *(u16 *)((u8 *)v0 + 2) = (u16)*arg1;
    }
}

void sub_020156EC(void *arg0, u16 *arg1);

void sub_020156EC(void *arg0, u16 *arg1) {
    void *v0;

    v0 = sub_02015550(arg0, 3);
    if (v0 == NULL) {
        *arg1 = 0;
        return;
    }
    *arg1 = *(u16 *)((u8 *)v0 + 2);
}
