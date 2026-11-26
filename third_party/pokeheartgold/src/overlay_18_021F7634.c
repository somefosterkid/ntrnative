#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "menu_input_state.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "system.h"

void ov18_021F75C8(PokedexAppData *arg0, s32 arg1);

void ov18_021F7634(PokedexAppData *arg0, s32 arg1);

void ov18_021F7634(PokedexAppData *arg0, s32 arg1) {
    ov18_021F75C8(arg0, arg1);
    PlaySE(0x8E8U);
}

void ov18_021F7648(PokedexAppData *arg0, s32 arg1);

void ov18_021F7648(PokedexAppData *arg0, s32 arg1) {
    ov18_021F75C8(arg0, arg1);
    MenuInputStateMgr_SetState(arg0->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
}

u32 ov18_021F765C(u32 *a1) {
    u32 y;

    return (u32)System_GetTouchHeldCoords(a1, &y);
}

s32 ov18_021F76E0(PokedexAppData *, s32);

extern GridCallbacks ov18_021FB6B8;

extern TouchscreenHitbox ov18_021FB744;

extern DpadMenuBox ov18_021FB8D4;

void ov18_021F7668(PokedexAppData *pokedexApp) {
    pokedexApp->unk_1864 = GridInputHandler_Create(&ov18_021FB744, &ov18_021FB8D4, &ov18_021FB6B8, pokedexApp, 1, 0U, HEAP_ID_POKEDEX_APP);
    ov18_021F11C0(pokedexApp, 0, 1);
    ov18_021F76E0(pokedexApp, 0);
}

u32 ov18_021F76B0(PokedexAppData *pokedexApp) {
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (8 & gSystem.newKeys) {
        return 4U;
    }
    return (u32)GridInputHandler_HandleInput_AllowHold(pokedexApp->unk_1864);
}
