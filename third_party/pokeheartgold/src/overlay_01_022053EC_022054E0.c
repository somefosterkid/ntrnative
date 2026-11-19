#include "global.h"

#include "error_handling.h"
#include "field_types_def.h"

u8 ov01_02205584(LocalMapObject *arg0);

u32 sub_020659A8(LocalMapObject *arg0);

u8 sub_0206599C(LocalMapObject *);

typedef struct {
    s32 a[48];
} ov01_02209750_Array;

extern const ov01_02209750_Array ov01_02209750;

s32 ov01_022054E0(LocalMapObject *arg0);

s32 ov01_022054E0(LocalMapObject *arg0) {
    s32 sp0[48];
    s32 temp_r0;
    u8 temp_r5;
    *(ov01_02209750_Array *)sp0 = ov01_02209750;
    if (ov01_02205584(arg0)) {
        return 0;
    }
    temp_r5 = (u8)sub_020659A8(arg0);
    if (temp_r5 == 0) {
        return 0;
    }
    temp_r0 = sub_0206599C(arg0);
    if (temp_r0 >= 0x10) {
        GF_AssertFail();
    }
    return *((s32 *)((u8 *)sp0 + ((u32)((temp_r5 - 1) << 0x18) >> 0x12) + temp_r0 * 4));
}
