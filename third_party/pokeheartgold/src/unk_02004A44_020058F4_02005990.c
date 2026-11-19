#include "global.h"

#include "sound.h"

void sub_02005990(u32 param_1);

void sub_02005990(u32 param_1) {
    u32 *puVar1;

    puVar1 = GF_SdatGetAttrPtr(7);
    *puVar1 = param_1;
    return;
}

void sub_020059A0(u32 param_1);

void sub_020059A0(u32 param_1) {
    u32 *puVar1;

    puVar1 = GF_SdatGetAttrPtr(8);
    *puVar1 = param_1;
    return;
}
