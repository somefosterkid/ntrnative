#include <stdint.h>

#include "global.h"

typedef struct {
    u32 unk_00_0 : 8;
    u32 unk_00_8 : 4;
    u32 unk_00_12 : 4;
    u32 unk_00_16 : 16;
    const u16 *unk_04;
    const u16 *unk_08;
} UnkStruct_ov97_02232E38;

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

typedef struct {
    u32 unk_00_0 : 8;
    u32 unk_00_8 : 4;
    u32 unk_00_12 : 4;
    u32 unk_00_16 : 16;
} UnkStruct_ov97_022339EC_sub1_sub1;

typedef struct {
    u32 unk_00_0 : 8;
    u32 unk_00_8 : 4;
    u32 unk_00_12 : 4;
    u32 unk_00_16 : 16;
} UnkStruct_ov97_022339EC_sub1_sub2;

typedef struct {
    UnkStruct_ov97_022339EC_sub1_sub1 unk_00;
    UnkStruct_ov97_022339EC_sub1_sub2 unk_04;
} UnkStruct_ov97_022339EC_sub1;

typedef struct {
    UnkStruct_ov97_022339EC_sub1 unk_00;
    u8 unk_08[16];
} UnkStruct_ov97_022339EC;

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

typedef union {
    UnkStruct_ov97_02232254 val1;
    UnkStruct_ov97_02232620 val2;
} UnkUnion_ov97_022334FC;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u32 unk_04;
    int unk_08;
    u16 unk_0C;
    u16 unk_0E;
    BOOL unk_10;
    BOOL unk_14;
    BOOL unk_18;
    BOOL unk_1C;
    BOOL unk_20;
    WMscanParam *unk_24;
    WMbssDesc *unk_28;
    u32 unk_2C;
    u32 unk_30;
    void *unk_34;
    void *unk_38;
    void *unk_3C;
    UnkStruct_ov97_02233268 unk_40;
    UnkStruct_ov97_022339EC unk_78;
    UnkUnion_ov97_022334FC unk_90;
    UnkStruct_ov97_02232AC8 unk_254;
} UnkStruct_ov97_0223222C;

WMParentParam *ov74_02231054(void);

UnkStruct_ov97_0223222C *ov74_0223105C(void);

UnkStruct_ov97_02232AC8 *ov74_02231184(void);

s32 ov74_02230A74(void);

s32 ov74_02230A7C(void);

s32 ov74_02230A14(void);

void ov74_02231194(void);

void ov74_02230A84(UnkStruct_ov97_02232E38 *param0, u8 *param1);

void ov74_02230A84(UnkStruct_ov97_02232E38 *param0, u8 *param1) {
    WMParentParam *v0 = ov74_02231054();
    UnkStruct_ov97_0223222C *v1 = ov74_0223105C();
    void *v2 = param1;
    u32 v3 = (u32)((uintptr_t)param1 & 0x1F);

    if (v3) {
        v3 = 0x20 - (u32)((uintptr_t)param1 & 0x1F);
        param1 = param1 + v3;
    }

    v1->unk_00 = (u8)0;
    v1->unk_01 = (u8)0;
    v1->unk_02 = (u8)0;
    v1->unk_03 = 4;
    v1->unk_04 = 0x400131;
    v1->unk_0C = 0;

    v1->unk_34 = param1;
    param1 += WM_SYSTEM_BUF_SIZE;

    v1->unk_38 = param1;
    param1 += v1->unk_2C = ov74_02230A74();

    v1->unk_3C = param1;
    param1 += v1->unk_30 = ov74_02230A7C();

    v1->unk_28 = (WMBssDesc *)param1;
    param1 += sizeof(WMBssDesc);

    v1->unk_24 = (WMScanParam *)param1;
    param1 += sizeof(WMScanParam);

    (void)WM_Init(v1->unk_34, 2);

    v1->unk_10 = ov74_02230A14();
    v1->unk_14 = 0;
    v1->unk_18 = 0;
    v1->unk_1C = 0;
    v1->unk_20 = 0;
    v1->unk_40.unk_00.unk_00.unk_00_0 = 0xF;
    v1->unk_40.unk_00.unk_00.unk_00_8 = 0x1;
    v1->unk_40.unk_00.unk_00.unk_00_12 = 1;
    v1->unk_40.unk_00.unk_04.unk_00_0 = param0->unk_00_0;
    v1->unk_40.unk_00.unk_04.unk_00_8 = param0->unk_00_8;
    v1->unk_40.unk_00.unk_04.unk_00_12 = param0->unk_00_12;
    v1->unk_40.unk_00.unk_04.unk_00_16 = param0->unk_00_16;

    {
        const u16 *v4 = param0->unk_08;
        const u16 *v5 = param0->unk_04;
        u32 v6 = 0;

        while ((v4) && (v6 < 12)) {
            v1->unk_40.unk_08.unk_00.unk_00[v6] = *v4;
            v1->unk_40.unk_08.unk_18.unk_00[v6] = *v5;
            v6++;
            v4++;
            v5++;
        }
    }

    v1->unk_78.unk_00 = *(UnkStruct_ov97_022339EC_sub1 *)&v1->unk_40.unk_00;

    ov74_02231194();

    ov74_02231184()->unk_08 = param1;
    param1 += v1->unk_2C;

    ov74_02231184()->unk_0C = param1;
    param1 += v1->unk_30;

    (void)WM_GetNextTgid();
}
