#include "global.h"

#include "overlay_49_getters.h"

#include "math_util.h"
#include "overlay_49_unk_struct_ov70_02263270.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

fx32 ov49_0225F394(UnkStruct_ov70_02263270 *param0);

void ov49_0225F430(void *arg0);
s32 ov49_0225F438(void *arg0);

struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov70_0225CA20_t;
struct UnkStruct_ov70_0225CC54_t;
struct UnkStruct_ov70_0225DEE8_t;
struct UnkStruct_ov70_02263344_t;
typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;
u32 ov45_0222A53C(const WiFiPlazaSystem *param0);
typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;
typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;
UnkStruct_ov70_0225CC54 *ov49_02258D70(UnkStruct_ov70_0225CA20 *param0, u32 param1);
void ov49_02258EEC(UnkStruct_ov70_0225CA20 *param0, UnkStruct_ov70_0225CC54 *param1, int param2);
void ov49_0225A04C(UnkStruct_ov70_0225DEE8 *param0, u8 param1, BOOL param2);
u32 ov49_0225A4E0(const UnkStruct_ov70_0225DEE8 *param0);
BOOL ov49_0225A4F0(const UnkStruct_ov70_0225DEE8 *param0);
void ov49_0225A53C(UnkStruct_ov70_0225DEE8 *param0, BOOL param1);
void ov49_0225A54C(UnkStruct_ov70_0225DEE8 *param0);
void ov49_0225A56C(UnkStruct_ov70_0225DEE8 *param0, u32 param1);
BOOL ov49_0225A5AC(const UnkStruct_ov70_0225DEE8 *param0, u32 param1);
typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;
u32 ov49_0225EF88(const UnkStruct_ov70_02263344 *param0);
void ov49_0225EF8C(UnkStruct_ov70_02263344 *param0, u32 param1);
BOOL ov49_0225F448(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

fx32 ov49_0225F394(UnkStruct_ov70_02263270 *param0) {
    u16 v0;

    switch (param0->unk_05) {
    case 0:
        param0->unk_00 = 0;
        break;
    case 1:
        if ((param0->unk_06 - 1) > 0) {
            param0->unk_06--;
        } else {
            param0->unk_05 = 2;
        }
        break;
    case 2:
        param0->unk_04++;

        v0 = (param0->unk_04 * 0x7fff) / 4;
        param0->unk_00 = FX_Mul(FX_SinIdx(v0), (FX32_CONST(6)));

        if (param0->unk_04 >= 4) {
            if (param0->unk_07 == 1) {
                param0->unk_04 = 0;
                param0->unk_05 = 1;
                param0->unk_06 = MTRandom() % 4;
            } else {
                param0->unk_05 = 0;
            }
        }
        break;
    }

    return param0->unk_00;
}

void ov49_0225F430(void *arg0) {
    *((s8 *)&((UnkStruct_ov70_02263270 *)arg0)->unk_07) = 0;
}

s32 ov49_0225F438(void *arg0) {
    if (((UnkStruct_ov70_02263270 *)arg0)->unk_05 != 0) {
        return 1;
    }
    return 0;
}

BOOL ov49_0225F448(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2) {
    UnkStruct_ov70_0225CA20 *v0 = ov49_02259FF0(param1);
    UnkStruct_ov70_0225CC54 *v1 = ov49_02258D70(v0, param2);
    WiFiPlazaSystem *v2 = ov49_02259FE8(param1);

    switch (ov49_0225EF88(param0)) {
    case 0:
        ov49_0225A56C(param1, param2);
        ov49_0225EF8C(param0, 1);

        if (param2 == ov45_0222A53C(v2)) {
            ov49_0225A53C(param1, 0);
        }
        break;
    case 1:
        if (ov49_0225A5AC(param1, param2) == 1) {
            ov49_0225EF8C(param0, 2);
        }
        break;
    case 2:
        if (param2 == ov45_0222A53C(v2)) {
            ov49_02258EEC(v0, v1, 1);
        } else {
            if (ov49_0225A4F0(param1) == 1) {
                if (param2 != ov49_0225A4E0(param1)) {
                    ov49_02258EEC(v0, v1, 2);
                }
            } else {
                ov49_02258EEC(v0, v1, 2);
            }

            ov49_0225A04C(param1, param2, 0);
        }

        ov49_0225A54C(param1);

        return 1;
    }

    return 0;
}
