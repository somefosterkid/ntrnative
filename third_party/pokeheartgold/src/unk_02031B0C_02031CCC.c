#include "global.h"

#include "unk_02031B0C.h"


u8 sub_02031CE8(SaveApricornBox *arg0);

void sub_02031CCC(SaveApricornBox *arg0, int arg1) {
    s32 v0;
    int new_var;
    new_var = 0xA * arg1;
    if ((*((u8 *)(((u8 *)arg0) + 9))) != 0) {
        v0 = (*((u8 *)(((u8 *)arg0) + 0xE))) + new_var;
        if (v0 > 0xFF) {
            *((u8 *)(((u8 *)arg0) + 0xE)) = 0xFFU;
            return;
        }
        *((u8 *)(((u8 *)arg0) + 0xE)) = (u8)v0;
    }
}

u8 sub_02031CE8(SaveApricornBox *arg0) {
    return arg0->juiceServings;
}
