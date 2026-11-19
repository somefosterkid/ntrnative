#include "global.h"

#include "constants/heap.h"

#include "overlay_42.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov70_0225C894_t;

struct UnkStruct_ov70_0225DEE8_t;

struct UnkStruct_ov70_02261E10_t;

struct UnkStruct_ov70_0226CF38_t;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

typedef struct UnkStruct_ov70_0226CF38_t UnkStruct_ov70_0226CF38;

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;

void ov49_0225E3F4(UnkStruct_ov70_02261E10 *param0, u32 param1, const VecFx32 *param2);

void ov49_0225E4F8(UnkStruct_ov70_02261E10 *param0, u32 param1, s32 param2);

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
    UnkStruct_ov44_02232914 unk_AA[3];
    u16 unk_B6;
    u16 unk_B8;
    fx32 unk_BC;
    fx32 unk_C0;
};

BOOL ov49_022694B4(UnkStruct_ov70_0226D2A0 *param0, UnkStruct_ov70_02261E10 *param1, u32 param2);

BOOL ov49_022694B4(UnkStruct_ov70_0226D2A0 *param0, UnkStruct_ov70_02261E10 *param1, u32 param2) {
    VecFx32 v0 = { 0 };
    u16 v1;
    u16 v2;
    BOOL v3 = 0;

    if ((param0->unk_04.val1.unk_00 + 1) < param0->unk_04.val1.unk_02) {
        param0->unk_04.val1.unk_00++;
    } else {
        param0->unk_04.val1.unk_00 = 0;

        if ((param0->unk_04.val1.unk_08 - 1) > 0) {
            param0->unk_04.val1.unk_08--;
        } else {
            v3 = 1;
        }
    }

    v2 = (param0->unk_04.val1.unk_00 * 0xffff) / param0->unk_04.val1.unk_02;
    v1 = FX_MulInline(FX_SinIdx(v2), ((fx32)(((param0->unk_04.val1.unk_04) > 0) ? ((param0->unk_04.val1.unk_04) * (1 << 12) + 0.5f) : ((param0->unk_04.val1.unk_04) * (1 << 12) - 0.5f)))) >> 12;

    v0.y = FX_MulInline(FX_SinIdx(v2), ((fx32)(((param0->unk_04.val1.unk_06) > 0) ? ((param0->unk_04.val1.unk_06) * (1 << 12) + 0.5f) : ((param0->unk_04.val1.unk_06) * (1 << 12) - 0.5f))));

    ov49_0225E4F8(param1, param2, v1);
    ov49_0225E3F4(param1, param2, &v0);

    return v3;
}

void ov45_0222AE24(WiFiPlazaSystem *param0, u32 param1, int param2);

int ov45_0222AE34(const WiFiPlazaSystem *param0, u32 param1);

void ov49_0225E420(const UnkStruct_ov70_02261E10 *param0, u32 param1, u32 param2, VecFx32 *param3);

void ov49_022695C4(UnkStruct_ov70_0226CF38 *param0, u32 param1);

void ov49_022695C4(UnkStruct_ov70_0226CF38 *param0, u32 param1) {
    int v0, v1;
    u32 v2;
    u32 v3;
    VecFx32 v4;
    fx32 v5;

    for (v0 = 0; v0 < 3; v0++) {
        v2 = (param1 * 3) + v0;
        v3 = ov45_0222AE34(param0->unk_04, v2);

        ov49_0225E420(param0->unk_08, param1, v0, &v4);

        for (v1 = v3; v1 < 3; v1++) {
            v5 = ((param0->unk_AA[v1].unk0 + 16) * ((fx32)0x0000000000001000L));

            if (v5 > v4.x) {
                ov45_0222AE24(param0->unk_04, v2, v1);
            }
        }
    }
}
