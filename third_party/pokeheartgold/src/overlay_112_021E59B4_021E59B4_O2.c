#include "global.h"

s32 CARD_SpiWaitWriteRange(u8 *, u8);

s32 ov112_021E5900(u8 *, s32);

extern u8 ov112_021FFA18[];

extern u8 ov112_021FFA20[];

void ov112_021E59B4(u8 *arg0, u8 arg1, s8 arg2, s8 arg3, s32 arg4);

void ov112_021E59B4(u8 *arg0, u8 arg1, s8 arg2, s8 arg3, s32 arg4) {
    s32 i;
    s32 crc;
    u8 *hdr = ov112_021FFA18;
    u8 *dst;

    hdr[0] = arg2;
    hdr[1] = arg3;
    dst = ov112_021FFA20;
    *(s32 *)(hdr + 4) = arg4;
    hdr[2] = 0;
    hdr[3] = 0;
    i = 0;
    while (i < arg1) {
        *dst++ = *arg0++;
        i++;
    }
    arg1 = (u8)(arg1 + 8);
    crc = ov112_021E5900(ov112_021FFA18, arg1);
    hdr[2] = crc;
    hdr[3] = crc >> 8;
    i = 0;
    while (i < arg1) {
        ov112_021FFA18[i] ^= 0xAA;
        i++;
    }
    CARD_SpiWaitWriteRange(ov112_021FFA18, arg1);
}
