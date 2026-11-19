#include "global.h"

void *ov74_0223105C(void);

u8 ov74_0223107C(void);

s32 ov74_02231724(void);

s32 ov74_0222FE78(void);

typedef struct {
    union {
        u8 val1[4];
        u32 val2;
    } unk_00;
} UnkStruct_ov97_022335A8;
typedef struct {
    union {
        u8 val1[2];
        u16 val2;
    } unk_00;
} UnkStruct_ov97_022335C8;
typedef struct {
    UnkStruct_ov97_022335A8 unk_00;
    UnkStruct_ov97_022335C8 unk_04;
    u16 unk_06;
    u8 unk_08;
    u8 unk_09;
    u8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov97_02232254_sub1;
typedef struct {
    UnkStruct_ov97_02232254_sub1 unk_00[8];
    u8 unk_60;
    u8 unk_61;
    u16 unk_62;
    u8 unk_64;
    u8 unk_65[3];
} UnkStruct_ov97_02232254;
UnkStruct_ov97_02232254 *ov74_0223115C(void);
void ov74_0222FEA0(u32 param0);
u32 ov74_0222FEC8(void);
BOOL ov74_0222FF28(void);

void *ov74_02231054(void);
void ov74_0222FF68(void);

s32 ov74_0222FE78(void) {
    void *v0;

    if (ov74_0223107C() == 0xC) {
        v0 = ov74_0223105C();
        if (*(s32 *)((u8 *)v0 + 0x1C) == 0) {
            ov74_02231724();
            *(s32 *)((u8 *)v0 + 0x1C) = 1;
        }
        return 1;
    }
    return 0;
}

void ov74_0222FEA0(u32 param0) {
    UnkStruct_ov97_02232254 *v0 = ov74_0223115C();
    int v1;

    for (v1 = 0; v1 < 8; v1++) {
        if ((v0->unk_00[v1].unk_00.unk_00.val2) && (v0->unk_00[v1].unk_06 == param0)) {
            v0->unk_00[v1].unk_08 = 0;
            v0->unk_00[v1].unk_0A = 0;
        }
    }
}

u32 ov74_0222FEC8(void) {
    UnkStruct_ov97_02232254 *v0 = ov74_0223115C();

    if (v0->unk_62 & 1) {
        v0->unk_62 &= ~1;
        return 1;
    } else if (v0->unk_62 & (1 << 6)) {
        v0->unk_62 &= ~(1 << 6);
        return 7;
    } else if (v0->unk_62 & (1 << 12)) {
        v0->unk_62 &= ~(1 << 12);
        return 13;
    } else {
        return 0;
    }
}

BOOL ov74_0222FF28(void) {
    UnkStruct_ov97_02232254 *v0 = ov74_0223115C();

    v0->unk_60 = 1;
    v0->unk_62 = WM_GetAllowedChannel();

    if ((v0->unk_62 == 0) || (v0->unk_62 == 0x8000)) {
        return 0;
    } else {
        v0->unk_61 = 102;
        v0->unk_64 = 0;
        return 1;
    }
}

void ov74_0222FF68(void) {
    void *v0;

    v0 = ov74_02231054();
    *(u16 *)((u8 *)v0 + 0x18) = WM_GetDispersionBeaconPeriod();
    *(u16 *)((u8 *)v0 + 0xC) = WM_GetNextTgid();
}
