#include "global.h"

#include "unk_02033AE0.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"

void ov00_021E70CC(s16 enabled);

void sub_02037930(BOOL param0);

void sub_02037930(BOOL param0) {
    sub_020378E4(param0);

    if (sub_02034044(sub_0203993C())) {
        if (param0) {
            ov00_021E70CC(0);
        } else {
            ov00_021E70CC(1);
        }
    }
}
