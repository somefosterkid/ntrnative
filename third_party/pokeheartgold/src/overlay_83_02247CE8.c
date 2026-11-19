#include "global.h"

#include "sys_task.h"
#include "touchscreen_list_menu.h"

void TouchscreenListMenu_Destroy(TouchscreenListMenu *menu);

void ov83_02247CE8(TouchscreenListMenu *menu);

void ov83_02247CE8(TouchscreenListMenu *menu) {
    TouchscreenListMenu_Destroy(menu);
}
