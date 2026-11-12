// Black's code at main 0x0200a870-0x0200a8a0. The name is pokebw2's file between musical_savedata.c and
// trainer_log_savedata.c (7 functions there, 4 here).

#include "types.h"
#include "gfl/std.h"

void func_0200a870(void *param_1, void *param_2) {
    MI_CpuCopy8(param_2, param_1, 0x1e8);
}

void func_0200a884(void *param_1, void *param_2) {
    MI_CpuCopy8(param_1, param_2, 0x1e8);
}

// The save block's parts after its first 0x1e8 bytes (game_data.c's func_020127e0, func_020127f0: the city's
// resident info)
void *func_0200a890(void *param_1) {
    return (u8 *)param_1 + 0x1e8;
}

void *func_0200a898(void *param_1) {
    return (u8 *)param_1 + 0x1fc;
}
