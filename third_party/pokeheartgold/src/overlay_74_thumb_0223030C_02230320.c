#include "global.h"

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

WMBssDesc *ov74_02231100(void);

UnkStruct_ov97_02232620 *ov74_02231154(void);

u32 ov74_022311A0(void);

u8 ov74_022311AC(void);

u32 ov74_022311BC(void);

u32 ov74_022311CC(void);

void ov74_02231454(UnkStruct_ov97_02232620 *param0);

BOOL ov74_0223161C(void);

void ov74_0222FE4C(void);

BOOL ov74_0222FE78(void);

BOOL ov74_0223026C(WMstartScanCallback *param0, WMBssDesc *param1);

BOOL ov74_02230320(WMstartScanCallback *param0);

BOOL ov74_02230320(WMstartScanCallback *param0) {
    WMBssDesc *v0 = ov74_02231100();
    UnkStruct_ov97_02232620 *v1 = ov74_02231154();

    if (ov74_0222FE78()) {
        return 1;
    }

    {
        int v2 = 0;
        int v3;

        for (v3 = 0; v3 < 8; v3++) {
            UnkStruct_ov97_022335A8 *v4 = &v1->unk_00[v3].unk_00;

            if (v4->unk_00.val2) {
                v2 = 1;
                break;
            }
        }

        if ((v2 != 0) && (v1->unk_1C0 == 0)) {
            if (ov74_0223161C() == 0) {
                ov74_0222FE4C();
            }

            return 1;
        }
    }

    DC_InvalidateRange(v0, sizeof(WMbssDesc));
    ov74_02231454(ov74_02231154());

    if ((param0->gameInfoLength >= 8) && (v0->gameInfo.ggid == ov74_022311A0())) {
        UnkStruct_ov97_02233268 *v5 = (UnkStruct_ov97_02233268 *)v0->gameInfo.userGameInfo;

        if ((v5->unk_00.unk_00.unk_00_0 == ov74_022311AC()) && (v5->unk_00.unk_00.unk_00_8 <= ov74_022311BC()) && (v5->unk_00.unk_04.unk_00_16 == ov74_022311CC())) {
            if (ov74_0223026C(param0, v0)) {
                UnkStruct_ov97_02232620 *v6 = ov74_02231154();

                if (++v6->unk_1C2 > 28) {
                    if (ov74_0223161C() == 0) {
                        ov74_0222FE4C();
                        return 1;
                    }
                }
            } else {
                if (ov74_0223161C() == 0) {
                    ov74_0222FE4C();
                }

                return 1;
            }
        }
    }

    return 0;
}
