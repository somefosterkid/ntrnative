#include "global.h"

#include "overlay_112_ir_state.h"

s32 ov112_021E5A5C(void);

s32 ov112_021E5A68(void *, s32, s32, s32);

s32 ov112_021E5E18(s32);

s32 ov112_021E5E28(void);

s32 ov112_021E5E48(void);



extern u8 ov112_021FFB64[];

void ov112_021E6A6C(u32 arg0);

void ov112_021E6A6C(u32 arg0) {
    u8 sp0[4];
    u64 tick;
    u16 v;

    ov112_021FFAA4.unk_14 = 0;
    if (arg0 == 0) {
        ov112_021E5E18(0xC);
        ov112_021E5A5C();
        return;
    }
    if (arg0 > 2) {
        ov112_021E5E18(0xC);
        ov112_021E5A5C();
        return;
    }
    tick = OS_GetTick();
    ((u32 *)&ov112_021FFAA4.unk_3C)[0] = (u32)tick;
    ((u32 *)&ov112_021FFAA4.unk_3C)[1] = (u32)(tick >> 32);
    switch (ov112_021FFB24.unk1C) {
    case 0xF0:
        ov112_021E5E28();
        MI_CpuCopy8(ov112_021FFAA4.unk_20 + 0x10, ov112_021FFB64, 0x28);
        ov112_021E5A68(ov112_021FFB64, 0x74, ov112_021FFB24.unk1C, 1);
        break;
    case 0xFE:
        ov112_021E5E28();
        ov112_021E5A68(((void *)ov112_021FFAA4.unk_28), 8, ov112_021FFB24.unk1C, 1);
        break;
    case 0xE2:
        sp0[0] = 0x80;
        v = ov112_021FFAA4.unk_00;
        sp0[1] = v >> 8;
        sp0[2] = v;
        sp0[3] = sp0[1] + 1 + sp0[2];
        ov112_021E5A68(sp0, 4, 0xA, 0);
        break;
    case 0xE0:
        ov112_021E5E28();
        ov112_021E5A68(ov112_021FFAA4.unk_20 + 0x10, 0x28, ov112_021FFB24.unk1C, 1);
        break;
    case 0x2A:
        ov112_021E5E28();
        ov112_021E5A68(ov112_021FFAA4.unk_20 + 0x10, 0x28, ov112_021FFB24.unk1D, 1);
        ov112_021E5E18(0xF);
        ov112_021E5A5C();
        break;
    case 0x32:
    case 0x40:
    case 0x52:
        ov112_021E5E48();
        ov112_021E5A68(NULL, 0, 0x20, 1);
        break;
    case 0x60:
    case 0xB0:
    case 0xB2:
    case 0xB4:
    case 0xB6:
    case 0xB8:
    case 0xBA:
    case 0xBC:
    case 0xBE:
        *(s32 *)(ov112_021FFAA4.unk_20 + 0x60) = 0;
        ov112_021E5A68(NULL, 0, 0x20, 1);
        break;
    }
}
