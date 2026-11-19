#include "global.h"

#include "overlay_112_ir_state.h"

void ov112_021E5D8C(u8 *arg0, u8 *arg1, char arg2, u8 *arg3);

void ov112_021E5E18(s32 arg0);

void ov112_021E5D8C(u8 *arg0, u8 *arg1, char arg2, u8 *arg3) {
    s32 v2;
    u32 v1;
    u32 v0;
    s32 v5;
    s32 v6;
    u32 v3;
    u32 size;
    u8 *v7;
    u8 *src;
    u8 *dest;
    u8 v4;
    v0 = arg2;
    dest = arg1;
    src = arg0;
    v5 = v0 + 0x7F;
    v2 = 0;
    v3 = ((u32)((v5 + (((u32)(v5 >> 6)) >> 0x19)) << 9)) >> 0x10;
    v7 = arg3;
    v1 = v3;
    if (((s32)v3) > 0) {
        do {
            size = v0;
            if (size > 0x80U) {
                size = 0x80;
            }
            if (size != 0x80) {
                MI_CpuCopy8(src, dest, size);
                *v7 = 0x80;
                v7 += 1;
            } else {
                v4 = (u8)MI_CompressLZImpl(src, size, dest, 0);
                if (v4 == 0) {
                    MI_CpuCopy8(src, dest, size);
                    *v7 = 0x80;
                    v7 += 1;
                } else {
                    *v7 = v4;
                    v7 += 1;
                }
            }
            src += 0x80;
            v0 = (u32)((u16)(v0 - size));
            dest += 0x80;
            v6 = v2 + 1;
            v2 = v6;
        } while (v6 < ((s32)v1));
    }
}

void ov112_021E5E18(s32 arg0) {
    if (ov112_021FFAA4.unk_14 == 0) {
        ov112_021FFAA4.unk_14 = arg0;
    }
}
