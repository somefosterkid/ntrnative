#include "global.h"

#include "overlay_49_getters.h"

#include "error_handling.h"
#include "overlay_42.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov70_0225C894_t;

struct UnkStruct_ov70_0225CA20_t;

struct UnkStruct_ov70_0225CC54_t;

struct UnkStruct_ov70_0225DEE8_t;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222CC3C;

BOOL ov45_0222AFF8(const WiFiPlazaSystem *param0);

BOOL ov45_0222B00C(const WiFiPlazaSystem *param0);

u32 ov45_0222B020(const WiFiPlazaSystem *param0);

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

UnkStruct_ov70_0225CC54 *ov70_0225CCAC(UnkStruct_ov70_0225CA20 *param0);

void ov49_02258E04(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov63_0222CC3C param1, int param2);

UnkStruct_ov63_0222CC3C ov49_02258E34(const UnkStruct_ov70_0225CC54 *param0);

u32 ov49_02258E60(const UnkStruct_ov70_0225CC54 *param0, int param1);

BOOL ov49_0225904C(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov70_0225CC54 *param1, u32 *param2, UnkStruct_ov63_0222CC3C *param3);


UnkStruct_ov70_0225C894 *ov49_0225A000(UnkStruct_ov70_0225DEE8 *param0);

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u8 unk_04;
    u8 unk_05[3];
} UnkStruct_ov70_02265CCC;

BOOL ov49_02261FC0(const UnkStruct_ov70_0225C894 *param0, const UnkStruct_ov70_0225CA20 *param1, const u32 *param2, u32 param3, UnkStruct_ov63_0222CC3C *param4);

extern const u32 ov49_02269B90[4];

extern const u32 ov49_02269BC0[4];

extern const u32 ov49_02269BA0[4];

extern const u32 ov49_02269BD0[4];

extern const u32 ov49_02269BB0[4];

extern const u32 ov49_02269C20[8];

extern const u32 ov49_02269C40[8];

UnkStruct_ov70_0225CC54 *ov49_02258DAC(UnkStruct_ov70_0225CA20 *param0);

BOOL ov49_02261DBC(UnkStruct_ov70_02265CCC *param0, WiFiPlazaSystem *param1, UnkStruct_ov70_0225DEE8 *param2, UnkStruct_ov70_0225CC54 *param3, u8 param4, int param5);

BOOL ov49_02261DBC(UnkStruct_ov70_02265CCC *param0, WiFiPlazaSystem *param1, UnkStruct_ov70_0225DEE8 *param2, UnkStruct_ov70_0225CC54 *param3, u8 param4, int param5) {
    u32 v0;
    u32 v1;
    UnkStruct_ov63_0222CC3C v2;
    UnkStruct_ov70_0225CA20 *v3;
    UnkStruct_ov70_0225CC54 *v4;
    UnkStruct_ov70_0225C894 *v5;
    BOOL v6 = 0;
    BOOL v7;
    v3 = ov49_02259FF0(param2);
    v4 = ov49_02258DAC(v3);
    v5 = ov49_0225A000(param2);
    v0 = ov49_02258E60(param3, 4);
    v1 = 1;
    switch (param4) {
    case 2:
        v7 = ov49_02261FC0(v5, v3, ov49_02269B90, 4, &v2);
        if (v7 == 0) {
            return 0;
        }
        break;

    case 3:
        v7 = ov49_02261FC0(v5, v3, ov49_02269BC0, 4, &v2);
        if (v7 == 0) {
            return 0;
        }
        break;

    case 4:
        v7 = ov49_02261FC0(v5, v3, ov49_02269BA0, 4, &v2);
        if (v7 == 0) {
            return 0;
        }
        break;

    case 5:
        v1 = 0;
        v7 = ov49_02261FC0(v5, v3, ov49_02269C20, 8, &v2);
        if (v7 == 0) {
            return 0;
        }
        break;

    case 6:
        v1 = 0;
        v7 = ov49_02261FC0(v5, v3, ov49_02269C40, 8, &v2);
        if (v7 == 0) {
            return 0;
        }
        break;

    case 7:
        v1 = 3;
        v7 = ov49_02261FC0(v5, v3, ov49_02269BB0, 4, &v2);
        if (v7 == 0) {
            return 0;
        }
        break;

    case 8:
        v1 = 0;
        v7 = ov49_02261FC0(v5, v3, ov49_02269BD0, 4, &v2);
        if (v7 == 0) {
            return 0;
        }
        break;

    case 10:
        return 0;

    case 9:
        if (ov45_0222AFF8(param1) == 0) {
            return 0;
        }
        if (ov45_0222B00C(param1) == 0) {
            return 0;
        }
        if (ov45_0222B020(param1) != v0) {
            return 0;
        }
        v7 = ov49_0225904C(v3, v4, &v1, &v2);
        (v7 == 1) ? ((void)0) : (GF_AssertFail());
        v1 = ov42_022282A4(v1);
        v6 = 1;
        break;

    case 0:
    case 1:
    case 11:
    case 12:
    case 13:
    case 14:
    default:
        return 0;
    }

    {
        UnkStruct_ov63_0222CC3C v8;
        v8 = ov49_02258E34(param3);
        ov49_02258E04(param3, v2, v1);
        param0->unk_00 = v8.unk_00 / 16;
        param0->unk_01 = v8.unk_02 / 16;
        param0->unk_02 = param4;
        param0->unk_03 = v6;
        param0->unk_04 = param5;
    }
    return 1;
}
