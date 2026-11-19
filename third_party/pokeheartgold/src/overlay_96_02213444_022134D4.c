#include "global.h"

#include "overlay_96_app_02210C98.h"

void ov96_022134D4(Ov96App_02210C98 *arg0, Ov96App_02210C98_Mover *arg1, VecFx32 *arg2);

void ov96_022134D4(Ov96App_02210C98 *arg0, Ov96App_02210C98_Mover *arg1, VecFx32 *arg2) {
    VecFx32 *temp_r2;
    int new_var;
    VecFx32 sp0 = { 0, 0, 0 };
    VEC_Normalize(arg2, &arg1->unk_20);
    arg1->unk_14 = arg1->unk_20;
    temp_r2 = &arg1->unk_14;
    new_var = 0xC;
    VEC_MultAdd(arg1->unk_40 << new_var, temp_r2, &sp0, temp_r2);
}
