#include "global.h"

void sub_02005A74(u32, u32, u32, u32, u32, u8, void *);

void GF_FadeStartMusicId(u32 param_1, u32 param_2, u32 param_3, u32 param_4, u8 param_5, void *param_6);

void GF_FadeStartMusicId(u32 param_1, u32 param_2, u32 param_3, u32 param_4, u8 param_5, void *param_6) {
    sub_02005A74(4, param_1, param_2, param_3, param_4, param_5, param_6);
    return;
}
