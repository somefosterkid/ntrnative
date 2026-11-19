#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "bg_window.h"
#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021E7E40(UnkStruct_0201956C *arg0);
void ov14_021E7E64(UnkStruct_0201956C *arg0);
void ov14_021E7E78(UnkStruct_0201956C *arg0);
void ov14_021E7E88(UnkStruct_0201956C *arg0);
void ov14_021E7EC0(UnkStruct_0201956C *arg0);
void ov14_021E7ED0(UnkStruct_0201956C *arg0);

void ov14_021E7E40(UnkStruct_0201956C *a0) {
    sub_020196E8(a0, 2, 0x15, 0x18);
    sub_020198FC(a0, 2, 0, -1, 0x12);
}

void ov14_021E7E64(UnkStruct_0201956C *a0) {
    sub_020198FC(a0, 2, 0, 1, 0x12);
}

void ov14_021E7E78(UnkStruct_0201956C *a0) {
    sub_020196E8(a0, 1, 2, 6);
}

void ov14_021E7E88(UnkStruct_0201956C *a0) {
    sub_020196E8(a0, 1, 0x15, 6);
}

void ov14_021E7E98(UnkStruct_ov14_boxapp *arg0) {
    FillBgTilemapRect(arg0->sys->unk_14, 1U, 0U, 2U, 0x15U, 0xBU, 0x12U, 0x10U);
}

void ov14_021E7EC0(UnkStruct_0201956C *a0) {
    sub_020196E8(a0, 1, 2, 0x18);
}

void ov14_021E7ED0(UnkStruct_0201956C *a0) {
    sub_020196E8(a0, 1, 0x15, 0x18);
}
