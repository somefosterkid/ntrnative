#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "menu_input_state.h"

void ov18_021F77C0(PokedexAppData *param_1, s32 param_2, u32 param_3, u32 param_4);

void ov18_021F7954(PokedexAppData *param_1, s32 param_2, u32 param_3, u32 param_4);

void ov18_021F7954(PokedexAppData *param_1, s32 param_2, u32 param_3, u32 param_4) {
    ov18_021F77C0(param_1, param_2, param_3, param_4);
    MenuInputStateMgr_SetState(param_1->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
    param_1->unk_189C = param_3;
    return;
}
