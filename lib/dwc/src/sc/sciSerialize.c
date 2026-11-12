// Black's sciSerialize.c (GameSpy's file, as pokebw2 names it), split from the glued ov175_021dd1ec.c.

#include "dwc/webservices_internal.h"


u8 *func_ov175_021e04fc(u8 *param_1, u32 param_2) {
    *param_1 = param_2;
    return param_1 + 1;
}

u8 *func_ov175_021e0504(u8 *param_1, s16 param_2) {
    u16 value;
    u8 *src;
    value = Ov175Swap16(param_2);
    src = (u8 *)&value;
    param_1[0] = src[0];
    param_1[1] = src[1];
    return param_1 + 2;
}

u8 *func_ov175_021e054c(u8 *param_1, u32 param_2) {
    u32 value;
    value = ((param_2 >> 24) & 0xff) | ((param_2 >> 8) & 0xff00) | ((param_2 << 8) & 0xff0000) | ((param_2 << 24) & 0xff000000);
    __memcpy(param_1, ((u8 *)&value), 4);
    return param_1 + 4;
}

u8 *func_ov175_021e05ac(u8 *param_1, u8 *param_2) {
    u32 data4[8];
    u32 data1;
    u32 data2;
    u32 data3;
    u8 bytes[8];
    s32 i;
    u8 *out;

    i = 0;
    data1 = 0;
    data2 = 0;
    data3 = 0;
    STD_TSScanf(param_2, "%8x", &data1);
    STD_TSScanf(param_2 + 9, "%4x", &data2);
    STD_TSScanf(param_2 + 0xe, "%4x", &data3);
    STD_TSScanf(param_2 + 0x13, "%2x", &data4[0]);
    STD_TSScanf(param_2 + 0x15, "%2x", &data4[1]);
    STD_TSScanf(param_2 + 0x18, "%2x%2x%2x%2x%2x%2x", &data4[2], &data4[3], &data4[4], &data4[5], &data4[6], &data4[7]);
    for (i = 0; i < 8; i++) {
        bytes[i] = data4[i];
    }
    out = func_ov175_021e054c(param_1, data1);
    out = func_ov175_021e0504(out, data2);
    out = func_ov175_021e0504(out, data3);
    *(Ov175Bytes8 *)out = *(Ov175Bytes8 *)bytes;
    return out + 8;
}
