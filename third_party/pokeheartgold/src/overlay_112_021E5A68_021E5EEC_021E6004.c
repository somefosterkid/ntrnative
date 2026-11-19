#include "global.h"

#include "overlay_112_ir_state.h"

s32 ov112_021E5A5C(void);

s32 ov112_021E5A68(void *, s32, s32, s32);

s32 ov112_021E5E18(s32);

void ov112_021E5EC4(u8 *, u8 *, u16);

s32 ov112_021E5EEC(void);

s32 ov112_021E6134(void);



void ov112_021E6004(void);

void ov112_021E6004(void) {
    u64 temp_ret_64;
    float temp_f;

    temp_ret_64 = OS_GetTick() - ov112_021FFAA4.unk_3C;
    temp_f = (float)((temp_ret_64 << 6) / 0x82EA) / 1000.0f;
    switch (ov112_021FFB24.unk1D) {
    case 0x3A:
        ov112_021FFB24.unk1D = 0x3CU;
        if (ov112_021FFAA4.unk_58 != NULL) {
            ov112_021E5EC4(ov112_021FFAA4.unk_58, (u8 *)0xD700, 0x28BE);
        } else {
            ov112_021E5EC4(ov112_021FFAA4.unk_2C, (u8 *)0xD700, 0x28BE);
        }
        ov112_021E5EEC();
        return;
    case 0x3C:
        ov112_021FFB24.unk1D = 0x3EU;
        MI_CpuCopy8(ov112_021FFAA4.unk_24 + 0x10, ov112_021FFAA4.unk_30 + 8, 0x28U);
        ov112_021E5EC4(ov112_021FFAA4.unk_30, (u8 *)0xD480, 0x224);
        ov112_021E5EEC();
        return;
    case 0x3E:
        if (ov112_021FFB24.unk1C == 0x52) {
            ov112_021FFB24.unk1D = 0x48U;
            ov112_021E5EC4((u8 *)0xCE80, ov112_021FFAA4.unk_38, 0xD4C);
            ov112_021E6134();
            return;
        }
        ov112_021E5A68(0, 0, 0x24, 1);
        return;
    case 0xB0:
    case 0xB2:
    case 0xB4:
    case 0xB6:
    case 0xB8:
    case 0xBA:
    case 0xBC:
    case 0xBE:
        ov112_021E5A68(0, 0, 0x24, 1);
        return;
    default:
        ov112_021E5E18(12);
        ov112_021E5A5C();
        return;
    }
}
