#include "global.h"

extern u8 ov96_0221AAE8;

s32 ov96_021E679C(s32 arg0, s32 arg1);

s32 ov96_021E679C(s32 arg0, s32 arg1) {
    return arg1 + *(u16 *)(&ov96_0221AAE8 + (arg0 * 2));
}
