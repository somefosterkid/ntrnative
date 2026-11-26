#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
struct UnkStruct_ov14_boxapp;

#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E7D8C(void *, s32, s32);
void ov14_021E8394(UnkStruct_0201956C *arg0);

void ov14_021E81A8(void *arg0, s32 arg1);
void ov14_021E81B4(UnkStruct_0201956C *arg0);
void ov14_021E81D8(UnkStruct_0201956C *arg0);
s32 ov14_021F5C84(void *, s32);
void ov14_021E8234(UnkStruct_0201956C *arg0);
void ov14_021E8248(UnkStruct_0201956C *arg0);
s32 ov14_021F5E94(void *, s32);
void ov14_021E825C(struct UnkStruct_ov14_boxapp *arg0);
void ov14_021E8294(UnkStruct_0201956C *arg0);
void ov14_021E82A8(UnkStruct_0201956C *arg0);
s32 ov14_021F5EB4(void *, s32);
s32 ov14_021F5EC4(void *, s32);
void ov14_021E82DC(void *arg0);
void ov14_021E8314(UnkStruct_0201956C *arg0);
s32 ov14_021E834C(UnkStruct_0201956C *arg0);
void ov14_021E8368(UnkStruct_0201956C *arg0);
void ov14_021E83F4(UnkStruct_0201956C *arg0);

void ov14_021E81A8(void *arg0, s32 arg1) {
    ov14_021E7D8C(arg0, arg1, 8);
}

void ov14_021E81B4(UnkStruct_0201956C *a0) {
    sub_020196E8(a0, 0xD, 0, -7);
    sub_020198FC(a0, 0xD, 0, 1, 7);
}

void ov14_021E81D8(UnkStruct_0201956C *a0) {
    sub_020196E8(a0, 0xD, 0, 0);
    sub_020198FC(a0, 0xD, 0, -1, 7);
}

void ov14_021E81FC(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F5C84(arg0, 0);
    sub_020196E8(arg0->sys->unk_2F0, 8, 0, 0x15);
}

void ov14_021E821C(void *arg0) {
    sub_020196E8(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 8, 0, 0x18);
}

void ov14_021E8234(UnkStruct_0201956C *a0) {
    sub_020198FC(a0, 8, 0, 1, 3);
}

void ov14_021E8248(UnkStruct_0201956C *a0) {
    sub_020198FC(a0, 8, 0, -1, 3);
}

void ov14_021E825C(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F5E94(arg0, 0);
    sub_020196E8(arg0->sys->unk_2F0, 9, 0xC, 0x15);
}

void ov14_021E827C(void *arg0) {
    sub_020196E8(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 9, 0xC, 0x18);
}

void ov14_021E8294(UnkStruct_0201956C *a0) {
    sub_020198FC(a0, 9, 0, 1, 3);
}

void ov14_021E82A8(UnkStruct_0201956C *a0) {
    sub_020198FC(a0, 9, 0, -1, 3);
}

void ov14_021E82BC(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F5EB4(arg0, 0);
    sub_020196E8(arg0->sys->unk_2F0, 0xA, 0x18, 0x15);
}

void ov14_021E82DC(void *arg0) {
    ov14_021F5EC4(arg0, 0);
    sub_020196E8(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 0xA, 0x18, 0x15);
}

void ov14_021E82FC(UnkStruct_0201956C *a0) {
    sub_0201980C(a0, 0xA);
    sub_020196E8(a0, 0xA, 0x18, 0x18);
}

void ov14_021E8314(UnkStruct_0201956C *a0) {
    sub_020198FC(a0, 0xA, 0, 1, 3);
}

void ov14_021E8328(UnkStruct_0201956C *a0) {
    sub_020196E8(a0, 0xA, 0x18, 0x18);
    sub_020198FC(a0, 0xA, 0, -1, 3);
}

s32 ov14_021E834C(UnkStruct_0201956C *arg0) {
    s8 v0[2];
    s32 v1;

    sub_02019B1C(arg0, 0xB, &v0[1], &v0[0]);
    v1 = v0[0] == 0x15 ? 1 : 0;
    return v1;
}

void ov14_021E8368(UnkStruct_0201956C *a0) {
    sub_0201980C(a0, 8);
    sub_0201980C(a0, 9);
    sub_020196E8(a0, 8, 0, 0x18);
    sub_020196E8(a0, 9, 0xC, 0x18);
}

void ov14_021E8394(UnkStruct_0201956C *a0) {
    char new_var;
    u32 v0;
    u32 v1;
    v0 = 0;
    v1 = 5;
    do {
        new_var = (s32)v1;
        sub_020196E8(a0, v0 + 3, 0x20, new_var);
        v0 += 1;
        v1 += 3;
    } while (v0 < 5U);
    sub_020196E8(a0, 0xB, 0x18, 0x18);
}

void ov14_021E83C4(UnkStruct_0201956C *a0) {
    u32 v0;
    u32 v1;
    char new_var;
    v0 = 0;
    v1 = 5;
    do {
        new_var = (s32)v1;
        sub_020196E8(a0, v0 + 3, 0x15, new_var);
        v0 += 1;
        v1 += 3;
    } while (v0 < 5U);
    sub_020196E8(a0, 0xB, 0x18, 0x15);
}

void ov14_021E83F4(UnkStruct_0201956C *a0) {
    u32 v0;
    u32 v1;
    char new_var2;
    int new_var;
    v0 = 0;
    new_var = 5;
    v1 = new_var;
    do {
        sub_0201980C(a0, v0 + 3);
        new_var2 = v1;
        sub_020196E8(a0, v0 + 3, 0x20, (s32)new_var2);
        v0 += 1;
        v1 += 3;
    } while (v0 < 5U);
    sub_0201980C(a0, 0xB);
    sub_020196E8(a0, 0xB, 0x18, 0x18);
}
