#include "global.h"

#include "error_handling.h"
#include "overlay_96_ov96_rd_obj.h"

void ov96_02218578(Ov96RdObj *obj, s32 state);
s32 ov96_022186CC(VecFx32 *dir);

void ov96_02218DF8(Ov96RdObj *obj, Ov96RdPos16 *target, s32 arg2);

void ov96_02218DF8(Ov96RdObj *obj, Ov96RdPos16 *target, s32 arg2) {
    VecFx32 sp0 = { 0, 0, 0 };
    s32 temp_r0;
    s32 var_r2;

    temp_r0 = obj->state;
    var_r2 = 1;
    if ((temp_r0 != 1) && (temp_r0 != 4)) {
        var_r2 = 0;
    }
    if (var_r2 == 0) {
        GF_AssertFail();
    }
    if (obj->f28) {
        GF_AssertFail();
    }
    if (obj->f26) {
        GF_AssertFail();
    }
    obj->f30 = 0;
    sp0.x = target->x << 0xC;
    sp0.y = target->y << 0xC;
    VEC_Subtract(&sp0, &obj->pos2C, &obj->dir);
    VEC_Normalize(&obj->dir, &obj->dir);
    VEC_MultAdd(0x4000, &obj->dir, &obj->dir, &obj->vel);
    if ((obj->vel.x != 0) || (obj->vel.y != 0)) {
        obj->f16 = ov96_022186CC(&obj->dir);
        obj->timer59 = 4;
        ov96_02218578(obj, 5);
    }
}
