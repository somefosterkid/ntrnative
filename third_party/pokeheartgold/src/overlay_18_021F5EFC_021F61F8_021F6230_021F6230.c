#include "global.h"

u8 ov18_021F6214(void *param_1, s32 param_2, void *param_3, s32);

s32 ov18_021F6230(void *param_1, s32 param_2, void *param_3, s32 param_4, u32 param_5);

s32 ov18_021F6230(void *param_1, s32 param_2, void *param_3, s32 param_4, u32 param_5) {
    u32 bVar1;

    bVar1 = ov18_021F6214(param_1, param_2, param_3, param_4);
    return (u32)(bVar1 >> 1) + (param_4 - (param_5 >> 1));
}

s32 ov18_021F6244(void *param_1, s32 param_2, void *param_3, s32 param_4, u32 param_5);

s32 ov18_021F6244(void *param_1, s32 param_2, void *param_3, s32 param_4, u32 param_5) {
    u32 bVar1;

    bVar1 = ov18_021F6214(param_1, param_2, param_3, param_4);
    return (param_4 + (param_5 >> 1)) - (u32)(bVar1 >> 1);
}
