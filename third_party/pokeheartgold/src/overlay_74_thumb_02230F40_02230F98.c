#include "global.h"

typedef struct {
    void *unk_00;
    void *unk_04;
    void *unk_08;
    void *unk_0C;
    u32 unk_10;
    u32 unk_14;
    vu8 unk_18;
    vu8 unk_19;
    u8 unk_1A;
    u8 unk_1B;
    u8 unk_1C;
} UnkStruct_ov97_02232AC8;

u8 ov74_02231064(void);

u8 ov74_02231094(void);

UnkStruct_ov97_02232AC8 *ov74_02231184(void);

typedef struct {
    u32 unk_00_0 : 8;
    u32 unk_00_8 : 4;
    u32 unk_00_12 : 4;
    u32 unk_00_16 : 16;
} UnkStruct_ov97_02233674_sub1;

typedef struct {
    u32 unk_00_0 : 8;
    u32 unk_00_8 : 4;
    u32 unk_00_12 : 4;
    u32 unk_00_16 : 16;
} UnkStruct_ov97_02233674_sub2;

typedef struct {
    u32 unk_00_0 : 8;
    u32 unk_00_8 : 8;
    u32 unk_00_16 : 16;
} UnkStruct_ov97_02233674_sub3;

typedef struct {
    u32 unk_00_0 : 8;
    u32 unk_00_8 : 24;
} UnkStruct_ov97_02233674_sub4;

typedef struct {
    UnkStruct_ov97_02233674_sub1 unk_00;
    UnkStruct_ov97_02233674_sub2 unk_04;
    UnkStruct_ov97_02233674_sub3 unk_08;
    UnkStruct_ov97_02233674_sub4 unk_0C;
} UnkStruct_ov97_02233674;

BOOL ov74_02230F98(void);

BOOL ov74_02230F98(void) {
    switch (ov74_02231094()) {
    case 1:
    case 2: {
        switch (ov74_02231064()) {
        case 11:
        case 10: {
            UnkStruct_ov97_02232AC8 *v0 = ov74_02231184();

            if (v0->unk_19 == 0) {
                UnkStruct_ov97_02233674 *v1 = (UnkStruct_ov97_02233674 *)v0->unk_0C;

                if ((v1->unk_08.unk_00_8 == 0) && (v0->unk_1B < 4)) {
                    return 1;
                }
            }
        } break;
        }
    } break;
    }

    return 0;
}

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

BOOL ov74_02231118(void);

UnkStruct_ov97_02232254 *ov74_0223115C(void);

BOOL ov74_02230FD4(void);

BOOL ov74_02230FD4(void) {
    switch (ov74_02231094()) {
    case 2: {
        UnkStruct_ov97_02232254 *v0 = ov74_0223115C();
        int v1;

        for (v1 = 0; v1 < 8; v1++) {
            if (v0->unk_00[v1].unk_00.unk_00.val2) {
                if (v0->unk_00[v1].unk_0A) {
                    return 1;
                }
            }
        }

        return 0;
    } break;

    default:
        return ov74_02231118();
        break;
    }
}

s32 *ov74_0223105C(void);

s32 ov74_02231088(s8);

s32 ov74_0223113C(s32);

s32 ov74_02231724(void);

void ov74_02231008(void);

void ov74_02231008(void) {
    s32 v2;
    void *v3;

    ov74_02231088(0xC);
    v2 = ov74_02231064();
    switch (v2) {
    case 0:
    case 1:
        WM_Finish();
        ov74_0223113C(1);
        return;
    case 9:
        if (ov74_02231118() == 0) {
            v3 = ov74_0223105C();
            ov74_02231724();
            *(s32 *)((u8 *)v3 + 0x1C) = 1;
        }
        return;
    }
}

s32 ov74_02231048(void);

s32 ov74_02231048(void) {
    return *(s32 *)((u8 *)(ov74_0223105C()) + 0x20);
}

extern s32 ov74_0223C920;

s32 *ov74_02231054(void);

s32 *ov74_02231054(void) {
    return &ov74_0223C920;
}

extern s32 ov74_0223D0C4;

s32 *ov74_0223105C(void) {
    return &ov74_0223D0C4;
}
