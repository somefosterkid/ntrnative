#include "global.h"

#include "overlay_112_ir_state.h"

extern void ov112_021E5A68(void *buf, u32 size, u32 cmd, u32 flag);



void Fail(u32 cmd, u32 code);

void ov112_021E5E18(s32 arg0);

void ov112_021E5A5C(void);

void ov112_021E6BF8(void);

typedef struct BF_ov112_021E6BF8_0 {
    u8 b0 : 1;
    u8 b1 : 1;
} BF_ov112_021E6BF8_0;

void ov112_021E6BF8(void) {
    u8 cmd = (ov112_021FFB24.unk1C);
    u8 *x;
    u8 *y;

    switch (cmd) {
    case 0x32:
        if (ov112_021FFAA4.unk_18 != 0 && ((u32)(ov112_021FFAA4.unk_24[0x5B] << 31) >> 31) != 0) {
            ov112_021E5A68(0, 0, 0x36, 1);
            ov112_021E5E18(4);
            ov112_021E5A5C();
            return;
        }
        ov112_021E5A68(ov112_021FFAA4.unk_20, 0x68, 0x32, 1);
        return;
    case 0x40:
        x = ov112_021FFAA4.unk_24;
        if ((((BF_ov112_021E6BF8_0 *)(&x[0x5B]))->b0) == 0) {
            ov112_021E5A68(0, 0, 0x44, 1);
            ov112_021E5E18(5);
            ov112_021E5A5C();
            return;
        }
        if ((((BF_ov112_021E6BF8_0 *)(&x[0x5B]))->b1) == 0) {
            ov112_021E5A68(0, 0, 0x44, 1);
            ov112_021E5E18(6);
            ov112_021E5A5C();
            return;
        }
        y = ov112_021FFAA4.unk_20;
        if (*(u32 *)(x + 0xC) != *(u32 *)(y + 0xC)) {
            ov112_021E5A68(0, 0, 0x44, 1);
            ov112_021E5E18(0xB);
            ov112_021E5A5C();
            return;
        }
        if (*(u32 *)(x + 4) != *(u32 *)(y + 4)) {
            ov112_021E5A68(0, 0, 0x44, 1);
            ov112_021E5E18(8);
            ov112_021E5A5C();
            return;
        }
        if (((u32) * (u16 *)(x + 0xA) >> 2) != ((u32) * (u16 *)(y + 0xA) >> 2)) {
            ov112_021E5A68(0, 0, 0x44, 1);
            ov112_021E5E18(9);
            ov112_021E5A5C();
            return;
        }
        if (x[0x5D] > y[0x5D]) {
            ov112_021E5A68(0, 0, 0x44, 1);
            ov112_021E5E18(0xA);
            ov112_021E5A5C();
            return;
        }
        ov112_021E5A68(y, 0x68, 0x40, 1);
        return;
    case 0x52:
        x = ov112_021FFAA4.unk_24;
        if ((((BF_ov112_021E6BF8_0 *)(&x[0x5B]))->b0) == 0) {
            ov112_021E5A68(0, 0, 0x56, 1);
            ov112_021E5E18(5);
            ov112_021E5A5C();
            return;
        }
        if ((((BF_ov112_021E6BF8_0 *)(&x[0x5B]))->b1) == 1) {
            ov112_021E5A68(0, 0, 0x56, 1);
            ov112_021E5E18(7);
            ov112_021E5A5C();
            return;
        }
        y = ov112_021FFAA4.unk_20;
        if (*(u32 *)(x + 0) != *(u32 *)(y + 0)) {
            ov112_021E5A68(0, 0, 0x56, 1);
            ov112_021E5E18(8);
            ov112_021E5A5C();
            return;
        }
        if (((u32) * (u16 *)(x + 8) >> 2) != ((u32) * (u16 *)(y + 8) >> 2)) {
            ov112_021E5A68(0, 0, 0x56, 1);
            ov112_021E5E18(9);
            ov112_021E5A5C();
            return;
        }
        if (*(u32 *)(x + 0xC) != *(u32 *)(y + 0xC)) {
            ov112_021E5A68(0, 0, 0x56, 1);
            ov112_021E5E18(0xB);
            ov112_021E5A5C();
            return;
        }
        if (x[0x5D] > y[0x5D]) {
            ov112_021E5A68(0, 0, 0x56, 1);
            ov112_021E5E18(0xA);
            ov112_021E5A5C();
            return;
        }
        ov112_021E5A68(y, 0x68, 0x52, 1);
        return;
    case 0x60:
        x = ov112_021FFAA4.unk_24;
        if ((((BF_ov112_021E6BF8_0 *)(&x[0x5B]))->b0) == 0) {
            y = ov112_021FFAA4.unk_20;
            ov112_021E5A68(y, 0x68, 0x64, 1);
            ov112_021E5E18(5);
            ov112_021E5A5C();
            return;
        }
        y = ov112_021FFAA4.unk_20;
        if (x[0x5D] > y[0x5D]) {
            ov112_021E5A68(y, 0x68, 0x64, 1);
            ov112_021E5E18(0xA);
            ov112_021E5A5C();
            return;
        }
        ov112_021E5A68(y, 0x68, 0x60, 1);
        return;
    case 0xB0:
    case 0xB2:
    case 0xB4:
    case 0xB6:
    case 0xB8:
    case 0xBA:
    case 0xBC:
    case 0xBE:
        x = ov112_021FFAA4.unk_24;
        if ((((BF_ov112_021E6BF8_0 *)(&x[0x5B]))->b0) == 0) {
            ov112_021E5A68(0, 0, 0xD8, 1);
            ov112_021E5E18(5);
            ov112_021E5A5C();
            return;
        }
        y = ov112_021FFAA4.unk_20;
        if (*(u32 *)(x + 0) != *(u32 *)(y + 0)) {
            ov112_021E5A68(0, 0, 0xD8, 1);
            ov112_021E5E18(8);
            ov112_021E5A5C();
            return;
        }
        if (((u32) * (u16 *)(x + 8) >> 2) != ((u32) * (u16 *)(y + 8) >> 2)) {
            ov112_021E5A68(0, 0, 0xD8, 1);
            ov112_021E5E18(9);
            ov112_021E5A5C();
            return;
        }
        if (x[0x5D] > y[0x5D]) {
            ov112_021E5A68(0, 0, 0xD8, 1);
            ov112_021E5E18(0xA);
            ov112_021E5A5C();
            return;
        }
        ov112_021E5A68(y, 0x68, (u8)(cmd - 0x10), 1);
        return;
    default:
        ov112_021E5E18(0xC);
        ov112_021E5A5C();
        return;
    }
}
