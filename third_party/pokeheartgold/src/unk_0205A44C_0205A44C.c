#include "global.h"

#include "unk_02037C94.h"

typedef unsigned int undefined4;

typedef unsigned char byte;

typedef unsigned int uint;

void sub_0205A44C(int param_1, int param_2, void *param_3, void *param_4);

void sub_0205A44C(int param_1, int param_2, void *param_3, void *param_4)

{
    *(undefined4 *)(*(u8 **)(param_4 + 0x80) + 0x2c) = 1;
    *(uint *)(*(u8 **)(param_4 + 0x80) + 0x40) = (uint) * (byte *)param_3;
    if (*(byte *)param_3 == 4) {
        sub_020388E0();
    }
    return;
}
