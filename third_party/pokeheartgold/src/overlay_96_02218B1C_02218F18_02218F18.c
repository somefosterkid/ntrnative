#include "global.h"

#include "overlay_96_ov96_rd_obj.h"

void ov96_02218F18(Ov96RdObj *param_1);

void ov96_02218F18(Ov96RdObj *param_1) {
    VecFx32 VStack_14;
    VecFx32 VStack_20 = { 0, 0, 0 };
    VEC_MultAdd(0xb33, &param_1->dir, &VStack_20, &VStack_14);
    VEC_Add(&param_1->vel, &VStack_14, &param_1->vel);
    VEC_Add(&param_1->pos2C, &param_1->vel, &param_1->pos2C);
    return;
}
