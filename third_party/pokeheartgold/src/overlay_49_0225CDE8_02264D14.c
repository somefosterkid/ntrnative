#include "global.h"

#include "overlay_49.h"
#include "overlay_49_unk_struct_ov49_02264cf8.h"

s32 ov49_0225EF8C(void *, s32);

void ov49_02264D14(UnkStruct_ov49_02264CF8 *arg0, void *arg1) {
    u8 v0;

    v0 = arg0->unk_06;
    if (v0 != 0) {
        arg0->unk_06 = (u8)(v0 - 1);
        return;
    }
    ov49_0225EF8C(arg1, arg0->unk_07);
}

s32 ov49_0225A0AC(void *);

void ov49_02264D30(UnkStruct_ov49_02264CF8 *arg0, void *arg1, void *arg2) {
    if (ov49_0225A0AC(arg2) != 0) {
        ov49_0225EF8C(arg1, arg0->unk_07);
    }
}
