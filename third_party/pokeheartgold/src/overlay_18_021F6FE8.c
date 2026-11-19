#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "menu_input_state.h"
#include "system.h"
#include "unk_02019BA4.h"
#include "sprite_system.h"
#include "unk_02020A0C.h"


void ov18_021F7018(PokedexAppData *arg0, s32 arg1);

u32 ov18_021F6FE8(PokedexAppData *pokedexApp) {
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (8 & gSystem.newKeys) {
        return 0x1BU;
    }
    return (u32)GridInputHandler_HandleInput_AllowHold(pokedexApp->unk_1864);
}

void ov18_021F7018(PokedexAppData *pokedexApp, s32 target) {
    u8 py[2];

    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(pokedexApp->unk_1864, target), &py[1], &py[0]);
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[0], (s16) py[1], (s16) py[0]);
    if (target >= 0x1B) {
        ov18_021F118C(pokedexApp, 0, 0x23);
        return;
    }
    ov18_021F118C(pokedexApp, 0, 0x28);
}
