#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "menu_input_state.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "system.h"

void ov18_021F76E0(PokedexAppData *arg0, s32 arg1);

void ov18_021F7720(PokedexAppData *arg0, s32 arg1);

void ov18_021F7720(PokedexAppData *arg0, s32 arg1) {
    ov18_021F76E0(arg0, arg1);
    PlaySE(0x8E8U);
}

void ov18_021F7734(PokedexAppData *arg0, s32 arg1);

void ov18_021F7734(PokedexAppData *arg0, s32 arg1) {
    ov18_021F76E0(arg0, arg1);
    MenuInputStateMgr_SetState(arg0->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
}

s32 ov18_021F77C0(PokedexAppData *, s32);

extern GridCallbacks ov18_021FB658;

extern TouchscreenHitbox ov18_021FB9A8;

extern DpadMenuBox ov18_021FBB0C;

void ov18_021F7748(PokedexAppData *pokedexApp) {
    pokedexApp->unk_1864 = GridInputHandler_Create(&ov18_021FB9A8, &ov18_021FBB0C, &ov18_021FB658, pokedexApp, 1, 0U, HEAP_ID_POKEDEX_APP);
    ov18_021F11C0(pokedexApp, 0, 1);
    ov18_021F77C0(pokedexApp, 0);
}

u32 ov18_021F7790(PokedexAppData *pokedexApp) {
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (8 & gSystem.newKeys) {
        return 0xFU;
    }
    return (u32)GridInputHandler_HandleInput_AllowHold(pokedexApp->unk_1864);
}
