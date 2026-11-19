#include "global.h"

typedef unsigned int undefined4;

u8 *sub_0205A4B4(int param_1, void *param_2, int param_3);

u8 *sub_0205A4B4(int param_1, void *param_2, int param_3)

{
    return *(u8 **)(*(u8 **)(param_2 + 0x80) + param_1 * 4 + 0x188);
}

typedef unsigned char undefined1;

void sub_0205A4C4(int param_1, int param_2, void *param_3, void *param_4);

void sub_0205A4C4(int param_1, int param_2, void *param_3, void *param_4)

{
    *(undefined1 *)(*(u8 **)(param_4 + 0x80) + param_1 + 0x176) = *(undefined1 *)param_3;
    return;
}
