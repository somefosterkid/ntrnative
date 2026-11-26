#include "global.h"

#include "math_util.h"
#include "overlay_49_getters.h"
#include "overlay_49_plaza_api.h"
#include "overlay_49_unk_struct_ov70_02263270.h"

void ov49_0225F334(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2, u32 param3, int param4);

void ov49_0225F334(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2, u32 param3, int param4) {
    UnkStruct_ov70_02260AD4 *v1;
    u32 v0;
    UnkStruct_ov70_02260AD4 *v2;
    UnkStruct_ov70_0225CC54 *v3;
    UnkStruct_ov70_0225CA20 *v4;

    v0 = param3;
    v4 = ov49_02259FF0(param0);
    v2 = ov49_0225A008(param0);
    v1 = v2;
    v3 = ov49_02258C5C(v4, param1, param2, param3);
    ov49_02258E7C(v4, v3, 0, param4);
    ov49_0225CC40(v2, v3);
}

void ov49_0225F374(UnkStruct_ov70_02263270 *param0);

void ov49_0225F374(UnkStruct_ov70_02263270 *param0) {
    param0->unk_00 = 0;
    param0->unk_04 = 0;
    param0->unk_05 = 1;
    param0->unk_06 = MTRandom() & 3;
    param0->unk_07 = 1;
}
