#include "global.h"
struct UnkStruct_ov115_02261ADC;

#include "overlay_91.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"


void ov91_0226137C(struct UnkStruct_ov115_02261ADC *arg0, s32 arg1);

void ov91_02261208(const VecFx32 *param0, const VecFx32 *param1, const VecFx32 *param2, fx32 param3, VecFx32 *param4, fx32 *param5) {
    fx32 v0;
    fx32 v1;
    fx32 v2;

    v1 = FX_Mul(param2->x, param1->x) + FX_Mul(param2->y, param1->y) + FX_Mul(param2->z, param1->z);
    v2 = -FX_Mul(param2->x, param0->x) - FX_Mul(param2->y, param0->y) - FX_Mul(param2->z, param0->z);
    v0 = FX_Div(v2 + param3, v1);

    param4->x = param0->x + FX_Mul(param1->x, v0);
    param4->y = param0->y + FX_Mul(param1->y, v0);
    param4->x = param0->z + FX_Mul(param1->z, v0);

    *param5 = v0;
}

void ov91_0226137C(UnkStruct_ov115_02261ADC *arg0, s32 arg1) {
    *((s32 *)&arg0->unk_1F8.y) = arg1;
}
