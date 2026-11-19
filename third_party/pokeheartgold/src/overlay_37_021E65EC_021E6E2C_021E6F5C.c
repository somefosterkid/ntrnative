#include "global.h"

#include "unk_02095EAC.h"
#include "unk_02035900.h"

void ov37_021E68AC(UnkStruct_02095EAC *param0);

void ov37_021E76C0(UnkStruct_02095EAC *param0);

void ov37_021E76D0(UnkStruct_02095EAC *param0, int param1);

void ov37_021E7844(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6F5C(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6F5C(UnkStruct_02095EAC *param0, int param1) {
    if ((param0->unk_9414 != 0) && (sub_02037454() != param0->unk_9414)) {
        param0->unk_9414 = 0;
    }

    if ((++param0->unk_374 > 60) && (param0->unk_9414 == 0)) {
        ov37_021E76C0(param0);
        ov37_021E7844(param0, 4);

        if (sub_0203769C() == 0) {
            param0->unk_9458 = 1;
            ov37_021E76D0(param0, 1);
        }
    }

    ov37_021E68AC(param0);
    return param1;
}
