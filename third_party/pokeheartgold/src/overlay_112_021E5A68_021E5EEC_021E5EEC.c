#include "global.h"

#include "overlay_112_ir_state.h"

s32 ov112_021E5A68(s8 *, s32, s32, s32);





void ov112_021E5EEC(void);

void ov112_021E5EEC(void) {
    s8 sp0[125];
    u8 **var_r0;
    s32 temp_r1;
    u16 temp_r4;
    u16 var_r5;
    u8 temp_r6;

    temp_r4 = (u16) ((s32)ov112_021FFB24.unk28);
    var_r5 = ov112_021FFB24.unk30;
    if ((u32)var_r5 > 0x80U) {
        var_r5 = 0x80;
    }
    var_r0 = NULL;
    switch (ov112_021FFB24.unk1D) {
    case 0x3A:
        if (ov112_021FFAA4.unk_50 != NULL) {
            var_r0 = &ov112_021FFAA4.unk_4C;
        }
        break;
    case 0x3C:
        if (ov112_021FFAA4.unk_58 != NULL) {
            var_r0 = &ov112_021FFAA4.unk_54;
        }
        break;
    }
    if (var_r0 == NULL) {
        temp_r1 = temp_r4 & 0x7F;
        if (temp_r1 != 0) {
            var_r5 = 0x80 - temp_r1;
            sp0[0] = (s8)temp_r4;
            MI_CpuCopy8(((s8 *)ov112_021FFB24.unk20), sp0 + 1, (u32)var_r5);
            ov112_021E5A68(sp0, (u8)(var_r5 + 1), 0xAU, (u8)((s32)temp_r4 >> 8));
        } else {
            ov112_021E5A68(((s8 *)ov112_021FFB24.unk20), (u8)var_r5, (u8)(2 | (temp_r4 & 0x80)), (u8)((s32)temp_r4 >> 8));
        }
    } else {
        temp_r6 = *(u8 *)(*var_r0 + ov112_021FFB24.unk36);
        if ((u32)temp_r6 > 0x80U) {
            OS_Halt();
        }
        if (temp_r6 == 0x80) {
            ov112_021E5A68(((s8 *)ov112_021FFB24.unk20), 0x80U, (u8)(2 | (temp_r4 & 0x80)), (u8)((s32)temp_r4 >> 8));
        } else {
            ov112_021E5A68(((s8 *)ov112_021FFB24.unk20), temp_r6, (u8)(0x80 & temp_r4), (u8)((s32)temp_r4 >> 8));
        }
    }
    ov112_021FFB24.unk20 = (u8 *)(&(((s8 *)ov112_021FFB24.unk20))[var_r5]);
    ov112_021FFB24.unk28 = ov112_021FFB24.unk28 + var_r5;
    ov112_021FFB24.unk30 = (u16)(ov112_021FFB24.unk30 - var_r5);
    ov112_021FFB24.unk36 = (u16)(ov112_021FFB24.unk36 + 1);
    ov112_021FFB24.unk3A = (u16)(ov112_021FFB24.unk3A + 1);
}
