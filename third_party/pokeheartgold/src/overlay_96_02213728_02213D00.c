#include "global.h"

#include "unk_02020E80.h"

void ov96_02213D00(Vec2D *param_1, Vec2D *param_2, Vec2D *param_3, s32 *param_4);

void ov96_02213D00(Vec2D *param_1, Vec2D *param_2, Vec2D *param_3, s32 *param_4) {
    Vec2D VStack_14;
    Vec2D VStack_1c;

    VStack_14.x = (s32)(*param_4 + ((u32)(*param_4 >> 0xb) >> 0x14)) >> 0xc;
    VStack_14.y = (s32)(param_4[1] + ((u32)(param_4[1] >> 0xb) >> 0x14)) >> 0xc;
    sub_02020F4C(param_1, param_2, param_3, &VStack_14, &VStack_1c);
    return;
}
