#include "global.h"

s32 ov112_021E7DC0(u8 *);

s32 ov112_021E7DFC(void *, void *);

s32 ov112_021E8CB8(u8 *, u8 *);

void ov112_021E7D7C(u8 *arg0);

void ov112_021E7D7C(u8 *arg0) {
    ov112_021E7DFC(arg0, arg0 + 0x10F4);
    ov112_021E8CB8(arg0 + 0xC910, arg0);
    ov112_021E7DC0(arg0);
}

void ov112_021E7DA4(u8 *arg0);

void ov112_021E7DA4(u8 *arg0) {
    ov112_021E8CB8(arg0 + 0xC910, arg0);
    ov112_021E7DC0(arg0);
}
