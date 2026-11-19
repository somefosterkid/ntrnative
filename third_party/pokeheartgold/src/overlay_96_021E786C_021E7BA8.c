#include "global.h"

typedef struct UnkStruct_ov96_021E7BA8_temp_r6 {
    u8 unk_0[0x8B4];
    u16 unk_8B4;
    u8 unk_8B6[0x1E];
    u16 unk_8D4;
} UnkStruct_ov96_021E7BA8_temp_r6;

s32 ov96_021E7D18(s32, s32, s32);

extern void *_0221A7D8;

void ov96_021E7BA8(u8 *arg0, void *arg1, void *arg2);

void ov96_021E7BA8(u8 *arg0, void *arg1, void *arg2) {
    void *sp4;
    void *sp0;
    u8 temp_r5;
    u8 var_r4;
    UnkStruct_ov96_021E7BA8_temp_r6 *temp_r6;

    sp0 = arg1;
    sp4 = arg2;
    var_r4 = 0;
    do {
        temp_r5 = (u8) * (s32 *)((u8 *)(arg0 + (var_r4 * 4)) + 0x3D8);
        temp_r6 = (void *)(arg0 + (var_r4 * 2));
        if (ov96_021E7D18(*(u16 *)(sp0 + (temp_r5 * 0x2C)), temp_r6->unk_8D4, *(u8 *)(((u8 *)&_0221A7D8) + temp_r5)) != 0) {
            *(u16 *)(sp4 + (temp_r5 * 2)) = temp_r6->unk_8B4;
        }
        var_r4 += 1;
    } while ((u32)var_r4 < 3U);
}
