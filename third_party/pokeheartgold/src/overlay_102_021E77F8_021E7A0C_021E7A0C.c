#include "global.h"

u32 ov102_021E7A0C(s16 *param_1);

u32 ov102_021E7A0C(s16 *param_1) {
    return (s32)((s8 *)param_1)[2] + (s32)*param_1 * (s32) * (s8 *)((u8 *)param_1 + 3) & 0xff;
}
