#include "global.h"

#include "network_icon.h"
#include "unk_02037C94.h"
#include "unk_0203A3B0.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: pointer-sized copy of the asm's .bss slot (asm/unk_0203A3B0.s: 4 bytes on the DS). The asm label stays for the DS build.
NetworkIcon *_021D4154;
#endif

void sub_0203A700(NetworkIcon *param_1);

void sub_0203A86C(void);

void sub_0203A86C(void) {
    if (_021D4154 != 0) {
        sub_0203A700(_021D4154);
    }
    return;
}

static void sub_0203A8CC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void sub_0203A880(void) {
    s32 v0;

    v0 = 0;
    if (sub_02039998() != 0) {
        v0 = 1;
    }
    sub_0203A8CC(0xF0, 0, v0, 1, 0x1C0);
}

void sub_0203A8A8(s32 arg0);

void sub_0203A8A8(s32 arg0) {
    s32 v0;

    v0 = 0;
    if (sub_02039998() != 0) {
        v0 = 1;
    }
    sub_0203A8CC(0xF0, 0, v0, arg0, 0x1E0);
}

s32 sub_02034DCC(void);

#if defined(SDK_PORT) || defined(NONMATCHING)
NetworkIcon *sub_0203A3B0(s32, s32, u16, u16, s32, s32 *, s32, s32);
#else
NetworkIcon *sub_0203A3B0(s32, s32, s32, s32, s32, s32 *, s32, s32);
#endif

extern s32 _0210F91C;

static void sub_0203A8CC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    if (sub_02034DCC() != 0) {
        if (_021D4154 != 0) {
            sub_0203A914();
        }
        _021D4154 = sub_0203A3B0(0, 0x59, arg0, arg1, arg2, &_0210F91C, arg3, arg4);
    }
}

void sub_0203A444(NetworkIcon *icon);

void sub_0203A914(void) {
    if (_021D4154 != 0) {
        sub_0203A444(_021D4154);
        _021D4154 = 0;
    }
    return;
}
