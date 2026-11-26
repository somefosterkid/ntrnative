#include "global.h"
#include "screen_fade_internal.h"

void sub_02013220(u32, u32, u32, u32);

void sub_020132A8(u32, u32, u32);

void sub_02013364(u32, u32, u32, u32, u32, u32);

void sub_02013440(u32, u32, u32, u32, u32);

void sub_02013468(u32, u32, u32, u32);

void sub_02013488(u32, u32, u32, u32, u32, u32, u32);

void sub_02010F84(u32 param_1, u32 param_2, u32 param_3, u32 param_4, u32 param_5, u32 param_6, u32 param_7, u32 param_8, u32 param_9, s32 param_10);

void sub_02010F84(u32 param_1, u32 param_2, u32 param_3, u32 param_4, u32 param_5, u32 param_6, u32 param_7, u32 param_8, u32 param_9, s32 param_10) {
    if (param_10 == 0) {
        sub_02013220(param_2, 0, param_4, param_5);
        sub_020132A8(param_3, 0, param_5);
        sub_02013364(param_6, param_7, param_8, param_9, param_4, param_5);
        return;
    }
    sub_02013440(param_1, param_2, 0, param_4, param_5);
    sub_02013468(param_1, param_3, 0, param_5);
    sub_02013488(param_1, param_6, param_7, param_8, param_9, param_4, param_5);
    return;
}
