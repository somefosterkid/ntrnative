#include "global.h"

#include "overlay_96_app_02210C98.h"

s32 ov96_02212F94(Ov96App_02210C98_Mover *, Ov96App_02210C98_Mover *, u8);

void ov96_02212F0C(Ov96App_02210C98_Mover *arg0, Ov96App_02210C98_Mover *arg1);

void ov96_02212F0C(Ov96App_02210C98_Mover *arg0, Ov96App_02210C98_Mover *arg1) {
    Ov96App_02210C98_Mover sp;

    sp.unk_14 = arg0->unk_14;
    sp.unk_20 = arg0->unk_20;
    sp.unk_39 = arg0->unk_39;
    sp.unk_3B = arg0->unk_3B;
    sp.unk_3C = arg0->unk_3C;
    sp.unk_3D = arg0->unk_3D;
    sp.unk_3E = arg0->unk_3E;
    sp.unk_40 = arg0->unk_40;
    sp.unk_41 = arg0->unk_41;
    sp.unk_8 = arg0->unk_8;
    ov96_02212F94(arg0, arg1, 0);
    ov96_02212F94(arg1, &sp, 1);
}
