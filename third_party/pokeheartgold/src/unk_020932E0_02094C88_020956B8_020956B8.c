#include "global.h"

#include "unk_020932E0.h"

void sub_020956B8(UnkStruct_020932E0 *arg0) {
    s32 idx;
    s32 var_r6;
    s32 var_r5;
    if (arg0->unk_46D8 != NULL) {
        var_r6 = -1;
        var_r5 = -1;
        switch (sub_02095DD8(arg0->unk_46B8)) {
        case 0:
        case 1:
        case 5:
            break;
        case 2:
        case 6:
            var_r5 = arg0->unk_F;
            idx = sub_02095DE8(arg0->unk_46B8);
            if (sub_020947C0(arg0, idx) == 1) {
                var_r6 = idx;
            }
            break;
        case 3:
            idx = sub_02095DE8(arg0->unk_46B8);
            if (sub_02094894(arg0, idx) == 1) {
                var_r5 = arg0->unk_884[idx].unk_0;
                var_r6 = arg0->unk_884[idx].unk_4;
            }
            break;
        case 4:
            if (arg0->unk_4680 != 0) {
                var_r5 = arg0->unk_F;
                var_r6 = arg0->unk_4684;
            }
            break;
        }
        if (arg0->unk_46E4 != var_r5 || arg0->unk_46E0 != var_r6) {
            arg0->unk_46D8(var_r5, var_r6, arg0->unk_46DC);
        }
        arg0->unk_46E4 = var_r5;
        arg0->unk_46E0 = var_r6;
    }
}
