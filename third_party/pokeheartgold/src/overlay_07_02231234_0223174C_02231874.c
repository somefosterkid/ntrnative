#include "global.h"

void ov07_02231514(u8 *, s16, s16);

void ov07_0223151C(u8 *, s16, s16);

void ov07_0223172C(void *, u8 *);

void ov07_0223174C(void *, u8 *);

void ov07_02231874(void *param_1);

void ov07_02231874(void *param_1) {
    u8 auStack_20[24];

    ov07_0223172C(param_1, auStack_20);
    ov07_02231514(auStack_20, 0, 3);
    ov07_0223151C(auStack_20, 0xc, 4);
    ov07_0223174C(param_1, auStack_20);
    return;
}
