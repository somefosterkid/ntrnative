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

typedef struct {
    u32 unk_00_0 : 8;
    u32 unk_00_8 : 4;
    u32 unk_00_12 : 4;
    u32 unk_00_16 : 16;
} UnkStruct_ov97_02233268_sub1_sub1;

typedef struct {
    u32 unk_00_0 : 8;
    u32 unk_00_8 : 4;
    u32 unk_00_12 : 4;
    u32 unk_00_16 : 16;
} UnkStruct_ov97_02233268_sub1_sub2;

typedef struct {
    UnkStruct_ov97_02233268_sub1_sub1 unk_00;
    UnkStruct_ov97_02233268_sub1_sub2 unk_04;
} UnkStruct_ov97_02233268_sub1;

typedef struct {
    u16 unk_00[12];
} UnkStruct_ov97_02233268_sub2_sub1;

typedef struct {
    UnkStruct_ov97_02233268_sub2_sub1 unk_00;
    UnkStruct_ov97_02233268_sub2_sub1 unk_18;
} UnkStruct_ov97_02233268_sub2;

typedef struct {
    UnkStruct_ov97_02233268_sub1 unk_00;
    UnkStruct_ov97_02233268_sub2 unk_08;
} UnkStruct_ov97_02233268;

u8 ov74_02231094(void);

UnkStruct_ov97_02232AC8 *ov74_02231184(void);

UnkStruct_ov97_02233268 *ov74_022311DC(void);

void ov74_02230EB4(void);

void ov74_02230F14(void *param0, u32 param1, u32 param2);

void ov74_02230EB4(void) {
    UnkStruct_ov97_02232AC8 *v0 = ov74_02231184();
    UnkStruct_ov97_02233268 *v1 = ov74_022311DC();

    if (ov74_02231094() == 1) {
        v0->unk_18 = 1;
        v0->unk_19 = 0;
        v0->unk_00 = (void *)&v1->unk_08;
        v0->unk_04 = (void *)&v1->unk_08;
        v0->unk_10 = 0;
        v0->unk_14 = 0;
        v0->unk_1A = 120;
        v0->unk_1B = 0;
        v0->unk_1C = (u8)0xFD;
    }
}

void ov74_02230EE8(void);

void ov74_02230EE8(void) {
    UnkStruct_ov97_02232AC8 *v0 = ov74_02231184();
    UnkStruct_ov97_02233268 *v1 = ov74_022311DC();

    v0->unk_18 = 1;
    v0->unk_19 = 0;
    v0->unk_00 = (void *)&v1->unk_08;
    v0->unk_04 = (void *)&v1->unk_08;
    v0->unk_10 = sizeof(UnkStruct_ov97_02233268_sub2);
    v0->unk_14 = sizeof(UnkStruct_ov97_02233268_sub2);
    v0->unk_1A = 120;
    v0->unk_1B = 0;
    v0->unk_1C = (u8)0xFE;
}

void ov74_02230F14(void *param0, u32 param1, u32 param2) {
    UnkStruct_ov97_02232AC8 *v0 = ov74_02231184();

    if ((int)param2 > 0xF0) {
        return;
    }

    v0->unk_18 = 2;
    v0->unk_19 = 3;
    v0->unk_00 = param0;
    v0->unk_04 = param0;
    v0->unk_10 = param1;
    v0->unk_14 = 0;
    v0->unk_1A = 120;
    v0->unk_1B = 0;
    v0->unk_1C = (u8)param2;
}
