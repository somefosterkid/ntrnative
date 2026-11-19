#include "global.h"
#include "overlay_96_ov96_rd_obj.h"

void ov96_02219174(Ov96RdObj *obj, s32 amount);

void ov96_02219174(Ov96RdObj *obj, s32 amount) {
    obj->info->value = obj->info->value - amount;
    if (obj->info->value < 0) {
        obj->info->value = 0;
    }
}
