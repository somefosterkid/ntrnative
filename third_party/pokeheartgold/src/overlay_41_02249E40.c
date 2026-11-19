#include "global.h"

void ov41_02249E40(NNSG2dScreenData *param0, u32 param1);

void ov41_02249E40(NNSG2dScreenData *param0, u32 param1) {
    int v0;
    u16 *v1;
    int v2 = param0->szByte / 2;
    v1 = (u16 *)param0->rawData;

    for (v0 = 0; v0 < v2; v0++) {
        v1[v0] += param1;
    }
}
