#include "global.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"


s32 ov14_021E8544(UnkStruct_0201956C *arg0) {
    s8 sp0[4];
    s32 var_r0;

    sub_02019B1C(arg0, 3, sp0 + 1, sp0);
    var_r0 = sp0[1] == 0x20 ? 0 : 1;
    return var_r0;
}
