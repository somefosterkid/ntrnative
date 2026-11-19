#include "global.h"
#include "comm_tool.h"


void sub_02037A98(u32 param_1, u32 param_2, u8 *param_3);

void sub_02037A98(u32 param_1, u32 param_2, u8 *param_3) {
    _021D414C->syncNo[*param_3] = param_3[1];
    return;
}

void sub_02037AAC(u32 param_1, u32 param_2, u8 *param_3);

void sub_02037AAC(u32 param_1, u32 param_2, u8 *param_3) {
    _021D414C->syncNoPersonal = *param_3;
    return;
}
