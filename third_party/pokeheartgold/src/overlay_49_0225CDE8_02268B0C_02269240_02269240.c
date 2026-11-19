#include "global.h"
#include "constants/heap.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct UnkStruct_ov70_0225C894_t;

struct UnkStruct_ov70_0225DEE8_t;

struct UnkStruct_ov70_02261E10_t;

struct UnkStruct_ov70_0226CF38_t;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

typedef struct UnkStruct_ov70_0226CF38_t UnkStruct_ov70_0226CF38;

typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222CC3C;

u32 ov45_0222AD2C(const WiFiPlazaSystem *param0);

u32 ov45_0222AD3C(const WiFiPlazaSystem *param0);

extern BOOL ov45_0222ADE8(const WiFiPlazaSystem *param0, u32 param1);

extern void ov45_0222ADF8(WiFiPlazaSystem *param0, u32 param1);

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;

BOOL ov49_0225E47C(UnkStruct_ov70_02261E10 *param0, u32 param1);

extern BOOL ov49_0225E4A4(UnkStruct_ov70_02261E10 *param0, u32 param1);

typedef struct {
    u16 unk_00;
    u16 unk_02;
    union {
        struct {
            s16 unk_00;
            s16 unk_02;
            u16 unk_04;
            s16 unk_06;
            s16 unk_08;
        } val1;
    } unk_04;
} UnkStruct_ov70_0226D2A0;

struct UnkStruct_ov70_0226CF38_t {
    UnkStruct_ov70_0225DEE8 *unk_00;
    WiFiPlazaSystem *unk_04;
    UnkStruct_ov70_02261E10 *unk_08;
    UnkStruct_ov70_0225C894 *unk_0C;
    u8 unk_10[9][3];
    u8 unk_2B;
    UnkStruct_ov70_0226D2A0 unk_2C[9];
    UnkStruct_ov63_0222CC3C unk_AA[3];
    u16 unk_B6;
    u16 unk_B8;
    fx32 unk_BC;
    fx32 unk_C0;
};

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
    u16 unk_0A;
} UnkStruct_ov70_0226E774;

void ov49_022693A4(BOOL param0, BOOL param1, BOOL param2, u32 seqID);

void ov49_022693D4(BOOL param0, BOOL param1, BOOL param2, u32 species);

BOOL ov49_022693F8(UnkStruct_ov70_0226D2A0 *param0, u32 param1, UnkStruct_ov70_02261E10 *param2, u32 param3);

extern const UnkStruct_ov70_0226E774 ov49_0226A8DC[5];

UnkStruct_ov70_0226CF38 *ov49_02268FAC(UnkStruct_ov70_0225DEE8 *param0, enum HeapID heapID);

void ov49_02269240(UnkStruct_ov70_0226CF38 *param0, u32 param1, BOOL param2, BOOL param3, u32 param4);

void ov49_02269240(UnkStruct_ov70_0226CF38 *param0, u32 param1, BOOL param2, BOOL param3, u32 param4) {
    int v0;
    BOOL v1;
    BOOL v2 = 0;
    u32 v3;
    u32 v4;
    BOOL v5;

    v4 = ov45_0222AD3C(param0->unk_04);
    v3 = ov45_0222AD2C(param0->unk_04);

    for (v0 = 0; v0 < 3; v0++) {
        v1 = ov45_0222ADE8(param0->unk_04, (param1 * 3) + v0);
        v5 = 0;

        if (param3 == 1) {
            if (v0 == param4) {
                v5 = 1;
            }
        }

        if (v1 == 1) {
            switch (v0) {
            case 0:
                v2 = ov49_0225E47C(param0->unk_08, param1);

                if (v2 == 1) {
                    if ((param1 % 3) == 2) {
                        ov49_022693D4(param2, param3, v5, ov49_0226A8DC[v4].unk_02);

                        if (ov49_0226A8DC[v4].unk_08 != 0xfffe) {
                            ov49_022693A4(param2, param3, v5, ov49_0226A8DC[v4].unk_08);
                        }
                    } else {
                        ov49_022693D4(param2, param3, v5, ov49_0226A8DC[v4].unk_00);

                        if (ov49_0226A8DC[v4].unk_04 != 0xfffe) {
                            ov49_022693A4(param2, param3, v5, ov49_0226A8DC[v4].unk_04);
                        }
                    }

                    ov45_0222ADF8(param0->unk_04, (param1 * 3) + v0);
                }
                break;
            case 1: {
                u32 v6;

                if ((param1 % 3) == 2) {
                    v6 = 1;
                } else {
                    v6 = 0;
                }

                v2 = ov49_022693F8(&param0->unk_2C[param1], v6, param0->unk_08, param1);

                if (v2 == 1) {
                    ov45_0222ADF8(param0->unk_04, (param1 * 3) + v0);
                    ov49_022693A4(param2, param3, v5, 1478);
                }
            } break;
            case 2:
                v2 = ov49_0225E4A4(param0->unk_08, param1);

                if (v2 == 1) {
                    ov45_0222ADF8(param0->unk_04, (param1 * 3) + v0);

                    if ((param1 % 3) == 2) {
                        ov49_022693A4(param2, param3, v5, ov49_0226A8DC[v4].unk_0A);
                    } else {
                        ov49_022693A4(param2, param3, v5, ov49_0226A8DC[v4].unk_06);
                    }
                }
                break;
            }
        }
    }
}
