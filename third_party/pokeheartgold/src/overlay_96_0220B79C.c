#include "global.h"

#include "error_handling.h"

typedef struct UnkStruct_ov96_0220B7CC {
    u8 unk_0[0x40];
    s32 unk_40;
    s32 unk_44;
} UnkStruct_ov96_0220B7CC;

typedef struct UnkStruct_ov96_0220B7B4 {
    u8 unk_0[0x40];
    s32 unk_40;
} UnkStruct_ov96_0220B7B4;

u32 ov96_0220B79C(u8 *param_1);

u8 ov96_0220B7B4(UnkStruct_ov96_0220B7B4 *arg0);
s32 ov96_0220B7CC(UnkStruct_ov96_0220B7CC *arg0);

u32 ov96_0220B79C(u8 *param_1) {
    unsigned short new_var;
    if (param_1 == 0) {
        GF_AssertFail();
    }
    new_var = (*((u32 *)(param_1 + 0x44))) & 0xff;
    return new_var;
}

u8 ov96_0220B7B4(UnkStruct_ov96_0220B7B4 *arg0) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    return (u8)((u32)(arg0->unk_40 << 0x1E) >> 0x1E);
}

s32 ov96_0220B7CC(UnkStruct_ov96_0220B7CC *arg0) {
    s32 v1;
    u32 v0;

    if (arg0 == NULL) {
        GF_AssertFail();
    }
    v1 = 1;
    v0 = (u32)(arg0->unk_40 << 0x16) >> 0x18;
    if ((v0 != 0) && ((v0 != 1) || ((u32)((u32)(arg0->unk_44 << 8) >> 0x18) < 2U))) {
        v1 = 0;
    }
    return v1;
}
