#include "global.h"

#include "overlay_02_02245B80.h"

s32 ov02_02246490(LegendFadeTask *env) {
    u8 temp_r0_2;
    u8 temp_r1;
    u8 temp_r1_2;
    u8 temp_r3;
    u32 var_r0;
    switch (env->fadePhase) {
    case 0:
        env->fadeWait = 0U;
        env->fadeStep = 0U;
        env->alpha = 0U;
        env->fadePhase = (u8)(env->fadePhase + 1);
        break;

    case 1:
        temp_r1 = env->fadeWait;
        env->fadeWait = (u8)(temp_r1 + 1);
        if (temp_r1 == 0) {
            temp_r3 = env->alpha;
            G2_SetBlendAlpha(2, 5, (s32)temp_r3, 0x1F - temp_r3);
            temp_r1_2 = env->fadeStep;
            env->fadeStep = (u8)(temp_r1_2 + 1);
            if (((u32)temp_r1_2) < 0xCU) {
                temp_r0_2 = env->alpha;
                var_r0 = temp_r0_2 + 1;
            } else {
                temp_r0_2 = env->alpha;
                var_r0 = 1;
                var_r0 = temp_r0_2 - var_r0;
            }
            env->alpha = var_r0;
            env->fadeWait = 0U;
            if (((u32)env->fadeStep) > 0x18U) {
                env->fadePhase = (u8)(env->fadePhase + 1);
            }
        }
        break;

    default:
        env->fadeStep = 0U;
        env->fadeWait = 0U;
        env->fadePhase = 0U;
        return 1;
    }

    return 0;
}
