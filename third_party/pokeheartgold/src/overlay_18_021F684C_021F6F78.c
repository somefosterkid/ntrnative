#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "menu_input_state.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"

s32 ov18_021F6F38();

void ov18_021F6F78(void);

void ov18_021F6F78(void) {
    ov18_021F6F38();
    PlaySE(0x8E8U);
}

void ov18_021F6F8C(void **arg0);

void ov18_021F6F8C(void **arg0) {
    ov18_021F6F38();
    MenuInputStateMgr_SetState(*(MenuInputStateMgr **)((u8 *)(*arg0) + 0xC), MENU_INPUT_STATE_TOUCH);
}

s32 ov18_021F7018(PokedexAppData *, s32);

extern GridCallbacks ov18_021FB6A8;

extern TouchscreenHitbox ov18_021FBA94;

extern DpadMenuBox ov18_021FBC34;

void ov18_021F6FA0(PokedexAppData *pokedexApp) {
    pokedexApp->unk_1864 = GridInputHandler_Create(&ov18_021FBA94, &ov18_021FBC34, &ov18_021FB6A8, pokedexApp, 1, 0U, HEAP_ID_POKEDEX_APP);
    ov18_021F11C0(pokedexApp, 0, 1);
    ov18_021F7018(pokedexApp, 0);
}
