#include "global.h"

#include "touchscreen_list_menu.h"
#include "overlay_65_trade_room.h"

s32 ov65_0221F890(void);

extern const TouchscreenListMenuTemplate ov65_0221FD3C;

TouchscreenListMenu *ov65_0221E928(TradeRoom *arg0, int arg1, int arg2);

TouchscreenListMenu *ov65_0221E928(TradeRoom *arg0, int arg1, int arg2) {
    TouchscreenListMenu *temp_r4;
    TouchscreenListMenuHeader spC;

    arg0->touchListMenuSpawner = TouchscreenListMenuSpawner_Create(HEAP_ID_26, NULL);
    MI_CpuFill8(&spC, 0U, sizeof(TouchscreenListMenuHeader));
    spC.template = ov65_0221FD3C;
    spC.listMenuItems = arg0->actionMenuChoices;
    spC.bgConfig = arg0->bgConfig;
    spC.numWindows = arg1;
    temp_r4 = TouchscreenListMenu_Create(arg0->touchListMenuSpawner, &spC, 0U, 0x11U, arg2, 0xDU, 0U);
    ov65_0221F890();
    return temp_r4;
}
