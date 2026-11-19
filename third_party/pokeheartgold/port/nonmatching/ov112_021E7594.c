#include "global.h"
void ov112_021E5D8C(u8 *src, u8 *dest, u32 size, u8 *flags);
s32 ov112_021E5EC4(s32, s32, s32);
extern u8 ov112_021FFAA4[];
extern u8 ov112_021FFB24[];

void ov112_021E7594(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    *(s32 *)(ov112_021FFAA4 + 0x2C) = arg0;
    *(s32 *)(ov112_021FFAA4 + 0x30) = arg1;
    *(s32 *)(ov112_021FFAA4 + 0x38) = arg2;
    *(s32 *)(ov112_021FFAA4 + 0x34) = arg3;
    ov112_021FFB24[0x1C] = 0x52;
    ov112_021FFB24[0x1D] = 0x20;
    ov112_021E5EC4(0, 0, 0xD4C);
    *(u16 *)(ov112_021FFB24 + 0x3C) = 0x382E;
    *(u16 *)(ov112_021FFB24 + 0x3A) = 0;
    *(u16 *)(ov112_021FFB24 + 0x38) = (*(u16 *)(ov112_021FFB24 + 0x3C) + 0x7F) / 0x80;
    if (*(u8 **)(ov112_021FFAA4 + 0x48) != NULL) {
        *(u8 **)(ov112_021FFAA4 + 0x54) = *(u8 **)(ov112_021FFAA4 + 0x44) + 0x119;
        *(u8 **)(ov112_021FFAA4 + 0x58) = *(u8 **)(ov112_021FFAA4 + 0x48) + 0x8C80;
        ov112_021E5D8C(*(u8 **)(ov112_021FFAA4 + 0x2C), *(u8 **)(ov112_021FFAA4 + 0x58), 0x28BE, *(u8 **)(ov112_021FFAA4 + 0x54));
        *(u8 **)(ov112_021FFAA4 + 0x5C) = *(u8 **)(ov112_021FFAA4 + 0x54) + 0x52;
        *(u8 **)(ov112_021FFAA4 + 0x60) = *(u8 **)(ov112_021FFAA4 + 0x58) + 0x2900;
        ov112_021E5D8C(*(u8 **)(ov112_021FFAA4 + 0x30), *(u8 **)(ov112_021FFAA4 + 0x60), 0x224, *(u8 **)(ov112_021FFAA4 + 0x5C));
    }
}
