/* PORTOK hk39 */
#include "unk_02005D10.h"
s32 ov28_0225E374(void *, void *, s32 *);

void ov28_0225E730(void *arg0) {
    u16 sp0[2];
    s16 pitch;
    s32 y;
    s32 d;
    u16 cnt;

    ov28_0225E374(arg0, &sp0[1], (s32 *)sp0);
    pitch = ((sp0[1] + (sp0[0] * 2)) << 6) - 0x3C0;
    if (*(s32 *)((u8 *)arg0 + 0x210) == 4) {
        y = (*(u32 *)((u8 *)arg0 + 0x218) >> 3) - 5;
        d = abs((*(s16 *)((u8 *)arg0 + 0x208) + 7) - ((*(u32 *)((u8 *)arg0 + 0x214) >> 3) - 3));
        pitch += (0x10 - (d + abs((*(s16 *)((u8 *)arg0 + 0x20A) + 7) - y))) << 6;
    }
    if (*(u16 *)((u8 *)arg0 + 0x338) == 0) {
        PlaySE_SetPitch(0x93D, pitch);
    }
    cnt = *(u16 *)((u8 *)arg0 + 0x338);
    if (cnt == 10) {
        cnt = 0;
    } else {
        cnt = cnt + 1;
    }
    *(u16 *)((u8 *)arg0 + 0x338) = cnt;
    if (*(s16 *)((u8 *)arg0 + 0x33A) != pitch) {
        *(s16 *)((u8 *)arg0 + 0x33A) = pitch;
    }
}
