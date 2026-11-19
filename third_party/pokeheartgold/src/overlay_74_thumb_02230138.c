#include "global.h"

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

void ov74_02231070(s8 param0);

void ov74_0223110C(s16 param0);

UnkStruct_ov97_02232254 *ov74_0223115C(void);

u8 ov74_022311AC(void);

u32 ov74_022311BC(void);

u32 ov74_022311CC(void);

void ov74_02231448(WMErrCode param0);

void ov74_0223144C(WMStateCode param0);

void ov74_02231458(UnkStruct_ov97_02232254 *param0);

BOOL ov74_02231670(void);

void ov74_0222FE4C(void);

UnkStruct_ov97_022335A8 ov74_0222FE5C(u8 *param0);

UnkStruct_ov97_022335C8 ov74_0222FE68(u8 *param0);

BOOL ov74_0222FE78(void);

void ov74_0222FEA0(u32 param0);

void ov74_02230138(void *param0);

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
UnkStruct_ov97_02232620 *ov74_02231154(void);
BOOL ov74_0223026C(WMstartScanCallback *param0, WMBssDesc *param1);

void ov74_02230138(void *param0) {
    WMstartParentCallback *v0 = (WMstartParentCallback *)param0;

    ov74_0223144C((WMStateCode)v0->state);

    if (v0->errcode == WM_ERRCODE_SUCCESS) {
        UnkStruct_ov97_02232254 *v1 = ov74_0223115C();

        ov74_02231070(8);

        if (ov74_0222FE78()) {
            return;
        }

        if (v1->unk_60 == 1) {
            v1->unk_60 = 2;
        }

        switch (v0->state) {
        case WM_STATECODE_PARENT_START:
            if (ov74_02231670() == 0) {
                ov74_0222FE4C();
            } else {
                ov74_0223110C(0);
            }
            break;
        case WM_STATECODE_CONNECTED:
            ov74_02231458(ov74_0223115C());

            if (v1->unk_60 == 2) {
                UnkStruct_ov97_022339EC *v2 = (UnkStruct_ov97_022339EC *)v0->ssid;

                if ((v2->unk_00.unk_00.unk_00_0 == ov74_022311AC()) && (v2->unk_00.unk_00.unk_00_8 <= ov74_022311BC()) && (v2->unk_00.unk_04.unk_00_16 == ov74_022311CC())) {
                    int v3;
                    UnkStruct_ov97_02232254 *v4 = ov74_0223115C();
                    UnkStruct_ov97_022335A8 v5 = ov74_0222FE5C(v0->macAddress);
                    UnkStruct_ov97_022335C8 v6 = ov74_0222FE68(v0->macAddress);

                    for (v3 = 0; v3 < 8; v3++) {
                        UnkStruct_ov97_022335A8 *v7 = &v4->unk_00[v3].unk_00;

                        if (v7->unk_00.val2 == v5.unk_00.val2) {
                            v4->unk_00[v3].unk_06 = v0->aid;
                            v4->unk_00[v3].unk_0A = 1;
                            break;
                        }

                        if (v7->unk_00.val2 == 0) {
                            v4->unk_00[v3].unk_00.unk_00.val2 = v5.unk_00.val2;
                            v4->unk_00[v3].unk_04.unk_00.val2 = v6.unk_00.val2;
                            v4->unk_00[v3].unk_06 = v0->aid;
                            v4->unk_00[v3].unk_0A = 1;
                            break;
                        }
                    }
                }
            }
            break;
        case WM_STATECODE_BEACON_SENT:
            break;
        case WM_STATECODE_DISCONNECTED:
            ov74_0222FEA0((u32)v0->aid);
            break;
        }
    } else {
        ov74_02231448((WMErrCode)v0->errcode);
        ov74_0222FE4C();
    }
}

BOOL ov74_0223026C(WMstartScanCallback *param0, WMBssDesc *param1) {
    int v0;
    UnkStruct_ov97_02232620 *v1 = ov74_02231154();
    UnkStruct_ov97_022335A8 v2 = ov74_0222FE5C(param1->bssid);
    UnkStruct_ov97_022335C8 v3 = ov74_0222FE68(param1->bssid);

    for (v0 = 0; v0 < 8; v0++) {
        UnkStruct_ov97_022335A8 *v4 = &v1->unk_00[v0].unk_00;
        UnkStruct_ov97_022335C8 *v5 = &v1->unk_00[v0].unk_34;

        if ((v4->unk_00.val2 == v2.unk_00.val2) && (v5->unk_00.val2 == v3.unk_00.val2)) {
            return 1;
        }

        if (v4->unk_00.val2 == 0) {
            v1->unk_00[v0].unk_00.unk_00.val2 = v2.unk_00.val2;
            v1->unk_00[v0].unk_04 = ((UnkStruct_ov97_02233268 *)param1->gameInfo.userGameInfo)->unk_08.unk_00;
            v1->unk_00[v0].unk_1C = ((UnkStruct_ov97_02233268 *)param1->gameInfo.userGameInfo)->unk_08.unk_18;
            v1->unk_00[v0].unk_34.unk_00.val2 = v3.unk_00.val2;
            v1->unk_00[v0].unk_36 = (u8)param0->linkLevel;
            return 1;
        }
    }

    return 0;
}
