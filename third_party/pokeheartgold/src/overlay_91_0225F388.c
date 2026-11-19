#include "global.h"

typedef struct {
    u32 unk_00[4];
    u32 unk_10;
    u32 unk_14;
} UnkStruct_ov115_022632C8;

void ov91_0225F388(UnkStruct_ov115_022632C8 *param0);
void ov91_0225F3A0(UnkStruct_ov115_022632C8 *param0, u32 param1, u32 param2);
void ov91_0225F3D4(UnkStruct_ov115_022632C8 *param0);
u32 ov91_0225F3E4(const UnkStruct_ov115_022632C8 *param0);
BOOL ov91_0225F3E8(const UnkStruct_ov115_022632C8 *param0, u32 param1);

void ov91_0225F388(UnkStruct_ov115_022632C8 *param0) {
    memset(param0, 0, sizeof(UnkStruct_ov115_022632C8));
    param0->unk_14 = 0xffff;
}

void ov91_0225F3A0(UnkStruct_ov115_022632C8 *param0, u32 param1, u32 param2) {
    param0->unk_00[param2] = param1;
    param0->unk_10 |= 1 << param2;

    if (param1 == 0) {
        return;
    }

    if (param0->unk_14 != 0xffff) {
        if (param0->unk_00[param0->unk_14] < param1) {
            param0->unk_14 = param2;
        }
    } else {
        param0->unk_14 = param2;
    }
}

void ov91_0225F3D4(UnkStruct_ov115_022632C8 *param0) {
    param0->unk_10 = 0;
    param0->unk_14 = 0xffff;
}

u32 ov91_0225F3E4(const UnkStruct_ov115_022632C8 *param0) {
    return param0->unk_14;
}

BOOL ov91_0225F3E8(const UnkStruct_ov115_022632C8 *param0, u32 param1) {
    int v0;
    int v1 = 0;

    for (v0 = 0; v0 < 4; v0++) {
        if ((param0->unk_10 & (1 << v0)) != 0) {
            v1++;
        }
    }

    if (param1 <= v1) {
        return 1;
    }

    return 0;
}
