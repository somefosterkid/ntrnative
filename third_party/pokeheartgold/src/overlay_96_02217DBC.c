#include "global.h"

#include "error_handling.h"

#include "overlay_96_ov96_rd_work.h"

void ov96_021EA894(struct Ov96SpriteSet *set);

void ov96_02217DBC(Ov96RdWork *work);

void ov96_02217DBC(Ov96RdWork *work) {
    ov96_021EA894(work->spriteSet);
    return;
}

void ov96_02218578(Ov96RdObj *param_1, s32 param_2);

void ov96_02218FFC(Ov96RdObj *param_1);

void ov96_02217DC8(Ov96RdObj *param_1, u32 param_2);

void ov96_02217DC8(Ov96RdObj *param_1, u32 param_2) {
    switch (param_2) {
    case 0:
        break;
    case 1:
        ov96_02218578(param_1, 1);
        return;
    case 2:
        ov96_02218578(param_1, 7);
        param_1->timer59 = 1;
        return;
    case 3:
        ov96_02218FFC(param_1);
        return;
    default:
        GF_AssertFail();
    }
    return;
}
