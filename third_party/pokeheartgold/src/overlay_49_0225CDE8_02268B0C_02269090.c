#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "sys_task.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

void Heap_Free(void *ptr);

void ov49_02269090(void *ptr);

void ov49_02269090(void *ptr) {
    Heap_Free(ptr);
}

struct UnkStruct_ov66_0222DFF8_t;

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

u32 ov45_0222A53C(const WiFiPlazaSystem *param0);

int ov45_0222AD80(const WiFiPlazaSystem *param0, u32 param1);

u32 ov45_0222ADA8(const WiFiPlazaSystem *param0, u32 param1);

void ov45_0222AE08(u32 param0, u32 *param1, u32 *param2);

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;

void ov49_0225E4CC(UnkStruct_ov70_02261E10 *param0, u32 param1, BOOL param2);

BOOL ov49_0225E524(const UnkStruct_ov70_02261E10 *param0, u32 param1);

BOOL ov49_0225E54C(const UnkStruct_ov70_02261E10 *param0, u32 param1);

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

void ov49_02269178(UnkStruct_ov70_0226CF38 *param0, u32 param1);

void ov49_022695C4(UnkStruct_ov70_0226CF38 *param0, u32 param1);

void ov49_02269240(UnkStruct_ov70_0226CF38 *param0, u32 param1, BOOL param2, BOOL param3, u32 param4);

BOOL ov49_02269430(UnkStruct_ov70_0226D2A0 *param0, UnkStruct_ov70_02261E10 *param1, u32 param2);

void ov49_02269098(UnkStruct_ov70_0226CF38 *param0);

void ov49_02269098(UnkStruct_ov70_0226CF38 *param0) {
    int v0;
    BOOL v1;
    u32 v2;
    u32 v3;
    u32 v4;
    BOOL v5;
    BOOL v6;

    v4 = ov45_0222A53C(param0->unk_04);
    v2 = ov45_0222ADA8(param0->unk_04, v4);

    if (v2 == 0xffffffff) {
        v5 = 0;
    } else {
        ov45_0222AE08(v2, &v2, &v3);
        v5 = 1;
    }

    for (v0 = 0; v0 < 9; v0++) {
        if (ov45_0222AD80(param0->unk_04, v0) == 1) {
            ov49_0225E4CC(param0->unk_08, v0, 1);
            ov49_02269178(param0, v0);
            ov49_022695C4(param0, v0);

            if (v5) {
                if (v2 == v0) {
                    v6 = 1;
                } else {
                    v6 = 0;
                }
            }

            ov49_02269240(param0, v0, v5, v6, v3);
        } else {
            ov49_0225E4CC(param0->unk_08, v0, 0);
        }
    }

    for (v0 = 0; v0 < 9; v0++) {
        v1 = ov49_0225E524(param0->unk_08, v0);
        param0->unk_10[v0][0] = v1;

        v1 = ov49_02269430(&param0->unk_2C[v0], param0->unk_08, v0);
        param0->unk_10[v0][1] = v1;

        v1 = ov49_0225E54C(param0->unk_08, v0);
        param0->unk_10[v0][2] = v1;
    }
}

BOOL ov49_02269154(const UnkStruct_ov70_0226CF38 *param0, u32 param1, int param2);

BOOL ov49_02269154(const UnkStruct_ov70_0226CF38 *param0, u32 param1, int param2) {
    GF_ASSERT(param1 < 9);
    GF_ASSERT(param2 < 3);
    return param0->unk_10[param1][param2];
}

u32 ov45_0222AD90(const WiFiPlazaSystem *param0, u32 param1);

u32 ov45_0222ADA0(const WiFiPlazaSystem *param0);

void ov49_0225E3B8(UnkStruct_ov70_02261E10 *param0, u32 param1, fx32 param2);

void ov49_02269178(UnkStruct_ov70_0226CF38 *param0, u32 param1) {
    s32 v0;
    s32 v1;
    fx32 v2;
    fx32 v3;

    v0 = ov45_0222AD90(param0->unk_04, param1);
    v1 = ov45_0222ADA0(param0->unk_04);
    v3 = FX_Div(param0->unk_C0, ((fx32)(((2) > 0) ? ((2) * (1 << 12) + 0.5f) : ((2) * (1 << 12) - 0.5f))));
    v2 = FX_Div(FX_MulInline(((fx32)(((v0) > 0) ? ((v0) * (1 << 12) + 0.5f) : ((v0) * (1 << 12) - 0.5f))), v3), ((fx32)(((v1) > 0) ? ((v1) * (1 << 12) + 0.5f) : ((v1) * (1 << 12) - 0.5f))));
    v2 = FX_MulInline(v2, ((fx32)(((2) > 0) ? ((2) * (1 << 12) + 0.5f) : ((2) * (1 << 12) - 0.5f))));
    v2 += param0->unk_BC;

    ov49_0225E3B8(param0->unk_08, param1, v2);
}
