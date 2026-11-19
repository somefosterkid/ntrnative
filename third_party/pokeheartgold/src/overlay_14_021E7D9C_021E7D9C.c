#include "global.h"

#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021E7D9C(void *arg0, u16 arg1);

void ov14_021E7D9C(void *arg0, u16 arg1) {
    u8 *temp_r1;
    u32 temp_r2;
    u16 var_r4;
    u32 temp_r0;
    void *var_r5;
    u8 *sum;
    u8 *new_var;
    var_r5 = arg0;
    var_r4 = arg1;
    temp_r0 = var_r4;
    temp_r1 = (u8 *)sub_02019B08(((UnkStruct_ov14_boxapp *)var_r5)->sys->unk_2F0, 2);
    temp_r2 = 0xB * ((u8)(((temp_r0 >> 1) * 3) + 2));
    temp_r2 <<= 1;
    temp_r1 += temp_r2;
    temp_r0 = ((u32)(((((u32)(temp_r0 << 0x1F)) >> 0x1D) + 3) << 0x18)) >> 0x17;
    new_var = temp_r1 + temp_r0;
    if (!((((UnkStruct_ov14_boxapp *)var_r5)->sys->unk_40C0) & (1 << var_r4))) {
        var_r4 = (u16)(var_r4 + 0xB);
        sum = temp_r1 + temp_r0;
    } else {
        var_r4 = (u16)(var_r4 + 0x2B);
        sum = new_var;
    }
    *((u16 *)sum) = (0xF000 & (*((u16 *)sum))) + var_r4;
}
