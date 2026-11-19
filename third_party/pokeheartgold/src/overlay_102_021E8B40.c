#include "global.h"

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    int unk_08;
} UnkStruct_ov20_021D18BC;

s32 ov102_021E8798(u16 *arg0);

s32 ov102_021E8C3C(void *arg0);

s32 ov102_021E8C64(UnkStruct_ov20_021D18BC *);

s32 ov102_021E8B40(UnkStruct_ov20_021D18BC *param0, u16 pressedKeysRepeatable);

s32 ov102_021E8B40(UnkStruct_ov20_021D18BC *param0, u16 pressedKeysRepeatable) {
    u16 v3;
    u32 v0;
    u8 v1;
    u8 v2;

    if (0x80 & pressedKeysRepeatable) {
        if (*(u16 *)((u8 *)param0 + 0xC) != 0) {
            return 4;
        }
        v2 = param0->unk_07;
        if ((u32)v2 < 4U) {
            param0->unk_07 = v2 + 1;
            v0 = (u32)ov102_021E8798(&param0->unk_00);
            v3 = param0->unk_04;
            if (v0 < (u32)v3) {
                return 1;
            }
            if ((v0 == v3) && (v0 & 1)) {
                param0->unk_06 = 0;
                return 1;
            }
            param0->unk_07 -= 1;
            *(u16 *)((u8 *)param0 + 0xC) = 1U;
            return 1;
        }
        if ((u32)param0->unk_00 < (u32)param0->unk_02) {
            param0->unk_08 = 1;
            param0->unk_00 += 1;
            if ((u32)ov102_021E8798(&param0->unk_00) < (u32)param0->unk_04) {
                return 2;
            }
            param0->unk_06 = 0;
            return 3;
        }
        *(u16 *)((u8 *)param0 + 0xC) = 1U;
        return 1;
    }
    if (pressedKeysRepeatable & 0x40) {
        if (*(u16 *)((u8 *)param0 + 0xC) != 0) {
            *(u16 *)((u8 *)param0 + 0xC) = 0U;
            return 1;
        }
        v1 = param0->unk_07;
        if (v1 != 0) {
            param0->unk_07 = v1 - 1;
            return 1;
        }
        if (param0->unk_00 != 0) {
            param0->unk_08 = -1;
            param0->unk_00 -= 1;
            return 2;
        }
        return 4;
    }
    if (pressedKeysRepeatable & 0x30) {
        param0->unk_06 ^= 1;
        if ((u32)ov102_021E8798(&param0->unk_00) < (u32)param0->unk_04) {
            return 1;
        }
        param0->unk_06 ^= 1;
        return 4;
    }
    if (0x200 & pressedKeysRepeatable) {
        return ov102_021E8C3C(param0);
    }
    if (pressedKeysRepeatable & 0x100) {
        return ov102_021E8C64(param0);
    }
    return 0;
}

s32 ov102_021E8C3C(void *arg0) {
    u16 v0;

    v0 = *(u16 *)arg0;
    if (v0 != 0) {
        if ((u32)v0 >= 5U) {
            *(u16 *)arg0 = (u16)(v0 - 5);
            *(s32 *)((u8 *)arg0 + 8) = -5;
        } else {
            *(s32 *)((u8 *)arg0 + 8) = (s32)(0 - v0);
            *(u16 *)arg0 = 0U;
        }
        return 2;
    }
    return 4;
}
