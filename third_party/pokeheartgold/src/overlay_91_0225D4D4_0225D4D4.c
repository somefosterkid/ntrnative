#include "global.h"

struct Vec2s32;

typedef struct Vec2s32 {
    s32 x;
    s32 y;
} Vec2s32;

void ov91_0225D4D4(u32 param0, u32 param1, u32 param2, s32 *param3, s32 *param4);

extern const Vec2s32 ov91_02261FAC[4][4][4];

void ov91_0225D4D4(u32 param0, u32 param1, u32 param2, s32 *param3, s32 *param4) {

    *param3 = ov91_02261FAC[param1][param0 - 1][param2].x;
    *param4 = ov91_02261FAC[param1][param0 - 1][param2].y;
}
