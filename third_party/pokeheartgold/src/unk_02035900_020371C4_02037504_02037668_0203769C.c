#include "global.h"

#include "unk_02033AE0.h"
#include "unk_02037C94.h"
#include "unk_02035900_work.h"

int ov00_021E6964(void);

u16 sub_020338F4(void);

u16 sub_0203769C(void);

u16 sub_0203769C(void) {
    s32 temp_r0;

    if (_021D4140.sCommunicationSystem != NULL) {
        if (sub_02034044((s32)sub_0203993C()) != 0) {
            temp_r0 = ov00_021E6964();
            if (temp_r0 != -1) {
                return (u16)temp_r0;
            }
        } else {
            return sub_020338F4();
        }
    }
    return 0U;
}
