#include "global.h"

#include "overlay_96_ov96_rd_obj.h"

void ov96_02218578(Ov96RdObj *param_1, s32 param_2);

void ov96_02218FFC(Ov96RdObj *param_1);

void ov96_02218FFC(Ov96RdObj *param_1) {
    fx32 fVar1;
    s32 iVar2;

    fVar1 = FX_Div(param_1->info->value, param_1->info->stats->s2);
    iVar2 = (0x1000 - fVar1) * 10 + -3;
    if (iVar2 < 0) {
        iVar2 = 0;
    }
    param_1->timer58 = iVar2 >> 0xc;
    ov96_02218578(param_1, 2);
    return;
}
