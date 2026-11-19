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

UnkStruct_ov97_02232AC8 *ov74_02231184(void);

u8 ov74_022311AC(void);

u32 ov74_022311BC(void);

void ov74_022313F0(void *param0);

BOOL ov74_02231424(void *param0);

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

void ov74_0223145C(UnkStruct_ov97_02233674 *param0);

void ov74_02230714(WMPortRecvCallback *param0);

void ov74_02230714(WMPortRecvCallback *param0) {
    UnkStruct_ov97_02232AC8 *v0 = ov74_02231184();

    if (v0->unk_19 == 1) {
        UnkStruct_ov97_02233674 *v1 = (UnkStruct_ov97_02233674 *)v0->unk_0C;

        DC_FlushRange(param0->data, param0->length);
        MI_CpuCopy8(param0->data, v0->unk_0C, param0->length);

        if ((v1->unk_00.unk_00_0 == ov74_022311AC()) && (v1->unk_00.unk_00_8 <= ov74_022311BC())) {
            ov74_022313F0(v0->unk_0C);

            if (v1->unk_08.unk_00_0 != v0->unk_1C) {
                return;
            }

            ov74_0223145C((UnkStruct_ov97_02233674 *)v0->unk_0C);

            if ((ov74_02231424(v0->unk_0C)) && (v1->unk_08.unk_00_8 != 1)) {
                v0->unk_19 = 2;
            } else {
                v0->unk_19 = 0;
            }
        }
    }
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

u8 ov74_02231094(void);

UnkStruct_ov97_02232254 *ov74_0223115C(void);

static void ov74_0223078C(WMPortRecvCallback *param0);

static void ov74_0223078C(WMPortRecvCallback *param0) {
    UnkStruct_ov97_02232AC8 *v0 = ov74_02231184();

    if ((ov74_02231094() == 2) && (v0->unk_19 != 2)) {
        UnkStruct_ov97_02233674 *v1 = (UnkStruct_ov97_02233674 *)v0->unk_0C;

        DC_FlushRange(param0->data, param0->length);
        MI_CpuCopy8(param0->data, v0->unk_0C, param0->length);

        if ((v1->unk_00.unk_00_0 == ov74_022311AC()) && (v1->unk_00.unk_00_8 <= ov74_022311BC())) {
            ov74_022313F0(v0->unk_0C);

            if (v1->unk_08.unk_00_0 != 0xFD) {
                return;
            } else {
                UnkStruct_ov97_02232254 *v2 = ov74_0223115C();
                int v3;

                for (v3 = 0; v3 < 8; v3++) {
                    if (v2->unk_00[v3].unk_06 == param0->aid) {
                        v2->unk_00[v3].unk_08 = 1;
                        v2->unk_00[v3].unk_09 = 3;
                        v0->unk_1A = 180;
                        v0->unk_19 = 0;
                        break;
                    }
                }
            }
        }
    }
}

void ov74_02230818(WMPortRecvCallback *param0);

void ov74_02230818(WMPortRecvCallback *param0) {
    UnkStruct_ov97_02232AC8 *v0 = ov74_02231184();

    if (v0->unk_1C == 0xFD) {
        ov74_0223078C(param0);
    } else {
        if (((v0->unk_19 == 1) || (v0->unk_19 == 3)) && (v0->unk_1B == 0)) {
            UnkStruct_ov97_02233674 *v1 = (UnkStruct_ov97_02233674 *)v0->unk_0C;

            DC_FlushRange(param0->data, param0->length);
            MI_CpuCopy8(param0->data, v0->unk_0C, param0->length);

            if ((v1->unk_00.unk_00_0 == ov74_022311AC()) && (v1->unk_00.unk_00_8 <= ov74_022311BC())) {
                ov74_022313F0(v0->unk_0C);

                if (v1->unk_08.unk_00_0 != v0->unk_1C) {
                    return;
                }

                ov74_0223145C((UnkStruct_ov97_02233674 *)v0->unk_0C);

                v0->unk_19 = 0;

                if (ov74_02231424(v0->unk_0C)) {
                    if (v1->unk_08.unk_00_0 == 0xFD) {
                        ov74_0223078C(param0);
                    } else {
                        MI_CpuCopy8((u8 *)v0->unk_0C + v1->unk_0C.unk_00_0, v0->unk_04, v1->unk_0C.unk_00_8);
                    }

                    v1->unk_08.unk_00_8 = 0;
                    v0->unk_1B = 4;
                } else {
                    v1->unk_08.unk_00_8 = 1;
                }
            }
        }
    }
}
