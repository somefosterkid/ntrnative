#include "global.h"

#include "error_handling.h"
#include "overlay_96_ov96_rd_obj.h"

void ov96_02218AF0(void *arg0);

void ov96_02218578(Ov96RdObj *param_1, s32 param_2);

s32 ov96_022187A8(void *, void *);

s32 ov96_02218934(void *);

s32 ov96_02218A50(void *);

s32 ov96_02218DF8(void *, void *);

s32 ov96_02218EB8(void *);

s32 ov96_02218F18(void *, void *);

void ov96_02218F58(void *arg0);

s32 ov96_02218FD4(void *);

s32 ov96_02219030(void *);

extern s32 ov96_0221D69C;

extern s32 ov96_0221D7D0;


void ov96_02218B1C(Ov96RdObj *arg0, void *arg1);

void ov96_02218B1C(Ov96RdObj *arg0, void *arg1) {
    s32 sp0[3];
    s32 temp_r0_5;
    s32 temp_r0_6;
    s8 temp_r0;
    u32 temp_r0_2;
    u32 temp_r0_3;
    u32 var_r5;
    VecFx32 *new_var;
    Ov96RdObj *var_r4;
    var_r4 = arg0;
    temp_r0 = var_r4->b5D;
    if (temp_r0 > 0) {
        var_r4->b5D = temp_r0 - 1;
        if (var_r4->b5D > 0) {
            return;
        }
    }
    temp_r0_2 = (u32)var_r4->state;
    switch (temp_r0_2) {
    case 5:

    case 7:

    case 8:

    case 10:

    case 11:

    case 12:
        break;

    case 6:
        ov96_02218F18(var_r4, arg1);
        break;

    default:

    case 0:

    case 1:

    case 2:

    case 3:

    case 4:

    case 9:
        ov96_022187A8(var_r4, arg1);
        break;
    }

    temp_r0_3 = (u32)var_r4->state;
    switch (temp_r0_3) {
    case 4:
        if (var_r4->f27) {
            GF_AssertFail();
        }
        if (ov96_02218934(var_r4) != 0) {
            ov96_02219030(var_r4);
        } else {
            if (var_r4->f29) {
                ov96_02218F58(var_r4);
            } else if (var_r4->f30) {
                ov96_02218DF8(var_r4, &var_r4->target);
            } else {
                var_r4->timer5E = var_r4->timer5E - 1;
                if (var_r4->timer5E <= 0) {
                    ov96_02218578(var_r4, 1);
                }
            }
        }
        break;

    case 3:
        var_r4->timer5B = var_r4->timer5B - 1;
        if (var_r4->timer5B <= 0) {
            ov96_02218A50(var_r4);
            ov96_02218578(var_r4, 4);
        }
        break;

    case 2:
        if (ov96_02218934(var_r4) != 0) {
            ov96_02219030(var_r4);
        } else {
            arg0->timer58 = arg0->timer58 - 1;
            if (arg0->timer58 <= 0) {
                ov96_02218578(arg0, 1);
            }
        }
        break;

    case 1:
        if (ov96_02218934(arg0) != 0) {
            ov96_02219030(arg0);
        } else {
            if (!var_r4->f27) {
                if (var_r4->f29) {
                    ov96_02218F58(arg0);
                } else if (var_r4->f30) {
                    ov96_02218DF8(arg0, &arg0->target);
                }
            }
        }
        break;

    case 8:
        arg0->timer5A = arg0->timer5A - 1;
        if (arg0->timer5A <= 0) {
            arg0->timer5A = 0;
            ov96_02218578(arg0, 9);
        }
        break;

    case 9:
        var_r5 = var_r4->info->stats->k2;
        var_r5 *= 0x14;
        arg0->unk_50 = (s32)((*((s16 *)(((arg0->timer5A) * 2) + (((u8 *)(&ov96_0221D7D0)) + var_r5)))) << 0xC);
        arg0->timer5A = arg0->timer5A + 1;
        if (arg0->timer5A < 0xA) {
        } else {
            arg0->timer5A = 0xA;
            ov96_02218578(arg0, 10);
        }
        break;

    case 10:
        arg0->timer5A = arg0->timer5A - 1;
        if (arg0->timer5A <= 0) {
            arg0->timer5A = 0;
            ov96_02218578(arg0, 0xB);
        }
        break;

    case 11:
        temp_r0_5 = var_r4->unk_50;
        if (temp_r0_5 == 0) {
            ov96_02218FD4(arg0);
            if (ov96_02218934(arg0) != 0) {
                ov96_02219030(arg0);
            }
        } else {
            temp_r0_6 = temp_r0_5 - ((*((u8 *)(((u8 *)(&ov96_0221D69C)) + var_r4->info->stats->k2))) << 0xC);
            arg0->unk_50 = temp_r0_6;
            if (temp_r0_6 <= 0) {
                arg0->unk_50 = 0;
            }
        }
        break;

    case 12:
        arg0->timer5A = arg0->timer5A - 1;
        if (arg0->timer5A <= 0) {
            ov96_02218578(arg0, 1);
        }
        break;

    case 5:
        arg0->timer59 = arg0->timer59 - 1;
        if (arg0->timer59 <= 0) {
            arg0->timer59 = 6;
            ov96_02218578(arg0, 6);
        }
        break;

    case 6:
        arg0->timer59 = arg0->timer59 - 1;
        if (arg0->timer59 <= 0) {
            arg0->pos20 = arg0->pos2C;
            new_var = (VecFx32 *)sp0;
            new_var->x = 0;
            new_var->y = 0;
            new_var->z = 0;
            arg0->vel = *new_var;
            ov96_02218578(arg0, 7);
        }
        break;

    case 7:
        arg0->timer59 = arg0->timer59 - 1;
        if (arg0->timer59 <= 0) {
            ov96_02218EB8(arg0);
        }
        break;

    default:
        GF_AssertFail();
        break;
    }

    ov96_02218AF0(arg0);
}
