#include "global.h"

#include "network_icon.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"

s32 sub_0203A8A8(s32);

void sub_0203A994(s32 a0);

void sub_0203A994(s32 a0) {
    if (sub_02037474() != 0) {
        if ((sub_02037454() > 1) || (sub_02039998() != 0)) {
            sub_0203A8A8(a0);
        }
    } else if (sub_02039998() != 0) {
        sub_0203A8A8(a0);
    }
}

void sub_0203A9C8(void);

void sub_0203A9C8(void) {
    if (_021D4154 != 0) {
        _021D4154->oamMode = 1;
    }
    return;
}
