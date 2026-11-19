#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "menu_input_state.h"

void ov59_02237F28(Ov59ApricornApp *arg0);

void ov59_02237F28(Ov59ApricornApp *arg0) {
    MenuInputStateMgr_SetState(arg0->parent->args->menuInputStatePtr, arg0->menuInputState);
}
