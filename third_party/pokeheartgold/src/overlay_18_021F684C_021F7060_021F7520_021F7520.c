#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "menu_input_state.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02019BA4.h"

s32 ov18_021F75C8(PokedexAppData *, u32);

extern TouchscreenHitbox ov18_021FB718;

u32 ov18_021F7520(PokedexAppData *pokedexApp) {
    s32 v0;
    s32 v1;
    u32 nextInput;

    v1 = GridInputHandler_GetNextInput(pokedexApp->unk_1864);
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (8 & gSystem.newKeys) {
        return 2U;
    }
    if (v1 == 0) {
        if (0x20 & gSystem.newAndRepeatedKeys) {
            return 5U;
        }
        if (0x10 & gSystem.newAndRepeatedKeys) {
            return 4U;
        }
        goto block_7;
    }
block_7:
    if (v1 == 1) {
        if (0x20 & gSystem.newAndRepeatedKeys) {
            return 7U;
        }
        if (0x10 & gSystem.newAndRepeatedKeys) {
            return 6U;
        }
        goto block_12;
    }
block_12:
    v0 = TouchscreenHitbox_FindRectAtTouchHeld(&ov18_021FB718);
    if (v0 != -1) {
        MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
        nextInput = (u32)v0 >> 1;
        GridInputHandler_SetNextInput(pokedexApp->unk_1864, (s32)(u8)nextInput);
        ov18_021F75C8(pokedexApp, nextInput);
        return v0 + 4;
    }
    return (u32)GridInputHandler_HandleInput_AllowHold(pokedexApp->unk_1864);
}
