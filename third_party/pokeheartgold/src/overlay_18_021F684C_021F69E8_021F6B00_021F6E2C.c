#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "menu_input_state.h"
#include "sprite_system.h"
#include "system.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"

u32 ov18_021F6E2C(PokedexAppData *pokedexApp) {
    s32 v0;

    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    v0 = 8;
    v0 = (!(gSystem.newKeys & 8)) ? GridInputHandler_HandleInput_AllowHold(pokedexApp->unk_1864) : v0;
    return (u32)v0;
}

extern u8 ov18_021FBD1C;

void ov18_021F6E58(PokedexAppData *arg0, s32 arg1);

void ov18_021F6E58(PokedexAppData *arg0, s32 target) {
    u8 py[2];
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(arg0->unk_1864, target), &py[1], &py[0]);
    ManagedSprite_SetPositionXY(arg0->unk_0670[0], py[1], (s16)(py[0]));
    ov18_021F118C(arg0, 0, (s32)(*((u8 *)((&ov18_021FBD1C) + target))));
}
