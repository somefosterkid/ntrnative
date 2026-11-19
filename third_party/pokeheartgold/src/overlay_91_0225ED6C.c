#include "global.h"

#include "overlay_91.h"
#include "system.h"


typedef struct {
    u32 unk_00;
    NNSG2dSVec2 unk_04[8];
    u16 unk_24;
    u16 unk_26;
    u32 unk_28;
} UnkStruct_ov115_02262CA4;
BOOL ov91_0225EE18(UnkStruct_ov115_02262CA4 *param0, NNSG2dSVec2 *param1);
void ov91_0225EDB0(UnkStruct_ov115_02262CA4 *param0);
void ov91_0225EDC8(UnkStruct_ov115_02262CA4 *param0, NNSG2dSVec2 param1);
BOOL ov91_0225EE18(UnkStruct_ov115_02262CA4 *param0, NNSG2dSVec2 *param1);
BOOL ov91_0225EE48(UnkStruct_ov115_02262CA4 *param0, NNSG2dSVec2 *param1);
BOOL ov91_0225EE64(UnkStruct_ov115_02262CA4 *param0, NNSG2dSVec2 *param1);
u32 ov91_0225EE88(UnkStruct_ov115_02262CA4 *param0);

s32 ov91_0225ED6C(void *arg0) {
    s32 v0;
    s32 v1;

    if ((u32) * (u32 *)((u8 *)arg0 + 0x48) < 2U) {
        return 0;
    }
    v1 = 0x80 - gSystem.touchX;
    v0 = -0x60 - gSystem.touchY;
    if ((s32)(FX_Sqrt(((v1 * v1) + (v0 * v0)) << 0xC) >> 0xC) <= 0x11E) {
        return 1;
    }
    return 0;
}

void ov91_0225EDB0(UnkStruct_ov115_02262CA4 *param0) {
    memset(param0->unk_04, 0, sizeof(NNSG2dSVec2) * 8);
    param0->unk_24 = 0;
    param0->unk_26 = 0;
}

void ov91_0225EDC8(UnkStruct_ov115_02262CA4 *param0, NNSG2dSVec2 param1) {
    if (((param0->unk_26 + 1) % 8) == param0->unk_24) {
        NNSG2dSVec2 v0;

        ov91_0225EE18(param0, &v0);
    }

    param0->unk_04[param0->unk_26] = param1;
    param0->unk_26 = (param0->unk_26 + 1) % 8;
}

BOOL ov91_0225EE18(UnkStruct_ov115_02262CA4 *param0, NNSG2dSVec2 *param1) {
    if (param0->unk_26 == param0->unk_24) {
        return 0;
    }

    *param1 = param0->unk_04[param0->unk_24];
    param0->unk_24 = (param0->unk_24 + 1) % 8;

    return 1;
}

BOOL ov91_0225EE48(UnkStruct_ov115_02262CA4 *param0, NNSG2dSVec2 *param1) {
    if (param0->unk_26 == param0->unk_24) {
        return 0;
    }

    *param1 = param0->unk_04[param0->unk_24];

    return 1;
}

BOOL ov91_0225EE64(UnkStruct_ov115_02262CA4 *param0, NNSG2dSVec2 *param1) {
    s32 v0;

    if (param0->unk_26 == param0->unk_24) {
        return 0;
    }

    v0 = param0->unk_26 - 1;

    if (v0 < 0) {
        v0 += 8;
    }

    *param1 = param0->unk_04[v0];

    return 1;
}

u32 ov91_0225EE88(UnkStruct_ov115_02262CA4 *param0) {
    if (param0->unk_24 > param0->unk_26) {
        return (8 - param0->unk_24) + param0->unk_26;
    }

    return param0->unk_26 - param0->unk_24;
}
