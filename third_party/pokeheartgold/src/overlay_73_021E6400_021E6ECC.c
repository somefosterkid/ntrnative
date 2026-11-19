#include "global.h"

#include "bg_window.h"
#include "text.h"
#include "unk_02035900.h"
#include "overlay_73_unk_struct_020961_e8_t.h"

typedef struct UnkStruct_ov73_021E6ECC {
    u8 unk_0[0x4C];
    s32 unk_4C;
    u8 unk_50[0x2C8];
    s32 unk_318;
} UnkStruct_ov73_021E6ECC;

s32 ov73_021E66F0(void *);

s32 ov73_021E756C(void *, s32, s32);

s32 ov73_021E75FC(s32);

s32 ov73_021E6ECC(UnkStruct_ov73_021E6ECC *arg0, s32 arg1);

s32 ov73_021E6ECC(UnkStruct_ov73_021E6ECC *arg0, s32 arg1) {
    if (ov73_021E75FC(arg0->unk_4C) != 0) {
        ov73_021E756C(arg0, 9, 0);
    }
    arg0->unk_318 = 0x12;
    ov73_021E66F0(arg0);
    return arg1;
}

s32 ov73_021E6EF8(void *arg0, s32 arg1);

s32 ov73_021E6EF8(void *arg0, s32 arg1) {
    if (ov73_021E75FC(*(s32 *)((u8 *)arg0 + 0x4C)) != 0) {
        *(s32 *)((u8 *)arg0 + 0x318) = 0xA;
    }
    ov73_021E66F0(arg0);
    return arg1;
}

s32 ov73_021E71E4(void *, s32, s8);

s32 ov73_021E781C(void *, s32);

s32 ov73_021E6F1C(UnkStruct_020961E8 *arg0, s32 arg1);

s32 ov73_021E6F1C(UnkStruct_020961E8 *arg0, s32 arg1) {
    if (sub_0203769C() == 0) {
        ov73_021E71E4(arg0, 0, 2);
    }
    arg0->unk_4A38 = -1;
    ov73_021E71E4(arg0, 1, 2);
    ScheduleBgTilemapBufferTransfer(arg0->unk_00, 1U);
    if ((arg0->unk_4C != 0xFF) && (ov73_021E75FC(arg0->unk_4C) == 0)) {
        RemoveTextPrinter((u8) arg0->unk_4C);
    }
    ov73_021E756C(arg0, 0xF, 1);
    arg0->unk_3A8 = 0x14;
    if (sub_0203769C() == 0) {
        ov73_021E781C(arg0, 0);
    }
    arg0->unk_4ABC = sub_02037454();
    arg0->unk_4AC4 = 0x12C;
    ov73_021E66F0(arg0);
    return arg1;
}
