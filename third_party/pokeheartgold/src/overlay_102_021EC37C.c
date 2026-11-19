#include "global.h"

#include "sys_task.h"
#include "sys_task_api.h"

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    SysTask *unk_1C;
} UnkStruct_ov20_021D4FF0;

SysTask *ov102_021E93D4(SysTaskFunc param0, void *param1, int param2);

void ov102_021EC3D4(SysTask *param0, void *param1);

void ov102_021EC37C(UnkStruct_ov20_021D4FF0 *param0, int param1, int param2, int param3, int param4, int param5);

BOOL ov102_021EC3AC(UnkStruct_ov20_021D4FF0 *param0);

void ov102_021EC37C(UnkStruct_ov20_021D4FF0 *param0, int param1, int param2, int param3, int param4, int param5) {
    param0->unk_00 = param1;
    param0->unk_04 = param2;
    param0->unk_14 = param5;
    param0->unk_08 = param3;
    param0->unk_10 = (fx32)(param4 - param3) / param5;
    param0->unk_0C = param4;
    param0->unk_18 = 0;
    param0->unk_1C = ov102_021E93D4(ov102_021EC3D4, param0, 0);
}

BOOL ov102_021EC3AC(UnkStruct_ov20_021D4FF0 *param0) {
    switch (param0->unk_18) {
    case 0:
        if (param0->unk_14 == 0) {
            SysTask_Destroy(param0->unk_1C);
            param0->unk_18++;
            return 1;
        }
        break;
    default:
        return 1;
    }

    return 0;
}

void ov102_021EC3D4(SysTask *param0, void *param1) {
    UnkStruct_ov20_021D4FF0 *v0 = param1;
    int v1, v2;

    if (v0->unk_14) {
        v0->unk_08 += v0->unk_10;
        v1 = v0->unk_08 >> 3;
        v0->unk_14--;
    } else {
        v1 = v0->unk_0C >> 3;
    }

    if (v1 > 16) {
        v1 = 16;
    }

    v2 = 16 - v1;

    G2_SetBlendAlpha(v0->unk_00, v0->unk_04, v1, v2);
}
