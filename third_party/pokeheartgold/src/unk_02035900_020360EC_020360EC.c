#include "global.h"

#include "unk_02035900.h"
#include "unk_02035900_work.h"

s32 sub_02036508(void);

s32 sub_02036AD8(void);

void sub_020360EC(SysTask *task, void *data);

void sub_020360EC(SysTask *task, void *data) {
    if (_021D4140.unk_01 != 0) {
        sub_02036AD8();
        if ((sub_0203769C() == 0) && (sub_020373B4(0U) != 0)) {
            sub_02036508();
        }
        _021D4140.unk_01 = 0U;
    }
}

s32 sub_0203567C(void);

s32 sub_020357FC(void);

void sub_0203611C(void);

void sub_0203611C(void) {
    if (sub_020357FC() != 0) {
        if (sub_0203769C() == 0) {
            if (sub_0203567C() == 0) {
                sub_02036024();
            }
        } else {
            sub_02036024();
        }
    }
}
