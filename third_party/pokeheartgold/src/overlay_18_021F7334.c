#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "menu_input_state.h"
#include "unk_02019BA4.h"

void ov18_021F719C(PokedexAppData *param_1, s32 param_2, u32 param_3, u32 param_4);

void ov18_021F7334(PokedexAppData *param_1, s32 param_2, u32 param_3, u32 param_4);

s32 ov18_021F7444(PokedexAppData *, s32);
extern GridCallbacks ov18_021FB648;
extern TouchscreenHitbox ov18_021FB6C8;
extern DpadMenuBox ov18_021FB780;

void ov18_021F7334(PokedexAppData *param_1, s32 param_2, u32 param_3, u32 param_4) {
    ov18_021F719C(param_1, param_2, param_3, param_4);
    MenuInputStateMgr_SetState(param_1->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
    param_1->unk_189C = param_3;
    return;
}

void ov18_021F7354(PokedexAppData *pokedexApp) {
    pokedexApp->unk_1864 = GridInputHandler_Create(&ov18_021FB6C8, &ov18_021FB780, &ov18_021FB648, pokedexApp, 1, 0U, HEAP_ID_POKEDEX_APP);
    ov18_021F11C0(pokedexApp, 0, 1);
    ov18_021F7444(pokedexApp, 0);
}
