#include "global.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"

s32 ov91_0225D6B8(void *, s32 *, void *, s32, s32, s32, s32);

void ov91_0225D628(u8 *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4);

s32 ov91_0225D750(void *);
void ov91_0225D67C(u8 *arg0);

void ov91_0225D628(u8 *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4) {
    u32 v1[12];
    s32 v0;
    s32 v2;
    s32 v3;
    u8 *v4;

    v4 = arg0;
    (*(s32 *)((u32 *)v1)) = *((s32 *)&((UnkStruct_ov115_02261ADC *)arg1)->unk_1C);
    (*(void **)((u8 *)((u32 *)v1) + 4)) = arg1 + 0x158;
    (*(s32 *)((u8 *)((u32 *)v1) + 20)) = 0x1000;
    (*(s32 *)((u8 *)((u32 *)v1) + 24)) = 0x1000;
    (*(s32 *)((u8 *)((u32 *)v1) + 28)) = 0x1000;
    v3 = 0;
    *(s16 *)((u8 *)((s32 *)((u32 *)v1)) + 0x20) = 0;
    v2 = arg2;
    v0 = arg3;
    (*(s32 *)((u8 *)((u32 *)v1) + 36)) = 0;
    (*(s32 *)((u8 *)v1 + 40)) = 1;
    (*(s32 *)((u8 *)v1 + 44)) = arg4;
    do {
        ov91_0225D6B8(v4, ((s32 *)((u32 *)v1)), arg1, v2, v3, v0, arg4);
        v3 += 1;
        v4 += 0x40;
    } while (v3 < 4);
}

void ov91_0225D67C(u8 *arg0) {
    s32 v0;
    u8 *v1;

    v1 = arg0;
    v0 = 0;
    do {
        ov91_0225D750(v1);
        v0 += 1;
        v1 += 0x40;
    } while (v0 < 4);
}
