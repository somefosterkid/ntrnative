#include "global.h"

#include "overlay_96_app_02210C98.h"

void ov96_02213444(Ov96App_02210C98_Mover *arg0);

void ov96_02213444(Ov96App_02210C98_Mover *arg0) {
    VecFx32 v = { 0, 0, 0 };
    Ov96App_02210C98_Mover *p = arg0;
    u8 i;

    i = 0;
    p->unk_14 = v;
    p->unk_20 = v;
    p->unk_38 = 0;
    p->unk_39 = 3;
    p->unk_3A = 0;
    p->unk_3B = 12;
    p->unk_3C = 0;
    p->unk_3D = 0;
    p->unk_3E = 0;
    p->unk_40 = 0;
    p->unk_41 = 0;
    p->unk_43 = 0;
    p->unk_45 = 0;
    p->unk_46 = 0;

    do {
        p->unk_2C[i] = 0;
        i++;
    } while (i < 12);
}
