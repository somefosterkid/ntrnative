#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "touchscreen_list_menu.h"

s32 ov59_02238F24(Ov59ApricornApp *arg0);

s32 ov59_02238F24(Ov59ApricornApp *arg0) {
    s32 temp_r0;
    s32 var_r0;

    temp_r0 = TouchscreenListMenu_HandleInput(arg0->touchMenu);
    var_r0 = -1;
    if (temp_r0 != -1) {
        arg0->menuInputState = (enum MenuInputState)TouchscreenListMenu_WasLastInputTouch(arg0->touchMenu);
        TouchscreenListMenu_Destroy(arg0->touchMenu);
        var_r0 = temp_r0;
    }
    return var_r0;
}
