#include "global.h"

#include "item.h"
#include "overlay_14_box_funcs.h"

u32 ov14_021E6070(UnkStruct_ov14_boxapp *, s32, s32, void *);
s32 ov14_021E6AA0(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2);

s32 ov14_021E6AA0(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2) {
    s32 sp0;
    u16 temp_r0;
    u16 temp_r7;

    if (ov14_021E6070(arg0, arg2, 0xAC, 0) == 0) {
        return 0;
    }
    if (ov14_021E6070(arg0, arg2, 0x4C, 0) != 0) {
        return 0;
    }
    temp_r0 = ov14_021E6070(arg0, arg1, 6, 0);
    sp0 = (s32)temp_r0;
    if (ItemIdIsMail(temp_r0) == 1) {
        return 0;
    }
    temp_r7 = ov14_021E6070(arg0, arg2, 6, 0);
    if (ItemIdIsMail(temp_r7) == 1) {
        return 0;
    }
    if ((sp0 == 0x70) && (ov14_021E6070(arg0, arg2, 5, 0) != 0x1E7)) {
        return 0;
    }
    if ((temp_r7 == 0x70) && (ov14_021E6070(arg0, arg1, 5, 0) != 0x1E7)) {
        return 0;
    }
    return 1;
}
