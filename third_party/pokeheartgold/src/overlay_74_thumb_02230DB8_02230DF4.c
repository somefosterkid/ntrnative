#include "global.h"

typedef struct {
    u16 unk_00[12];
} UnkStruct_ov97_02233268_sub2_sub1;

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
    UnkStruct_ov97_02233268_sub2_sub1 unk_04;
    UnkStruct_ov97_02233268_sub2_sub1 unk_1C;
    UnkStruct_ov97_022335C8 unk_34;
    u8 unk_36;
    u8 unk_37;
} UnkStruct_ov97_02232620_sub1;

typedef struct {
    UnkStruct_ov97_02232620_sub1 unk_00[8];
    u16 unk_1C0;
    u8 unk_1C2;
    u8 unk_1C3_0 : 4;
    u8 unk_1C3_4 : 4;
} UnkStruct_ov97_02232620;

u8 ov74_02231064(void);

u8 ov74_02231094(void);

UnkStruct_ov97_02232620 *ov74_02231154(void);

BOOL ov74_02231584(void);

void ov74_02230DF4(u32 param0);

void ov74_02230DF4(u32 param0) {
    if ((ov74_02231094() == 1) && (ov74_02231064() == 7)) {
        UnkStruct_ov97_02232620 *v0 = ov74_02231154();

        if ((v0->unk_1C3_4 == 1) && (v0->unk_00[param0].unk_00.unk_00.val2)) {
            v0->unk_1C3_0 = (u8)param0;
            v0->unk_1C0 = 120;

            ov74_02231584();
        }
    }
}

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

u32 ov74_02230E44(void);

u32 ov74_02230E44(void) {
    int v0 = ov74_02231094();
    u32 v1 = 0;

    if (v0 == 2) {
        UnkStruct_ov97_02232254 *v2 = ov74_0223115C();

        if (v2->unk_60 == 2) {
            u32 i;

            for (i = 0; i < 8; i++) {
                if ((v2->unk_00[i].unk_00.unk_00.val2 != 0) && (v2->unk_00[i].unk_0A)) {
                    v1++;
                }
            }
        }
    }

    return v1;
}
