#include "global.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "math_util.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_0200A090.h"

SysTask *ov69_021E6300();

s32 ov69_021E758C(void);

void ov69_021E75A0(VecFx32 *param0);
u32 ov69_021E75F8(const VecFx32 *param0, const VecFx32 *param1);

s32 ov69_021E758C(void) {
    if (ov69_021E6300() != 0) {
        return 1;
    }
    return 0;
}

void ov69_021E75A0(VecFx32 *param0) {
    if (param0->x >= 0) {
        param0->x = param0->x % 0xffff;
    } else {
        param0->x = param0->x + (0xffff * (((((param0->x) < 0) ? (-(param0->x)) : (param0->x)) / 0xffff) + 1));
    }

    if (param0->y >= 0) {
        param0->y = param0->y % 0xffff;
    } else {
        param0->y = param0->y + (0xffff * (((((param0->y) < 0) ? (-(param0->y)) : (param0->y)) / 0xffff) + 1));
    }
}

u32 ov69_021E75F8(const VecFx32 *param0, const VecFx32 *param1) {
    s32 v0, v1;
    u32 v2;

    v0 = (((param0->x - param1->x) < 0) ? (-(param0->x - param1->x)) : (param0->x - param1->x));
    v1 = (((param0->y - param1->y) < 0) ? (-(param0->y - param1->y)) : (param0->y - param1->y));

    if (v0 > GF_DegreeToSinCosIdxNoWrap(180)) {
        v0 = 0xffff - v0;
    }

    if (v1 > GF_DegreeToSinCosIdxNoWrap(180)) {
        v1 = 0xffff - v1;
    }

    v2 = FX_Sqrt(((v0 * v0) + (v1 * v1)) << 12) >> 12;

    return v2;
}
