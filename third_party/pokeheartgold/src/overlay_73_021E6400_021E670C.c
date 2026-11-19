#include "global.h"

#include "bg_window.h"
#include "unk_02035900.h"

typedef struct UnkStruct_ov73_021E671C {
    BgConfig *unk_0;
    u8 unk_4[0x48];
    s32 unk_4C;
    u8 unk_50[0x2C8];
    s32 unk_318;
    s32 unk_31C;
    u8 unk_320[0x4718];
    s32 unk_4A38;
} UnkStruct_ov73_021E671C;

void ov73_021E670C(void *arg0, s32 arg1);

void ov73_021E670C(void *arg0, s32 arg1) {
    *(s32 *)((u8 *)arg0 + 0x318) = 0x1E;
    *(s32 *)((u8 *)arg0 + 0x31C) = arg1;
}

s32 ov73_021E66F0(void *);

s32 ov73_021E71E4(void *, s32, s8);

s32 ov73_021E75FC(s32);

s32 ov73_021E671C(UnkStruct_ov73_021E671C *arg0, s32 arg1);

s32 ov73_021E671C(UnkStruct_ov73_021E671C *arg0, s32 arg1) {
    if (ov73_021E75FC(arg0->unk_4C) != 0) {
        arg0->unk_318 = (s32) arg0->unk_31C;
        if (arg0->unk_31C == 3) {
            if (sub_0203769C() == 0) {
                ov73_021E71E4(arg0, 0, 0);
            }
            arg0->unk_4A38 = -1;
            ov73_021E71E4(arg0, 1, 0);
            ScheduleBgTilemapBufferTransfer(arg0->unk_0, 1U);
        }
    }
    ov73_021E66F0(arg0);
    return arg1;
}

s32 ov73_021E6778(void *arg0, s32 arg1);

s32 ov73_021E6778(void *arg0, s32 arg1) {
    s32 v0;

    if (sub_0203769C() == 0) {
        v0 = 0;
        sub_02037030(0x75, &v0, 1);
    }
    *(s32 *)((u8 *)arg0 + 0x318) = 3;
    ov73_021E66F0(arg0);
    return arg1;
}
