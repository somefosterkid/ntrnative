#include "global.h"
#include "overlay_96_ov96_rd_obj.h"

void ov96_02218578(struct Ov96RdObj *param_1, s32 param_2);

void ov96_021EAF78(void *sprite, s32 x, s32 y, VecFx32 *out, s32 *outY, s32 *radius);
s32 ov96_02218598(Ov96RdObj *arg0, Ov96RdObj *arg1);

void ov96_02218578(Ov96RdObj *param_1, s32 param_2) {
    u8 uVar1;

    if (param_2 == 4) {
        uVar1 = 0x3c;
    } else {
        uVar1 = 0;
    }
    param_1->timer5E = uVar1;
    if (param_2 == 7) {
        param_1->timer59 = 0x10;
    }
    param_1->state = param_2;
    return;
}

s32 ov96_02218598(Ov96RdObj *arg0, Ov96RdObj *arg1) {
    VecFx32 v0;
    VecFx32 a;
    VecFx32 b;
    s32 v2;
    s32 v1;
    s32 v3;
    int new_var;
    v3 = arg0->unk_50 - arg1->unk_50;
    if (v3 < 0) {
        v3 = 0 - v3;
    }
    if (v3 > 0x5000) {
        return 0;
    }
    ov96_021EAF78(arg0->info->sprite, arg0->pos2C.x, arg0->pos2C.y, &a, &a.y, &v2);
    ov96_021EAF78(arg1->info->sprite, arg1->pos2C.x, arg1->pos2C.y, &b, &b.y, &v1);
    if (arg0->state == 6) {
        v2 += 8;
    }
    if (arg1->state == 6) {
        v1 += 8;
    }
    new_var = (v2 + v1) << 0xC;
    a.z = 0;
    b.z = 0;
    VEC_Subtract(&a, &b, &v0);
    if (VEC_Mag(&v0) <= ((s32)new_var)) {
        return 1;
    }
    return 0;
}
