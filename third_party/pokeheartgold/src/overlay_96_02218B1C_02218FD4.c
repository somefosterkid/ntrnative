#include "global.h"
#include "overlay_96_ov96_rd_obj.h"

void ov96_02218578(Ov96RdObj *param_1, s32 param_2);

void ov96_02218FD4(Ov96RdObj *param_1);

void ov96_02218FD4(Ov96RdObj *param_1) {
    ov96_02218578(param_1, 0xc);
    param_1->timer5A = 10;
    param_1->unk_50 = 0;
    param_1->pos20 = param_1->pos2C;
    return;
}
