#include "global.h"

void ov112_021F3140(void *arg0, void *arg1);

void ov112_021F3140(void *arg0, void *arg1) {
    s32 var_r5;
    int new_var2;
    int new_var;
    void *var_r0;
    var_r0 = arg0;
    new_var2 = 0x19;
    var_r5 = 0;
    do {
        if ((*((u8 *)(((u8 *)var_r0) + 0x84))) == 0x1D) {
            *((u16 *)(((u8 *)var_r0) + 0xC)) = (u16)(*((u16 *)arg1));
            *((u8 *)(((u8 *)var_r0) + 0x86)) = (u8)(((*((u8 *)(((u8 *)var_r0) + 0x86))) & (~0x1F)) | (0x1F & (((u32)((*((u8 *)(((u8 *)arg1) + 2))) << 0x1B)) >> 0x1B)));
            new_var = ((u32)((*((u8 *)(((u8 *)arg1) + 2))) << new_var2)) >> 0x1E;
            *((u8 *)(((u8 *)var_r0) + 0x86)) = (u8)((((u32)(new_var << 0x1E)) >> new_var2) | ((*((u8 *)(((u8 *)var_r0) + 0x86))) & (~0x60)));
            *((u8 *)(((u8 *)var_r0) + 0x86)) = (u8)(((*((u8 *)(((u8 *)var_r0) + 0x86))) & (~0x80)) | ((u8)((((u32)((*((u8 *)(((u8 *)arg1) + 2))) << 0x18)) >> 0x1F) << 7)));
        }
        var_r5 += 1;
        var_r0 += 0x88;
    } while (var_r5 < 0x18);
}
