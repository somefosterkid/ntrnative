#include "global.h"

#include "error_handling.h"
#include "unk_02005D10.h"
#include "overlay_49_unk_struct_ov70_02263270.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov70_0225CA20_t;

struct UnkStruct_ov70_0225CC54_t;

struct UnkStruct_ov70_0225DEE8_t;

struct UnkStruct_ov70_02261E10_t;

struct UnkStruct_ov70_022630A4_t;

struct UnkStruct_ov70_02263344_t;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222CC3C;

int ov45_0222AD80(const WiFiPlazaSystem *param0, u32 param1);

u32 ov45_0222ADA8(const WiFiPlazaSystem *param0, u32 param1);

void ov45_0222AE08(u32 param0, u32 *param1, u32 *param2);

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

void ov49_02258D54(UnkStruct_ov70_0225CC54 *param0);

UnkStruct_ov70_0225CC54 *ov49_02258D70(UnkStruct_ov70_0225CA20 *param0, u32 param1);

void ov49_02258DB4(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov63_0222CC3C param1);

void ov49_02258EEC(UnkStruct_ov70_0225CA20 *param0, UnkStruct_ov70_0225CC54 *param1, int param2);

BOOL ov49_02258F38(const UnkStruct_ov70_0225CC54 *param0);

void ov49_02259148(UnkStruct_ov70_0225CC54 *param0, const VecFx32 *param1);

void ov49_02259154(const UnkStruct_ov70_0225CC54 *param0, VecFx32 *param1);

void ov49_02259160(UnkStruct_ov70_0225CC54 *param0, int param1);

void ov49_02259184(UnkStruct_ov70_0225CC54 *param0, BOOL param1);

void ov49_022591B4(UnkStruct_ov70_0225CC54 *param0, u32 param1);

typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;

typedef struct UnkStruct_ov70_022630A4_t UnkStruct_ov70_022630A4;

WiFiPlazaSystem *ov49_02259FE8(UnkStruct_ov70_0225DEE8 *param0, UnkStruct_ov70_0225DEE8 *);

UnkStruct_ov70_0225CA20 *ov49_02259FF0(UnkStruct_ov70_0225DEE8 *param0);

UnkStruct_ov70_02261E10 *ov49_02259FF8(UnkStruct_ov70_0225DEE8 *param0);

UnkStruct_ov70_022630A4 *ov49_0225A010(UnkStruct_ov70_0225DEE8 *param0);

BOOL ov49_0225A520(const UnkStruct_ov70_0225DEE8 *param0, u32 param1);

void ov49_0225E420(const UnkStruct_ov70_02261E10 *param0, u32 param1, u32 param2, VecFx32 *param3);

typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;

typedef BOOL (*UnkFuncPtr_ov70_0226DA18)(UnkStruct_ov70_02263344 *, UnkStruct_ov70_0225DEE8 *, u32);

typedef struct {
    UnkFuncPtr_ov70_0226DA18 unk_00;
    UnkFuncPtr_ov70_0226DA18 unk_04;
} UnkStruct_ov70_0226DA18;

void *ov49_0225EF40(UnkStruct_ov70_02263344 *param0, u32 param1);

void ov49_0225EF68(UnkStruct_ov70_02263344 *param0);

void *ov49_0225EF84(UnkStruct_ov70_02263344 *param0);

u32 ov49_0225EF88(const UnkStruct_ov70_02263344 *param0);

void ov49_0225EF8C(UnkStruct_ov70_02263344 *param0, u32 param1);

void ov49_0225EF98(UnkStruct_ov70_022630A4 *param0, u32 param1, const UnkStruct_ov70_0226DA18 *param2, void *param3);

typedef struct {
    UnkStruct_ov70_0225CC54 *unk_00;
    u32 unk_04;
    u32 unk_08;
    fx32 unk_0C;
    fx32 unk_10;
    s32 unk_14;
    UnkStruct_ov70_02263270 unk_18;
} UnkStruct_ov70_02265840;

void ov49_0225F374(UnkStruct_ov70_02263270 *param0);

fx32 ov49_0225F394(UnkStruct_ov70_02263270 *param0);

void ov49_0225F430(UnkStruct_ov70_02263270 *param0);

BOOL ov49_0225F438(const UnkStruct_ov70_02263270 *param0);

BOOL ov49_0226154C(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

extern const UnkStruct_ov70_0226DA18 ov49_02269B78;

BOOL ov49_02261930(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov49_02261930(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2) {
    UnkStruct_ov70_02265840 *v0;
    UnkStruct_ov70_0225CA20 *v1;
    UnkStruct_ov70_02261E10 *v2;
    WiFiPlazaSystem *v3 = ov49_02259FE8(param1, param1);
    v1 = ov49_02259FF0(param1);
    v2 = ov49_02259FF8(param1);
    v0 = ov49_0225EF84(param0);

    switch (ov49_0225EF88(param0)) {
    case 0: {
        v0 = ov49_0225EF40(param0, sizeof(UnkStruct_ov70_02265840));
        v0->unk_00 = ov49_02258D70(v1, param2);
        v0->unk_04 = ov45_0222ADA8(v3, param2);

        ((v0->unk_04 != 0xffffffff) ? (void)0 : GF_AssertFail());

        ov45_0222AE08(v0->unk_04, &v0->unk_04, &v0->unk_08);
        ov49_02258EEC(v1, v0->unk_00, 3);
        ov49_0225EF8C(param0, 1);
    } break;

    case 1:
        if (ov49_02258F38(v0->unk_00)) {
            {
                VecFx32 v4;

                ov49_0225E420(v2, v0->unk_04, v0->unk_08, &v4);
                v0->unk_10 = v4.x + ((fx32)(((-16) > 0) ? ((-16) * (1 << 12) + 0.5f) : ((-16) * (1 << 12) - 0.5f)));
                ov49_02259154(v0->unk_00, &v4);
                v0->unk_0C = v4.y;
                v0->unk_14 = 0;
            }

            {
                UnkStruct_ov63_0222CC3C v5 = { 0 };
                ov49_02258DB4(v0->unk_00, v5);
            }

            PlaySE(1614);

            ov49_02259184(v0->unk_00, 1);
            ov49_0225EF8C(param0, 2);
        }
        break;
    case 2: {
        VecFx32 v6;
        VecFx32 v7;
        BOOL v8 = 0;

        v0->unk_14++;

        if (v0->unk_14 >= 24) {
            v0->unk_14 = 24;
            v8 = 1;
        }

        ov49_0225E420(v2, v0->unk_04, v0->unk_08, &v6);

        v7.z = v6.z;
        v7.x = v0->unk_10;
        v7.y = FX_Div(FX_MulInline(((fx32)(((v0->unk_14) > 0) ? ((v0->unk_14) * (1 << 12) + 0.5f) : ((v0->unk_14) * (1 << 12) - 0.5f))), (v6.y - v0->unk_0C)), ((fx32)(((24) > 0) ? ((24) * (1 << 12) + 0.5f) : ((24) * (1 << 12) - 0.5f))));
        v7.y += v0->unk_0C;

        ov49_02259148(v0->unk_00, &v7);

        if (v8 == 1) {
            ov49_02259184(v0->unk_00, 0);
            ov49_022591B4(v0->unk_00, 1 << 3);
            ov49_02259160(v0->unk_00, 2);
            ov49_0225EF8C(param0, 3);
        }
    } break;

    case 3: {
        VecFx32 v9;
        fx32 v10;

        if (ov49_0225A520(param1, v0->unk_04) == 1) {
            if (ov49_0225F438(&v0->unk_18) == 0) {
                ov49_0225F374(&v0->unk_18);
            }
        } else {
            if (ov49_0225F438(&v0->unk_18) == 1) {
                ov49_0225F430(&v0->unk_18);
            }
        }

        v10 = ov49_0225F394(&v0->unk_18);
        ov49_0225E420(v2, v0->unk_04, v0->unk_08, &v9);
        v9.y += v10;
        ov49_02259148(v0->unk_00, &v9);

        if (ov45_0222AD80(v3, v0->unk_04) == 2) {
            ov49_0225EF8C(param0, 4);
        }
    } break;
    case 4:
        ov49_02258D54(v0->unk_00);
        ov49_0225EF68(param0);

        {
            UnkStruct_ov70_022630A4 *v11;

            v11 = ov49_0225A010(param1);
            ov49_0225EF98(v11, param2, &ov49_02269B78, 0L);
        }
        break;
    }

    return 0;
}
