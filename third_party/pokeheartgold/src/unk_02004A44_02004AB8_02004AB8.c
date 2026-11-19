#include "global.h"

#include "sound.h"

void sub_02004AB8(u16 param_1);

void sub_02004AB8(u16 param_1) {
    u16 *puVar1;

    puVar1 = GF_SdatGetAttrPtr(0x3a);
    *puVar1 = param_1;
    return;
}

void Sound_SetFieldBGM(u16 param_1);

void Sound_SetFieldBGM(u16 param_1) {
    u16 *puVar1;

    puVar1 = GF_SdatGetAttrPtr(0x20);
    *puVar1 = param_1;
    return;
}
