#include "global.h"

#include "unk_020932E0.h"

void sub_0209511C(UnkStruct_020932E0 *arg0) {
    int var_r4;
    var_r4 = 0;
    if ((s32)arg0->unk_D > 0) {
        do {
            arg0->unk_46B4[var_r4].unk_0 = arg0->unk_884[var_r4].unk_0;
            arg0->unk_46B4[var_r4].unk_4 = arg0->unk_884[var_r4].unk_4;
            var_r4 += 1;
        } while (var_r4 < (s32)arg0->unk_D);
    }
}
