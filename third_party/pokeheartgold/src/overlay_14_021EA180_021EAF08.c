#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E7AD4(void *, u16, u32, s32);
s32 ov14_021E7FEC(void *);
s32 ov14_021E80A8();
s32 ov14_021F2A04(struct UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021EAF08(UnkStruct_ov14_boxapp *app);
struct UnkStruct_ov14_boxapp;

s32 ov14_021E7B8C(void *);
s32 ov14_021EAF08(UnkStruct_ov14_boxapp *arg0) {
    s32 v4;
    u16 v0;
    u8 v1;
    UnkStruct_ov14_boxsys *v2;
    UnkStruct_ov14_boxsys *v3;

    v3 = arg0->sys;
    v4 = ov14_021E80A8();
    v0 = v3->unk_10;
    switch (v0) {
    case 0:
        if (ov14_021F2A04(arg0->sys, 0xB) == 0) {
            v1 = arg0->unk21;
            arg0->unk21 = (u8) * *(u16 **)((u8 *)v3 + 0xC);
            ov14_021E7AD4(arg0, v1, v1, 1);
            v2 = arg0->sys;
            if (*((u8 *)&v2->unk_448[2]) != 0) {
                *((u8 *)&v2->unk_448[2]) = 0U;
                ov14_021E7FEC(((u8 *)arg0->sys->unk_2F0));
                ov14_021F3488(arg0, 1, 1);
            }
            v3->unk_10 = 1U;
        }
    default:
        break;
    case 1:
        if ((ov14_021E7B8C(arg0) == 0) && (v4 == 0)) {
            v3->unk_10 = 0U;
            return 0;
        }
        break;
    }
    return 1;
}
