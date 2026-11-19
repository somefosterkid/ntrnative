#include "global.h"
#include "overlay_96_ov96_rd_obj.h"
struct Ov96RdObj;

s32 ov96_02218688(Ov96RdObj *obj);

s32 ov96_02218688(Ov96RdObj *obj) {
    if (obj->f27) {
        return 0;
    }
    if (obj->f28) {
        return 0;
    }
    if (obj->f26) {
        return 0;
    }
    if (obj->state == 1) {
        return 1;
    }
    return 0;
}

void ov96_022186B8(struct Ov96RdObj *param_1, s16 *param_2);

void ov96_022186B8(Ov96RdObj *param_1, s16 *param_2) {
    param_1->pos20.x = (s32)*param_2 << 0xc;
    param_1->pos20.y = (s32)param_2[1] << 0xc;
    return;
}
