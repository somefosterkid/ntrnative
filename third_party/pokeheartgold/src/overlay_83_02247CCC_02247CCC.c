#include "global.h"

#include "touchscreen_list_menu.h"

void ov83_02247CCC(TouchscreenListMenuSpawner *param_1, TouchscreenListMenuHeader *param_2, u8 param_3, u8 param_4, u8 param_5);

void ov83_02247CCC(TouchscreenListMenuSpawner *param_1, TouchscreenListMenuHeader *param_2, u8 param_3, u8 param_4, u8 param_5) {
    TouchscreenListMenu_Create(param_1, param_2, 0, param_3, param_4, param_5, 0);
    return;
}
