// Black's code at main 0x0200e874-0x0200e8b0. The name is pokebw2's file between traded_pokemon_savedata.c and
// pokedex_skin_save.c (4 functions there, 4 here).

#include "types.h"
#include "field/unity_tower.h"
#include "gfl/std.h"
#include "save/save_control.h"
#include "struct_decls.h"

u32 func_0200e874(void) {
    return 0x10;
}

void UnityTowerSave_Init(UnityTowerFloor *save) {
    MI_CpuFill8(save, 0, 0x10);
}

void func_0200e884(void *param_1, SaveControl *param_2) {
    void *dest;

    dest = SaveControl_GetBlockPtr(param_2, 0x40);
    MIi_CpuCopy32(param_1, dest, 0x10);
}

void func_0200e89c(SaveControl *param_1, void *param_2) {
    void *src;

    src = SaveControl_GetBlockPtr(param_1, 0x40);
    MIi_CpuCopy32(src, param_2, 0x10);
}
