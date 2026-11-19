#include "global.h"

#include "unk_0201956C.h"
#include "overlay_14_box_funcs.h"

void ov14_021E7FB8(UnkStruct_0201956C *arg0);
void ov14_021E7FEC(UnkStruct_0201956C *arg0);
void ov14_021E8048(UnkStruct_0201956C *arg0);
void ov14_021E805C(UnkStruct_0201956C *arg0);
void ov14_021E8094(UnkStruct_0201956C *arg0);

s32 sub_02019B1C(UnkStruct_0201956C *, s32, s8 *, s8 *);

void ov14_021E7FB8(UnkStruct_0201956C *a0) {
    s8 v0[2];
    s8 v1;

    sub_02019B1C(a0, 1, &v0[1], &v0[0]);
    v1 = v0[0];
    if (v1 != 0x18) {
        sub_020198FC(a0, 1, 0, 1, (s32)(u8)(0x18 - v1));
    }
}

void ov14_021E7FEC(UnkStruct_0201956C *a0) {
    s8 v0[2];
    s8 v1;

    sub_02019B1C(a0, 1, &v0[1], &v0[0]);
    v1 = v0[0];
    if (v1 != 6) {
        sub_020198FC(a0, 1, 0, -1, (s32)(u8)(v1 - 6));
    }
}

void ov14_021E8020(UnkStruct_0201956C *a0) {
    sub_020198FC(a0, 1, 1, 0, 0x13);
    sub_020198FC(a0, 0xA, 1, 0, 0x13);
}

void ov14_021E8048(UnkStruct_0201956C *a0) {
    sub_020198FC(a0, 1, 1, 0, 0x13);
}

void ov14_021E805C(UnkStruct_0201956C *a0) {
    sub_020198FC(a0, 1, -1, 0, 0x13);
    sub_020196E8(a0, 0xA, 0x2B, 0x15);
    sub_020198FC(a0, 0xA, -1, 0, 0x13);
}

void ov14_021E8094(UnkStruct_0201956C *a0) {
    sub_020198FC(a0, 1, -1, 0, 0x13);
}
