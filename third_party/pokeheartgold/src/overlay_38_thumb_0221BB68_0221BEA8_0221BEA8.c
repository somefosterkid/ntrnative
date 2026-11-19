#include <dwc.h>

#include "global.h"

s32 ov38_0221BB44(s32);

extern s32 _02225040;

extern s8 ov38_0222508C;

extern s8 ov38_02225094;

extern s8 ov38_0222512C;

s32 ov38_0221BEA8(u8 *url, s32 dwcProfileID, void *data, s32 dataSize, u8 *response, s32 maxResponseLength);

s32 ov38_0221BEA8(u8 *url, s32 dwcProfileID, void *data, s32 dataSize, u8 *response, s32 maxResponseLength) {
    s32 temp_r4;
    s8 *temp_r0;
    s8 *temp_r6_2;
    u32 temp_r5;
    u32 temp_r6;

    if (*(s32 *)(&_02225040) != 2) {
        return 1;
    }
    *(s32 *)((u8 *)(&_02225040) + 0x10) = dwcProfileID;
    *(void **)((u8 *)(&_02225040) + 0x14) = data;
    *(s32 *)((u8 *)(&_02225040) + 0x18) = dataSize;
    temp_r4 = dataSize + 8;
    *(u8 **)((u8 *)(&_02225040) + 0x1C) = response;
    *(s32 *)((u8 *)(&_02225040) + 0x20) = maxResponseLength;
    temp_r6 = strlen((s8 *)url);
    temp_r0 = DWC_Alloc((DWCAllocType)(0xA), temp_r6 + 0x45 + ov38_0221BB44(temp_r4));
    *(s8 **)((u8 *)(&_02225040) + 0x24) = temp_r0;
    if (temp_r0 == NULL) {
        return 2;
    }
    sprintf(temp_r0, &ov38_0222512C, url, dwcProfileID);
    temp_r6_2 = *(s8 **)((u8 *)(&_02225040) + 0x24);
    temp_r5 = strlen(temp_r6_2);
    *(s8 **)((u8 *)(&_02225040) + 0x28) = &(&temp_r6_2[temp_r5])[strlen(&ov38_0222508C)];
    *(s8 **)((u8 *)(&_02225040) + 0x2C) = &(*(s8 **)((u8 *)(&_02225040) + 0x28) + 0x28)[strlen(&ov38_02225094)];
    *(s32 *)((u8 *)(&_02225040) + 0x30) = (s32)(ov38_0221BB44(temp_r4) + 1);
    *(s32 *)(&_02225040) = 3;
    return 0;
}
