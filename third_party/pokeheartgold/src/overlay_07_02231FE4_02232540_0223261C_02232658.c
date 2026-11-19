#include "global.h"

extern const int ov07_022375BC[20][4];

s32 ov07_0223251C(s32 arg0);

int ov07_02232658(int param0, int param1);

int ov07_02232658(int param0, int param1) {
    int v0 = ov07_0223251C(param0);
    return ov07_022375BC[v0][param1];
}
