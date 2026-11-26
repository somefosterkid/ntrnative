#include "global.h"

#include "bg_window.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 sub_0200EA68(Window *, s32, u8, s8, u8, u8 arg5);
#else
s32 sub_0200EA68(Window *, s32, s32, s32, s32, s32 arg5);
#endif

void sub_0200EB68(Window *window, s32 a1);

void sub_0200EB68(Window *window, s32 a1) {
    short new_var3;
    unsigned int new_var4;
    int new_var5;
    short new_var;
    unsigned char new_var2;
    new_var5 = 0;
    new_var3 = 3;
    new_var2 = new_var3;
    sub_0200EA68(window, 0x16, a1, new_var3, new_var2, 0);
    new_var4 = (new_var = new_var5);
    new_var3 = new_var4;
}
