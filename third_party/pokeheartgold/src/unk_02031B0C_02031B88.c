#include "global.h"

#include "error_handling.h"
#include "unk_02031B0C.h"

u8 ApricornBox_TakeApricorn(u8 *arg0, s32 arg1, u32 arg2);

u8 ApricornBox_TakeApricorn(u8 *arg0, s32 arg1, u32 arg2) {
    u8 v0;

    if (arg1 >= 7) {
        GF_AssertFail();
        return 0U;
    }
    v0 = *(u8 *)(arg0 + arg1);
    if ((u32)v0 >= arg2) {
        *(u8 *)(arg0 + arg1) = v0 - arg2;
    }
    return *(u8 *)(arg0 + arg1);
}

int ApricornBox_CountApricorn(SaveApricornBox *apricornBox, u32 a1) {
    u8 *new_var;
    unsigned int new_var2;
    if ((6 + 1) <= ((s32)a1)) {
        GF_AssertFail();
        return 0;
    }
    new_var = (u8 *)apricornBox;
    new_var2 = new_var[a1];
    return (u32)((u8)new_var2);
}
