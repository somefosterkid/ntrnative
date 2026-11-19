#include "global.h"

typedef union {
    struct {
        f32 val1_00;
        f32 val1_04;
        f32 val1_08;
        f32 val1_0C;
    };
    f32 val2[4];
} UnkUnion_ov116_02266FEC;

void ov92_022630E8(UnkUnion_ov116_02266FEC *param0);

void ov92_022630E8(UnkUnion_ov116_02266FEC *param0) {
    param0->val1_00 = 1.0;
    param0->val1_04 = 0;
    param0->val1_08 = 0;
    param0->val1_0C = 0;
}
