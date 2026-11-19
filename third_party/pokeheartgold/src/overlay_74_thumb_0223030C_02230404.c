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

void ov74_02231070(s8 param0);

u8 ov74_02231094(void);

WMBssDesc *ov74_02231100(void);

UnkStruct_ov97_02232620 *ov74_02231154(void);

void ov74_02231448(WMErrCode param0);

void ov74_0223144C(WMStateCode param0);

void ov74_02231450(WMBssDesc *param0);

void ov74_0222FE4C(void);

BOOL ov74_0222FE78(void);

void ov74_0223030C(void);

BOOL ov74_02230320(WMstartScanCallback *param0);

void ov74_02230404(void *param0);

void ov74_02230404(void *param0) {
    WMstartScanCallback *v0 = (WMstartScanCallback *)param0;
    ov74_0223144C((WMStateCode)v0->state);

    if (v0->errcode == WM_ERRCODE_SUCCESS) {
        {
            WMBssDesc *v1 = ov74_02231100();
            ov74_02231450(v1);
        }

        ov74_02231070(6);

        if (ov74_0222FE78()) {
            return;
        }

        switch (ov74_02231094()) {
        case 1: {
            UnkStruct_ov97_02232620 *v2 = ov74_02231154();
            v2->unk_1C3_4 = 1;
        } break;
        }

        switch (v0->state) {
        case WM_STATECODE_PARENT_FOUND:
            if (ov74_02230320(v0) != 0) {
                break;
            }
        case WM_STATECODE_PARENT_NOT_FOUND:
            ov74_0223030C();
            break;
        default:
            ov74_02231448((WMErrCode)v0->errcode);
            ov74_0222FE4C();
            break;
        }
    } else {
        ov74_02231448((WMErrCode)v0->errcode);
        ov74_0222FE4C();
    }
}

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
    UnkStruct_ov97_02233268_sub2_sub1 unk_00;
    UnkStruct_ov97_02233268_sub2_sub1 unk_18;
} UnkStruct_ov97_02233268_sub2;

typedef struct {
    UnkStruct_ov97_02233268_sub1 unk_00;
    UnkStruct_ov97_02233268_sub2 unk_08;
} UnkStruct_ov97_02233268;

WMParentParam *ov74_02231054(void);

u32 ov74_022311A0(void);

u8 ov74_022311AC(void);

u32 ov74_022311BC(void);

u32 ov74_022311CC(void);

void ov74_02231454(UnkStruct_ov97_02232620 *param0);

BOOL ov74_0223161C(void);

UnkStruct_ov97_022335A8 ov74_0222FE5C(u8 *param0);

static BOOL ov74_02230478(void);

static BOOL ov74_02230478(void) {
    WMBssDesc *v0 = ov74_02231100();
    UnkStruct_ov97_02232620 *v1 = ov74_02231154();
    WMParentParam *v2 = ov74_02231054();

    if (ov74_0222FE78()) {
        return 1;
    }

    DC_InvalidateRange(v0, sizeof(WMbssDesc));
    ov74_02231454(ov74_02231154());

    if ((v0->gameInfoLength >= 8) && (v0->gameInfo.ggid == ov74_022311A0())) {
        UnkStruct_ov97_02233268 *v3 = (UnkStruct_ov97_02233268 *)v0->gameInfo.userGameInfo;

        if ((v3->unk_00.unk_00.unk_00_0 == ov74_022311AC()) && (v3->unk_00.unk_00.unk_00_8 <= ov74_022311BC()) && (v3->unk_00.unk_04.unk_00_16 == ov74_022311CC())) {
            UnkStruct_ov97_022335A8 v4 = ov74_0222FE5C(v0->bssid);
            UnkStruct_ov97_022335A8 *v5 = &v1->unk_00[v1->unk_1C3_0].unk_00;

            if (v5->unk_00.val2 == v4.unk_00.val2) {
                if (ov74_0223161C() == 0) {
                    ov74_0222FE4C();
                }

                return 1;
            }
        }
    }

    return 0;
}

BOOL ov74_02231584(void);

void ov74_0222FFAC(void);

void ov74_02230520(void *param0);

void ov74_02230520(void *param0) {
    WMstartScanCallback *v0 = (WMstartScanCallback *)param0;

    ov74_0223144C((WMStateCode)v0->state);

    if (v0->errcode == WM_ERRCODE_SUCCESS) {
        UnkStruct_ov97_02232620 *v1 = ov74_02231154();

        v1->unk_1C3_4 = 2;
        ov74_02231070(6);

        if (ov74_0222FE78()) {
            return;
        }

        switch (v0->state) {
        case WM_STATECODE_PARENT_FOUND:
            if (ov74_02230478() != 0) {
                break;
            }
        case WM_STATECODE_PARENT_NOT_FOUND:
            ov74_0222FFAC();

            if (ov74_02231584() == 0) {
                ov74_0222FE4C();
            }
            break;
        default:
            ov74_02231448((WMErrCode)v0->errcode);
            ov74_0222FE4C();
            break;
        }
    } else {
        ov74_02231448((WMErrCode)v0->errcode);
        ov74_0222FE4C();
    }
}
