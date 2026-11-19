#include "global.h"

#include "overlay_96_ov96_rd_obj.h"

void ov96_02218A68(Ov96RdObj *param_1);

void ov96_02219398(Ov96RdObj *obj);

void ov96_02218A50(Ov96RdObj *param_1);

void ov96_02218A50(Ov96RdObj *param_1) {
    param_1->info->value = param_1->info->stats->s2;
    ov96_02219398(param_1);
    ov96_02218A68(param_1);
    return;
}
