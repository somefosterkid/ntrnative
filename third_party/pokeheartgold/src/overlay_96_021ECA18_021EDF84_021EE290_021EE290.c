#include "global.h"

s32 ov96_021ECC58(s32 *arg0, s32 *arg1);

typedef struct {
    u32 a : 16;
    u32 b : 8;
    u32 pad : 4;
    u32 c : 4;
} UnkStruct_ov96_021EE290;

void ov96_021EE290(UnkStruct_ov96_021EE290 *param_1);

void ov96_021EE290(UnkStruct_ov96_021EE290 *param_1) {
    s32 i;
    u32 best;
    s32 idx;
    UnkStruct_ov96_021EE290 *p;

    best = 0;
    for (i = 0, p = param_1; i < 4; i++, p++) {
        if (p->b == 0) {
            p->b = p->b + 1;
            if (p->c > best) {
                best = p->c;
                idx = i;
            }
        }
    }
    param_1[idx].b -= 1;
    param_1[idx].a += 1;
    MATH_QSort(param_1, 4, 4, (MATHCompareFunc)(ov96_021ECC58), (void *)0x0);
    param_1[0].a -= 1;
}
