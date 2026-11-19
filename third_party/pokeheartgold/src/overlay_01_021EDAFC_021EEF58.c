#include "global.h"

#include "bg_window.h"
#include "overlay_01.h"
#include "overlay_01_field_menu_manager.h"
#include "overlay_03.h"
#include "render_window.h"

ListMenuItem *ov01_021EEF58(FieldMenuManager *menu) {
    return menu->listMenuChoicesStrings;
}

u8 ov01_021EEF60(FieldMenuManager *menu) {
    return menu->optionsCount;
}

Window *ov01_021EEF68(FieldSystem *fieldSystem, u16 a1) {
    Window *window;

    window = AllocWindows(HEAP_ID_FIELD1, 1);
    ov03_02256730(fieldSystem, window, (u32)a1);
    return window;
}

void ov01_021EEF88(Window *window) {
    ClearWindowPixelBuffer(window, 0);
    WindowArray_Delete(window, 1);
}
