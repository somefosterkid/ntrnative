#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "list_menu_items.h"
#include "touchscreen_list_menu.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_02238D38(TouchscreenListMenu *menu, s32 cursorPos, void *callbackArg, s32 event);

s32 ov70_02238FB4(s32);

extern const TouchscreenListMenuTemplate _022451EC;

TouchscreenListMenu *ov70_02238CAC(GTSApplicationState *arg0, u8 arg1, u8 arg2);

TouchscreenListMenu *ov70_02238CAC(GTSApplicationState *arg0, u8 arg1, u8 arg2) {
    TouchscreenListMenuHeader sp18;
    TouchscreenListMenu *temp_r4;

    arg0->unk_11CC = TouchscreenListMenuSpawner_Create(HEAP_ID_61, NULL);
    MI_CpuFill8(&sp18, 0U, sizeof(TouchscreenListMenuHeader));
    sp18.template = _022451EC;
    sp18.listMenuItems = arg0->unk_11AC;
    sp18.bgConfig = arg0->bgConfig;
    sp18.numWindows = arg1;
    temp_r4 = (struct TouchscreenListMenu *)((void (*)(struct TouchscreenListMenu *, unsigned char, void *, int))(TouchscreenListMenu_CreateWithCallback(arg0->unk_11CC, &sp18, 0U, 0x14U, (u8)(s32)arg2, 0xAU, 0U, (TouchscreenListMenuCallback)(ov70_02238D38), NULL, 1)));
    ov70_02238FB4(1);
    return temp_r4;
}
