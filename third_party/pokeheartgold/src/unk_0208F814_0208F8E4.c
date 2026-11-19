#include "global.h"

#include "unk_02095EAC.h"
#include "unk_02032844.h"
#include "unk_02035900.h"
#include "unk_0208F814.h"

void sub_0208F8E4(int param0, int param1, void *param2, void *param3) {
    UnkStruct_02095EAC *v0 = (UnkStruct_02095EAC *)param3;
    UnkStruct_ov58_021D2820 *v1 = (UnkStruct_ov58_021D2820 *)param2;

    if (v0 == NULL) {
        return;
    }

    if (param0 == 0) {
        int v2;

        for (v2 = 0; v2 < 5; v2++) {
            v0->unk_43E6[v2] = v1[v2];
        }
    }

    if (v0->unk_43E6[0].unk_09 == 2) {
        (void)0;
    }
}

void ov37_021E6FC8(UnkStruct_02095EAC *param0, int param1, u8 param2);

void sub_0208F918(int param0, int param1, void *param2, void *param3) {
    UnkStruct_02095EAC *v0 = (UnkStruct_02095EAC *)param3;

    ov37_021E6FC8(v0, 3, 0);
    MI_CpuClearFast(v0->unk_4434, 30 * 15 * 32);

    if (sub_0203769C() == 0) {
        v0->unk_37C = sub_02037454();
        v0->unk_380 = sub_02033250();
        v0->unk_9458 = 1;
    }
}
