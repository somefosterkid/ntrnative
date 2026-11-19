#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "menu_input_state.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "system.h"

void ov18_021F6E58();

void ov18_021F6E98(void);

void ov18_021F6E98(void) {
    ov18_021F6E58();
    PlaySE(0x8E8U);
}

void ov18_021F6EAC(void **arg0);

void ov18_021F6EAC(void **arg0) {
    ov18_021F6E58();
    MenuInputStateMgr_SetState(*(MenuInputStateMgr **)((u8 *)(*arg0) + 0xC), MENU_INPUT_STATE_TOUCH);
}

s32 ov18_021F6F38(PokedexAppData *, s32);

extern GridCallbacks ov18_021FB668;

extern TouchscreenHitbox ov18_021FB828;

extern DpadMenuBox ov18_021FB968;

void ov18_021F6EC0(PokedexAppData *pokedexApp) {
    pokedexApp->unk_1864 = GridInputHandler_Create(&ov18_021FB828, &ov18_021FB968, &ov18_021FB668, pokedexApp, 1, 0U, HEAP_ID_POKEDEX_APP);
    ov18_021F11C0(pokedexApp, 0, 1);
    ov18_021F6F38(pokedexApp, 0);
}

u32 ov18_021F6F08(PokedexAppData *pokedexApp) {
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (8 & gSystem.newKeys) {
        return 6U;
    }
    return (u32)GridInputHandler_HandleInput_AllowHold(pokedexApp->unk_1864);
}
