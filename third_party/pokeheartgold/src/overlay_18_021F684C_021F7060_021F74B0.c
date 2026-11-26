#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "menu_input_state.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"

void ov18_021F7444(PokedexAppData *pokedexApp, s32 target);

void ov18_021F74B0(PokedexAppData *pokedexApp, s32 target);

void ov18_021F74B0(PokedexAppData *pokedexApp, s32 target) {
    ov18_021F7444(pokedexApp, target);
    PlaySE(0x8E8U);
}

void ov18_021F74C4(PokedexAppData *arg0, s32 target);

void ov18_021F74C4(PokedexAppData *arg0, s32 target) {
    ov18_021F7444(arg0, target);
    MenuInputStateMgr_SetState(arg0->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
}

s32 ov18_021F75C8(PokedexAppData *, s32);

extern GridCallbacks ov18_021FB678;

extern TouchscreenHitbox ov18_021FB6DC;

extern DpadMenuBox ov18_021FB7A0;

void ov18_021F74D8(PokedexAppData *pokedexApp) {
    pokedexApp->unk_1864 = GridInputHandler_Create(&ov18_021FB6DC, &ov18_021FB7A0, &ov18_021FB678, pokedexApp, 1, 0U, HEAP_ID_POKEDEX_APP);
    ov18_021F11C0(pokedexApp, 0, 1);
    ov18_021F75C8(pokedexApp, 0);
}
