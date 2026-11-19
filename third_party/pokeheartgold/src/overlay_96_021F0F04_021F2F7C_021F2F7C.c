#include "global.h"

s32 ov96_021EB06C(void *, s32, s32, s32 *, s32 *);

s32 ov96_021F2F7C(void *arg0, s32 arg1, s32 arg2);

s32 ov96_021F2F7C(void *arg0, s32 arg1, s32 arg2) {
    s32 sp8;
    s32 sp4;

    ov96_021EB06C(arg0, (arg1 / 0x1000), (arg2 / 0x1000), &sp8, &sp4);
    if ((sp8 < 0x10) || (sp8 >= 0xF0) || (sp4 < 0x10) || (sp4 >= 0xB0)) {
        return 1;
    }
    return 0;
}
