#include "global.h"

#include "error_handling.h"
#include "overlay_49.h"
#include "overlay_49_unk_struct_ov49_02264cf8.h"

s32 ov45_0222AB28(s32, s32);

s32 ov49_02259FE8();

void *ov49_0225A30C(void *, s32, s32);

void ov49_02264C50(void *arg0, s32 arg1, s32 arg2) {
    s32 v0;
    s32 v1;
    s32 v2;
    s32 v3;

    v0 = ov49_02259FE8();
    v2 = ov45_0222AB28(v0, arg1);
    v1 = ov45_0222AB28(v0, arg2);
    v3 = 0x29;
    if ((v2 == 1) && (v1 == 1)) {
        v3 = 0x2AA;
    } else if ((v2 == 1) && (v1 == 0)) {
        v3 = 0x226;
    } else if ((v2 == 0) && (v1 == 1)) {
        v3 = 0x2A9;
    }
    ov49_0225A30C(arg0, 3, v3);
}

s16 ov45_0222A53C(s32);

s32 ov45_0222A578(s32, u32);

s32 ov45_0222A5C0(s32);

u32 ov49_02264B94(s32);

s32 ov49_02264CA8(UnkStruct_ov49_02264CF8 *arg0, s32 arg1, u8 arg2) {
    s32 v0;
    s32 v1;

    if (arg2 == -1U) {
        GF_AssertFail();
    }
    arg0->unk_03 = arg2;
    arg0->unk_08 = ov45_0222A53C(arg1);
    arg0->unk_0A = 1;
    v1 = ov45_0222A5C0(arg1);
    v0 = ov45_0222A578(arg1, arg0->unk_03);
    if (v0 == 0) {
        return 0;
    }
    arg0->unk_04 = ov49_02264B94(v1);
    arg0->unk_05 = ov49_02264B94(v0);
    return 1;
}

void ov49_02264CF8(void) {
}
