#include "global.h"

#include "network_icon.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_0203A3B0.h"

void sub_0203A478(NetworkIcon *, s32, s32);

void sub_0203A948(s32 arg0, s32 arg1);

void sub_0203A948(s32 arg0, s32 arg1) {
    if (_021D4154 != 0) {
        sub_0203A478(_021D4154, arg0, arg1);
    }
}

void sub_0203A964(void) {
    if (sub_02037474()) {
        if (sub_02037454() > 1 || sub_02039998()) {
            sub_0203A880();
        }
    } else if (sub_02039998()) {
        sub_0203A880();
    }
}
