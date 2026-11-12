// Black's code at main 0x0200e8e8-0x0200e904. The name is pokebw2's file between pokedex_skin_save.c and
// livematch_savedata.c (3 functions there, 3 here).

#include "types.h"
#include "gfl/std.h"
#include "save/save_control.h"
#include "save/trial_house.h"
#include "struct_decls.h"

u32 func_0200e8e8(void) {
    return 0x5c;
}

void func_0200e8ec(void *param_1) {
    MI_CpuFill8(param_1, 0, 0x5c);
}

TrialHouseSave *func_0200e8f8(SaveControl *save) {
    TrialHouseSave *pTVar1;

    pTVar1 = SaveControl_GetBlockPtr(save, 0x41);
    return pTVar1;
}
