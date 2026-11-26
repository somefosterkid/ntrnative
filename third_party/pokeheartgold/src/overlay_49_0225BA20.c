#include "global.h"
#include "overlay_49_unk_struct_ov70_0225f350.h"

void ov49_0225CAA8(void *param_1, void **param_2);

void ov49_0225BA20(UnkStruct_ov70_0225F350 *param_1, void **param_2);

void ov49_0225BA34(UnkStruct_ov70_0225F350 *arg0, s16 arg1);

void ov49_0225BA20(UnkStruct_ov70_0225F350 *param_1, void **param_2) {
    ov49_0225CAA8(&param_1->unk_114, param_2);
    param_1->unk_01 = 0;
    return;
}

void ov49_0225BA34(UnkStruct_ov70_0225F350 *arg0, s16 arg1) {
    arg0->unk_114.unk_06 = arg1;
}
