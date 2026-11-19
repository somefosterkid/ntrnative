#include "global.h"

#include "overlay_112_ir_state.h"

void ov112_021E5A5C(void);

void ov112_021E5E18(s32 arg0);

void ov112_021E6134(void);

s32 ov112_021E5A68(void *, s32, s32, s32);

void ov112_021E5EC4(u8 *, u8 *, u16);

s32 ov112_021E5EEC(void);

s32 ov112_021E6004(void);

s32 ov112_021E6164();

s32 ov112_021E6BF8(void);





void ov112_021E6F60(void *arg0, u32 arg1, s32 arg2);

void ov112_021E6F60(void *arg0, u32 arg1, s32 arg2) {
    u32 temp_r3;
    u32 *p;
    u8 b;

    switch (arg2) {
    case 0x22:
        MI_CpuCopy8(arg0, ov112_021FFAA4.unk_24, 0x68U);
        p = (u32 *)ov112_021FFAA4.unk_24;
        temp_r3 = p[0x19];
        p[0x19] = (u32)((u8)(temp_r3 >> 0x18) | ((0xFF000000U >> 0x10) & (temp_r3 >> 8)) | ((temp_r3 << 8) & (0xFF000000U >> 8)) | ((temp_r3 << 0x18) & 0xFF000000));
        ov112_021E6BF8();
        break;
    case 0x26:
        b = ov112_021FFB24.unk1C;
        switch (b) {
        case 0x32:
            ov112_021E5A68(0, 0, 0x38, 1);
            break;
        case 0x40:
            ov112_021E5A68(0, 0, 0x4E, 1);
            ov112_021E5E18(0xF);
            ov112_021E5A5C();
            break;
        case 0x52:
            ov112_021E5A68(0, 0, 0x5A, 1);
            break;
        case 0x60:
            ov112_021E5A68(0, 0, 0x66, 1);
            ov112_021E5E18(0xF);
            ov112_021E5A5C();
            break;
        case 0xB0:
            ov112_021E5A68(0, 0, 0xC0, 1);
            break;
        case 0xB2:
            ov112_021E5A68(0, 0, 0xC2, 1);
            break;
        case 0xB4:
            ov112_021E5A68(0, 0, 0xC4, 1);
            break;
        case 0xB6:
            ov112_021E5A68(0, 0, 0xC6, 1);
            break;
        case 0xB8:
        case 0xBA:
        case 0xBC:
        case 0xBE:
            if (((u32)(*ov112_021FFAA4.unk_34 << 28) >> 28) == 0xF) {
                switch (ov112_021FFAA4.unk_04) {
                case 0:
                    ov112_021E5A68(0, 0, 0xD0, 1);
                    break;
                case 1:
                    ov112_021E5A68(0, 0, 0xD2, 1);
                    break;
                case 2:
                    ov112_021E5A68(0, 0, 0xD4, 1);
                    break;
                case 3:
                    ov112_021E5A68(0, 0, 0xD6, 1);
                    break;
                }
            } else {
                ov112_021E5A68(0, 0, b, 1);
            }
            break;
        }
        break;
    case 0x9E:
        ov112_021E5E18(0xC);
        ov112_021E5A5C();
        break;
    case 0x9C:
        ov112_021E5E18(0xC);
        ov112_021E5A5C();
        break;
    case 0x28:
        ov112_021E5E18(3);
        ov112_021E5A5C();
        break;
    case 0x34:
        ov112_021FFB24.unk1D = 0x3A;
        if (ov112_021FFAA4.unk_50 != NULL) {
            ov112_021E5EC4(ov112_021FFAA4.unk_50, (u8 *)0x280, 0x8C50);
        } else {
            ov112_021E5EC4(ov112_021FFAA4.unk_10, (u8 *)0x280, 0x8C50);
        }
        ov112_021E5EEC();
        break;
    case 0x38:
        ov112_021E5E18(0xF);
        ov112_021E5A5C();
        break;
    case 0x42:
        ov112_021FFB24.unk1D = 0x46;
        ov112_021E5EC4((u8 *)0x8F00, ov112_021FFAA4.unk_08, 0x2A);
        ov112_021E6134();
        break;
    case 0x44:
        ov112_021E5E18(0xE);
        ov112_021E5A5C();
        break;
    case 0x54:
        ov112_021FFB24.unk1D = 0x3C;
        if (ov112_021FFAA4.unk_58 != NULL) {
            ov112_021E5EC4(ov112_021FFAA4.unk_58, (u8 *)0xD700, 0x28BE);
        } else {
            ov112_021E5EC4(ov112_021FFAA4.unk_2C, (u8 *)0xD700, 0x28BE);
        }
        ov112_021E5EEC();
        break;
    case 0x5A:
        ov112_021E5E18(0xF);
        ov112_021E5A5C();
        break;
    case 0x5E:
        ov112_021E5E18(0xF);
        ov112_021E5A5C();
        break;
    case 0x62:
        ov112_021FFB24.unk1D = 0x48;
        ov112_021E5EC4((u8 *)0xCE80, ov112_021FFAA4.unk_38, 0xD4C);
        ov112_021E6134();
        break;
    case 0x64:
        ov112_021E5E18(0xE);
        ov112_021E5A5C();
        break;
    case 0xA0:
    case 0xA2:
    case 0xA4:
    case 0xA6:
    case 0xA8:
    case 0xAA:
    case 0xAC:
    case 0xAE:
        ov112_021FFB24.unk1D = 0x48;
        ov112_021E5EC4((u8 *)0xCE80, ov112_021FFAA4.unk_38, 0xD4C);
        ov112_021E6134();
        break;
    case 0xC0:
    case 0xC2:
    case 0xC4:
    case 0xC6:
    case 0xC8:
    case 0xCA:
    case 0xCC:
    case 0xCE:
        ov112_021E5E18(0xF);
        ov112_021E5A5C();
        break;
    case 0xF0:
        ov112_021E5E18(0xF);
        ov112_021E5A5C();
        break;
    case 0xFE:
        ov112_021E5E18(0xF);
        ov112_021E5A5C();
        break;
    case 0xE2:
        ov112_021E5E18(0xF);
        ov112_021E5A5C();
        break;
    case 0xE0:
        ov112_021E5E18(0xF);
        ov112_021E5A5C();
        break;
    case 0x04:
        if (ov112_021FFB24.unk30 == 0) {
            ov112_021E6004();
        } else {
            ov112_021E5EEC();
        }
        break;
    case 0x0E:
        MI_CpuCopy8(arg0, (void *)ov112_021FFB24.unk28, arg1);
        ov112_021FFB24.unk20 += arg1;
        ov112_021FFB24.unk28 += arg1;
        ov112_021FFB24.unk30 -= arg1;
        ov112_021FFB24.unk36 += 1;
        ov112_021FFB24.unk3A += 1;
        if (ov112_021FFB24.unk30 == 0) {
            ov112_021E6164();
        } else {
            ov112_021E6134();
        }
        break;
    case 0xF4:
        break;
    }
}
