#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "touchscreen_list_menu.h"

extern const TouchscreenListMenuTemplate ov59_0223C630[];

extern const Ov59MenuDef ov59_0223C668[];

void ov59_02238E98(Ov59ApricornApp *work);

void ov59_02238E98(Ov59ApricornApp *work) {
    TouchscreenListMenuHeader header;

    MI_CpuFill8(&header, 0, sizeof(TouchscreenListMenuHeader));
    header.template = ov59_0223C630[1];
    header.listMenuItems = work->listMenuItems;
    header.bgConfig = work->bgConfig;
    header.numWindows = ov59_0223C668[work->mode - 1].count;
    work->touchMenu = TouchscreenListMenu_Create(work->menuSpawner, &header, work->menuInputState, ov59_0223C668[work->mode - 1].x, ov59_0223C668[work->mode - 1].y, 8, 0);
}
