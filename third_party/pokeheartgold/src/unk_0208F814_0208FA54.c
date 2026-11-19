#include "global.h"

#include "unk_02095EAC.h"
#include "error_handling.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_0208F814.h"

void ov37_021E6FC8(UnkStruct_02095EAC *param0, int param1, u8 param2);

void sub_0208FAF8(UnkStruct_02095EAC *param0, int param1);

BOOL sub_02037108(int cmd, const void *data, int size);

void sub_0208FA54(int param0, int param1, void *param2, void *param3) {
    UnkStruct_02095EAC *v0 = (UnkStruct_02095EAC *)param3;
    u8 v1 = *(u8 *)param2;

    ov37_021E6FC8(v0, 1, v1);

    if ((sub_0203769C() == 0) && (v0->unk_364 == 1)) {
        v0->unk_4430 = 0;
        sub_0208FAF8(v0, v0->unk_4430);
    }
}

void sub_0208FA88(void) {
    return;
}

void sub_0208FA8C(void) {
    return;
}

void sub_0208FA90(int param0, int param1, void *param2, void *param3) {
    UnkStruct_02095EAC *v0 = (UnkStruct_02095EAC *)param3;

    if (sub_0203769C() != 0) {
        ov37_021E6FC8(v0, 15, 0);
    }
}

void sub_0208FAA8(int param0, int param1, void *param2, void *param3) {
    UnkStruct_02095EAC *v0 = (UnkStruct_02095EAC *)param3;
    u8 v1;

    GF_ASSERT(sub_0203769C() == 0);

    if (sub_0203769C() == 0) {
        if (v0->unk_9460 != 0) {
            v1 = param0;
            sub_02037108(123, &v1, 1);
        } else {
            v0->unk_9460 = 1;
        }
    }

    sub_020398D4(0, 1);
}

void sub_0208FAEC(int param0, int param1, void *param2, void *param3) {
    UnkStruct_02095EAC *v0 = (UnkStruct_02095EAC *)param3;
    v0->unk_9458 = 1;
}
