#include "global.h"

#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

void ov14_021E85AC(UnkStruct_0201956C *arg0);
void ov14_021E8600(UnkStruct_0201956C *arg0);

void ov14_021E8560(UnkStruct_0201956C *arg0);
void ov14_021E8718(UnkStruct_0201956C *arg0);
void ov14_021E8588(UnkStruct_0201956C *arg0);
void ov14_021E85BC(UnkStruct_0201956C *arg0);
void ov14_021E85D0(UnkStruct_0201956C *arg0);
s32 ov14_021E82DC(void *arg0);
s32 ov14_021F4720(void *);
s32 ov14_021F4848(void *);
s32 ov14_021F48B4(void *);
s32 ov14_021F57B8(void *);
void ov14_021E8664(void *arg0);
void ov14_021E86E0(UnkStruct_0201956C *arg0);
void ov14_021E8704(UnkStruct_0201956C *arg0);

void ov14_021E8560(UnkStruct_0201956C *a0) {
    sub_020196E8(a0, 0xC, 0x20, 0xE);
    sub_020198FC(a0, 0xC, -1, 0, 0xB);
}

void ov14_021E8588(UnkStruct_0201956C *a0) {
    sub_020196E8(a0, 0xC, 0x15, 0xE);
    sub_020198FC(a0, 0xC, 1, 0, 0xB);
}

void ov14_021E85AC(UnkStruct_0201956C *a0) {
    sub_020196E8(a0, 0xE, 0, 0x18);
}

void ov14_021E85BC(UnkStruct_0201956C *a0) {
    sub_020198FC(a0, 0xE, 0, -1, 3);
}

void ov14_021E85D0(UnkStruct_0201956C *a0) {
    sub_020198FC(a0, 0xE, 0, 1, 3);
}

void sub_02019B1C(UnkStruct_0201956C *, s32, s8 *, s8 *);

s32 ov14_021E85E4(UnkStruct_0201956C *arg0) {
    s8 v0[2];
    s32 v1;

    sub_02019B1C(arg0, 0xE, &v0[1], &v0[0]);
    v1 = v0[0] == 0x15 ? 1 : 0;
    return v1;
}

void ov14_021E8600(UnkStruct_0201956C *a0) {
    sub_020196E8(a0, 0xF, 0, 0x18);
}

void ov14_021E8610(UnkStruct_0201956C *a0) {
    sub_020196E8(a0, 0xF, 0, 0x15);
}

void ov14_021E8620(UnkStruct_0201956C *a0) {
    sub_020198FC(a0, 0xF, 0, -1, 3);
}

void ov14_021E8634(UnkStruct_0201956C *a0) {
    sub_020198FC(a0, 0xF, 0, 1, 3);
}

s32 ov14_021E8648(UnkStruct_0201956C *arg0) {
    s8 v0[2];
    s32 v1;

    sub_02019B1C(arg0, 0xF, &v0[1], &v0[0]);
    v1 = v0[0] == 0x15 ? 1 : 0;
    return v1;
}

void ov14_021E8664(void *arg0) {
    ov14_021E82DC(arg0);
    sub_0201980C(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 8);
    sub_0201980C(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 9);
    sub_020196E8(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 8, 0, 0x18);
    sub_020196E8(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 9, 0xC, 0x18);
    sub_020196E8(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 0, 0, 0);
    ov14_021F57B8(arg0);
    ov14_021F4720(arg0);
    ov14_021F4848(arg0);
    ov14_021F48B4(arg0);
    ov14_021F47B8(arg0, 0x30);
}

void ov14_021E86E0(UnkStruct_0201956C *a0) {
    sub_020196E8(a0, 0, 0, -6);
    sub_020198FC(a0, 0, 0, 1, 6);
}

void ov14_021E8704(UnkStruct_0201956C *a0) {
    sub_020198FC(a0, 0, 0, -1, 6);
}

void ov14_021E8718(UnkStruct_0201956C *a0) {
    sub_020196E8(a0, 0xC, 0x15, 8);
    sub_020196E8(a0, 6, 0x15, 0xE);
    sub_020196E8(a0, 7, 0x15, 0x11);
}
