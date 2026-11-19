#include "global.h"

#include "menu_input_state.h"
#include "application/pokedex/pokedex_internal.h"
#include "unk_02019BA4.h"
#include "system.h"

void ov18_021F7018(PokedexAppData *pokedexApp, s32 target);

void ov18_021F7104(PokedexAppData *pokedexApp, int newTarget, int prevTarget);

s32 ov18_021F719C(PokedexAppData *, s32);
extern GridCallbacks ov18_021FB638;
extern TouchscreenHitbox ov18_021FBA40;
extern DpadMenuBox ov18_021FBB94;

void ov18_021F7104(PokedexAppData *pokedexApp, int newTarget, int prevTarget) {
    ov18_021F7018(pokedexApp, newTarget);
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
    pokedexApp->unk_189C = prevTarget;
}

void ov18_021F7124(PokedexAppData *pokedexApp) {
    pokedexApp->unk_1864 = GridInputHandler_Create(&ov18_021FBA40, &ov18_021FBB94, &ov18_021FB638, pokedexApp, 1, 0U, HEAP_ID_POKEDEX_APP);
    ov18_021F11C0(pokedexApp, 0, 1);
    ov18_021F719C(pokedexApp, 0);
}

u32 ov18_021F716C(PokedexAppData *pokedexApp) {
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (8 & gSystem.newKeys) {
        return 0x12U;
    }
    return (u32)GridInputHandler_HandleInput_AllowHold(pokedexApp->unk_1864);
}
