#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "menu_input_state.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"

s32 ov18_021F7444();

void ov18_021F74B0(void);

void ov18_021F74B0(void) {
    ov18_021F7444();
    PlaySE(0x8E8U);
}

void ov18_021F74C4(void **arg0);

void ov18_021F74C4(void **arg0) {
    ov18_021F7444();
    MenuInputStateMgr_SetState(*(MenuInputStateMgr **)((u8 *)(*arg0) + 0xC), MENU_INPUT_STATE_TOUCH);
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
