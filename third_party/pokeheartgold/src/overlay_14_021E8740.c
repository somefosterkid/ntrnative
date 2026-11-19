#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "unk_0201956C.h"
#include "bg_window.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021E8944(UnkStruct_0201956C *arg0);

s32 ov14_021E8664(void *arg0);
s32 ov14_021E8718(void *);
s32 ov14_021F6070(void *);
void ov14_021E8740(struct UnkStruct_ov14_boxapp *arg0);
void ov14_021E87BC(UnkStruct_0201956C *arg0);
void ov14_021E8824(UnkStruct_ov14_boxsys *arg0);
void ov14_021E884C(UnkStruct_ov14_boxsys *arg0);
void ov14_021E88BC(UnkStruct_0201956C *arg0);
void ov14_021E88F8(UnkStruct_0201956C *arg0);
void ov14_021E895C(struct UnkStruct_ov14_boxapp *arg0, s32 arg1);
void ov14_021E875C(UnkStruct_0201956C *arg0);

s32 sub_02019B1C(UnkStruct_0201956C *, s32, s8 *, s8 *);

void ov14_021E8740(UnkStruct_ov14_boxapp *arg0) {
    ov14_021E8664(arg0);
    ov14_021F6070(arg0);
    ov14_021E8718(((u8 *)arg0->sys->unk_2F0));
}

void ov14_021E875C(UnkStruct_0201956C *a0) {
    sub_020196E8(a0, 0xC, 0x20, 8);
    sub_020196E8(a0, 6, 0x20, 0xE);
    sub_020196E8(a0, 7, 0x20, 0x11);
    sub_020198FC(a0, 0xC, -1, 0, 0xB);
    sub_020198FC(a0, 6, -1, 0, 0xB);
    sub_020198FC(a0, 7, -1, 0, 0xB);
}

void ov14_021E87BC(UnkStruct_0201956C *a0) {
    sub_020198FC(a0, 0xC, 1, 0, 0xB);
    sub_020198FC(a0, 6, 1, 0, 0xB);
    sub_020198FC(a0, 7, 1, 0, 0xB);
}

void ov14_021E87F4(void *arg0) {
    sub_0201980C(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 8);
    sub_0201980C(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 9);
    ov14_021E821C(arg0);
    ov14_021E827C(arg0);
}

void ov14_021E8824(UnkStruct_ov14_boxsys *arg0) {
    if (arg0->unk_44E_4 != 0) {
        sub_020198FC(arg0->unk_2F0, 0x10, -1, 0, 0xC);
    }
}

void ov14_021E884C(UnkStruct_ov14_boxsys *arg0) {
    if (arg0->unk_44E_4 != 0) {
        sub_020198FC(arg0->unk_2F0, 0x10, 1, 0, 0xC);
    }
}

void ov14_021E8874(void *arg0) {
    if (((UnkStruct_ov14_boxsys *)arg0)->unk_44E_4 != 0) {
        sub_0201980C(((UnkStruct_ov14_boxsys *)arg0)->unk_2F0, 0x10);
        sub_020196E8(((UnkStruct_ov14_boxsys *)arg0)->unk_2F0, 0x10, 0x14, 0xC);
    }
}

void ov14_021E88A4(UnkStruct_0201956C *a0) {
    sub_0201980C(a0, 0x10);
    sub_020196E8(a0, 0x10, 0x20, 0xC);
}

void ov14_021E88BC(UnkStruct_0201956C *a0) {
    s8 v0[2];
    s8 v1;

    sub_02019B1C(a0, 0x10, &v0[1], &v0[0]);
    v1 = v0[0];
    if (v1 != 0xF) {
        sub_020198FC(a0, 0x10, 0, -1, (s32)(u8)(9 - (0x18 - v1)));
    }
}

void ov14_021E88F8(UnkStruct_0201956C *a0) {
    s8 v0[2];
    s8 v1;

    sub_02019B1C(a0, 0x10, &v0[1], &v0[0]);
    v1 = v0[0];
    if (v1 != 0x18) {
        sub_020198FC(a0, 0x10, 0, 1, (s32)(u8)(0x18 - v1));
    }
}

void ov14_021E892C(UnkStruct_0201956C *a0) {
    sub_0201980C(a0, 0x10);
    sub_020196E8(a0, 0x10, 0, 0xF);
}

void ov14_021E8944(UnkStruct_0201956C *a0) {
    sub_0201980C(a0, 0x10);
    sub_020196E8(a0, 0x10, 0, 0x18);
}

void ov14_021E895C(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    u16 fillValue;
    u16 v0;

    v0 = 0;
    do {
        if ((1 << v0) & arg1) {
            fillValue = v0 + 0x3A;
        } else {
            fillValue = v0 + 0x1A;
        }
        FillBgTilemapRect(arg0->sys->unk_14, 5U, fillValue, (u8)(v0 + 0xF), 0x12U, 1U, 1U, 0U);
        v0 += 1;
    } while ((u32)v0 < 6U);
    ScheduleBgTilemapBufferTransfer(arg0->sys->unk_14, 5U);
}
