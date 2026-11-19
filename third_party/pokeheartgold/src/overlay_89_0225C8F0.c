#include "global.h"
#include "overlay_89.h"

u32 ov45_0222EC7C(s32 param0);

typedef struct {
    BOOL unk_00;
    s32 unk_04;
} UnkStruct_ov113_022607EC;

BOOL ov89_0225C8F0(s32 param0, UnkStruct_ov113_022607EC *param1);

s32 ov45_0222D844(void);
s32 ov45_0222EFAC(s32, s32, s32);

BOOL ov89_0225C8F0(s32 param0, UnkStruct_ov113_022607EC *param1) {
    if (param1->unk_04 == param0) {
        return 0;
    }

    if (param1->unk_00 == 0) {
        return 0;
    }

    if (ov45_0222EC7C(param0) == 0xffffffff) {
        return 0;
    }

    return 1;
}

s32 ov89_0225C91C(s32 arg0) {
    if (ov45_0222D844() == 1) {
        return 0;
    }
    ov45_0222EFAC(2, arg0, 0xC);
    return 1;
}
