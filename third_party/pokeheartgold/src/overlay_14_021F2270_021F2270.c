#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2270(struct UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2);

u8 sub_02019B10(UnkStruct_0201956C *, s32);


s32 ov14_021F2270(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2) {
    u16 sp4;
    u16 sp2;
    s8 sp0[2];
    s32 v;

    sub_02019B1C(arg0->sys->unk_2F0, arg1, &sp0[1], &sp0[0]);
    sub_02019B44(arg0->sys->unk_2F0, arg1, &sp4, &sp2);
    *((u8 *)&arg0->sys->unk_88D4[0]) = (*((u8 *)&arg0->sys->unk_88D4[0]) & ~1) | 1;
    v = sub_02019B10(arg0->sys->unk_2F0, arg1);
    *((u8 *)&arg0->sys->unk_88D4[0]) = (*((u8 *)&arg0->sys->unk_88D4[0]) & ~0xFE) | ((u32)v << 0x19 >> 0x18);
    *((u8 *)&arg0->sys->unk_88D4[1]) = (*((u8 *)&arg0->sys->unk_88D4[1]) & ~0xF) | 0xD;
    *((u8 *)&arg0->sys->unk_88D4[1]) = (*((u8 *)&arg0->sys->unk_88D4[1]) & ~0xF0) | 0xC0;
    *((s8 *)&arg0->sys->unk_88D4[2]) = 0;
    *((s8 *)&arg0->sys->unk_88D4[3]) = 0;
    *((s8 *)&arg0->sys->unk_88D4[4]) = sp0[1];
    *((s8 *)&arg0->sys->unk_88D4[5]) = sp0[0];
    *((s8 *)&arg0->sys->unk_88D4[6]) = sp4;
    *((s8 *)&arg0->sys->unk_88D4[7]) = sp2;
    *((s32 *)&arg0->unk30) = arg2;
    return 8;
}
