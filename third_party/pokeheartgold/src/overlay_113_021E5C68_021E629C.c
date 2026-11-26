#include "global.h"

#include "bg_window.h"
#include "pm_string.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov113_021E6274(Window *, String *, s32, s32, u16);
#else
u8 ov113_021E6274(Window *, String *, s32, s32, s32);
#endif

void ov113_021E629C(Window *arg0, String *arg1, s32 arg2, s32 arg3);

void ov113_021E629C(Window *arg0, String *arg1, s32 arg2, s32 arg3) {
    ov113_021E6274(arg0, arg1, 0, arg2, arg3);
}
