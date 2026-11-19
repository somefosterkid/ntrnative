#include "global.h"

#include "unk_02031B0C.h"

extern const ApricornJuice _020F6902[5];

BOOL sub_02031CEC(SaveApricornBox *apricornBox, int a1, ApricornJuice *a2) {
    ApricornJuice *temp_r4;
    int var_r1;
    var_r1 = a1;
    if (((s32)var_r1) >= 9) {
        var_r1 = 0;
    }
    if (((s32)var_r1) <= 4) {
        *a2 = _020F6902[var_r1];
    } else {
        if (((s32)var_r1) <= 7) {
            temp_r4 = &apricornBox->juiceRecords[var_r1 - 5].data;
            if (sub_02031D80(temp_r4) == 0) {
                sub_02031B5C(a2);
                return 0;
            }
            *a2 = *temp_r4;
        } else {
            if (apricornBox->juiceServings == 0) {
                sub_02031B5C(a2);
                return 0;
            }
            *a2 = apricornBox->juice;
        }
    }
    return 1;
}
