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

void ov92_022630F8(UnkUnion_ov116_02266FEC *param0, UnkUnion_ov116_02266FEC *param1);

void ov92_022630F8(UnkUnion_ov116_02266FEC *param0, UnkUnion_ov116_02266FEC *param1) {
    *param0 = *param1;
}
